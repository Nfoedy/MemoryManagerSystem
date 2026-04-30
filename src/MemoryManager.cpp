#include "MemoryManager/MemoryManager.h"
#include "MemoryManager/SmallObjectAllocator.h"
#include "MemoryManager/GeneralAllocator.h"

#include <iostream> // per debug
#include <unordered_map> // per tenere traccia delle allocazioni


namespace MM
{
    namespace
    {
        // Struttura che contiene le info relative ad una signola allocazione
        struct AllocationInfo
        {
            size_t size; // Dimensione dell'allocazione
            const char* file; // Nome del file in cui è stata effettuata l'allocazione
            int line; // Numero di linea in cui è stata effettuata l'allocazione
        };

        // Parametri per lo SmallObjectAllocator 
        static constexpr std::size_t SMALL_ALLOCATION_THRESHOLD = 64;      // Dimensione massima per usare il pool
        static constexpr std::size_t SMALL_ALLOCATION_BLOCK_COUNT = 1000;    // Numero di blocchi del pool

        // Statistiche globali del MemomryManager
        static std::size_t g_TotalAllocated = 0; // Variabile globale per tenere traccia della memoria totale allocata
        static std::size_t g_CurrentAllocated = 0; // Variabile globale per tenere traccia della memoria attualmente allocata
        static std::size_t g_AllocationCount = 0; // Variabile globale per tenere traccia del numero di allocazioni
        static std::size_t g_FreeCount = 0; // Variabile globale per tenere traccia del numero di deallocazioni

        static std::unordered_map<void*, AllocationInfo> g_Allocations; // Mappa per tenere traccia delle allocazioni (puntatore -> informazioni sull'allocazione)
    
        SmallObjectAllocator g_SmallAllocator(SMALL_ALLOCATION_THRESHOLD, SMALL_ALLOCATION_BLOCK_COUNT); // SmallObjectAllocator per gestire allocazioni di piccoli oggetti (blocchi di 64 byte, 1000 blocchi totali)
        GeneralAllocator g_GeneralAllocator;   // Variabile per gestire tutte le allocazioni che non vengono gestite dallo SmallObjAllocator

        // Restituisce il nome dell'allocatore usato
        const char* GetAllocatorName(bool isSmallAllocation)
        {
            return isSmallAllocation ? "Pool" : "General";
        }
    }
    

    void* Malloc(std::size_t size, const char* file, int line)
    {

        // Controllo di sicurezza: allocare 0 byte è sospetto, quindi va segnalato
        if(size == 0)
        {
            std::cout << "[MM][Warning] Requested allocation of 0 bytes at " << file << " : " << line << std::endl;
        }

        const bool isSmallAllocation = size <= SMALL_ALLOCATION_THRESHOLD;      // Controllo in base alla dimensione richiesta

        void* ptr = nullptr;    // Puntatore che conterrà l'indirizzo di memoria allocata     

        // SmallObjectAllocator o GeneralAllocator
        if(isSmallAllocation)
        {
            ptr = g_SmallAllocator.Allocate(); // Se la dimensione è inferiore o uguale a THRESHOLD, utilizza il SmallObjectAllocator 
        }
        else
        {
            ptr = g_GeneralAllocator.Allocate(size); // Se la memoria è > THRESHOLD utilizziamo il GeneralAllocator
        }

        // Stampa un messaggio di errore se l'allocazione fallisce 
        if(ptr == nullptr)
        {
            std::cout << "[MM][Error] Allocation failed for " << size << " bytes at " << file << ":" << line << std::endl; 
            return nullptr;
        }

        g_TotalAllocated += size; // Aggiorna la memoria totale allocata
        g_CurrentAllocated += size; // Aggiorna la memoria attualmente allocata
        g_AllocationCount++; // Incrementa il contatore delle allocazioni

        // Memorizza le informazioni dell'allocazione nella mappa
        g_Allocations[ptr] = AllocationInfo
        {
            size, 
            file, 
            line
        }; 

        // Log debug con nome dell'allocatore usato, dimensione, indirizzo e posizione nel codice
        std::cout << "[MM][" << GetAllocatorName(isSmallAllocation) << "] Allocated " << size << " bytes | Address : " << ptr << " | Location : "
        << file << " : " << line << std::endl; 

        return ptr; // Restituisce il puntatore alla memoria allocata
    }



    void Free(void* ptr)
    {
        // free(nullptr) è valido : non fa nulla
        if(ptr == nullptr)
        {
            return;
        }

        auto it = g_Allocations.find(ptr); // Cerca il puntatore nella mappa delle allocazioni

        // Stampa un messaggio di avviso se si tenta di liberare memoria non allocata
        if(it == g_Allocations.end())
        {
            std::cout << "[MM][Warning] Attemped to free unknown pointer | Address: " << ptr << std::endl; 
            return;
        }

        std::size_t size = it->second.size; // Ottiene la dimensione dell'allocazione dalla mappa

        const bool isSmallAllocation = size <= SMALL_ALLOCATION_THRESHOLD;  // Determina quale allocatore deve gestire la deallocazione

        g_CurrentAllocated -= size; // Aggiorna la memoria attualmente allocata
        g_FreeCount++; // Incrementa il contatore delle deallocazioni

        g_Allocations.erase(it); // Rimuove l'allocazione dalla mappa
        
        // SmallObjectAllocator o GeneralAllocator
        if(isSmallAllocation)
        {
            g_SmallAllocator.Free(ptr); // Se la dimensione è inferiore, utilizza il SmallObjectAllocator per liberare la memoria
        }
        else
        {
            g_GeneralAllocator.Free(ptr); // Altrimenti, utilizza il General Allocator per liberare la memoria
        }

        // Log debug della deallocazione
        std::cout << "[MM][" << GetAllocatorName(isSmallAllocation) << "] | Freed " << size << " bytes | Address : " << ptr << std::endl;
    }


    void PrintStats()
    {
        std::cout << "\n========= MEMORY MANAGER STATS =========\n";
        std::cout << "Total Allocated: " << g_TotalAllocated << " bytes" << std::endl; // Stampa la memoria totale allocata
        std::cout << "Current Allocated: " << g_CurrentAllocated << " bytes" << std::endl; // Stampa la memoria attualmente allocata
        std::cout << "Allocation Count: " << g_AllocationCount << std::endl; // Stampa il numero di allocazioni
        std::cout << "Free Count: " << g_FreeCount << std::endl; // Stampa il numero di deallocazioni
        std::cout << "Active Blocks: " << g_Allocations.size() << std::endl; // Stampa il numero di blocchi attivi (non ancora liberati)
        std::cout << "==========================================\n" << std::endl;
    }


    void DumpLeaks()
    {
        std::cout << "\n========= MEMORY LEAK REPORT =========\n";

        if(g_Allocations.empty())
        {
            std::cout << "No memory leaks detected." << std::endl; // Stampa un messaggio se non sono state rilevate perdite di memoria
        }
        else
        {
            std::cout << "Memory leaks detected: " << std::endl; 

            // Ogni elemento della mappa contiene
            // ptr --> indirizzo della memoria allocata
            // info --> informazioni dell'allocazione
            for (const auto& [ptr, info ] : g_Allocations)
            {
                std::cout << "-- Address: " << ptr << " | Size : " << info.size << " bytes | Location: " << info.file << " : " << info.line << std::endl;
            }
        }

        std::cout << "===============================" << std::endl; 

    }



}
