#ifndef __DEMOD_2400_H
#define __DEMOD_2400_H
#ifdef __cplusplus
extern "C"
{
#endif

    struct mag_buf;

    void demodulate_2400(struct mag_buf *mag);
    void demodulate_2400_ac(struct mag_buf *mag);

#ifdef __cplusplus
}
#endif
#endif /* __DEMOD_2400_H */