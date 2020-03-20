#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../includes/hash_table.h"
#include "../includes/io_utils.h"
#include "../includes/utils.h"

static inline
bucket_ptr __bucket_create(hash_table_ptr hash_table, size_t bucket_entries) {
  bucket_ptr bucket = (bucket_ptr) malloc(sizeof(*bucket));
  if (bucket == NULL) {
    report_error("Could not allocate memory for Hash Table Bucket. Exiting...");
    exit(EXIT_FAILURE);
  }
  bucket->bucket_size_ = 0U;
  bucket->table_ = (bucket_entry_ptr) malloc(hash_table->bucket_entries_ * sizeof(bucket_entry_t));
  if (bucket->table_ == NULL) {
    report_error("Could not allocate memory for Hash Table Bucket Table. Exiting...");
    exit(EXIT_FAILURE);
  }
  for (size_t i = 0U; i != hash_table->bucket_entries_; ++i) {
    bucket->table_[i].key_ = NULL;
    bucket->table_[i].value_ = NULL;
  }
  return bucket;
}

static inline
void __bucket_clear(hash_table_ptr hash_table, bucket_ptr bucket) {
  for (size_t i = 0U; i != hash_table->bucket_entries_; ++i) {
    if (bucket->table_[i].key_ != NULL) {
      if (hash_table->ht_key_delete_func_ != NULL) {
        hash_table->ht_key_delete_func_(bucket->table_[i].key_);
      }
      if (hash_table->ht_value_delete_func_ != NULL) {
        hash_table->ht_value_delete_func_(bucket->table_[i].value_);
      }
    }
  }
  free(bucket->table_);
  free(bucket);
}

static inline
int __bucket_insert(hash_table_ptr hash_table, bucket_ptr* bucket, void* key, void* value) {
  /* If bucket is full return FAIL */
  if ((*bucket)->bucket_size_ == hash_table->bucket_entries_) {
    return FAIL;
  }
  /* Else search for an empty position */
  for (size_t i = 0U; i != hash_table->bucket_entries_; ++i) {
    if ((*bucket)->table_[i].key_ == NULL) {
      (*bucket)->table_[i].key_ = key;
      (*bucket)->table_[i].value_ = value;
      (*bucket)->bucket_size_++;
      return SUCCESS;
    }
  }
}

static inline
void* __bucket_find(hash_table_ptr hash_table, bucket_ptr bucket, void* key) {
  for (size_t i = 0U; i != hash_table->bucket_entries_; ++i) {
    if (bucket->table_[i].key_ != NULL) {
      int res = hash_table->ht_key_cmp_func_(bucket->table_[i].key_, key);
      if (!res) {
        return bucket->table_[i].value_;
      }
    }
  }
  return NULL;
}

static inline
void __bucket_print(hash_table_ptr hash_table, bucket_ptr bucket, FILE* out) {
  for (size_t i = 0U; i != hash_table->bucket_entries_; ++i) {
    if (bucket->table_[i].key_ != NULL) {
      if (hash_table->ht_key_print_func_ != NULL) {
        printf("------------ KEY ------------\n");
        hash_table->ht_key_print_func_(bucket->table_[i].key_, out);
        printf("-----------------------------\n");
      }
      if (hash_table->ht_value_print_func_ != NULL) {
        printf("----------- VALUE -----------\n");
        hash_table->ht_value_print_func_(bucket->table_[i].value_, out);
        printf("-----------------------------\n");
      }
    }
  }
}

hash_table_ptr hash_table_create(size_t ht_entries, size_t bucket_size,
                                 size_t (*hash_func)(const void*),
                                 int (*key_cmp_func)(void*, void*),
                                 void (*key_print_func)(void*, FILE*),
                                 void (*value_print_func)(void*, FILE*),
                                 void (*key_delete_func)(void*),
                                 void (*value_delete_func)(void*)) {

  /* Allocate memory for the newly created hash table */
  hash_table_ptr hash_table = (hash_table_ptr) malloc(sizeof(*hash_table));
  if (hash_table == NULL) {
    report_error("Could not allocate memory for Hash Table Data Structure. Exiting...");
    exit(EXIT_FAILURE);
  }
  /* Initialize hash table */
  hash_table->ht_entries_ = ht_entries;
  /* Each bucket stores two void pointers size of 8 bytes, so in total 16 bytes */
  hash_table->bucket_entries_ = bucket_size / 16;
  hash_table->ht_hash_func_ = hash_func;
  hash_table->ht_key_cmp_func_ = key_cmp_func;
  hash_table->ht_key_print_func_ = key_print_func;
  hash_table->ht_value_print_func_ = value_print_func;
  hash_table->ht_key_delete_func_ = key_delete_func;
  hash_table->ht_value_delete_func_ = value_delete_func;
  hash_table->table_ = (list_ptr*) malloc(ht_entries * sizeof(list_ptr));
  if (hash_table->table_ == NULL) {
    report_error("Could not allocate memory for Hash Table Table. Exiting...");
    exit(EXIT_FAILURE);
  }
  for (size_t i = 0U; i != ht_entries; ++i) {
    hash_table->table_[i] = list_create(bucket_ptr, NULL, NULL, NULL);
    bucket_ptr bucket = __bucket_create(hash_table, hash_table->bucket_entries_);
    list_push_front(&hash_table->table_[i], &bucket);
  }
  return hash_table;
}

void hash_table_clear(hash_table_ptr hash_table) {
  for (size_t i = 0U; i != hash_table->ht_entries_; ++i) {
    size_t no_buckets = list_size(hash_table->table_[i]);
    for (size_t j = 0U; j != no_buckets; ++j) {
      list_node_ptr list_node = list_pop_front(&hash_table->table_[i]);
      bucket_ptr bucket = *((bucket_ptr*) list_node->data_);
      __bucket_clear(hash_table, bucket);
      free(list_node);
    }
    list_clear(hash_table->table_[i]);
  }
  free(hash_table->table_);
  free(hash_table);
}

int hash_table_insert(hash_table_ptr* hash_table, void* key, void* value) {
  void* result = hash_table_find(*hash_table, key);
  if (result == NULL) {
    size_t pos = (*hash_table)->ht_hash_func_(key) % (*hash_table)->ht_entries_;
    list_ptr entry_bucket_chain = (*hash_table)->table_[pos];
    size_t entry_no_buckets = list_size(entry_bucket_chain);
    for (size_t i = 1U; i <= entry_no_buckets; ++i) {
      list_node_ptr list_node = list_get(entry_bucket_chain, i);
      if (list_node == NULL) {
        report_warning("Bucket chain index is out of bounds!");
        break;
      } else {
        bucket_ptr entry_bucket = *((bucket_ptr*) list_node->data_);
        int res = __bucket_insert(*hash_table, &entry_bucket, key, value);
        if (res == FAIL) {
          bucket_ptr new_bucket = __bucket_create(*hash_table, (*hash_table)->bucket_entries_);
          __bucket_insert(*hash_table, &new_bucket, key, value);
          list_push_back(&(*hash_table)->table_[pos], &new_bucket);
        }
        return SUCCESS;
      }
    }
  }
  return FAIL;
}

void* hash_table_find(hash_table_ptr hash_table, void* key) {
  size_t pos = hash_table->ht_hash_func_(key) % hash_table->ht_entries_;
  list_ptr entry_bucket_chain = hash_table->table_[pos];
  size_t entry_no_buckets = list_size(entry_bucket_chain);
  for (size_t i = 1U; i <= entry_no_buckets; ++i) {
    list_node_ptr list_node = list_get(entry_bucket_chain, i);
    if (list_node == NULL) {
      report_warning("Bucket chain index is out of bounds!");
      break;
    } else {
      bucket_ptr entry_bucket = *((bucket_ptr*) list_node->data_);
      void* result =  __bucket_find(hash_table, entry_bucket, key);
      if (result != NULL)
        return result;
    }
  }
  return NULL;
}

void hash_table_print(hash_table_ptr hash_table, FILE* out) {
  for (size_t i = 0U; i < hash_table->ht_entries_; ++i) {
    list_ptr entry_bucket_chain = hash_table->table_[i];
    size_t entry_no_buckets = list_size(entry_bucket_chain);
    for (size_t j = 1U; j <= entry_no_buckets; ++j) {
      list_node_ptr list_node = list_get(entry_bucket_chain, j);
      if (list_node == NULL) {
        report_warning("Bucket chain index is out of bounds!");
        break;
      } else {
        bucket_ptr entry_bucket = *((bucket_ptr*) list_node->data_);
        __bucket_print(hash_table, entry_bucket, out);
      }
    }
  }
}
