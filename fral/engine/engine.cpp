#include "engine.h"

#include <boost/interprocess/file_mapping.hpp>
#include <fstream>
#include <iostream>

const size_t EMPTY_IDX = -1;

namespace fral {

FRAL::FRAL(const char* fileName, size_t size, size_t maxEntries)
    : maxEntries(maxEntries), fileName(fileName) {
  auto admin = maxEntries * sizeof(ssize_t) + sizeof(Map);
  createFile(size + admin);
  createMMRegion();

  map->heapStart.store(admin);
  map->heapNext.store(map->heapStart);
  map->indexNext.store(0);

  for (int i = 0; i < maxEntries; i++) {
    map->records[i].store(EMPTY_IDX);
  }
}

FRAL::FRAL(const char* fileName) : fileName(fileName) {
  createMMRegion();
  maxEntries = (map->heapStart - sizeof(Map)) / sizeof(ssize_t);
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

void* FRAL::allocate(size_t sz) {
  if (sz == 0) {
    return nullptr;
  }

  auto currentEntry = map->heapNext.fetch_add(sz);

  if (currentEntry + sz > mappedRegion->get_size()) {
    return nullptr;
  }

  auto currentAddress = ((char*)map) + currentEntry;

  return currentAddress;
}

int FRAL::append(void* blob) {
  auto offset = (ssize_t)((char*)blob - (char*)map);
  auto empty_idx = EMPTY_IDX;

  for (auto index = map->indexNext.load(); index < maxEntries; index++) {
    if (map->records[index].compare_exchange_weak(empty_idx, offset)) {
      map->indexNext.store(index + 1);

      return index;
    }

    empty_idx = EMPTY_IDX;  // EMPTY_IDX was updated
  }
  return -1;
}

void* FRAL::load(int idx) {
  if (idx < 0 || idx >= maxEntries || map->records[idx] == EMPTY_IDX) {
    return nullptr;
  }
  return ((char*)map) + map->records[idx];
}

int FRAL::size() {
  for (auto index = map->indexNext.load(); index < maxEntries; ++index) {
    if (map->records[index].load() == EMPTY_IDX) {
      return index + 1;
    }
  }
  return maxEntries;
}

}  // namespace fral