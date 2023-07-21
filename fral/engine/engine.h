#ifndef FRAL_ENGINE_H
#define FRAL_ENGINE_H

#include <atomic>
#include <boost/interprocess/mapped_region.hpp>

struct Map {
  size_t heapStart;
  int maxEntries;
  size_t maxMemory;
  std::atomic<size_t> heapNext;
  std::atomic<size_t> heapTotal;
  std::atomic<int> indexNext;
  std::atomic<size_t> records[0];
};

namespace fral {

class FRAL {
 public:
  FRAL(const char* fileName, size_t maxMemory, int maxEntries);

  FRAL(const char* fileName);

  virtual ~FRAL() = default;

  virtual void* allocate(size_t sz);

  int append(void* blob);

  void* load(int idx) const;

  int size() const;

  int maxSize() const;

  size_t memory() const;

  size_t maxMemory() const;

  static size_t getBlobSize(void* blob);

  void primeCache();

  void* operator[](int idx) const;

 protected:
  boost::interprocess::mapped_region* mappedRegion{};

  Map* map{};

  const char* fileName;

  void createFile(long long size);

  void createMMRegion();
};
}  // namespace fral

#endif  // FRAL_ENGINE_H
