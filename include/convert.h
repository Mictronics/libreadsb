#ifndef __CONVERT_H
#define __CONVERT_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>

    struct converter_state;

    typedef enum
    {
        INPUT_UC8 = 0,
        INPUT_SC16,
        INPUT_SC16Q11
    } input_format_t;

    typedef void (*iq_convert_fn)(void *iq_data,
                                  uint16_t *mag_data,
                                  unsigned nsamples,
                                  struct converter_state *state,
                                  double *out_mean_level,
                                  double *out_mean_power);

    iq_convert_fn init_converter(input_format_t format,
                                 double sample_rate,
                                 int filter_dc,
                                 struct converter_state **out_state);

    void cleanup_converter(struct converter_state *state);

#ifdef __cplusplus
}
#endif
#endif /* __CONVERT_H */