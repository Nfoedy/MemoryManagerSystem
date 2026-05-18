#include "MemoryManager/SmallObjectAllocator.h"

#include <limits>   // std::numeric_limits


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
        if(maxObjectSize == 0 || chunkSize == 0 || alignment == 0)
        {
            return;
        }

        m_Alignment = alignment;

        // Arrotondo maxObjectiSize all'allineamento scelto
        m_MaxObjectSize = ((maxObjectSize + alignment - 1) / alignment) * alignment;
        
        // Salvo la dimensione indicativa di ogni Chunk
        m_ChunkSize = chunkSize;

        // Svuoto i FixedAllocator già presenti
        m_Allocators.clear();

        // Calcolo quanti FixedAllocator servono
        const std::size_t allocatorCount = m_MaxObjectSize / m_Alignment;

        // Riservo spazio nel vector
        m_Allocators.reserve(allocatorCount);

        // Creo tutti i FixedAllocator
        for(std::size_t i = 0; i < allocatorCount; i++)
        {
            // Calcolo della blockSize
            const std::size_t blockSize = (i + 1) * m_Alignment;

            // Calcolo quanti blocchi mettere dentro ogni Chunk
            std::size_t blocksPerChunk = m_ChunkSize / blockSize;

            if(blocksPerChunk == 0)
            {
                blocksPerChunk = 1;
            }

            // Il Chunk usa unsigned char per salvare gli indici dei blocchi liberi, quindi massimo 255 blocchi
            const std::size_t maxBlocks = static_cast<std::size_t>(std::numeric_limits<unsigned char>::max());

            if(blocksPerChunk > maxBlocks)
            {
                blocksPerChunk = maxBlocks;
            }

            // Creo un FixedAllocator per questo blockSize
            m_Allocators.emplace_back(
                blockSize,
                static_cast<unsigned char>(blocksPerChunk)
            );
        }
    }



    void* SmallObjectAllocator::Allocate(std::size_t size)
    {
        if(!CanHandle(size))
        {
            return nullptr;
        }

        // Calcolo l'indice del FixedAllocator corretto
        const std::size_t index = GetAllocatorIndex(size);

        // Delego l'allocazione al FixedAllocator corretto
        return m_Allocators[index].Allocate();
    }


    void SmallObjectAllocator::Deallocate(void* ptr, std::size_t size)
    {
        if(ptr == nullptr)
        {
            return;
        }

        if(!CanHandle(size))
        {
            return;
        }


        // Trovo il FixedAllocator corretto in base alla size originale
        const std::size_t index = GetAllocatorIndex(size);

        // Delego la deallocazione al FixedAllocator corretto
        m_Allocators[index].Deallocate(ptr);

    }

    
    // Dice se una certa size può essere gestita da questo SmallObjectAllocator
    bool SmallObjectAllocator::CanHandle(std::size_t size) const
    {
        return size > 0 && size <= m_MaxObjectSize && !m_Allocators.empty();
    }



    std::size_t SmallObjectAllocator::GetMaxObjectSize() const
    {
        return m_MaxObjectSize;
    }


    std::size_t SmallObjectAllocator::GetAlignment() const
    {
        return m_Alignment;
    }


    std::size_t SmallObjectAllocator::GetAllocatorCount() const
    {
        return m_Allocators.size();
    }


    std::size_t SmallObjectAllocator::RoundUp(std::size_t size) const
    {
        return ((size + m_Alignment - 1) / m_Alignment) * m_Alignment;
    }


    std::size_t SmallObjectAllocator::GetAllocatorIndex(std::size_t size) const
    {
        const std::size_t roundedSize = RoundUp(size);

        return (roundedSize / m_Alignment) - 1;
    }

}