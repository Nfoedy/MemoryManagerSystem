#include "MemoryManager/MemoryManager.h"

#include <cstdlib>  // per std::malloc e std::free
#include <iostream> // per debug


namespace MM
{

    static std::size_t g_TotalAllocated = 0; // Variabile globale per tenere traccia della memoria totale allocata
    static std::size_t g_CurrentAllocated = 0; // Variabile globale per tenere traccia della memoria attualmente allocata
    static std::size_t g_AllocationCount = 0; // Variabile globale per tenere traccia del numero di allocazioni
    static std::size_t g_FreeCount = 0; // Variabile globale per tenere traccia del numero di deallocazioni


    void* Malloc(std::size_t size)
    {
        void* ptr = std::malloc(size);   // Alloca memoria usando malloc

        if(ptr == nullptr)
        {
            std::cout << "[MM] Allocation failed for " << size << " bytes at address " << ptr << std::endl; // Stampa un messaggio di errore se l'allocazione fallisce
            return nullptr;
        }

        g_TotalAllocated += size; // Aggiorna la memoria totale allocata
        g_CurrentAllocated += size; // Aggiorna la memoria attualmente allocata
        g_AllocationCount++; // Incrementa il contatore delle allocazioni

        std::cout << "[MM] Allocating " << size << " bytes at address " << ptr << std::endl; // Allocazione di size nell'indirizzo ptr

        return ptr; // Restituisce il puntatore alla memoria allocata
    }


    void Free(void* ptr)
    {
        if(ptr == nullptr)
        {
            return;
        }

        std::free(ptr); // Libera la memoria usando free
        g_FreeCount++; // Incrementa il contatore delle deallocazioni

        std::cout << "[MM] Freeing memory at address " << ptr << std::endl; // Libera la memoria all'indirizzo ptr
    }


    void PrintStats()
    {
        std::cout << "[MM] Memory Manager Stats: " << std::endl;
        std::cout << "Total Allocated: " << g_TotalAllocated << " bytes" << std::endl; // Stampa la memoria totale allocata
        std::cout << "Current Allocated: " << g_CurrentAllocated << " bytes" << std::endl; // Stampa la memoria attualmente allocata
        std::cout << "Allocation Count: " << g_AllocationCount << std::endl; // Stampa il numero di allocazioni
        std::cout << "Free Count: " << g_FreeCount << std::endl; // Stampa il numero di deallocazioni
    }



}
