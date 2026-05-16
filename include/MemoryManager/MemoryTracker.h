#pragma once

#include <unordered_map>  // per tenere traccia delle allocazioni
#include <cstddef> 

namespace MM
{


    // Struttura che contiene le info relative ad una signola allocazione
    struct AllocationInfo
    {
        std::size_t size; // Dimensione dell'allocazione
        const char* file; // Nome del file in cui è stata effettuata l'allocazione
        int line; // Numero di linea in cui è stata effettuata l'allocazione
    };


    /* Classe responsabile del tracking delle allocazioni*/
    class MemoryTracker
    {
    public:

        void Register(void* ptr, std::size_t size, const char* file, int line);   // Registra una nuova allocazione nella mappa

        // Rimuove un allocazione dalla mappa
        // Ritorna true se trovata, false se il puntatore non esiste
        bool Unregister(void* ptr, std::size_t& outSize);  

        void PrintLeaks() const;   // Stampa le statistiche base sull'utilizzo della memoria

        std::size_t GetActiveAllocations() const;   // Restituisce il numero di allocazioni attive

    private:

        std::unordered_map<void*, AllocationInfo> m_Allocations; // Mappa : puntatore --> informazioni allocazione

    };

}
