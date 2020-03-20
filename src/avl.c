#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../includes/avl.h"
#include "../includes/macros.h"
#include "../includes/io_utils.h"
#include "../includes/utils.h"

avl_ptr __avl_create(size_t data_size,
                     int (*avl_cmp_func)(void*, void*),
                     void (*avl_print_func)(void*, FILE*)) {

  avl_ptr avl = malloc(sizeof(*avl));
  if (avl == NULL) {
    report_error("Could not allocate memory for AVL Data Structure. Exiting...");
    exit(EXIT_FAILURE);
  }
  avl->root_ = NULL;
  avl->data_size_ = data_size;
  avl->avl_cmp_func_ = avl_cmp_func;
  avl->avl_print_func_ = avl_print_func;
  return avl;
}

static inline
avl_node_ptr __rotate_ll(avl_node_ptr parent, int parent_factor,
                         int *child_factor, int *h_delta) {

  avl_node_ptr child = parent->left_;
  int c_left = (child->left_) ? 1 : 0;
  int c_right = (child->right_) ? 1 : 0;
  int p_right;
  if (*child_factor < 0) {
    // child->left > child->right
    c_left = c_right - (*child_factor);
    p_right = c_left + parent_factor + 1;
    if (h_delta) {
      *h_delta = MAX(c_left, MAX(c_right, p_right) + 1) - (c_left + 1);
    }
  } else {
    // child->left <= child->right
    c_right = c_left + (*child_factor);
    p_right = c_right + parent_factor + 1;
    if (h_delta) {
      *h_delta = MAX(c_left, MAX(c_right, p_right) + 1) - (c_right + 1);
    }
  }
  *child_factor = (MAX(c_right, p_right) + 1) - c_left;
  parent->balance_factor_ =  p_right - c_right;
  parent->left_ = child->right_;
  if (child->right_) {
    child->right_->parent_ =  parent;
  }
  child->right_ = parent;
  child->parent_ = parent->parent_;
  parent->parent_ = child;
  return child;
}

static inline
avl_node_ptr __rotate_rr(avl_node_ptr parent, int parent_factor,
                         int *child_factor, int *h_delta) {

  avl_node_ptr child = parent->right_;
  int c_left = (child->left_) ? 1 : 0;
  int c_right = (child->right_) ? 1 : 0;
  int p_left;
  if (*child_factor < 0) {
    // child->left > child->right
    c_left = c_right - (*child_factor);
    p_left = c_left - parent_factor + 1;
    if (h_delta) {
      *h_delta = MAX(c_right, MAX(c_left, p_left) + 1) - (c_left + 1);
    }
  } else {
    // child->left <= child->right
    c_right = c_left + (*child_factor);
    p_left = c_right - parent_factor + 1;
    if (h_delta) {
      *h_delta = MAX(c_right, MAX(c_left, p_left) + 1) - (c_right + 1);
    }
  }
  *child_factor = c_right - (MAX(c_left, p_left) + 1);
  parent->balance_factor_ += c_left - p_left;
  parent->right_ = child->left_;
  if (child->left_) {
    child->left_->parent_ = parent;
  }
  child->left_ = parent;
  child->parent_ =  parent->parent_;
  parent->parent_ =  child;
  return child;
}

static inline
avl_node_ptr __rotate_lr(avl_node_ptr parent, int parent_factor) {
  int h_delta = 0;
  avl_node_ptr child = parent->left_;
  int child_factor;
  if (child->right_) {
    child_factor = child->right_->balance_factor_;
    parent->left_ = __rotate_rr(child, child->balance_factor_, &child_factor, &h_delta);
  } else {
    child_factor = child->balance_factor_;
  }
  avl_node_ptr res = __rotate_ll(parent, parent_factor - h_delta, &child_factor, NULL);
  res->balance_factor_ += child_factor;
  return res;
}

static inline
avl_node_ptr __rotate_rl(avl_node_ptr parent, int parent_factor) {
  int h_delta = 0;
  avl_node_ptr child = parent->right_;
  int child_factor;
  if (child->left_) {
    child_factor = child->left_->balance_factor_;
    parent->right_ = __rotate_ll(child, child->balance_factor_, &child_factor, &h_delta);
  } else {
    child_factor = child->left_->balance_factor_;
  }
  avl_node_ptr res = __rotate_rr(parent, parent_factor + h_delta, &child_factor, NULL);
  res->balance_factor_ += child_factor;
  return res;
}


static avl_node_ptr __fix_balance(avl_node_ptr node, int balance_factor) {
  int h_delta = node->balance_factor_ + balance_factor;
  int child_factor;
  if (node != NULL) {
    if (h_delta < -1 && node->left_) {
      // balance left subtree
      if (node->left_->balance_factor_ <= 0) {
        child_factor = node->left_->balance_factor_;
        node = __rotate_ll(node, h_delta, &child_factor, NULL);
        node->balance_factor_ = child_factor;
      } else {
        node = __rotate_lr(node, h_delta);
      }
    } else if (h_delta > 1 && node->right_) {
      if (node->right_->balance_factor_ >= 0) {
        child_factor = node->right_->balance_factor_;
        node = __rotate_rr(node, h_delta, &child_factor, NULL);
        node->balance_factor_ = child_factor;
      } else {
        node = __rotate_rl(node, h_delta);
      }
    } else {
      node->balance_factor_ += balance_factor;
    }
  }
  return node;
}

void avl_insert(avl_ptr* avl, void* new_data) {
  /* Allocate and initialize a new avl node */
  // + data_size means that the size of the variable-length
  // array 'data' of the node will be of size data_size.
  avl_node_ptr new_node = (avl_node_ptr) malloc(sizeof(avl_node_t) + (*avl)->data_size_);
  if (new_node == NULL) {
    report_error("Could not allocate memory for AVL Node. Exiting...");
    exit(EXIT_FAILURE);
  }
  memcpy(new_node->data_, new_data, (*avl)->data_size_);
  /* Traverse avl tree to find the correct position to insert the new node */
  avl_node_ptr current = (*avl)->root_;
  avl_node_ptr parent = NULL;
  while (current != NULL) {
    int cmp_res = (*avl)->avl_cmp_func_(new_node->data_, (*avl)->root_->data_);
    parent = current;
    if (cmp_res <= 0) {
      current = current->left_;
    } else {
      current = current->right_;
    }
  }
  /* Update the members of the new avl node */
  new_node->parent_ = parent;
  new_node->right_ = new_node->left_ = NULL;
  new_node->balance_factor_ = 1;
  if (parent != NULL) {
    if ((*avl)->avl_cmp_func_(new_node->data_, parent->data_) < 0) {
      parent->left_ = new_node;
    } else {
      parent->right_ = new_node;
    }
  } else {
    (*avl)->root_ = new_node;
  }
  /* Balancing process bottom up */
  int balance_factor = 0;
  while (new_node != NULL) {
    parent = new_node->parent_;
    if (parent != NULL) {
      int old_balance_factor = new_node->balance_factor_;
      if (parent->right_ == new_node) {
        new_node = __fix_balance(new_node, balance_factor);
        parent->right_ = new_node;
      } else {
        new_node = __fix_balance(new_node, balance_factor);
        parent->left_ = new_node;
      }
      // calculate balance factor for parent
      if (new_node->left_ == NULL && new_node->right_ == NULL) {
        if (parent->left_ == new_node) balance_factor = -1;
        else balance_factor = 1;
      } else {
        balance_factor = 0;
        if (abs(old_balance_factor) < abs(new_node->balance_factor_)) {
          if (parent->left_ == new_node) balance_factor = -1;
          else balance_factor = 1;
        }
      }
    } else if (new_node == (*avl)->root_) {
      (*avl)->root_ = __fix_balance((*avl)->root_, balance_factor);
      break;
    }
    if (balance_factor == 0) break;
    new_node = parent;
  }
}

avl_node_ptr avl_find(avl_ptr avl, void* data) {
  avl_node_ptr temp = avl->root_;
  while (temp) {
    int cmp_res = avl->avl_cmp_func_(data, temp->data_);
    if (cmp_res < 0) {
      temp = temp->left_;
    } else if (cmp_res > 0) {
      temp = temp->right_;
    } else {
      return temp;
    }
  }
  return NULL;
}

static inline
void __avl_print_inorder(avl_ptr avl, avl_node_ptr current_root, FILE* out) {
  avl_node_ptr temp = current_root;
  if (temp != NULL) {
    __avl_print_inorder(avl, temp->left_, out);
    avl->avl_print_func_(temp->data_, out);
    __avl_print_inorder(avl, temp->right_, out);
  }
}

void avl_print_inorder(void* v, FILE* out) {
  avl_ptr avl = (avl_ptr) v;
  __avl_print_inorder(avl, avl->root_, out);
}

static inline
void __avl_clear(avl_node_ptr temp) {
  if (temp == NULL)
    return;

  __avl_clear(temp->left_);
  __avl_clear(temp->right_);

  __FREE(temp);
}

void avl_clear(void* v) {
  avl_ptr avl = (avl_ptr) v;
  if (avl != NULL) {
    __avl_clear(avl->root_);
    __FREE(avl);
  }
}
