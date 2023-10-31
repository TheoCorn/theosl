#include "../ss.h"
#include "../vec.h"
#include <stddef.h>
#include <stdio.h>
#include <unistd.h>

struct error1 {
  int e;
  char* mes;
};

typedef struct error1 Error1;

DEF_CLASS(error, Error1 e1; int e2;)
typedef struct error_class Error;
DEF_RESULT(size_t, Error)

DEF_MAYBE(size_t);

Result(size_t, Error) get_size_t() {
  Result(size_t, Error) res;
  int e = scanf("%zu", &res.data.ok);
  if (e != 1) {
    res.type = RESULT_ERR;
    res.data.err.type = 0;
    res.data.err.data.e1.e = e;
    res.data.err.data.e1.mes = "unable to read size_t"; 
    return res;
  }

  res.type = RESULT_OK;
  return res;
}

int main() {
  int d;
  SIF(int e = scanf("%d", &d);, e != 1, puts("error reading"); return -1;)

  // remember that repeat expects positive integers (comperes with size_t)
  if (d > 0) repeat(d) {
    printf("%d\n", d);
  }

  size_t num = num;
  // at this point just use normal code (this is just for testing SIFE and Result at the same time)
  SIFE(
    Result(size_t, Error) gnr = get_size_t();, 

    gnr.type == RESULT_ERR, 

    Error err = gnr.data.err;
    switch (err.type) {
      case 0: printf("terminating with error: %s", err.data.e1.mes); return err.data.e1.e;
      case 1: printf("terminating with unnamed error of value%d", err.data.e2); return err.data.e2;
    }, 

    num = gnr.data.ok;
  )

  // equivalent normal code (mutch better)
  // {
  //   Result(size_t, Error) gnr = get_size_t(); 
  //   if (gnr.type == RESULT_ERR) {
  //     Error err = gnr.data.err;
  //     switch (err.type) {
  //       case 0: printf("terminating with error: %s", err.data.e1.mes); return err.data.e1.e;
  //       case 1: printf("terminating with unnamed error of value%d", err.data.e2); return err.data.e2;
  //     } 
  //   } else {
  //     num = gnr.data.ok;
  //     printf("num: %zu\n", num);
  //   }
  // }

  // Ok maybe SIFE is never that nice, but with SIF being quite useful I thought I would try it TAKE IT OR LEAVE IT
  SIFE(int n; int e = scanf("%d", &n);, e != 1, puts("invalid input");, printf("you have entered the number %d\n", n);)

  
  loop { 
    puts("endless");
    sleep(5);
  }

  
}
