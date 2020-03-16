#ifndef __IO_UTILS__
  #define __IO_UTILS__

  /* Parses command line arguments and stores them to program_parameters_t */
  void parse_arguments(int*, char**);

  /* Reports Error Messages */
  void report_error(const char *fmt, ...);
  /* Reports Warning Messages */
  void report_warning(const char *fmt, ...);

#endif
