#include "MemoryManager/Chunk.h"

#include <cstdlib>   //std::malloc , std::free


namespace MM
{
    Chunk::~Chunk()
    {
        Release();
    }


    Chunk::Chunk(Chunk&& other) noexcept
        : m_Data(other.m_Data),
          m_FirstAvailableBlock(other.m_FirstAvailableBlock),
          m_BlocksAvailable(other.m_BlocksAvailable)
    {
      // Svuotiamo l'oggetto sorgente per evitare doppia free
      other.m_Data = nullptr;
      other.m_FirstAvailableBlock = 0;
      other.m_BlocksAvailable = 0;
    }


    Chunk& Chunk::operator=(Chunk&& other) noexcept
    {
      if(this != &other)
      {
        // Livera enventuale memoria già posseduta da questo Chunk
        Release();

        // Trasferisce la memoria dall'altro Chunk a questo
        m_Data = other.m_Data;
        m_FirstAvailableBlock = other.m_FirstAvailableBlock;
        m_BlocksAvailable = other.m_BlocksAvailable;

        // Svuota l'altro Chunk per evitare doppia free
        other.m_Data = nullptr;
        other.m_FirstAvailableBlock = 0;
        other.m_BlocksAvailable = 0;
      }

      return *this;
    }


    void Chunk::Init(std::size_t blockSize, unsigned char blocks)
    {
      // Se il Chunk era già stato inizializzato, liberiamo prima la vecchia memoria
      Release();

      if(blockSize == 0 || blocks == 0)
      {
        return; 
      }

      // Alloca un unico blocco di memoria: blockSize * numero di blocchi
      m_Data = static_cast<unsigned char*>(std::malloc(blockSize * blocks));

      // Se Malloc fallisce, lasciamo il Chunk in stato vuoto
      if(!m_Data)
      {
        m_FirstAvailableBlock = 0;
        m_BlocksAvailable = 0;
        return;
      }

      m_FirstAvailableBlock = 0;
      m_BlocksAvailable = blocks;

      /*
        Costruzione della free list interna.

        Ogni blocco libero usa il suo primo byte per salvare l'indice del prossimo blocco libero.
      */

      unsigned char* current = m_Data;

      for(unsigned char i = 0; i < blocks; ++i)
      {
        *current = static_cast<unsigned char>(i + 1);
        current += blockSize;
      }
    }


    void Chunk::Release()
    {
      // std::free(nullptr) è valido, quindi non serve controllare
      std::free(m_Data);

      m_Data = nullptr;
      m_FirstAvailableBlock = 0;
      m_BlocksAvailable = 0;
    }



    void* Chunk::Allocate(std::size_t blockSize)
    {
      // Se non ci sono blocchi liberi l'allocazione fallisce
      if(m_BlocksAvailable == 0)
      {
        return nullptr;
      }

      // Calcola l'indirizzo del primo blocco disponibile
      unsigned char* result = m_Data + (m_FirstAvailableBlock * blockSize);

      // Il primo byte del blocco livero contiene l'indice del prossimo blocco libero
      m_FirstAvailableBlock = *result;

      // Un blocco libero in meno
      --m_BlocksAvailable;

      // Restituisce il blocco all'utente
      return result;

    }


    void Chunk::Deallocate(void* ptr, std::size_t blockSize)
    {
      if(ptr == nullptr || m_Data == nullptr)
      {
        return;
      }

      unsigned char* releasedBlock = static_cast<unsigned char*>(ptr);

      // Calcola l'indice del blocco restituito
      const std::size_t blockIndex = static_cast<std::size_t>(releasedBlock - m_Data) / blockSize;

      /*
        Rimetto il blocco liberato in testa alla free list

        Il blocco appena liberato contiene l'indice del vecchio blocco libero.
        Poi m_FirstAvailableBlock diventa l'indice del blocco appena liberato
      */
      *releasedBlock = m_FirstAvailableBlock;
      m_FirstAvailableBlock = static_cast<unsigned char>(blockIndex);

      // Un blocco libero in più
      ++m_BlocksAvailable;

    }


    bool Chunk::HasAvailableBlocks() const
    {
      return m_BlocksAvailable > 0;
    }


    bool Chunk::Owns(void* ptr, std::size_t blockSize, unsigned char blocks) const
    {
      if(ptr == nullptr || m_Data == nullptr)
      {
        return false;  
      }

      const unsigned char* address = static_cast<const unsigned char*>(ptr);
      const unsigned char* begin = m_Data;
      const unsigned char* end = m_Data + (blockSize * blocks);

      return address >= begin && address < end;
    }



    bool Chunk::IsCompletelyFree(unsigned char blocks) const
    {
      return m_BlocksAvailable == blocks;
    }


}



