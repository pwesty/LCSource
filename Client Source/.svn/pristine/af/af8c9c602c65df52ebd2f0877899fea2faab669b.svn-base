#define SKADEBUG 1
// special assert for ska 
#if SKADEBUG
  #ifdef NDEBUG
    #define SKAASSERT(__ignore) ((void)0)
  #else
    #define SKAASSERT(expr) ASSERT(expr)
  #endif
#else
  #define SKAASSERT(__ignore) ((void)0)
#endif

