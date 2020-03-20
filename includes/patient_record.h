#ifndef __PATIENT_RECORD__
  #define __PATIENT_RECORD__

  #include <stdint.h>
  #include <time.h>

  #define NO_PATIENT_RECORD_TOKENS 7
  #define PATIENT_RECORD_DELIMITER " "
  #define EXIT_DATE_NOT_SPECIFIED "00-01-1900"

  enum patient_record_error_codes {
    VALID_PATIENT_RECORD,
    INVALID_RECORD_ID,
    INVALID_PATIENT_FIRST_NAME,
    INVALID_PATIENT_LAST_NAME,
    INVALID_DISEASE_ID,
    INVALID_COUNTRY,
    INVALID_ENTRY_DATE,
    INVALID_EXIT_DATE,
    INVALID_EARLIER_EXIT_DATE
  };

  typedef struct patient_record {
    char* record_id;
    char* patient_first_name;
    char* patient_last_name;
    char* disease_id;
    char* country;
    struct tm entry_date;
    struct tm exit_date;
  } patient_record_t;

  typedef patient_record_t* patient_record_ptr;

  /*
    Utility function to create a new patient record struct given
    patient record tokens
  */
  patient_record_ptr patient_record_create(char**);
  /* Utility function to print a patient record struct */
  void patient_record_print(void*, FILE*);
  /* Utility function to compare two patient record structs */
  int64_t patient_record_compare(void*, void*);
  /* Utility function to delete an allocated patient record struct */
  void patient_record_delete(void*);

  /*
    Given an array of tokens about a patient record, returns a success
    code if everything was fine or the correspodent error code.
  */
  int validate_patient_record_tokens(char**);
  /* Print message given a patient_record error code */
  void print_patient_record_error(char**, int);

#endif
