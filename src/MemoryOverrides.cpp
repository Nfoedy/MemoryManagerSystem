#include "MemoryManager/MemoryManager.h"

#include <cstdlib>      // std::malloc, std::free
#include <cstddef>      // std::size_t
#include <new>          // std::bad_alloc



#ifdef USE_MM_GLOBAL_OVERRIDES

// Flag thread-local per evitare ricorsione quando il MemoryManager usa internamenti new/delete
static thread_local bool g_InsideMM = false;    

// Override globale dell'operatore new. 
// Se USE_MM_GLOBAL_OVERRIDES è definitio, ogni "new" passa da qui
void* operator new(std::size_t size)
{
    // Prima di MM::Initialize() o durante chiamate interne, usa malloc diretto
    if(g_InsideMM || !MM::IsInitialized())
    {
        void* ptr = std::malloc(size);

        if(ptr == nullptr)
        {
            throw std::bad_alloc();
        }

        return ptr;
    }

    // Da qui in poi l'allocazione passa dal MemoryManager
    g_InsideMM = true;

    void* ptr = MM::Malloc(size, "global_new", 0);

    g_InsideMM = false;

    if(ptr == nullptr)
    {
        throw std::bad_alloc();
    }

    return ptr;
    
}



// Override globale dell'operatore new[] per gli array dinamici
void* operator new[](std::size_t size)
{
    return operator new(size);
}



// Override globale dell'operatore delete classico. Da ora in poi quando si scrive "delete ... " il programma passera da questa funzione
// Attivo solo se USE_MM_GLOBAL_OVERRIDES è definito
void operator delete(void* ptr) noexcept
{
    // Se il puntatore è nullo non fa nulla
    if(ptr == nullptr)
    {
        return;
    }

    // Prima di MM::Initialize() o durante chiamate interne, ysa free diretto
    if(g_InsideMM || !MM::IsInitialized()) 
    {
        std::free(ptr);
        return;
    }

    // Da qui in poi la deallocazione passa dal MemoryManager
    g_InsideMM = true; 

    MM::Free(ptr); 

    g_InsideMM = false; 
   
}


// Override globale dell'operatore delete con size, usato da alcuni compilatori moderni
void operator delete(void* ptr, std::size_t size) noexcept
{
    // La size non serve perchè MM::Free recupera le informazioni dal MemoryTracker
    (void)size; 

    operator delete(ptr);   
}



// Override globale dell'operatore delete[] per gli array dinamici
void operator delete[](void* ptr) noexcept
{
    operator delete(ptr);
}


// Override globale dell'operatore delete[] con size
void operator delete[](void* ptr, std::size_t size) noexcept
{
    (void)size;

    operator delete(ptr);
}


#endif  // USE_MM_GLOBAL_OVERRIDES
