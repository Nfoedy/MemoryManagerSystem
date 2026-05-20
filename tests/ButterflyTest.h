#pragma once

#include "MemoryManager/MemoryManager.h"

#include <iostream>


// Testa deallocazioni alternate: primo, ultimo, secondo, penultimo...
inline void RunButterflyTest()
{
    std::cout << "\n===== BUTTERFLY TEST =====" << std::endl;

    constexpr int COUNT = 8;
    constexpr std::size_t ALLOCATION_SIZE = 32;

    void* ptrs[COUNT] = {};

    // Alloca più blocchi della stessa dimensione
    for(int i = 0; i < COUNT; ++i)
    {
        ptrs[i] = MM_MALLOC(ALLOCATION_SIZE);
    }

    int left = 0;
    int right = COUNT - 1;

    // Libera alternando dall'inizio e dalla fine verso il centro
    while(left <= right)
    {
        MM_FREE(ptrs[left]);

        if(left != right)
        {
            MM_FREE(ptrs[right]);
        }

        ++left;
        --right;
    }

    std::cout << "[ButterflyTest] Completed alternating deallocation pattern." << std::endl;
}