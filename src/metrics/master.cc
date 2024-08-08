#include "metrics/master.h"
#include "metrics/utils.h"

#ifdef HAVE_PROMETHEUS
using namespace metrics::master;

Master::Master(std::shared_ptr<prometheus::Registry>& registry) {
		// NOLINTBEGIN(cppcoreguidelines-prefer-member-initializer)
		packet_client_counter = &setup_family(
		    "metadata_observed_packets_client_total",
		    "Number of observed packets from and for client", registry);
		byte_client_counter = &setup_family(
		    "metadata_observed_bytes_client_total",
		    "Number of observed bytes from and for client", registry);
		filesystem_counter = &setup_family(
		    "metadata_stats_total",
		    "Number of observed filesystem operations", registry);
		chunk_counter= &setup_family(
		    "metadata_chunk_operations_total",
		    "Number of chunk operations", registry);
		// NOLINTEND(cppcoreguidelines-prefer-member-initializer)

		// clang-format off
		// A very hacky way to allow compile time checking if metrics have been
		// set. Any enum value not used will throw a compile time error.
		master::Counters start = KEY_START;
		switch (start) {
        case KEY_START:
			[[fallthrough]];
		case CHUNK_DELETE:
			master_counters[CHUNK_DELETE] =
			    Counter(
				{{"chunk", "operations"}, {"operation", "delete"}},
			    chunk_counter);
			[[fallthrough]];
		case CHUNK_REPLICATE:
			master_counters[CHUNK_REPLICATE] =
			    Counter(
				{{"chunk", "operations"}, {"operation", "replicate"}},
			    chunk_counter);
			[[fallthrough]];
		case FS_STATFS:
			master_counters[FS_STATFS] =
				Counter(
			    {{"filesystem", "operations"}, {"operation", "STATFS"}},
			    filesystem_counter);
			[[fallthrough]];
		case FS_GETATTR:
			master_counters[FS_GETATTR] =
				Counter(
			    {{"filesystem", "operations"}, {"operation", "GETATTR"}},
			    filesystem_counter);
			[[fallthrough]];
		case FS_SETATTR:
			master_counters[FS_SETATTR] =
				Counter(
			    {{"filesystem", "operations"}, {"operation", "SETATTR"}},
			    filesystem_counter);
			[[fallthrough]];
		case FS_LOOKUP:
			master_counters[FS_LOOKUP] =
			    Counter(
				{{"filesystem", "operations"}, {"operation", "LOOKUP"}},
			    filesystem_counter);
			[[fallthrough]];
		case FS_MKDIR:
			master_counters[FS_MKDIR] =
			    Counter(
				{{"filesystem", "operations"}, {"operation", "MKDIR"}},
			    filesystem_counter);
			[[fallthrough]];
		case FS_RMDIR:
			master_counters[FS_RMDIR] =
			    Counter(
				{{"filesystem", "operations"}, {"operation", "RMDIR"}},
			    filesystem_counter);
			[[fallthrough]];
		case FS_SYMLINK:
			master_counters[FS_SYMLINK] =
				Counter(
			    {{"filesystem", "operations"}, {"operation", "SYMLINK"}},
			    filesystem_counter);
			[[fallthrough]];
		case FS_READLINK:
			master_counters[FS_READLINK] =
				Counter(
			    {{"filesystem", "operations"}, {"operation", "READLINK"}},
			    filesystem_counter);
			[[fallthrough]];
		case FS_MKNOD:
			master_counters[FS_MKNOD] =
			    Counter(
				{{"filesystem", "operations"}, {"operation", "MKNOD"}},
			    filesystem_counter);
			[[fallthrough]];
		case FS_UNLINK:
			master_counters[FS_UNLINK] =
			    Counter(
				{{"filesystem", "operations"}, {"operation", "UNLINK"}},
			    filesystem_counter);
			[[fallthrough]];
		case FS_RENAME:
			master_counters[FS_RENAME] =
			    Counter(
				{{"filesystem", "operations"}, {"operation", "RENAME"}},
			    filesystem_counter);
			[[fallthrough]];
		case FS_LINK:
			master_counters[FS_LINK] =
			    Counter(
				{{"filesystem", "operations"}, {"operation", "LINK"}},
			    filesystem_counter);
			[[fallthrough]];
		case FS_READDIR:
			master_counters[FS_READDIR] = Counter(
			    {{"filesystem", "operations"}, {"operation", "READDIR"}},
			    filesystem_counter);
			[[fallthrough]];
		case FS_OPEN:
			master_counters[FS_OPEN] =
			    Counter(
				{{"filesystem", "operations"}, {"operation", "OPEN"}},
			    filesystem_counter);
			[[fallthrough]];
		case FS_READ:
			master_counters[FS_READ] =
			    Counter(
				{{"filesystem", "operations"}, {"operation", "READ"}},
			    filesystem_counter);
			[[fallthrough]];
        case FS_WRITE:
			master_counters[FS_WRITE] =
			    Counter(
				{{"filesystem", "operations"}, {"operation", "WRITE"}},
			    filesystem_counter);
			[[fallthrough]];
		case CLIENT_RX_PACKETS:
			master_counters[CLIENT_RX_PACKETS] =
				Counter(
			    {{"protocol", "tcp"}, {"direction", "rx"}, {"type", "packet"}},
				packet_client_counter);
			[[fallthrough]];
		case CLIENT_TX_PACKETS:
			master_counters[CLIENT_TX_PACKETS] =
			    Counter(
				{{"protocol", "tcp"}, {"direction", "tx"}, {"type", "packet"}},
			    packet_client_counter);
			[[fallthrough]];
        case CLIENT_RX_BYTES:
			master_counters[CLIENT_RX_BYTES] =
			    Counter(
				{{"protocol", "tcp"}, {"direction", "rx"}, {"type", "bytes"}},
			    byte_client_counter);
			[[fallthrough]];
        case CLIENT_TX_BYTES:
			master_counters[CLIENT_TX_BYTES] =
			    Counter(
				{{"protocol", "tcp"}, {"direction", "tx"}, {"type", "bytes"}},
			    byte_client_counter);
			[[fallthrough]];
        case KEY_END:
			break;
		}
		// clang-format on
}
#endif
