#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../includes/macros.h"
#include "../includes/utils.h"

uint8_t string_to_int64(char* value, int64_t* value_out) {
    char* valid;
    *value_out = strtoll(value, &valid, 10);
    if (*valid != '\0') return 0;
    return 1;
}

size_t hash_uint(void* key) {
  unsigned int x = atoi(key);
  x = ((x >> 16) ^ x) * 0x45d9f3b;
  x = ((x >> 16) ^ x) * 0x45d9f3b;
  x = (x >> 16) ^ x;
  return x;
}

size_t hash_string(void* value) {
  // djb2 hash function
  size_t hash = 5381;
  for (char* s = value; *s != '\0'; s++) {
    hash = (hash << 5) + hash + *s;
  }
  return hash;
}

void print_string(FILE* out, void* v) {
  fprintf(out, "%s\n", (char*) v);
}

int compare_string(void* v, void* w) {
  char* str1 = (char*) v;
  char* str2 = (char*) w;
  return strcmp(str1, str2);
}

void destroy_string(void *v) {
  char* s = (char *)v;
  free(s);
}

/* DD-MM-YYYY format */
int is_valid_date(char* date) {
  if (strlen(date) != 10) {
    return 0;
  } else {
    for (size_t i = 0; i < strlen(date); ++i) {
      if (i == 2 || i == 5) {
        if (date[i] != '-')
          return 0;
      } else {
        if (!isdigit(date[i]))
          return 0;
      }
    }
  }
  return 1;
}
/* Unspecified '-' */
int is_unspecified_date(char* date) {
  if ((strlen(date) == 1) && (date[0] == '-')) {
    return 1;
  } else {
    return 0;
  }
}

int is_alpharithmetic(char* str) {
  for (size_t i = 0; i < strlen(str); ++i) {
    if (!isalnum(str[i]))
      return 0;
  }
  return 1;
}

int is_alphabetical(char* str) {
  for (size_t i = 0; i < strlen(str); ++i) {
    if (!isalpha(str[i]))
      return 0;
  }
  return 1;
}

int is_number(char* str) {
  for (size_t i = 0; i < strlen(str); ++i) {
    if (!isdigit(str[i]))
      return 0;
  }
  return 1;
}

int64_t compare_date(char* date1, char* date2) {
  // Convert date1 string to struct tm
  struct tm date1_tm;
  memset(&date1_tm, 0, sizeof(struct tm));
  strptime(date1, "%d-%m-%Y", &date1_tm);
  // Convert date2 string to struct tm
  struct tm date2_tm;
  memset(&date2_tm, 0, sizeof(struct tm));
  strptime(date2, "%d-%m-%Y", &date2_tm);
  // Convert date1_tm back to seconds
  char date1_buf[11];
  strftime(date1_buf, sizeof(date1_buf), "%s", &date1_tm);
  size_t date1_to_secs;
  string_to_int64(date1_buf, (int64_t *) &date1_to_secs);
  // Convert date2_tm back to seconds
  char date2_buf[11];
  strftime(date2_buf, sizeof(date2_buf), "%s", &date2_tm);
  size_t date2_to_secs;
  string_to_int64(date2_buf, (int64_t *) &date2_to_secs);
  // Return the difference
  return date1_to_secs - date2_to_secs;
}
