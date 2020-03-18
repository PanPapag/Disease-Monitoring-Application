#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../includes/macros.h"
#include "../includes/utils.h"

uint8_t string_to_int64(char* value, int64_t* value_out) {
    char* valid;
    *value_out = strtoll(value, &valid, 10);
    if (*valid != '\0') return 0;
    return 1;
}

size_t hash_uint(void* key, size_t no_buckets) {
  unsigned int x = atoi(key);
  x = ((x >> 16) ^ x) * 0x45d9f3b;
  x = ((x >> 16) ^ x) * 0x45d9f3b;
  x = (x >> 16) ^ x;
  return x % no_buckets;
}

size_t hash_string(void* value, size_t no_buckets) {
  char* str = *((char**) value);
  // djb2 hash function
  size_t hash = 5381;
  for (char* s = str; *s != '\0'; s++)
	  hash = (hash << 5) + hash + *s;
  return hash % no_buckets;
}

void print_string(void* v) {
  printf("%s\n", *((char**) v));
}

int compare_string(void* v, void* w) {
  char* str1 = *((char**) v);
  char* str2 = *((char**) w);
  return strcmp(str1, str2);
}

void tokenize_string(char* value, char delimiter[], char **res) {
  int i = 0;
  char* token = strtok(value, " ");
  while (token != NULL) {
    res[i++] = token;
    token = strtok(NULL, " ");
  }
}

size_t count_tokens(char* value, char delimiter[]) {
  size_t cnt = 0;
  char* token = strtok(value, " ");
  while (token != NULL) {
    cnt++;
    token = strtok(NULL, " ");
  }
  return cnt;
}
