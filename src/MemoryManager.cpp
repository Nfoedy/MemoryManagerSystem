#include "MemoryManager/MemoryManager.h"
#include "MemoryManager/SmallObjectAllocator.h"
#include "MemoryManager/GeneralAllocator.h"
#include "MemoryManager/MemoryTracker.h"

#include <iostream> // std::cout per log e debug
#include <cstddef>  // std::size_t
#include <cstring>  // std::strcmp


namespace MM
{
    namespace
    {
        // Parametri per lo SmallObjectAllocator 
        static constexpr std::size_t SMALL_ALLOCATION_THRESHOLD = 64;       // Dimensione massima gestita dal SOBA
        static constexpr std::size_t SMALL_ALLOCATION_CHUNK_SIZE = 4096;    // Dimensione indicativa di ogni Chunk
        static constexpr std::size_t SMALL_ALLOCATION_ALIGNMENT = 8;        // Step delle size class: 8, 16, 24...

        // Statistiche globali del MemomryManager
        static std::size_t g_TotalAllocated = 0;        // Variabile globale per tenere traccia della memoria totale allocata
        static std::size_t g_CurrentAllocated = 0;      // Variabile globale per tenere traccia della memoria attualmente allocata
        static std::size_t g_AllocationCount = 0;       // Variabile globale per tenere traccia del numero di allocazioni
        static std::size_t g_FreeCount = 0;             // Variabile globale per tenere traccia del numero di deallocazioni

        static bool g_IsInitialized = false;        // Indica se il MemoryManager è pronto per i global new/delete overrides
    
        SmallObjectAllocator g_SmallAllocator(SMALL_ALLOCATION_THRESHOLD, SMALL_ALLOCATION_CHUNK_SIZE, SMALL_ALLOCATION_ALIGNMENT); // SmallObjectAllocator per gestire allocazioni di piccoli oggetti 
        GeneralAllocator g_GeneralAllocator;   // Variabile per gestire tutte le allocazioni che non vengono gestite dallo SmallObjAllocator
        MemoryTracker g_MemoryTracker;    // Oggetto resposabile del tracking delle allocazioni attive

        // Restituisce il nome dell'allocatore usato
        const char* GetAllocatorName(bool isSmallAllocation)
        {
            return isSmallAllocation ? "SOBA" : "General";
        }
    }


    void Initialize()
    {
        // Abilita il MemoryManager per l'uso dei global overrides
        g_IsInitialized = true;
    }

    void Shutdown()
    {   
        // Disabilita il MemoryManager prima della chiusura del programma
        g_IsInitialized = false;
    }

    bool IsInitialized()
    {
        // Ritorna true se il MemoryManager è pronto
        return g_IsInitialized;
    }



    void* Malloc(std::size_t size, const char* file, int line)
    {

        // Controllo di sicurezza: allocare 0 byte è sospetto, quindi va segnalato
        if(size == 0)
        {
            std::cout << "[MM][Warning] Requested allocation of 0 bytes at " << file << " : " << line << std::endl;
            size = 1;
        }

        // Le allocazioni "global_new" vengono tracciate ma non stampate nei log normali
        const bool isGlobalNew = (file != nullptr && std::strcmp(file, "global_new") == 0);
        const bool shouldLog = !isGlobalNew;

        // Decide se usare il SmallObjectAllocator oppure il GeneralAllocator
        const bool isSmallAllocation = size <= SMALL_ALLOCATION_THRESHOLD;      

        void* ptr = nullptr;    // Puntatore che conterrà l'indirizzo di memoria allocata     

        // SOBA o GeneralAllocator
        if(isSmallAllocation)
        {
            ptr = g_SmallAllocator.Allocate(size);   // Se la dimensione è inferiore o uguale utilizza il SmallObjectAllocator 
        }
        else
        {
            ptr = g_GeneralAllocator.Allocate(size); // Se la memoria è maggiore utilizza il GeneralAllocator
        }

        // Stampa un messaggio di errore se l'allocazione fallisce 
        if(ptr == nullptr)
        {
            std::cout << "[MM][Error] Allocation failed for " << size << " bytes at " << file << ":" << line << std::endl; 
            return nullptr;
        }

        
        g_TotalAllocated += size;           // Aggiorna la memoria totale allocata
        g_CurrentAllocated += size;         // Aggiorna la memoria attualmente allocata
        g_AllocationCount++;                // Incrementa il contatore delle allocazioni

        g_MemoryTracker.Register(ptr,size,file,line);   // Registra l'allocazione nel MemoryTracker

        if(shouldLog)
        {
            // Log debug con nome dell'allocatore usato, dimensione, indirizzo e posizione nel codice
            std::cout << "[MM][" << GetAllocatorName(isSmallAllocation) << "] Allocated " << size << " bytes | Address : " << ptr << " | Location : "
            << file << " : " << line << std::endl; 
        }

        return ptr; // Restituisce il puntatore alla memoria allocata
    }



    void Free(void* ptr)
    {
        // free(nullptr) è valido : non fa nulla
        if(ptr == nullptr)
        {
            return;
        }

        AllocationInfo info{};

        // Recupera le info dell'allocazione e la rimuove dal tracker
        if(!g_MemoryTracker.Unregister(ptr, info))
        {
            return;
        }

        const std::size_t size = info.size;

        // Se l'allocazione arriva dal Global Override, la tracciamo ma non stampiamo log numerosi
        const bool isGlobalNew = (info.file != nullptr && std::strcmp(info.file, "global_new") == 0);
        const bool shouldLog = !isGlobalNew;
        

        const bool isSmallAllocation = size <= SMALL_ALLOCATION_THRESHOLD;  

        g_CurrentAllocated -= size;         // Aggiorna la memoria attualmente allocata
        g_FreeCount++;                      // Incrementa il contatore delle deallocazioni
        
        // SOBA o GeneralAllocator
        if(isSmallAllocation)
        {
            g_SmallAllocator.Deallocate(ptr,size); // Se la dimensione è inferiore, utilizza il SmallObjectAllocator per liberare la memoria
        }
        else
        {
            g_GeneralAllocator.Free(ptr); // Altrimenti, utilizza il General Allocator per liberare la memoria
        }

        // Stampa il log solo per free esplicite, non per global_new
        if(shouldLog)
        {
            std::cout << "[MM][" << GetAllocatorName(isSmallAllocation) << "] Freed "
                    << size << " bytes | Address : " << ptr
                    << std::endl;
        }
        
    }


    void PrintStats()
    {
        std::cout << "\n========== MEMORY MANAGER STATS ==========\n";
        std::cout << "Total Allocated: " << g_TotalAllocated << " bytes" << std::endl;          // Stampa la memoria totale allocata
        std::cout << "Current Allocated: " << g_CurrentAllocated << " bytes" << std::endl;      // Stampa la memoria attualmente allocata
        std::cout << "Allocation Count: " << g_AllocationCount << std::endl;                    // Stampa il numero di allocazioni
        std::cout << "Free Count: " << g_FreeCount << std::endl;                                // Stampa il numero di deallocazioni
        std::cout << "Active Blocks: " << g_MemoryTracker.GetActiveAllocations() << std::endl;  // Stampa il numero di blocchi attivi (non ancora liberati)
        std::cout << "==========================================\n" << std::endl;
    }


    void DumpLeaks()
    {
        // Stampa tutte le allocazioni anocra attive nel MemoryTracker
        std::cout << "\n=========== MEMORY LEAK REPORT ===========\n";   
        g_MemoryTracker.PrintLeaks();
        std::cout << "==========================================\n" << std::endl; 
    }

}
