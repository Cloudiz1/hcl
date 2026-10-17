#ifndef HCL_DA
#define HCL_DA

#include <stdalign.h>

#include "hcl_arena.h"

#ifndef HCL_DA_DEFAULT_SIZE
#define HCL_DA_DEFAULT_SIZE 16
#endif

#ifndef HCL_DA_SCALE_FACTOR
#define HCL_DA_SCALE_FACTOR 16
#endif

// clang-format off
#define hcl_define_da(type, name)    \
    __hcl_define_da(type, name)      \
	__hcl_define_da_init(type, name) \
	__hcl_define_da_push(type, name) \
	__hcl_define_da_clear(name)
// clang-format on

#define __hcl_define_da(type, name) \
    typedef struct {                \
        type *data;                 \
        hcl_arena *arena;           \
        size_t length;              \
        size_t cap;                 \
    } hcl_da##name;

#define __hcl_define_da_init(type, name)                               \
    hcl_da##name *hcl_da_##name(hcl_arena *arena) {                    \
        void *mem = hcl_alloc(                                         \
            arena,                                                     \
            sizeof(hcl_da##name) + HCL_DA_DEFAULT_SIZE * sizeof(type), \
            alignof(hcl_da##name));                                    \
                                                                       \
        hcl_da##name *da = (hcl_da##name *)mem;                        \
        da->data = (char *)mem + sizeof(hcl_da##name);                 \
        da->arena = arena;                                             \
        da->length = 0;                                                \
        da->cap = HCL_DA_DEFAULT_SIZE;                                 \
        return da;                                                     \
    }

#define __hcl_define_da_push(type, name)               \
    void hcl_da_push(hcl_arena##name *da, type data) { \
        if (da->length >= da->cap) {                   \
            type *old = da->data;                      \
            size_t size = da->cap * sizeof(type);      \
            da->data = hcl_alloc(                      \
                da->arena,                             \
                HCL_DA_SCALE_FACTOR * size,            \
                alignof(type));                        \
                                                       \
            memcpy(da->data, old, size);               \
            arena->count *= HCL_DA_SCALE_FACTOR;       \
        }                                              \
                                                       \
        da->data[da->length] = data;                   \
        da->length++;                                  \
    }

#define __hcl_define_da_clear(name)          \
    void hcl_da_clear(hcl_arena##name *da) { \
        da->length = 0;                      \
    }

#endif
