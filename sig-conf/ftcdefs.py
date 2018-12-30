FTC = """
// Shared state
{SHARED_STATE_DECLARE}

// FTC Block definition
{FTC_BLOCK_DEF}

// From device declarations
{FROM_DEVICE_DECLARES}

// To device declarations
{TO_DEVICE_DECLARES}

// Scheduling
{THREAD_SCHEDULES}

// FTC Block declarations
{FTC_BLOCKS_DECLARES}

// MB graph
{LINKS}
"""

FTC_BLOCK = """elementclass FTBlock {{
$queue, $src_ip{MB_PARAM}|
    input
    -> MarkIPHeader(14)
    -> IPFilter(allow udp && src {SRC_IP_FILTER}/16)
    -> PMProcess(QUEUE $queue)
    -> {MB}
    -> PMConstruct(QUEUE $queue)
    -> MarkIPHeader(14)
    -> StoreEtherAddress({DATA_SRC_MAC}, src) // {DATA_SRC_NAME}
    -> StoreEtherAddress({DATA_DST_MAC}, dst) // {DATA_DST_NAME}
    -> StoreIPAddress($src_ip, src) // {DATA_SRC_NAME}
    -> StoreIPAddress({DATA_DST_IP}, dst) // {DATA_DST_NAME}
    -> output;
}}
"""

FTC_BLOCK_WITH_FORWARDER = """elementclass FTBlock {{
$queue, $src_ip{MB_PARAM} |
    forwarder::Forwarder(CHAIN {CHAIN});

    // State channel
    input[1]
    -> MarkIPHeader(14)
    -> IPFilter(allow udp && src 2.0.0.0/16)
    -> [1]forwarder;

    // Data channel
    input[0]
    -> MarkIPHeader(14)
    -> IPFilter(allow udp && src 1.0.0.0/16)
    -> forwarder
    -> PMProcess(QUEUE $queue)
    -> {MB}
    -> PMConstruct(QUEUE $queue)
    -> MarkIPHeader(14)
    -> StoreIPAddress($src_ip, src) // {DATA_SRC_NAME}
    -> StoreEtherAddress({DATA_SRC_MAC}, src) // {DATA_SRC_NAME}
    -> StoreEtherAddress({DATA_DST_MAC}, dst) // {DATA_DST_NAME}
    -> StoreIPAddress({DATA_DST_IP}, dst) // {DATA_DST_NAME}
    -> output;
}}
"""

FTC_BLOCK_WITH_BUFFER = """elementclass FTBlock {{
$queue, $DATA_SRC_IP, $STATE_SRC_IP{MB_PARAM} |
    buffer::Buffer(BATCH {BATCH}, CHAIN {CHAIN});

    input
    -> MarkIPHeader(14)
    -> IPFilter(allow udp && src {SRC_IP_FILTER}/16)
    -> PMProcess(QUEUE $queue)
    -> {MB}
    -> PMConstruct(QUEUE $queue)
    -> buffer;

    // Data channel
    buffer[0]
    -> MarkIPHeader(14)
    -> StoreEtherAddress({DATA_SRC_MAC}, src) // {DATA_SRC_NAME}
    -> StoreEtherAddress({DATA_DST_MAC}, dst) // {DATA_DST_NAME}
    -> StoreIPAddress($DATA_SRC_IP, src) // {DATA_SRC_NAME}
    -> StoreIPAddress({DATA_DST_IP}, dst) // {DATA_DST_NAME}
    -> [0]output;

    // State channel
    buffer[1]
    -> MarkIPHeader(14)
    -> StoreEtherAddress({STATE_SRC_MAC}, src) // {STATE_SRC_NAME}
    -> StoreEtherAddress({STATE_DST_MAC}, dst) // {STATE_DST_NAME}
    -> StoreIPAddress($STATE_SRC_IP, src) // {STATE_SRC_NAME}
    -> StoreIPAddress({STATE_DST_IP}, dst) // {STATE_DST_NAME}
    -> [1]output;
}}
"""

LINK_FORMAT_STR = """// Queue {QUEUE}
{FROM_DATA_DEVICE_NAME}
-> {FTC_BLOCK_NAME}
-> {TO_DATA_DEVICE_NAME};
"""

LINK_WITH_FORWARDER_FORMAT_STR = """// Queue {QUEUE}
{FROM_DATA_DEVICE_NAME}
-> [0]{FTC_BLOCK_NAME};

{FROM_STATE_DEVICE_NAME}
-> [1]{FTC_BLOCK_NAME};

{FTC_BLOCK_NAME}
-> {TO_DATA_DEVICE_NAME};
"""

LINK_WITH_BUFFER_FORMAT_STR = """// Queue {QUEUE}
{FROM_DATA_DEVICE_NAME}
-> {FTC_BLOCK_NAME};

{FTC_BLOCK_NAME}[0]
-> {TO_DATA_DEVICE_NAME};

{FTC_BLOCK_NAME}[1]
-> {TO_STATE_DEVICE_NAME};
"""

SHARED_STATE_FORMAT_STR = "shared_state::SharedLockFreeState(CHAIN {CHAIN}, ID {ID}, F {F})"

FROM_DEVICE_NAME_FORMAT_STR = "{CHANNEL}_fd{QUEUE}"
THREAD_SCHED_FORMAT_STR = "StaticThreadSched({});"
FROM_DEVICE_FORMAT_STR = "{CHANNEL}_fd{QUEUE}::FromDPDKDevice({DEVICE}, {QUEUE});"

TO_DEVICE_NAME_FORMAT_STR = "{CHANNEL}_td{QUEUE}"
TO_DEVICE_FORMAT_STR = "{CHANNEL}_td{QUEUE}::ToDPDKDevice({DEVICE}, {QUEUE});"

FTC_BLOCK_NAME_FORMAT_STR = "ftc{}"
FTC_BLOCK_FORMAT_STR = "ftc{QUEUE}::FTBlock({QUEUE}, {DATA_SRC_IP}{MB_PARAMS});"
FTC_BLOCK_WITH_BUFFER_FORMAT_STR = "ftc{QUEUE}::FTBlock({QUEUE}, {DATA_SRC_IP}, " \
                                  "{STATE_SRC_IP}{MB_PARAMS});"

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

STATE_SRC_NAME = "STATE_SRC_NAME"
STATE_SRC_IP = "STATE_SRC_IP"
STATE_DST_NAME = "STATE_DST_NAME"
STATE_DST_IP = "STATE_DST_IP"
STATE_SRC_MAC = "STATE_SRC_MAC"
STATE_DST_MAC = "STATE_DST_MAC"

DEVICE = "DEVICE"
QUEUE = "QUEUE"
CHANNEL = "CHANNEL"
STATE = "state"
DATA = "data"
MB_PARAMS = "MB_PARAMS"

BATCH = "BATCH"
SRC_IP_FILTER = "SRC_IP_FILTER"

SHARED_STATE_DECLARE = "SHARED_STATE_DECLARE"
FTC_BLOCK_DEF = "FTC_BLOCK_DEF"
FROM_DEVICE_DECLARES = "FROM_DEVICE_DECLARES"
TO_DEVICE_DECLARES = "TO_DEVICE_DECLARES"
THREAD_SCHEDULES = "THREAD_SCHEDULES"
FTC_BLOCKS_DECLARES = "FTC_BLOCKS_DECLARES"
LINKS = "LINKS"


COUNTER = 'counter'
COUNTER_MB = 'FTLockFreeCounter(INDEX $index)'
COUNTER_MB_PARAMS = ["$index"]

FTC_OUTPUT_NAME_FORMAT_STR = 'ft-{}.click'
