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
        uint32_t addr;       // The 24-bit ICAO identifier of the aircraft, as 6 hex digits. The identifier may start with '~', this means that the address is a non-ICAO address (e.g. from TIS-B).
        char flight_id[12];  // Flight ID, the flight name or aircraft registration as 8 chars.
        uint32_t squawk;     // Mode A code (Squawk), encoded as 4 octal digits.
        uint32_t category;   // Emitter category to identify particular aircraft or vehicle classes (values A0 - D7).
        int32_t alt_baro;    // The aircraft barometric altitude in feet.
        int32_t mag_heading; // Heading, degrees clockwise from magnetic north.
        uint32_t ias;        // Indicated air speed in knots.
        double lat;          // Aircraft position latitude in decimal degrees.
        double lon;          // Aircraft position longitude in decimal degrees.
        uint64_t messages;   // Total number of Mode S messages received from this aircraft.
        uint64_t seen_ms;    // When a message was last received from this aircraft. (in milliseconds!!!)
        float rssi;          // Recent average RSSI (signal power), in dbFS; this will always be negative.
        uint32_t distance;   // Distance to site in meter.
        air_ground_t air_ground;
        int32_t alt_geom;         // Geometric (GNSS / INS) altitude in feet referenced to the WGS84 ellipsoid.
        int32_t baro_rate;        // Rate of change of barometric altitude, feet/minute.
        int32_t geom_rate;        // Rate of change of geometric (GNSS / INS) altitude. feet/minute
        uint32_t gs;              // Ground speed in knots.
        uint32_t tas;             // True air speed in knots.
        float mach;               // Mach number.
        int32_t true_heading;     // Heading, degrees clockwise from true north.
        int32_t track;            // True track over ground in degrees (0-359).
        float track_rate;         // Rate of change of track, degrees/second.
        float roll;               // Roll, degrees, negative is left roll.
        float nav_qnh;            // Navigation Accuracy for Velocity.
        int32_t nav_altitude_mcp; // Selected altitude from the Mode Control Panel / Flight Control Unit (MCP/FCU) or equivalent equipment.
        int32_t nav_altitude_fms; // Selected altitude from the Flight Management System (FMS).
        int32_t nav_heading;      // Selected heading (True or Magnetic is not defined in DO-260B, mostly Magnetic as that is the de facto standard).
        uint32_t nic;             // Navigation Integrity Category.
        uint32_t rc;              // Radius of Containment, meters; a measure of position integrity derived from NIC & supplementary bits.
        int32_t version;          // ADS-B Version Number 0, 1, 2 (3-7 are reserved)
        uint32_t nic_baro;        // Navigation Integrity Category for Barometric Altitude
        uint32_t nac_p;           // Navigation Accuracy for Position
        uint32_t nac_v;           // Navigation Accuracy for Velocity
        uint32_t sil;             // Source Integity Level
        uint32_t seen_pos;        // How long ago (in seconds before "now") the position was last updated.
        bool alert;               // Flight status alert bit.
        bool spi;                 // Flight status special position identification bit.
        uint32_t gva;             // Geometric Vertical Accuracy
        uint32_t sda;             // System Design Assurance
        double declination;       // Geomagnetic declination depending on position
        uint32_t wind_speed;      // Calculated wind speed
        uint32_t wind_direction;  // Calculated wind direction
        addr_type_t addr_type;
        emergency_t emergency;
        sil_type_t sil_type;

        struct
        {
            bool autopilot;
            bool vnav;
            bool althold;
            bool approach;
            bool lnav;
            bool tcas;
        } nav_modes;

        struct
        {
            uint32_t callsign;
            uint32_t altitude;
            uint32_t alt_geom;
            uint32_t gs;
            uint32_t ias;
            uint32_t tas;
            uint32_t mach;
            uint32_t track;
            uint32_t track_rate;
            uint32_t roll;
            uint32_t mag_heading;
            uint32_t true_heading;
            uint32_t baro_rate;
            uint32_t geom_rate;
            uint32_t squawk;
            uint32_t emergency;
            uint32_t nav_qnh;
            uint32_t nav_altitude_mcp;
            uint32_t nav_altitude_fms;
            uint32_t nav_heading;
            uint32_t nav_modes;
            uint32_t lat;
            uint32_t lon;
            uint32_t nic;
            uint32_t rc;
            uint32_t nic_baro;
            uint32_t nac_p;
            uint32_t nac_v;
            uint32_t sil;
            uint32_t sil_type;
            uint32_t gva;
            uint32_t sda;
            uint32_t wind;
        } valid_source;
        // Remaining variables are all readsb internal use.
        double signalLevel[8]; // Last 8 Signal Amplitudes
        int signalNext;        // next index of signalLevel to use
        int altitude_baro_reliable;
        int geom_delta; // Difference between Geometric and Baro altitudes
        unsigned cpr_odd_lat;
        unsigned cpr_odd_lon;
        unsigned cpr_odd_nic;
        unsigned cpr_odd_rc;
        unsigned cpr_even_lat;
        unsigned cpr_even_lon;
        unsigned cpr_even_nic;
        unsigned cpr_even_rc;
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

        cpr_type_t cpr_odd_type;
        cpr_type_t cpr_even_type;
        nav_altitude_source_t nav_altitude_src; // source of altitude used by automation
        int pos_reliable_odd;                   // Number of good global CPRs, indicates position reliability
        int pos_reliable_even;
        float gs_last_pos; // Save a groundspeed associated with the last position
        // data extracted from opstatus etc
        int adsb_version;              // ADS-B version (from ADS-B operational status); -1 means no ADS-B messages seen
        int adsr_version;              // As above, for ADS-R messages
        int tisb_version;              // As above, for TIS-B messages
        heading_type_t adsb_hrd;       // Heading Reference Direction setting (from ADS-B operational status)
        heading_type_t adsb_tah;       // Track Angle / Heading setting (from ADS-B operational status)
        heading_type_t heading_type;   // Type of indicated heading, mag or true
        unsigned nic_a : 1;            // NIC supplement A from opstatus
        unsigned nic_c : 1;            // NIC supplement C from opstatus
        int modeA_hit;                 // did our squawk match a possible mode A reply in the last check period?
        int modeC_hit;                 // did our altitude match a possible mode C reply in the last check period?
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