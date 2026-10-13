#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#pragma once

typedef struct {
	void *base;
	size_t counter;
	size_t capacity;
} hcl_arena;

hcl_arena *hcl_arena_alloc(size_t size) {
	void *mem = malloc(sizeof(hcl_arena) + size);
	hcl_arena *arena = mem;
	arena->base = mem + sizeof(hcl_arena);
	arena->counter = 0;
	return arena;
}

void hcl_free_arena(hcl_arena *arena) {
	free(arena);
}

void *hcl_arena_pusha(hcl_arena *arena, size_t size, size_t align) {
	if (arena->counter + size > arena->capacity) {
		fprintf(stderr, "arena OOM");
		exit(1);
	}

	arena->counter += ((ptrdiff_t) arena->base + arena->counter) & (align - 1);

	void *out = arena->base + arena->counter;
	arena->counter += size;
	return out;
}

void hcl_arena_clear(hcl_arena *arena) {
	arena->counter = 0;
}
