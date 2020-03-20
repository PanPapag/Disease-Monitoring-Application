#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../includes/avl.h"
#include "../includes/commands.h"
#include "../includes/hash_table.h"
#include "../includes/utils.h"
#include "../includes/patient_record.h"

hash_table_ptr patient_record_ht;

int validate_global_disease_stats(int argc, char** argv) {
  if (argc != 1 && argc != 3) {
    return INVALID_COMMAND;
  }
  if (argc == 1) {
    return VALID_COMMAND;
  } else {
    char* date1 = argv[1];
    char* date2 = argv[2];
    if (!is_valid_date(date1) || !is_valid_date(date2))
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
      if (!is_valid_date(date1))
        return INVALID_COMMAND;
      // Check date2
      char* date2 = argv[3];
      if (!is_valid_date(date2))
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
      if (!is_valid_date(date1))
        return INVALID_COMMAND;
      // Check date2
      char* date2 = argv[4];
      if (!is_valid_date(date2))
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
      if (!is_valid_date(date1))
        return INVALID_COMMAND;
      // Check date2
      char* date2 = argv[4];
      if (!is_valid_date(date2))
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
      if (!is_valid_date(date1))
        return INVALID_COMMAND;
      // Check date2
      char* date2 = argv[4];
      if (!is_valid_date(date2))
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
    if (!is_valid_date(entry_date))
      return INVALID_COMMAND;
    if (argc == 8) {
      /* exit_date: DD-MM-YYYY format or - (not specified) */
      char* exit_date = argv[7];
      if (!is_valid_date(exit_date) && !is_unspecified_date(exit_date)) {
        return INVALID_COMMAND;
      }
      else {
        if (!is_unspecified_date(exit_date)) {
          /* Check if exit date is earlier than the entry date */
          if (compare_date(entry_date, exit_date) > 0)
            return INVALID_COMMAND;
        }
      }
    }
  }
  return VALID_COMMAND;
}

void execute_insert_patient_record(int argc, char** argv) {
  patient_record_ptr patient_record = patient_record_create(argv);
  void* result = hash_table_find(patient_record_ht, patient_record->record_id);
  /* If record Id not found */
  if (result == NULL) {
    /* Update patient record hash table */
    hash_table_insert(&patient_record_ht, patient_record->record_id, patient_record);
    /* Update disease hash table */

    /* Update country hash table */
  } else {
    report_warning("Patient record with Record ID: <%s> already exists. "
                   "Discarding patient record.", patient_record->record_id);
  }
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
    if (!is_valid_date(exit_date)) {
      return INVALID_COMMAND;
    }
  }
  return VALID_COMMAND;
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

int validate_exit(int argc, char** argv) {
  return argc == 1 ? VALID_COMMAND : INVALID_COMMAND;
}
