#include "perf_monitor.h"

PERF_DECLARE(PROCESSING);
PERF_DECLARE(FILTER);

void processing_run(void)
{
    PERF_BEGIN(PROCESSING);

    /*
     * Traitement
     */

    PERF_BEGIN(FILTER);

    /*
     * Filtrage
     */

    PERF_END(FILTER);

    PERF_END(PROCESSING);
}

void processing_print_perf(void)
{
    PERF_PRINT(PROCESSING);
    PERF_PRINT(FILTER);
}