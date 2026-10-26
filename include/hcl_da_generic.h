/* this is simply a wrapper around "hcl_da.h", you must include that and define the types using the macros found there first
 * example usage:
 * #include "hcl_da.h"
 * #define HCL_DA_TYPES(X) \
 * 		X(int)             \
 * 		X(float)           \
 * 		X(char_ptr)        \
 * #include "hcl_da_generic.h"
 * ...
 */

// its important these are the aliases from hcl_da_define(type, alias)
#ifdef __HCL_EXAMPLE_BOILTERPLATE
#include "hcl_da.h"

hcl_define_da(int)
hcl_define_da_alias(char *, nstr)

#define HCL_DA_ALIASES(X) \
    X(int)                \
    X(nstr)

#include "hcl_da_generic.h"
#endif

#ifndef HCL_DA_GENERIC
#define HCL_DA_GENERIC

#ifndef HCL_DA
#error "hcl_da.h" must be included first
#endif

#ifndef HCL_DA_ALIASES
#error HCL_DA_ALIASES must be defined. Refer to "hcl_da_generic.h" for an example
#endif

// clang-format off
__attribute__((error("Unknown type in _Generic call. Check your definition of HCL_DA_TYPES.")))
void __hcl_da_generic_unknown(void *_a, ...) {}

#define __HCL_PUSH_CASE(alias) hcl_da_##alias *: hcl_da_##alias##_push,
#define hcl_da_push(arena, x) _Generic((arena), \
	HCL_DA_ALIASES(__HCL_PUSH_CASE)             \
	default: __hcl_da_generic_unknown           \
)(arena, x)

#define __HCL_GET_CASE(alias) hcl_da_##alias *: hcl_da_##alias##_get,
#define hcl_da_get(arena, x) _Generic((arena), \
	HCL_DA_ALIASES(__HCL_GET_CASE)             \
	default: __hcl_da_generic_unknown          \
)(arena, x)

#define __HCL_CLEAR_CASE(alias) hcl_da_##alias *: hcl_da_##alias##_clear,
#define hcl_da_clear(arena) _Generic((arena), \
	HCL_DA_ALIASES(__HCL_CLEAR_CASE)          \
	default: __hcl_da_generic_unknown         \
)(arena)

#endif
