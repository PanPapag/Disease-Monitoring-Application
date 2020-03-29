#ifndef __COMMANDS__
  #define __COMMANDS__

  enum command_codes {
    INVALID_COMMAND,
    VALID_COMMAND
  };

  typedef struct country_stats_t* country_stats_ptr;
  struct country_stats_t {
    char* disease_id;
    int no_patients;
  } country_stats_t;

  typedef struct disease_stats_t* disease_stats_ptr;
  struct disease_stats_t {
    char* country;
    int no_patients;
  } disease_stats_t;

  int validate_global_disease_stats(int, char**);
  void execute_global_disease_stats(int, char**);

  int validate_disease_frequency(int, char**);
  void execute_disease_frequency(int, char**);

  int validate_topk_diseases(int, char**);
  void execute_topk_diseases(int, char**);

  int validate_topk_countries(int, char**);
  void execute_topk_countries(int, char**);

  int validate_insert_patient_record(int, char**);
  int execute_insert_patient_record(char**, int);

  int validate_record_patient_exit(int, char**);
  void execute_record_patient_exit(char**);

  int validate_num_current_patients(int, char**);
  void execute_num_current_patients(int, char**);

  int validate_exit(int, char**);
  void execute_exit(void);

#endif
