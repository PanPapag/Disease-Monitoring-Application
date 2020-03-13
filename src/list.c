#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../includes/list.h"

list_ptr __list_create(size_t data_size,
                   int (*cmp_func)(void*, void*),
                   void (*print_func)(FILE*, void*)) {

  list_ptr list = malloc(sizeof(*list));
  list->head_ = NULL;
  list->tail_ = NULL;
  list->size_ = 0U;
  list->data_size_ = data_size;
  list->cmp_func_ = cmp_func;
  list->print_func_ = print_func;
  return list;
}

void list_push_front(list_ptr* list, void* new_data) {
  /* Allocate node and initialize node */
  // + data_size means that the size of the variable-length
  // array 'data_' of the node will be of size data_size.
  list_node_ptr new_node = (list_ptr) malloc(sizeof(list_node_t) + (*list)->data_size_);
  memcpy(new_node->data_, new_data, (*list)->data_size_);
  /* Since we are adding at the begining, prev is always NULL */
  new_node->prev_ = NULL;
  /* Link the old list off the new node */
  new_node->next_ = (*list)->head_;
  /* If list is empty, then make the new node as tail */
  if ((*list)->tail_ == NULL) {
    (*list)->tail_ = new_node;
  }
  /* Change prev of head node to new node */
  if ((*list)->head_ != NULL) {
      (*list)->head_->prev_ = new_node;
  }
  /* Move the head to point to the new node */
  (*list)->head_ = new_node;
  /* Increment the size of the list */
  (*list)->size_++;
}

void list_push_back(list_ptr* list, void* new_data) {
  /* Allocate node and initialize node */
  // + data_size means that the size of the variable-length
  // array 'data' of the node will be of size data_size.
  list_node_ptr new_node = (list_node_ptr) malloc(sizeof(list_node_t) + (*list)->data_size_);
  memcpy(new_node->data_, new_data, (*list)->data_size_);
  /* Since we are adding at the end, next is always NULL */
  new_node->next_ = NULL;
  /* Make last node as the previous of the new node */
  new_node->prev_ = (*list)->tail_ ;
  /* If list is empty, then make the new node as head */
  if ((*list)->head_ == NULL) {
    new_node->prev_ = NULL;
    (*list)->head_ = new_node;
  }
  /* Change the next of the last node */
  if ((*list)->tail_ != NULL) {
    (*list)->tail_->next_ = new_node;
  }
  /* Move the tail to point to the new node */
  (*list)->tail_ = new_node;
  /* Increment the size of the list */
  (*list)->size_++;

}

void list_sorted_insert(list_ptr* list, void *new_data) {
  list_node_ptr current;
  /* Allocate and initialize node */
  // + data_size means that the size of the variable-length
  // array 'data' of the node will be of size data_size.
  list_node_ptr new_node = (list_node_ptr) malloc(sizeof(list_node_t) + (*list)->data_size_);
  memcpy(new_node->data_, new_data, (*list)->data_size_);
  new_node->prev_ = new_node->next_ = NULL;
  /* If list is empty */
  if ((*list)->head_ == NULL) {
    (*list)->head_ = new_node;
    (*list)->tail_ = new_node;
  } else {
    /* Check to insert at the beggining of the list */
    if ((*list)->cmp_func_(new_node->data_, (*list)->head_->data_) < 0) {
      new_node->next_ = (*list)->head_;
      new_node->next_->prev_ = new_node;
      (*list)->head_ = new_node;
    /* Check to inser at the end of the list */
    } else if ((*list)->cmp_func_(new_node->data_, (*list)->tail_->data_) > 0) {
      new_node->prev_ = (*list)->tail_ ;
      (*list)->tail_->next_ = new_node;
      (*list)->tail_ = new_node;
    } else {
      current = (*list)->head_;
      /* Find position to be inserted */
      while (current->next_ != NULL && (*list)->cmp_func_(new_node->data_, current->next_->data_) > 0) {
        current = current->next_;
      }
      new_node->next_ = current->next_;
      if (current->next_ != NULL) {
        new_node->next_->prev_ = new_node;
      }
      current->next_ = new_node;
      new_node->prev_ = current;
    }
  }
  /* Increment the size of the list */
  (*list)->size_++;
}

size_t list_size(list_ptr list) {
  return list->size_;
}

void list_print(list_ptr list, FILE* out) {
  list_node_ptr temp = list->head_;
  while (temp != NULL) {
    list->print_func_(out, temp->data_);
    temp = temp->next_;
  }
}

void list_print_reverse(list_ptr list, FILE* out) {
  list_node_ptr temp = list->tail_;
  while (temp != NULL) {
    list->print_func_(out, temp->data_);
    temp = temp->prev_;
  }
}

void list_clear(list_ptr list) {
  list_node_ptr current = list->head_;
  list_node_ptr next;
  while (current != NULL) {
    next = current->next_;
    free(current);
    current = next;
  }
  list->head_ = NULL;
  free(list);
}
