TF = """
// Shared locks and state
{SHARED_STATE_DECLARE}

// TF Block definition
{TF_BLOCK_DEF}

// From device declarations
{FROM_DEVICE_DECLARES}

// To device declarations
{TO_DEVICE_DECLARES}

// Scheduling
{THREAD_SCHEDULES}

// TF Block declarations
{TF_BLOCKS_DECLARES}

// MB graph
{LINKS}
"""

TF_BLOCK = """elementclass TFBlock {{
$queue, $src_ip{MB_PARAM}|
    input
    -> MarkIPHeader(14)
    -> IPFilter(allow udp && src {SRC_IP_FILTER}/16)
    -> {MB}
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
-> {TF_BLOCK_NAME}
-> {TO_DATA_DEVICE_NAME};
"""
SHARED_STATE_FORMAT_STR = "shared_locks::SharedLocks(LOCKS {LOCKS});" \
                          "\ntrans::Transmitter(QUEUES {QUEUES}, {REPLICAS});" \
                          "\nshared_state::TFSharedState" \
                          "(SHARED_LOCKS shared_locks, TRANSMITTER trans, BATCH {BATCH}, QUEUES {QUEUES});"


SHARED_STATE_COUNTER_FORMAT_STR = "shared_locks::SharedLocks(LOCKS {LOCKS});" \
                                  "\ntrans::Transmitter(QUEUES {QUEUES}, {REPLICAS});" \
                                  "\nshared_state::TFSharedStateCounter" \
                                  "(SHARED_LOCKS shared_locks, TRANSMITTER trans, BATCH {BATCH}, QUEUES {QUEUES}," \
                                  "\n\tSHARING_LEVEL {SHARING_LEVEL});"

SHARED_STATE_NAT_FORMAT_STR = "shared_locks::SharedLocks(LOCKS {LOCKS});" \
                                  "\ntrans::Transmitter(QUEUES {QUEUES}, {REPLICAS});" \
                                  "\nshared_state::TFSharedStateAddrRewriter" \
                                  "(SHARED_LOCKS shared_locks, TRANSMITTER trans, BATCH {BATCH}, QUEUES {QUEUES});"

FROM_DEVICE_NAME_FORMAT_STR = "fd{QUEUE}"
THREAD_SCHED_FORMAT_STR = "StaticThreadSched({});"
FROM_DEVICE_FORMAT_STR = "fd{QUEUE}::FromDPDKDevice({DEVICE}, {QUEUE});"

TO_DEVICE_NAME_FORMAT_STR = "td{QUEUE}"
TO_DEVICE_FORMAT_STR = "td{QUEUE}::ToDPDKDevice({DEVICE}, {QUEUE});"

TF_BLOCK_NAME_FORMAT_STR = "tf{}"
TF_BLOCK_FORMAT_STR = "tf{QUEUE}::TFBlock({QUEUE}, {DATA_SRC_IP}{MB_PARAMS});"

MB = "MB"
MB_PARAM = 'MB_PARAM'

DATA_SRC_NAME = "DATA_SRC_NAME"
DATA_SRC_IP = "DATA_SRC_IP"
DATA_DST_NAME = "DATA_DST_NAME"
DATA_DST_IP = "DATA_DST_IP"
DATA_SRC_MAC = "DATA_SRC_MAC"
DATA_DST_MAC = "DATA_DST_MAC"

DEVICE = "DEVICE"
QUEUE = "QUEUE"
QUEUES = "QUEUES"
CHANNEL = "CHANNEL"
STATE = "state"
DATA = "data"
MB_PARAMS = "MB_PARAMS"

SRC_IP_FILTER = "SRC_IP_FILTER"

SHARED_STATE_DECLARE = "SHARED_STATE_DECLARE"
TF_BLOCK_DEF = "TF_BLOCK_DEF"
FROM_DEVICE_DECLARES = "FROM_DEVICE_DECLARES"
TO_DEVICE_DECLARES = "TO_DEVICE_DECLARES"
THREAD_SCHEDULES = "THREAD_SCHEDULES"
TF_BLOCKS_DECLARES = "TF_BLOCKS_DECLARES"
LINKS = "LINKS"
LOCKS = "LOCKS"

COUNTER = 'counter'
COUNTER_MB = 'TFCounter(INDEX $index, SHARED_STATE shared_state)'
COUNTER_MB_PARAMS = ["$index"]

NAT = 'nat'
NAT_MB = 'TFAddrRewriter(pattern $ip_min-$ip_max - 0 0, ' \
         'SHARED_LOCKS shared_locks, ' \
         'SHARED_STATE shared_state, ' \
         'QUEUE $queue)'
NAT_MB_PARAMS = ['$ip_min', '$ip_max']

BATCH = 'BATCH'
REPLICAS = 'REPLICAS'
BEAMER_MUX_MB = 'TFBeamerMux(QUEUE $queue, SHARED_STATE shared_state, RING_SIZE 200000, MAX_STATES 800)'
BEAMER_MUX_MB_PARAMS = []

SHARING_LEVEL = "SHARING_LEVEL"
TF_OUTPUT_NAME_FORMAT_STR = 'tf-{}.click'

TF_DATA_DEVICE_ID = 0