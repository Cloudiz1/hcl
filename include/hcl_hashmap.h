// TODO: make this generic
#include <cstdint>
#include <stdint.h>
#include "hcl_da.h"

#define hcl_define_map(kt, vt, eq, hash) hcl_define_map_alias(kt, vt, kt, eq, hash)
#define hcl_define_map_alias(kt, vt, alias, eq, hash)

typedef struct {
	int k;
	int v;
} hcl_node;

typedef struct {
	uint64_t control;
	hcl_node slots[8];
} hcl_group;

hcl_define_da(hcl_group);

typedef struct {
	hcl_da_hcl_group groups;
	bool (*eq)(int, int);
	uint64_t (*hash)(int, int);
} hcl_hashmap;
