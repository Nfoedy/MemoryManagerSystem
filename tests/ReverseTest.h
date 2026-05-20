#pragma once

#include "MemoryManager/MemoryManager.h"

#include <iostream>


// Testa deallocazioni in ordine inverso rispetto alle allocazioni
inline void RunReverseTest()
{
    std::cout << "\n===== REVERSE TEST =====" << std::endl;

    constexpr int COUNT = 8;

    void* ptrs[COUNT] = {};

    std::size_t sizes[COUNT] =
    {
        8, 12, 24, 32, 48, 64, 96, 128
    };

    // Alloca blocchi di dimensioni diverse
    for(int i = 0; i < COUNT; ++i)
    {
        ptrs[i] = MM_MALLOC(sizes[i]);
    }

    // Libera i blocchi in ordine inverso
    for(int i = COUNT - 1; i >= 0; --i)
    {
        MM_FREE(ptrs[i]);
    }

    std::cout << "[ReverseTest] Completed reverse deallocation pattern." << std::endl;
}