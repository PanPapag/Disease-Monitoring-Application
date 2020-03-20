#ifndef __COMMANDS__
  #define __COMMANDS__

  enum command_codes {
    INVALID_COMMAND,
    VALID_COMMAND
  };

  int validate_global_disease_stats(int, char**);

  int validate_disease_frequency(int, char**);

  int validate_topk_diseases(int, char**);

  int validate_topk_countries(int, char**);

  int validate_insert_patient_record(int, char**);
  void execute_insert_patient_record(int, char**);

  int validate_record_patient_exit(int, char**);

  int validate_num_current_patients(int, char**);

  int validate_exit(int, char**);

#endif