#include "perf_monitor.h"

#if PERF_MONITORING

#include <stdio.h>
#include <time.h>


static uint64_t perf_get_time_ns(void)
{
    struct timespec ts;

    if (clock_gettime(CLOCK_MONOTONIC, &ts) != 0)
    {
        return 0U;
    }

    return ((uint64_t)ts.tv_sec * 1000000000ULL)
         + (uint64_t)ts.tv_nsec;
}


void perf_start(perf_timer_t *timer)
{
    if (timer == NULL)
    {
        return;
    }

    timer->start_ns = perf_get_time_ns();
}


void perf_stop(perf_timer_t *timer)
{
    uint64_t end_ns;
    uint64_t elapsed_ns;

    if (timer == NULL)
    {
        return;
    }

    end_ns = perf_get_time_ns();

    if (end_ns < timer->start_ns)
    {
        return;
    }

    elapsed_ns = end_ns - timer->start_ns;

    timer->total_ns += elapsed_ns;
    timer->count++;

    if ((timer->count == 1U) ||
        (elapsed_ns < timer->min_ns))
    {
        timer->min_ns = elapsed_ns;
    }

    if (elapsed_ns > timer->max_ns)
    {
        timer->max_ns = elapsed_ns;
    }
}


void perf_print(const perf_timer_t *timer)
{
    unsigned long avg_us;
    unsigned long min_us;
    unsigned long max_us;

    if (timer == NULL)
    {
        return;
    }

    if (timer->count == 0U)
    {
        return;
    }

    avg_us = (unsigned long)
        ((timer->total_ns / timer->count) / 1000U);

    min_us = (unsigned long)
        (timer->min_ns / 1000U);

    max_us = (unsigned long)
        (timer->max_ns / 1000U);

    printf("%s count=%lu avg=%lu us min=%lu us max=%lu us\n",
           timer->name,
           timer->count,
           avg_us,
           min_us,
           max_us);
}

#endif /* PERF_MONITORING */