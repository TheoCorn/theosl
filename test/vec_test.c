#include "../vec.h"
#include <stdio.h>

DEF_VEC(size_t)

int main() {
  Vec(size_t) vec;
  vec_size_t_init(&vec);
  vec_size_t_rappend(&vec, 0);
  vec_size_t_rappend(&vec, 0);
  vec_size_t_rappend(&vec, 0);
  vec_size_t_rappend(&vec, 0);
  vec_size_t_rappend(&vec, 1);
  vec_size_t_rappend(&vec, 2);
  vec_size_t_rappend(&vec, 3);
  vec_size_t_rappend(&vec, 4);

  VEC_FOR_EACH(size_t, vec) {
     printf("%zu ", *it); 
  }

   
}
