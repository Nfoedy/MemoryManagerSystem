#pragma once

#include "MemoryManager/FixedAllocator.h"

#include <cstddef>  // std::size_t
#include <vector>   // std::vector


namespace MM
{
    /*
        SmallObjectAllocator

        Gestisce più FixedAllocator, ognuno specializzato per una size class
    */

    class SmallObjectAllocator
    {
    public:
        SmallObjectAllocator() = default;

        SmallObjectAllocator(
            std::size_t maxObjectSize,
            std::size_t chunkSize,
            std::size_t alignment = 8
        );

        void Init(
            std::size_t maxObjectSize,
            std::size_t chunkSize,
            std::size_t aligment = 8
        );


        // Alloca memoria raw per una richiesta piccola
        void* Allocate(std::size_t size);
        
        // Dealloca memoria raw precendentemente ottenura da Allocate(size)
        void Deallocate(void* ptr, std::size_t size);

        // Ritorna true se la size può essere gestita da questo allocator
        bool CanHandle(std::size_t size) const;

        std::size_t GetMaxObjectSize() const;
        std::size_t GetAlignment() const;
        std::size_t GetAllocatorCount() const;


    private:

        std::size_t RoundUp(std::size_t size) const;
        std::size_t GetAllocatorIndex(std::size_t size) const;

    private:

        std::size_t m_MaxObjectSize = 0;        // Dimensione massima gestica come Small Object
        std::size_t m_ChunkSize = 0;            // Dimensione indicativa di ogni Chunk
        std::size_t m_Alignment = 8;            // Step delle size class
        
        std::vector<FixedAllocator> m_Allocators;       // FixedAllocator per size diverse
        
    };


}
