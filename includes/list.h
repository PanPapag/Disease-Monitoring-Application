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
      int (*cmp_func_)(void*, void*);
      void (*print_func_)(FILE*, void*);
  } list_t;

  #define list_create(t, dc, dp) __list_create(sizeof(t), dc, dp)
  list_ptr __list_create(size_t, int (*)(void*, void*), void (*)(FILE*, void*));

  void list_clear(list_ptr);

  void list_push_front(list_ptr*, void*);
  void list_push_back(list_ptr*, void*);
  void list_sorted_insert(list_ptr*, void*);

  list_node_ptr list_pop_front(list_ptr);
  list_node_ptr list_pop_back(list_ptr);

  list_node_ptr list_front(list_ptr);
  list_node_ptr list_back(list_ptr);

  size_t list_size(list_ptr);

  void list_print(list_ptr, FILE* out);

#endif
