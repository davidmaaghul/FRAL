#ifndef FRAL_ENGINE2_H
#define FRAL_ENGINE2_H

//Todo: need to generalize this with FRAL1 (lots of repeated code here)
//assert atomic is address free for architecture

#include <atomic>
#include <boost/interprocess/mapped_region.hpp>

static_assert(std::atomic<int>::is_always_lock_free);
static_assert(std::atomic<size_t>::is_always_lock_free);

struct Map2 {
    size_t heapStart;
    size_t memorySize;
    int maxEntries;
    size_t maxMemory;
    std::atomic<size_t> heapNext;
    std::atomic<size_t> heapTotal;
    std::atomic<int> indexNext;
    size_t records[0];
};


namespace fral {

    class FRAL2 {
    public:
        FRAL2(const char* fileName, size_t maxMemory, int maxEntries);

        FRAL2(const char* fileName);

        virtual ~FRAL2() = default;

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

        Map2* map{};

        const char* fileName;

        void createFile(long long size);

        void createMMRegion();
    };
}  // namespace fral


#endif //FRAL_ENGINE2_H
