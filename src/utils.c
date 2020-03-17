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

size_t hash_uint(unsigned int x, size_t no_buckets) {
  x = ((x >> 16) ^ x) * 0x45d9f3b;
  x = ((x >> 16) ^ x) * 0x45d9f3b;
  x = (x >> 16) ^ x;
  return x % no_buckets;
}

size_t hash_string(char* value, size_t no_buckets) {
  // djb2 hash function
  size_t hash = 5381;
  for (char* s = value; *s != '\0'; s++)
	  hash = (hash << 5) + hash + *s;
  return hash % no_buckets;
}

void print_string(void* v) {
  char* s = *((char *) v);
  printf("%s\n",s);
}
