#ifndef __HEAP__
  #define __HEAP__

  typedef struct heap_node_t* heap_node_ptr;
  typedef struct heap_t* heap_ptr;

  struct heap_node_t {
    heap_node_ptr left_;
    heap_node_ptr right_;
    heap_node_ptr parent_;
    void* key_;
  } heap_node_t;

  struct heap_t {
    heap_node_ptr root_;
    size_t size_;
    int (*heap_cmp_func_)(void*, void*);
    void (*heap_print_func_)(void*, FILE*);
    void (*heap_delete_func_)(void*);
  } heap_t;

  /* Creates a new avl tree given compare, print and delete functions */
  heap_ptr heap_create(int (*)(void*, void*), void (*)(void*, FILE*), void (*)(void*));

  /* Clear a heap by deallocating all memory allocated */
  void heap_clear(void*);

  /* Inserts a new key to the heap */
  void heap_insert_max(heap_ptr*, void*);

  /* Method to remove max element of the heap */
  void* heap_extract_max(heap_ptr*);

#endif
