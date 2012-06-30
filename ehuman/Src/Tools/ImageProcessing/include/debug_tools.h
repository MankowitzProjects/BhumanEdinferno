#ifndef DEBUG_TOOLS_H_INCLUDED
#define DEBUG_TOOLS_H_INCLUDED

#include <iostream>

using namespace std;

#define DEBUG_MODE                      0

static inline void prt(const char *msg)
{
    cout << msg;
}

#if (DEBUG_MODE)
static inline void prt_debug(const char *msg)
{
    cout << msg;
}
#else
    #define prt_debug(n)
#endif

#endif // DEBUG_TOOLS_H_INCLUDED
