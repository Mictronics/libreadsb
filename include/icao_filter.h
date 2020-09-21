#ifndef __ICAO_FILTER_H
#define __ICAO_FILTER_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>

    // Call once:
    void icao_filter_init();

    // Add an address to the filter
    void icao_filter_add(uint32_t addr);

    // Test if the given address matches the filter
    int icao_filter_test(uint32_t addr);

    // Test if the top 16 bits match any previously added address.
    // If they do, returns an arbitrary one of the matched
    // addresses. Returns 0 on failure.
    uint32_t icao_filter_test_fuzzy(uint32_t partial);

    // Call this periodically to allow the filter to expire
    // old entries.
    void icao_filter_expire();

#ifdef __cplusplus
}
#endif
#endif /* __ICAO_FILTER_H */