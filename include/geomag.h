#ifndef __GEOMAG_H
#define __GEOMAG_H
#ifdef __cplusplus
extern "C"
{
#endif

    int geomag_init();
    int geomag_calc(double alt, double lat, double lon, double decimal_year, double *dec, double *dip, double *ti, double *gv);

#ifdef __cplusplus
}
#endif
#endif /* __GEOMAG_H */