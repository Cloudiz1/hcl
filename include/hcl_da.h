#ifndef HCL_DA
#define HCL_DA

#include "hcl_arena.h"
#include <stdalign.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#ifndef HCL_DA_DEFAULT_SIZE
#define HCL_DA_DEFAULT_SIZE 8
#endif

#ifndef HCL_DA_SCALE_FACTOR
#define HCL_DA_SCALE_FACTOR 2
#endif

#define hcl_define_da(type) hcl_define_da_alias(type, type)

#define hcl_define_da_alias(type, alias)                                                    \
    typedef struct {                                                                        \
        type *data;                                                                         \
        hcl_arena *arena;                                                                   \
        size_t length;                                                                      \
        size_t cap;                                                                         \
    } hcl_da_##alias;                                                                       \
                                                                                            \
    static inline hcl_da_##alias *hcl_da_##alias##_init_c(hcl_arena *arena, size_t count) { \
        void *mem = hcl_alloc_align(                                                        \
            arena,                                                                          \
            sizeof(hcl_da_##alias) + count * sizeof(type),                                  \
            alignof(hcl_da_##alias));                                                       \
                                                                                            \
        hcl_da_##alias *da = (hcl_da_##alias *)mem;                                         \
        da->data = (type *)((char *)mem + sizeof(hcl_da_##alias));                          \
        da->arena = arena;                                                                  \
        da->length = 0;                                                                     \
        da->cap = HCL_DA_DEFAULT_SIZE;                                                      \
        return da;                                                                          \
    }                                                                                       \
                                                                                            \
    static inline hcl_da_##alias *hcl_da_##alias##_init(hcl_arena *arena) {                 \
        return hcl_da_##alias##_init_c(arena, HCL_DA_DEFAULT_SIZE);                         \
    }                                                                                       \
                                                                                            \
    static inline void hcl_da_##alias##_push(hcl_da_##alias *da, type data) {               \
        if (da->length >= da->cap) {                                                        \
            type *old = da->data;                                                           \
            size_t org_size = da->cap * sizeof(type);                                       \
            da->data = (type *)hcl_alloc_align(                                             \
                da->arena,                                                                  \
                HCL_DA_SCALE_FACTOR * org_size,                                             \
                alignof(type));                                                             \
                                                                                            \
            memcpy(da->data, old, org_size);                                                \
            da->cap *= HCL_DA_SCALE_FACTOR;                                                 \
        }                                                                                   \
                                                                                            \
        da->data[da->length] = data;                                                        \
        da->length++;                                                                       \
    }                                                                                       \
                                                                                            \
    static inline type hcl_da_##alias##_get(hcl_da_##alias *da, size_t index) {             \
        if (index > da->length) {                                                           \
            fprintf(stderr, "hcl: out of bounds index\n");                                  \
            exit(1);                                                                        \
        }                                                                                   \
                                                                                            \
        return da->data[index];                                                             \
    }                                                                                       \
                                                                                            \
    static inline void hcl_da_##alias##_clear(hcl_da_##alias *da) {                         \
        da->length = 0;                                                                     \
    }

#endif
