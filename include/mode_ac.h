#ifndef __MODE_AC_H
#define __MODE_AC_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "readsb_def.h"

    /* Convert from a (hex) mode A value to a 0-4095 index */
    static inline unsigned mode_a_to_index(unsigned mode_a)
    {
        return (mode_a & 0x0007) | ((mode_a & 0x0070) >> 1) | ((mode_a & 0x0700) >> 2) | ((mode_a & 0x7000) >> 3);
    }

    /* Convert from a 0-4095 index to a (hex) mode A value */
    static inline unsigned index_to_mode_a(unsigned index)
    {
        return (index & 0007) | ((index & 0070) << 1) | ((index & 0700) << 2) | ((index & 07000) << 3);
    }

    void decode_mode_a_message(modes_message_t *mm, int ModeA);
    void mode_ac_init();
    int mode_a_to_mode_c(unsigned int modeA);
    unsigned mode_c_to_mode_a(int modeC);

#ifdef __cplusplus
}
#endif
#endif /* __MODE_AC_H */