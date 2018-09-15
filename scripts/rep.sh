#!/bin/bash

if [ "$#" -ne 1 ]; then
    echo "Illegal number of parameters"
    exit 1
fi

ip=$1
dpdk=/home/eghaznavi/Milad/dpdk-stable-17.05.2/usertools
pinglist=(
    "192.168.1.101"
    "192.168.1.102"
    "192.168.1.103"
    "192.168.1.104"
    "192.168.1.107"
    "192.168.1.108"
    "192.168.1.109"
    "192.168.1.110"
    "192.168.1.111"
    "192.168.1.112"
)
pingcount=2
sleeptime=3
int="eth0"
nic="03:00.0"
dpdkdrv="igb_uio"
kerneldrv="ixgbe"
cwd=$(pwd)

echo "Rebinding the interface to kernel..."
cd ${dpdk}
./dpdk-devbind.py -u ${nic}
./dpdk-devbind.py -b ${kerneldrv} ${nic}

echo "Bring up the interface..."
ifconfig ${int} ${ip}

echo "Sleep..."
sleep ${sleeptime}

for i in "${pinglist[@]}"; do   # The quotes are necessary here
    echo "Pinging ${i}..."
    ping ${i} -c ${pingcount}
done

echo "Rebind interface to dpdk..."
ifconfig ${int} down
./dpdk-devbind.py -u ${nic}
./dpdk-devbind.py -b ${dpdkdrv} ${nic}

echo "Verify the status"
./dpdk-devbind.py --status

cd ${cwd}
