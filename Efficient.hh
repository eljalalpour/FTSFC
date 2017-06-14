#pragma once

#include <sstream>
#include <iostream>
#include <chrono>
#include <unordered_map>

#define ENABLE_DEBUG 1
#define ENABLE_LOG   1

#ifdef ENABLE_DEBUG
#define DEBUG(...) click_chatter(__VA_ARGS__)
#else
#define DEBUG(...)
#endif

#ifdef ENABLE_LOG
#define LOG(...) click_chatter(__VA_ARGS__)
#else
#define LOG(...)
#endif

#define PORTS_2_2 "2/2"
#define PORTS_2_1 "2/1"
#define PORTS_1_2 "1/2"

#define MB_ID_MAX 512

#define CURRENT_TIMESTAMP std::chrono::high_resolution_clock::now().time_since_epoch().count()

#define MAKE_UNIQUE_PACKET_ID(IP_OFFSET, IP_ID, SEQ_NUM) (((uint64_t)IP_OFFSET) << 48) | (((uint64_t)IP_ID) << 32) | ((uint64_t)SEQ_NUM)

// FTPacketId type identifies a packet uniquely
typedef uint64_t FTPacketId;

typedef int64_t FTTimestamp;

// FTMBId type identifies a MB uniquely
typedef uint16_t VLANId;

// FTMBId type identifies a MB uniquely
typedef uint8_t FTMBId;

struct fw_state {
    uint64_t ts;
    uint8_t  ack;
    uint64_t lc;
    int32_t  cnt;
};

struct mn_state {
    uint64_t ts;
    uint8_t  ack;
    uint64_t lc;
    int32_t  cnt;
};

struct nat_state {
    uint64_t ts;
    uint8_t  ack;
    uint64_t lc;

    int32_t src_ip1;
    int16_t src_prt1;
    int32_t dst_ip1;
    int16_t dst_prt1;

    int32_t src_ip2;
    int16_t src_prt2;
    int32_t dst_ip2;
    int16_t dst_prt2;
};