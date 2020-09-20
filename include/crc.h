#ifndef __CRC_H
#define __CRC_H
#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>

// Global max for fixable bit erros
#define MODES_MAX_BITERRORS 2

    struct errorinfo
    {
        uint32_t syndrome;               // CRC syndrome
        int errors;                      // number of errors
        int8_t bit[MODES_MAX_BITERRORS]; // bit positions to fix (-1 = no bit)
    };

    void modes_checksum_init(int fixBits);
    uint32_t modes_checksum(uint8_t *msg, int bitlen);
    struct errorinfo *modes_checksum_diagnose(uint32_t syndrome, int bitlen);
    void modes_checksum_fix(uint8_t *msg, struct errorinfo *info);
    void crc_cleanup_tables(void);

#ifdef __cplusplus
}
#endif
#endif /* __CRC_H */