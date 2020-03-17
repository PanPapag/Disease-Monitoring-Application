#ifndef __IO_UTILS__
  #define __IO_UTILS__

  /* Parses command line arguments and stores them to program_parameters_t */
  void parse_arguments(int*, char**);

  /* Reads patient record file line by line, checks errors and update structures */
  void read_patient_records_file_and_update_structures(void);

  /* Reports Error Messages */
  void report_error(const char *fmt, ...);
  /* Reports Warning Messages */
  void report_warning(const char *fmt, ...);


#endif
