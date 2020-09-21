#ifndef __MODE_AC_H
#define __MODE_AC_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "readsb_def.h"

    void decode_mode_a_message(struct modesMessage *mm, int ModeA);
    void mode_ac_init();
    int mode_a_to_mode_c(unsigned int modeA);
    unsigned mode_c_to_mode_a(int modeC);

#ifdef __cplusplus
}
#endif
#endif /* __MODE_AC_H */