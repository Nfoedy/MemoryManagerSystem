#include "MemoryManager/MemoryManager.h"

#include <iostream>


class Player
{
public:
    Player(int hp)
        : m_Hp(hp)
    {
    }

private:
    int m_Hp;
};


class BigEnemy
{
public:
    BigEnemy(int id)
        : m_Id(id)
    {
    }

private:
    int m_Id;
    char m_Data[128];
};


#ifndef USE_MM_GLOBAL_OVERRIDES

int main()
{
    std::cout << "======== MEMORY MANAGER API TEST ========" << std::endl;

    std::cout << "\n--- Raw allocations ---" << std::endl;

    void* smallBlock = MM_MALLOC(12);     // Small allocation -> SOBA
    void* largeBlock = MM_MALLOC(128);    // Large allocation -> GeneralAllocator

    MM_FREE(smallBlock);
    MM_FREE(largeBlock);


    std::cout << "\n--- Object allocations ---" << std::endl;

    Player* player = MM_NEW(Player, 100);     // Small object -> SOBA
    BigEnemy* enemy = MM_NEW(BigEnemy, 1);    // Large object -> GeneralAllocator

    MM_DELETE(player);
    MM_DELETE(enemy);


    std::cout << "\n--- Final report ---" << std::endl;

    MM::PrintStats();
    MM::DumpLeaks();

    return 0;
}

#else

int main()
{
    std::cout << "======== GLOBAL NEW/DELETE OVERRIDE TEST ========" << std::endl;

    /*
        With global overrides enabled, avoid printing while the MemoryManager
        is initialized, because std::cout may allocate internally.
    */
    MM::Initialize();

    Player* player = new Player(200);

    delete player;

    MM::Shutdown();


    std::cout << "\n--- Final report ---" << std::endl;

    MM::PrintStats();
    MM::DumpLeaks();

    return 0;
}

#endif