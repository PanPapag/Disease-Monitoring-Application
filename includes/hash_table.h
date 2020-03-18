#ifndef __HASH_TABLE__
  #define __HASH_TABLE__

  #include <stdint.h>
  #include "list.h"

  enum ht_insert_codes {
    FAIL,
    SUCCESS
  };

  typedef uint8_t byte_t;

  typedef struct bucket_t* bucket_ptr;
  typedef struct hash_table_t* hash_table_ptr;

  typedef struct bucket_entry_t {
    void* key_;
    void* value_;
  } bucket_entry_t;

  struct bucket_t {
    size_t key_size_;
    size_t value_size_;
    bucket_entry_t* table_;
    int (*bucket_key_cmp_func_)(void*, void*);
    void (*bucket_key_print_func_)(FILE*, void*);
    void (*bucket_value_print_func_)(FILE*, void*);
    size_t bucket_entries_;
  } bucket_t;

  struct hash_table_t {
    size_t ht_entries_;
    list_ptr* table_;
    size_t (*ht_hash_func_)(const void*, const size_t);
  } hash_table_t;

  /*
   Creates a new hash table given a type, number of entries, bucket size,
   hash, compare and print functions.
  */
  #define hash_table_create(kt, vt, ne, bs, hf, kcf, vcf, kpf, vpf) \
  ({ \
    __hash_table_create(sizeof(kt), sizeof(vt), ne, bs, hf, kcf, vcf, kpf, vpf); \
  })
  hash_table_ptr __hash_table_create(size_t, size_t, size_t, size_t,
    size_t (*)(const void*, const size_t), int (*)(void*, void*),
    int (*)(void*, void*), void (*)(FILE*, void*), void (*)(FILE*, void*));

  /* Deletes hash table */
  void hash_table_clear(hash_table_ptr);

  /* Adds a pair of (key, value) to the hash table */
  int hash_table_insert(hash_table_ptr*, void*, void*);
  /* Given a key returns the value associated with it. NULL if not found */
  void* hash_table_find(hash_table_ptr, void*);
  /* Utility function to print the whole hash table */
  void hash_table_print(FILE* out, hash_table_ptr);

#endif
