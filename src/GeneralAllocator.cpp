#include "MemoryManager/GeneralAllocator.h"

#include <cstdlib>      // std::malloc, std::free

void* GeneralAllocator::Allocate(std::size_t size)
{
    // Alloca memoria raw usando malloc come fallback generale
    return std::malloc(size);
}


void GeneralAllocator::Free(void* ptr)
{
    // Libera memoria raw precedentemente allocata con malloc
    std::free(ptr);
}

