#ifndef __LIST__
  #define __LIST__

  #include <stdint.h>

  typedef uint8_t byte_t;

  typedef struct list_node_t* list_node_ptr;
  typedef struct list_t* list_ptr;

  struct list_node_t {
    list_node_ptr prev_;
    list_node_ptr next_;
    byte_t data_[];
  } list_node_t;

  struct list_t {
      list_node_ptr head_;
      list_node_ptr tail_;
      size_t size_;
      size_t data_size_;
      int (*list_cmp_func_)(void*, void*);
      void (*list_print_func_)(FILE*, void*);
  } list_t;

  /* Creates a new list given a type, compare and print functions */
  #define list_create(t, dc, dp) __list_create(sizeof(t), dc, dp)
  list_ptr __list_create(size_t, int (*)(void*, void*), void (*)(FILE*, void*));

  /* Delete list */
  void list_clear(list_ptr);
  /* Delete a given node */
  void list_remove(list_ptr*, list_node_ptr);

  /* Adds an element to the beginning */
  void list_push_front(list_ptr*, void*);
  /* Adds an element to the end */
  void list_push_back(list_ptr*, void*);
  /* Adds an element accordingly to the compare function */
  void list_sorted_insert(list_ptr*, void*);

  /* Removes and returns the first element */
  list_node_ptr list_pop_front(list_ptr*);
  /* Removes and returns the last element */
  list_node_ptr list_pop_back(list_ptr*);

  /* Access the first element */
  list_node_ptr list_front(list_ptr);
  /* Access the last element */
  list_node_ptr list_back(list_ptr);

  /* Find the element equals a given one */
  list_node_ptr list_find(list_ptr, void*);
  /* Returns the size of the list */
  size_t list_size(list_ptr);
  /* Prints the list from the beginning to the end */
  void list_print(list_ptr, FILE* out);
  /* Prints the list from the end to the beginning */
  void list_print_reverse(list_ptr, FILE* out);

#endif
