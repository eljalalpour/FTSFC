#!/bin/bash

if [ "$#" -ne 1 ]; then
    echo "Illegal number of parameters"
    exit 1
fi

ip=$1
dpdk=/home/eghaznavi/Milad/dpdk-stable-17.05.2/usertools
pinglist=(
    "10.70.0.100"
    "10.70.0.1"
    "10.70.0.2"
    "10.70.0.3"
    "10.70.0.4"
    "10.70.0.5"
    "10.70.0.6"
    "10.70.0.7"
    "10.70.0.8"
    "10.70.0.9"
    "10.70.0.10"
    "10.70.0.11"
    "10.70.0.12"
)
pingcount=2
sleeptime=3
int="p1p1"
int2='p2p1'
nic="03:00.0"
cwd=$(pwd)

echo "Bring up the interface..."
ifconfig ${int} ${ip}
ifconfig ${int2} ${ip}

echo "Sleep..."
sleep ${sleeptime}

for i in "${pinglist[@]}"; do   # The quotes are necessary here
    echo "Pinging ${i}..."
    ping ${i} -c ${pingcount}
done

echo "Rebind interface to dpdk..."
ifconfig ${int} down

cd ${cwd}
