#ifndef UTIL_STRING_H
#define UTIL_STRING_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


char* str_slice(char *str, int start_index, int end_index);
char* str_cat(char *str_a, char *str_b);
char** str_split(char *str, char delim, size_t *outLen);
char* str_replace(char *str, char *replace_str);

#endif // UTIL_STRING_H