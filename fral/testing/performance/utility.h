#ifndef FRAL_TESTING_PERFORMANCE_TESTS_UTILITY_H
#define FRAL_TESTING_PERFORMANCE_TESTS_UTILITY_H

#include <chrono>

#include "../../engine/engine.h"

using namespace std::chrono;
const int GB_TO_B = 1000000000;

int sqlite_entries(int sz, int gib){
    size_t totalSize = static_cast<size_t>(gib) * GB_TO_B;
    return totalSize / sz;
}

size_t create(int size, const std::string& name, int gib, int maxEntries = 0) {
  assert(size > sizeof(high_resolution_clock::time_point));
  size_t totalSize = static_cast<size_t>(gib) * GB_TO_B;

  assert(totalSize % size == 0);
  auto entries = totalSize / size;
  if (maxEntries > 0) {
    entries = maxEntries;
    assert(entries * size <= totalSize);
  }

  fral::FRAL(name.c_str(), totalSize, entries);
  return entries;
}

#endif  // FRAL_TESTING_PERFORMANCE_TESTS_UTILITY_H
