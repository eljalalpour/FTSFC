
// Shared state
shared_locks::SharedLocks(LOCKS 8);
shared_state::FTSharedStateCounter(SHARED_LOCKS shared_locks, CHAIN 1, ID 0, F 1, SHARING_LEVEL 1);

// FTC Block definition
elementclass FTBlock {
$queue, $src_ip|
    input
    -> MarkIPHeader(14)
    -> IPFilter(allow udp && src 1.1.0.0/16)
    -> PMProcess(QUEUE $queue, SHARED_STATE shared_state)
    -> MarkIPHeader(14)
    -> StoreEtherAddress(f4:52:14:5a:90:70, src) // aqua08
    -> StoreEtherAddress(f4:52:14:69:a5:20, dst) // aqua01
    -> StoreIPAddress($src_ip, src) // aqua08
    -> StoreIPAddress(10.70.0.1, dst) // aqua01
    -> output;
}


// From device declarations
data_fd0::FromDPDKDevice(1, 0);

// To device declarations
data_td0::ToDPDKDevice(1, 0);

// Scheduling
StaticThreadSched(data_fd0 0);

// FTC Block declarations
ftc0::FTBlock(0, 1.1.0.0);

// MB graph
// Queue 0
data_fd0
-> ftc0
-> data_td0;

