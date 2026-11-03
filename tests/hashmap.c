#include "../include/hcl_hashmap.h"
#include "__internal_testing.h"
#include <stdint.h>

// TODO: replace this with the generator used in the xxhash3 gh
void xxhash3() {
	uint64_t expected = 0x2D06800538D394C2ULL;
    uint64_t hash = __hcl_hash_empty();
    hcl_assert_eq(hash, expected);

	expected = 0xC44BDFF4074EECDBULL;
	uint8_t zero = 0;
	hash = __hcl_hash_1to3(&zero, 1);
	hcl_assert_eq(hash, expected);
}

int main(void) {
    hcl_test("hash function");
	xxhash3();
}
