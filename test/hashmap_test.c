#include "../vec.h"
#include "../hashmap.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../cmp.h"

DEF_GHASH_MAP_ENTERY(str, size_t, eq_str, cmp_size_t)
typedef GHashMapEntery(str, size_t) ghme_str_size_t;
DEF_VEC(ghme_str_size_t)
DEF_GENERIC_HASH_MAP(str, size_t, 3)

int main() {
  
}
