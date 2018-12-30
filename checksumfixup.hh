#ifndef CLICK_CLICKITYCLACK_CHECKSUM_HH
#define CLICK_CLICKITYCLACK_CHECKSUM_HH

#include <click/config.h>
#include <click/glue.hh>

CLICK_DECLS

namespace ClickityClack
{

    static inline uint32_t checksumFixup16(uint16_t oldField, uint16_t newField, uint32_t sum)
    {
        return sum + oldField - newField;
    }

    static inline uint32_t checksumFixup32(uint32_t oldField, uint32_t newField, uint32_t sum)
    {
        return checksumFixup16(oldField >> 16, newField >> 16,
                               checksumFixup16(oldField & 0xffff, newField & 0xffff,
                                               sum));
    }

    static inline uint32_t checksumFixup64(uint64_t oldField, uint64_t newField, uint32_t sum)
    {
        return checksumFixup32(oldField >> 32, newField >> 32,
                               checksumFixup32(oldField & 0xffffffff, newField & 0xffffffff,
                                               sum));
    }

    static inline uint32_t checksumMetaFixup(uint16_t oldField, uint16_t newField, uint32_t sum)
    {
        return checksumFixup16(newField, oldField, sum);
    }

    static inline uint16_t checksumFold(uint32_t sum)
    {
        uint16_t ret = (sum >> 16) + (sum & 0xffff);
        ret &= 0xffff;
        return ret;
    }

}

CLICK_ENDDECLS

#endif /* CLICK_CLICKITYCLACK_CHECKSUM_HH */