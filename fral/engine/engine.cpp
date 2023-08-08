#include "engine.h"

#include <fstream>
#include <iostream>

#include <boost/interprocess/file_mapping.hpp>

#define EMPTY_IDX 0;

namespace fral {

FRAL::FRAL(const char *fileName, size_t maxMemory, size_t maxEntries)
    : fileName(fileName) {
  auto admin = maxEntries * sizeof(size_t) + sizeof(Map);
  createFile(maxMemory + admin + maxEntries * sizeof(size_t));
  createMMRegion();

  map->heapStart = admin;
  map->maxEntries = maxEntries;
  map->maxMemory = maxMemory;
  map->clientCount.store(1);
  map->memorySize = mappedRegion->get_size();
  map->heapNext.store(map->heapStart);
  map->indexNext.store(0);

  for (size_t i = 0; i < maxEntries; i++) {
    map->records[i] = EMPTY_IDX;
  }
}

FRAL::FRAL(const char *fileName) : fileName(fileName) {
  createMMRegion();
  checkClient();
}

void FRAL::primeCache() {
  const size_t pageSize = mappedRegion->get_page_size();
  auto startAddress = (char *)map;
  auto endAddress = startAddress + mappedRegion->get_size();
  for (auto address = startAddress; address < endAddress; address += pageSize) {
    volatile char value = *address;
  }
}

void FRAL::createFile(long long size) {
  boost::interprocess::file_mapping::remove(fileName);
  std::filebuf buffer;
  buffer.open(
      fileName,
      std::ios_base::in | std::ios_base::out | std::ios_base::trunc |
          std::ios_base::binary);
  buffer.pubseekoff(size - 1, std::ios_base::beg);
  buffer.sputc(0);
}

void FRAL::createMMRegion() {
  boost::interprocess::file_mapping m_file(
      fileName,
      boost::interprocess::read_write);
  mappedRegion = new boost::interprocess::mapped_region(
      m_file,
      boost::interprocess::read_write);
  map = (Map *)mappedRegion->get_address();
}

void FRAL::checkClient() {
  // Check client on each connection - this is really checking for max
  // connections
  auto clientNo = map->clientCount.fetch_add(1);
  if (clientNo > MAX_CLIENTS) {
    std::cerr << "OVER MAX CLIENTS LIMIT - ERROR!" << std::endl;
    exit(1);
  }
}

void *FRAL::allocate(size_t sz) {
  // prevent wrap around
  if (sz > MAX_ALLOCATION || map->heapNext > map->memorySize) {
    return nullptr;
  }

  auto currentEntry = map->heapNext.fetch_add(sz + sizeof(size_t *));

  if (currentEntry + sz + sizeof(size_t *) > map->memorySize) {
    return nullptr;
  }

  char *currentAddress = ((char *)map) + currentEntry;
  *(size_t *)currentAddress = sz;

  return currentAddress + sizeof(size_t *);
}

int FRAL::append(void *blob) {
  auto offset = (size_t)((char *)blob - (char *)map);
  size_t empty_idx = EMPTY_IDX;

  for (auto index = map->indexNext.load(); index < map->maxEntries; index++) {
    if (map->records[index].compare_exchange_weak(empty_idx, offset)) {
      map->indexNext.store(index + 1);
      return index;
    }
    empty_idx = EMPTY_IDX;  // EMPTY_IDX was updated
  }
  return -1;
}

void *FRAL::load(int idx) const {
  size_t empty_idx = EMPTY_IDX;
  if (map->records[idx].load() == empty_idx) {
    return nullptr;
  }
  return ((char *)map) + map->records[idx];
}

void *FRAL::operator[](int idx) const { return load(idx); }

size_t FRAL::size() const {
  size_t empty_idx = EMPTY_IDX;
  for (auto index = map->indexNext.load(); index < map->maxEntries; ++index) {
    if (map->records[index].load() == empty_idx) {
      return index;
    }
  }
  return map->maxEntries;
}

size_t FRAL::maxSize() const { return map->maxEntries; }

size_t FRAL::maxMemory() const { return map->maxMemory; }

size_t FRAL::getBlobSize(void *blob) {
  char *sizeAddress = (char *)blob - sizeof(size_t *);
  return *(size_t *)sizeAddress;
}

}  // namespace fral