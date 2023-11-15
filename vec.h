#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VEC_ADD 10
#define VEC_INIT_CAP 10

#define Vec(type) struct vec_##type##_t

#define DEF_VEC(type)                                                          \
  struct vec_##type##_t {                                                      \
    type *data;                                                                \
    size_t len, cap;                                                           \
  };                                                                           \
                                                                               \
  int vec_##type##_init_with_capacity(Vec(type) * vec, size_t len) {           \
    vec->data = (type *)malloc(len * sizeof(type));                            \
    if (vec->data == NULL) {                                                   \
      return -1;                                                               \
    }                                                                          \
    vec->cap = len;                                                            \
    vec->len = 0;                                                              \
    return 0;                                                                  \
  }                                                                            \
                                                                               \
  int vec_##type##_init(struct vec_##type##_t *vec) {                          \
    return vec_##type##_init_with_capacity(vec, VEC_INIT_CAP);                 \
  }                                                                            \
                                                                               \
  int vec_##type##_inm_to0(Vec(type) * vec, size_t len) {                      \
    vec->data = (type *)calloc(sizeof(type), len);                             \
    if (vec->data == NULL) {                                                   \
      return -1;                                                               \
    }                                                                          \
    vec->cap = len;                                                            \
    vec->len = len;                                                            \
    return 0;                                                                  \
  }                                                                            \
                                                                               \
  void vec_##type##_free(struct vec_##type##_t *vec) { free(vec->data); }      \
                                                                               \
  int vec_##type##_extend_by(struct vec_##type##_t *vec, size_t extend) {      \
    type *data =                                                               \
        (type *)realloc(vec->data, (vec->cap + extend) * sizeof(type));        \
    if (data == NULL) {                                                        \
      return -1;                                                               \
    }                                                                          \
    vec->data = data;                                                          \
    vec->cap += extend;                                                        \
    return 0;                                                                  \
  }                                                                            \
                                                                               \
  int vec_##type##_extend(struct vec_##type##_t *vec) {                        \
    return vec_##type##_extend_by(vec, vec->len / 2);                          \
  }                                                                            \
                                                                               \
  type *vec_##type##_uninit_append(struct vec_##type##_t *vec) {               \
    if (vec->len == vec->cap) {                                                \
      vec_##type##_extend(vec);                                                \
    }                                                                          \
    return vec->data + vec->len++;                                             \
  }                                                                            \
                                                                               \
  void vec_##type##_append(struct vec_##type##_t *vec, type *value) {          \
    if (vec->len == vec->cap) {                                                \
      vec_##type##_extend(vec);                                                \
    }                                                                          \
    vec->data[vec->len++] = *value;                                            \
  }                                                                            \
                                                                               \
  void vec_##type##_rappend(struct vec_##type##_t *vec, type value) {          \
    if (vec->len == vec->cap) {                                                \
      vec_##type##_extend(vec);                                                \
    }                                                                          \
    vec->data[vec->len++] = value;                                             \
  }                                                                            \
                                                                               \
  void vec_##type##_prepend(struct vec_##type##_t *vec, type value) {          \
    if (vec->cap == vec->len) {                                                \
      vec_##type##_extend(vec);                                                \
    }                                                                          \
                                                                               \
    memmove(vec->data + 1, vec->data, vec->len);                               \
    vec->data[0] = value;                                                      \
  }                                                                            \
                                                                               \
  void vec_##type##_clear(struct vec_##type##_t *vec) { vec->len = 0; }        \
                                                                               \
  type *vec_##type##_find(Vec(type) * vec, bool (*predicate)(void *, type *),  \
                          void *pred_arg) {                                    \
    for (size_t i = 0; i < vec->len; ++i) {                                    \
      if (predicate(pred_arg, vec->data + i)) {                                \
        return vec->data + i;                                                  \
      }                                                                        \
    }                                                                          \
    return NULL;                                                               \
  }                                                                            \
                                                                               \
  type *vec_##type##_find_exact(Vec(type) * vec, type * a) {                   \
    for (size_t i = 0; i < vec->len; ++i) {                                    \
      if (memcmp(a, vec->data + i, sizeof(type)) == 0) {                       \
        return vec->data + i;                                                  \
      }                                                                        \
    }                                                                          \
    return NULL;                                                               \
  }                                                                            \
                                                                               \
  type *vec_##type##_find_max(Vec(type) * vec, int (*cmp)(type *, type *)) {   \
    if (vec->len == 0)                                                         \
      return NULL;                                                             \
    if (vec->len == 1)                                                         \
      return vec->data;                                                        \
    type *max = vec->data;                                                     \
    for (size_t i = 1; i < vec->len; ++i) {                                    \
      type *b = vec->data + i;                                                 \
      if (cmp(max, b) < 0) {                                                   \
        max = b;                                                               \
      }                                                                        \
    }                                                                          \
    return max;                                                                \
  }                                                                            \
                                                                               \
  type *vec_##type##_find_min(Vec(type) * vec, int (*cmp)(type *, type *)) {   \
    if (vec->len == 0)                                                         \
      return NULL;                                                             \
    if (vec->len == 1)                                                         \
      return vec->data;                                                        \
    type *min = vec->data;                                                     \
    for (size_t i = 1; i < vec->len; ++i) {                                    \
      type *b = vec->data + i;                                                 \
      if (cmp(min, b) > 0) {                                                   \
        min = b;                                                               \
      }                                                                        \
    }                                                                          \
    return min;                                                                \
  }                                                                            \
                                                                               \
  void vec_##type##_print(Vec(type) * vec, void (*print)(type *)) {            \
    for (size_t i = 0; i < vec->len; ++i) {                                    \
      print(vec->data + i);                                                    \
    }                                                                          \
    putc('\n', stdout);                                                        \
  }                                                                            \
                                                                               \
  type vec_##type##_pop_back(Vec(type) * vec) {                                \
    return vec->data[--vec->len];                                              \
  }                                                                            \
                                                                               \
  type *vec_##type##_last(Vec(type) * vec) {                                   \
    return vec->data + vec->len - 1;                                           \
  }                                                                            \
                                                                               \
  type *vec_##type##_first(Vec(type) * vec) { return vec->data; }

/**
  creates `for it in vec` loop where `it` is a type*
*/
#define VEC_FOR_EACH(type, vec)                                                \
  for (type *it = vec.data; it < vec.data + vec.len; ++it)

#define VECP_FOR_EACH(type, vecp)                                              \
  for (type *it = vecp->data; it < vecp->data + vecp->len; ++it)
