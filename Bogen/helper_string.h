#pragma once 

char* str_toupper(char *str);
char* str_tolower(char *str);

char* str_first_upper(char *str);

char* str_filename_h(char *filename);
char* str_filename_c(char *filename);
char* str_replace(char *str, char exisitng, char target);
void str_separate(char *str, char *namespace, char *type);