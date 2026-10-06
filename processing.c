#include "processing.h"
#include "perf_monitor.h"

PERF_DECLARE(PROCESSING);
PERF_DECLARE(FILTER);


static void filter_run(void)
{
    volatile unsigned long i;

    PERF_BEGIN(FILTER);

    /*
     * Simulation d'un traitement.
     * Remplacer par ton vrai code.
     */
    for (i = 0U; i < 100000U; ++i)
    {
        /* traitement */
    }

    PERF_END(FILTER);
}


void processing_run(void)
{
    volatile unsigned long i;

    PERF_BEGIN(PROCESSING);

    /*
     * Premier traitement
     */
    for (i = 0U; i < 50000U; ++i)
    {
        /* traitement */
    }

    filter_run();

    /*
     * Deuxième traitement
     */
    for (i = 0U; i < 50000U; ++i)
    {
        /* traitement */
    }

    PERF_END(PROCESSING);
}


void processing_print_perf(void)
{
    PERF_PRINT(PROCESSING);
    PERF_PRINT(FILTER);
}