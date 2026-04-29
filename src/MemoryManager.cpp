#include "MemoryManager/MemoryManager.h"

#include <cstdlib>  // per std::malloc e std::free
#include <iostream> // per debug
#include <unordered_map> // per tenere traccia delle allocazioni


namespace MM
{
    struct AllocationInfo
    {
        size_t size; // Dimensione dell'allocazione
        const char* file; // Nome del file in cui è stata effettuata l'allocazione
        int line; // Numero di linea in cui è stata effettuata l'allocazione
    };


    static std::size_t g_TotalAllocated = 0; // Variabile globale per tenere traccia della memoria totale allocata
    static std::size_t g_CurrentAllocated = 0; // Variabile globale per tenere traccia della memoria attualmente allocata
    static std::size_t g_AllocationCount = 0; // Variabile globale per tenere traccia del numero di allocazioni
    static std::size_t g_FreeCount = 0; // Variabile globale per tenere traccia del numero di deallocazioni

    static std::unordered_map<void*, AllocationInfo> g_Allocations; // Mappa per tenere traccia delle allocazioni (puntatore -> informazioni sull'allocazione)


    void* Malloc(std::size_t size, const char* file, int line)
    {
        void* ptr = std::malloc(size);   // Alloca memoria usando malloc

        if(ptr == nullptr)
        {
            std::cout << "[MM] Allocation failed for " << size << " bytes at " << file << ":" << line << std::endl; // Stampa un messaggio di errore se l'allocazione fallisce
            return nullptr;
        }

        g_TotalAllocated += size; // Aggiorna la memoria totale allocata
        g_CurrentAllocated += size; // Aggiorna la memoria attualmente allocata
        g_AllocationCount++; // Incrementa il contatore delle allocazioni

        g_Allocations[ptr] = AllocationInfo
        {
            size, 
            file, 
            line
        }; // Memorizza le informazioni sull'allocazione nella mappa

        std::cout << "[MM] Allocated " << size << " bytes at address " << ptr << " (" << file << " : " << line << " )" << std::endl; // Stampa un messaggio di debug con i dettagli dell'allocazione

        return ptr; // Restituisce il puntatore alla memoria allocata
    }


    void Free(void* ptr)
    {
        if(ptr == nullptr)
        {
            return;
        }

        auto it = g_Allocations.find(ptr); // Cerca il puntatore nella mappa delle allocazioni

        if(it == g_Allocations.end())
        {
            std::cout << "[MM] Warning: try to free unallocated memory at address " << ptr << std::endl; // Stampa un messaggio di avviso se si tenta di liberare memoria non allocata
            return;
        }

        std::size_t size = it->second.size; // Ottiene la dimensione dell'allocazione dalla mappa

        g_CurrentAllocated -= size; // Aggiorna la memoria attualmente allocata
        g_FreeCount++; // Incrementa il contatore delle deallocazioni

        g_Allocations.erase(it); // Rimuove l'allocazione dalla mappa

        std::free(ptr); // Libera la memoria usando free
        
        std::cout << "[MM] Freeing memory at address " << ptr << std::endl; // Libera la memoria all'indirizzo ptr
    }


    void PrintStats()
    {
        std::cout << "[MM] Memory Manager Stats: " << std::endl;
        std::cout << "Total Allocated: " << g_TotalAllocated << " bytes" << std::endl; // Stampa la memoria totale allocata
        std::cout << "Current Allocated: " << g_CurrentAllocated << " bytes" << std::endl; // Stampa la memoria attualmente allocata
        std::cout << "Allocation Count: " << g_AllocationCount << std::endl; // Stampa il numero di allocazioni
        std::cout << "Free Count: " << g_FreeCount << std::endl; // Stampa il numero di deallocazioni
        std::cout << "Active Blocks: " << g_Allocations.size() << std::endl; // Stampa il numero di blocchi attivi (non ancora liberati)
    }


    void DumpLeaks()
    {
        std::cout << "[MM] Memory Leaks: " << std::endl;

        if(g_Allocations.empty())
        {
            std::cout << "No memory leaks detected." << std::endl; // Stampa un messaggio se non sono state rilevate perdite di memoria
        }
        else
        {
            std::cout << "Memory leaks detected: " << std::endl; 

            // Stampa i dettagli di ogni perdita di memoria rilevata
            for (const auto& allocation : g_Allocations)
            {
                const void* ptr = allocation.first; // Ottiene il puntatore alla memoria allocata
                const AllocationInfo& info = allocation.second; // Ottiene le informazioni sull'allocazione

                std::cout << " - Leaked " << ptr << " | Size " << info.size << " bytes " << " | Location " << info.file << ":" << info.line << std::endl; // Stampa i dettagli della perdita di memoria
            }
        }


        std::cout << "===============================" << std::endl; 

    }



}
