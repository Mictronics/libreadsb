#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fifo.h"
#include "crc.h"
#include "icao_filter.h"
#include "mode_ac.h"
#include "track.h"
#include "geomag.h"
#include "readsb.h"
#include "readsb_def.h"

readsb_t lib_state;

static void cleanup()
{
    /* Go through tracked aircraft chain and free up any used memory */
    for (int j = 0; j < AIRCRAFTS_BUCKETS; j++)
    {
        struct aircraft *a = lib_state.aircrafts[j], *na;
        while (a)
        {
            na = a->next;
            if (a)
                free(a);
            a = na;
        }
    }

    fifo_destroy();
    crc_cleanup_tables();
}

enum error_no readsb_init(readsb_config_t *config)
{
    if (config != NULL)
    {
        memcpy(&lib_state.config, config, sizeof(readsb_config_t));
    }
    else
    {
        lib_state.config.freq = MODES_DEFAULT_FREQ;
        lib_state.config.dc_filter = 0;
        lib_state.config.nfix_crc = 1;
        lib_state.config.mode_ac = 0;
        lib_state.config.max_range = 1852 * 300; // 300nm
        lib_state.config.altitude = 0;
        lib_state.config.latitude = 0.0;
        lib_state.config.longitude = 0.0;
        fprintf(stderr, "libreadsb: Using default configuration\n");
    }

    lib_state.sample_rate = (double)2400000.0;

    // Allocate the various buffers used by Modes
    lib_state.trailing_samples = (MODES_PREAMBLE_US + MODES_LONG_MSG_BITS + 16) * 1e-6 * lib_state.sample_rate;

    if (!fifo_create(MODES_MAG_BUFFERS, MODES_MAG_BUF_SAMPLES + lib_state.trailing_samples, lib_state.trailing_samples))
    {
        fprintf(stderr, "libreadsb: Out of memory allocating FIFO\n");
        return ERR_FAILURE;
    }

    // Validate the users Lat/Lon home location inputs
    if ((lib_state.config.latitude > 90.0)      // Latitude must be -90 to +90
        || (lib_state.config.latitude < -90.0)  // and
        || (lib_state.config.longitude > 360.0) // Longitude must be -180 to +360
        || (lib_state.config.longitude < -180.0))
    {
        lib_state.config.latitude = lib_state.config.longitude = 0.0;
    }
    else if (lib_state.config.longitude > 180.0)
    { // If Longitude is +180 to +360, make it -180 to 0
        lib_state.config.longitude -= 360.0;
    }
    // If both Lat and Lon are 0.0 then the users location is either invalid/not-set, or (s)he's in the
    // Atlantic ocean off the west coast of Africa. This is unlikely to be correct.
    // Set the user LatLon valid flag only if either Lat or Lon are non zero. Note the Greenwich meridian
    // is at 0.0 Lon,so we must check for either fLat or fLon being non zero not both.
    // Testing the flag at runtime will be much quicker than ((fLon != 0.0) || (fLat != 0.0))
    lib_state.bUserFlags &= ~MODES_USER_LATLON_VALID;
    if ((lib_state.config.latitude != 0.0) || (lib_state.config.longitude != 0.0))
    {
        lib_state.bUserFlags |= MODES_USER_LATLON_VALID;
    }

    // Prepare error correction tables
    modes_checksum_init(lib_state.config.nfix_crc);
    icao_filter_init();
    mode_ac_init();
    geomag_init();

    // init stats:
    lib_state.stats_current.start = lib_state.stats_current.end =
        lib_state.stats_alltime.start = lib_state.stats_alltime.end =
            lib_state.stats_periodic.start = lib_state.stats_periodic.end =
                lib_state.stats_5min.start = lib_state.stats_5min.end =
                    lib_state.stats_15min.start = lib_state.stats_15min.end = mstime();

    for (int j = 0; j < 15; ++j)
    {
        lib_state.stats_1min[j].start = lib_state.stats_1min[j].end = lib_state.stats_current.start;
    }

    return ERR_SUCCESS;
}

void readsb_close()
{
    lib_state.is_exit = 1;

    cleanup();
}