#ifndef HCL_TESTING
#define HCL_TESTING

#include <stdio.h>

#define HCL_TESTING_GREEN "\033[32m"
#define HCL_TESTING_RED "\033[31m"
#define HCL_TESTING_CLEAR "\033[0m"

#define hcl_test(name) fprintf(stderr, "module: %s\n", name)
#define __hcl_print_test(name) fprintf(stderr, "    %s: ", name)

#define __hcl_pass() fprintf(stderr, HCL_TESTING_GREEN "passed\n" HCL_TESTING_CLEAR)
#define __hcl_fail() fprintf(stderr, HCL_TESTING_RED "FAILED\n" HCL_TESTING_CLEAR)

#define hcl_assert(condition)     \
    do {                          \
        __hcl_print_test(#condition); \
        if (condition)            \
            __hcl_pass();         \
        else                      \
            __hcl_fail();         \
    } while (0)

#define hcl_assert_eq(lhs, rhs)         \
    do {                                \
        __hcl_print_test(#lhs " == " #rhs); \
        if (lhs == rhs)                 \
            __hcl_pass();               \
        else                            \
            __hcl_fail();               \
    } while (0)

#endif
