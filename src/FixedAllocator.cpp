#include "MemoryManager/FixedAllocator.h"

#include <utility>  // std::move


namespace MM
{
    FixedAllocator::FixedAllocator(std::size_t blockSize, unsigned char blocksPerChunk)
    {
        Init(blockSize, blocksPerChunk);
    }

    void FixedAllocator::Init(std::size_t blockSize, unsigned char blocksPerChunk)
    {
        // Salva la dimensione fissa dei blocchi e quanti blocchi avrà ogni Chunk
        m_BlockSize = blockSize;
        m_BlocksPerChunk = blocksPerChunk;

        // Rimuove eventuali Chunk precedenti
        m_Chunks.clear();

        // Reset degli indirizzi cached usati per allocazione e deallocazione
        m_AllocChunkIndex = INVALID_CHUNK_INDEX;
        m_DeallocChunkIndex = INVALID_CHUNK_INDEX;
    }


    void* FixedAllocator::Allocate()
    {
        // Se l'allocator non è inizializzato correttamente, l'allocazione fallisce
        if(m_BlockSize == 0 || m_BlocksPerChunk == 0)
        {
            return nullptr;
        }

        // Prova prima l'ultimo Chunk usato per allocare
        if(m_AllocChunkIndex != INVALID_CHUNK_INDEX 
            && m_AllocChunkIndex < m_Chunks.size() 
            && m_Chunks[m_AllocChunkIndex].HasAvailableBlocks())
        {
            return m_Chunks[m_AllocChunkIndex].Allocate(m_BlockSize);
        }

        // Se il Chunk cached non va bene, allora cerco un altro Chunk con spazio libero
        for(std::size_t i = 0; i < m_Chunks.size(); ++i)
        {
            if(m_Chunks[i].HasAvailableBlocks())
            {
                m_AllocChunkIndex = i;
                return m_Chunks[i].Allocate(m_BlockSize);
            }
        }

        // Se nessun Chunk ha spazio, ne creo uno nuovo
        Chunk newChunk;
        newChunk.Init(m_BlockSize, m_BlocksPerChunk);

        // Se il nuovo Chunk non è valido, l'allocazione fallisce
        if(!newChunk.HasAvailableBlocks())
        {
            return nullptr;
        }

        // Sposta il nuovo Chunk dentro il vector senza copiarlo
        m_Chunks.push_back(std::move(newChunk));

        // Aggiorna l'indice del Chunk usato per allocare
        m_AllocChunkIndex = m_Chunks.size() - 1;

        // Alloca dal nuovo Chunk appena creato
        return m_Chunks[m_AllocChunkIndex].Allocate(m_BlockSize);
    }


    void FixedAllocator::Deallocate(void* ptr)
    {
        // Deallocare un nullpt non fa nulla
        if(ptr == nullptr)
        {
            return;
        }

        // Prima provo l'ultimo Chunk usato per deallocare.
        if(m_DeallocChunkIndex != INVALID_CHUNK_INDEX &&
           m_DeallocChunkIndex < m_Chunks.size() &&
           m_Chunks[m_DeallocChunkIndex].Owns(ptr, m_BlockSize, m_BlocksPerChunk))
        {
            m_Chunks[m_DeallocChunkIndex].Deallocate(ptr, m_BlockSize);
            return;
        }

        // Se non è quello giusto, cerco il Chunk proprietario del puntatore
        for(std::size_t i = 0; i < m_Chunks.size(); ++i)
        {
            if(m_Chunks[i].Owns(ptr, m_BlockSize, m_BlocksPerChunk))
            {
                m_DeallocChunkIndex = i;
                m_Chunks[i].Deallocate(ptr, m_BlockSize);
                return;
            }
        }

        // Se arriva qui, il puntatore non appartiene a questo FixedAllocator
    }


    bool FixedAllocator::Owns(void* ptr) const
    {
        // Un nullptr non può appartenere a nessun Chunk
        if(ptr == nullptr)
        {
            return false;
        }

        // Controlla se almeno un Chunk gestito possiede il puntatore
        for(const Chunk& chunk : m_Chunks)
        {
            if(chunk.Owns(ptr, m_BlockSize, m_BlocksPerChunk))
            {
                return true;
            }
        }

        return false;
    }


    std::size_t FixedAllocator::GetBlockSize() const
    {
        // Restituisce la dimensione fissa dei blocchi gestiti
        return m_BlockSize;
    }

    
    std::size_t FixedAllocator::GetChunkCount() const
    {
        // Restituisce quanti Chunk sono attualmente gestiti
        return m_Chunks.size();
    }


}
