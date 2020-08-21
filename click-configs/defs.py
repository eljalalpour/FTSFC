AQUA_MACHINES = ["aqua{:02d}".format(s) for s in range(0, 13)]
NIC_10G_IP_LIST = ["192.168.1.1{:02d}".format(s) for s in range(0, 13)]
# Setup your network mac list here. These are used for data network
NIC_10G_MAC_LIST = [
    '',
    '',
    '',
    '',
    '',
    '',
    '',
    '',
    '',
    '',
    '',
    '',
    '',
]

NIC_40G_IP_LIST = [""]
NIC_40G_IP_LIST.extend(["10.70.0.{}".format(s) for s in range(1, 13)])
# List other set of network macs here, this is by default is used for data channel
NIC_40G_MAC_LIST = [
    '',
    '',
    '',
    '',
    '',
    '',
    '',
    '',
    '',
    '',
    '',
    '',
    '',
]

STATE_DEVICE_ID = 0
DATA_DEVICE_ID = 1

LATENCY_STATE_DEVICE_ID = 1
LATENCY_DATA_DEVICE_ID = 0

THROUGHPUT_STATE_DEVICE_ID = 0
THROUGHPUT_DATA_DEVICE_ID = 1

FIRST_AQUA_MACHINE_IN_CHAIN = 7
FIRST_AQUA_MACHINE_FOR_REPLICAS = 2
CPUS = 8

COUNTER = 'counter'
LB = 'lb'
NAT = 'nat'
MONITOR = 'monitor'
STATE_GEN = 'state'

SNAPSHOT = 'snapshot'

MBS = [
    COUNTER,
    LB,
    NAT,
    MONITOR,
    STATE_GEN,
    SNAPSHOT
]


IN_TR_SRC_IP_MIN = "1.0.0.0"
IN_TR_SRC_IP_MAX = "1.0.255.255"

NO_SHARED_STATE = '// No shared state'

NAT_LOCKS = 131072
COUNTER_LOCKS = 8
LB_LOCKS = 1


THROUGHPUT_METRIC = 'thr'
LATENCY_METRIC = 'lat'

SHARING_LEVEL = "SHARING_LEVEL"
LOCKS = 'LOCKS'

DEBUG_COUNTER = '\n-> c{}::Counter()'

PERIOD = 'PERIOD'
DELAY = 'DELAY'

GEN_SERVER_INDEX = -FIRST_AQUA_MACHINE_IN_CHAIN + 1
