#include "perf_monitor.h"
#include "processing.h"


int main(void)
{
    unsigned int i;

    PERF_BEGIN_FUNC();

    for (i = 0U; i < 100U; ++i)
    {
        processing_run();
    }

    PERF_END_FUNC();


    /*
     * Affichage console
     */
    PERF_PRINT_ALL();


    /*
     * Écriture fichier
     */
    PERF_SAVE("perf_results.txt");


    return 0;
}