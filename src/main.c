#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../includes/avl.h"
#include "../includes/hash_table.h"
#include "../includes/utils.h"

program_parameters_t parameters;

int main(int* argc, char* argv[]) {
  srandom((unsigned int) ((time(NULL) ^ (intptr_t) printf) & (intptr_t) main));
  parse_arguments(argc, argv);
  printf("%s\n",parameters.patient_records_filename);
  return EXIT_SUCCESS;
}
