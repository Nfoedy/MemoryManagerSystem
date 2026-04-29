#pragma once

#include <cstddef>  // per std::size_t

// Namespace perchè cosi evito conflitti con altre funzioni Malloc
namespace MM
{
    void* Malloc(std::size_t size);    // Alloca un blocco di memoria di dim size e restituisce un puntatore ad esso
    void Free(void* ptr);       // Libera un blocco di memoria precedentemente allocato con Malloc

    void PrintStats();  // Stampa le statistiche base sull'utilizzo della memoria
}


