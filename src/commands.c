#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../includes/avl.h"
#include "../includes/commands.h"
#include "../includes/hash_table.h"
#include "../includes/heap.h"
#include "../includes/io_utils.h"
#include "../includes/list.h"
#include "../includes/macros.h"
#include "../includes/patient_record.h"
#include "../includes/utils.h"

program_parameters_t parameters;

hash_table_ptr patient_record_ht;
hash_table_ptr disease_ht;
hash_table_ptr country_ht;

list_ptr diseases_names;
list_ptr countries_names;

int validate_global_disease_stats(int argc, char** argv) {
  if (argc != 1 && argc != 3) {
    return INVALID_COMMAND;
  }
  if (argc == 1) {
    return VALID_COMMAND;
  } else {
    char* date1 = argv[1];
    char* date2 = argv[2];
    if (!is_valid_date_string(date1) || !is_valid_date_string(date2))
      return INVALID_COMMAND;
    /* Check if date2 is earlier than the date1 */
    if (compare_date_strings(date1, date2) > 0)
      return INVALID_COMMAND;
  }
  return VALID_COMMAND;
}

static inline
int __count_patients_between(avl_node_ptr current_root, int* counter,
                             struct tm date1, struct tm date2, char* country) {

  avl_node_ptr temp = current_root;
  patient_record_ptr patient_record = NULL;
  /* Prune left search below this node as all entries have entry date < date1 */
  if (temp != NULL) {
    patient_record = (patient_record_ptr) temp->data_;
    if (compare_date_tm(date1, patient_record->entry_date) <= 0) {
      __count_patients_between(temp->left_, counter, date1, date2, country);
    }
    patient_record = (patient_record_ptr) temp->data_;
    /* Check if patient_record exit date is not specified */
    if (compare_date_tm(patient_record->entry_date, date2) <= 0) {
      /* Check upper bound */
      if (country != NULL) {
        if (!strcmp(country, patient_record->country)) {
          /* Update counter */
          (*counter)++;
        }
      } else {
        /* Update counter */
        (*counter)++;
      }
    }
    __count_patients_between(temp->right_, counter, date1, date2, country);
  }
}

static inline
int __num_patients_between(avl_ptr disease_avl, char* date1, char* date2, char* country) {
  int counter = 0;
  /* Convert string dates to struct tm format */
  struct tm date1_tm;
  memset(&date1_tm, 0, sizeof(struct tm));
  strptime(date1, "%d-%m-%Y", &date1_tm);

  struct tm date2_tm;
  memset(&date2_tm, 0, sizeof(struct tm));
  strptime(date2, "%d-%m-%Y", &date2_tm);
  /*
    Traverse inorder the AVL Tree and each time a patient record is found
    between the given range increase the counter for current patients.
    We can prune the traversal under the node in which a patient record has
    entry date < date1 or exit date > date2.
  */
   __count_patients_between(disease_avl->root_, &counter, date1_tm, date2_tm, country);
  return counter;
}

void execute_global_disease_stats(int argc, char** argv) {
  /* Print for every disease the total number of patients */
  for (size_t i = 1U; i <= list_size(diseases_names); ++i) {
    /* Get every disease id */
    list_node_ptr list_node = list_get(diseases_names, i);
    char* disease_id = (*(char**) list_node->data_);
    /* Get for the current disease its AVL tree */
    void* result = hash_table_find(disease_ht, disease_id);
    if (result == NULL) {
      report_warning("There is no disease recorded with Disease ID: <%s>",
                     disease_id);
    } else {
      /* Cast result to avl pointer */
      avl_ptr disease_avl = (avl_ptr) result;
      if (argc == 0) {
        /* Print total number of patients - size of AVL tre */
        printf("%s %d\n", disease_id, avl_size(disease_avl));
      } else {
        /* Print total number of patients in the given date range */
        printf("%s %d\n", disease_id,
               __num_patients_between(disease_avl, argv[0], argv[1], NULL));
      }
    }
  }
}

int validate_disease_frequency(int argc, char** argv) {
  if (argc != 4 && argc != 5) {
    return INVALID_COMMAND;
  } else {
    if (argc == 4) {
      // Check virus name to contain only letters, numbers and possibly '-'
      char* virus_name = argv[1];
      for (size_t i = 0; i < strlen(virus_name); ++i) {
        if (!isalnum(virus_name[i]) && virus_name[i] != '-')
          return INVALID_COMMAND;
      }
      // Check date1
      char* date1 = argv[2];
      if (!is_valid_date_string(date1))
        return INVALID_COMMAND;
      // Check date2
      char* date2 = argv[3];
      if (!is_valid_date_string(date2))
        return INVALID_COMMAND;
    } else {
      // Check virus name to contain only letters, numbers and possibly '-'
      char* virus_name = argv[1];
      for (size_t i = 0; i < strlen(virus_name); ++i) {
        if (!isalnum(virus_name[i]) && virus_name[i] != '-')
          return INVALID_COMMAND;
      }
      // Check date1
      char* date1 = argv[2];
      if (!is_valid_date_string(date1))
        return INVALID_COMMAND;
      // Check date2
      char* date2 = argv[3];
      if (!is_valid_date_string(date2))
        return INVALID_COMMAND;
      // Check if exit date is earlier than the entry date/
      if (compare_date_strings(date1, date2) > 0)
        return INVALID_COMMAND;
      // Check country to contain only letters
      char* country = argv[4];
      if (!is_alphabetical(country))
        return INVALID_COMMAND;
    }
  }
  return VALID_COMMAND;
}

void execute_disease_frequency(int argc, char** argv) {
  void* result = hash_table_find(disease_ht, argv[0]);
  if (result == NULL) {
    report_warning("There is no disease recorded with Disease ID: <%s>", argv[0]);
  } else {
    /* Cast result to avl pointer */
    avl_ptr disease_avl = (avl_ptr) result;
    /* Determine if country argument was given or not */
    if (argc == 3) {
      /* Print total number of patients in the given date range */
      printf("%s %d\n", argv[0],
             __num_patients_between(disease_avl, argv[1], argv[2], NULL));
    } else {
      /* Print total number of patients for given country in the given date range */
      printf("%s %d\n", argv[0],
             __num_patients_between(disease_avl, argv[1], argv[2], argv[3]));
    }
  }
}

int validate_topk_diseases(int argc, char** argv) {
  if (argc != 3 && argc != 5) {
    return INVALID_COMMAND;
  } else {
    if (argc == 3) {
      // Check k
      char* k = argv[1];
      if (!is_number(k))
        return INVALID_COMMAND;
      // Check country to contain only letters
      char* country = argv[2];
      if (!is_alphabetical(country))
        return INVALID_COMMAND;
    } else {
      // Check k
      char* k = argv[1];
      if (!is_number(k))
        return INVALID_COMMAND;
      // Check country to contain only letters
      char* country = argv[2];
      if (!is_alphabetical(country))
        return INVALID_COMMAND;
      // Check date1
      char* date1 = argv[3];
      if (!is_valid_date_string(date1))
        return INVALID_COMMAND;
      // Check date2
      char* date2 = argv[4];
      if (!is_valid_date_string(date2))
        return INVALID_COMMAND;
      // Check if exit date is earlier than the entry date
      if (compare_date_strings(date1, date2) > 0)
        return INVALID_COMMAND;
    }
  }
  return VALID_COMMAND;
}

static inline
void __util_traverse_for_topk(avl_node_ptr current_root, hash_table_ptr ht,
  char* (*get_value_to_hash)(patient_record_ptr)) {

  avl_node_ptr temp = current_root;
  if (temp != NULL) {
    __util_traverse_for_topk(temp->left_, ht, get_value_to_hash);
    /* Update hash table value  */
    patient_record_ptr patient_record = (patient_record_ptr) temp->data_;
    void* result = hash_table_find(ht, get_value_to_hash(patient_record));
    if (result != NULL) {
      (*(int*)result)++;
    }
    __util_traverse_for_topk(temp->right_, ht, get_value_to_hash);
  }
}

static inline
void __util_date_traverse_for_topk(avl_node_ptr current_root, hash_table_ptr ht,
  char* (*get_value_to_hash)(patient_record_ptr), struct tm date1, struct tm date2) {

  avl_node_ptr temp = current_root;
  patient_record_ptr patient_record = NULL;
  /* Prune left search below this node as all entries have entry date < date1 */
  if (temp != NULL) {
    patient_record = (patient_record_ptr) temp->data_;
    if (compare_date_tm(date1, patient_record->entry_date) <= 0) {
      __util_date_traverse_for_topk(temp->left_, ht, get_value_to_hash, date1, date2);
    }

    patient_record = (patient_record_ptr) temp->data_;
    if (compare_date_tm(patient_record->entry_date, date2) <= 0) {
      /* Update hash table value  */
      void* result = hash_table_find(ht, get_value_to_hash(patient_record));
      if (result != NULL) {
        (*(int*)result)++;
      }
    }
    __util_date_traverse_for_topk(temp->right_, ht, get_value_to_hash, date1, date2);
  }
}

static inline
void __util_execute_topk(avl_ptr avl, hash_table_ptr ht,
  char* (*get_value_to_hash)(patient_record_ptr), char* date1, char* date2) {

  if (date1 == NULL && date2 == NULL) {
    __util_traverse_for_topk(avl->root_, ht, get_value_to_hash);
  } else {
    /* Convert string dates to struct tm format */
    struct tm date1_tm;
    memset(&date1_tm, 0, sizeof(struct tm));
    strptime(date1, "%d-%m-%Y", &date1_tm);

    struct tm date2_tm;
    memset(&date2_tm, 0, sizeof(struct tm));
    strptime(date2, "%d-%m-%Y", &date2_tm);

    __util_date_traverse_for_topk(avl->root_, ht, get_value_to_hash, date1_tm, date2_tm);
  }
}

void execute_topk_diseases(int argc, char** argv) {
  /* Extract info from arguemnts */
  int k = atoi(argv[0]);
  char* country = argv[1];
  /* Get for the given country its AVL tree */
  void* result = hash_table_find(country_ht, country);
  if (result == NULL) {
    report_warning("There is no Country: <%s> recorded.", country);
  } else {
    /* Cast result to avl pointer */
    avl_ptr country_avl = (avl_ptr) result;
    /* Get the total number of diseases reported in the system */
    int no_diseases = list_size(diseases_names);
    /*
      Create a hash table to to map disease id -> total number of patients
      for the given country
    */
    hash_table_ptr country_stats_ht = hash_table_create(no_diseases,
                                                     parameters.bucket_size,
                                                     hash_string, compare_string,
                                                     print_string, print_int,
                                                     NULL, destroy_int);
    /* Initialize hash table - for each disease set total number ot patients to 0 */
    for (size_t i = 1U; i <= no_diseases; ++i) {
      /* Get every disease id */
      list_node_ptr list_node = list_get(diseases_names, i);
      char* disease_id = (*(char**) list_node->data_);
      /* Check if current disease already inserted to the country stats hash table */
      result = hash_table_find(country_stats_ht, disease_id);
      if (result == NULL) {
        int* init_patients = create_int(0);
        /* Update hash table */
        hash_table_insert(&country_stats_ht, disease_id, init_patients);
      } else {
        report_warning("Disease with Disease ID: <%s> already specified.",
                       disease_id);
      }
    }
    /* Update Hash Table country_stats while traversing country AVL */
    if (argc == 2) {
      /* Get total numer of patients for the current country */
      __util_execute_topk(country_avl, country_stats_ht, get_disease_id, NULL, NULL);
    } else {
      /* Get total number of patients for the current country in a given date range */
      __util_execute_topk(country_avl, country_stats_ht, get_disease_id, argv[2], argv[3]);
    }
    /* Copy content to country stats structure and Remove Hash table country_stats_ht */
    country_stats_ptr country_stats_table[no_diseases];
    for (size_t i = 0U; i < no_diseases; ++i) {
      /* Get every disease id */
      list_node_ptr list_node = list_get(diseases_names, i+1);
      char* disease_id = (*(char**) list_node->data_);
      /* Check if current disease has been inserted to the country stats hash table */
      result = hash_table_find(country_stats_ht, disease_id);
      if (result == NULL) {
        report_warning("Disease with Disease ID: <%s> not found.", disease_id);
      } else {
        /* Copy disease id and store total number of patients */
        country_stats_table[i] = (country_stats_ptr) malloc(sizeof(*country_stats_table[i]));
        country_stats_table[i]->disease_id = (char*) malloc((strlen(disease_id) + 1) * sizeof(char));
        strcpy(country_stats_table[i]->disease_id, disease_id);
        country_stats_table[i]->no_patients = (*(int*) result);
      }
    }
    /* Delete hash table */
    hash_table_clear(country_stats_ht);
    /*
      Build on the fly a max heap and insert there the elements
      of the country_stats_table
    */
    heap_ptr heap = heap_create(compare_country_stats, NULL, NULL);
    for (size_t i = 0; i < no_diseases; ++i) {
      heap_insert_max(&heap, country_stats_table[i]);
    }
    /* Extract top k */
    for (size_t i = 0; i < k; ++i) {
      result = heap_extract_max(&heap);
      if (result != NULL) {
        country_stats_ptr country_stats_entry = (country_stats_ptr) result;
        if (argc == 2) {
          printf("%s %d\n", country_stats_entry->disease_id, country_stats_entry->no_patients);
        } else {
          printf("%s %d\n", country_stats_entry->disease_id, country_stats_entry->no_patients);
        }
      }
    }
    /* Delete heap */
    heap_clear(heap);
    /* Free memory allocated for the country stats table */
    for (size_t i = 0U; i < no_diseases; ++i) {
      __FREE(country_stats_table[i]->disease_id);
      __FREE(country_stats_table[i]);
    }
  }
}

int validate_topk_countries(int argc, char** argv) {
  if (argc != 3 && argc != 5) {
    return INVALID_COMMAND;
  } else {
    if (argc == 3) {
      // Check k
      char* k = argv[1];
      if (!is_number(k))
        return INVALID_COMMAND;
      // Check virus name to contain only letters, numbers and possibly '-'
      char* virus_name = argv[2];
      for (size_t i = 0; i < strlen(virus_name); ++i) {
        if (!isalnum(virus_name[i]) && virus_name[i] != '-')
          return INVALID_COMMAND;
      }
    } else {
      // Check k
      char* k = argv[1];
      if (!is_number(k))
        return INVALID_COMMAND;
      // Check virus name to contain only letters, numbers and possibly '-'
      char* virus_name = argv[2];
      for (size_t i = 0; i < strlen(virus_name); ++i) {
        if (!isalnum(virus_name[i]) && virus_name[i] != '-')
          return INVALID_COMMAND;
      }
      // Check date1
      char* date1 = argv[3];
      if (!is_valid_date_string(date1))
        return INVALID_COMMAND;
      // Check date2
      char* date2 = argv[4];
      if (!is_valid_date_string(date2))
        return INVALID_COMMAND;
      // Check if exit date is earlier than the entry date/
      if (compare_date_strings(date1, date2) > 0)
        return INVALID_COMMAND;
    }
  }
  return VALID_COMMAND;
}

void execute_topk_countries(int argc, char** argv) {
  /* Extract info from arguemnts */
  int k = atoi(argv[0]);
  char* disease_id = argv[1];
  /* Get for the given disease its AVL tree */
  void* result = hash_table_find(disease_ht, disease_id);
  if (result == NULL) {
    report_warning("There is no Disease with Disease ID: <%s> recorded.", disease_id);
  } else {
    /* Cast result to avl pointer */
    avl_ptr disease_avl = (avl_ptr) result;
    /* Get the total number of countries reported in the system */
    int no_countries = list_size(countries_names);
    /*
      Create a hash table to to map country -> total number of patients
      for the given country affected by the disease
    */
    hash_table_ptr disease_stats_ht = hash_table_create(no_countries,
                                                     parameters.bucket_size,
                                                     hash_string, compare_string,
                                                     print_string, print_int,
                                                     NULL, destroy_int);
    /* Initialize hash table - for each country set total number ot patients to 0 */
    for (size_t i = 1U; i <= no_countries; ++i) {
      /* Get every disease id */
      list_node_ptr list_node = list_get(countries_names, i);
      char* country = (*(char**) list_node->data_);
      /* Check if current country already inserted to the disease stats hash table */
      result = hash_table_find(disease_stats_ht, country);
      if (result == NULL) {
        int* init_patients = create_int(0);
        /* Update hash table */
        hash_table_insert(&disease_stats_ht, country, init_patients);
      } else {
        report_warning("Country: <%s> already specified.",
                       disease_id);
      }
    }
    /* Update Hash Table disease_stats while traversing disease AVL */
    if (argc == 2) {
      /* Get total numer of patients for the current disease */
      __util_execute_topk(disease_avl, disease_stats_ht, get_country, NULL, NULL);
    } else {
      /* Get total number of patients for the current disease in a given date range */
      __util_execute_topk(disease_avl, disease_stats_ht, get_country, argv[2], argv[3]);
    }
    /* Copy content to disease stats structure and Remove Hash table disease_stats_ht */
    disease_stats_ptr disease_stats_table[no_countries];
    for (size_t i = 0U; i < no_countries; ++i) {
      /* Get every disease id */
      list_node_ptr list_node = list_get(countries_names, i+1);
      char* country = (*(char**) list_node->data_);
      /* Check if current country has been inserted to the disease stats hash table */
      result = hash_table_find(disease_stats_ht, country);
      if (result == NULL) {
        report_warning("Country: <%s> not found.", disease_id);
      } else {
        /* Copy disease id and store total number of patients */
        disease_stats_table[i] = (disease_stats_ptr) malloc(sizeof(*disease_stats_table[i]));
        disease_stats_table[i]->country = (char*) malloc((strlen(country) + 1) * sizeof(char));
        strcpy(disease_stats_table[i]->country, country);
        disease_stats_table[i]->no_patients = (*(int*) result);
      }
    }
    /* Delete hash table */
    hash_table_clear(disease_stats_ht);
    /*
      Build on the fly a max heap and insert there the elements
      of the disease_stats_table
    */
    heap_ptr heap = heap_create(compare_disease_stats, NULL, NULL);
    for (size_t i = 0; i < no_countries; ++i) {
      heap_insert_max(&heap, disease_stats_table[i]);
    }
    /* Extract top k */
    for (size_t i = 0; i < k; ++i) {
      result = heap_extract_max(&heap);
      if (result != NULL) {
        disease_stats_ptr disease_stats_entry = (disease_stats_ptr) result;
        if (argc == 2) {
          printf("%s %d\n", disease_stats_entry->country, disease_stats_entry->no_patients);
        } else {
          printf("%s %d\n", disease_stats_entry->country, disease_stats_entry->no_patients);
        }
      }
    }
    /* Delete heap */
    heap_clear(heap);
    /* Free memory allocated for the disease stats table */
    for (size_t i = 0U; i < no_countries; ++i) {
      __FREE(disease_stats_table[i]->country);
      __FREE(disease_stats_table[i]);
    }
  }
}

int validate_insert_patient_record(int argc, char** argv) {
  if (argc != 7 && argc != 8) {
    return INVALID_COMMAND;
  } else {
    /* record_id: Only letters and numbers */
    char* record_id = argv[1];
    if (!is_alpharithmetic(record_id))
      return INVALID_COMMAND;
    /* patient_first_name: Only letters */
    char* patient_first_name = argv[2];
    if (!is_alphabetical(patient_first_name))
      return INVALID_COMMAND;
    /* patient_last_name: Only letters */
    char* patient_last_name = argv[3];
    if (!is_alphabetical(patient_last_name))
      return INVALID_COMMAND;
    /* disease_id: Only letters, numbers and maybe character '-' */
    char* disease_id = argv[4];
    for (size_t i = 0; i < strlen(disease_id); ++i) {
      if (!isalnum(disease_id[i]) && disease_id[i] != '-')
        return INVALID_COMMAND;
    }
    /* country: Only letters */
    char* country = argv[5];
    if (!is_alphabetical(country))
      return INVALID_COMMAND;
    /* entry_date: DD-MM-YYYY format */
    char* entry_date = argv[6];
    if (!is_valid_date_string(entry_date))
      return INVALID_COMMAND;
    if (argc == 8) {
      /* exit_date: DD-MM-YYYY format or - (not specified) */
      char* exit_date = argv[7];
      if (!is_valid_date_string(exit_date) && !is_unspecified_date_string(exit_date)) {
        return INVALID_COMMAND;
      }
      else {
        if (!is_unspecified_date_string(exit_date)) {
          /* Check if exit date is earlier than the entry date */
          if (compare_date_strings(entry_date, exit_date) > 0)
            return INVALID_COMMAND;
        }
      }
    }
  }
  return VALID_COMMAND;
}

int execute_insert_patient_record(char** argv, int read_flag) {
  patient_record_ptr patient_record = patient_record_create(argv);
  void* result = hash_table_find(patient_record_ht, patient_record->record_id);
  /* If record Id not found */
  if (result == NULL) {
    /* Update patient record hash table */
    hash_table_insert(&patient_record_ht, patient_record->record_id, patient_record);
    /* Search if patient record disease id exists */
    result = hash_table_find(disease_ht, patient_record->disease_id);
    if (result == NULL) {
      /* Store disease to global diseases_names list */
      list_push_front(&diseases_names, &patient_record->disease_id);
      /* If not found create a new AVL tree to store pointers to patient record */
      avl_ptr new_disease_avl = avl_create(patient_record_compare,
                                           patient_record_print);
      avl_insert(&new_disease_avl, patient_record);
      /* Update disease hash table */
      hash_table_insert(&disease_ht, patient_record->disease_id, new_disease_avl);
    } else {
      /* Update disease hash table by insert patient_record pointer to AVL tree */
      avl_ptr disease_avl = (avl_ptr) result;
      avl_insert(&disease_avl, patient_record);
    }
    /* Search if patient record country exists */
    result = hash_table_find(country_ht, patient_record->country);
    if (result == NULL) {
      /* Store country to global countries_names list */
      list_push_front(&countries_names, &patient_record->country);
      /* If not found create a new AVL tree to store pointers to patient record */
      avl_ptr new_country_avl = avl_create(patient_record_compare,
                                           patient_record_print);
      avl_insert(&new_country_avl, patient_record);
      /* Update country hash table */
      hash_table_insert(&country_ht, patient_record->country, new_country_avl);
    } else {
      /* Update country hash table by insert patient_record pointer to AVL tree */
      avl_ptr country_avl = (avl_ptr) result;
      avl_insert(&country_avl, patient_record);
    }
    if (!read_flag) {
      printf("Record added\n"); 
    }
  } else {
    report_warning("Patient record with Record ID: <%s> already exists. ",
                   patient_record->record_id);
    /* Delete patient record and return */
    patient_record_delete(patient_record);
    return EXIT;
  }
  return CONTINUE;
}

int validate_record_patient_exit(int argc, char** argv) {
  if (argc != 3) {
    return INVALID_COMMAND;
  } else {
    /* record_id: Only letters and numbers */
    char* record_id = argv[1];
    if (!is_alpharithmetic(record_id))
      return INVALID_COMMAND;
    /* exit_date: DD-MM-YYYY format  */
    char* exit_date = argv[2];
    if (!is_valid_date_string(exit_date)) {
      return INVALID_COMMAND;
    }
  }
  return VALID_COMMAND;
}

void execute_record_patient_exit(char** argv) {
  /* Search if patient record country exists */
  void* result = hash_table_find(patient_record_ht, argv[0]);
  if (result == NULL) {
    report_warning("Patient record with Record ID: <%s> not found.", argv[0]);
  } else {
    // Cast result to patient record pointer
    patient_record_ptr patient_record = (patient_record_ptr) result;
    /* Convert exit_date struct tm to buffer */
    char exit_date_buffer[BUFFER_SIZE];
    strftime(exit_date_buffer, sizeof(exit_date_buffer), "%d-%m-%Y", &patient_record->exit_date);
    /* Convert entry_date struct tm to buffer */
    char entry_date_buffer[BUFFER_SIZE];
    strftime(entry_date_buffer, sizeof(entry_date_buffer), "%d-%m-%Y", &patient_record->entry_date);
    /* exit_date has to be greater than entry_date */
    if (compare_date_strings(entry_date_buffer, argv[1]) > 0) {
      report_warning("Entry Date [%s] is after the given Exit Date [%s].",
                      entry_date_buffer, argv[1]);
      return;
    }
    /* Check if exit date is not specified */
    if (!strcmp(exit_date_buffer, EXIT_DATE_NOT_SPECIFIED)) {
      // Update exit day of patient record with the given record id
      memset(&patient_record->exit_date, 0, sizeof(struct tm));
      strptime(argv[1], "%d-%m-%Y", &patient_record->exit_date);
      // Print success message
      printf("Record updated\n");
      patient_record_print(patient_record, stdout);
    } else {
      report_warning("Patient record Exit Date with Record ID: "
                     "<%s> is already specified.", argv[0]);
    }
  }
}

int validate_num_current_patients(int argc, char** argv) {
  if (argc != 1 && argc != 2) {
    return INVALID_COMMAND;
  } else {
    if (argc == 2) {
      /* disease_id: Only letters, numbers and maybe character '-' */
      char* disease_id = argv[1];
      for (size_t i = 0; i < strlen(disease_id); ++i) {
        if (!isalnum(disease_id[i]) && disease_id[i] != '-')
          return INVALID_COMMAND;
      }
    }
  }
  return VALID_COMMAND;
}

static inline
int __count_current_patients(avl_node_ptr current_root, int* counter) {
  avl_node_ptr temp = current_root;
  if (temp != NULL) {
    __count_current_patients(temp->left_, counter);
    patient_record_ptr patient_record = (patient_record_ptr) temp->data_;
    if (is_unspecified_date_tm(patient_record->exit_date)) {
      (*counter)++;
    }
    __count_current_patients(temp->right_, counter);
  }
}

static inline
int __num_current_patients_util(avl_ptr disease_avl) {
  int counter = 0;
  /*
    Traverse inorder the AVL Tree and each time a patient record with unspecified
    exit date is found increase the counter for current patients
  */
   __count_current_patients(disease_avl->root_, &counter);
  return counter;
}

void execute_num_current_patients(int argc, char** argv) {
  if (argc == 0) {
    /* Print for every disease the number of patients that are still in hospital */
    for (size_t i = 1U; i <= list_size(diseases_names); ++i) {
      /* Get every disease id */
      list_node_ptr list_node = list_get(diseases_names, i);
      char* disease_id = (*(char**) list_node->data_);
      /* Get for the current disease its AVL tree */
      void* result = hash_table_find(disease_ht, disease_id);
      if (result == NULL) {
        report_warning("There is no disease recorded with Disease ID: <%s>",
                       disease_id);
      } else {
        /* Cast result to avl pointer */
        avl_ptr disease_avl = (avl_ptr) result;
        /* Execute main algorithm to print current patients */
        int no_current_patients = __num_current_patients_util(disease_avl);
        printf("%s %d\n", disease_id, no_current_patients);
      }
    }
  } else {
    /* Get for the current disease its AVL tree */
    void* result = hash_table_find(disease_ht, argv[0]);
    if (result == NULL) {
      report_warning("There is no disease recorded with Disease ID: <%s>", argv[0]);
    } else {
      /* Cast result to avl pointer */
      avl_ptr disease_avl = (avl_ptr) result;
      /* Execute main algorithm to find number of current patients */
      int no_current_patients = __num_current_patients_util(disease_avl);
      printf("%s %d\n", argv[0], no_current_patients);
    }
  }
  /* Print a new line for better output format in console */
  printf("\n");
}

int validate_exit(int argc, char** argv) {
  return argc == 1 ? VALID_COMMAND : INVALID_COMMAND;
}

void execute_exit() {
  printf("exiting\n");
  /* Free all memory allocated by the program */
  hash_table_clear(patient_record_ht);
  hash_table_clear(disease_ht);
  hash_table_clear(country_ht);
  list_clear(diseases_names);
  list_clear(countries_names);
  exit(EXIT_SUCCESS);
}
