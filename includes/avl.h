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
    int balance_factor_;
    byte_t data_[];
  } avl_node_t;

  struct avl_t {
    avl_node_ptr root_;
    size_t data_size_;
    int (*avl_cmp_func_)(void*, void*);
    void (*avl_print_func_)(FILE*, void*);
  } avl_t;

  /* Creates a new avl tree given a type, compare and print functions */
  #define avl_create(t, dc, dp) __avl_create(sizeof(t), dc, dp)
  avl_ptr __avl_create(size_t, int (*)(void*, void*), void (*)(FILE*, void*));

  /* Deletes avl tree */
  #define avl_clear(t) \
  ({\
    __avl_clear(t->root_); \
    free(t); \
  })
  void __avl_clear(avl_node_ptr);

  /* Adds an element to the avl tree */
  void avl_insert(avl_ptr*, void*);

  /* Find the element equals a given one */
  avl_node_ptr avl_find(avl_ptr, void*);

  /* Inorder printing of the avl tree */
  #define avl_print_inorder(t, f) __avl_print_inorder(t, t->root_, f);
  void __avl_print_inorder(avl_ptr, avl_node_ptr, FILE* out);

#endif
