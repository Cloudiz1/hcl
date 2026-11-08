#ifndef HCL_HASHMAP
#define HCL_HASHMAP

#define u8 uint8_t
#define u32 uint32_t
#define u64 uint64_t
#define u128 unsigned __int128
#define usize size_t

#include <stdint.h>
#include <string.h>

static const u64 PRIME32_1 = 0x9E3779B1U;           // 0b10011110001101110111100110110001
static const u64 PRIME32_2 = 0x85EBCA77U;           // 0b10000101111010111100101001110111
static const u64 PRIME32_3 = 0xC2B2AE3DU;           // 0b11000010101100101010111000111101
static const u64 PRIME64_1 = 0x9E3779B185EBCA87ULL; // 0b1001111000110111011110011011000110000101111010111100101010000111
static const u64 PRIME64_2 = 0xC2B2AE3D27D4EB4FULL; // 0b1100001010110010101011100011110100100111110101001110101101001111
static const u64 PRIME64_3 = 0x165667B19E3779F9ULL; // 0b0001011001010110011001111011000110011110001101110111100111111001
static const u64 PRIME64_4 = 0x85EBCA77C2B2AE63ULL; // 0b1000010111101011110010100111011111000010101100101010111001100011
static const u64 PRIME64_5 = 0x27D4EB2F165667C5ULL; // 0b0010011111010100111010110010111100010110010101100110011111000101
static const u64 PRIME_MX1 = 0x165667919E3779F9ULL; // 0b0001011001010110011001111001000110011110001101110111100111111001
static const u64 PRIME_MX2 = 0x9FB21C651E98DF25ULL; // 0b1001111110110010000111000110010100011110100110001101111100100101

// clang-format off
static const u8 __hcl_secret[192] = {
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

static inline u32 __hcl_read4(const void *data, usize offset) {
    u32 d;
    memcpy(&d, (char *)data + offset, 4);
    return d;
}

static inline u64 __hcl_read8(const void *data, usize offset) {
    u64 d;
    memcpy(&d, (char *)data + offset, 8);
    return d;
}

static inline u64 __hcl_rs8(usize offset) {
    return __hcl_read8((void *)__hcl_secret, offset);
}

static inline u64 __hcl_avalanche(uint64_t x) {
    x ^= (x >> 37);
    x *= PRIME_MX1;
    x ^= (x >> 32);
    return x;
}

static inline u64 __hcl_avalanche64(uint64_t x) {
    x ^= (x >> 33);
    x *= PRIME64_2;
    x ^= (x >> 29);
    x *= PRIME64_3;
    x ^= (x >> 32);
    return x;
}

#define __hcl_rotl(x, r) ((x) << (r) | (x) >> (64 - (r)))

static inline u64 __hcl_hash_empty() {
    u64 s[2] = {__hcl_rs8(56), __hcl_rs8(64)};
    return __hcl_avalanche64(s[0] ^ s[1]);
}

static inline u64 __hcl_hash_1to3(const void *in, usize len) {
    const u8 *in_s = (u8 *)in;
    u32 combined = in_s[len - 1] | len << 8 | in_s[0] << 16 | in_s[len >> 1] << 24;
    u64 secret = __hcl_rs8(0);
    u32 high = secret >> 32;
    u32 low = secret;

    u64 val = high ^ low ^ combined;
    return __hcl_avalanche64(val);
}

static inline u64 __hcl_hash_4to8(const void *in, usize len) {
	u32 f = __hcl_read4(in, 0);
	u32 l = __hcl_read4(in, len - 4);
	u64 combined = l | (u64)f << 32;

    u64 secret[2] = {__hcl_rs8(8), __hcl_rs8(16)};
    u64 val = secret[0] ^ secret[1] ^ combined;

    val ^= __hcl_rotl(val, 49) ^ __hcl_rotl(val, 24);
    val *= PRIME_MX2;
    val ^= (val >> 35) + len;
    val *= PRIME_MX2;
    val ^= val >> 28;
    return val;
}

static inline u64 __hcl_hash_9to16(const void *in, usize len) {
    u64 f = __hcl_read8(in, 0);
	u64 l = __hcl_read8(in, len - 8);

	u64 s[2] = {
		__hcl_rs8(24) ^ __hcl_rs8(32),
		__hcl_rs8(40) ^ __hcl_rs8(48)
	};

	u64 low = s[0] ^ f;
	u64 high = s[1] ^ l;
	u128 mul_result = (u128)low * high;
	u64 hr = mul_result >> 64;
	u64 lr = mul_result;

	u64 val = len + __builtin_bswap64(low) + high + (lr ^ hr);
	return __hcl_avalanche(val);
}

static inline u64 __hcl_mix_step(u8 in[16], usize secret_offset) {
	u64 data[2] = {__hcl_read8(in, 0), __hcl_read8(in, 8)};
	u64 secret[2] = {__hcl_rs8(secret_offset), __hcl_rs8(secret_offset + 8)};
	u128 mul_res = (u128)(data[0] ^ secret[0]) * (data[1] ^ secret[1]);

	u64 higher = mul_res >> 64;
	u64 lower = mul_res;

  	return lower ^ higher;
}

static inline u64 __hcl_hash_17to128(void *data, usize len) {
	u64 acc = len * PRIME64_1;
	u64 nrounds = ((len - 1) >> 5) + 1;

	u8 mix_step_in[16];
	for (int i = nrounds - 1; i >= 0; i--) {
		usize offset_s = i * 16;
		usize offset_e = len - i * 16 - 16;

		memcpy(mix_step_in, (char *)data + offset_s, 16);
		acc += __hcl_mix_step(mix_step_in, i*32);

		memcpy(mix_step_in, (char *)data + offset_e, 16);
		acc += __hcl_mix_step(mix_step_in, i*32+16);
	}

	return __hcl_avalanche(acc);
}

static inline u64 __hcl_hash_129to240(void *data, usize len) {
	u64 acc = len * PRIME64_1;

	for (int i = 0; i < 8; i++) {
		acc += __hcl_mix_step((u8 *)(char *)data + i * 16, i * 16);
	}

	acc = __hcl_avalanche(acc);

	u64 num_chunks = len >> 4;
	for (int i = 8; i < num_chunks; i++) {
		acc += __hcl_mix_step((u8 *)(char *)data + i * 16, (i - 8) * 16 + 3);
	}

	acc += __hcl_mix_step((u8 *)(char *)data + (len - 16), 119);
	return __hcl_avalanche(acc);
}

// all of these change with secret, but my secret is constant and default
#define HCL_SECRET_LEN 192 
#define HCL_SPB 16 // stripesPerBlock
#define HCL_BLOCK_SIZE 1024

// TODO: we can vectorize that loop
static inline void __hcl_accumulate(u64 acc[8], const u64 stripe[8], usize secret_offset) {
	// TODO: this is constant, we should make that more clear
	u64 secret_words[8];
	memcpy(secret_words, (char *)__hcl_secret + secret_offset, 64);

	for (int i = 0; i < 8; i++) {
		u64 value = stripe[i] ^ secret_words[i];
		acc[i ^ 1] += stripe[i];

		u32 higher = value >> 32;
		u32 lower = value;

		acc[i] += (u64)lower * higher;
	}
}

static inline void __hcl_round_accumulate(u64 acc[8], u8 block[HCL_BLOCK_SIZE]) {
	for (int n = 0; n < HCL_SPB; n++) {
		u64 stripe[8];
		memcpy(stripe, (char *)block + n * 64, 64);
		__hcl_accumulate(acc, stripe, n * 8);
	}
}

// TODO: this can be vectorized too
static inline void __hcl_round_scramble(u64 acc[8]) {
	for (int i = 0; i < 8; i++) {
		acc[i] ^= acc[i] >> 47;
		acc[i] ^= __hcl_rs8(HCL_SECRET_LEN - 64 + i * 8);
		acc[i] *= PRIME32_1;
	}
}

static inline void __hcl_round(u64 acc[8], u8 block[HCL_BLOCK_SIZE]) {
	__hcl_round_accumulate(acc, block);
	__hcl_round_scramble(acc);
}

static inline u64 __hcl_hash_large(void *data, usize len) {
	usize secret_offset = 11;

	u64 result = len * PRIME64_1;
	const u64 *last_stripe = (u64 *)((char *)data + len - 64);

	u64 acc[8] = {
		PRIME32_3, PRIME64_1, PRIME64_2, PRIME64_3,
		PRIME64_4, PRIME32_2, PRIME64_5, PRIME32_1
	};

	int i = 0;
	while (len > HCL_BLOCK_SIZE) {
		__hcl_round(acc, (u8 *)((char *)data + i * HCL_BLOCK_SIZE));
		len -= HCL_BLOCK_SIZE;
		i++;
	}

	usize n_full_stripes = (len - 1)/64;
	for (int n = 0; n < n_full_stripes; n++) {
		u64 *stripe = (u64 *)((char *)data + i * HCL_BLOCK_SIZE + n * 64);
		__hcl_accumulate(acc, stripe, n * 8);
	}

	__hcl_accumulate(acc, last_stripe, HCL_SECRET_LEN - 71);

	u64 secret_words[8];
	memcpy(secret_words, (char *)__hcl_secret + secret_offset, 64);

	for (i = 0; i < 4; i++) {
		u128 mul_result = (u128)(acc[i*2] ^ secret_words[i*2]) *
			(acc[i*2+1] ^ secret_words[i*2+1]);

		u64 higher = mul_result >> 64;
		u64 lower = mul_result;

		result += lower ^ higher;
	}

	return __hcl_avalanche(result);
}

static inline u64 hcl_hash(void *in, size_t len) {
	if (len == 0) return __hcl_hash_empty();
	else if (len <= 3) return __hcl_hash_1to3(in, len);
	else if (len <= 8) return __hcl_hash_4to8(in, len);
	else if (len <= 16) return __hcl_hash_9to16(in, len);
	else if (len <= 128) return __hcl_hash_17to128(in, len);
	else if (len <= 240) return __hcl_hash_129to240(in, len);
	else return __hcl_hash_large(in, len);
}

#endif
