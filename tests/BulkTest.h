#pragma once

#include "MemoryManager/MemoryManager.h"

#include <iostream>


// Testa più allocazioni consecutive seguite da più deallocazioni consecutive
inline void RunBulkTest()
{
    std::cout << "\n===== BULK TEST =====" << std::endl;

    constexpr int COUNT = 8;

    void* ptrs[COUNT] = {};

    std::size_t sizes[COUNT] =
    {
        8, 12, 24, 32, 48, 64, 96, 128
    };

    // Alloca tutti i blocchi in sequenza
    for(int i = 0; i < COUNT; ++i)
    {
        ptrs[i] = MM_MALLOC(sizes[i]);
    }

    // Libera tutti i blocchi nello stesso ordine
    for(int i = 0; i < COUNT; ++i)
    {
        MM_FREE(ptrs[i]);
    }

    std::cout << "[BulkTest] Completed bulk allocation/deallocation pattern." << std::endl;
}