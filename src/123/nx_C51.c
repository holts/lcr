#include "nx_c51.h"

void DelayCycles(BYTE n)
{
    while(--n);
}

void Delay1(BYTE n)
{
    while(n--)
    {
        DelayCycles(255);
        DelayCycles(100);
    }
}
