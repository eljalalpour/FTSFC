import os
from subprocess import Popen, PIPE
import logging

SUCCESS_CODE = 0
FIRST_RATE = 100000
FIRST_COUNT = 400000
DST_PATH = "dst.%d.txt"
SRC_PATH = "src.%d.txt"

def run_cmd(cmd):
    global logger
    logger = logging.getLogger()
    process = Popen(cmd, shell=True, stdout=PIPE)
    process.wait()
    if process.returncode == SUCCESS_CODE:
        logger.debug(process.stdout.read())
        return process.stdout.read()


def process(rate, count):
    count_dst= 0
    count_src = 0
    errorrate = 100
    dst = "click " + "-e " + '\'require(package "FTSFC");FromDevice(p2)-> FTFilterElement(7)-> VLANDecap-> CheckIPHeader(14)-> FTLoggerElement(dst.%d.txt)-> ToDump(dst.pcap);\'' % (
        rate)
    run_cmd(dst)
    src = "click " + "-e " + '\'require(package "FTSFC");FTFastTCPGen(%d, %d, 60, 68:5b:35:c5:e9:39, 192.168.200.18,68:5b:35:c5:e9:55, 192.168.200.17, 100, 10)-> FTLoggerElement(src.%d.txt)-> VLANEncap(VLAN_ID 5)-> VLANEncap(VLAN_ID 5)-> ToDevice(p1);\'' % (
        rate, count, rate)
    run_cmd(src)
    if os.path.isfile(DST_PATH):
        count_dst = "wc " + "-l " + DST_PATH % (rate)
        print(count_dst)
    if os.path.isfile(SRC_PATH):
        count_src = "wc " + "-l " + SRC_PATH % (rate)
    #error = (int)count_src - (int)count_dst
    error = 0.15
    if count_src == 0:
        logger.error("devision to zero")
    else:
        errorrate = error/count_src
    return error, errorrate


if __name__ == "__main__":
    rate = FIRST_RATE
    count = FIRST_COUNT
    process(rate, count)
    # while True:
    #     error, errorrate = process(rate, count)
    #     if 0.01 < errorrate < 0.02:
    #         print(rate)
    #         break
    #     elif errorrate > 0.02:
    #         rate = rate / 2
    #     else:
    #         rate = rate * 2



