#include "engine2.h"

namespace fral {

size_t FRAL2::getBlobSize(void *blob) {
  char *sizeAddress = (char *)blob - sizeof(size_t *);
  return *(size_t *)sizeAddress;
}

void *FRAL2::allocate(ssize_t sz) {
  if (sz == 0) {
    return nullptr;
  }

  auto currentEntry = map->heapNext.fetch_add(sz + sizeof(size_t *));

  if (currentEntry + sz > mappedRegion->get_size()) {
    return nullptr;
  }

  char *currentAddress = ((char *)map) + currentEntry;
  *(size_t *)currentAddress = sz;

  return currentAddress + sizeof(size_t *);
}

}  // namespace fral