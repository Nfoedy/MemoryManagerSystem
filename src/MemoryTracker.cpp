#include "MemoryManager/MemoryTracker.h"

#include <iostream>
#include <cstring>
#include <cstddef>


namespace MM

{

    // Registra una nuova allocazione
    void MemoryTracker::Register(void* ptr, std::size_t size, const char* file, int line)
    {    
        // Ignora allocazioni interne (global override / STL / iostream)
        if(file != nullptr && std::strcmp(file, "global_new") == 0)
        {
            return;
        }

        m_Allocations[ptr] = { size, file, line };   // Inserisce o aggiorna nella mappa il puntatore con le relative info
    }


    // Rimuove una allocazione dalla mappa
    bool MemoryTracker::Unregister(void* ptr, std::size_t& outSize)
    {
        auto it = m_Allocations.find(ptr);      // cerca il puntatore nella mappa

        if(it == m_Allocations.end()) return false;   // se non esiste restituisce false

        outSize = it->second.size;     // Salva la dimensione dell'allocazione (serve al MM)

        m_Allocations.erase(it);     // rimuove il puntatore dalla mappa

        return true;    // restituisce true perchè la rimozione è avvenuta con successo
    }


    // Stampa le allocazioni ancora attive (memory leaks)
    void MemoryTracker::PrintLeaks() const
    {
        // Se la mappa è vuota, non ci sono leak
        if(m_Allocations.empty())
        {
            std::cout << "No memory leaks detected." << std::endl;
            return;
        }

        // Se ci sono elementi significa che ci sono leak
        std::cout << "Memory Leaks detected : " << std::endl;

        // Itera su tutte le allocazioni ancora attive
        // ptr --> indirizzo della memoria allocata
        // info --> informazioni dell'allocazione
        for (const auto& [ptr, info ] : m_Allocations)
        {
            std::cout << "-- Address: " << ptr << " | Size : " << info.size << " bytes | Location: " << info.file << " : " << info.line << std::endl;
        }

    }


    // Restituisce il numero di allocazioni ancora attive
    size_t MemoryTracker::GetActiveAllocations() const
    {
        return m_Allocations.size();     // Numero di elementi nella mappa
    }

}