#ifndef PERF_MONITOR_H
#define PERF_MONITOR_H

#ifndef PERF_MONITORING
#define PERF_MONITORING 0
#endif

#if PERF_MONITORING

void perf_begin(const char *name);
void perf_end(const char *name);

void perf_print_all(void);
void perf_save(const char *filename);


/* Chrono nommé */
#define PERF_BEGIN(name) \
    perf_begin(#name)

#define PERF_END(name) \
    perf_end(#name)


/* Chrono de fonction avec __func__ */
#define PERF_BEGIN_FUNC() \
    perf_begin(__func__)

#define PERF_END_FUNC() \
    perf_end(__func__)


/* Résultats */
#define PERF_PRINT_ALL() \
    perf_print_all()

#define PERF_SAVE(filename) \
    perf_save(filename)


#else

/*
 * PERF désactivé :
 * toutes les macros disparaissent.
 */
#define PERF_BEGIN(name)
#define PERF_END(name)

#define PERF_BEGIN_FUNC()
#define PERF_END_FUNC()

#define PERF_PRINT_ALL()
#define PERF_SAVE(filename)

#endif

#endif