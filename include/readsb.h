#ifndef __READSB_H
#define __READSB_H

#ifdef __cplusplus
extern "C"
{
#endif
#include "readsb_export.h"

    enum sdr_type
    {
        SDR_NONE = 0,
        SDR_IFILE,
        SDR_RTLSDR,
        SDR_BLADERF,
        SDR_MICROBLADERF,
        SDR_MODESBEAST,
        SDR_PLUTOSDR,
        SDR_GNS
    };

    typedef struct
    {
        enum sdr_type sdr_type; // where are we getting data from?
        char *dev_name;
        int gain;
        int enable_agc;
        int freq;
        int ppm_error;
        int biastee;
        int dc_filter;         // should we apply a DC filter?
        int nfix_crc;          // Number of crc bit error(s) to correct
        int check_crc;         // Only display messages with good CRC
        int mode_ac;           // Enable decoding of SSR Modes A & C
        int mode_ac_auto;      // allow toggling of A/C by Beast commands
        int metric;            // Use metric units
        int use_gnss;          // Use GNSS altitudes with H suffix ("HAE", though it isn't always) when available
        int max_range;         // Maximum decoding range in meters
        int stats_polar_range; // Collect/show a range histogram?
        int altitude;          // Receiver altitude.
        double latitude;       // Receiver location latitude.
        double longitude;      // Receiver location longitude.
    } readsb_config_t;

    READSB_API void say_hello();

    READSB_API void readsb_init(readsb_config_t *config);
    READSB_API void readsb_open();
    READSB_API void readsb_close();

#ifdef __cplusplus
}
#endif

#endif /* __READSB_H */