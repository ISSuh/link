/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_BASE_ENDIAN_H_
#define LINK_BASE_ENDIAN_H_

#include <cstdint>

namespace nlink {
namespace base {

// Returns a value with all bytes in |x| swapped, i.e. reverses the endianness.
inline uint16_t ByteSwap(uint16_t x) {
  return __builtin_bswap16(x);
}

inline uint32_t ByteSwap(uint32_t x) {
  return __builtin_bswap32(x);
}

inline constexpr uint64_t ByteSwap(uint64_t x) {
  return __builtin_bswap64(x);
}

inline uintptr_t ByteSwapUintPtrT(uintptr_t x) {
  static_assert(
    sizeof(uintptr_t) == 4 || sizeof(uintptr_t) == 8,
    "Unsupported uintptr_t size");

  if (sizeof(uintptr_t) == 4) {
    return ByteSwap(static_cast<uint32_t>(x));
  }
  return ByteSwap(static_cast<uint64_t>(x));
}

inline uint16_t ByteSwapToLE16(uint16_t x) {
#if defined(__LITTLE_ENDIAN__)
  return x;
#else
  return ByteSwap(x);
#endif
}

inline uint32_t ByteSwapToLE32(uint32_t x) {
#if defined(__LITTLE_ENDIAN__)
  return x;
#else
  return ByteSwap(x);
#endif
}

inline uint64_t ByteSwapToLE64(uint64_t x) {
#if defined(__LITTLE_ENDIAN__)
  return x;
#else
  return ByteSwap(x);
#endif
}

inline uint16_t NetToHost16(uint16_t x) {
#if defined(__LITTLE_ENDIAN__)
  return ByteSwap(x);
#else
  return x;
#endif
}

inline uint32_t NetToHost32(uint32_t x) {
#if defined(__LITTLE_ENDIAN__)
  return ByteSwap(x);
#else
  return x;
#endif
}

inline uint64_t NetToHost64(uint64_t x) {
#if defined(__LITTLE_ENDIAN__)
  return ByteSwap(x);
#else
  return x;
#endif
}

inline uint16_t HostToNet16(uint16_t x) {
#if defined(__LITTLE_ENDIAN__)
  return ByteSwap(x);
#else
  return x;
#endif
}

inline uint32_t HostToNet32(uint32_t x) {
#if defined(__LITTLE_ENDIAN__)
  return ByteSwap(x);
#else
  return x;
#endif
}

inline uint64_t HostToNet64(uint64_t x) {
#if defined(__LITTLE_ENDIAN__)
  return ByteSwap(x);
#else
  return x;
#endif
}

}  // namespace base
}  // namespace nlink

#endif  // LINK_BASE_ENDIAN_H_
