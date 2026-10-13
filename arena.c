#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#pragma once

typedef struct {
	void *pool;
	size_t counter;
	size_t capacity;
} hcl_arena;

hcl_arena *hcl_arena_alloc(size_t size) {
	hcl_arena *arena = malloc(sizeof(hcl_arena));
	arena->pool = malloc(size);
	arena->counter = 0;
	return arena;
}

void hcl_free_arena(hcl_arena *arena) {
	free(arena->pool);
	free(arena);
}

void *hcl_arena_push(hcl_arena *arena, size_t size) {
	if (arena->counter + size > arena->capacity) {
		// probably not ideal, but this is fine for my use case
		// i should however create an expanding arena as well
		fprintf(stderr, "arena OOM");
		exit(1);
	}

	void *out = arena->pool + arena->counter;
	arena->counter += size;
	return out;
}

void *hcl_arena_push_zero(hcl_arena *arena, size_t size) {
	void *out = hcl_arena_push(arena, size);
	memset(out, 0, size);
	return out;
}

void hcl_arena_pop(hcl_arena *arena, size_t size) {
	arena->capacity -= size;
}

void hcl_arena_clear(hcl_arena *arena) {
	arena->counter = 0;
}

