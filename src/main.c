#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../includes/macros.h"
#include "../includes/hash_table.h"
#include "../includes/patient_record.h"
#include "../includes/utils.h"

program_parameters_t parameters;

extern hash_table_ptr patient_record_ht;

int main(int argc, char* argv[]) {
  srandom((unsigned int) ((time(NULL) ^ (intptr_t) printf) & (intptr_t) main));
  parse_arguments(argc, argv);
  /* Define number of buckets as the max size of disease and country hash table */
  size_t no_buckets = MAX(parameters.ht_disease_size, parameters.ht_country_size);
  /* patient_record_ht: record id --> pointer to patient record structure */
  patient_record_ht = hash_table_create(no_buckets, parameters.bucket_size,
                                        hash_string,
                                        compare_string, patient_record_compare,
                                        print_string, patient_record_print,
                                        NULL, patient_record_delete);
  read_patient_records_file_and_update_structures();
  main_loop();
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


  hash_table_print(patient_record_ht, stdout);


  hash_table_clear(patient_record_ht);
  return EXIT_SUCCESS;
}
