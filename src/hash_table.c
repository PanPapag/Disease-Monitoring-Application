#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../includes/hash_table.h"
#include "../includes/utils.h"

static inline
bucket_ptr __bucket_create(size_t bucket_entries, size_t key_size, size_t value_size) {
  bucket_ptr bucket = (bucket_ptr) malloc(sizeof(*bucket));
  bucket->bucket_entries_ = bucket_entries;
  bucket->table_ = (bucket_entry_t*) malloc(bucket_entries * sizeof(bucket_entry_t));
  for (size_t i = 0U; i != bucket_entries; ++i) {
    bucket->table_[i].key_ = malloc(key_size);
    bucket->table_[i].value_ = malloc(value_size);
  }
  return bucket;
}

static inline
void __bucket_clear(bucket_ptr bucket) {
  for (size_t i = 0U; i != bucket->bucket_entries_; ++i) {
    free(bucket->table_[i].key_);
    free(bucket->table_[i].value_);
  }
  free(bucket->table_);
  free(bucket);
}

hash_table_ptr __hash_table_create(size_t key_size, size_t value_size,
                                   size_t ht_entries, size_t bucket_size,
                                   size_t (*hash_func)(const void*, const size_t),
                                   int (*cmp_func)(void*, void*),
                                   void (*key_print_func)(FILE*, void*),
                                   void (*value_print_func)(FILE*, void*)) {

  size_t entries_per_bucket_node_ = bucket_size / (key_size + value_size);
  hash_table_ptr hash_table = malloc(sizeof(*hash_table));
  hash_table->key_size_ = value_size;
  hash_table->value_size_ = value_size;
  hash_table->ht_entries_ = ht_entries;
  hash_table->table_ = (list_ptr*) malloc(ht_entries * sizeof(list_ptr));
  hash_table->ht_hash_func_ = hash_func;
  hash_table->ht_key_print_func_ = key_print_func;
  hash_table->ht_value_print_func_ = value_print_func;
  for (size_t i = 0U; i != ht_entries; ++i) {
    hash_table->table_[i] = list_create(bucket_ptr, cmp_func, value_print_func);
    bucket_ptr bucket = __bucket_create(entries_per_bucket_node_, key_size, value_size);
    list_push_front(&hash_table->table_[i], &bucket);
  }
  return hash_table;
}

void hash_table_clear(hash_table_ptr hash_table) {
  for (size_t i = 0U; i != hash_table->ht_entries_; ++i) {
    for (size_t j = 0U; j != list_size(hash_table->table_[i]); ++j) {
      list_node_ptr list_node = list_pop_front(&hash_table->table_[i]);
      bucket_ptr bucket = *((bucket_ptr) list_node->data_); //TODO FIX IT
      printf("ENTRIES OUT: %d\n",bucket->bucket_entries_); 
      //__bucket_clear(bucket);
    }
    list_clear(hash_table->table_[i]);
  }
  free(hash_table->table_);
  free(hash_table);
}
