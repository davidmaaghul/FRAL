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
    FRAL(const char *fileName, size_t size, size_t maxEntries);

    FRAL(const char* fileName);

    virtual void* allocate(size_t sz);

    int append(void* blob);

    void* load(int idx);

    int size();

    void primeCache();

protected:
  boost::interprocess::mapped_region* mappedRegion{};

  size_t maxEntries;

  Map* map{};

  const char* fileName;

  void createFile(long long size);

  void createMMRegion();
};
}  // namespace fral

#endif  // FRAL_ENGINE_H
