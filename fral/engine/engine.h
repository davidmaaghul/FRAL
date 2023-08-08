#ifndef FRAL_ENGINE_H
#define FRAL_ENGINE_H

#include <atomic>

#include <boost/interprocess/mapped_region.hpp>

#define MAX_MEMORY (1ul << 60)
#define MAX_ALLOCATION (1ul << 40)
#define MAX_CLIENTS (1ul << 20)

// assert atomic is address free for architecture

static_assert(std::atomic<size_t>::is_always_lock_free);
static_assert(std::numeric_limits<size_t>::max() > MAX_MEMORY);

struct Map {
  size_t heapStart;
  size_t memorySize;
  std::atomic<size_t> clientCount;
  size_t maxEntries;
  size_t maxMemory;
  std::atomic<size_t> heapNext;
  std::atomic<size_t> indexNext;
  std::atomic<size_t> records[0];
};

namespace fral {

class FRAL {
public:
  FRAL(const char* fileName, size_t maxMemory, size_t maxEntries);

  FRAL(const char* fileName);

  virtual ~FRAL() = default;

  virtual void* allocate(size_t sz);

  int append(void* blob);

  void* load(int idx) const;

  size_t size() const;

  size_t maxSize() const;

  size_t maxMemory() const;

  static size_t getBlobSize(void* blob);

  void primeCache();

  void* operator[](int idx) const;

protected:
  boost::interprocess::mapped_region* mappedRegion{};

  Map* map{};

  const char* fileName;

  void checkClient();

  void createFile(long long size);

  void createMMRegion();
};
}  // namespace fral

#endif  // FRAL_ENGINE_H
