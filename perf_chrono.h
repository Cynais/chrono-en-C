#ifndef PERF_MONITOR_H
#define PERF_MONITOR_H

#ifndef PERF_MONITORING
#define PERF_MONITORING 0
#endif

#if PERF_MONITORING

#include <stdint.h>

typedef struct
{
    const char *name;

    uint64_t start_ns;
    uint64_t total_ns;
    uint64_t min_ns;
    uint64_t max_ns;

    unsigned long count;

} perf_timer_t;


/* API */
void perf_start(perf_timer_t *timer);
void perf_stop(perf_timer_t *timer);
void perf_print(const perf_timer_t *timer);


/*
 * Chrono nommé
 *
 * Exemple :
 *
 * PERF_DECLARE(PROCESSING);
 *
 * PERF_BEGIN(PROCESSING);
 * ...
 * PERF_END(PROCESSING);
 *
 * PERF_PRINT(PROCESSING);
 */
#define PERF_DECLARE(site_name) \
    static perf_timer_t perf_##site_name = \
    { #site_name, 0U, 0U, 0U, 0U, 0U }

#define PERF_BEGIN(site_name) \
    perf_start(&perf_##site_name)

#define PERF_END(site_name) \
    perf_stop(&perf_##site_name)

#define PERF_PRINT(site_name) \
    perf_print(&perf_##site_name)


/*
 * Chrono automatique d'une fonction.
 * Le nom vient de __func__.
 *
 * Exemple :
 *
 * void processing_run(void)
 * {
 *     PERF_DECLARE_FUNC();
 *
 *     PERF_BEGIN_FUNC();
 *     ...
 *     PERF_END_FUNC();
 *
 *     PERF_PRINT_FUNC();
 * }
 */
#define PERF_DECLARE_FUNC() \
    static perf_timer_t perf_func = \
    { 0, 0U, 0U, 0U, 0U, 0U }

#define PERF_BEGIN_FUNC() \
    do \
    { \
        perf_func.name = __func__; \
        perf_start(&perf_func); \
    } while (0)

#define PERF_END_FUNC() \
    perf_stop(&perf_func)

#define PERF_PRINT_FUNC() \
    perf_print(&perf_func)


#else /* PERF_MONITORING == 0 */


/*
 * Profiling désactivé.
 *
 * Toutes les macros disparaissent au preprocessing.
 * Aucun timer, aucun appel, aucune donnée PERF.
 */
#define PERF_DECLARE(site_name)
#define PERF_BEGIN(site_name)
#define PERF_END(site_name)
#define PERF_PRINT(site_name)

#define PERF_DECLARE_FUNC()
#define PERF_BEGIN_FUNC()
#define PERF_END_FUNC()
#define PERF_PRINT_FUNC()


#endif /* PERF_MONITORING */

#endif /* PERF_MONITOR_H */