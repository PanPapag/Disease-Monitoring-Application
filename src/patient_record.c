#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../includes/macros.h"
#include "../includes/patient_record.h"
#include "../includes/utils.h"

void patient_record_print(FILE* out, void* v) {
  patient_record_ptr patient_record = *((patient_record_ptr *) v);
  char entry_date_buffer[BUFFER_SIZE];
  char exit_date_buffer[BUFFER_SIZE];
  fprintf(out, "Record ID: %s\n", patient_record->record_id);
  fprintf(out, "Patient First Name: %s\n", patient_record->patient_first_name);
  fprintf(out, "Patient Last Name: %s\n", patient_record->patient_last_name);
  fprintf(out, "Disease ID: %s\n", patient_record->disease_id);
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
