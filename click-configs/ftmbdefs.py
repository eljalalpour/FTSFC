FTMB = """
// Shared locks and state
{SHARED_STATE_DECLARE}

// FTMB Block definition
{FTMB_BLOCK_DEF}

// From device declarations
{FROM_DEVICE_DECLARES}

// To device declarations
{TO_DEVICE_DECLARES}

// Scheduling
{THREAD_SCHEDULES}

// FTMB Block declarations
{FTMB_BLOCKS_DECLARES}

// MB graph
{LINKS}
"""

FTMB_MASTER_BLOCK = """elementclass FTMBBlock {{
$queue, $src_ip{MB_PARAM}|
    input
    -> MarkIPHeader(14)
    -> IPFilter(allow udp && src {SRC_IP_FILTER}/16)
    -> FTMBInputLogger()
    -> {MB}
    -> MarkIPHeader(14)
    -> StoreEtherAddress({DATA_SRC_MAC}, src) // {DATA_SRC_NAME}
    -> StoreEtherAddress({DATA_DST_MAC}, dst) // {DATA_DST_NAME}
    -> StoreIPAddress($src_ip, src) // {DATA_SRC_NAME}
    -> StoreIPAddress({DATA_DST_IP}, dst) // {DATA_DST_NAME}
    -> output;
}}
"""

FTMB_SLAVE_BLOCK = """elementclass FTMBBlock {{
$queue, $src_ip |
    input
    -> MarkIPHeader(14)
    -> IPFilter(allow udp && src {SRC_IP_FILTER}/16)
    -> FTMBOutputLogger()
    -> MarkIPHeader(14)
    -> StoreEtherAddress({DATA_SRC_MAC}, src) // {DATA_SRC_NAME}
    -> StoreEtherAddress({DATA_DST_MAC}, dst) // {DATA_DST_NAME}
    -> StoreIPAddress($src_ip, src) // {DATA_SRC_NAME}
    -> StoreIPAddress({DATA_DST_IP}, dst) // {DATA_DST_NAME}
    -> output;
}}
"""

LINK_FORMAT_STR = """// Queue {QUEUE}
{FROM_DATA_DEVICE_NAME}
-> {FTMB_BLOCK_NAME}
-> {TO_DATA_DEVICE_NAME};
"""
SHARED_STATE_FORMAT_STR = "shared_locks::SharedLocks(LOCKS {LOCKS});" \
                          "\nshared_state::FTMBSharedState" \
                          "(SHARED_LOCKS shared_locks, BATCH {BATCH}, QUEUES {QUEUES},\n" \
                          "\tSRCIP 0.0.0.0, DSTIP {DATA_DST_IP},\n" \
                          "\tSRCETH {DATA_SRC_MAC}, DSTETH {DATA_DST_MAC});"


SHARED_STATE_COUNTER_FORMAT_STR = "shared_locks::SharedLocks(LOCKS {LOCKS});" \
                                  "\nshared_state::FTMBSharedStateCounter" \
                                  "(SHARED_LOCKS shared_locks, BATCH {BATCH}, QUEUES {QUEUES},\n" \
                                  "\tSRCIP 0.0.0.0, DSTIP {DATA_DST_IP},\n" \
                                  "\tSRCETH {DATA_SRC_MAC}, DSTETH {DATA_DST_MAC},\n" \
                                  "\tSHARING_LEVEL {SHARING_LEVEL});"

SHARED_STATE_NAT_FORMAT_STR = "shared_locks::SharedLocks(LOCKS {LOCKS});" \
                              "\nshared_state::FTMBSharedStateAddrRewriter" \
                              "(SHARED_LOCKS shared_locks, BATCH {BATCH}, QUEUES {QUEUES},\n" \
                              "\tSRCIP 0.0.0.0, DSTIP {DATA_DST_IP},\n" \
                              "\tSRCETH {DATA_SRC_MAC}, DSTETH {DATA_DST_MAC});"

FROM_DEVICE_NAME_FORMAT_STR = "fd{QUEUE}"
THREAD_SCHED_FORMAT_STR = "StaticThreadSched({});"
FROM_DEVICE_FORMAT_STR = "fd{QUEUE}::FromDPDKDevice({DEVICE}, {QUEUE});"

TO_DEVICE_NAME_FORMAT_STR = "td{QUEUE}"
TO_DEVICE_FORMAT_STR = "td{QUEUE}::ToDPDKDevice({DEVICE}, {QUEUE});"

FTMB_BLOCK_NAME_FORMAT_STR = "ftmb{}"
FTMB_MASTER_BLOCK_FORMAT_STR = "ftmb{QUEUE}::FTMBBlock({QUEUE}, {DATA_SRC_IP}{MB_PARAMS});"
FTMB_SLAVE_BLOCK_FORMAT_STR = "ftmb{QUEUE}::FTMBBlock({QUEUE}, {DATA_SRC_IP});"

MB = "MB"
MB_PARAM = 'MB_PARAM'

DATA_SRC_NAME = "DATA_SRC_NAME"
DATA_SRC_IP = "DATA_SRC_IP"
DATA_DST_NAME = "DATA_DST_NAME"
DATA_DST_IP = "DATA_DST_IP"
DATA_SRC_MAC = "DATA_SRC_MAC"
DATA_DST_MAC = "DATA_DST_MAC"

STATE_SRC_NAME = "STATE_SRC_NAME"
STATE_SRC_IP = "STATE_SRC_IP"
STATE_DST_NAME = "STATE_DST_NAME"
STATE_DST_IP = "STATE_DST_IP"
STATE_SRC_MAC = "STATE_SRC_MAC"
STATE_DST_MAC = "STATE_DST_MAC"

DEVICE = "DEVICE"
QUEUE = "QUEUE"
QUEUES = "QUEUES"
CHANNEL = "CHANNEL"
STATE = "state"
DATA = "data"
MB_PARAMS = "MB_PARAMS"

SRC_IP_FILTER = "SRC_IP_FILTER"

SHARED_STATE_DECLARE = "SHARED_STATE_DECLARE"
FTMB_BLOCK_DEF = "FTMB_BLOCK_DEF"
FROM_DEVICE_DECLARES = "FROM_DEVICE_DECLARES"
TO_DEVICE_DECLARES = "TO_DEVICE_DECLARES"
THREAD_SCHEDULES = "THREAD_SCHEDULES"
FTMB_BLOCKS_DECLARES = "FTMB_BLOCKS_DECLARES"
LINKS = "LINKS"
LOCKS = "LOCKS"

COUNTER = 'counter'
COUNTER_MB = 'FTMBCounter(INDEX $index, SHARED_STATE shared_state)'
COUNTER_MB_PARAMS = ["$index"]

NAT = 'nat'
NAT_MB = 'FTMBAddrRewriter(pattern $ip_min-$ip_max - 0 0, ' \
         'SHARED_STATE shared_state, ' \
         'QUEUE $queue)'
NAT_MB_PARAMS = ['$ip_min', '$ip_max']

BATCH = 'BATCH'

BEAMER_MUX_MB = 'FTMBBeamerMux(QUEUE $queue, SHARED_STATE shared_state, RING_SIZE 200000, MAX_STATES 800)'
BEAMER_MUX_MB_PARAMS = []

SNAPSHOT_MB = 'FTMBSnapshotCounter(INDEX $index, SHARED_STATE shared_state, PERIOD {PERIOD}, DELAY {DELAY})'
SNAPSHOT_MB_PARAMS = ["$index"]

SHARING_LEVEL = "SHARING_LEVEL"
FTMB_OUTPUT_NAME_FORMAT_STR = 'ftmb-{}.click'
