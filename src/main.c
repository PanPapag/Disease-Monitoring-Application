#include <stdio.h>
#include <stdlib.h>

#include "../includes/avl.h"
#include "../includes/macros.h"
#include "../includes/hash_table.h"
#include "../includes/patient_record.h"
#include "../includes/utils.h"

program_parameters_t parameters;

extern hash_table_ptr patient_record_ht;
extern hash_table_ptr disease_ht;
extern hash_table_ptr country_ht;

int* create_int(int value) {
	int* p = malloc(sizeof(int));
	*p = value;
	return p;
}

int compare_ints(void* a, void* b) {
	return *(int*)a - *(int*)b;
}

void print_int(void *v, FILE *out) {
	fprintf(out, "%d\n", (*(int *)v));
}

int main(int argc, char* argv[]) {
	/* Parse command line arguments and update program parameters */
  parse_arguments(argc, argv);
  /* Define number of buckets as the max size of disease and country hash table */
  size_t no_buckets = MAX(parameters.ht_disease_size, parameters.ht_country_size);
  /* patient_record_ht: record id --> pointer to patient record structure */
  patient_record_ht = hash_table_create(no_buckets, parameters.bucket_size,
                                        hash_string, compare_string,
                                        print_string, patient_record_print,
                                        NULL, patient_record_delete);
  /* Create Disease Hash Table */
  disease_ht = hash_table_create(parameters.ht_disease_size,
																 parameters.bucket_size,
                                 hash_string, compare_string,
                                 print_string, avl_print_inorder,
                                 NULL, avl_clear);
  /* Create Country Hash Table */
  country_ht = hash_table_create(parameters.ht_country_size,
		                             parameters.bucket_size,
                                 hash_string, compare_string,
                                 print_string, avl_print_inorder,
                                 NULL, avl_clear);
  /* Read patient Record File and Update all Data Structures */
	read_patient_records_file_and_update_structures();
	/* Execute the app until command exit is given */
  main_loop();
	/* DEBUG Purposes */
	//
	// avl_ptr avl = avl_create(compare_ints, print_int);
	//
	// int N = 10000;
	//
	// int* value_array[N];
	//
	// // Δοκιμάζουμε την insert με νέες τιμές κάθε φορά και με αυτόματο free
	// for (int i = 0; i < N; i++) {
	//
	// 	value_array[i] = create_int(i);
	//
	// 	avl_insert(&avl, value_array[i]);
	// }
	//
	// printf("----------PRINT AVL --------\n");
	// avl_print_inorder(avl, stdout);
	//
	// avl_node_ptr res_avl = avl_find(avl, value_array[0]);
	// printf("FOUND: %d -> %d correct\n",*((int *)res_avl->data_), *value_array[0]);
	//
	// avl_clear(avl);

  /* Everything find return EXIT_SUCCESS */
  return EXIT_SUCCESS;
}
