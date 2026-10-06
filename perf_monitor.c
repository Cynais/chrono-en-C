#include "perf_monitor.h"

#if PERF_MONITORING

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <time.h>


#define PERF_MAX_TIMERS 64U
#define PERF_MAX_DEPTH  16U

#define PERF_INVALID_TIMER (-1)


typedef struct
{
    const char *name;

    uint64_t total_ns;
    uint64_t min_ns;
    uint64_t max_ns;

    unsigned long count;

} perf_timer_t;


typedef struct
{
    int timer;
    uint64_t start_ns;

} perf_stack_entry_t;


/* Registre des chronos */
static perf_timer_t g_timers[PERF_MAX_TIMERS];
static unsigned int g_timer_count = 0U;


/* Pile pour les chronos imbriqués */
static perf_stack_entry_t g_stack[PERF_MAX_DEPTH];
static unsigned int g_depth = 0U;


/*
 * Horloge monotone pour mesurer une durée.
 */
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


/*
 * Recherche un chrono existant.
 */
static int perf_find_timer(const char *name)
{
    unsigned int i;

    for (i = 0U; i < g_timer_count; ++i)
    {
        if (strcmp(g_timers[i].name, name) == 0)
        {
            return (int)i;
        }
    }

    return PERF_INVALID_TIMER;
}


/*
 * Recherche le chrono ou le crée.
 */
static int perf_get_timer(const char *name)
{
    int timer;

    timer = perf_find_timer(name);

    if (timer != PERF_INVALID_TIMER)
    {
        return timer;
    }

    if (g_timer_count >= PERF_MAX_TIMERS)
    {
        return PERF_INVALID_TIMER;
    }

    timer = (int)g_timer_count;

    g_timers[timer].name = name;

    g_timers[timer].total_ns = 0U;
    g_timers[timer].min_ns = 0U;
    g_timers[timer].max_ns = 0U;
    g_timers[timer].count = 0U;

    g_timer_count++;

    return timer;
}


void perf_begin(const char *name)
{
    int timer;

    if (name == NULL)
    {
        return;
    }

    if (g_depth >= PERF_MAX_DEPTH)
    {
        return;
    }

    timer = perf_get_timer(name);

    if (timer == PERF_INVALID_TIMER)
    {
        return;
    }

    g_stack[g_depth].timer = timer;
    g_stack[g_depth].start_ns = perf_get_time_ns();

    g_depth++;
}


void perf_end(const char *name)
{
    perf_stack_entry_t *entry;
    perf_timer_t *timer;

    uint64_t end_ns;
    uint64_t elapsed_ns;

    int timer_index;

    if (name == NULL)
    {
        return;
    }

    if (g_depth == 0U)
    {
        return;
    }

    timer_index = perf_find_timer(name);

    if (timer_index == PERF_INVALID_TIMER)
    {
        return;
    }

    entry = &g_stack[g_depth - 1U];

    /*
     * BEGIN / END doivent correspondre.
     *
     * Exemple correct :
     *
     * BEGIN(A)
     *   BEGIN(B)
     *   END(B)
     * END(A)
     */
    if (entry->timer != timer_index)
    {
        return;
    }

    end_ns = perf_get_time_ns();

    g_depth--;

    if (end_ns < entry->start_ns)
    {
        return;
    }

    elapsed_ns = end_ns - entry->start_ns;

    timer = &g_timers[timer_index];

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


/*
 * Écrit une ligne de statistiques.
 */
static void perf_write_timer(
    FILE *output,
    const perf_timer_t *timer)
{
    unsigned long avg_us;
    unsigned long min_us;
    unsigned long max_us;

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

    fprintf(output,
            "%s count=%lu avg=%lu us min=%lu us max=%lu us\n",
            timer->name,
            timer->count,
            avg_us,
            min_us,
            max_us);
}


void perf_print_all(void)
{
    unsigned int i;

    printf("\nPERFORMANCE REPORT\n");

    for (i = 0U; i < g_timer_count; ++i)
    {
        perf_write_timer(
            stdout,
            &g_timers[i]);
    }
}


void perf_save(const char *filename)
{
    FILE *file;
    unsigned int i;

    if (filename == NULL)
    {
        return;
    }

    file = fopen(filename, "w");

    if (file == NULL)
    {
        return;
    }

    fprintf(file, "PERFORMANCE REPORT\n");

    for (i = 0U; i < g_timer_count; ++i)
    {
        perf_write_timer(
            file,
            &g_timers[i]);
    }

    fclose(file);
}

#endif