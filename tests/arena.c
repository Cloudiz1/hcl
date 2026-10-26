#include "../include/hcl_arena.h"
#include "__internal_testing.h"

void arena_tests() {
    hcl_arena *arena = hcl_arena_new(1024);
    hcl_assert_eq(arena->offset, 0);
    hcl_assert_eq(arena->capacity, 1024);

    typedef struct {
        int bar;
        char baz;
    } foo;

    // offset will be 48 (6 * 8) bytes due to padding
    foo *a = hcl_alloc_align(arena, 6 * sizeof(foo), sizeof(int));
    hcl_assert_eq(arena->offset, 48);

    // alignment
    hcl_assert_eq((uintptr_t)a % sizeof(int), 0);
    hcl_assert_eq(((uintptr_t)arena->base + arena->offset) % sizeof(int), 0);

    // misalign by 1 (offset = 49)
    hcl_alloc_align(arena, sizeof(char), sizeof(char));

    // check for alignment again
    int *b = hcl_alloc_align(arena, sizeof(int), sizeof(int));
    hcl_assert_eq((uintptr_t)b % sizeof(int), 0);

    // we expect 3 byte padding for 4 byte alignment.
    // offset should be at 56 (49 + 3 + 4)
    hcl_assert_eq(arena->offset, 56);

    // should align to max_ali
    int *c = hcl_alloc(arena, sizeof(char));
    hcl_assert_eq((uintptr_t)c % sizeof(max_align_t), 0);
}

int main(void) {
    hcl_test("arena");
    arena_tests();
}
