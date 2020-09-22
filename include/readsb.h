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
        SDR_RTLSDR,
        SDR_BLADERF,
        SDR_MICROBLADERF,
        SDR_MODESBEAST,
        SDR_PLUTOSDR,
        SDR_GNS
    };

    /* Library configuration */
    typedef struct
    {
        unsigned freq;              // Receiver frequency we are listen on
        unsigned dc_filter;         // Should we apply a DC filter?
        unsigned nfix_crc;          // Number of crc bit error(s) to correct
        unsigned check_crc;         // Only display messages with good CRC
        unsigned mode_ac;           // Enable decoding of SSR Modes A & C
        unsigned metric;            // Use metric units
        unsigned use_gnss;          // Use GNSS altitudes with H suffix ("HAE", though it isn't always) when available
        unsigned max_range;         // Maximum decoding range in meters
        unsigned stats_polar_range; // Collect/show a range histogram?
        int altitude;               // Receiver altitude.
        double latitude;            // Receiver location latitude.
        double longitude;           // Receiver location longitude.
    } readsb_config_t;

    /* RTL-SDR device configuration */
    typedef struct
    {
        char *dev_name;
        int gain;
        int enable_agc;
        int ppm_error;
        int biastee;
    } readsb_rtlsdr_config_t;

    /* BladeRF and uBladeRF device configuration */
    typedef struct
    {
        char *dev_name;
        char *fpga_path;
        unsigned decimation;
        unsigned bandwidth;
    } readsb_bladerf_config_t;

    /* ADLAM-Pluto device configuration */
    typedef struct
    {
        char *uri;
        char *network;
        int gain;
    } readsb_plutosdr_config_t;

    /* Mode-S Beast and GNS Hulc device configuration */
    typedef struct
    {
        char *dev_name;
        int filter_df045;
        int filter_df1117;
        int mode_ac;
        int mlat_timestamp;
        int fec;
        int crc;
    } readsb_beastgns_config_t;

    READSB_API void say_hello();

    READSB_API int readsb_init(readsb_config_t *lib_config);
    READSB_API int readsb_open(enum sdr_type sdr_type, void *sdr_config);
    READSB_API int readsb_close();
    READSB_API unsigned readsb_get_aircraft_count();
    READSB_API void *readsb_get_aircraft_by_address(unsigned addr);

#ifdef __cplusplus
}
#endif

#endif /* __READSB_H */