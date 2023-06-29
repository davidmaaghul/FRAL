#ifndef FRAL_ENGINE_H
#define FRAL_ENGINE_H

#include <atomic>
#include <boost/interprocess/mapped_region.hpp>

struct Map {
  std::atomic<size_t> heapStart;
  std::atomic<size_t> heapNext;
  std::atomic<size_t> indexNext;
  std::atomic<size_t> records[0];
};

namespace fral {

class FRAL {
 public:
  FRAL(const char* fileName, size_t size, size_t maxEntries);

  explicit FRAL(const char* fileName);

  void* allocate(size_t sz);

  int append(void* blob);

  void* load(int idx);

  static size_t getBlobSize(void* blob);

  int size();

  size_t getMaxEntries() const;

  void primeCache();

 private:
  boost::interprocess::mapped_region* mappedRegion{};

  size_t maxEntries;

  Map* map{};

  const char* fileName;

  void createFile(long long size);

  void createMMRegion();
};
}  // namespace fral

#endif  // FRAL_ENGINE_H
