#pragma once

#include <cstddef>   // std::size_t


namespace MM
{
    /* Chunk
    Rappresenta un blocco di memoria diviso in tanti blocchi più piccoli, tutti della stessa dim.
    è il livello più basso dello SmallObjectAllocator 
    */

    class Chunk
    {

    public:
        Chunk() = default;
        ~Chunk();

        // Evitiamo copie accidentali: Chunk possiede memoria raw
        Chunk(const Chunk&) = delete;
        Chunk& operator=(const Chunk&) = delete;

        // Permettiamo lo spostamento, utile in futuro con std::vector<Chunk>
        Chunk(Chunk&& other) noexcept;
        Chunk& operator=(Chunk&& other) noexcept;

        // Inizializza il chunk con blocci di dimensione blockSize
        void Init(std::size_t blockSize, unsigned char blocks);

        // Libera memoria gestita dal chunk
        void Release();

        // Alloca un blocco dal chunk
        void* Allocate(std::size_t blockSize);

        // Restituisce un blocco al chunk
        void Deallocate(void* ptr, std::size_t blockSize);

        // Ritorna true se il chunk ha almeno un blocco libero
        bool HasAvailableBlocks() const;

        // Ritorna true se il puntatore appartiene a questo chunk
        bool Owns(void* ptr, std::size_t blockSize, unsigned char blocks) const;

        // Ritorna true se tutti i bloccji sono liberi
        bool IsCompletelyFree(unsigned char blcoks) const;

    private:

        unsigned char* m_Data = nullptr;            // Memoria gestita dal Chunk
        unsigned char m_FirstAvailableBlock = 0;    // Indice del primo blocco libero
        unsigned char m_BlocksAvailable = 0;         // Numero di blocchi liberi
    };
}