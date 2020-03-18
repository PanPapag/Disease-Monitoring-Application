#include <fcntl.h>
#include <getopt.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "../includes/hash_table.h"
#include "../includes/macros.h"
#include "../includes/io_utils.h"
#include "../includes/patient_record.h"
#include "../includes/utils.h"

program_parameters_t parameters;

hash_table_ptr patient_record_ht;

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
  int line_counter = 0;
  char buffer[BUFFER_SIZE], copy_buffer[BUFFER_SIZE];
  char *patient_record_tokens[NO_PATIENT_RECORD_TOKENS];
  /* Open file for read only - handles binary fille too */
  FILE* fp = fopen(parameters.patient_records_filename, "rb+");
  /* Read file line by line */
  while (fgets(buffer, sizeof(buffer), fp) != NULL) {
    /* Discard '\n' that fgets() stores */
    buffer[strlen(buffer) - 1] = '\0';
    /* Copy line to another buffer in order to be tokenized independently */
    memset(copy_buffer, 0, sizeof(copy_buffer));
    strcpy(copy_buffer, buffer);
    /* Count tokens to check if patient record is valid */
    if (count_tokens(copy_buffer, PATIENT_RECORD_DELIMITER) != NO_PATIENT_RECORD_TOKENS) {
      report_error("Invalid number of arguments in patient record <%s>. "
                   "Discarding patient record.", buffer);
    }
    /* Store each token of the line into an array */
    tokenize_string(buffer, PATIENT_RECORD_DELIMITER, patient_record_tokens);
    /* Check patient record tokens' validity */
    int code = validate_patient_record_tokens(patient_record_tokens);
    if (code != VALID_PATIENT_RECORD) {
      printf("TODO \n"); // TODO
    }
    /* Create a new patient record */
    patient_record_ptr patient_record = patient_record_create(patient_record_tokens);
    int res = hash_table_insert(&patient_record_ht, &patient_record->record_id, &patient_record);
    if (res == FAIL) {
      printf("FAILED\n");
    } else {
      printf("SUCCESS\n");
    }
  }
  /* Close file pointer */
  fclose(fp);
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
