#ifndef __UTILS__
  #define __UTILS__

  #include <stdint.h>

  #define MAX(x, y) ((x) < (y) ? (y) : (x))

  #define BUFFER_SIZE 255

  typedef struct program_parameters {
    char* patient_records_filename;
    size_t ht_disease_size;
    size_t ht_country_size;
    size_t bucket_size;
  } program_parameters_t;

  extern program_parameters_t parameters;

  /* Converts a given string to 64 bits integer */
  uint8_t string_to_int64(char*, int64_t*);

#endif
