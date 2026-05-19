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
    MM::Initialize();

    std::cout << "===== MEMORY MANAGER TEST =====" << std::endl;

    // TEST 1 - Raw allocations
    std::cout << "\n--- TEST 1: MM_MALLOC / MM_FREE ---" << std::endl;

    void* small = MM_MALLOC(12);   // Pool, size class 16
    void* big   = MM_MALLOC(128);  // GeneralAllocator

    MM_FREE(small);
    MM_FREE(big);


    // TEST 2 - Object allocation
    std::cout << "\n--- TEST 2: MM_NEW / MM_DELETE ---" << std::endl;

    Player* player = MM_NEW(Player, 100);

    MM_DELETE(player);


    // TEST 3 - Global new/delete, solo se abilitato
    std::cout << "\n--- TEST 3: GLOBAL NEW / DELETE ---" << std::endl;

#ifdef USE_MM_GLOBAL_OVERRIDES
    std::cout << "Global override ENABLED" << std::endl;

    Player* globalPlayer = new Player(200);

    delete globalPlayer;
#else
    std::cout << "Global override DISABLED" << std::endl;
#endif


    // Final report
    std::cout << "\n--- FINAL REPORT ---" << std::endl;

    MM::PrintStats();
    MM::DumpLeaks();

    MM::Shutdown();

    return 0;
}