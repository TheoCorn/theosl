#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VEC_ADD 10
#define VEC_INIT_CAP 10

#define DEFVEC(type)                                                           \
  struct vec_##type##_t {                                                      \
    type *data;                                                                \
    size_t len, cap;                                                           \
  };                                                                           \
                                                                               \
  int vec_##type##_init(struct vec_##type##_t *vec) {                          \
    vec->data = (type *)malloc(VEC_INIT_CAP * sizeof(type));                   \
    if (vec->data == NULL) {                                                   \
      return -1;                                                               \
    }                                                                          \
    vec->cap = VEC_INIT_CAP;                                                   \
    vec->len = 0;                                                              \
    return 0;                                                                  \
  }                                                                            \
                                                                               \
  void vec_##type##_free(struct vec_##type##_t *vec) { free(vec->data); }      \
                                                                               \
  int vec_##type##_extend_by(struct vec_##type##_t *vec, size_t extend) {      \
    int *data = (int *)realloc(vec->data, (vec->cap + extend) * sizeof(type)); \
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
      vec_int_extend(vec);                                                     \
    }                                                                          \
                                                                               \
    memmove(vec->data + 1, vec->data, vec->len);                               \
    vec->data[0] = value;                                                      \
  }

#define Vec(type) struct vec_##type##_t
