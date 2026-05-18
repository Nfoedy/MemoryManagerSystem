#include "MemoryManager/MemoryManager.h"

#include <iostream>


class Player
{
public:
    Player(int hp)
        : m_Hp(hp)
    {
        std::cout << "[Player] Constructor | HP: " << m_Hp << std::endl;
    }

    ~Player()
    {
        std::cout << "[Player] Destructor" << std::endl;
    }

private:
    int m_Hp;
};




int main()
{
    std::cout << "\n===== TEST MEMORY MANAGER ADVANCED =====\n";

    // Small allocations: devono usare il nuovo SmallObjectAllocator
    void* a = MM_MALLOC(4);     // size class 8
    void* b = MM_MALLOC(12);    // size class 16
    void* c = MM_MALLOC(32);    // size class 32
    void* d = MM_MALLOC(64);    // size class 64

    // General allocation: deve usare GeneralAllocator
    void* e = MM_MALLOC(128);

    MM_FREE(a);
    MM_FREE(b);
    MM_FREE(c);
    MM_FREE(d);
    MM_FREE(e);

    std::cout << "\n===== TEST MM_NEW / MM_DELETE =====\n";

    Player* player = MM_NEW(Player, 100);
    MM_DELETE(player);

    std::cout << "\n===== TEST LEAK DETECTION =====\n";

    Player* leakedPlayer = MM_NEW(Player, 200);

    // leakedPlayer NON viene liberato apposta,
    // così controlliamo se il MemoryTracker rileva il leak.

    MM::PrintStats();
    MM::DumpLeaks();


    return 0;
    
}