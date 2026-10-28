#ifndef HCL_HELPER
#define HCL_HELPER

#define hcl_likely(x) __builtin_expect(x, 1)
#define hcl_unlikely(x) __builtin_expect(x, 0)

#endif
