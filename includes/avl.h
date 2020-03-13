#ifndef __AVL__
  #define __AVL__

  #include <stdint.h>

  typedef uint8_t byte_t;

  typedef struct avl_node_t* avl_node_ptr;
  typedef struct avl_t* avl_ptr;

  struct avl_node_t {
    avl_node_ptr left_;
    avl_node_ptr right_;
    avl_node_ptr parent_;
    size_t balance_factor_;
    byte_t data_[];
  } avl_node_t;

  struct avl_t {
    avl_node_ptr root_;
    size_t data_size_;
    int (*avl_node_cmp_func_)(avl_node_ptr, avl_node_ptr);
    int (*avl_data_cmp_func_)(avl_node_ptr, void*);
    void (*avl_print_func_)(FILE*, void*);
  } avl_t;

  /* Creates a new avl tree given a type, compare and print functions */
  #define avl_create(t, dc, nc, dp) __avl_create(sizeof(t), dc, nc, dp)
  avl_ptr __avl_create(size_t, int (*)(avl_node_ptr, avl_node_ptr),
                       int (*)(avl_node_ptr, void*), void (*)(FILE*, void*));


#endif
