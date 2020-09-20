#ifndef __READSB_H
#define __READSB_H

#ifdef __cplusplus
extern "C"
{
#endif
#include "readsb_export.h"

    READSB_API void say_hello();

    READSB_API void readsb_init();
    READSB_API void readsb_open();
    READSB_API void readsb_close();

#ifdef __cplusplus
}
#endif

#endif /* __READSB_H */