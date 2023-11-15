#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/*
#define lambda(lambda$_ret, lambda$_args, lambda$_body)                        \
  ({                                                                           \
    lambda$_ret lambda$__anon$ lambda$_args\
lambda$_body &amp;                                                             \
    lambda$__anon$;                                                            \
  })
*/

#define loop while (1)
#define repeat(n) for (size_t __i = 0; __i < n; ++__i)

// TODO figure out how to add enum of types (indexes) so just a one does not
// have a bunch of magic numbers
#define _DEF_CLASS_STRICTNAME(name, classes)                                   \
  struct name {                                                                \
    size_t type;                                                               \
    union {                                                                    \
      classes                                                                  \
    } data;                                                                    \
  };

#define Class(name) struct name##_class
// taged union / rust style enum / kotlin sealed class
#define DEF_CLASS(name, classes) _DEF_CLASS_STRICTNAME(name##_class, classes)

// stateful if
// usee for creating a go style `if (e := foo(); e == nil)` code
#define SIF(block, condition, do)                                              \
  {                                                                            \
    block if (condition) { do }                                                \
  }

#define SIFE(block, condition, do, edo)                                        \
  {                                                                            \
    block if (condition) { do }                                                \
    else {                                                                     \
      edo                                                                      \
    }                                                                          \
  }

#define Either(left_t, right_t) either_##left_t##_##right_t##_t
#define DEF_Either(left_t, right_t)                                            \
  _DEF_CLASS_STRICTNAME(Either(left_t, right_t), left_t left; right_t right;)

#define RESULT_NAME(ok_t, err_t) result_##ok_t##_##err_t##_t
#define Result(ok_t, err_t) struct RESULT_NAME(ok_t, err_t)
#define DEF_RESULT(ok_t, err_t)                                                \
  _DEF_CLASS_STRICTNAME(RESULT_NAME(ok_t, err_t), ok_t ok; err_t err;)

#define RESULT_OK 0
#define RESULT_ERR 1

/*
  Maybe / Option is often worse than using pointers and checking if null, 
  though if you need pass by value for some reason consider it
*/
#define Maybe(type_t) struct maybe_##type_t_t
#define DEF_MAYBE(type_t)                                                      \
  Maybe(type_t) {                                                              \
    type_t it;                                                                 \
    bool hasit;                                                                \
  }
#define Option(type_t) Maybe(type_t)
#define DEF_OPTION(type_t) DEF_MAYBE(type_t)


// indicates that a pointer is not null
#define ref(type_t) type_t*

// enable using char* in collections
typedef char* str;

