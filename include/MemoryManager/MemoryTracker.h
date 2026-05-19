#pragma once

#include <unordered_map>    // std::unordered_map per associare ogni puntatore alle sue informazioni
#include <cstddef>          // std::size_t


namespace MM
{
    // Struttura che contiene le info relative ad una signola allocazione
    struct AllocationInfo
    {
        std::size_t size;       // Dimensione dell'allocazione in byte
        const char* file;       // Nome del file in cui è stata effettuata l'allocazione
        int line;               // Numero di linea in cui è stata effettuata l'allocazione
    };


    // Classe responsabile del tracking delle allocazioni ancora attive
    class MemoryTracker
    {
    public:

        void Register(void* ptr, std::size_t size, const char* file, int line);   // Registra una nuova allocazione nella mappa

        // Rimuove un'allocazione dalla mappa
        // Ritorna true se trovata, false se il puntatore non esiste e ritorna tutte le info dell'allocazione rimossa
        bool Unregister(void* ptr, AllocationInfo& outInfo);  

        void PrintLeaks() const;   // Stampa tutte le allocazioni ancora attive, cioè i possibili memory leak

        std::size_t GetActiveAllocations() const;   // Restituisce il numero di allocazioni attive

    private:

        std::unordered_map<void*, AllocationInfo> m_Allocations;    // Mappa ogni puntatore allocato alle sue informazioni di debug
    };
}
