#ifndef __READSB_DEF_H
#define __READSB_DEF_H

#ifdef __cplusplus
extern "C"
{
#endif

#define MODES_DEFAULT_FREQ 1090000000
#define MODES_RTL_BUFFERS 16                           // Number of RTL buffers
#define MODES_RTL_BUF_SIZE (16 * 16384)                // 256k
#define MODES_MAG_BUF_SAMPLES (MODES_RTL_BUF_SIZE / 2) // Each sample is 2 bytes
#define MODES_MAG_BUFFERS 12                           // Number of magnitude buffers (should be smaller than RTL_BUFFERS for flowcontrol to work)
#define MODES_AUTO_GAIN -100                           // Use automatic gain
#define MODES_MAX_GAIN 999999                          // Use max available gain
#define MODEAC_MSG_BYTES 2

#define MODES_PREAMBLE_US 8 // microseconds = bits
#define MODES_PREAMBLE_SAMPLES (MODES_PREAMBLE_US * 2)
#define MODES_PREAMBLE_SIZE (MODES_PREAMBLE_SAMPLES * sizeof(uint16_t))
#define MODES_LONG_MSG_BYTES 14
#define MODES_SHORT_MSG_BYTES 7
#define MODES_LONG_MSG_BITS (MODES_LONG_MSG_BYTES * 8)
#define MODES_SHORT_MSG_BITS (MODES_SHORT_MSG_BYTES * 8)
#define MODES_LONG_MSG_SAMPLES (MODES_LONG_MSG_BITS * 2)
#define MODES_SHORT_MSG_SAMPLES (MODES_SHORT_MSG_BITS * 2)
#define MODES_LONG_MSG_SIZE (MODES_LONG_MSG_SAMPLES * sizeof(uint16_t))
#define MODES_SHORT_MSG_SIZE (MODES_SHORT_MSG_SAMPLES * sizeof(uint16_t))

#define MODES_OS_PREAMBLE_SAMPLES (20)
#define MODES_OS_PREAMBLE_SIZE (MODES_OS_PREAMBLE_SAMPLES * sizeof(uint16_t))
#define MODES_OS_LONG_MSG_SAMPLES (268)
#define MODES_OS_SHORT_MSG_SAMPLES (135)
#define MODES_OS_LONG_MSG_SIZE (MODES_LONG_MSG_SAMPLES * sizeof(uint16_t))
#define MODES_OS_SHORT_MSG_SIZE (MODES_SHORT_MSG_SAMPLES * sizeof(uint16_t))

#define MODES_OUT_BUF_SIZE (16 * 1024)
#define MODES_OUT_FLUSH_SIZE (15 * 1024)
#define MODES_OUT_FLUSH_INTERVAL (60000)

#define MODES_USER_LATLON_VALID (1 << 0)

#define INVALID_ALTITUDE (-9999)

#define MODES_NON_ICAO_ADDRESS (1 << 24) // Set on addresses to indicate they are not ICAO addresses
#define MODES_NOTUSED(V) ((void)V)
#define AIRCRAFTS_BUCKETS 2048

#ifdef __cplusplus
}
#endif
#endif /* __READSB_DEF_H */