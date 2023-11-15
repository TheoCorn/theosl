#include "ss.h"
#include "vec.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

DEF_VEC(size_t)

#define Tree(type) struct tree_##type##_t
#define DLTree(type) struct dltree_##type##_t

#define TREE_FUNC_NAME(type, children, rest) tree_##type##_##children##rest
#define DL_TREE_FUNC_NAME(type, children, rest) dltree_##type##_##children##rest

#define DEF_DL_TREE(type, nchildren)                                           \
  struct dltree_##type##_t {                                                   \
    type data;                                                                 \
    struct dltree_##type##_t *parent;                                          \
    struct dltree_##type##_t *children[nchildren];                             \
  };                                                                           \
                                                                               \
  void DL_TREE_FUNC_NAME(type, nchildren, _print)(                             \
      DLTree(type) * node, void (*print)(DLTree(type) *)) {                    \
    print(node);                                                               \
    for (size_t i = 0; i < nchildren; ++i) {                                   \
      if (node->children[i]) {                                                 \
        DL_TREE_FUNC_NAME(type, nchildren, _print)(node->children[i], print);  \
      }                                                                        \
    }                                                                          \
  }                                                                            \
                                                                               \
  DLTree(type) *                                                               \
      DL_TREE_FUNC_NAME(type, nchildren, _new_from_data)(type * data) {        \
    DLTree(type) *node = (DLTree(type) *)calloc(1, sizeof(DLTree(type)));      \
    node->data = *data;                                                        \
    return node;                                                               \
  }                                                                            \
                                                                               \
  ssize_t DL_TREE_FUNC_NAME(type, nchildren, _nor_num_children)(               \
      struct dltree_##type##_t * node) {                                       \
    for (size_t i = 0; i < nchildren; ++i) {                                   \
      if (node->children[i] == NULL) {                                         \
        return i;                                                              \
      }                                                                        \
    }                                                                          \
    return -1;                                                                 \
  }                                                                            \
  size_t DL_TREE_FUNC_NAME(type, nchildren,                                    \
                           _num_children)(struct dltree_##type##_t * node) {   \
    size_t n = 0;                                                              \
    for (size_t i = 0; i < nchildren; ++i) {                                   \
      if (node->children[i] != NULL)                                           \
        n++;                                                                   \
    }                                                                          \
    return n;                                                                  \
  }                                                                            \
                                                                               \
  int DL_TREE_FUNC_NAME(type, nchildren, _shift_down_left)(                    \
      struct dltree_##type##_t * current, struct dltree_##type##_t * new) {    \
    size_t i = 0;                                                              \
    for (; i < nchildren; ++i) {                                               \
      if (new->children[i] == 0)                                               \
        break;                                                                 \
    }                                                                          \
                                                                               \
    if (i == nchildren) {                                                      \
      return -1;                                                               \
    }                                                                          \
    if (i != 0) {                                                              \
      memmove(new->children + sizeof(struct dltree##type##_t *),               \
              new->children, sizeof(struct dltree##type##_t *) * (i + 1));     \
    }                                                                          \
                                                                               \
    if (current->parent == NULL) {                                             \
      new->children[0] = current;                                              \
      current->parent = new;                                                   \
      return 1;                                                                \
    }                                                                          \
                                                                               \
    size_t cur_index = 0;                                                      \
    for (; cur_index < nchildren; cur_index++) {                               \
      if (current->parent->children[cur_index] == current) {                   \
        current->parent->children[cur_index] = new;                            \
        break;                                                                 \
      }                                                                        \
    }                                                                          \
    if (cur_index == nchildren) {                                              \
      return -2;                                                               \
    }                                                                          \
                                                                               \
    current->parent = new;                                                     \
    new->children[0] = current;                                                \
    return 0;                                                                  \
  }                                                                            \
                                                                               \
  int DL_TREE_FUNC_NAME(type, nchildren, _shift_down_right)(                   \
      struct dltree_##type##_t * current, struct dltree_##type##_t * new) {    \
    size_t i = nchildren - 1;                                                  \
    for (; i >= 0; --i) {                                                      \
      if (new->children[i] == 0)                                               \
        break;                                                                 \
    }                                                                          \
                                                                               \
    if (i == -1) {                                                             \
      return -1;                                                               \
    }                                                                          \
    if (i != nchildren - 1) {                                                  \
      memmove(new->children + i, new->children + i + 1,                        \
              sizeof(struct dltree##type##_t *) * (nchildren - 1 - i));        \
    }                                                                          \
                                                                               \
    if (current->parent == NULL) {                                             \
      new->children[nchildren - 1] = current;                                  \
      current->parent = new;                                                   \
      return 1;                                                                \
    }                                                                          \
                                                                               \
    size_t cur_index = 0;                                                      \
    for (; cur_index < nchildren; cur_index++) {                               \
      if (current->parent->children[cur_index] == current) {                   \
        current->parent->children[cur_index] = new;                            \
        break;                                                                 \
      }                                                                        \
    }                                                                          \
    if (cur_index == nchildren) {                                              \
      return -2;                                                               \
    }                                                                          \
                                                                               \
    current->parent = new;                                                     \
    new->children[nchildren - 1] = current;                                    \
    return 0;                                                                  \
  }                                                                            \
                                                                               \
  int DL_TREE_FUNC_NAME(type, nchildren, _add_child)(                          \
      struct dltree_##type##_t * parent, struct dltree_##type##_t * child) {   \
    for (size_t i = 0; i < nchildren; ++i) {                                   \
      if (parent->children[i] == NULL) {                                       \
        parent->children[i] = child;                                           \
        return 0;                                                              \
      }                                                                        \
    }                                                                          \
    return -1;                                                                 \
  }                                                                            \
                                                                               \
  DLTree(type) * DL_TREE_FUNC_NAME(type, nchildren, _walk_up_until)(           \
                     DLTree(type) * node,                                      \
                     bool (*predicate)(void *, DLTree(type) *),                \
                     void *pred_arg) {                                         \
    DLTree(type) *current = node->parent;                                      \
    while (current != NULL) {                                                  \
      if (predicate(pred_arg, current)) {                                      \
        return current;                                                        \
      }                                                                        \
      current = current->parent;                                               \
    }                                                                          \
                                                                               \
    return NULL;                                                               \
  }                                                                            \
                                                                               \
  bool has_aditional_children(void *arg, DLTree(type) * node) {                \
    Vec(size_t) *lch_index = (Vec(size_t) *)arg;                               \
    size_t *last = vec_size_t_last(lch_index);                                 \
                                                                               \
    size_t i = 0;                                                              \
    for (; i < nchildren; ++i) {                                               \
      if (lch_index->data[i]) {                                                      \
        *vec_size_t_last(lch_index) = i;                                        \
        return true;                                                           \
      }                                                                        \
    }                                                                          \
    lch_index->len--;                                                          \
    return false;                                                              \
  }                                                                            \
                                                                               
  // DLTree(type) * DL_TREE_FUNC_NAME(type, nchildren, _dfs)(                     \
  //                    DLTree(type) * node, bool (*predicate)(DLTree(type) *)) { \
  //   Vec(size_t) indexes;                                                       \
  //   vec_size_t_init(&indexes);                                                 \
  //                                                                              \
  //   DLTree(type) *current = node;                                              \
  //   bool current_indexed = false;                                              \
  //   loop {                                                                     \
  //     if (predicate(current)) {                                                \
  //       return current;                                                        \
  //     }                                                                        \
  //     size_t i = 0;                                                            \
  //     for (; i < nchildren; ++i) {                                             \
  //       if (indexes.data[i]) {                                                    \
  //         if (current_indexed) {                                               \
  //           *vec_size_t_last(&indexes) = i;                                    \
  //         } else {                                                             \
  //           vec_size_t_rappend(&indexes, i);                                   \
  //         }                                                                    \
  //         current = indexes.data[i];                                              \
  //         current_indexed = false;                                             \
  //       }                                                                      \
  //     }                                                                        \
  //                                                                              \
  //     if (i == nchildren) {                                                    \
  //       current = DL_TREE_FUNC_NAME(type, nchildren, _walk_up_until)(          \
  //           current, has_aditional_children, (void *)&indexes);                \
  //       if (current == NULL) {                                                 \
  //         return NULL;                                                         \
  //       }                                                                      \
  //     }                                                                        \
  //   }                                                                          \
  //                                                                              \
  //   vec_size_t_free(&indexes);                                                 \
  // }

#define DEF_DL_TREE_DROP(type, nchildren, drop_fun)                            \
  int DL_TREE_FUNC_NAME(type, nchildren, _drop_node)(DLTree(type) * node) {    \
    drop_fun(&node->data);                                                     \
    free(node);                                                                \
    return 0;                                                                  \
  }                                                                            \
                                                                               \
  int DL_TREE_FUNC_NAME(type, nchildren, _drop_tree)(DLTree(type) * node) {    \
    for (size_t i = 0; i < nchildren; ++i) {                                   \
      if (node->children[i]) {                                                 \
        int e =                                                                \
            DL_TREE_FUNC_NAME(type, nchildren, _drop_tree)(node->children[i]); \
        if (e) {                                                               \
          return e;                                                            \
        }                                                                      \
      }                                                                        \
    }                                                                          \
    return DL_TREE_FUNC_NAME(type, nchildren, _drop_node)(node);               \
  }

#define DEF_TREE(type, nchildren)                                              \
  Tree(type) {                                                                 \
    type data;                                                                 \
    Tree(type) * parent;                                                       \
    Tree(type) * children[nchildren];                                          \
  };                                                                           \
                                                                               \
  void TREE_FUNC_NAME(type, nchildren, _print)(Tree(type) * node,              \
                                               void (*print)(Tree(type) *)) {  \
    print(node);                                                               \
    for (size_t i = 0; i < nchildren; ++i) {                                   \
      if (node->children[i]) {                                                 \
        TREE_FUNC_NAME(type, nchildren, _print)(node->children[i], print);     \
      }                                                                        \
    }                                                                          \
  }                                                                            \
                                                                               \
  Tree(type) * TREE_FUNC_NAME(type, nchildren, _new_from_data)(type * data) {  \
    Tree(type) *node = (Tree(type) *)calloc(1, sizeof(Tree(type)));            \
    node->data = *data;                                                        \
    return node;                                                               \
  }                                                                            \
                                                                               \
  ssize_t TREE_FUNC_NAME(type, nchildren,                                      \
                         _nor_num_children)(Tree(type) * node) {               \
    for (size_t i = 0; i < nchildren; ++i) {                                   \
      if (node->children[i] == NULL) {                                         \
        return i;                                                              \
      }                                                                        \
    }                                                                          \
    return -1;                                                                 \
  }                                                                            \
  size_t TREE_FUNC_NAME(type, nchildren,                                       \
                        _num_children)(Tree(type) _t * node) {                 \
    size_t n = 0;                                                              \
    for (size_t i = 0; i < nchildren; ++i) {                                   \
      if (node->children[i] != NULL)                                           \
        n++;                                                                   \
    }                                                                          \
    return n;                                                                  \
  }                                                                            \
                                                                               \
  int TREE_FUNC_NAME(type, nchildren, _shift_down_left)(                       \
      Tree(type) _t * current, Tree(type) _t * new) {                          \
    size_t i = 0;                                                              \
    for (; i < nchildren; ++i) {                                               \
      if (new->children[i] == 0)                                               \
        break;                                                                 \
    }                                                                          \
                                                                               \
    if (i == nchildren) {                                                      \
      return -1;                                                               \
    }                                                                          \
    if (i != 0) {                                                              \
      memmove(new->children + sizeof(Tree(typet *),               \
              new->children, sizeof(Tree(typet *) * (i + 1));                  \
    }                                                                          \
                                                                               \
    if (current->parent == NULL) {                                             \
      new->children[0] = current;                                              \
      current->parent = new;                                                   \
      return 1;                                                                \
    }                                                                          \
                                                                               \
    size_t cur_index = 0;                                                      \
    for (; cur_index < nchildren; cur_index++) {                               \
      if (current->parent->children[cur_index] == current) {                   \
        current->parent->children[cur_index] = new;                            \
        break;                                                                 \
      }                                                                        \
    }                                                                          \
    if (cur_index == nchildren) {                                              \
      return -2;                                                               \
    }                                                                          \
                                                                               \
    current->parent = new;                                                     \
    new->children[0] = current;                                                \
    return 0;                                                                  \
  }                                                                            \
                                                                               \
  int TREE_FUNC_NAME(type, nchildren, _shift_down_right)(                      \
      Tree(type) _t * current, Tree(type) _t * new) {                          \
    size_t i = nchildren - 1;                                                  \
    for (; i >= 0; --i) {                                                      \
      if (new->children[i] == 0)                                               \
        break;                                                                 \
    }                                                                          \
                                                                               \
    if (i == -1) {                                                             \
      return -1;                                                               \
    }                                                                          \
    if (i != nchildren - 1) {                                                  \
      memmove(new->children + i, new->children + i + 1,                        \
              sizeof(Tree(typet *) * (nchildren - 1 - i));                     \
    }                                                                          \
                                                                               \
    if (current->parent == NULL) {                                             \
      new->children[nchildren - 1] = current;                                  \
      current->parent = new;                                                   \
      return 1;                                                                \
    }                                                                          \
                                                                               \
    size_t cur_index = 0;                                                      \
    for (; cur_index < nchildren; cur_index++) {                               \
      if (current->parent->children[cur_index] == current) {                   \
        current->parent->children[cur_index] = new;                            \
        break;                                                                 \
      }                                                                        \
    }                                                                          \
    if (cur_index == nchildren) {                                              \
      return -2;                                                               \
    }                                                                          \
                                                                               \
    current->parent = new;                                                     \
    new->children[nchildren - 1] = current;                                    \
    return 0;                                                                  \
  }                                                                            \
                                                                               \
  int TREE_FUNC_NAME(type, nchildren, _add_child)(Tree(type) _t * parent,      \
                                                  Tree(type) _t * child) {     \
    for (size_t i = 0; i < nchildren; ++i) {                                   \
      if (parent->children[i] == NULL) {                                       \
        parent->children[i] = child;                                           \
        return 0;                                                              \
      }                                                                        \
    }                                                                          \
    return -1;                                                                 \
  }

// Tree(type) * TREE_FUNC_NAME(type, nchildren, _walk_until)(              \
  //                    Tree(type) * node, bool (*predicate)(Tree(type) *)) { \
  //   Tree(type) *current = node->parent;                                      \
  //   while (current != NULL) {                                             \
  //     if (predicate(current)) {                                                \
  //       return current;                                                        \
  //     }                                                                        \
  //     current = current->parent;                                               \
  //   }                                                                          \
  //                                                                              \
  //   return NULL;                                                               \
  // }

#define DEF_TREE_DROP(type, nchildren, drop_fun)                               \
  int TREE_FUNC_NAME(type, nchildren, _drop_node)(Tree(type) * node) {         \
    drop_fun(&node->data);                                                     \
    free(node);                                                                \
    return 0;                                                                  \
  }                                                                            \
                                                                               \
  int TREE_FUNC_NAME(type, nchildren, _drop_tree)(Tree(type) * node) {         \
    for (size_t i = 0; i < nchildren; ++i) {                                   \
      if (node->children[i]) {                                                 \
        int e =                                                                \
            TREE_FUNC_NAME(type, nchildren, _drop_tree)(node->children[i]);    \
        if (e) {                                                               \
          return e;                                                            \
        }                                                                      \
      }                                                                        \
    }                                                                          \
    return TREE_FUNC_NAME(type, nchildren, _drop_node)(node);                  \
  }
