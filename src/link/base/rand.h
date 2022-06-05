/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_BASE_RAND_H_
#define LINK_BASE_RAND_H_

#include <cstdint>

namespace nlink {
namespace base {

uint64_t RandUint64();
uint64_t RandUint32();

uint64_t RandGeneratorWithRange(uint64_t range);

int32_t RandInt(int32_t min, int32_t max);

// Returns a random double in range [0, 1). Thread-safe.
double RandDouble();

}  // namespace base
}  // namespace nlink

#endif  // LINK_BASE_RAND_H_
