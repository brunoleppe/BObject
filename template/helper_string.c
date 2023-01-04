#include "helper_string.h"
#include <string.h>
#include <ctype.h>


char* str_toupper(char *str){
    char *aux = str;
    while(*str){
        *str = toupper(*str);
        str++;
    }
    return aux;
}

char* str_tolower(char *str){
    char *aux = str;
    while(*str){
        *str = tolower(*str);
        str++;
    }
    return aux;
}

char* str_first_upper(char *str)
{
    str[0] = toupper(str[0]);
    return str;
}

char* str_replace(char *str, char exisitng, char target)
{
    char *aux = str;
    while(*str){
        if(*str == exisitng)
            *str = target;
        str++;
    }
    return aux;
}

void str_separate(char *str, char *namespace, char *type)
{
    char *aux = namespace;
    *aux++ = *str++;
    while(*str){
        if(isupper(*str))
            aux = type;
        *aux++ = *str++;
    }
}