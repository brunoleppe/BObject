#include "debug.h"
#include <stdarg.h>

char *colors[] = {
    "\033[0;34m",
    "\033[0;32m",
    "\033[0;31m"
};

int debug_print(DebugColors color, char *fmt, ...)
{
    printf("%s",colors[color]);

    va_list list;
    va_start(list,fmt);
    int res = vprintf(fmt,list);
    printf("\033[0m"); 

    return res;
}