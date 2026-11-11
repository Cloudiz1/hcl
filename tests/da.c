#include "__internal_testing.h"

// force a realloc
#define HCL_DA_DEFAULT_SIZE 2
#include "../include/hcl_da.h"

hcl_define_da(int)
hcl_define_da_alias(char *, nstr)

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
	hcl_arena_free(arena);
}

#define HCL_DA_ALIASES(X) \
    X(int)                \
    X(nstr)

#include "../include/hcl_da_generic.h"

void generic_da_test() {
	hcl_arena *arena = hcl_arena_new(1024);
	hcl_da_int *dai = hcl_da_int_init(arena);
	hcl_da_nstr *das = hcl_da_nstr_init(arena);

	hcl_da_push(dai, 2);
	hcl_da_push(das, "meow");
	hcl_da_push(das, ":3");

	hcl_assert_eq(dai->length, 1);
	hcl_assert_eq(hcl_da_get(dai, 0), 2);

	hcl_assert_eq(das->length, 2);
	hcl_assert_str_eq(das->data[0], "meow");
	hcl_assert_str_eq(das->data[1], ":3");

	hcl_da_clear(dai);
	hcl_assert_eq(dai->length, 0);
	hcl_assert_eq(das->length, 2);

	hcl_da_clear(das);
	hcl_assert_eq(das->length, 0);

	hcl_arena_free(arena);
}

hcl_define_da_alias(int *, ip)

void da_ptrs_test() {
	hcl_arena *arena = hcl_arena_new(1024);
	hcl_da_ip *ip = hcl_da_ip_init(arena);	

	for (int i = 0; i < 4; i++) {
		int *a = hcl_alloc(arena, 4*sizeof(int));
		int s = i*4;
		a[0] = s;
		a[1] = s + 1;
		a[2] = s + 2;
		a[3] = s + 3;

		hcl_da_ip_push(ip, a);
	}

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			hcl_assert(ip->data[i][j] == i*4+j);
		}
	}
}

int main(void) {
    hcl_test("dynamic array");
    da_tests();

    hcl_test("generic dynamic array");
	generic_da_test();

    hcl_test("dynamic array w pointers");
	da_ptrs_test();
	return 0;
}
