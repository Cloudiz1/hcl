#include "__internal_testing.h"

#include "../include/hcl_da.h"

hcl_define_da(int)
hcl_define_da_alias(char *, nstr)

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
}

int main(void) {
	hcl_test("generic dynamic arrays");
	generic_da_test();
}
