#ifndef HCL_HASHMAP
#define HCL_HASHMAP

#include "hcl_da.h"

#include <cstdint>
#include <stdint.h>
#include <string.h>

static const uint64_t PRIME32_1 = 0x9E3779B1U;           // 0b10011110001101110111100110110001
static const uint64_t PRIME32_2 = 0x85EBCA77U;           // 0b10000101111010111100101001110111
static const uint64_t PRIME32_3 = 0xC2B2AE3DU;           // 0b11000010101100101010111000111101
static const uint64_t PRIME64_1 = 0x9E3779B185EBCA87ULL; // 0b1001111000110111011110011011000110000101111010111100101010000111
static const uint64_t PRIME64_2 = 0xC2B2AE3D27D4EB4FULL; // 0b1100001010110010101011100011110100100111110101001110101101001111
static const uint64_t PRIME64_3 = 0x165667B19E3779F9ULL; // 0b0001011001010110011001111011000110011110001101110111100111111001
static const uint64_t PRIME64_4 = 0x85EBCA77C2B2AE63ULL; // 0b1000010111101011110010100111011111000010101100101010111001100011
static const uint64_t PRIME64_5 = 0x27D4EB2F165667C5ULL; // 0b0010011111010100111010110010111100010110010101100110011111000101
static const uint64_t PRIME_MX1 = 0x165667919E3779F9ULL; // 0b0001011001010110011001111001000110011110001101110111100111111001
static const uint64_t PRIME_MX2 = 0x9FB21C651E98DF25ULL; // 0b1001111110110010000111000110010100011110100110001101111100100101

static const uint64_t __hcl_seed = 0;

// clang-format off
static const uint8_t __hcl_secret[192] = {
  0xb8, 0xfe, 0x6c, 0x39, 0x23, 0xa4, 0x4b, 0xbe, 0x7c, 0x01, 0x81, 0x2c, 0xf7, 0x21, 0xad, 0x1c,
  0xde, 0xd4, 0x6d, 0xe9, 0x83, 0x90, 0x97, 0xdb, 0x72, 0x40, 0xa4, 0xa4, 0xb7, 0xb3, 0x67, 0x1f,
  0xcb, 0x79, 0xe6, 0x4e, 0xcc, 0xc0, 0xe5, 0x78, 0x82, 0x5a, 0xd0, 0x7d, 0xcc, 0xff, 0x72, 0x21,
  0xb8, 0x08, 0x46, 0x74, 0xf7, 0x43, 0x24, 0x8e, 0xe0, 0x35, 0x90, 0xe6, 0x81, 0x3a, 0x26, 0x4c,
  0x3c, 0x28, 0x52, 0xbb, 0x91, 0xc3, 0x00, 0xcb, 0x88, 0xd0, 0x65, 0x8b, 0x1b, 0x53, 0x2e, 0xa3,
  0x71, 0x64, 0x48, 0x97, 0xa2, 0x0d, 0xf9, 0x4e, 0x38, 0x19, 0xef, 0x46, 0xa9, 0xde, 0xac, 0xd8,
  0xa8, 0xfa, 0x76, 0x3f, 0xe3, 0x9c, 0x34, 0x3f, 0xf9, 0xdc, 0xbb, 0xc7, 0xc7, 0x0b, 0x4f, 0x1d,
  0x8a, 0x51, 0xe0, 0x4b, 0xcd, 0xb4, 0x59, 0x31, 0xc8, 0x9f, 0x7e, 0xc9, 0xd9, 0x78, 0x73, 0x64,
  0xea, 0xc5, 0xac, 0x83, 0x34, 0xd3, 0xeb, 0xc3, 0xc5, 0x81, 0xa0, 0xff, 0xfa, 0x13, 0x63, 0xeb,
  0x17, 0x0d, 0xdd, 0x51, 0xb7, 0xf0, 0xda, 0x49, 0xd3, 0x16, 0x55, 0x26, 0x29, 0xd4, 0x68, 0x9e,
  0x2b, 0x16, 0xbe, 0x58, 0x7d, 0x47, 0xa1, 0xfc, 0x8f, 0xf8, 0xb8, 0xd1, 0x7a, 0xd0, 0x31, 0xce,
  0x45, 0xcb, 0x3a, 0x8f, 0x95, 0x16, 0x04, 0x28, 0xaf, 0xd7, 0xfb, 0xca, 0xbb, 0x4b, 0x40, 0x7e,
};
// clang-format on

// rotate left
// r != 0
#define __hcl_rotl(x, r) ((x) << (r) | (x) >> (64 - (r)))

// read 64 bits from an array of uint8
static inline uint64_t __hcl_rarr64(const uint8_t *p) {
    return (uint64_t)p[0] |
           ((uint64_t)p[1] << 8) |
           ((uint64_t)p[2] << 16) |
           ((uint64_t)p[3] << 24) |
           ((uint64_t)p[4] << 32) |
           ((uint64_t)p[5] << 40) |
           ((uint64_t)p[6] << 48) |
           ((uint64_t)p[7] << 56);
}

static inline uint64_t __hcl_avalanche(uint64_t x) {
    x = x ^ (x >> 37);
    x = x * PRIME_MX1;
    x = x ^ (x >> 32);
    return x;
}

static inline uint64_t __hcl_avalanche64(uint64_t x) {
    x = x ^ (x >> 33);
    x = x * PRIME64_2;
    x = x ^ (x >> 29);
    x = x * PRIME64_3;
    x = x ^ (x >> 32);
    return x;
}

static inline uint64_t __hcl_hash_empty() {
    uint64_t s1 = __hcl_rarr64(__hcl_secret + 56);
    uint64_t s2 = __hcl_rarr64(__hcl_secret + 64);
    return __hcl_avalanche64(__hcl_seed ^ s1 ^ s2);
}

static inline uint64_t __hcl_hash_1to3(const uint8_t *in, size_t len) {
    uint32_t combined = in[len - 1] | len << 8 | in[0] << 16 | in[len >> 1] << 24;
    uint64_t secret = __hcl_rarr64(__hcl_secret);
    uint32_t high = secret >> 32;
    uint32_t low = secret;

    uint64_t val = ((high ^ low) + __hcl_seed) ^ combined;
    return __hcl_avalanche64(val);
}

static inline uint64_t __hcl_hash_4to8(const uint64_t in, size_t len) {
    // omit the modifiedSeed found in the original implementation
    // since it reduces to zero on a default seed

    uint64_t secret = __hcl_rarr64(__hcl_secret + 8) ^ __hcl_rarr64(__hcl_secret + 16);

    // lhs - seed is a no op, omit
    uint64_t val = secret ^ in;
    val ^= __hcl_rotl(val, 49) ^ __hcl_rotl(val, 24);
    val *= PRIME_MX2;
    val ^= (val >> 35) + len;
    val *= PRIME_MX2;
    val ^= val >> 28;
    return val;
}

static inline uint64_t __hcl_hash_8to16(const uint8_t *in, size_t len) {
    uint64_t f = __hcl_rarr64(in);

    // cap the u64 to only bytes we own, if the size is 9, 10-16 is garbage!!
    uint64_t l = __hcl_rarr64(in + len - 8);

    uint64_t s1 = __hcl_rarr64(__hcl_secret + 24) ^ __hcl_rarr64(__hcl_secret + 32);
    uint64_t s2 = __hcl_rarr64(__hcl_secret + 40) ^ __hcl_rarr64(__hcl_secret + 48);

    // again, plus seed minus seed is a no op, omit
    uint64_t low = s1 ^ f;
    uint64_t high = s2 ^ l;
    unsigned __int128 mulResult = low * high;
    uint64_t hr = mulResult >> 64;
    uint64_t lr = mulResult;

    uint64_t val = len + __builtin_bswap64(low) + high + (lr ^ hr);
    return __hcl_avalanche(val);
}

static inline uint64_t __hcl_hash_small(void *data, size_t len) {
    if (len == 0) {
        return __hcl_hash_empty();
    } else if (len < 4) {
        return __hcl_hash_1to3((uint8_t *)data, len);
    } else if (len < 8) {
        return __hcl_hash_4to8((uint64_t)data, len);
    } else {
        return __hcl_hash_8to16((uint8_t *)data, len);
    }
}

static inline uint64_t __hcl_mix_step(uint8_t *data, size_t secret_offset) {
    uint64_t data_words[2] = {
        __hcl_rarr64(data),
        __hcl_rarr64(data + 8),
    };

    uint64_t secret_words[2] = {
        __hcl_rarr64(__hcl_secret + secret_offset),
        __hcl_rarr64(__hcl_secret + secret_offset + 16),
    };

    // omit seed. seed of zero is no op
    unsigned __int128 mul_result = (data_words[0] ^ secret_words[0]) *
                                   (data_words[1] ^ secret_words[1]);

    uint64_t hr = mul_result >> 64;
    uint64_t lr = mul_result;
    return hr ^ lr;
}

static inline uint64_t __hcl_hash_17to128(void *data, size_t len) {
    uint64_t acc = len * PRIME64_1;
    uint64_t nrounds = ((len - 1) >> 5) + 1;
    for (int i = nrounds - 1; i >= 0; i--) {
        size_t offset_s = i * 16;
        size_t offset_e = len - i * 16 - 16;

        unsigned __int128 d1, d2;
        memcpy(&d1, (char *)data + offset_s, 16);
        memcpy(&d2, (char *)data + offset_e, 16);

        acc += __hcl_mix_step((uint8_t *)d1, (size_t)i * 32);
        acc += __hcl_mix_step((uint8_t *)d2, (size_t)i * 32 + 16);
    }

    return __hcl_avalanche(acc);
}

static inline uint64_t __hcl_hash_129to240(void *data, size_t len) {
    uint64_t acc = len * PRIME64_1;
    uint64_t chunks = len >> 4;
    for (int i = 0; i < 8; i++) {
        uint8_t *d;
        memcpy((char *)data + i * 16, &d, 16);
        acc += __hcl_mix_step(d, i * 16);
    }

    acc = __hcl_avalanche(acc);

    for (int i = 0; i < 8; i++) {
        unsigned __int128 d;
        memcpy(&d, (char *)data + i * 16, 16);
        acc += __hcl_mix_step((uint8_t *)d, (i - 8) * 16 + 3);
    }

	unsigned __int128 d;
	memcpy(&d, (char *)data - 16, 16);
	acc += __hcl_mix_step((uint8_t *)d, 119);
	return __hcl_avalanche(acc);
}

#define hcl_define_map(kt, vt, eq, hash) hcl_define_map_alias(kt, vt, kt, eq, hash)

#define hcl_define_map_alias(kt, vt, alias, eq, hash) \
    typedef struct {                                  \
        kt k;                                         \
        kv v;                                         \
    } hcl_node_##alias;                               \
                                                      \
    typedef struct {                                  \
        uint64_t control;                             \
        hcl_node_##alias slots[8];                    \
    } hcl_group_##alias;                              \
                                                      \
    hcl_define_da(hcl_group_##alias);                 \
                                                      \
    typedef struct {                                  \
        hcl_da_hcl_group_##alias groups;              \
    } hcl_hashmap_##alias;

#endif
