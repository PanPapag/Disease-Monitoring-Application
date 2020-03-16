#ifndef __HASH_TABLE__
  #define __HASH_TABLE__

  #include <stdint.h>
  #include "list.h"

  typedef uint8_t byte_t;

  typedef struct bucket_t* bucket_ptr;
  typedef struct hash_table_t* hash_table_ptr;

  typedef struct bucket_entry_t {
    void* key_;
    void* value_;
  } bucket_entry_t;

  struct bucket_t {
    bucket_entry_t* table_;
    size_t bucket_entries_;
  } bucket_t;

  struct hash_table_t {
    size_t key_size_;
    size_t value_size_;
    size_t ht_entries_;
    list_ptr* table_;
    size_t (*ht_hash_func_)(const void*, const size_t);
    void (*ht_key_print_func_)(FILE*, void*);
    void (*ht_value_print_func_)(FILE*, void*);
  } hash_table_t;

  /*
   Creates a new hash table given a type, number of entries, bucket size,
   hash, compare and print functions.
  */
  #define hash_table_create(kt, vt, ne, bs, hf, cf, kpf, vpf) \
  ({ \
    __hash_table_create(sizeof(kt), sizeof(vt), ne, bs, hf, cf, kpf, vpf); \
  })
  hash_table_ptr __hash_table_create(size_t, size_t, size_t, size_t,
    size_t (*)(const void*, const size_t), int (*)(void*, void*),
    void (*)(FILE*, void*), void (*)(FILE*, void*));

  /* Deletes hash table */
  void hash_table_clear(hash_table_ptr);

  /* Adds an element to the avl tree */
  void hash_table_insert(void*, void*);

#endif
