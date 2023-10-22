#include "../io.h"
#include <stdio.h>

int main() {
  Vec(int) iv;
  vec_int_init(&iv);
  int e = iscan(&iv);
  if (e != 0) {
    printf("iscan error\n");
  }
  
  for (size_t i = 0; i < iv.len; ++i) {
    printf("%d ", iv.data[i]);
  }
  putc('\n', stdout);
}
