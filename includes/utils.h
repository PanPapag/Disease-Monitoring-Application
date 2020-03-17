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
  size_t hash_uint(unsigned int, size_t);
  /* Hash Function for strings */
  size_t hash_string(char*, size_t);

  /* Prints a given string */
  void print_string(void*);

  /* Tokenizes a string given a delimiter and returns the tokens to an array */
  void tokenize_string(char*, char[], char**);
  /* Returns the number of tokens of a string given its delimiter */
  size_t count_tokens(char*, char[]);

#endif
