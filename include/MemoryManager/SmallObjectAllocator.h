#pragma once

#include "MemoryManager/FixedAllocator.h"

#include <cstddef>  // std::size_t
#include <vector>   // std::vector


namespace MM
{
    /*
        SmallObjectAllocator

        Gestisce più FixedAllocator, ognuno specializzato per una size class.
        È l'allocator usato per le allocazioni piccole.
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

        // Inizializza la size class e crea i FixedAllocator necessari
        void Init(
            std::size_t maxObjectSize,
            std::size_t chunkSize,
            std::size_t aligment = 8
        );


        // Alloca memoria raw usando il FixedAllocator corretto per la size richiesta
        void* Allocate(std::size_t size);
        
        // Dealloca memoria raw precendentemente ottenura da Allocate(size)
        void Deallocate(void* ptr, std::size_t size);

        // Ritorna true se la size può essere gestita dallo SmallObjectAllocator
        bool CanHandle(std::size_t size) const;

        // Restituisce la dimensione massima gestista come small object
        std::size_t GetMaxObjectSize() const;

        // Restistuisce l'allinemanto usato per calcolare la size class
        std::size_t GetAlignment() const;

        // Restituisce il numero di FixedAllocator creati
        std::size_t GetAllocatorCount() const;


    private:

        // Arrotonda una size al multiplo successivo all'allineamento
        std::size_t RoundUp(std::size_t size) const;

        // Converte una size nell'indice del FixedAllocator corretto
        std::size_t GetAllocatorIndex(std::size_t size) const;

    private:

        std::size_t m_MaxObjectSize = 0;        // Dimensione massima gestica come Small Object
        std::size_t m_ChunkSize = 0;            // Dimensione indicativa di ogni Chunk
        std::size_t m_Alignment = 8;            // Step delle size class: 8, 16, 24...
        
        std::vector<FixedAllocator> m_Allocators;       // FixedAllocator per size class diverse
        
    };


}
