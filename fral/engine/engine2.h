#ifndef FRAL_ENGINE2_H
#define FRAL_ENGINE2_H

#include <atomic>
#include <boost/interprocess/mapped_region.hpp>

#include "engine.h"

namespace fral {

class FRAL2 : public FRAL {
 public:
  FRAL2(const char* fileName, size_t size, size_t maxEntries)
      : FRAL(fileName, size + sizeof(ssize_t) * maxEntries, maxEntries) {}

  explicit FRAL2(const char* fileName) : FRAL(fileName) {}

  void* allocate(size_t sz) override;

  static size_t getBlobSize(void* blob);
};
}  // namespace fral

#endif  // FRAL_ENGINE2_H
