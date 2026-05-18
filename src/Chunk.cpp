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

          


}



