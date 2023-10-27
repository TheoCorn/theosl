#include "../ss.h"
#include "../vec.h"
#include <stdio.h>

int main() {
  int d;
  SIF(int e = scanf("%d", &d);, e != 1, puts("error reading"); return -1;)

  repeat(10) {
    printf("%d\n", d);
  }

  // loop puts("endless");

  
}
