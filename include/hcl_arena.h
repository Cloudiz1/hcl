#ifndef HCL_ARENA
#define HCL_ARENA

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <sys/mman.h>

typedef struct {
    void *base;
    size_t offset;
    size_t capacity;
} hcl_arena;

static inline hcl_arena *hcl_arena_new(size_t size) {
	void *mem = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    hcl_arena *arena = (hcl_arena *) mem;
    arena->base = (char *) mem + sizeof(hcl_arena);
    arena->offset = 0;
    arena->capacity = size;
    return arena;
}

static inline void *hcl_alloc_align(hcl_arena *arena, size_t size, size_t align) {
    uintptr_t addr = (uintptr_t)arena->base + arena->offset;
    uintptr_t pad = -addr & (align - 1);
    uintptr_t remaining = arena->capacity - arena->offset;

    if (pad > remaining || size > remaining - pad) {
        fprintf(stderr, "hcl: arena OOM\n");
        exit(1);
    }

    void *out = (char *)arena->base + arena->offset + pad;
    arena->offset += pad + size;
    return out;
}

static inline void *hcl_alloc(hcl_arena *arena, size_t size) {
	return hcl_alloc_align(arena, size, sizeof(max_align_t));
}

static inline void hcl_arena_clear(hcl_arena *arena) { 
	arena->offset = 0; 
}

static inline void hcl_arena_free(hcl_arena *arena) {
	free(arena); 
}

#endif
