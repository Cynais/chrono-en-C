#include "processing.h"

int main(void)
{
    unsigned int i;

    for (i = 0U; i < 100U; ++i)
    {
        processing_run();
    }

    processing_print_perf();

    return 0;
}