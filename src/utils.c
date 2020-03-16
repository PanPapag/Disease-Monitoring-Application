#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../includes/utils.h"

uint8_t string_to_int64(char* str, int64_t* value_out) {
    char* valid;
    *value_out = strtoll(str, &valid, 10);
    if (*valid != '\0') return 0;
    return 1;
}
