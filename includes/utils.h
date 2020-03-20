#ifndef __UTILS__
  #define __UTILS__

  #include <stdint.h>

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

  /* Hash Function for strings */
  size_t hash_string(void*);
  /* Prints a given string */
  void print_string(void*, FILE* out);
  /* Compares two given strings */
  int compare_string(void*, void*);
  /* Free a pointer to char */
  void destroy_string(void*);

  /* Checks if a date string format is valid */
  int is_valid_date(char*);
  /* Checks if a date string format is unspecified aka '-' */
  int is_unspecified_date(char*);
  /* Checks if a string contains only letters and number */
  int is_alpharithmetic(char*);
  /* Checks if a string contains only letters */
  int is_alphabetical(char*);
  /* Checks if a string is number */
  int is_number(char*);

  /*
  Compares two given date formated strings.
  Returns:
    - 0 if dates are equal.
    - > 0 if date1 is after of date2
    - < 0 if date1 is previous of date2
  */
  int64_t compare_date(char*, char*);

  /* Given an array of strings it returns the same array without the first string */
  char** prune_command_name(char**, size_t);

#endif
