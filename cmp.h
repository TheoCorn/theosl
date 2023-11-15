#include <string.h>

#define DEF_CMP_NUM(type)                                                      \
  int cmp_##type(type *a, type *b) {                                           \
    if (*a == *b) {                                                              \
      return 0;                                                                \
    }                                                                          \
    return *a > *b ? 1 : -1;                                                     \
  }

#ifndef __THEOSL_CMP_FUNS
#define __THEOSL_CMP_FUNS

DEF_CMP_NUM(size_t)
DEF_CMP_NUM(int)
DEF_CMP_NUM(long)
DEF_CMP_NUM(double)
DEF_CMP_NUM(float)


/**
  Comperes the 
  @param arg expexts a pointer to a string that is the ground thuth (aka. char**)
  @param str the string to check 
*/
bool eq_str(void* arg, const char* str) {
  return !strcmp(*((char**) arg), str);
}

#endif
