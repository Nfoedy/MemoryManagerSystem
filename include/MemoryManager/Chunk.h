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

        // Disabilita la copia perchè Chunk possiede memoria raw
        Chunk(const Chunk&) = delete;
        Chunk& operator=(const Chunk&) = delete;

        // Abilita lo spostamento, utile quando Chunk viene gestito da std::vector
        Chunk(Chunk&& other) noexcept;
        Chunk& operator=(Chunk&& other) noexcept;

        // Inizializza il chunk con blocci di dim fissa
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
        bool IsCompletelyFree(unsigned char blocks) const;

    private:

        unsigned char* m_Data = nullptr;            // Puntatore alla memoria row gestita dal Chunk
        unsigned char m_FirstAvailableBlock = 0;    // Indice del primo blocco libero nella free list
        unsigned char m_BlocksAvailable = 0;        // Numero di blocchi liberi
    };
}