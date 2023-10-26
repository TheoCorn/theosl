#include "../io.h"
#include <stdio.h>

int main() {
  Vec(int) iv;
  vec_int_init(&iv);
  int e = iscan(&iv);
  if (e != 0) {
    printf("iscan error\n");
  }
  
  vec_int_print(&iv, STDOUT_PRINTER(int, "%d"));
}
