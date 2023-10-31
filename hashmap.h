#include "ss.h"

#define HashMap(key_t, value_t, hfn) struct hash_map_##key_t##_##value_t##_##hfn
#define DEF_HASH_MAP(key_t, value_t, hash_f) \
  HashMap(key_t, value_t, hash_f) { \
    \
  }   
