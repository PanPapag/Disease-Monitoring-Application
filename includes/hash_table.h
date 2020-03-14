#ifndef __HASH_TABLE__
  #define __HASH_TABLE__

  #include <stdint.h>
  #include "list.h"

  struct hash_table_t {
    list_ptr bucket_chain_;
    size_t no_buckets_;
    size_t entries_per_bucket_node_;
    size_t (*hash_function_)(const void*, const size_t);
  } hash_rable_t;


#endif
