#ifndef DEBUG_H
#define DEBUG_H

#include <stdio.h>

#define DEBUG_PRINT(...)        debug_print(DEBUG,__VA_ARGS__)     
#define INFO_PRINT(...)         debug_print(INFO,__VA_ARGS__)
#define ERROR_PINRT(...)         debug_print(ERROR,__VA_ARGS__)

typedef enum DebugColors{
    INFO,
    DEBUG,
    ERROR,
}DebugColors;

int debug_print(DebugColors color, char *fmt, ...);

#endif /*DEBUG_H*/