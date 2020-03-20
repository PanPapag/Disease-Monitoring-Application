#include <assert.h>
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../includes/macros.h"
#include "../includes/patient_record.h"
#include "../includes/io_utils.h"
#include "../includes/utils.h"

patient_record_ptr patient_record_create(char** patient_record_tokens) {
  /* Allocate memory for patient_record_ptr */
  patient_record_ptr patient_record = malloc(sizeof(*patient_record));
  if (patient_record == NULL) {
    report_error("Could not allocate memory for Patient Record. Exiting...");
    exit(EXIT_FAILURE);
  }
  /* Allocate memory and store record_id */
  patient_record->record_id = (char*) malloc((strlen(patient_record_tokens[0]) + 1) * sizeof(char));
  if (patient_record->record_id == NULL) {
    report_error("Could not allocate memory for Patient Record Record ID. Exiting...");
    exit(EXIT_FAILURE);
  }
  strcpy(patient_record->record_id, patient_record_tokens[0]);
  /* Allocate memory and store patient_first_name */
  patient_record->patient_first_name = (char*) malloc((strlen(patient_record_tokens[1]) + 1) * sizeof(char));
  if (patient_record->patient_first_name == NULL) {
    report_error("Could not allocate memory for Patient Record First Name. Exiting...");
    exit(EXIT_FAILURE);
  }
  strcpy(patient_record->patient_first_name, patient_record_tokens[1]);
  /* Allocate memory and store patient_last_name */
  patient_record->patient_last_name = (char*) malloc((strlen(patient_record_tokens[2]) + 1) * sizeof(char));
  if (patient_record->patient_last_name == NULL) {
    report_error("Could not allocate memory for Patient Record Last Name. Exiting...");
    exit(EXIT_FAILURE);
  }
  strcpy(patient_record->patient_last_name, patient_record_tokens[2]);
  /* Allocate memory and store disease_id */
  patient_record->disease_id = (char*) malloc((strlen(patient_record_tokens[3]) + 1) * sizeof(char));
  if (patient_record->disease_id == NULL) {
    report_error("Could not allocate memory for Patient Record Disease ID. Exiting...");
    exit(EXIT_FAILURE);
  }
  strcpy(patient_record->disease_id, patient_record_tokens[3]);
  /* Allocate memory and store country */
  patient_record->country = (char*) malloc((strlen(patient_record_tokens[4]) + 1) * sizeof(char));
  if (patient_record->country == NULL) {
    report_error("Could not allocate memory for Patient Record Country. Exiting...");
    exit(EXIT_FAILURE);
  }
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

void patient_record_print(void* v, FILE* out) {
  patient_record_ptr patient_record = (patient_record_ptr *) v;
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

int64_t patient_record_compare(void* a, void* b) {
	patient_record_ptr pr1 = (patient_record_ptr *) a;
	patient_record_ptr pr2 = (patient_record_ptr *) b;
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
  patient_record_ptr patient_record = (patient_record_ptr *) v;
  if (patient_record != NULL) {
    __FREE(patient_record->record_id);
    __FREE(patient_record->disease_id);
    __FREE(patient_record->patient_first_name);
    __FREE(patient_record->patient_last_name);
    __FREE(patient_record->country);
    __FREE(patient_record);
  }
}

int validate_patient_record_tokens(char** patient_record_tokens) {
  /* record_id: Only letters and numbers */
  char* record_id = patient_record_tokens[0];
  if (!is_alpharithmetic(record_id))
    return INVALID_RECORD_ID;
  /* patient_first_name: Only letters */
  char* patient_first_name = patient_record_tokens[1];
  if (!is_alphabetical(patient_first_name))
    return INVALID_PATIENT_FIRST_NAME;
  /* patient_last_name: Only letters */
  char* patient_last_name = patient_record_tokens[2];
  if (!is_alphabetical(patient_last_name))
    return INVALID_PATIENT_LAST_NAME;
  /* disease_id: Only letters, numbers and maybe character '-' */
  char* disease_id = patient_record_tokens[3];
  for (size_t i = 0; i < strlen(disease_id); ++i) {
    if (!isalnum(disease_id[i]) && disease_id[i] != '-')
      return INVALID_DISEASE_ID;
  }
  /* country: Only letters */
  char* country = patient_record_tokens[4];
  if (!is_alphabetical(country))
    return INVALID_COUNTRY;
  /* entry_date: DD-MM-YYYY format */
  char* entry_date = patient_record_tokens[5];
  if (!is_valid_date(entry_date))
    return INVALID_ENTRY_DATE;
  /* exit_date: DD-MM-YYYY format or - (not specified) */
  char* exit_date = patient_record_tokens[6];
  if (!is_valid_date(exit_date) && !is_unspecified_date(exit_date)) {
    return INVALID_EXIT_DATE;
  }
  else {
    if (!is_unspecified_date(exit_date)) {
      /* Check if exit date is earlier than the entry date */
      if (compare_date(entry_date, exit_date) > 0)
        return INVALID_EARLIER_EXIT_DATE;
    }
  }
  /* Everything fine return success */
  return VALID_PATIENT_RECORD;
}

void print_patient_record_error(char** patient_record_tokens, int code) {
  switch (code) {
    case INVALID_RECORD_ID:
      report_warning("Invalid Record ID: <%s> format. Discarding patient record.",
                     patient_record_tokens[0]);
      break;
    case INVALID_PATIENT_FIRST_NAME:
      report_warning("Invalid Patient First Name: <%s> format. Discarding patient record.",
                     patient_record_tokens[1]);
      break;
    case INVALID_PATIENT_LAST_NAME:
      report_warning("Invalid Patient Last Name: <%s> format. Discarding patient record.",
                     patient_record_tokens[2]);
      break;
    case INVALID_DISEASE_ID:
      report_warning("Invalid Disease ID: <%s> format. Discarding patient record.",
                     patient_record_tokens[3]);
      break;
    case INVALID_COUNTRY:
      report_warning("Invalid Country: <%s> format. Discarding patient record.",
                     patient_record_tokens[4]);
      break;
    case INVALID_ENTRY_DATE:
      report_warning("Invalid Entry Date: <%s> format. Discarding patient record.",
                     patient_record_tokens[5]);
      break;
    case INVALID_EXIT_DATE:
      report_warning("Invalid Exit Date: <%s> format. Discarding patient record.",
                     patient_record_tokens[6]);
      break;
    case INVALID_EARLIER_EXIT_DATE:
      report_warning("Invalid Exit Date: <%s> is earlier than Entry Date <%s>. "
                     "Discarding patient record.",
                     patient_record_tokens[6], patient_record_tokens[5]);
      break;
  }
}
