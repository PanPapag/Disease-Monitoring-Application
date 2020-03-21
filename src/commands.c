#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../includes/avl.h"
#include "../includes/commands.h"
#include "../includes/hash_table.h"
#include "../includes/io_utils.h"
#include "../includes/utils.h"
#include "../includes/patient_record.h"

hash_table_ptr patient_record_ht;
hash_table_ptr disease_ht;
hash_table_ptr country_ht;

list_ptr diseases_names;

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
  }
  return VALID_COMMAND;
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
    }
  }
  return VALID_COMMAND;
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
    }
  }
  return VALID_COMMAND;
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
    }
  }
  return VALID_COMMAND;
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

int execute_insert_patient_record(char** argv) {
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
    /* Convert struct tm to buffer */
    char exit_date_buffer[BUFFER_SIZE];
    strftime(exit_date_buffer, sizeof(exit_date_buffer), "%d-%m-%Y", &patient_record->exit_date);
    /* Check if exit date is not specified */
    if (!strcmp(exit_date_buffer, EXIT_DATE_NOT_SPECIFIED)) {
      // Update exit day of patient record with the given record id
      memset(&patient_record->exit_date, 0, sizeof(struct tm));
      strptime(argv[1], "%d-%m-%Y", &patient_record->exit_date);
      // Print success message
      printf("\nCommand <recordPatientExit> executed.\n");
      printf("Patient record with Record ID: <%s> updated successfully.\n\n", argv[0]);
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
int __count_current_patients(avl_node_ptr current_root, int* counter, int flag) {
  avl_node_ptr temp = current_root;
  if (temp != NULL) {
    __count_current_patients(temp->left_, counter, flag);
    patient_record_ptr patient_record = (patient_record_ptr) temp->data_;
    if (is_unspecified_date_tm(patient_record->exit_date)) {
      if (flag) {
        patient_record_print(patient_record, stdout);
      }
      (*counter)++;
    }
    __count_current_patients(temp->right_, counter, flag);
  }
}

static inline
int __num_current_patients_util(avl_ptr disease_avl, char* disease_id, int flag) {
  int counter = 0;
  /*
    Traverse inorder the AVL Treeand each time a patient record with unspecified
    exit date is found increase the counter for current patients
  */
  printf("--------------------------------------------------------\n");
  if (flag) {
    printf("Disease: <%s> - Current patients\n\n", disease_id);
  }
   __count_current_patients(disease_avl->root_, &counter, flag);
  return counter;
}

void execute_num_current_patients(int argc, char** argv) {
  printf("\nCommand <numCurrentPatients> executed.\n\n");
  printf("--------------------------------------------------------\n");
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
        int no_current_patients = __num_current_patients_util(disease_avl, disease_id, 1);
        if (no_current_patients == 0) {
          printf("No are patients are currently affected by <%s>.\n\n",disease_id);
        }
      }
      printf("--------------------------------------------------------\n");
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
      int no_current_patients = __num_current_patients_util(disease_avl, argv[0], 0);
      printf("Disease: <%s> - Number of current patients: <%d>\n\n",
             argv[0], no_current_patients);
      printf("--------------------------------------------------------\n");
    }
  }
  printf("--------------------------------------------------------\n");
}

int validate_exit(int argc, char** argv) {
  return argc == 1 ? VALID_COMMAND : INVALID_COMMAND;
}

void execute_exit() {
  printf("\nExiting..\n\n");
  /* Free all memory allocated by the program */
  hash_table_clear(patient_record_ht);
  hash_table_clear(disease_ht);
  hash_table_clear(country_ht);
  list_clear(diseases_names);
  exit(EXIT_SUCCESS);
}
