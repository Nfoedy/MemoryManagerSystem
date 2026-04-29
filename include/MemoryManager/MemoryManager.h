#pragma once

#include <cstddef>  // per std::size_t

// Namespace perchè cosi evito conflitti con altre funzioni Malloc
namespace MM
{
    void* Malloc(std::size_t size);    // Alloca un blocco di memoria di dim size e restituisce un puntatore ad esso
    void Free(void* ptr);       // Libera un blocco di memoria precedentemente allocato con Malloc

    void PrintStats();  // Stampa le statistiche base sull'utilizzo della memoria
    void DumpLeaks();  // Stampa  perdite di memoria

    // NEW
    template<typename T, typename... Args>
    T* New(Args&&... args)    // Alloca memoria per un oggetto di tipo T e lo costruisce con i parametri passati
    {
        // 1. Allocazione memoria
        void* mem = MM::Malloc(sizeof(T)); // Alloca memoria per un oggetto di tipo T

        if(!mem)
        {
            return nullptr; // Se l'allocazione fallisce, restituisce nullptr
        }

        // 2. Costruzione oggetto
        return new(mem) T(std::forward<Args>(args)...); // Costruisce l'oggetto di tipo T nella memoria allocata usando il costruttore con i parametri passati
    }


    // DELETE
    template<typename T>
    void Delete(T* ptr)      // Distrugge un oggetto di tipo T e libera la memoria associata
    {
        if(!ptr)
        {
            return; // Se il puntatore è nullptr, non fa nulla
        }

        // 1. Distruzione oggetto
        ptr->~T(); // Chiama il distruttore dell'oggetto di tipo T

        // 2. Liberazione memoria
        MM:Free(ptr); // Libera la memoria associata all'oggetto usando la funzione Free del MemoryManager
    }




}


