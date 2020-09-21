#ifndef __TRACK_H
#define __TRACK_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <stdbool.h>
#include "util.h"
#include "readsb_def.h"

/* Maximum age of tracked aircraft in milliseconds */
#define TRACK_AIRCRAFT_TTL (10 * 60000)

/* Maximum age of a tracked aircraft with only 1 message received, in milliseconds */
#define TRACK_AIRCRAFT_ONEHIT_TTL 60000

/* Minimum number of repeated Mode A/C replies with a particular Mode A code needed in a
 * 1 second period before accepting that code.
 */
#define TRACK_MODEAC_MIN_MESSAGES 4

/* Special value for Rc unknown */
#define RC_UNKNOWN 0

#define ALTITUDE_BARO_RELIABLE_MAX 20

    typedef struct
    {
        uint64_t stale_interval;      /* how long after an update until the data is stale */
        uint64_t expire_interval;     /* how long after an update until the data expires */
        uint64_t updated;             /* when it arrived */
        uint64_t stale;               /* when it goes stale */
        uint64_t expires;             /* when it expires */
        uint64_t next_reduce_forward; /* when to next forward the data for reduced beast output */
        datasource_t source;          /* where the data came from */
        uint32_t padding;
    } data_validity;

    /* Structure used to describe the state of one tracked aircraft */
    struct aircraft
    {
        uint32_t addr;         // ICAO address
        addr_type_t addr_type; // highest priority address type seen for this aircraft
        uint64_t seen;         // Time (millis) at which the last packet was received
        double signalLevel[8]; // Last 8 Signal Amplitudes
        double declination;    // Geomagnetic declination depending on position
        long messages;         // Number of Mode S messages received
        int signalNext;        // next index of signalLevel to use
        int alt_baro;          // Altitude (Baro)
        int alt_baro_reliable;
        int alt_geom;      // Altitude (Geometric)
        int geom_delta;    // Difference between Geometric and Baro altitudes
        int baro_rate;     // Vertical rate (barometric)
        int geom_rate;     // Vertical rate (geometric)
        unsigned distance; // Distance to site in meter.
        unsigned ias;
        unsigned tas;
        unsigned squawk;           // Squawk
        unsigned category;         // Aircraft category A0 - D7 encoded as a single hex byte. 00 = unset
        unsigned nav_altitude_mcp; // FCU/MCP selected altitude
        unsigned nav_altitude_fms; // FMS selected altitude
        unsigned cpr_odd_lat;
        unsigned cpr_odd_lon;
        unsigned cpr_odd_nic;
        unsigned cpr_odd_rc;
        unsigned cpr_even_lat;
        unsigned cpr_even_lon;
        unsigned cpr_even_nic;
        unsigned cpr_even_rc;

        float nav_qnh;     // Altimeter setting (QNH/QFE), millibars
        float nav_heading; // target heading, degrees (0-359)
        float gs;
        float mach;
        float track;        // Ground track
        float track_rate;   // Rate of change of ground track, degrees/second
        float roll;         // Roll angle, degrees right
        float mag_heading;  // Magnetic heading
        float true_heading; // True heading

        uint64_t next_reduce_forward_DF11;
        data_validity callsign_valid;
        data_validity altitude_baro_valid;
        data_validity altitude_geom_valid;
        data_validity geom_delta_valid;
        data_validity gs_valid;
        data_validity ias_valid;
        data_validity tas_valid;
        data_validity mach_valid;
        data_validity track_valid;
        data_validity track_rate_valid;
        data_validity roll_valid;
        data_validity mag_heading_valid;
        data_validity true_heading_valid;
        data_validity baro_rate_valid;
        data_validity geom_rate_valid;
        data_validity nic_a_valid;
        data_validity nic_c_valid;
        data_validity nic_baro_valid;
        data_validity nac_p_valid;
        data_validity nac_v_valid;
        data_validity sil_valid;
        data_validity gva_valid;
        data_validity sda_valid;
        data_validity squawk_valid;
        data_validity emergency_valid;
        data_validity airground_valid;
        data_validity nav_qnh_valid;
        data_validity nav_altitude_mcp_valid;
        data_validity nav_altitude_fms_valid;
        data_validity nav_altitude_src_valid;
        data_validity nav_heading_valid;
        data_validity nav_modes_valid;
        data_validity cpr_odd_valid;  // Last seen even CPR message
        data_validity cpr_even_valid; // Last seen odd CPR message
        data_validity position_valid;
        data_validity alert_valid;
        data_validity spi_valid;

        char callsign[12]; // Flight number

        emergency_t emergency;   // Emergency/priority status
        air_ground_t air_ground; // air/ground status
        // Set of engaged automation modes.
        struct
        {
            bool autopilot;
            bool vnav;
            bool althold;
            bool approach;
            bool lnav;
            bool tcas;
        } nav_modes;
        cpr_type_t cpr_odd_type;
        cpr_type_t cpr_even_type;
        nav_altitude_source_t nav_altitude_src; // source of altitude used by automation

        double lat, lon;      // Coordinates obtained from CPR encoded data
        unsigned nic;         // NIC of last computed position
        unsigned rc;          // Rc of last computed position
        int pos_reliable_odd; // Number of good global CPRs, indicates position reliability
        int pos_reliable_even;
        float gs_last_pos; // Save a groundspeed associated with the last position

        // data extracted from opstatus etc
        int adsb_version;            // ADS-B version (from ADS-B operational status); -1 means no ADS-B messages seen
        int adsr_version;            // As above, for ADS-R messages
        int tisb_version;            // As above, for TIS-B messages
        heading_type_t adsb_hrd;     // Heading Reference Direction setting (from ADS-B operational status)
        heading_type_t adsb_tah;     // Track Angle / Heading setting (from ADS-B operational status)
        heading_type_t heading_type; // Type of indicated heading, mag or true
        unsigned nic_a : 1;          // NIC supplement A from opstatus
        unsigned nic_c : 1;          // NIC supplement C from opstatus
        unsigned nic_baro : 1;       // NIC baro supplement from TSS or opstatus
        unsigned nac_p : 4;          // NACp from TSS or opstatus
        unsigned nac_v : 3;          // NACv from airborne velocity or opstatus
        unsigned sil : 2;            // SIL from TSS or opstatus
        unsigned gva : 2;            // GVA from opstatus
        unsigned sda : 2;            // SDA from opstatus
        unsigned alert : 1;          // FS Flight status alert bit
        unsigned spi : 1;            // FS Flight status SPI (Special Position Identification) bit
        sil_type_t sil_type;         // SIL supplement from TSS or opstatus
        int modeA_hit;               // did our squawk match a possible mode A reply in the last check period?
        int modeC_hit;               // did our altitude match a possible mode C reply in the last check period?

        modes_message_t first_message; // A copy of the first message we received for this aircraft.
        struct aircraft *next;         // Next aircraft in our linked list
    };

    /* Mode A/C tracking is done separately, not via the aircraft list,
     * and via a flat array rather than a list since there are only 4k possible values
     * (nb: we ignore the ident/SPI bit when tracking)
     */
    extern uint32_t modeAC_count[4096];
    extern uint32_t modeAC_match[4096];
    extern uint32_t modeAC_age[4096];

    /* is this bit of data valid? */
    static inline int
    track_data_valid(const data_validity *v)
    {
        return (v->source != SOURCE_INVALID && messageNow() < v->expires);
    }

    /* is this bit of data fresh? */
    static inline int track_data_stale(const data_validity *v)
    {
        return (v->source != SOURCE_INVALID && messageNow() < v->stale);
    }

    /* what's the age of this data, in milliseconds? */
    static inline uint64_t track_data_age(const data_validity *v)
    {
        if (v->source == SOURCE_INVALID)
            return ~(uint64_t)0;
        if (v->updated >= messageNow())
            return 0;
        return (messageNow() - v->updated);
    }

    /* Update aircraft state from data in the provided mesage.
     * Return the tracked aircraft.
     */
    struct modesMessage;
    struct aircraft *track_update_from_message(modes_message_t *mm);

    /* Call periodically */
    void track_periodic_update();

    static inline int
    min(int a, int b)
    {
        if (a < b)
            return a;
        else
            return b;
    }

#ifdef __cplusplus
}
#endif
#endif /* __TRACK_H */