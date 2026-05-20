#pragma once

#include "MemoryManager/MemoryManager.h"

#include <iostream>


// Testa più allocazioni della stessa dimensione per stressare una singola size class
inline void RunSameTest()
{
    std::cout << "\n===== SAME SIZE TEST =====" << std::endl;

    constexpr int COUNT = 8;
    constexpr std::size_t ALLOCATION_SIZE = 32;

    void* ptrs[COUNT] = {};

    // Alloca più blocchi tutti della stessa dimensione
    for(int i = 0; i < COUNT; ++i)
    {
        ptrs[i] = MM_MALLOC(ALLOCATION_SIZE);
    }

    // Libera tutti i blocchi allocati
    for(int i = 0; i < COUNT; ++i)
    {
        MM_FREE(ptrs[i]);
    }

    std::cout << "[SameTest] Completed " << COUNT
              << " allocations of " << ALLOCATION_SIZE
              << " bytes." << std::endl;
}