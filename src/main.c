#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../includes/avl.h"
#include "../includes/list.h"
#include "../includes/hash_table.h"
#include "../includes/patient_record.h"
#include "../includes/utils.h"

program_parameters_t parameters;

int main(int* argc, char* argv[]) {
  srandom((unsigned int) ((time(NULL) ^ (intptr_t) printf) & (intptr_t) main));
  parse_arguments(argc, argv);
  size_t no_buckets = MAX(parameters.ht_disease_size, parameters.ht_country_size);
  //avl_ptr avl = avl_create(patient_record_ptr, cmp, print);

  /*
  struct tm test;
  char buf[255];
  memset(&test, 0, sizeof(struct tm));
  strptime("-", "%d-%m-%Y", &test);
  puts(buf);

  list_ptr l = list_create(patient_record_ptr, NULL, patient_record_print);

  patient_record_ptr p = malloc(sizeof(*p));
  p->record_id = (char*) malloc(BUFFER_SIZE * sizeof(char));
  strcpy(p->record_id, "1256X");
  p->disease_id = (char*) malloc(BUFFER_SIZE * sizeof(char));
  strcpy(p->disease_id, "CORONA");
  p->patient_first_name = (char*) malloc(BUFFER_SIZE * sizeof(char));
  strcpy(p->patient_first_name, "PANTELIS");
  p->patient_last_name = (char*) malloc(BUFFER_SIZE * sizeof(char));
  strcpy(p->patient_last_name, "PAPAGEORGIOU");
  memset(&p->entry_date, 0, sizeof(struct tm));
  strptime("12-2-2019", "%d-%m-%Y", &p->entry_date);
  memset(&p->exit_date, 0, sizeof(struct tm));
  strptime("-", "%d-%m-%Y", &p->exit_date);

  list_push_front(&l, &p);
  list_print(l, stdout);
  */

  return EXIT_SUCCESS;
}
