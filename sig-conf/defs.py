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
CPUS = 8


COUNTER_MB = 'counter'
LB_MB = 'lb'
