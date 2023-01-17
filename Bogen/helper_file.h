#pragma once

#include <stdbool.h>

void file_header(char *namespace, char *type, char *parent, bool derivable);
void file_source(char *namespace, char *type, char *parent, bool private, bool implements, char *interfaces[], int interface_count);
void interface_header(char *namespace, char *type, char *parent);
void interface_source(char *namespace, char *type, char *parent);