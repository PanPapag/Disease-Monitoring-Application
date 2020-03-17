#include <fcntl.h>
#include <getopt.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "../includes/io_utils.h"
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
