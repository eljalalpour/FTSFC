
// Shared state
shared_locks::SharedLocks(LOCKS 8);
shared_state::FTSharedStateCounter(SHARED_LOCKS shared_locks, CHAIN 1, ID 0, F 1, SHARING_LEVEL 1);

// FTC Block definition
elementclass FTBlock {
$queue, $src_ip, $index|
    input
    -> MarkIPHeader(14)
    -> IPFilter(allow udp && src 1.0.0.0/16)
    -> PMConstruct(QUEUE $index, SHARED_STATE shared_state)
    -> MarkIPHeader(14)
    -> StoreEtherAddress(e4:1d:2d:13:9e:d0, src) // aqua07
    -> StoreEtherAddress(f4:52:14:5a:90:70, dst) // aqua08
    -> StoreIPAddress($src_ip, src) // aqua07
    -> StoreIPAddress(10.70.0.8, dst) // aqua08
    -> output;
}

// From device declarations
data_fd0::FromDPDKDevice(1, 0);

// To device declarations
data_td0::ToDPDKDevice(1, 0);

// Scheduling
StaticThreadSched(data_fd0 0);

// FTC Block declarations
ftc0::FTBlock(0, 1.1.0.0, 0);

// MB graph
// Queue 0
data_fd0
-> ftc0
-> data_td0;

