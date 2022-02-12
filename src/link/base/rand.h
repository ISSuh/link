/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#ifndef LINK_BASE_RAND_H_
#define LINK_BASE_RAND_H_

#include <stddef.h>

uint64_t RandUint64();
uint64_t RandUint32();

uint64_t RandGeneratorWithRange(uint64_t range);

int32_t RandInt(int32_t min, int32_t max);

// Returns a random double in range [0, 1). Thread-safe.
BASE_EXPORT double RandDouble();

#endif  // LINK_BASE_RAND_H_
