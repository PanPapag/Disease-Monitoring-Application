#ifndef __PATIENT_RECORD__
  #define __PATIENT_RECORD__

  #include <time.h>

  #define EXIT_DATE_NOT_SPECIFIED "00-01-1900"

  typedef struct patient_record {
    char* record_id;
    char* patient_first_name;
    char* patient_last_name;
    char* disease_id;
    struct tm entry_date;
    struct tm exit_date;
  } patient_record_t;

  typedef patient_record_t* patient_record_ptr;

  /* Utility function to print a patient record struct */
  void patient_record_print(FILE*, void*);
  /* Utility function to compare two patient record structs */
  int patient_record_compare(void*, void*);
  /* Utility function to delete an allocated patient record struct */
  void patient_record_delete(void*);

#endif
