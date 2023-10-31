#include <stddef.h>
#include <stdint.h>

// #define lambda(lambda$_ret, lambda$_args, lambda$_body)                        \
//   ({                                                                           \
//     lambda$_ret lambda$__anon$ lambda$_args\
// lambda$_body &amp;                                                             \
//     lambda$__anon$;                                                            \
//   })

#define loop while (1)
#define repeat(n) for (size_t __i = 0; __i < n; ++__i)

// taged union / rust style enum / kotlin sealed class
#define CLASS(name, classes)                                                   \
  struct name##_class {                                                        \
    size_t type;                                                               \
    union {                                                                    \
      classes                                                                  \
    } data;                                                                    \
  };

// stateful if
// usee for creating a go style
#define SIF(block, condition, do)                                              \
  {                                                                            \
    block if (condition) { do }                                                \
  }

#define Result(left, right) CLASS(result_##left##_##right##_t, left; right;)
#define Maybe(type) CLASS(maybe_##type##_t, type;)

