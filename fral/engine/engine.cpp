#include "engine.h"

#include <boost/interprocess/file_mapping.hpp>
#include <fstream>
#include <iostream>

const size_t EMPTY_IDX = 0;

namespace fral {

FRAL::FRAL(const char* fileName, size_t maxMemory, int maxEntries)
    : fileName(fileName) {
  auto admin = maxEntries * sizeof(size_t) + sizeof(Map);
  createFile(maxMemory + admin + maxEntries*sizeof(size_t));
  createMMRegion();

  map->heapStart = admin;
  map->maxEntries = maxEntries;
  map->maxMemory = maxMemory;
  map->heapNext.store(map->heapStart);
  map->heapTotal.store(0);
  map->indexNext.store(0);

  for (size_t i = 0; i < maxEntries; i++) {
    map->records[i].store(EMPTY_IDX);
  }
}

FRAL::FRAL(const char* fileName) : fileName(fileName) {
  createMMRegion();
}

void FRAL::primeCache() {
  const size_t pageSize = mappedRegion->get_page_size();
  auto startAddress = (char*)map;
  auto endAddress = startAddress + mappedRegion->get_size();
  for (auto address = startAddress; address < endAddress; address += pageSize) {
    volatile char value = *address;
  }
}

void FRAL::createFile(long long size) {
  boost::interprocess::file_mapping::remove(fileName);
  std::filebuf buffer;
  buffer.open(fileName, std::ios_base::in | std::ios_base::out |
                            std::ios_base::trunc | std::ios_base::binary);
  buffer.pubseekoff(size - 1, std::ios_base::beg);
  buffer.sputc(0);
}

void FRAL::createMMRegion() {
  boost::interprocess::file_mapping m_file(fileName,
                                           boost::interprocess::read_write);
  mappedRegion = new boost::interprocess::mapped_region(
      m_file, boost::interprocess::read_write);
  map = (Map*)mappedRegion->get_address();
}

void *FRAL::allocate(size_t sz) {

    if (sz == 0) {
        return nullptr;
    }

    auto currentEntry = map->heapNext.fetch_add(sz + sizeof(size_t *));

    if (currentEntry + sz + sizeof(size_t *) > mappedRegion->get_size()) {
        return nullptr;
    }

    map->heapTotal.fetch_add(sz);

    char *currentAddress = ((char *)map) + currentEntry;
    *(size_t *)currentAddress = sz;

    return currentAddress + sizeof(size_t *);
}


int FRAL::append(void* blob) {
  auto offset = (size_t)((char*)blob - (char*)map);
  auto empty_idx = EMPTY_IDX;

  for (auto index = map->indexNext.load(); index < map->maxEntries; index++) {
    if (map->records[index].compare_exchange_weak(empty_idx, offset)) {
      map->indexNext.store(index + 1);

      return index;
    }

    empty_idx = EMPTY_IDX;  // EMPTY_IDX was updated
  }
  return -1;
}

void* FRAL::load(int idx) const {
  if (idx >= map->maxEntries || map->records[idx] == EMPTY_IDX) {
    return nullptr;
  }
  return ((char*)map) + map->records[idx];
}

void* FRAL::operator[](int idx) const {
    return load(idx);
}

int FRAL::size() const {
  for (auto index = map->indexNext.load(); index < map->maxEntries; ++index) {
    if (map->records[index].load() == EMPTY_IDX) {
      return index;
    }
  }
  return map->maxEntries;
}

int FRAL::maxSize() const {
    return map->maxEntries;
}

size_t FRAL::memory() const {
    return map->heapTotal;
}

size_t FRAL::maxMemory() const {
    return map->maxMemory;
}


size_t FRAL::getBlobSize(void *blob) {
    char *sizeAddress = (char *)blob - sizeof(size_t *);
    return *(size_t *)sizeAddress;
}


}  // namespace fral