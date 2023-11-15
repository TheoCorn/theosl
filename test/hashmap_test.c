#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "../vec.h"
#include "../hashmap.h"
#include "../cmp.h"
#include "../hashfun.h"

// DEF_GHASH_MAP_ENTERY(str, size_t, eq_str, cmp_size_t)
// typedef GHashMapEntery(str, size_t) ghme_str_size_t;
// DEF_VEC(ghme_str_size_t)
DEF_GENERIC_HASH_MAP(str, size_t, 10, eq_str, cmp_size_t)

#define wordmap_t struct ghash_map_str_size_t_10
#define MAP_FUN(name) ghash_map_str_size_t_10_##name

int main() {
  // GHashMap(str, size_t, 10) map = GHASH_MAP_FUNC_NAME(str, size_t, 10, new)(strptrusize_djb2);
  wordmap_t map = MAP_FUN(new)(strptrusize_djb2);


  char* k0 = "abcd";
  size_t v0 = 51;
  MAP_FUN(insert)(&map, &k0, &v0);

  size_t* g0 = MAP_FUN(get)(&map, &k0);

  if (g0 == NULL) {
    printf("ERROR: inserted value of key %s is not aviable\n", k0);
  } else {
    printf("g0 := v0 = %zu\n", *g0);
  }

  if (MAP_FUN(contains)(&map, &k0)) {
    printf("contains %s\n", k0);
  } else {
    printf("does not contain %s\n", k0);
  }

  char* k1 = "bc";
  size_t v1 = 10;
  if (MAP_FUN(contains)(&map, &k1)) {
    printf("contains %s\n", k1);
  } else {
    printf("does not contain %s\n", k1);
  }

  MAP_FUN(insert)(&map, &k1, &v1);

  for (size_t i = 0; i < 10; ++i) {
    VEC_FOR_EACH(__ghash_map_entery_str_size_t, (map.buckets[i])) {
      printf("%s : %zu\n", it->key, it->value);
    }
  }

  MAP_FUN(free)(&map);
  
}
