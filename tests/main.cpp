#include "MemoryManager/MemoryManager.h"
#include <iostream>


class Player
{
public:
    Player(int hp)
    {
        std::cout << "Player creato con hp: " << hp << std::endl;
    }

    ~Player()
    {
        std::cout << "Player distrutto" << std::endl;
    }
};



int main()
{
    //void* ptr1 = MM::Malloc(64);    // Alloca 64 byte di memoria e ottiene un puntatore ad essa
    //void* ptr2 = MM::Malloc(128);

    //MM::Free(ptr1);    // Libera la memoria allocata in precedenza
    //MM::Free(ptr2);    // Libera la memoria allocata in precedenza

    Player* p1 = MM::New<Player>(100); // Alloca memoria per un oggetto Player e lo costruisce con hp = 100

    MM::Delete(p1); // Distrugge l'oggetto Player e libera la memoria associata

    MM::PrintStats();
    MM::DumpLeaks();

    return 0;   
}