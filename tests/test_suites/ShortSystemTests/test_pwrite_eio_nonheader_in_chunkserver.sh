# Create an installation with 3 chunkservers, 1 disk each.
# CS 0 has a disk which will fail during the test when writing bigger files.
USE_RAMDISK=YES \
	CHUNKSERVERS=3 \
	CHUNKSERVER_EXTRA_CONFIG="HDD_TEST_FREQ = 10000" \
	CHUNKSERVER_0_DISK_0="$RAMDISK_DIR/pwrite_far_EIO_hdd_0" \
	MOUNT_EXTRA_CONFIG="mfscachemode=NEVER" \
	MASTER_EXTRA_CONFIG="CHUNKS_LOOP_MIN_TIME = 1`
			`|CHUNKS_LOOP_MAX_CPU = 90`
			`|ACCEPTABLE_DIFFERENCE = 1.0`
			`|CHUNKS_WRITE_REP_LIMIT = 20`
			`|OPERATIONS_DELAY_INIT = 0`
			`|OPERATIONS_DELAY_DISCONNECT = 0" \
	setup_local_empty_saunafs info

# Restart the first chunkserver preloading pwrite with EIO-throwing version
LD_PRELOAD="${SAUNAFS_INSTALL_FULL_LIBDIR}/libchunk_operations_eio.so" \
		assert_success saunafs_chunkserver_daemon 0 restart
saunafs_wait_for_all_ready_chunkservers

# Create a directory with many small files on mountpoint. Expect no damaged disks.
cd "${info[mount0]}"
mkdir test
saunafs setgoal 2 test
FILE_SIZE=1234 assert_success file-generate test/small_{1..20}
sleep 1
assert_awk_finds_no '$4 != "no"' "$(saunafs_admin_master_no_password list-disks)"

# Write a couple of bigger files. This should trigger a failure on CS0.
FILE_SIZE=1M assert_success file-generate test/big_{1..10}

# Assert that exactly disks marked "pwrite_far_EIO" are marked as damaged
sleep 1
list=$(saunafs_admin_master_no_password list-disks)
assert_equals 3 "$(wc -l <<< "$list")"
assert_awk_finds_no '(/EIO/ && $4 != "yes") || (!/EIO/ && $4 != "no")' "$list"

# Assert that data is replicated to chunkservers 1, 2 and no chunk is stored on cs 0
for f in test/*; do
	assert_eventually_prints "" "saunafs fileinfo '$f' | grep ':${info[chunkserver0_port]}'"
	assert_eventually_prints 2 "saunafs fileinfo '$f' | grep copy | wc -l"
done
