#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../includes/avl.h"

avl_ptr __avl_create(size_t data_size,
                      int (*avl_node_cmp_func)(avl_node_ptr, avl_node_ptr),
                      int (*avl_data_cmp_func)(avl_node_ptr, void*),
                      void (*avl_print_func)(FILE*, void*)) {

  avl_ptr avl = malloc(sizeof(*avl));
  avl->root_ = NULL;
  avl->data_size_ = data_size;
  avl->avl_node_cmp_func_ = avl_node_cmp_func;
  avl->avl_data_cmp_func_ = avl_data_cmp_func;
  avl->avl_print_func_ = avl_print_func;
  return avl;
}
