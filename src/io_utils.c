#include <fcntl.h>
#include <getopt.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <wordexp.h>

#include "../includes/hash_table.h"
#include "../includes/macros.h"
#include "../includes/io_utils.h"
#include "../includes/patient_record.h"
#include "../includes/utils.h"

program_parameters_t parameters;

static struct option options[] = {
      {"p",    required_argument, NULL, 'p'},
      {"h1",   required_argument, NULL, 'd'},
      {"h2",   required_argument, NULL, 'c'},
      {"b",    required_argument, NULL, 'b'},
      {"help", no_argument,       NULL, 'h'},
      {0, 0, 0,                         0}
};

static inline
void __usage() {
    fprintf(stderr,
            "You must provide the following arguments:\n"
            "\t-p <Patient Records File>\n"
            "\t-h1 <Number of buckets in disease hash table>\n"
            "\t-h2 <Number of buckets in country hash table>\n"
            "\t-b <Bucket size in bytes>\n");
    exit(EXIT_FAILURE);
}

void parse_arguments(int* argc, char* argv[]) {
  if (argc < 9) __usage();
  int c;
  while (1) {
    int option_index;
    c = getopt_long_only(argc, argv, "p:d:c:b:h", options, &option_index);
    if (c == -1) break;
    switch (c) {
      case 'p': {
        parameters.patient_records_filename = optarg;
        break;
      }
      case 'd': {
        size_t ht_disease_size;
        if (!string_to_int64(optarg, (int64_t *) &ht_disease_size)) {
          report_error("The number of buckets provided for the disease hash table is invalid. Exiting...");
          exit(EXIT_FAILURE);
        }
        parameters.ht_disease_size = ht_disease_size;
        break;
      }
      case 'c': {
        size_t ht_country_size;
        if (!string_to_int64(optarg, (int64_t *) &ht_country_size)) {
          report_error("The number of buckets provided for the country hash table is invalid. Exiting...");
          exit(EXIT_FAILURE);
        }
        parameters.ht_country_size = ht_country_size;
        break;
      }
      case 'b': {
        size_t bucket_size;
        if (!string_to_int64(optarg, (int64_t *) &bucket_size)) {
          report_error("The Bucket size provided is invalid. Exiting...");
          exit(EXIT_FAILURE);
        }
        parameters.bucket_size = bucket_size;
        break;
      }
      case 'h':
        __usage();
      case '?':
        break;
      default:
        abort();
    }
  }
}

void read_patient_records_file_and_update_structures() {
  char buffer[BUFFER_SIZE];
  char **patient_record_tokens;
  int patient_record_no_tokens;
  patient_record_ptr* t;
  wordexp_t p;
  /* Open file for read only - handles binary fille too */
  FILE* fp = fopen(parameters.patient_records_filename, "rb+");
  /* Read file line by line */
  while (fgets(buffer, sizeof(buffer), fp) != NULL) {
    /* Discard '\n' that fgets() stores */
    buffer[strlen(buffer) - 1] = '\0';
    wordexp(buffer, &p, 0);
    patient_record_tokens = p.we_wordv;
    patient_record_no_tokens = p.we_wordc;
    /* Count tokens to check if patient record is valid */
    if (patient_record_no_tokens != NO_PATIENT_RECORD_TOKENS) {
      report_error("Invalid number of arguments in patient record <%s>. "
                   "Discarding patient record.", buffer);
    }
    /* Check patient record tokens' validity */
    int code = validate_patient_record_tokens(patient_record_tokens);
    if (code == VALID_PATIENT_RECORD) {
      execute_insert_patient_record(patient_record_tokens);
    } else {
      print_patient_record_error(patient_record_tokens, code);
    }
    /* Free wordexp object */
    wordfree(&p);
  }
  /* Close file pointer */
  fclose(fp);
}

void main_loop(void) {
  char command[BUFFER_SIZE];
  while (1) {
    /* Read command from the stdin */
    printf("> ");
    memset(&command, 0, sizeof(command));
    fgets(command, BUFFER_SIZE, stdin);
    command[strlen(command) - 1] = '\0';
    /* Handle command and call correspodent function */
    int result = handle_command(command);
    if (result == EXIT) {
      break;
    }
  }
}

int handle_command(char command[]) {
  wordexp_t p;
  char** command_tokens;
  int command_no_tokens;
  char** command_argv;
  int command_argc;
  int command_code = CONTINUE;
  /* Use API POSIX to extract arguments */
  wordexp(command, &p, 0);
  command_tokens = p.we_wordv;
  command_no_tokens = p.we_wordc;
  /* Call correspoding command function */
  if (!strcmp(command_tokens[0], "globalDiseaseStats")) {
    if (validate_global_disease_stats(command_no_tokens, command_tokens)) {
      printf("EXECUTE\n");
    } else {
      report_warning("Invalid <%s> command.", command_tokens[0]);
      fprintf(stderr, "Usage: globalDiseaseStats [date1 date2]\n");
    }
  } else if (!strcmp(command_tokens[0], "diseaseFrequency")) {
    if (validate_disease_frequency(command_no_tokens, command_tokens)) {
      printf("EXECUTE\n");
    } else {
      report_warning("Invalid <%s> command.", command_tokens[0]);
      fprintf(stderr, "Usage: diseaseFrequency virusName [country] date1 date2\n");
    }
  } else if (!strcmp(command_tokens[0], "topk-Diseases")) {
    if (validate_topk_diseases(command_no_tokens, command_tokens)) {
      printf("EXECUTE\n");
    } else {
      report_warning("Invalid <%s> command.", command_tokens[0]);
      fprintf(stderr, "Usage: topk-Diseases k country [date1 date2]\n");
    }
  } else if (!strcmp(command_tokens[0], "topk-Countries")) {
    if (validate_topk_countries(command_no_tokens, command_tokens)) {
      printf("EXECUTE\n");
    } else {
      report_warning("Invalid <%s> command.", command_tokens[0]);
      fprintf(stderr, "Usage: topk-Countries k disease [date1 date2]\n");
    }
  } else if (!strcmp(command_tokens[0], "insertPatientRecord")) {
    if (validate_insert_patient_record(command_no_tokens, command_tokens)) {
      command_argv = prune_command_name(command_tokens, command_no_tokens);
      execute_insert_patient_record(command_argv);
      __FREE(command_argv);
    } else {
      report_warning("Invalid <%s> command.", command_tokens[0]);
      fprintf(stderr, "Usage: insertPatientRecord recordID patientFirstName "
                      "patientLastName diseaseID country entryDate [exitDate]\n");
    }
  } else if (!strcmp(command_tokens[0], "recordPatientExit")) {
    if (validate_record_patient_exit(command_no_tokens, command_tokens)) {
      printf("EXECUTE\n");
    } else {
      report_warning("Invalid <%s> command.", command_tokens[0]);
      fprintf(stderr, "Usage: recordPatientExit recordID exitDate\n");
    }
  } else if (!strcmp(command_tokens[0], "numCurrentPatients")) {
    if (validate_num_current_patients(command_no_tokens, command_tokens)) {
      printf("EXECUTE\n");
    } else {
      report_warning("Invalid <%s> command.", command_tokens[0]);
      fprintf(stderr, "Usage: numCurrentPatients [disease]\n");
    }
  } else if (!strcmp(command_tokens[0], "exit")) {
    if (validate_exit(command_no_tokens, command_tokens)) {
      execute_exit();
      command_code = EXIT;
    } else {
      report_warning("Invalid <%s> command.", command_tokens[0]);
      fprintf(stderr, "Usage: exit\n");
    }
  } else {
    report_warning("Unknown command: <%s>.", command);
  }
  /* Print new line for better format in the console */
  printf("\n");
  /* Free wordexp object */
  wordfree(&p);
  /* Return command code */
  return command_code;
}

static void __report(const char* tag, const char* fmt, va_list args) {
  fprintf(stderr, "%s", tag);
  vfprintf(stderr, fmt, args);
  fputc('\n', stderr);
  va_end(args);
}

void report_error(const char* fmt, ...) {
  va_list args;
  va_start(args, fmt);
  __report("[ERROR]: ", fmt, args);
}

void report_warning(const char* fmt, ...) {
  va_list args;
  va_start(args, fmt);
  __report("[WARNING]: ", fmt, args);
}
