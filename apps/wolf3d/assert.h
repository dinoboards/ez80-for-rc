
#define assert(a)                                                                                                                  \
  if (!(a)) {                                                                                                                      \
    printf("Assertion failed: %s\n", #a);                                                                                          \
    abort();                                                                                                                       \
  }
