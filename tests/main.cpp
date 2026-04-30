#include "MemoryManager/MemoryManager.h"
#include "MemoryManager/SmallObjectAllocator.h"

#include <iostream>

/*
class Player
{
public:
    Player(int hp) : m_Hp(hp)
    {
        std::cout << "Player creato con hp: " << m_Hp << std::endl;
    }

    ~Player()
    {
        std::cout << "Player distrutto" << std::endl;
    }

private: 
    int m_Hp;
};

*/

int main()
{
    //void* ptr1 = MM::Malloc(64);    // Alloca 64 byte di memoria e ottiene un puntatore ad essa
    //void* ptr2 = MM::Malloc(128);

    //MM::Free(ptr1);    // Libera la memoria allocata in precedenza
    //MM::Free(ptr2);    // Libera la memoria allocata in precedenza

    /*
    Player* p1 = MM_NEW(Player, 100); // Alloca memoria per un oggetto Player e lo costruisce con hp = 100

    MM_DELETE(p1); // Distrugge l'oggetto Player e libera la memoria associata

    MM::PrintStats();
    MM::DumpLeaks();
    */

    /*
    SmallObjectAllocator allocator(32, 10); // Crea un SmallObjectAllocator con blocchi di 32 byte e 10 blocchi totali

    void* blocco1 = allocator.Allocate(); // Alloca un blocco di memoria dal SmallObjectAllocator
    void* blocco2 = allocator.Allocate(); // Alloca un altro blocco di memoria dal SmallObjectAllocator

    allocator.Free(blocco1); // Libera il primo blocco di memoria
    allocator.Free(blocco2); // Libera il secondo blocco di memoria

    std::cout << "SmallObjectAllocator test completato" << std::endl;
    */

    /*
    void* a = MM_MALLOC(32);   // Pool
    void* b = MM_MALLOC(128);  // Heap

    MM_FREE(a);
    MM_FREE(b);

    */
   
    /*
        // TEST GENERAL ALLOCATOR

        // Small
        int* x = (int*)MM_MALLOC(4);

        // General
        void* y = MM_MALLOC(128);

        MM_FREE(x);
        MM_FREE(y);

        MM::PrintStats();   // Stampa le caratteristiche
        MM::DumpLeaks();    // Controlla evenetuali leak
    */


    /*
    int* x = MM_NEW(int,10);

    MM::PrintStats();
    MM::DumpLeaks();
    */

    int* a = (int*)MM_MALLOC(4);
    int* b = MM_NEW(int, 20);

    MM_FREE(a);
    // b NON lo libero → leak

    MM::PrintStats();
    MM::DumpLeaks();


    return 0;
}