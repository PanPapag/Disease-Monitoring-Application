#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//#include "../includes/avl.h"
#include "../includes/list.h"
#include "../includes/macros.h"
#include "../includes/hash_table.h"
#include "../includes/patient_record.h"
#include "../includes/utils.h"

int global_counter;

test_ptr create_test_ptr(int a, int b) {
  test_ptr test = (test_ptr) malloc((sizeof(*test)));
  test->a = a;
  test->b = b;
  return test;
}

void test_print(FILE* out, void* v) {
  test_ptr t = (*(test_ptr *) v);
  fprintf(out, "A: %d\n", t->a);
  fprintf(out, "B: %d\n", t->b);
  printf("\n");
}

int test_compare(void* v, void* b) {
  test_ptr k = (*(test_ptr *) v);
  test_ptr m = (*(test_ptr *) v);
  return k->a - m->a;
}

void destroy_test(void* v) {
  test_ptr t = (*(test_ptr *) v);
  free(t);
}

void destroy_str(void *v) {
  char* s = (char *)v;
  free(s);
}


program_parameters_t parameters;

extern hash_table_ptr patient_record_ht;


int main(int argc, char* argv[]) {
  srandom((unsigned int) ((time(NULL) ^ (intptr_t) printf) & (intptr_t) main));
  parse_arguments(argc, argv);
  /* Define number of buckets as the max size of disease and country hash table */
  size_t no_buckets = MAX(parameters.ht_disease_size, parameters.ht_country_size);
  /* patient_record_ht: record id --> pointer to patient record structure */
  // patient_record_ht = hash_table_create(STRING, patient_record_ptr,
  //                                       no_buckets, parameters.bucket_size,
  //                                       hash_string,
  //                                       compare_string, patient_record_compare,
  //                                       print_string, patient_record_print);
  //read_patient_records_file_and_update_structures();
  /* Create Disease Hash Table */ /*
  hash_table_ptr disease_ht = hash_table_create(char*, avl_ptr,
                                                parameters.ht_disease_size,
                                                parameters.bucket_size,
                                                NULL, //TODO HASH F
                                                patient_record_compare,
                                                NULL, // TOSO PRINT CHAR*
                                                patient_record_print);
  /* Create Country Hash Table */ /*
  hash_table_ptr country_ht = hash_table_create(char*, avl_ptr,
                                                parameters.ht_disease_size,
                                                parameters.bucket_size,
                                                NULL, //TODO HASH F
                                                patient_record_compare,
                                                NULL, // TOSO PRINT CHAR*
                                                patient_record_print); */

  /* Create AVL tree to store patient records */
  // avl_ptr avl = avl_create(patient_record_ptr,
  //                          patient_record_compare,
  //                          patient_record_print);
  /*
  struct tm test;
  char buf[255];
  memset(&test, 0, sizeof(struct tm));
  strptime("-", "%d-%m-%Y", &test);
  puts(buf);
  */
  /*
  list_ptr l = list_create(patient_record_ptr, patient_record_compare, patient_record_print, patient_record_delete);

  patient_record_ptr p = malloc(sizeof(*p));
  p->record_id = (char*) malloc(BUFFER_SIZE * sizeof(char));
  strcpy(p->record_id, "1256X");
  p->disease_id = (char*) malloc(BUFFER_SIZE * sizeof(char));
  strcpy(p->disease_id, "CORONA-19");
  p->patient_first_name = (char*) malloc(BUFFER_SIZE * sizeof(char));
  strcpy(p->patient_first_name, "PANTELIS");
  p->patient_last_name = (char*) malloc(BUFFER_SIZE * sizeof(char));
  strcpy(p->patient_last_name, "PAPAGEORGIOU");
  memset(&p->entry_date, 0, sizeof(struct tm));
  strptime("12-2-2019", "%d-%m-%Y", &p->entry_date);
  memset(&p->exit_date, 0, sizeof(struct tm));
  strptime("-", "%d-%m-%Y", &p->exit_date);

  patient_record_ptr v = malloc(sizeof(*v));
  v->record_id = (char*) malloc(BUFFER_SIZE * sizeof(char));
  strcpy(v->record_id, "1256X");
  v->disease_id = (char*) malloc(BUFFER_SIZE * sizeof(char));
  strcpy(v->disease_id, "CORONA-18");
  v->patient_first_name = (char*) malloc(BUFFER_SIZE * sizeof(char));
  strcpy(v->patient_first_name, "PANTELIS");
  v->patient_last_name = (char*) malloc(BUFFER_SIZE * sizeof(char));
  strcpy(v->patient_last_name, "PAPAGEORGIOU");
  memset(&v->entry_date, 0, sizeof(struct tm));
  strptime("12-2-2018", "%d-%m-%Y", &v->entry_date);
  memset(&v->exit_date, 0, sizeof(struct tm));
  strptime("-", "%d-%m-%Y", &v->exit_date);

  list_sorted_insert(&l, &p);
  list_sorted_insert(&l, &v);
  list_print(l, stdout);
  list_clear(l);
  */
  //hash_table_print(stdout, patient_record_ht);
  //hash_table_clear(patient_record_ht);
  // ------------------------------ HT TEST ----------------------------------
  hash_table_ptr test_ht = hash_table_create(STRING, test_ptr,
                                            no_buckets, parameters.bucket_size,
                                            hash_string,
                                            compare_string, test_compare,
                                            print_string, test_print,
                                            destroy_str, destroy_test);
  int a, b;
  test_ptr test;
  char *key[5];
  for (int i = 0; i < 5; ++i)
    key[i] = (char*) malloc(sizeof(char) * 9);

  strcpy(key[0],"PANTELIS");
  strcpy(key[1],"@KOSTAS@");
  strcpy(key[2],"@HELENI@");
  strcpy(key[3],"ANASTASI");
  strcpy(key[4],"@@MEMA@@");
  for (int i = 0; i < 1; ++i) {
    a = (i * 10) + 1;
    b = (a * 2) + 2;
    test_ptr test = create_test_ptr(a,b);
    hash_table_insert(&test_ht, &key[i], &test);
  }
  printf("----------------------------------------------------------------------\n" );
  //hash_table_print(stdout, test_ht);
  //hash_table_clear(test_ht);
  return EXIT_SUCCESS;
}
