#pragma once

#include <cstddef>

/*
    GeneralAllocator

    Allocatore generico usato per tutte le allocazioni che non rientrano nello SmallObjectAllocator.

    In questa prima versione usa internamente malloc/free, ma viene separato in una classe didicata
    per mantenere il MamoryManager pulito e facilmente estendibile in futuro

*/


class GeneralAllocator
{

public:
    void* Allocate(std::size_t size);
    void Free(void* ptr);

};
