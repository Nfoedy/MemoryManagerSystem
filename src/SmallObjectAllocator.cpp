#include "MemoryManager/SmallObjectAllocator.h"

#include <cstdlib> // per std::malloc e std::free


/* 
    Il SmallObjectAllocator è un allocatore di memoria progettato per gestire in modo efficiente piccoli oggetti.
    
    Funziona pre-allocando un blocco di memoria suddiviso in elemente e dimensione fissa (blockSize) e mantenendo una "FreeList" dei blocchi disponibili. 
    In questo modo evita chiamate ripetute a malloc/free per ogni oggetto piccolo, riducendo frammentazione e migliorando le performance.

    Utilizzo in un FPS:
    Viene usato per oggetti come proiettili, particelle, effetti visivi o componenti leggeti.
    Invece di allocare memoria ogni volta, il sistema riutilizza blocchi già esistenti,
    garantendo allocazioni veloci e frame rate stabili. 

*/


SmallObjectAllocator::SmallObjectAllocator(size_t blockSize, size_t blockCount)
    : m_BlockSize(blockSize), m_BlockCount(blockCount)
{
    m_Memory = std::malloc(blockSize * blockCount); // Alloca un blocco di memoria per tutti i blocchi

    // Costuizione Free List
    char* current = (char*)m_Memory; // Puntatore corrente alla memoria allocata

    for (size_t i = 0; i < blockCount - 1; ++i)
    {
        char* next = current + blockSize; // Calcola l'indirizzo del blocco successivo
        *(void**)current = next; // Imposta il puntatore al blocco successivo nella lista libera
        current = next; // Sposta il puntatore corrente al blocco successivo
    }

    *(void**)current = nullptr; // L'ultimo blocco punta a nullptr

    m_FreeList = m_Memory; // Inizializza la lista libera con il primo blocco
}


void* SmallObjectAllocator::Allocate()
{
    if(!m_FreeList)
    {
        return nullptr; // Se la lista libera è vuota, restituisce nullptr
    }

    void* block = m_FreeList; // Prende il primo blocco dalla lista libera
    m_FreeList = *(void**)m_FreeList; // Aggiorna la lista libera al blocco successivo
    
    return block; // Restituisce il blocco allocato
}


void SmallObjectAllocator::Free(void* ptr)
{
    *(void**)ptr = m_FreeList; // Inserisce il blocco restituito all'inizio della lista libera
    m_FreeList = ptr; // Aggiorna la lista libera al blocco restituito
}

// Destructor 
SmallObjectAllocator::~SmallObjectAllocator()
{
    std::free(m_Memory); // Libera la memoria allocata
}

