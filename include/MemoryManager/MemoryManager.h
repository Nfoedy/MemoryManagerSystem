#pragma once

#include <cstddef>  // per std::size_t
#include <utility>  // per std::forward
#include <new>      // per std::bad_alloc


// Namespace perchè cosi evito conflitti con altre funzioni Malloc
namespace MM
{
    void* Malloc(std::size_t size, const char* file, int line);  // Alloca un blocco di memoria di dimensione size e tiene traccia del file e della linea di codice per il debug
    void Free(void* ptr);       // Libera un blocco di memoria precedentemente allocato con Malloc

    void PrintStats();  // Stampa le statistiche base sull'utilizzo della memoria
    void DumpLeaks();  // Stampa  perdite di memoria

    // NEW
    template<typename T, typename... Args>
        T* New(const char* file, int line, Args&&... args)    // Alloca memoria per un oggetto di tipo T, lo costruisce con i parametri passati e tiene traccia del file e della linea di codice per il debug
        {
        // 1. Allocazione memoria
        void* mem = MM::Malloc(sizeof(T), file, line);   // Alloca memoria per un oggetto di tipo T

        if(!mem)
        {
            return nullptr; // Se l'allocazione fallisce, restituisce nullptr
        }

        // 2. Costruzione oggetto
        return new(mem) T(std::forward<Args>(args)...); // Costruisce l'oggetto di tipo T nella memoria allocata usando il costruttore con i parametri passati
    }


    // DELETE
    template<typename T>
    void Delete(T* ptr)      // Distrugge un oggetto di tipo T e libera la memoria associata
    {
        if(!ptr)
        {
            return; // Se il puntatore è nullptr, non fa nulla
        }

        // 1. Distruzione oggetto
        ptr->~T(); // Chiama il distruttore dell'oggetto di tipo T

        // 2. Liberazione memoria
        MM:Free(ptr); // Libera la memoria associata all'oggetto usando la funzione Free del MemoryManager
    }

}

// MACRO
#define MM_MALLOC(size) MM::Malloc(size, __FILE__, __LINE__) // Macro per chiamare Malloc con informazioni sul file e sulla linea di codice
#define MM_FREE(ptr) MM::Free(ptr) // Macro per chiamare Free

#define MM_NEW(type, ...) MM::New<type>(__FILE__, __LINE__, ##__VA_ARGS__) // Macro per chiamare New con informazioni sul file e sulla linea di codice
#define MM_DELETE(ptr) MM::Delete(ptr) // Macro per chiamare Delete
