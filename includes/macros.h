#ifndef __MACROS__
  #define __MACROS__

  #include <stdlib.h>

  #define MAX(x, y) ((x) < (y) ? (y) : (x))

  #define __FREE(ptr) \
  do {                \
    free(ptr);        \
    ptr = NULL;       \
  } while(0)

  #define BUFFER_SIZE 255

  #define STRING char*
  
#endif
