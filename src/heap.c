#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../includes/heap.h"

heap_ptr heap_create(int (*heap_cmp_func)(void*, void*),
                     void (*heap_print_func)(void*, FILE*),
                     void (*heap_delete_func)(void*)) {

  heap_ptr heap = malloc(sizeof(*heap));
  if (heap == NULL) {
   report_error("Could not allocate memory for Heap Data Structure. Exiting...");
   exit(EXIT_FAILURE);
  }
  heap->root_ = NULL;
  heap->end_leaf_ = NULL;
  heap->size_ = 0U;
  heap->heap_cmp_func_ = heap_cmp_func;
  heap->heap_print_func_ = heap_print_func;
  heap->heap_delete_func_ = heap_delete_func;
  return heap;
}

void heap_insert(heap_ptr heap, void* key) {
  // At first allocate a new heap node to store the key
  heap_node_ptr new_node = (heap_node_ptr) malloc(sizeof(heap_node_t));
  if (new_node == NULL) {
    report_error("Could not allocate memory for Heap Node. Exiting...");
    exit(EXIT_FAILURE);
  }
  new_node->key_ = key;
  new_node->left_ = new_node->right_ = NULL;
  new_node->parent_ = NULL;
  // Insert the new key at the end
  if (heap->root_ == NULL) {
    heap->root_ = new_node;
    heap->end_leaf_ = new_node;
  } else {
    if (heap->end_leaf_->left_ == NULL) {
      heap->end_leaf_->left_ = new_node;
    } else {
      heap->end_leaf_->right_ = new_node;
    }
    new_node->parent_ = heap->end_leaf_;
    heap->end_leaf_ = new_node;
  }
  // Fix the Heap property if it is violated TODO

}
