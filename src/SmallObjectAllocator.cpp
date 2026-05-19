#include "MemoryManager/SmallObjectAllocator.h"

#include <limits>   // std::numeric_limits per ottenere il massimo valore di unsigned char


namespace MM
{
    SmallObjectAllocator::SmallObjectAllocator(
        std::size_t maxObjectSize,
        std::size_t chunkSize,
        std::size_t alignment
    )
    {
        Init(maxObjectSize, chunkSize, alignment);
    }


    void SmallObjectAllocator::Init(
        std::size_t maxObjectSize,
        std::size_t chunkSize,
        std::size_t alignment
    )
    {
        // Evita configurazioni non valide dell'allocator
        if(maxObjectSize == 0 || chunkSize == 0 || alignment == 0)
        {
            return;
        }

        // Salva l'allinemaneto usato per costruire le size class
        m_Alignment = alignment;

        // Arrotonda la dimensione massima al multiplo successivo dell'allineamento
        m_MaxObjectSize = ((maxObjectSize + alignment - 1) / alignment) * alignment;
        
        // Salva la dimensione indicativa di ogni Chunk
        m_ChunkSize = chunkSize;

        // Rimuove eventuali FixedAllocator già presenti
        m_Allocators.clear();

        // Calcola quanti FixedAllocator servono in base a maxObjectSize e alignment
        const std::size_t allocatorCount = m_MaxObjectSize / m_Alignment;

        // Riserva spazio nel vector per evitare riallocazioni inutili
        m_Allocators.reserve(allocatorCount);

        // Crea un FixedAllocator per ogni size class
        for(std::size_t i = 0; i < allocatorCount; i++)
        {
            // Calcola la dimensione dei blocchi gestiti da questo FixedAllocator
            const std::size_t blockSize = (i + 1) * m_Alignment;

            // Calcola quanti blocchi inserire in ogni Chunk
            std::size_t blocksPerChunk = m_ChunkSize / blockSize;

            // Garantisce almeno un blocco per Chunk
            if(blocksPerChunk == 0)
            {
                blocksPerChunk = 1;
            }

            // Limita i blocchi per Chunk perchè Chunk usa unsigned char come indice
            const std::size_t maxBlocks = static_cast<std::size_t>(std::numeric_limits<unsigned char>::max());

            // Evita di superare il massimo numero di blocchi rappresentabile
            if(blocksPerChunk > maxBlocks)
            {
                blocksPerChunk = maxBlocks;
            }

            // Crea il FixedAllocator per questa size class
            m_Allocators.emplace_back(
                blockSize,
                static_cast<unsigned char>(blocksPerChunk)
            );
        }
    }



    void* SmallObjectAllocator::Allocate(std::size_t size)
    {
        // Se la size non è gestibile, l'allocazione fallisce
        if(!CanHandle(size))
        {
            return nullptr;
        }

        // Calcola l'indice del FixedAllocator corretto
        const std::size_t index = GetAllocatorIndex(size);

        // Delega l'allocazione al FixedAllocator corretto
        return m_Allocators[index].Allocate();
    }


    void SmallObjectAllocator::Deallocate(void* ptr, std::size_t size)
    {
        // Deallocare nullptr non fa nulla
        if(ptr == nullptr)
        {
            return;
        }

        // Se la lize non è gestita da questo allocator, non fa nulla
        if(!CanHandle(size))
        {
            return;
        }


        // Trova il FixedAllocator corretto in base alla size originale
        const std::size_t index = GetAllocatorIndex(size);

        // Delega la deallocazione al FixedAllocator corretto
        m_Allocators[index].Deallocate(ptr);

    }

    
    bool SmallObjectAllocator::CanHandle(std::size_t size) const
    {
        // Ritorna true se la size rientra nella soglia small e l'allocator è inizializzato
        return size > 0 && size <= m_MaxObjectSize && !m_Allocators.empty();
    }


    std::size_t SmallObjectAllocator::GetMaxObjectSize() const
    {
        // Restituisce la dim massima gestita come small object
        return m_MaxObjectSize;
    }


    std::size_t SmallObjectAllocator::GetAlignment() const
    {
        // Restituisce l'allineamento usato per le size class
        return m_Alignment;
    }


    std::size_t SmallObjectAllocator::GetAllocatorCount() const
    {
        // Restituisce il numero di FixedAllocator creati
        return m_Allocators.size();
    }


    std::size_t SmallObjectAllocator::RoundUp(std::size_t size) const
    {
        // Arrotonda la size al multiplo successivo dell'allineamento
        return ((size + m_Alignment - 1) / m_Alignment) * m_Alignment;
    }


    std::size_t SmallObjectAllocator::GetAllocatorIndex(std::size_t size) const
    {
        // Calcola la size class e la converte nell'indice del FixedAllocator
        const std::size_t roundedSize = RoundUp(size);

        return (roundedSize / m_Alignment) - 1;
    }

}