#!/bin/bash

if [[ "$#" -ne 3 ]]; then
    echo "Illegal number of parameters"
    exit 1
fi

MB=$1
APPR=$2
THRD=$3
START=1
STEP=1
END=20
LIMIT=9999

SAVE_PATH='/home/eghaznavi/Milad/evaluate/sig19'

MOONGEN='/home/eghaznavi/Milad/MoonGen/build/MoonGen'
LUA_SCRIPT='/home/eghaznavi/Milad/FTSFC/moongen/load-latency.lua'

PKT_SIZE=256
DURATION=10

RED='\033[0;31m'
NC='\033[0m' # No Color

for rate in $(seq $START $STEP $END);
do
        mbps=$(( rate * PKT_SIZE ))
        if [[ ${mbps} -gt ${LIMIT} ]]
        then break; fi

        echo -e "${RED}===========================================${NC}"
        echo -e "${RED}   Running for rate ${mbps} Mbps...${NC}"
        echo -e "${RED}===========================================${NC}"

        PRE_NAME="$SAVE_PATH/lat-${MB}-${APPR}-t-${THRD}-r-${mbps}-p-${PKT_SIZE}"

        ${MOONGEN} ${LUA_SCRIPT} -r ${mbps} -o ${PRE_NAME}.csv -d ${DURATION} -s ${PKT_SIZE} | tee ${PRE_NAME}.txt

#       read -p "Press enter to continue..."

done;