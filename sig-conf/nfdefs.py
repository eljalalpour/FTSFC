NF = """
// Shared state
{SHARED_STATE_DECLARE}

// NF Block definition
{NF_BLOCK_DEF}

// From device declarations
{FROM_DEVICE_DECLARES}

// To device declarations
{TO_DEVICE_DECLARES}

// Scheduling
{THREAD_SCHEDULES}

// NF Block declarations
{NF_BLOCKS_DECLARES}

// MB graph
{LINKS}
"""

NF_BLOCK = """elementclass NFBlock {{
$src_ip{MB_PARAM} |
    input
    -> MarkIPHeader(14)
    -> IPFilter(allow udp && src {SRC_IP_FILTER}/16)
    -> {MB}
    -> MarkIPHeader(14)
    -> StoreEtherAddress({DATA_SRC_MAC}, src) // {DATA_SRC_NAME}
    -> StoreEtherAddress({DATA_DST_MAC}, dst) // {DATA_DST_NAME}
    -> StoreIPAddress($src_ip, src) // {DATA_SRC_NAME}
    -> StoreIPAddress({DATA_DST_IP}, dst) // {DATA_DST_NAME}
    -> output
}}
"""

NAT_NF_BLOCK = """elementclass PreNFBlock {{
    input
    -> MarkIPHeader(14)
    -> IPFilter(allow udp && src {SRC_IP_FILTER}/16)
    -> output
}}

elementclass PostNFBlock {{
$src_ip |
    input
    -> MarkIPHeader(14)
    -> StoreEtherAddress({DATA_SRC_MAC}, src) // {DATA_SRC_NAME}
    -> StoreEtherAddress({DATA_DST_MAC}, dst) // {DATA_DST_NAME}
    -> StoreIPAddress($src_ip, src) // {DATA_SRC_NAME}
    -> StoreIPAddress({DATA_DST_IP}, dst) // {DATA_DST_NAME}
    -> output
}}
"""

LINK_FORMAT_STR = """// Queue {QUEUE}
{FROM_DATA_DEVICE_NAME}
-> {NF_BLOCK_NAME}
-> {TO_DATA_DEVICE_NAME};
"""

NAT_LINK_FORMAT_STR = """// Queue {QUEUE}
{FROM_DATA_DEVICE_NAME}
-> {PRE_NF_BLOCK_NAME}
-> [{QUEUE}]nat[{QUEUE}]
-> {POST_NF_BLOCK_NAME}
-> {TO_DATA_DEVICE_NAME};
"""

SHARED_STATE_FORMAT_STR = "array::LockFreeArray;"
NAT_SHARED_STATE_FORMAT_STR = "shared::SharedLocks;"

FROM_DEVICE_NAME_FORMAT_STR = "fd{QUEUE}"
THREAD_SCHED_FORMAT_STR = "StaticThreadSched({});"
FROM_DEVICE_FORMAT_STR = "fd{QUEUE}::FromDPDKDevice({DEVICE}, {QUEUE});"

TO_DEVICE_NAME_FORMAT_STR = "td{QUEUE}"
TO_DEVICE_FORMAT_STR = "td{QUEUE}::ToDPDKDevice({DEVICE}, {QUEUE});"

NF_BLOCK_NAME_FORMAT_STR = "nfb{}"
NF_BLOCK_FORMAT_STR = "nfb{QUEUE}::NFBlock({DATA_SRC_IP}{MB_PARAMS});"

NAT_PRE_NF_BLOCK_NAME_FORMAT_STR = "pre_nfb{}"
NAT_POST_NF_BLOCK_NAME_FORMAT_STR = "post_nfb{}"
NAT_NF_BLOCK_FORMAT_STR = "pre_nfb{QUEUE}::PreNFBlock();" \
                          "\npost_nfb{QUEUE}::PostNFBlock({DATA_SRC_IP});"

ID = "ID"
F = "F"
CHAIN = "CHAIN"
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
DATA = "data"
MB_PARAMS = "MB_PARAMS"

BATCH = "BATCH"
SRC_IP_FILTER = "SRC_IP_FILTER"

SHARED_STATE_DECLARE = "SHARED_STATE_DECLARE"
NF_BLOCK_DEF = "NF_BLOCK_DEF"
FROM_DEVICE_DECLARES = "FROM_DEVICE_DECLARES"
TO_DEVICE_DECLARES = "TO_DEVICE_DECLARES"
THREAD_SCHEDULES = "THREAD_SCHEDULES"
NF_BLOCKS_DECLARES = "NF_BLOCKS_DECLARES"
LINKS = "LINKS"

COUNTER_MB = 'NFLockFreeCounter(INDEX $index)'
COUNTER_MB_PARAMS = ["$index"]

BEAMER_MUX_MB = 'BeamerMux(RING_SIZE 200000, MAX_STATES 800)'
BEAMER_MUX_MB_PARAMS = []

# NAT_MB_DEF = 'nat::AddrRewriter({})'
# NAT_MB = '[$port]nat[$port]'
NAT_MB = 'AddrRewriter({PATTERN})'
NAT_MB_PARAMS = ['$pattern']
# NAT_MB_PARAMS_FORMAT = 'pattern {IP_MIN}-{IP_MAX} - {PORT} {PORT}'
NAT_MB_PARAMS_FORMAT = 'pattern {IP_MIN}-{IP_MAX} - 0 0'
IP_MIN = 'IP_MIN'
IP_MAX = 'IP_MAX'
PORT = 'PORT'
PATTERN = 'PATTERN'

NF_OUTPUT_NAME_FORMAT_STR = 'nf-{}.click'
