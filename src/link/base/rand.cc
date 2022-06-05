/**
 *
 *  Copyright:  Copyright (c) 2022, ISSuh
 *
 */

#include "link/base/rand.h"

#include <random>

namespace nlink {
namespace base {

uint64_t RandUint64() {
  return 0;
}

uint64_t RandUint32() {
  return 0;
}

uint64_t RandGeneratorWithRange(uint64_t range) {
  return 0;
}

int32_t RandInt(int32_t min, int32_t max) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<int> dis(min, max);
  return dis(gen);
}

// Returns a random double in range [0, 1). Thread-safe.
double RandDouble() {
  return 0;
}

}  // namespace base
}  // namespace nlink