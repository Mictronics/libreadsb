#ifndef __MODES_S_H
#define __MODES_S_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <assert.h>
#include "readsb_def.h"

    // datafield extraction helpers

    // The first bit (MSB of the first byte) is numbered 1, for consistency
    // with how the specs number them.

    // Extract one bit from a message.

    static inline __attribute__((always_inline)) unsigned
    getbit(unsigned char *data, unsigned bitnum)
    {
        unsigned bi = bitnum - 1;
        unsigned by = bi >> 3;
        unsigned mask = 1 << (7 - (bi & 7));

        return (data[by] & mask) != 0;
    }

    // Extract some bits (firstbit .. lastbit inclusive) from a message.

    static inline __attribute__((always_inline)) unsigned
    getbits(unsigned char *data, unsigned firstbit, unsigned lastbit)
    {
        unsigned fbi = firstbit - 1;
        unsigned lbi = lastbit - 1;
        unsigned nbi = (lastbit - firstbit + 1);

        unsigned fby = fbi >> 3;
        unsigned lby = lbi >> 3;
        unsigned nby = (lby - fby) + 1;

        unsigned shift = 7 - (lbi & 7);
        unsigned topmask = 0xFF >> (fbi & 7);

        assert(fbi <= lbi);
        assert(nbi <= 32);
        assert(nby <= 5);

        if (nby == 5)
        {
            return ((data[fby] & topmask) << (32 - shift)) |
                   (data[fby + 1] << (24 - shift)) |
                   (data[fby + 2] << (16 - shift)) |
                   (data[fby + 3] << (8 - shift)) |
                   (data[fby + 4] >> shift);
        }
        else if (nby == 4)
        {
            return ((data[fby] & topmask) << (24 - shift)) |
                   (data[fby + 1] << (16 - shift)) |
                   (data[fby + 2] << (8 - shift)) |
                   (data[fby + 3] >> shift);
        }
        else if (nby == 3)
        {
            return ((data[fby] & topmask) << (16 - shift)) |
                   (data[fby + 1] << (8 - shift)) |
                   (data[fby + 2] >> shift);
        }
        else if (nby == 2)
        {
            return ((data[fby] & topmask) << (8 - shift)) |
                   (data[fby + 1] >> shift);
        }
        else if (nby == 1)
        {
            return (data[fby] & topmask) >> shift;
        }
        else
        {
            return 0;
        }
    }

    int modes_message_len_by_type(int type);
    int score_modes_message(unsigned char *msg, int validbits);
    int decode_modes_message(modes_message_t *mm, unsigned char *msg);
    void use_modes_message(modes_message_t *mm);

#ifdef __cplusplus
}
#endif
#endif /* __MODE_S_H */