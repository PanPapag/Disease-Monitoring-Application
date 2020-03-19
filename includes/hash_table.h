#ifndef __HASH_TABLE__
  #define __HASH_TABLE__

  #include <stdint.h>
  #include "list.h"

  enum ht_insert_codes {
    FAIL,
    SUCCESS
  };

  typedef struct bucket_t* bucket_ptr;
  typedef struct bucket_entry_t* bucket_entry_ptr;
  typedef struct hash_table_t* hash_table_ptr;

  typedef struct bucket_entry_t {
    void* key_;
    void* value_;
  } bucket_entry_t;

  struct bucket_t {
    size_t bucket_size_;
    bucket_entry_t* table_;
  } bucket_t;

  struct hash_table_t {
    size_t ht_entries_;
    size_t bucket_entries_;
    list_ptr* table_;
    size_t (*ht_hash_func_)(const void*);
    int (*ht_key_cmp_func_)(void*, void*);
    int (*ht_value_cmp_func_)(void*, void*);
    void (*ht_key_print_func_)(FILE*, void*);
    void (*ht_value_print_func_)(FILE*, void*);
    void (*ht_key_delete_func_)(void*);
    void (*ht_value_delete_func_)(void*);
  } hash_table_t;

  /*
   Creates a new hash table given a type, number of entries, bucket size,
   hash, compare, print and delete functions.
  */
  hash_table_ptr hash_table_create(size_t, size_t, size_t (*)(const void*),
    int (*)(void*, void*), int (*)(void*, void*),
    void (*)(FILE*, void*), void (*)(FILE*, void*),
    void (*)(void*), void (*)(void*));

  /* Deletes hash table */
  void hash_table_clear(hash_table_ptr);

  /* Adds a pair of (key, value) to the hash table */
  int hash_table_insert(hash_table_ptr*, void*, void*);
  /* Given a key returns the value associated with it. NULL if not found */
  void* hash_table_find(hash_table_ptr, void*);
  /* Utility function to print the whole hash table */
  void hash_table_print(hash_table_ptr, FILE*);

#endif
