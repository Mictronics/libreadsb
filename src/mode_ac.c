#include <assert.h>
#include "readsb_def.h"
#include "mode_ac.h"

// Input format is : 00:A4:A2:A1:00:B4:B2:B1:00:C4:C2:C1:00:D4:D2:D1

static int mode_a_to_c_lut[4096];
static unsigned mode_c_to_a_lut[4096];

/* Convert from a (hex) mode A value to a 0-4095 index */
static inline unsigned mode_a_to_index(unsigned mode_a)
{
    return (mode_a & 0x0007) | ((mode_a & 0x0070) >> 1) | ((mode_a & 0x0700) >> 2) | ((mode_a & 0x7000) >> 3);
}

/* Convert from a 0-4095 index to a (hex) mode A value */
static inline unsigned index_to_mode_a(unsigned index)
{
    return (index & 0007) | ((index & 0070) << 1) | ((index & 0700) << 2) | ((index & 07000) << 3);
}

static int internal_mode_a_to_mode_c(unsigned int mode_a)
{
    unsigned int FiveHundreds = 0;
    unsigned int OneHundreds = 0;

    if ((mode_a & 0xFFFF8889) != 0 || // check zero bits are zero, D1 set is illegal
        (mode_a & 0x000000F0) == 0)
    { // C1,,C4 cannot be Zero
        return INVALID_ALTITUDE;
    }

    if (mode_a & 0x0010)
    {
        OneHundreds ^= 0x007;
    } // C1
    if (mode_a & 0x0020)
    {
        OneHundreds ^= 0x003;
    } // C2
    if (mode_a & 0x0040)
    {
        OneHundreds ^= 0x001;
    } // C4

    // Remove 7s from OneHundreds (Make 7->5, snd 5->7).
    if ((OneHundreds & 5) == 5)
    {
        OneHundreds ^= 2;
    }

    // Check for invalid codes, only 1 to 5 are valid
    if (OneHundreds > 5)
    {
        return INVALID_ALTITUDE;
    }

    //if (ModeA & 0x0001) {FiveHundreds ^= 0x1FF;} // D1 never used for altitude
    if (mode_a & 0x0002)
    {
        FiveHundreds ^= 0x0FF;
    } // D2
    if (mode_a & 0x0004)
    {
        FiveHundreds ^= 0x07F;
    } // D4

    if (mode_a & 0x1000)
    {
        FiveHundreds ^= 0x03F;
    } // A1
    if (mode_a & 0x2000)
    {
        FiveHundreds ^= 0x01F;
    } // A2
    if (mode_a & 0x4000)
    {
        FiveHundreds ^= 0x00F;
    } // A4

    if (mode_a & 0x0100)
    {
        FiveHundreds ^= 0x007;
    } // B1
    if (mode_a & 0x0200)
    {
        FiveHundreds ^= 0x003;
    } // B2
    if (mode_a & 0x0400)
    {
        FiveHundreds ^= 0x001;
    } // B4

    // Correct order of OneHundreds.
    if (FiveHundreds & 1)
    {
        OneHundreds = 6 - OneHundreds;
    }

    return ((FiveHundreds * 5) + OneHundreds - 13);
}

void mode_ac_init()
{
    for (unsigned i = 0; i < 4096; ++i)
    {
        unsigned mode_a = index_to_mode_a(i);
        int mode_c = internal_mode_a_to_mode_c(mode_a);
        mode_a_to_c_lut[i] = mode_c;

        mode_c += 13;
        if (mode_c >= 0 && mode_c < 4096)
        {
            assert(mode_c_to_a_lut[mode_c] == 0);
            mode_c_to_a_lut[mode_c] = mode_a;
        }
    }
}

/* Given a mode A value (hex-encoded, see above),
 * return the mode C value (signed multiple of 100s of feet)
 * or INVALID_ALITITUDE if not a valid mode C value.
 */
int mode_a_to_mode_c(unsigned mode_a)
{
    unsigned i = mode_a_to_index(mode_a);
    if (i >= 4096)
        return INVALID_ALTITUDE;

    return mode_a_to_c_lut[i];
}

/* Given a mode C value (signed multiple of 100s of feet),
 * return the mode A value, or 0 if not a valid mode C value.
 */
unsigned mode_c_to_mode_a(int mode_c)
{
    mode_c += 13;
    if (mode_c < 0 || mode_c >= 4096)
        return 0;

    return mode_c_to_a_lut[mode_c];
}

void decode_mode_a_message(struct modesMessage *mm, int ModeA)
{
    mm->source = SOURCE_MODE_AC;
    mm->addrtype = ADDR_MODE_A;
    mm->msgtype = 32; // Valid Mode S DF's are DF-00 to DF-31.
    // so use 32 to indicate Mode A/C

    mm->msgbits = 16; // Fudge up a Mode S style data stream
    mm->msg[0] = mm->verbatim[0] = (ModeA >> 8);
    mm->msg[1] = mm->verbatim[1] = (ModeA);

    // Fudge an address based on Mode A (remove the Ident bit)
    mm->addr = (ModeA & 0x0000FF7F) | MODES_NON_ICAO_ADDRESS;

    // Set the Identity field to ModeA
    mm->squawk = ModeA & 0x7777;
    mm->squawk_valid = 1;

    // Flag ident in flight status
    mm->spi = (ModeA & 0x0080) ? 1 : 0;
    mm->spi_valid = 1;

    // Decode an altitude if this looks like a possible mode C
    if (!mm->spi)
    {
        int mode_c = mode_a_to_mode_c(ModeA);
        if (mode_c != INVALID_ALTITUDE)
        {
            mm->altitude_baro = mode_c * 100;
            mm->altitude_baro_unit = UNIT_FEET;
            mm->altitude_baro_valid = 1;
        }
    }

    // Not much else we can tell from a Mode A/C reply.
    // Just fudge up a few bits to keep other code happy
    mm->correctedbits = 0;
}