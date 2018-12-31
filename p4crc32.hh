#ifndef CLICK_BEAMER_P4CRC32_HH
#define CLICK_BEAMER_P4CRC32_HH

#include <click/config.h>

CLICK_DECLS

namespace Beamer
{

    struct HashTouple
    {
        uint32_t src_ip;
        uint16_t src_port;
    } __attribute__((packed));

    uint32_t p4_crc32(const char *buf, size_t len);

    uint32_t p4_crc32_6(const char *buf);

}

CLICK_ENDDECLS

#endif /* CLICK_BEAMER_P4CRC32_HH */