#ifndef HCL_INTRINSICS
#define HCL_INTRINSICS

#if defined(_MSC_VER)
    #include <stdlib.h>
    #define bswap64(x) _byteswap_uint64(x)
#elif defined(__GNUC__) || defined(__clang__)
    #define bswap64(x) __builtin_bswap64(x)
#else
static inline unsigned long long bswap64(unsigned long long x) {
    return (((x & 0x00000000000000FFULL) << 56) |
            ((x & 0x000000000000FF00ULL) << 40) |
            ((x & 0x0000000000FF0000ULL) << 24) |
            ((x & 0x00000000FF000000ULL) << 8) |
            ((x & 0x000000FF00000000ULL) >> 8) |
            ((x & 0x0000FF0000000000ULL) >> 24) |
            ((x & 0x00FF000000000000ULL) >> 40) |
            ((x & 0xFF00000000000000ULL) >> 56));
}
#endif

#if defined(_MSC_VER) && !defined(_DEBUG)
    #define HCL_ASSUME_ALIGNED(ptr, align) __assume(((uintptr_t)(ptr) & ((align) - 1)) == 0)
#elif defined(__GNUC__) || defined(__clang__)
    #define HCL_ASSUME_ALIGNED(ptr, align) (ptr) = (__typeof__(ptr))__builtin_assume_aligned((ptr), (align))
#else
    #define HCL_ASSUME_ALIGNED(ptr, align) (void)0
#endif

// defines a 64 bit simd multiplication
// expects both v1 and v2 to be __m256i
// overwrites v1 with the new value
#define hcl_mm256_mul_epu64(v1, v2)                       \
    do {                                                  \
        __m256i mul_even = _mm256_mul_epu32((v1), (v2));  \
        __m256i v1_odd = _mm256_srli_epi64((v1), 32);     \
        __m256i mul_odd = _mm256_mul_epu32(v1_odd, (v2)); \
        mul_odd = _mm256_slli_epi64(mul_odd, 32);         \
        v1 = _mm256_add_epi64(mul_even, mul_odd);         \
    } while (0)

#endif
