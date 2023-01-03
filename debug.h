#ifndef DEBUG_H
#define DEBUG_H

#include <stdio.h>

#ifdef __DEBUG__
#define DEBUG_PRINT(...)        debug_print(DEBUG,__VA_ARGS__)     
#define INFO_PRINT(...)         debug_print(INFO,__VA_ARGS__)
#define ERROR_PINRT(...)         debug_print(ERROR,__VA_ARGS__)
#else
#define DEBUG_PRINT(...)
#define INFO_PRINT(...)
#define ERROR_PINRT(...)
#endif

typedef enum DebugColors{
    INFO,
    DEBUG,
    ERROR,
}DebugColors;

int debug_print(DebugColors color, char *fmt, ...);

#endif /*DEBUG_H*/