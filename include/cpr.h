#ifndef __CPR_H
#define __CPR_H
#ifdef __cplusplus
extern "C"
{
#endif

    int decode_cpr_airborne(int even_cprlat, int even_cprlon,
                          int odd_cprlat, int odd_cprlon,
                          int fflag,
                          double *out_lat, double *out_lon);

    int decode_cpr_surface(double reflat, double reflon,
                         int even_cprlat, int even_cprlon,
                         int odd_cprlat, int odd_cprlon,
                         int fflag,
                         double *out_lat, double *out_lon);

    int decode_cpr_relative(double reflat, double reflon,
                          int cprlat, int cprlon,
                          int fflag, int surface,
                          double *out_lat, double *out_lon);

#ifdef __cplusplus
}
#endif
#endif /* __CPR_H */