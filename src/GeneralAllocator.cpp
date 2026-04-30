#include "MemoryManager/GeneralAllocator.h"

#include <cstdlib>

void* GeneralAllocator::Allocate(size_t size)
{
    return std::malloc(size);
}


void GeneralAllocator::Free(void* ptr)
{
    std::free(ptr);
}

