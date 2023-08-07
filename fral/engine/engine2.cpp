#include "engine2.h"
#include <boost/interprocess/file_mapping.hpp>
#include <fstream>
#include <iostream>

const size_t EMPTY_IDX = 0;

namespace fral {

    FRAL2::FRAL2(const char* fileName, size_t maxMemory, int maxEntries)
            : fileName(fileName) {
        auto admin = maxEntries * sizeof(size_t) + sizeof(Map2);
        createFile(maxMemory + admin + maxEntries*sizeof(size_t));
        createMMRegion();

        map->heapStart = admin;
        map->maxEntries = maxEntries;
        map->maxMemory = maxMemory;
        map->memorySize = mappedRegion->get_size();
        map->heapNext.store(map->heapStart);
        map->heapTotal.store(0);
        map->indexNext.store(0);

        for (size_t i = 0; i < maxEntries; i++) {
            map->records[i] = EMPTY_IDX;
        }
    }

    FRAL2::FRAL2(const char* fileName) : fileName(fileName) {
        createMMRegion();
    }

    void FRAL2::primeCache() {
        const size_t pageSize = mappedRegion->get_page_size();
        auto startAddress = (char*)map;
        auto endAddress = startAddress + mappedRegion->get_size();
        for (auto address = startAddress; address < endAddress; address += pageSize) {
            volatile char value = *address;
        }
    }

    void FRAL2::createFile(long long size) {
        boost::interprocess::file_mapping::remove(fileName);
        std::filebuf buffer;
        buffer.open(fileName, std::ios_base::in | std::ios_base::out |
                              std::ios_base::trunc | std::ios_base::binary);
        buffer.pubseekoff(size - 1, std::ios_base::beg);
        buffer.sputc(0);
    }

    void FRAL2::createMMRegion() {
        boost::interprocess::file_mapping m_file(fileName, boost::interprocess::read_write);
        mappedRegion = new boost::interprocess::mapped_region(m_file, boost::interprocess::read_write);
        map = (Map2*) mappedRegion->get_address();
    }

    void *FRAL2::allocate(size_t sz) {

        auto newMem = sz + sizeof(size_t *);

        if (map->heapNext.load() + newMem > map->memorySize) {
            return nullptr;
        }

        auto currentEntry = map->heapNext.fetch_add(sz + sizeof(size_t *));

        if(currentEntry + newMem > map->memorySize){
            return nullptr;
        }

        map->heapTotal.fetch_add(sz);

        char *currentAddress = ((char *)map) + currentEntry;
        *(size_t *)currentAddress = sz;

        return currentAddress + sizeof(size_t *);
    }


    int FRAL2::append(void* blob) {

        if(map->indexNext.load() >= map->maxEntries){
            return -1;
        }

        auto currentIdx = map->indexNext.fetch_add(1);

        if(currentIdx >= map->maxEntries){
            return -1;
        }

        map->records[currentIdx] = (size_t)((char*)blob - (char*)map);

        return currentIdx;
    }

    void* FRAL2::load(int idx) const {
        if (map->records[idx] == EMPTY_IDX) {
            return nullptr;
        }
        return ((char*)map) + map->records[idx];
    }

    void* FRAL2::operator[](int idx) const {
        return load(idx);
    }

    int FRAL2::size() const {
        return map->indexNext;
    }

    int FRAL2::maxSize() const {
        return map->maxEntries;
    }

    size_t FRAL2::memory() const {
        return map->heapTotal;
    }

    size_t FRAL2::maxMemory() const {
        return map->maxMemory;
    }


    size_t FRAL2::getBlobSize(void *blob) {
        char *sizeAddress = (char *)blob - sizeof(size_t *);
        return *(size_t *)sizeAddress;
    }


}  // namespace fral