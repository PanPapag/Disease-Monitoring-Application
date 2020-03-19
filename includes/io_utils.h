#ifndef __IO_UTILS__
  #define __IO_UTILS__

  enum handle_command_codes {
    CONTINUE,
    EXIT
  };

  /* Parses command line arguments and stores them to program_parameters_t */
  void parse_arguments(int*, char**);

  /* Reads patient record file line by line, checks errors and update structures */
  void read_patient_records_file_and_update_structures(void);

  /* Reads commands from stdin until exit will be given */
  void main_loop(void);

  /* Given a command read from stding determine which function to call */
  int handle_command(char[]);

  /* Reports Error Messages */
  void report_error(const char *fmt, ...);
  /* Reports Warning Messages */
  void report_warning(const char *fmt, ...);


#endif
