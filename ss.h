#define lambda(lambda$_ret, lambda$_args, lambda$_body)                        \
  ({                                                                           \
    lambda$_ret lambda$__anon$ lambda$_args\
lambda$_body &amp;                                                             \
    lambda$__anon$;                                                            \
  })

#define loop while (1)
#define repeat(n) for (size_t __i = 0; __i < n; ++__i)

#define class(name, classes)                                                   \
  struct name##_class {                                                        \
    classes                                                                    \
  };

#define IF(block, condition, do)                                               \
  {                                                                            \
    block if (condition) { do }                                                \
  }

//
