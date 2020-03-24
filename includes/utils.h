#ifndef __UTILS__
  #define __UTILS__

  #include <stdint.h>
  #include <time.h>

  typedef struct program_parameters {
    char* patient_records_filename;
    size_t ht_disease_size;
    size_t ht_country_size;
    size_t bucket_size;
  } program_parameters_t;

  extern program_parameters_t parameters;

  /* Converts a given string to 64 bits integer */
  uint8_t string_to_int64(char*, int64_t*);

  /* Hash Function for unsigned integers */
  size_t hash_uint(void*);
  /* Returns an integer in a new memory address with the given value */
  int* create_int(int);
  /* Prints a given int* */
  void print_int(void*, FILE* out);
  /* Compares two given integers */
  int compare_int(void*, void*);
  /* Free a pointer to int */
  void destroy_int(void*);

  /* Hash Function for strings */
  size_t hash_string(void*);
  /* Prints a given string */
  void print_string(void*, FILE* out);
  /* Compares two given strings */
  int compare_string(void*, void*);
  /* Free a pointer to char */
  void destroy_string(void*);

  /* Checks if a string contains only letters and number */
  int is_alpharithmetic(char*);
  /* Checks if a string contains only letters */
  int is_alphabetical(char*);
  /* Checks if a string is number */
  int is_number(char*);

  /* Checks if a date string format is valid */
  int is_valid_date(char*);
  /* Checks if a date string format is unspecified aka '-' */
  int is_unspecified_date_string(char*);
  /* Checks if a date in struct tm format is unspecified aka 00-01-1900 */
  int is_unspecified_date_tm(struct tm);
  /*
  Compares two given dates formated strings.
  Returns:
    - 0 if dates are equal.
    - > 0 if date1 is after of date2
    - < 0 if date1 is previous of date2
  */
  int64_t compare_date_strings(char*, char*);
  /* Compares two given dates in struct tm format */
  int64_t compare_date_tm(struct tm, struct tm);

  /* Given an array of strings it returns the same array without the first string */
  char** prune_command_name(char**, size_t);

  /* Compare function for country_stats structures */
  int compare_country_stats(void*, void*);
  /* Compare function for country_stats structures */
  int compare_disease_stats(void*, void*);

#endif
