#include <stdio.h>
#include <string.h>
#include "util.h"
#include "icao_filter.h"

// hash table size, must be a power of two:
#define ICAO_FILTER_SIZE 4096

// Millis between filter expiry flips:
#define MODES_ICAO_FILTER_TTL 60000

// Open-addressed hash table with linear probing.
// We store each address twice to handle Data/Parity
// which need to match on a partial address (top 16 bits only).

// Maintain two tables and switch between them to age out entries.

static uint32_t icao_filter_a[ICAO_FILTER_SIZE];
static uint32_t icao_filter_b[ICAO_FILTER_SIZE];
static uint32_t *icao_filter_active;

static uint32_t icao_hash(uint32_t a)
{
    // Jenkins one-at-a-time hash, unrolled for 3 bytes
    uint32_t hash = 0;

    hash += a & 0xff;
    hash += hash << 10;
    hash ^= hash >> 6;

    hash += (a >> 8) & 0xff;
    hash += (hash << 10);
    hash ^= (hash >> 6);

    hash += (a >> 16) & 0xff;
    hash += (hash << 10);
    hash ^= (hash >> 6);

    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);

    return hash & (ICAO_FILTER_SIZE - 1);
}

void icao_filter_init()
{
    memset(icao_filter_a, 0, sizeof(icao_filter_a));
    memset(icao_filter_b, 0, sizeof(icao_filter_b));
    icao_filter_active = icao_filter_a;
}

void icao_filter_add(uint32_t addr)
{
    uint32_t h, h0;
    h0 = h = icao_hash(addr);
    while (icao_filter_active[h] && icao_filter_active[h] != addr)
    {
        h = (h + 1) & (ICAO_FILTER_SIZE - 1);
        if (h == h0)
        {
            fprintf(stderr, "ICAO hash table full, increase ICAO_FILTER_SIZE\n");
            return;
        }
    }
    if (!icao_filter_active[h])
        icao_filter_active[h] = addr;

    // also add with a zeroed top byte, for handling DF20/21 with Data Parity
    h0 = h = icao_hash(addr & 0x00ffff);
    while (icao_filter_active[h] && (icao_filter_active[h] & 0x00ffff) != (addr & 0x00ffff))
    {
        h = (h + 1) & (ICAO_FILTER_SIZE - 1);
        if (h == h0)
        {
            fprintf(stderr, "ICAO hash table full, increase ICAO_FILTER_SIZE\n");
            return;
        }
    }
    if (!icao_filter_active[h])
        icao_filter_active[h] = addr;
}

int icao_filter_test(uint32_t addr)
{
    uint32_t h, h0;

    h0 = h = icao_hash(addr);
    while (icao_filter_a[h] && icao_filter_a[h] != addr)
    {
        h = (h + 1) & (ICAO_FILTER_SIZE - 1);
        if (h == h0)
            break;
    }
    if (icao_filter_a[h] == addr)
        return 1;

    h = h0;
    while (icao_filter_b[h] && icao_filter_b[h] != addr)
    {
        h = (h + 1) & (ICAO_FILTER_SIZE - 1);
        if (h == h0)
            break;
    }
    if (icao_filter_b[h] == addr)
        return 1;

    return 0;
}

uint32_t icao_filter_test_fuzzy(uint32_t partial)
{
    uint32_t h, h0;

    partial &= 0x00ffff;
    h0 = h = icao_hash(partial);
    while (icao_filter_a[h] && (icao_filter_a[h] & 0x00ffff) != partial)
    {
        h = (h + 1) & (ICAO_FILTER_SIZE - 1);
        if (h == h0)
            break;
    }
    if ((icao_filter_a[h] & 0x00ffff) == partial)
        return icao_filter_a[h];

    h = h0;
    while (icao_filter_b[h] && (icao_filter_b[h] & 0x00ffff) != partial)
    {
        h = (h + 1) & (ICAO_FILTER_SIZE - 1);
        if (h == h0)
            break;
    }
    if ((icao_filter_b[h] & 0x00ffff) == partial)
        return icao_filter_b[h];

    return 0;
}

// call this periodically:

void icao_filter_expire()
{
    static uint64_t next_flip = 0;
    uint64_t now = mstime();

    if (now >= next_flip)
    {
        if (icao_filter_active == icao_filter_a)
        {
            memset(icao_filter_b, 0, sizeof(icao_filter_b));
            icao_filter_active = icao_filter_b;
        }
        else
        {
            memset(icao_filter_a, 0, sizeof(icao_filter_a));
            icao_filter_active = icao_filter_a;
        }
        next_flip = now + MODES_ICAO_FILTER_TTL;
    }
}
