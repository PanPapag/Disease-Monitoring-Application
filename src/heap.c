#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../includes/heap.h"
#include "../includes/list.h"
#include "../includes/macros.h"

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

static inline
void __heap_clear(heap_ptr heap, heap_node_ptr temp) {
  if (temp == NULL)
    return;

  __heap_clear(heap, temp->left_);
  __heap_clear(heap, temp->right_);

  if (heap->heap_delete_func_ != NULL) {
    heap->heap_delete_func_(temp->key_);
  }
  __FREE(temp);
}

void heap_clear(void* v) {
  heap_ptr heap = (heap_ptr) v;
  if (heap != NULL) {
    __heap_clear(heap, heap->root_);
    __FREE(heap);
  }
}

/* Function to insert element in level order in a binary tree representing a heap */
static inline
void __insert(heap_node_ptr temp, heap_node_ptr new_node) {
  list_ptr queue = list_create(heap_node_ptr, NULL, NULL, NULL);
  list_push_front(&queue, &temp);
  // Do level order traversal until we find
  // an empty place.
  while (list_size(queue) != 0) {
    /* Get first node data and free allocated memory */
    list_node_ptr queue_front_node = list_pop_front(&queue);
    heap_node_ptr temp = (*(heap_node_ptr*) queue_front_node->data_);
    free(queue_front_node);
    /* Check wether to insert or push to the queue and continue all the way down */
    if (temp->left_ == NULL) {
      temp->left_ = new_node;
      new_node->parent_ = temp;
      break;
    } else {
      list_push_back(&queue, &temp->left_);
    }

    if (temp->right_ == NULL) {
      temp->right_ = new_node;
      new_node->parent_ = temp;
      break;
    } else {
      list_push_back(&queue, &temp->right_);
    }
  }
  list_clear(queue);
}

void heap_insert_max(heap_ptr* heap, void* key) {
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
  // Fix the Heap property if it is violated
  heap_node_ptr temp = new_node;
  while (temp != (*heap)->root_ && (*heap)->heap_cmp_func_(temp->key_, temp->parent_->key_) >= 0) {
    /* Swap values */
    void* t_key = temp->key_;
    void* p_key = temp->parent_->key_;
    temp->key_ = p_key;
    temp->parent_->key_ = t_key;
    /* Go to parent node */
    temp = temp->parent_;
  }
  // Increase heap size
  (*heap)->size_++;
}

/*
  A recursive method to heapify a subtree with the root at given index
 This method assumes that the subtrees are already heapified
*/
static inline
void max_heapify(heap_ptr heap, heap_node_ptr current) {
  /* Alwayes check for sanity and return immediately */
  if (current == NULL) {
    return;
  }
  heap_node_ptr left = current->left_;
  heap_node_ptr right = current->right_;
  heap_node_ptr max = current;
  if (left != NULL) {
    if (heap->heap_cmp_func_(left->key_, max->key_) >= 0) {
      max = left;
    }
  }
  if (right != NULL) {
    if (heap->heap_cmp_func_(right->key_, max->key_) >= 0) {
      max = right;
    }
  }
  if (max != current) {
    /* Swap values */
    void* max_key = max->key_;
    void* cur_key = current->key_;
    max->key_ = cur_key;
    current->key_ = max_key;
    /* Continue recursively */
    max_heapify(heap, max);
  }
}

/* A utility function to get the ith node (by level order) of a heap */
static inline
heap_node_ptr __get_ith_heap_node(heap_node_ptr root, const size_t i) {
  size_t  b = i;
  /* Sanity check: If no tree, always return NULL. */
  if (!root || i < 1)
    return NULL;
  /* If i is 1, we return the root. */
  if (i == 1)
    return root;
  /* Set b to the value of the most significant binary digit
     set in b. This is a known trick. */
  while (b & (b - 1))
    b &= b - 1;
  /* We ignore that highest binary digit. */
  b >>= 1;
  /* Walk down the tree as directed by b. */
  while (b) {
    if (i & b) {
      if (root->right_)
        root = root->right_;
      else
        return NULL; /* Not a complete tree, or outside the tree. */
    } else {
      if (root->left_)
        root = root->left_;
      else
        return NULL; /* Not a complete tree, or outside the tree. */
    }
    /* Next step. */
    b >>= 1;
  }
  /* This is where we arrived at. */
  return root;
}

void* heap_extract_max(heap_ptr* heap) {
  void* max_key = NULL;
  if ((*heap)->root_ != NULL) {
    /* In an max heap the max element is always in the root */
    max_key = (*heap)->root_->key_;
    /*
      Set the root's key to the end node's key of the heap and free
      it right after
    */
    heap_node_ptr last = __get_ith_heap_node((*heap)->root_, (*heap)->size_);
    (*heap)->root_->key_ = last->key_;
    if (last->parent_ != NULL) {
      if (last->parent_->left_ == last) {
        last->parent_->left_ = NULL;
      } else {
        last->parent_->right_ = NULL;
      }
    }
    // TODO __FREE(last);
    /* Decrease heap size and free memory allocated */
    (*heap)->size_--;
    /* Heapify the complete binary tree */
    max_heapify(*heap, (*heap)->root_);
  }
  return max_key;
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
