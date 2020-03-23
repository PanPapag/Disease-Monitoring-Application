#include <stdio.h>
#include <stdlib.h>

#include "../includes/avl.h"
#include "../includes/macros.h"
#include "../includes/hash_table.h"
#include "../includes/patient_record.h"
#include "../includes/utils.h"

#include "../includes/heap.h"

program_parameters_t parameters;

extern hash_table_ptr patient_record_ht;
extern hash_table_ptr disease_ht;
extern hash_table_ptr country_ht;

extern list_ptr diseases_names;

int main(int argc, char* argv[]) {
	/* Parse command line arguments and update program parameters */
  parse_arguments(argc, argv);
  /* Define number of buckets as the max size of disease and country hash table */
  size_t no_buckets = MAX(parameters.ht_disease_size, parameters.ht_country_size);
  /* patient_record_ht: record id --> pointer to patient record structure */
  // patient_record_ht = hash_table_create(no_buckets, parameters.bucket_size,
  //                                       hash_string, compare_string,
  //                                       print_string, patient_record_print,
  //                                       NULL, patient_record_delete);
  // /* Create Disease Hash Table */
  // disease_ht = hash_table_create(parameters.ht_disease_size,
	// 															 parameters.bucket_size,
  //                                hash_string, compare_string,
  //                                print_string, avl_print_inorder,
  //                                NULL, avl_clear);
  // /* Create Country Hash Table */
  // country_ht = hash_table_create(parameters.ht_country_size,
	// 	                             parameters.bucket_size,
  //                                hash_string, compare_string,
  //                                print_string, avl_print_inorder,
  //                                NULL, avl_clear);
	// /* Initialize a list to store all disease names */
	// diseases_names = list_create(STRING*, print_string, compare_string, NULL);
  // /* Read patient Record File and Update all Data Structures */
	// read_patient_records_file_and_update_structures();
	// /* Execute the app until command exit is given */
  // main_loop();
	/* DEBUG Purposes */
  heap_ptr heap = heap_create(compare_int, print_int, destroy_int);
  int* test[10];
  for (int i = 0; i < 10; ++i) {
    test[i] = create_int(i);
    heap_insert_max(&heap, test[i]);
  }
  //printLevelOrder(heap->root_);
  for (int i = 0; i < 10; ++i) {
    void* res = heap_extract_max(&heap);
    printf("MAX: %d\n",(*(int*)res));
  }
  heap_clear(heap);
  /* Everything find return EXIT_SUCCESS */
  return EXIT_SUCCESS;
}
