FT = """
// Shared state
{SHARED_STATE_DECLARE}

// FT Block definition
{FT_BLOCK_DEF}

// From device declarations
{FROM_DEVICE_DECLARES}

// To device declarations
{TO_DEVICE_DECLARES}

// Scheduling
{THREAD_SCHEDULES}

// FT Block declarations
{FT_BLOCKS_DECLARES}

// MB graph
{LINKS}
"""

FT_BLOCK = """elementclass FTBlock {{
$id, $src_ip{MB_PARAM}|
    input
    -> MarkIPHeader(14)
    -> IPFilter(allow udp && src {SRC_IP_FILTER}/16)
    -> PMProcess
    -> FTLockFreeCounter(INDEX $index)
    -> PMConstruct(ID $id)
    -> MarkIPHeader(14)
    -> StoreEtherAddress({DATA_SRC_MAC}, src) // {DATA_SRC_NAME}
    -> StoreEtherAddress({DATA_DST_MAC}, dst) // {DATA_DST_NAME}
    -> StoreIPAddress($src_ip, src) // {DATA_SRC_NAME}
    -> StoreIPAddress({DATA_DST_IP}, dst) // {DATA_DST_NAME}
    -> output;
}}
"""

FT_BLOCK_WITH_FORWARDER = """elementclass FTBlock {{
$id, $src_ip{MB_PARAM} |
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
    -> PMProcess
    -> FTLockFreeCounter(INDEX $index)
    -> PMConstruct(ID $id)
    -> MarkIPHeader(14)
    -> StoreIPAddress($src_ip, src) // {DATA_SRC_NAME}
    -> StoreEtherAddress({DATA_SRC_MAC}, src) // {DATA_SRC_NAME}
    -> StoreEtherAddress({DATA_DST_MAC}, dst) // {DATA_DST_NAME}
    -> StoreIPAddress({DATA_DST_IP}, dst) // {DATA_DST_NAME}
    -> output;
}}
"""

FT_BLOCK_WITH_BUFFER = """elementclass FTBlock {{
$id, $DATA_SRC_IP, $STATE_SRC_IP{MB_PARAM} |
    buffer::Buffer(BATCH {BATCH}, CHAIN {CHAIN});

    input
    -> MarkIPHeader(14)
    -> IPFilter(allow udp && src {SRC_IP_FILTER}/16)
    -> PMProcess
    -> FTLockFreeCounter(INDEX $index)
    -> PMConstruct(ID $id)
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
-> {FT_BLOCK_NAME}
-> {TO_DATA_DEVICE_NAME};
"""

LINK_WITH_FORWARDER_FORMAT_STR = """// Queue {QUEUE}
{FROM_DATA_DEVICE_NAME}
-> [0]{FT_BLOCK_NAME};

{FROM_STATE_DEVICE_NAME}
-> [1]{FT_BLOCK_NAME};

{FT_BLOCK_NAME}
-> {TO_DATA_DEVICE_NAME};
"""

LINK_WITH_BUFFER_FORMAT_STR = """// Queue {QUEUE}
{FROM_DATA_DEVICE_NAME}
-> {FT_BLOCK_NAME};

{FT_BLOCK_NAME}[0]
-> {TO_DATA_DEVICE_NAME};

{FT_BLOCK_NAME}[1]
-> {TO_STATE_DEVICE_NAME};
"""

SHARED_STATE_FORMAT_STR = "shared_state::SharedLockFreeState(CHAIN {CHAIN}, ID {ID}, F {F})"

FROM_DEVICE_NAME_FORMAT_STR = "{CHANNEL}_fd{QUEUE}"
THREAD_SCHED_FORMAT_STR = "StaticThreadSched({});"
FROM_DEVICE_FORMAT_STR = "{CHANNEL}_fd{QUEUE}::FromDPDKDevice({DEVICE}, {QUEUE});"

TO_DEVICE_NAME_FORMAT_STR = "{CHANNEL}_td{QUEUE}"
TO_DEVICE_FORMAT_STR = "{CHANNEL}_td{QUEUE}::ToDPDKDevice({DEVICE}, {QUEUE});"

FT_BLOCK_NAME_FORMAT_STR = "ftb{}"
FT_BLOCK_FORMAT_STR = "ftb{QUEUE}::FTBlock({ID}, {DATA_SRC_IP}{MB_PARAMS});"
FT_BLOCK_WITH_BUFFER_FORMAT_STR = "ftb{QUEUE}::FTBlock({ID}, {DATA_SRC_IP}, " \
                                  "{STATE_SRC_IP}{MB_PARAMS});"

ID = "ID"
F = "F"
CHAIN = "CHAIN"
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
FT_BLOCK_DEF = "FT_BLOCK_DEF"
FROM_DEVICE_DECLARES = "FROM_DEVICE_DECLARES"
TO_DEVICE_DECLARES = "TO_DEVICE_DECLARES"
THREAD_SCHEDULES = "THREAD_SCHEDULES"
FT_BLOCKS_DECLARES = "FT_BLOCKS_DECLARES"
LINKS = "LINKS"

AQUA_MACHINES = ["aqua{:02d}".format(s) for s in range(1, 13)]
NIC_10G_IP_LIST = ["192.168.1.1{:02d}".format(s) for s in range(1, 13)]
NIC_10G_MAC_LIST = [
    '0c:c4:7a:73:fa:72',
    '0c:c4:7a:73:fa:dc',
    '0c:c4:7a:73:fa:26',
    '0c:c4:7a:73:fa:20',
    '0c:c4:7a:73:fa:6e',
    '',  # Aqua06 is out of order
    '0c:c4:7a:73:fa:54',
    '0c:c4:7a:73:fa:6a',
    '0c:c4:7a:73:f9:ec',
    '0c:c4:7a:73:fa:18',
    '0c:c4:7a:73:fa:e6',
    '0c:c4:7a:73:fa:9e',
]

NIC_40G_IP_LIST = ["10.70.0.{}".format(s) for s in range(1, 13)]
NIC_40G_MAC_LIST = [
    'f4:52:14:69:a5:20',
    'e4:1d:2d:13:9c:30',
    'e4:1d:2d:13:9f:80',
    'e4:1d:2d:13:9f:e0',
    'e4:1d:2d:13:9e:b0',
    '',  # Aqua06 is out of order
    'e4:1d:2d:13:9e:d0',
    'f4:52:14:5a:90:70',
    'e4:1d:2d:13:9c:60',
    'e4:1d:2d:0c:89:e0',
    'e4:1d:2d:13:9e:e0',
    'e4:1d:2d:0c:89:d0',
]

STATE_DEVICE_ID = 0
DATA_DEVICE_ID = 1

FIRST_AQUA_MACHINE_IN_CHAIN = 6

COUNTER = 'counter'
COUNTER_MB = 'FTLockFreeCounter(INDEX $index)'
COUNTER_MB_PARAMS = ["$index"]

FTC_OUTPUT_NAME_FORMAT_STR = 'ft-{}.click'
DEFAULT_FAILS = 1
DEFAULT_THREADS = 1
DEFAULT_BATCH = 5

CPUS = 8