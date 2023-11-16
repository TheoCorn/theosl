#include "ss.h"
#include <stdbool.h>

#define GHashMap(key_t, value_t, nbuckets)                                     \
  struct ghash_map_##key_t##_##value_t##_##nbuckets

#define GHASH_MAP_FUNC_NAME(key_t, value_t, nbuckets, rest)                    \
  ghash_map_##key_t##_##value_t##_##nbuckets##_##rest

#define _GHashMapEntry_NAME(key_t, value_t) _ghash_map_entry_##key_t##_##value_t

#define _GHashMapEntry_FUNC_NAME(key_t, value_t, rest)                         \
  _ghash_map_entry_##key_t##_##value_t##_##rest

#define GHashMapEntry(key_t, value_t) struct _GHashMapEntry_NAME(key_t, value_t)

#define DEF_GENERIC_HASH_MAP(key_t, value_t, nbuckets, key_cmp_f, value_cmp_f) \
  GHashMapEntry(key_t, value_t) {                                              \
    key_t key;                                                                 \
    value_t value;                                                             \
  };                                                                           \
                                                                               \
  bool _GHashMapEntry_FUNC_NAME(key_t, value_t, find_by_key)(                  \
      void *arg, GHashMapEntry(key_t, value_t) * it) {                         \
    return key_cmp_f(arg, (it->key));                                          \
  }                                                                            \
                                                                               \
  bool _GHashMapEntry_FUNC_NAME(key_t, value_t, find_by_value)(                \
      void *arg, GHashMapEntry(key_t, value_t) * it) {                         \
    return value_cmp_f(arg, &(it->value));                                     \
  }                                                                            \
                                                                               \
  typedef GHashMapEntry(key_t, value_t) __ghash_map_entry_##key_t##_##value_t; \
  DEF_VEC(__ghash_map_entry_##key_t##_##value_t)                               \
                                                                               \
  GHashMap(key_t, value_t, nbuckets) {                                         \
    Vec(__ghash_map_entry_##key_t##_##value_t) buckets[nbuckets];              \
    size_t (*hash_f)(key_t *);                                                 \
  };                                                                           \
                                                                               \
  GHashMap(key_t, value_t, nbuckets) GHASH_MAP_FUNC_NAME(                      \
      key_t, value_t, nbuckets, new)(size_t(*hashf)(key_t *)) {                \
    GHashMap(key_t, value_t, nbuckets) ghm;                                    \
    for (size_t i = 0; i < nbuckets; ++i) {                                    \
      vec___ghash_map_entry_##key_t##_##value_t##_init(ghm.buckets + i);       \
    }                                                                          \
    ghm.hash_f = hashf;                                                        \
    return ghm;                                                                \
  }                                                                            \
                                                                               \
  void GHASH_MAP_FUNC_NAME(key_t, value_t, nbuckets,                           \
                           free)(GHashMap(key_t, value_t, nbuckets) * ghm) {   \
    for (size_t i = 0; i < nbuckets; ++i) {                                    \
      vec___ghash_map_entry_##key_t##_##value_t##_free(ghm->buckets + i);      \
    }                                                                          \
  }                                                                            \
                                                                               \
  size_t GHASH_MAP_FUNC_NAME(key_t, value_t, nbuckets, bucket_of)(             \
      GHashMap(key_t, value_t, nbuckets) * ghm, key_t * key) {                 \
    return ghm->hash_f(key) % nbuckets;                                        \
  }                                                                            \
                                                                               \
  void GHASH_MAP_FUNC_NAME(key_t, value_t, nbuckets,                           \
                           insert)(GHashMap(key_t, value_t, nbuckets) * ghm,   \
                                   key_t * key, value_t * value) {             \
    size_t bucket = ghm->hash_f(key) % nbuckets;                               \
    vec_##__ghash_map_entry_##key_t##_##value_t##_rappend(                     \
        ghm->buckets + bucket,                                                 \
        (__ghash_map_entry_##key_t##_##value_t){*key, *value});                \
  }                                                                            \
                                                                               \
  bool GHASH_MAP_FUNC_NAME(key_t, value_t, nbuckets, contains)(                \
      GHashMap(key_t, value_t, nbuckets) * ghm, key_t * key) {                 \
    size_t bucket = ghm->hash_f(key) % nbuckets;                               \
    return vec_##__ghash_map_entry_##key_t##_##value_t##_find(                 \
               ghm->buckets + bucket,                                          \
               _ghash_map_entry_##key_t##_##value_t##_find_by_key,             \
               (void *)key) != NULL;                                           \
  }                                                                            \
                                                                               \
  value_t *GHASH_MAP_FUNC_NAME(key_t, value_t, nbuckets, get)(                 \
      GHashMap(key_t, value_t, nbuckets) * ghm, key_t * key) {                 \
    size_t bucket = ghm->hash_f(key) % nbuckets;                               \
    __ghash_map_entry_##key_t##_##value_t *entry =                             \
        vec___ghash_map_entry_##key_t##_##value_t##_find(                      \
            ghm->buckets + bucket,                                             \
            _ghash_map_entry_##key_t##_##value_t##_find_by_key, (void *)key);  \
    if (entry) {                                                               \
      return &(entry->value);                                                  \
    } else {                                                                   \
      return NULL;                                                             \
    }                                                                          \
  }                                                                            \
                                                                               \
  GHashMapEntry(key_t, value_t) *                                              \
      GHASH_MAP_FUNC_NAME(key_t, value_t, nbuckets, get_entry)(                \
          GHashMap(key_t, value_t, nbuckets) * ghm, key_t * key) {             \
    size_t bucket = ghm->hash_f(key) % nbuckets;                               \
    return vec___ghash_map_entry_##key_t##_##value_t##_find(                   \
        ghm->buckets + bucket,                                                 \
        _ghash_map_entry_##key_t##_##value_t##_find_by_key, (void *)key);      \
  }                                                                            \
                                                                               \
  void GHASH_MAP_FUNC_NAME(key_t, value_t, nbuckets,                           \
                           upsert)(GHashMap(key_t, value_t, nbuckets) * ghm,   \
                                   key_t * key, value_t * value) {             \
    size_t bucket = ghm->hash_f(key) % nbuckets;                               \
                                                                               \
    GHashMapEntry(key_t, value_t) *current =                                   \
        vec___ghash_map_entry_##key_t##_##value_t##_find(                      \
            ghm->buckets + bucket,                                             \
            _ghash_map_entry_##key_t##_##value_t##_find_by_key, (void *)key);  \
                                                                               \
    if (current) {                                                             \
      current->value = *value;                                                 \
    } else {                                                                   \
      vec_##__ghash_map_entry_##key_t##_##value_t##_rappend(                   \
          ghm->buckets + bucket,                                               \
          (__ghash_map_entry_##key_t##_##value_t){*key, *value});              \
    }                                                                          \
  }                                                                            \
                                                                               \
  void GHASH_MAP_FUNC_NAME(key_t, value_t, nbuckets, mutsert)(                 \
      GHashMap(key_t, value_t, nbuckets) * ghm, key_t * key, value_t * value,  \
      void (*mutator)(void *, __ghash_map_entry_##key_t##_##value_t *),        \
      void *mutator_arg) {                                                     \
    size_t bucket = ghm->hash_f(key) % nbuckets;                               \
                                                                               \
    GHashMapEntry(key_t, value_t) *current =                                   \
        vec___ghash_map_entry_##key_t##_##value_t##_find(                      \
            ghm->buckets + bucket,                                             \
            _ghash_map_entry_##key_t##_##value_t##_find_by_key, (void *)key);  \
                                                                               \
    if (current) {                                                             \
      mutator(mutator_arg, current);                                           \
    } else {                                                                   \
      vec_##__ghash_map_entry_##key_t##_##value_t##_rappend(                   \
          ghm->buckets + bucket,                                               \
          (__ghash_map_entry_##key_t##_##value_t){*key, *value});              \
    }                                                                          \
  }                                                                            \
                                                                               
  // Vec(__ghash_map_entry_##key_t##_##value_t##)                                 \
  //     GHASH_MAP_FUNC_NAME(key_t, value_t, nbuckets,                            \
  //                         to_vec)(GHashMap(key_t, value_t, nbuckets) * ghm) {  \
  //   struct vec___ghash_map_entry_##key_t##_##value_t##_t vec;                  \
  //   vec___ghash_map_entry_##key_t##_##value_t##_init(&vec);                    \
  //   for (Vec(__ghash_map_entry_##key_t##_##value_t) *__bucket__ =              \
  //            ghm->buckets;                                                     \
  //        __bucket__ < ghm->buckets + nbuckets; ++__bucket__)                   \
  //     VECP_FOR_EACH(__ghash_map_entry_##key_t##_##value_t, __bucket__) {       \
  //       vec___ghash_map_entry_##key_t##_##value_t##_append(&vec, it);          \
  //     }                                                                        \
  //                                                                              \
  //   return vec;                                                                \
  // }

// Vec(__ghash_map_entry_##key_t##_##value_t)                                   \
  //     GHASH_MAP_FUNC_NAME(key_t, value_t, nbuckets,                            \
  //                         to_vec)(GHashMap(key_t, value_t, nbuckets) * ghm) {  \
  //   struct vec___ghash_map_entry_##key_t##_##value_t##_t vec;                         \
  //   vec___ghash_map_entry_##key_t##_##value_t##_init(&vec);                    \
  //   for (struct vec___ghash_map_entry_##key_t##_##value_t##_t *__bucket__ =           \
  //            ghm->buckets;                                                     \
  //        __bucket__ < ghm->buckets + nbuckets; ++__bucket__)                   \
  //     VECP_FOR_EACH(_ghash_map_entry_##key_t##_##value_t##_t, __bucket__) {   \
  //       vec___ghash_map_entry_##key_t##_##value_t##_rappend(it);               \
  //     }                                                                        \
  // }

#define GHASHMAP_FOR_EACH(entry_t, mpt, nbuckets)                              \
  for (Vec(entry_t) *__bucket__ = mpt->buckets;                                \
       __bucket__ < mpt->buckets + nbuckets; ++__bucket__)                     \
  VECP_FOR_EACH(entry_t, __bucket__)

/*#define HashMap(key_t, value_t, hfn) struct
hash_map_##key_t##_##value_t##_##hfn
#define DEF_HASH_MAP(key_t, value_t, hash_f) \
  HashMap(key_t, value_t, hash_f) { \
    \
  }
*/
