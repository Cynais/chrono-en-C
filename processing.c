#include "processing.h"
#include "perf_monitor.h"


static void filter_run(void)
{
    volatile unsigned long i;

    PERF_BEGIN_FUNC();

    for (i = 0U; i < 100000U; ++i)
    {
        /* traitement */
    }

    PERF_END_FUNC();
}


void processing_run(void)
{
    volatile unsigned long i;

    /*
     * Chrono automatiquement nommé "processing_run"
     */
    PERF_BEGIN_FUNC();


    for (i = 0U; i < 50000U; ++i)
    {
        /* traitement */
    }


    /*
     * Chrono ponctuel nommé MY_ZONE
     */
    PERF_BEGIN(MY_ZONE);

    filter_run();

    PERF_END(MY_ZONE);


    for (i = 0U; i < 50000U; ++i)
    {
        /* traitement */
    }


    PERF_END_FUNC();
}