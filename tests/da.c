#include "__internal_testing.h"

// force a realloc
#define HCL_DA_DEFAULT_SIZE 2
#include "../include/hcl_da.h"

hcl_define_da(int)

void da_tests() {
    hcl_arena *arena = hcl_arena_new(1024);

    hcl_da_int *ints = hcl_da_int_init(arena);
    hcl_assert_eq(ints->length, 0);
    hcl_assert_eq(ints->cap, HCL_DA_DEFAULT_SIZE);

    for (int i = 0; i < 4; i++) {
        hcl_da_int_push(ints, i * i);
    }

    hcl_assert_eq(ints->length, 4);
    hcl_assert_eq(ints->cap, 4);
    for (int i = 0; i < 4; i++) {
        hcl_assert_eq(hcl_da_int_get(ints, i), i * i);
    }

    hcl_da_int_clear(ints);
    hcl_assert_eq(ints->length, 0);
    hcl_assert_eq(ints->cap, 4);

    for (int i = 0; i < 8; i++) {
        hcl_da_int_push(ints, i * i);
    }

	hcl_assert_eq(hcl_da_int_get(ints, 7), 7 * 7);
}

int main(void) {
    hcl_test("dynamic array");
    da_tests();
}
