#ifndef CLICK_BEAMER_DIPMAP_HH
#define CLICK_BEAMER_DIPMAP_HH

#include <click/config.h>

CLICK_DECLS

namespace Beamer
{

    template <typename MAP_ENTRY, typename LOG_HEADER> class DIPMapBase
    {
    public:
        typedef MAP_ENTRY MapEntry;
        typedef LOG_HEADER LogHeader;

    protected:
        unsigned long long count;

        volatile MapEntry *buf;

    public:
        DIPMapBase() {}

        DIPMapBase(unsigned long count)
                : count(count)
        {
            buf = new MAP_ENTRY[count]; assert(buf);
            memset(const_cast<MapEntry *>(buf), 0, sizeof(count * sizeof(MapEntry)));
        }

        void init(unsigned long count)
        {
            *this = DIPMapBase(count);
        }

        unsigned long size() const
        {
            return count;
        }

        void updateEntry(unsigned long index, uint32_t dip, LogHeader header);

        void putEntries(unsigned long index, MapEntry *entries, unsigned long count)
        {
            for (unsigned long long i = 0; i < count; i++)
                buf[index + i] = entries[i];
        }

        MapEntry get(unsigned long hash)
        {
            return buf[hash % count];
        }
    };

    class PlainDIPMap: public DIPMapBase<uint32_t, int[0]>
    {
    public:
        void updateEntry(unsigned long index, uint32_t dip, LogHeader header)
        {
            (void)header;

            buf[index] = dip;
        }
    };

    struct DIPHistoryEntry
    {
        uint32_t current;
        uint32_t prev;
        uint32_t timestamp; /* Unix timestamp */

        DIPHistoryEntry() {}

        DIPHistoryEntry(const DIPHistoryEntry &other)
                : current(other.current), prev(other.prev), timestamp(other.timestamp) {}

        DIPHistoryEntry(volatile const DIPHistoryEntry &other)
                : current(other.current), prev(other.prev), timestamp(other.timestamp) {}

        volatile DIPHistoryEntry& operator = (const DIPHistoryEntry &other) volatile
        {
            this->current = 0;
            this->prev = other.prev;
            this->timestamp = other.timestamp;
            this->current = other.current;

            return *this;
        }
    } __attribute__((packed));

    struct DIPHistoryLogHeader
    {
        uint32_t timestamp; /* Unix timestamp */
    } __attribute__((packed));

    class DIPHistoryMap: public DIPMapBase<DIPHistoryEntry, DIPHistoryLogHeader>
    {
    public:
        void updateEntry(unsigned long index, uint32_t dip, LogHeader header)
        {
            volatile MapEntry *entry = &buf[index];

            /* the DIP might see current == prev; handle this case carefully */

            entry->prev = entry->current;
            entry->timestamp = header.timestamp;
            entry->current = dip;
        }
    };

}

CLICK_ENDDECLS

#endif /* CLICK_BEAMER_DIPMAP_HH */