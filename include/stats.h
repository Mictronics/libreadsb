#ifndef __STATS_H
#define __STATS_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <time.h>
#include "crc.h"

    struct stats
    {
        uint64_t start;
        uint64_t end;
        // Mode S demodulator counts:
        uint32_t demod_preambles;
        uint32_t demod_rejected_bad;
        uint32_t demod_rejected_unknown_icao;
        uint32_t demod_accepted[MODES_MAX_BITERRORS + 1];
        uint64_t samples_processed;
        uint64_t samples_dropped;
        // Mode A/C demodulator counts:
        uint32_t demod_modeac;
        // number of signals with power > -3dBFS
        uint32_t strong_signal_count;
        // noise floor:
        double noise_power_sum;
        uint64_t noise_power_count;
        // mean signal power:
        double signal_power_sum;
        uint64_t signal_power_count;
        // peak signal power seen
        double peak_signal_power;
        // timing:
        struct timespec demod_cpu;
        struct timespec reader_cpu;
        struct timespec background_cpu;
        // remote messages:
        uint32_t remote_received_modeac;
        uint32_t remote_received_modes;
        uint32_t remote_rejected_bad;
        uint32_t remote_rejected_unknown_icao;
        uint32_t remote_accepted[MODES_MAX_BITERRORS + 1];
        // total messages:
        uint32_t messages_total;
        // CPR decoding:
        unsigned int cpr_surface;
        unsigned int cpr_airborne;
        unsigned int cpr_global_ok;
        unsigned int cpr_global_bad;
        unsigned int cpr_global_skipped;
        unsigned int cpr_global_range_checks;
        unsigned int cpr_global_speed_checks;
        unsigned int cpr_local_ok;
        unsigned int cpr_local_skipped;
        unsigned int cpr_local_range_checks;
        unsigned int cpr_local_speed_checks;
        unsigned int cpr_local_aircraft_relative;
        unsigned int cpr_local_receiver_relative;
        unsigned int cpr_filtered;
        // number of altitude messages ignored because
        // we had a recent DF17/18 altitude
        unsigned int suppressed_altitude_messages;
        // aircraft:
        // total "new" aircraft (i.e. not seen in the last 30 or 300s)
        unsigned int unique_aircraft;
        // we saw only a single message
        unsigned int single_message_aircraft;
        double longest_distance; // Longest range decoded, in *metres*
        uint32_t with_positions; // Aircrafts with positions
        uint32_t mlat_positions; // Positions from mlat source
        uint32_t tisb_positions; // Positions from tisb source
    };

    struct range_stats
    {
        // Maximum polar ranges
#define POLAR_RANGE_RESOLUTION 5 // degree
#define POLAR_RANGE_BUCKETS (360 / POLAR_RANGE_RESOLUTION)
        uint32_t polar_range[POLAR_RANGE_BUCKETS];
    };

    void add_stats(const struct stats *st1, const struct stats *st2, struct stats *target);
    void reset_stats(struct stats *st);
    void add_timespecs(const struct timespec *x, const struct timespec *y, struct timespec *z);

#ifdef __cplusplus
}
#endif
#endif /* __STATS_H */