#include "MemoryManager/MemoryManager.h"

#include <cstdlib>
#include <cstddef>
#include <new>



#ifdef USE_MM_GLOBAL_OVERRIDES


static thread_local bool g_InsideMM = false;  // Flag per evitare una ricorsione infinita. Dice se siamo dentro al MM   

// Override globale dell'operatore new. 
// Se USE_MM_GLOBAL_OVERRIDES è definitio, ogni "new" passa da qui
void* operator new(std::size_t size)
{
    // Evita ricorsione infinita
    if(g_InsideMM || !MM::IsInitialized())
    {

        void* ptr = std::malloc(size);

        if(ptr == nullptr)
        {
            throw std::bad_alloc();
        }

        return ptr;
    }

    g_InsideMM = true;

    void* ptr = MM::Malloc(size, "global_new", 0);

    g_InsideMM = false;

    if(ptr == nullptr)
    {
        throw std::bad_alloc();
    }

    return ptr;
    
}



// Override globale dell'operatore new[]
void* operator new[](std::size_t size)
{
    return operator new(size);
}



// Override globale dell'operatore delete classico. Da ora in poi quando si scrive "delete ... " il programma passera da questa funzione
void operator delete(void* ptr) noexcept
{
    // Se il puntatore è nullo non fa nulla
    if(ptr == nullptr)
    {
        return;
    }

    // Se siamo già dentro al MM usiamo free direttamente per evitare ricorsione
    if(g_InsideMM || !MM::IsInitialized()) 
    {
        std::free(ptr);
        return;
    }


    g_InsideMM = true; // Segna che siamo entrati nel MM

    MM::Free(ptr);  // Libera la memoria passando dal MM

    g_InsideMM = false;  // Usciamo dal MM
   
}


// Override globale dell'operatore delete con size. Da ora in poi quando si scrive "delete ... " il programma passera da questa funzione
void operator delete(void* ptr, size_t size) noexcept
{

    (void)size; // In questa implementazione la size non ci serve, perchè il MM recupera la dim dalla mappa delle allocazioni

    operator delete(ptr);
   
}



// Override globale dell'operatore delete[] classico
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


#endif
