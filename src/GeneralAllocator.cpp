#include "MemoryManager/GeneralAllocator.h"

#include <cstdlib>

void* GeneralAllocator::Allocate(std::size_t size)
{
    return std::malloc(size);
}


void GeneralAllocator::Free(void* ptr)
{
    std::free(ptr);
}

