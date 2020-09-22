#ifndef __READSB_H
#define __READSB_H

#ifdef __cplusplus
extern "C"
{
#endif
#include <stdint.h>
#include <stddef.h>
#include "readsb_export.h"

    enum error_no
    {
        ERR_SUCCESS = 0,
        ERR_FAILURE,
    };

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
        uint32_t freq;      // Receiver frequency we are listen on
        uint32_t max_range; // Maximum decoding range in meters
        uint32_t altitude;  // Receiver altitude.
        double latitude;    // Receiver location latitude.
        double longitude;   // Receiver location longitude.
        uint8_t nfix_crc;   // Number of crc bit error(s) to correct
        uint8_t mode_ac;    // Enable decoding of SSR Modes A & C
        uint8_t dc_filter;  // Should we apply a DC filter?
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

    READSB_API enum error_no readsb_init(readsb_config_t *config);
    READSB_API enum error_no readsb_open(enum sdr_type sdr_type, void *config);
    READSB_API void readsb_close();
    READSB_API unsigned readsb_get_aircraft_count();
    READSB_API void *readsb_get_aircraft_by_address(unsigned addr);

#ifdef __cplusplus
}
#endif

#endif /* __READSB_H */