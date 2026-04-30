#include "MemoryManager/MemoryManager.h"

#include <cstdlib>


static bool g_InsideMM = false;  // Variabile per evitare una ricorsione infinita. Dice se siamo dentro al MM   

// Override globale dell'operatore new. Da ora in poi quando si scrive "new Tipo()"", il programma passerà da questa funzione
void* operator new(size_t size)
{
    // Se siamo già dentro al MM non richiama la MM::Malloc
    if(g_InsideMM) return std::malloc(size);

    g_InsideMM = true;   // Segna che siamo entrati nel MM

    void* ptr = MM::Malloc(size, "global_new", 0); // Chiede memoria al MM, usando un file fittizio "global_new"

    g_InsideMM = false;  // Usciamo dal MM

    return ptr;  // Restituisce il puntatore allocato

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
    if(g_InsideMM) 
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

    // Se il puntatore è nullo non fa nulla
    if(ptr == nullptr)
    {
        return;
    }

    // Se siamo già dentro al MM usiamo free direttamente per evitare ricorsione
    if(g_InsideMM) 
    {
        std::free(ptr);
        return;
    }


    g_InsideMM = true; // Segna che siamo entrati nel MM

    MM::Free(ptr);  // Libera la memoria passando dal MM

    g_InsideMM = false;  // Usciamo dal MM
   
}




