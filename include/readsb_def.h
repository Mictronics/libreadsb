#ifndef __READSB_DEF_H
#define __READSB_DEF_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <stdatomic.h>
#include <pthread.h>
#include "stats.h"

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

    /* Where did a bit of data arrive from? In order of increasing priority */
    typedef enum
    {
        SOURCE_INVALID = 0,        /* data is not valid */
        SOURCE_MODE_AC = 1,        /* A/C message */
        SOURCE_MLAT = 2,           /* derived from mlat */
        SOURCE_MODE_S = 3,         /* data from a Mode S message, no full CRC */
        SOURCE_MODE_S_CHECKED = 4, /* data from a Mode S message with full CRC */
        SOURCE_TISB = 5,           /* data from a TIS-B extended squitter message */
        SOURCE_ADSR = 6,           /* data from a ADS-R extended squitter message */
        SOURCE_ADSB = 7,           /* data from a ADS-B extended squitter message */
    } datasource_t;

    typedef enum
    {
        UNIT_FEET,
        UNIT_METERS
    } altitude_unit_t;

    typedef enum
    {
        ALTITUDE_BARO,
        ALTITUDE_GEOM
    } altitude_source_t;

    typedef enum
    {
        CPR_SURFACE,
        CPR_AIRBORNE,
        CPR_COARSE
    } cpr_type_t;

    typedef enum
    {
        HEADING_INVALID,          // Not set
        HEADING_GROUND_TRACK,     // Direction of track over ground, degrees clockwise from true north
        HEADING_TRUE,             // Heading, degrees clockwise from true north
        HEADING_MAGNETIC,         // Heading, degrees clockwise from magnetic north
        HEADING_MAGNETIC_OR_TRUE, // HEADING_MAGNETIC or HEADING_TRUE depending on the HRD bit in opstatus
        HEADING_TRACK_OR_HEADING  // GROUND_TRACK / MAGNETIC / TRUE depending on the TAH bit in opstatus
    } heading_type_t;

    typedef enum
    {
        COMMB_UNKNOWN,
        COMMB_AMBIGUOUS,
        COMMB_EMPTY_RESPONSE,
        COMMB_DATALINK_CAPS,
        COMMB_GICB_CAPS,
        COMMB_AIRCRAFT_IDENT,
        COMMB_ACAS_RA,
        COMMB_VERTICAL_INTENT,
        COMMB_TRACK_TURN,
        COMMB_HEADING_SPEED
    } commb_format_t;

    typedef enum
    {
        NAV_MODE_AUTOPILOT = 1,
        NAV_MODE_VNAV = 2,
        NAV_MODE_ALT_HOLD = 4,
        NAV_MODE_APPROACH = 8,
        NAV_MODE_LNAV = 16,
        NAV_MODE_TCAS = 32
    } nav_modes_t;

    typedef enum
    {
        NAV_ALT_INVALID,
        NAV_ALT_UNKNOWN,
        NAV_ALT_AIRCRAFT,
        NAV_ALT_MCP,
        NAV_ALT_FMS
    } nav_altitude_source_t;

    typedef enum
    {
        SDR_NONE = 0,
        SDR_IFILE,
        SDR_RTLSDR,
        SDR_BLADERF,
        SDR_MICROBLADERF,
        SDR_MODESBEAST,
        SDR_PLUTOSDR,
        SDR_GNS
    } sdr_type_t;

    /*
     * Type of underlying message, one of:
     */
    typedef enum
    {
        ADDR_ADSB_ICAO = 0,      // Mode S or ADS-B, ICAO address, transponder sourced.
        ADDR_ADSB_ICAO_NT = 1,   // ADS-B, ICAO address, non-transponder.
        ADDR_ADSR_ICAO = 2,      // ADS-R, ICAO address.
        ADDR_TISB_ICAO = 3,      // TIS-B, ICAO address.
        ADDR_ADSB_OTHER = 4,     // ADS-B, other address format.
        ADDR_ADSR_OTHER = 5,     // ADS-R, other address format.
        ADDR_TISB_TRACKFILE = 6, // TIS-B, Mode A code + track file number.
        ADDR_TISB_OTHER = 7,     // TIS-B, other address format.
        ADDR_MODE_A = 8,         // Mode A.
        ADDR_UNKNOWN = 9         // Unknown address format.
    } addr_type_t;

    /*
     * ADS-B emergency/priority status, a superset of the 7x00 squawks
     */
    typedef enum
    {
        EMERGENCY_NONE = 0,
        EMERGENCY_GENERAL = 1,
        EMERGENCY_LIFEGUARD = 2,
        EMERGENCY_MINFUEL = 3,
        EMERGENCY_NORDO = 4,
        EMERGENCY_UNLAWFUL = 5,
        EMERGENCY_DOWNED = 6,
        EMERGENCY_RESERVED = 7
    } emergency_t;

    typedef enum
    {
        AG_INVALID = 0,
        AG_GROUND = 1,
        AG_AIRBORNE = 2,
        AG_UNCERTAIN = 3
    } air_ground_t;

    /*
     * Interpretation of SIL: unknown, perhour, persample.
     */
    typedef enum
    {
        SIL_INVALID = 0,
        SIL_UNKNOWN = 1,
        SIL_PER_SAMPLE = 2,
        SIL_PER_HOUR = 3
    } sil_type_t;

    typedef struct
    {
        uint64_t timestampMsg;    // Timestamp of the message (12MHz clock)
        uint64_t sysTimestampMsg; // Timestamp of the message (system time)
        // Generic fields
        unsigned char msg[MODES_LONG_MSG_BYTES];      // Binary message.
        unsigned char verbatim[MODES_LONG_MSG_BYTES]; // Binary message, as originally received before correction
        int msgbits;                                  // Number of bits in message
        int msgtype;                                  // Downlink format #
        uint32_t crc;                                 // Message CRC
        int correctedbits;                            // No. of bits corrected
        uint32_t addr;                                // Address Announced
        addr_type_t addrtype;                         // address format / source
        int remote;                                   // If set this message is from a remote station
        int score;                                    // Scoring from scoreModesMessage, if used
        int sbs_in;                                   // Signifies this message is coming from basestation input
        int reduce_forward;                           // forward this message for reduced beast output
        datasource_t source;                          // Characterizes the overall message source
        double signalLevel;                           // RSSI, in the range [0..1], as a fraction of full-scale power
        // Raw data, just extracted directly from the message
        // The names reflect the field names in Annex 4
        unsigned IID; // extracted from CRC of DF11s
        unsigned AA;
        unsigned AC;
        unsigned CA;
        unsigned CC;
        unsigned CF;
        unsigned DR;
        unsigned FS;
        unsigned ID;
        unsigned KE;
        unsigned ND;
        unsigned RI;
        unsigned SL;
        unsigned UM;
        unsigned VS;
        unsigned metype; // DF17/18 ME type
        unsigned mesub;  // DF17/18 ME subtype

        unsigned char MB[7];
        unsigned char MD[10];
        unsigned char ME[7];
        unsigned char MV[7];

        // Decoded data
        unsigned altitude_baro_valid : 1;
        unsigned altitude_geom_valid : 1;
        unsigned track_valid : 1;
        unsigned track_rate_valid : 1;
        unsigned heading_valid : 1;
        unsigned roll_valid : 1;
        unsigned gs_valid : 1;
        unsigned ias_valid : 1;
        unsigned tas_valid : 1;
        unsigned mach_valid : 1;
        unsigned baro_rate_valid : 1;
        unsigned geom_rate_valid : 1;
        unsigned squawk_valid : 1;
        unsigned callsign_valid : 1;
        unsigned cpr_valid : 1;
        unsigned cpr_odd : 1;
        unsigned cpr_decoded : 1;
        unsigned cpr_relative : 1;
        unsigned category_valid : 1;
        unsigned geom_delta_valid : 1;
        unsigned from_mlat : 1;
        unsigned from_tisb : 1;
        unsigned spi_valid : 1;
        unsigned spi : 1;
        unsigned alert_valid : 1;
        unsigned alert : 1;
        unsigned emergency_valid : 1;
        unsigned padding : 13;

        // valid if altitude_baro_valid:
        int altitude_baro;                  // Altitude in either feet or meters
        altitude_unit_t altitude_baro_unit; // the unit used for altitude

        // valid if altitude_geom_valid:
        int altitude_geom;                  // Altitude in either feet or meters
        altitude_unit_t altitude_geom_unit; // the unit used for altitude

        // following fields are valid if the corresponding _valid field is set:
        int geom_delta;              // Difference between geometric and baro alt
        float heading;               // ground track or heading, degrees (0-359). Reported directly or computed from from EW and NS velocity
        heading_type_t heading_type; // how to interpret 'track_or_heading'
        float track_rate;            // Rate of change of track, degrees/second
        float roll;                  // Roll, degrees, negative is left roll

        struct
        {
            // Groundspeed, kts, reported directly or computed from from EW and NS velocity
            // For surface movement, this has different interpretations for v0 and v2; both
            // fields are populated. The tracking layer will update "gs.selected".
            float v0;
            float v2;
            float selected;
        } gs;
        unsigned ias;          // Indicated airspeed, kts
        unsigned tas;          // True airspeed, kts
        double mach;           // Mach number
        int baro_rate;         // Rate of change of barometric altitude, feet/minute
        int geom_rate;         // Rate of change of geometric (GNSS / INS) altitude, feet/minute
        unsigned squawk;       // 13 bits identity (Squawk), encoded as 4 hex digits
        char callsign[16];     // 8 chars flight number, NUL-terminated
        unsigned category;     // A0 - D7 encoded as a single hex byte
        emergency_t emergency; // emergency/priority status

        // valid if cpr_valid
        cpr_type_t cpr_type; // The encoding type used (surface, airborne, coarse TIS-B)
        unsigned cpr_lat;    // Non decoded latitude.
        unsigned cpr_lon;    // Non decoded longitude.
        unsigned cpr_nucp;   // NUCp/NIC value implied by message type

        air_ground_t airground; // air/ground state

        // valid if cpr_decoded:
        double decoded_lat;
        double decoded_lon;
        unsigned decoded_nic;
        unsigned decoded_rc;

        commb_format_t commb_format; // Inferred format of a comm-b message

        // various integrity/accuracy things

        struct
        {
            unsigned nic_a_valid : 1;
            unsigned nic_b_valid : 1;
            unsigned nic_c_valid : 1;
            unsigned nic_baro_valid : 1;
            unsigned nac_p_valid : 1;
            unsigned nac_v_valid : 1;
            unsigned gva_valid : 1;
            unsigned sda_valid : 1;

            unsigned nic_a : 1;    // if nic_a_valid
            unsigned nic_b : 1;    // if nic_b_valid
            unsigned nic_c : 1;    // if nic_c_valid
            unsigned nic_baro : 1; // if nic_baro_valid

            unsigned nac_p : 4; // if nac_p_valid
            unsigned nac_v : 3; // if nac_v_valid

            unsigned sil : 2; // if sil_type != SIL_INVALID

            unsigned gva : 2; // if gva_valid

            unsigned sda : 2; // if sda_valid
            unsigned padding : 7;
            sil_type_t sil_type;
        } accuracy;

        // Operational Status

        struct
        {
            sil_type_t sil_type;
            heading_type_t tah;
            heading_type_t hrd;

            enum
            {
                ANGLE_HEADING,
                ANGLE_TRACK
            } track_angle;

            unsigned cc_lw;
            unsigned cc_antenna_offset;

            unsigned valid : 1;
            unsigned version : 3;

            unsigned om_acas_ra : 1;
            unsigned om_ident : 1;
            unsigned om_atc : 1;
            unsigned om_saf : 1;

            unsigned cc_acas : 1;
            unsigned cc_cdti : 1;
            unsigned cc_1090_in : 1;
            unsigned cc_arv : 1;
            unsigned cc_ts : 1;
            unsigned cc_tc : 2;
            unsigned cc_uat_in : 1;
            unsigned cc_poa : 1;
            unsigned cc_b2_low : 1;
            unsigned cc_lw_valid : 1;
            unsigned padding : 13;
        } opstatus;

        // combined:
        //   Target State & Status (ADS-B V2 only)
        //   Comm-B BDS4,0 Vertical Intent

        struct
        {
            unsigned fms_altitude; // FMS selected altitude
            unsigned mcp_altitude; // MCP/FCU selected altitude
            float qnh;             // altimeter setting (QFE or QNH/QNE), millibars
            float heading;         // heading, degrees (0-359) (could be magnetic or true heading; magnetic recommended)
            unsigned heading_valid : 1;
            unsigned fms_altitude_valid : 1;
            unsigned mcp_altitude_valid : 1;
            unsigned qnh_valid : 1;
            unsigned modes_valid : 1;
            unsigned padding : 27;
            heading_type_t heading_type;

            nav_altitude_source_t altitude_source;

            nav_modes_t modes;
        } nav;
    } modes_message_t;

    // Library global state
    typedef struct
    {
        unsigned trailing_samples; // extra trailing samples in magnitude buffers
        atomic_int is_exit;        // Exit from the main loop when true
        int beast_fd;              // Local Modes-S Beast handler
        int filter_persistence;    // Maximum number of consecutive implausible positions from global CPR to invalidate a known position.
        double sample_rate;        // actual sample rate in use (in hz)
        int aircraft_history_next;
        int aircraft_history_full;
        int stats_latest_1min;
        int bUserFlags; // Flags relating to the user details
        int8_t biastee;
        struct stats stats_current;
        struct stats stats_alltime;
        struct stats stats_periodic;
        struct stats stats_1min[15];
        struct stats stats_5min;
        struct stats stats_15min;
        struct range_stats stats_range;
        struct timespec reader_cpu_accumulator; // accumulated CPU time used by the reader thread
        struct timespec reader_cpu_start;       // start time for the last reader thread CPU measurement
        pthread_mutex_t reader_cpu_mutex;       // mutex protecting reader_cpu_accumulator
        pthread_t reader_thread;
    } readsb_t;

    extern readsb_t lib_state;

#ifdef __cplusplus
}
#endif
#endif /* __READSB_DEF_H */