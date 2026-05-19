#pragma once

#include <cstddef>   // std::size_t

/*
    GeneralAllocator

    Allocatore generico usato per tutte le allocazioni che non rientrano nello SmallObjectAllocator.

    In questa prima versione usa internamente malloc/free, ma viene separato in una classe didicata
    per mantenere il MamoryManager pulito e facilmente estendibile in futuro

*/


class GeneralAllocator
{
public:
    // Alloca memoria raw usando l'allocatore generale
    void* Allocate(std::size_t size);

    // Libera memoria raw precendentemente allocata dal GeneralAllocator
    void Free(void* ptr);
};
