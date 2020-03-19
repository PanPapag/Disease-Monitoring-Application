#include <assert.h>
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../includes/macros.h"
#include "../includes/patient_record.h"
#include "../includes/utils.h"

patient_record_ptr patient_record_create(char** patient_record_tokens) {
  /* Allocate memory for patient_record_ptr */
  patient_record_ptr patient_record = malloc(sizeof(*patient_record));
  /* Allocate memory and store record_id */
  patient_record->record_id = (char*) malloc((strlen(patient_record_tokens[0]) + 1) * sizeof(char));
  strcpy(patient_record->record_id, patient_record_tokens[0]);
  /* Allocate memory and store patient_first_name */
  patient_record->patient_first_name = (char*) malloc((strlen(patient_record_tokens[1]) + 1) * sizeof(char));
  strcpy(patient_record->patient_first_name, patient_record_tokens[1]);
  /* Allocate memory and store patient_last_name */
  patient_record->patient_last_name = (char*) malloc((strlen(patient_record_tokens[2]) + 1) * sizeof(char));
  strcpy(patient_record->patient_last_name, patient_record_tokens[2]);
  /* Allocate memory and store disease_id */
  patient_record->disease_id = (char*) malloc((strlen(patient_record_tokens[3]) + 1) * sizeof(char));
  strcpy(patient_record->disease_id, patient_record_tokens[3]);
  /* Allocate memory and store country */
  patient_record->country = (char*) malloc((strlen(patient_record_tokens[4]) + 1) * sizeof(char));
  strcpy(patient_record->country, patient_record_tokens[4]);
  /* Store entry_date using struct tm format */
  memset(&patient_record->entry_date, 0, sizeof(struct tm));
  strptime(patient_record_tokens[5], "%d-%m-%Y", &patient_record->entry_date);
  /* Store exit_date using struct tm format */
  memset(&patient_record->exit_date, 0, sizeof(struct tm));
  strptime(patient_record_tokens[6], "%d-%m-%Y", &patient_record->exit_date);
  /* Return patient record pointer */
  return patient_record;
}

void patient_record_print(FILE* out, void* v) {
  patient_record_ptr patient_record = *((patient_record_ptr *) v);
  char entry_date_buffer[BUFFER_SIZE];
  char exit_date_buffer[BUFFER_SIZE];
  fprintf(out, "Record ID: %s\n", patient_record->record_id);
  fprintf(out, "Patient First Name: %s\n", patient_record->patient_first_name);
  fprintf(out, "Patient Last Name: %s\n", patient_record->patient_last_name);
  fprintf(out, "Disease ID: %s\n", patient_record->disease_id);
  fprintf(out, "Country: %s\n", patient_record->country);
  strftime(entry_date_buffer, sizeof(entry_date_buffer), "%d-%m-%Y", &patient_record->entry_date);
  fprintf(out, "Entry Date: %s\n", entry_date_buffer);
  strftime(exit_date_buffer, sizeof(exit_date_buffer), "%d-%m-%Y", &patient_record->exit_date);
  if (!strcmp(exit_date_buffer, EXIT_DATE_NOT_SPECIFIED)) {
    fprintf(out, "Exit Date: -\n");
  } else {
    fprintf(out, "Exit Date: %s\n", exit_date_buffer);
  }
  printf("\n");
}

int patient_record_compare(void* a, void* b) {
	patient_record_ptr pr1 = *((patient_record_ptr *) a);
	patient_record_ptr pr2 = *((patient_record_ptr *) b);
  char pr1_entry_date_buffer[BUFFER_SIZE];
  char pr2_entry_date_buffer[BUFFER_SIZE];
  strftime(pr1_entry_date_buffer, sizeof(pr1_entry_date_buffer), "%s", &pr1->entry_date);
  strftime(pr2_entry_date_buffer, sizeof(pr2_entry_date_buffer), "%s", &pr2->entry_date);
  size_t pr1_entry_date_to_secs;
  size_t pr2_entry_date_to_secs;
  string_to_int64(pr1_entry_date_buffer, (int64_t *) &pr1_entry_date_to_secs);
  string_to_int64(pr2_entry_date_buffer, (int64_t *) &pr2_entry_date_to_secs);
	return pr1_entry_date_to_secs - pr2_entry_date_to_secs;
}

void patient_record_delete(void* v) {
  patient_record_ptr patient_record = *((patient_record_ptr *) v);
  if (patient_record != NULL) {
    __FREE(patient_record->record_id);
    __FREE(patient_record->disease_id);
    __FREE(patient_record->patient_first_name);
    __FREE(patient_record->patient_last_name);
    __FREE(patient_record);
  }
}

int validate_patient_record_tokens(char** patient_record_tokens) {
  /* record_id: Only letters and numbers */
  char* record_id = patient_record_tokens[0];
  for (size_t i = 0; i < strlen(record_id); ++i) {
    if (!isalnum(record_id[i]))
      return INVALID_RECORD_ID;
  }
  /* patient_first_name: Only letters */
  char* patient_first_name = patient_record_tokens[1];
  for (size_t i = 0; i < strlen(patient_first_name); ++i) {
    if (!isalpha(patient_first_name[i]))
      return INVALID_PATIENT_FIRST_NAME;
  }
  /* patient_last_name: Only letters */
  char* patient_last_name = patient_record_tokens[2];
  for (size_t i = 0; i < strlen(patient_last_name); ++i) {
    if (!isalpha(patient_last_name[i]))
      return INVALID_PATIENT_LAST_NAME;
  }
  /* disease_id: Only letters, numbers and maybe character '-' */
  char* disease_id = patient_record_tokens[3];
  for (size_t i = 0; i < strlen(disease_id); ++i) {
    if (!isalnum(disease_id[i]) && disease_id[i] != '-')
      return INVALID_DISEASE_ID;
  }
  /* country: Only letters */
  char* country = patient_record_tokens[4];
  for (size_t i = 0; i < strlen(country); ++i) {
    if (!isalpha(country[i]))
      return INVALID_COUNTRY;
  }
  /* entry_date: DD-MM-YYYY format */
  char* entry_date = patient_record_tokens[5];
  if (strlen(entry_date) == 10) {
    for (size_t i = 0; i < strlen(entry_date); ++i) {
      if (i == 2 || i == 5) {
        if (entry_date[i] != '-')
          return INVALID_ENTRY_DATE;
      } else {
        if (!isdigit(entry_date[i]))
          return INVALID_ENTRY_DATE;
      }
    }
  } else {
    return INVALID_ENTRY_DATE;
  }
  /* exit_date: DD-MM-YYYY format or - (not specified) */
  char* exit_date = patient_record_tokens[6];
  if (strlen(exit_date) == 1) {
    if (exit_date[0] != '-')
      return INVALID_EXIT_DATE;
  }
  else if (strlen(exit_date) == 10) {
    for (size_t i = 0; i < strlen(exit_date); ++i) {
      if (i == 2 || i == 5) {
        if (exit_date[i] != '-')
          return INVALID_EXIT_DATE;
      } else {
        if (!isdigit(exit_date[i]))
          return INVALID_EXIT_DATE;
      }
    }
    /* Check if exit date is earlier than the entry date */
    // Convert entry_date string to struct tm
    struct tm entry_date_tm;
    memset(&entry_date_tm, 0, sizeof(struct tm));
    strptime(entry_date, "%d-%m-%Y", &entry_date_tm);
    // Convert exit_date string to struct tm
    struct tm exit_date_tm;
    memset(&exit_date_tm, 0, sizeof(struct tm));
    strptime(exit_date, "%d-%m-%Y", &exit_date_tm);
    // Convert entry_date_tm back to seconds
    char entry_date_buf[11];
    strftime(entry_date_buf, sizeof(entry_date_buf), "%s", &entry_date_tm);
    size_t entry_date_to_secs;
    string_to_int64(entry_date_buf, (int64_t *) &entry_date_to_secs);
    // Convert entry_date_tm back to seconds
    char exit_date_buf[11];
    strftime(exit_date_buf, sizeof(exit_date_buf), "%s", &exit_date_tm);
    size_t exit_date_to_secs;
    string_to_int64(exit_date_buf, (int64_t *) &exit_date_to_secs);
    // Check if exit date is valid
    if (exit_date_to_secs < entry_date_to_secs)
      return INVALID_EXIT_DATE;
  } else {
    return INVALID_EXIT_DATE;
  }
  /* Everything fine return success */
  return VALID_PATIENT_RECORD;
}
