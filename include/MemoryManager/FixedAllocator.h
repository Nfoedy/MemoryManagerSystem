#pragma once

#include "MemoryManager/Chunk.h"

#include <cstddef> // std::size_t
#include <vector> // std::vector


namespace MM
{
    /*
        FixedAllocator gestisce più Chunk della stessa dim.
    
    */


    class FixedAllocator
    {

    public:
        FixedAllocator() = default;
        FixedAllocator(std::size_t blockSize, unsigned char blocksPerChunk);

        FixedAllocator(const FixedAllocator&) = delete;
        FixedAllocator& operator=(const FixedAllocator&) = delete;

        FixedAllocator(FixedAllocator&&) noexcept = default;
        FixedAllocator& operator=(FixedAllocator&&) noexcept = default;

        void Init(std::size_t blockSize, unsigned char blocksPerChunk);

        // Alloca un blocco della dimensione gestita da questo FixedAllocator
        void* Allocate();

        // Restituisce un blocco al Chunk corretto
        void Deallocate(void* ptr);

        // Ritorna true se questo FixedAllocator gestisce il puntatore
        bool Owns(void* ptr) const;

        std::size_t GetBlockSize() const;
        std::size_t GetChunkCount() const;

    private:
        static constexpr std::size_t INVALID_CHUNK_INDEX = static_cast<std::size_t>(-1);

        std::size_t m_BlockSize = 0;            // Dimensione fissa dei blocchi gestiti
        unsigned char m_BlocksPerChunk = 0;     // Numero di blocchi per ogni Chunk

        std::vector<Chunk> m_Chunks;            // Lista dei Chunk gestiti

        std::size_t m_AllocChunkIndex = INVALID_CHUNK_INDEX;        // Ultimo Chunk usato per allocare
        std::size_t m_DeallocChunkIndex = INVALID_CHUNK_INDEX;      // Ultimo Chunk usato per deallocare
    };

}