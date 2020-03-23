#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../includes/heap.h"
#include "../includes/list.h"

heap_ptr heap_create(int (*heap_cmp_func)(void*, void*),
                     void (*heap_print_func)(void*, FILE*),
                     void (*heap_delete_func)(void*)) {

  heap_ptr heap = malloc(sizeof(*heap));
  if (heap == NULL) {
   report_error("Could not allocate memory for Heap Data Structure. Exiting...");
   exit(EXIT_FAILURE);
  }
  heap->root_ = NULL;
  heap->size_ = 0U;
  heap->heap_cmp_func_ = heap_cmp_func;
  heap->heap_print_func_ = heap_print_func;
  heap->heap_delete_func_ = heap_delete_func;
  return heap;
}

/* Function to insert element in level order in a binary tree representing a heap */
static inline
void __insert(heap_node_ptr temp, heap_node_ptr new_node) {
  list_ptr queue = list_create(heap_node_ptr, NULL, NULL, NULL);
  list_push_front(&queue, &temp);
  // Do level order traversal until we find
  // an empty place.
  while (list_size(queue) != 0) {
    list_node_ptr queue_front_node = list_pop_front(&queue);
    heap_node_ptr temp = (*(heap_node_ptr*) queue_front_node->data_);

    if (temp->left_ == NULL) {
      temp->left_ = new_node;
      new_node->parent_ = temp->left_;
      break;
    } else {
      list_push_back(&queue, &temp->left_);
    }

    if (temp->right_ == NULL) {
      temp->right_ = new_node;
      new_node->parent_ = temp->right_;
      break;
    } else {
      list_push_back(&queue, &temp->right_);
    }
  }
  list_clear(queue);
}

void heap_insert(heap_ptr* heap, void* key) {
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
  if ((*heap)->root_ == NULL) {
    (*heap)->root_ = new_node;
  } else {
    __insert((*heap)->root_, new_node);
  }
  // Fix the Heap property if it is violated TODO
}

/* Print nodes at a given level */
void printGivenLevel(heap_node_ptr root, int level)
{
    if (root == NULL) {
      return;
    }
    if (level == 1)
        printf("%d ", (*(int*)root->key_));
    else if (level > 1)
    {
        printGivenLevel(root->left_, level-1);
        printGivenLevel(root->right_, level-1);
    }
}

/* Function to print level order traversal a tree*/
void printLevelOrder(heap_node_ptr root)
{
    int i;
    for (i=1; i<=4; i++) {
      printf("LEVEL: %d\n",i);
      printGivenLevel(root, i);
      printf("\n");
    }
}
