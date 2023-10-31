#include "ss.h"
#include <stdbool.h>

#define GHashMap(key_t, value_t, nbuckets)                                     \
  struct ghash_map_##key_t##_##value_t_##nbuckets

#define GHASH_MAP_FUNC_NAME(key_t, value_t, nbuckets, rest)                    \
  ghash_map_##key_t##_##value_t##_##nbuckets##_##rest

#define _GHashMapEntery_NAME(key_t, value_t)                                   \
  _ghash_map_entery_##key_t##_##value_t
#define GHashMapEntery(key_t, value_t)                                         \
  struct _GHashMapEntery_NAME(key_t, value_t)
#define DEF_GHASH_MAP_ENTERY(key_t, value_t, key_cmp_f, value_cmp_f)           \
  GHashMapEntery(key_t, value_t) {                                             \
    key_t key;                                                                 \
    value_t value                                                              \
  };                                                                           \
                                                                               \
  bool _GHashMapEntery_NAME(key_t, value_t)##_find_by_key(                     \
      void *arg, GHashMapEntery(key_t, value_t) * it) {                        \
    return key_cmp_f(&(it->key));                                              \
  }                                                                            \
                                                                               \
  bool _GHashMapEntery_NAME(key_t, value_t)##_find_by_value(                   \
      void *arg, GHashMapEntery(key_t, value_t) * it) {                        \
    return value_cmp_f(&(it->value));                                          \
  }

/**
  @breaf before a GHashMap(key_t, value_t, ...) is defined a Vec(struct
  GHashMapEntery(key_t, value_t)) must be defined
*/
#define DEF_GENERIC_HASH_MAP(key_t, value_t, nbuckets)                         \
  GHashMap(key_t, value_t, nbuckets) {                                         \
    Vec(GHashMapEntery(key_t, value_t)) buckets[nbuckets];                     \
    size_t (*hash_f)(key_t *);                                                 \
  };                                                                           \
                                                                               \
  GHashMap(key_t, value_t, nbuckets) GHASH_MAP_FUNC_NAME(                      \
      key_t, value_t, nbuckets, new)(size_t(*hashf)(key_t *)) {                \
    GHashMap(key_t, value_t, nbuckets) ghm;                                    \
    ghm.buckets = {0};                                                         \
    ghm.hash_f = hashf;                                                        \
    return ghm;                                                                \
  }                                                                            \
                                                                               \
  size_t _##GHASH_MAP_FUNC_NAME(key_t, value_t, nbuckets, bucket_of)(          \
      GHashMap(key_t, value_t, nbuckets) * ghm, key_t * key) {                 \
    return ghm->hash_f(key) % nbuckets;                                        \
  }                                                                            \
                                                                               \
  int GHASH_MAP_FUNC_NAME(key_t, value_t, nbuckets,                            \
                          insert)(GHashMap(key_t, value_t, nbuckets) * ghm,    \
                                  key_t * key, value_t * value) {              \
    size_t bucket = ghm->hash_f(key) % nbuckets;                               \
    return vec_##_GHashMapEntery_NAME(key_t, value_t)##_rappend(               \
        ghm->buckets + bucket, {*key, *value});                                \
  }                                                                            \
                                                                               \
  bool GHASH_MAP_FUNC_NAME(key_t, value_t, nbuckets, contains)(                \
      GHashMap(key_t, value_t, nbuckets) * ghm, key_t * key) {                 \
    size_t bucket = ghm->hash_f(key) % nbuckets;                               \
    return vec_##_GHashMapEntery_NAME(key_t, value_t)##_find(                  \
               ghm->buckets + bucket,                                          \
               _GHashMapEntery_NAME(key_t, value_t)##_find_by_key) != NULL;    \
  }                                                                            \
                                                                               \
  value_t *GHASH_MAP_FUNC_NAME(key_t, value_t, nbuckets, get)(                 \
      GHashMap(key_t, value_t, nbuckets) * ghm, key_t * key) {                 \
    size_t bucket = ghm->hash_f(key) % nbuckets;                               \
    return vec_##_GHashMapEntery_NAME(key_t, value_t)##_find(                  \
        ghm->buckets + bucket,                                                 \
        _GHashMapEntery_NAME(key_t, value_t)##_find_by_key);                   \
  }

/*#define HashMap(key_t, value_t, hfn) struct
hash_map_##key_t##_##value_t##_##hfn
#define DEF_HASH_MAP(key_t, value_t, hash_f) \
  HashMap(key_t, value_t, hash_f) { \
    \
  }
*/
