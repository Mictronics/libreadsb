#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <pthread.h>
#include "readsb_def.h"
#include "util.h"

uint64_t _messageNow = 0;

uint64_t mstime(void)
{
    struct timeval tv;
    uint64_t mst;

    gettimeofday(&tv, NULL);
    mst = ((uint64_t)tv.tv_sec) * 1000;
    mst += tv.tv_usec / 1000;
    return mst;
}

int64_t receiveclock_ns_elapsed(uint64_t t1, uint64_t t2)
{
    return (t2 - t1) * 1000U / 12U;
}

int64_t receiveclock_ms_elapsed(uint64_t t1, uint64_t t2)
{
    return (t2 - t1) / 12000U;
}

void normalize_timespec(struct timespec *ts)
{
    if (ts->tv_nsec > 1000000000)
    {
        ts->tv_sec += ts->tv_nsec / 1000000000;
        ts->tv_nsec = ts->tv_nsec % 1000000000;
    }
    else if (ts->tv_nsec < 0)
    {
        long adjust = ts->tv_nsec / 1000000000 + 1;
        ts->tv_sec -= adjust;
        ts->tv_nsec = (ts->tv_nsec + 1000000000 * adjust) % 1000000000;
    }
}

void get_deadline(uint32_t timeout_ms, struct timespec *ts)
{
    clock_gettime(CLOCK_REALTIME, ts);
    ts->tv_nsec += timeout_ms * 1000000;
    normalize_timespec(ts);
}

/* record current CPU time in start_time */
void start_cpu_timing(struct timespec *start_time)
{
    clock_gettime(CLOCK_THREAD_CPUTIME_ID, start_time);
}

/* add difference between start_time and the current CPU time to add_to, return elapsed time */
int64_t end_cpu_timing(const struct timespec *start_time, struct timespec *add_to)
{
    struct timespec end_time;
    clock_gettime(CLOCK_THREAD_CPUTIME_ID, &end_time);
    add_to->tv_sec += end_time.tv_sec - start_time->tv_sec;
    add_to->tv_nsec += end_time.tv_nsec - start_time->tv_nsec;
    normalize_timespec(add_to);
    return ((int64_t)end_time.tv_sec * 1000UL + end_time.tv_nsec / 1000000UL) - ((int64_t)start_time->tv_sec * 1000UL + start_time->tv_nsec / 1000000UL);
}

/* add difference between start_time and the current CPU time to add_to; then store the current CPU time in start_time */
void update_cpu_timing(struct timespec *start_time, struct timespec *add_to)
{
    struct timespec end_time;
    clock_gettime(CLOCK_THREAD_CPUTIME_ID, &end_time);
    add_to->tv_sec += end_time.tv_sec - start_time->tv_sec;
    add_to->tv_nsec += end_time.tv_nsec - start_time->tv_nsec;
    normalize_timespec(add_to);
    *start_time = end_time;
}

/* Set trhead name if supported. */
void set_thread_name(const char *name)
{
#if (__GLIBC__ > 2) || (__GLIBC__ == 2 && __GLIBC_MINOR__ >= 12)
    pthread_setname_np(pthread_self(), name);
#else
    MODES_NOTUSED(name);
#endif
}