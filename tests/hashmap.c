#include "../include/hcl_hashmap.h"
#include "__internal_testing.h"
#include "__hashmap_cases.h"

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define PRIME32 2654435761U
#define PRIME64 11400714785074694797ULL

void xxhash3() {
	__hcl_print_test("xxhash3");
	size_t const buffer_size = 4160;

	u8* const buffer = malloc(buffer_size);
	if (buffer == NULL) {
		fprintf(stderr, "could not allocate buffer.");
		exit(1);
	}

	u64 byte_gen = PRIME32;
	for (int i = 0; i < buffer_size; i++) {
		buffer[i] = (u8)(byte_gen >> 56);
		byte_gen *= PRIME64;
	}

	// recall the length is exactly i
	for (int i = 0; i < buffer_size; i++) {
		u64 expected = hash_cases[i];
		u64 hash = hcl_hash(buffer, i);
		if (hash != expected) {
			__hcl_fail();
			fprintf(stderr, "    xxhash3 failed on case: %d.\n"
		   									"    expected: %lX\n"
		   									"    found:    %lX\n", i, expected, hash);
			exit(1);
		}
	}

	__hcl_pass();
}

int main(void) {
    hcl_test("hash function");
	xxhash3();
}
