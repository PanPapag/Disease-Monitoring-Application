#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../includes/hash_table.h"
#include "../includes/io_utils.h"
#include "../includes/utils.h"

static inline
bucket_ptr __bucket_create(size_t bucket_entries,
                           int (*key_cmp_func)(void*, void*),
                           int (*key_print_func)(FILE*, void*),
                           int (*value_print_func)(FILE*, void*),
                           void (*key_delete_func)(void*),
                           void (*value_delete_func)(void*),
                           size_t key_size, size_t value_size) {

  bucket_ptr bucket = (bucket_ptr) malloc(sizeof(*bucket));
  bucket->key_size_ = value_size;
  bucket->value_size_ = value_size;
  bucket->bucket_entries_ = bucket_entries;
  bucket->bucket_key_cmp_func_ = key_cmp_func;
  bucket->bucket_key_print_func_ = key_print_func;
  bucket->bucket_value_print_func_ = value_print_func;
  bucket->bucket_key_delete_func_ = key_delete_func;
  bucket->bucket_value_delete_func_ = value_delete_func;
  bucket->table_ = (bucket_entry_t*) malloc(bucket_entries * sizeof(bucket_entry_t));
  for (size_t i = 0U; i != bucket_entries; ++i) {
    bucket->table_[i].key_ = malloc(key_size);
    bucket->table_[i].key_ = NULL;
    bucket->table_[i].value_ = malloc(value_size);
    bucket->table_[i].value_ = NULL;
  }
  return bucket;
}

static inline
int __bucket_insert(bucket_ptr* bucket, void* key, void* value) {
  for (size_t i = 0U; i != (*bucket)->bucket_entries_; ++i) {
    if ((*bucket)->table_[i].key_ == NULL) {
      (*bucket)->table_[i].key_ = key;
      (*bucket)->table_[i].value_ = value;
      printf("K %p\n",(*bucket)->table_[i].key_);
      printf("V %p\n",(*bucket)->table_[i].value_);
      // printf("%s\n",(*(char**)(*bucket)->table_[i].key_));
      // patient_record_print(stdout, (*bucket)->table_[i].value_);
      // printf("--------------------\n");
      // // memcpy((*bucket)->table_[i].key_, key, (*bucket)->key_size_);
      // memcpy((*bucket)->table_[i].value_, value, (*bucket)->value_size_);
      (*bucket)->bucket_key_print_func_(stdout, (*bucket)->table_[i].key_);
      (*bucket)->bucket_value_print_func_(stdout, (*bucket)->table_[i].value_);
      return SUCCESS;
    }
  }
  return FAIL;
}

static inline
void* __bucket_find(bucket_ptr bucket, void* key) {
  for (size_t i = 0U; i != bucket->bucket_entries_; ++i) {
    if (bucket->table_[i].key_ != NULL) {
      int res = bucket->bucket_key_cmp_func_(bucket->table_[i].key_, key);
      if (res) {
        // bucket->bucket_key_print_func_(stdout, bucket->table_[i].key_);
        // bucket->bucket_value_print_func_(stdout, bucket->table_[i].value_);
        // bucket->bucket_key_print_func_(stdout, key);
        // printf("--------------------------\n" );
        return bucket->table_[i].value_;
      }
    }
  }
  return NULL;
}

static inline
void __bucket_print(FILE* out, bucket_ptr bucket) {
  for (size_t i = 0U; i != bucket->bucket_entries_; ++i) {
    if (bucket->table_[i].key_ != NULL) {
      // printf("K' %p\n", bucket->table_[i].key_);
      // printf("V' %p\n", bucket->table_[i].value_);
      bucket->bucket_key_print_func_(out, bucket->table_[i].key_);
      bucket->bucket_value_print_func_(out, bucket->table_[i].value_);
    }
  }
}

static inline
void __bucket_clear(bucket_ptr bucket) {
  for (size_t i = 0U; i != bucket->bucket_entries_; ++i) {
    bucket->bucket_key_delete_func_(bucket->table_[i].key_);
    bucket->bucket_value_delete_func_(bucket->table_[i].value_);
  }
  free(bucket->table_);
  free(bucket);
}

hash_table_ptr __hash_table_create(size_t key_size, size_t value_size,
                                   size_t ht_entries, size_t bucket_size,
                                   size_t (*hash_func)(const void*, const size_t),
                                   int (*key_cmp_func)(void*, void*),
                                   int (*value_cmp_func)(void*, void*),
                                   void (*key_print_func)(FILE*, void*),
                                   void (*value_print_func)(FILE*, void*),
                                   void (*key_delete_func)(void*),
                                   void (*value_delete_func)(void*)) {

  size_t entries_per_bucket_node_ = bucket_size / (key_size + value_size);
  hash_table_ptr hash_table = (hash_table_ptr) malloc(sizeof(*hash_table));
  hash_table->ht_entries_ = ht_entries;
  hash_table->table_ = (list_ptr*) malloc(ht_entries * sizeof(list_ptr));
  hash_table->ht_hash_func_ = hash_func;
  for (size_t i = 0U; i != ht_entries; ++i) {
    hash_table->table_[i] = list_create(bucket_ptr, value_cmp_func, value_print_func, NULL);
    bucket_ptr bucket = __bucket_create(entries_per_bucket_node_,
                                        key_cmp_func,
                                        key_print_func, value_print_func,
                                        key_delete_func, value_delete_func,
                                        key_size, value_size);
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
      __bucket_clear(bucket);
      free(list_node);
    }
    list_clear(hash_table->table_[i]);
  }
  free(hash_table->table_);
  free(hash_table);
}

int hash_table_insert(hash_table_ptr* hash_table, void* key, void* value) {
  //printf("TEST IN: %p\n",value);
  void* result = hash_table_find(*hash_table, key);
  if (result == NULL) {
    size_t pos = (*hash_table)->ht_hash_func_(key, (*hash_table)->ht_entries_);
    list_ptr entry_bucket_chain = (*hash_table)->table_[pos];
    size_t entry_no_buckets = list_size(entry_bucket_chain);
    for (size_t i = 1U; i <= entry_no_buckets; ++i) {
      list_node_ptr list_node = list_get(entry_bucket_chain, i);
      if (list_node == NULL) {
        report_warning("Bucket chain index is out of bounds!");
        break;
      } else {
        bucket_ptr entry_bucket = *((bucket_ptr*) list_node->data_);
        int res = __bucket_insert(&entry_bucket, key, value);
        if (res == FAIL) {
          bucket_ptr new_bucket = __bucket_create(entry_bucket->bucket_entries_,
                                                  entry_bucket->bucket_key_cmp_func_,
                                                  entry_bucket->bucket_key_print_func_,
                                                  entry_bucket->bucket_value_print_func_,
                                                  entry_bucket->bucket_key_delete_func_,
                                                  entry_bucket->bucket_value_delete_func_,
                                                  entry_bucket->key_size_,
                                                  entry_bucket->value_size_);
          list_push_back((*hash_table)->table_[i], &new_bucket);
          __bucket_insert(&new_bucket, key, value);
        }
        return SUCCESS;
      }
    }
  }
  return FAIL;
}

void* hash_table_find(hash_table_ptr hash_table, void* key) {
  size_t pos = hash_table->ht_hash_func_(key, hash_table->ht_entries_);
  list_ptr entry_bucket_chain = hash_table->table_[pos];
  size_t entry_no_buckets = list_size(entry_bucket_chain);
  for (size_t i = 1U; i <= entry_no_buckets; ++i) {
    list_node_ptr list_node = list_get(entry_bucket_chain, i);
    if (list_node == NULL) {
      report_warning("Bucket chain index is out of bounds!");
      break;
    } else {
      bucket_ptr entry_bucket = *((bucket_ptr*) list_node->data_);
      return __bucket_find(entry_bucket, key);
    }
  }
}

void hash_table_print(FILE* out, hash_table_ptr hash_table) {
  printf("----------------------------------------------------------------------\n" );
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
        __bucket_print(out, entry_bucket);
      }
    }
  }
}
