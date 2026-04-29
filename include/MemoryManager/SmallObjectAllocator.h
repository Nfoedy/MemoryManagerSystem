#pragma once

#include <cstddef>

class SmallObjectAllocator
{
public: 
    SmallObjectAllocator(size_t blockSize, size_t blockCount);
    ~SmallObjectAllocator();

    void* Allocate();
    void Free(void* ptr);

private:
    void* m_Memory = nullptr; // Puntatore alla memoria allocata
    void* m_FreeList = nullptr; // Puntatore alla lista libera

    size_t m_BlockSize; // Dimensione di ogni blocco
    size_t m_BlockCount; // Numero totale di blocchi
};




