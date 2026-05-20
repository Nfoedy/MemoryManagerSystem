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
    std::cout << "========== MEMORY MANAGER API TEST ==========" << std::endl;

    std::cout << "\n--- Raw allocations ---" << std::endl;

    void* smallBlock = MM_MALLOC(12);
    void* largeBlock = MM_MALLOC(128);

    MM_FREE(smallBlock);
    MM_FREE(largeBlock);


    std::cout << "\n--- Object allocations ---" << std::endl;

    Player* player = MM_NEW(Player, 100);
    BigEnemy* enemy = MM_NEW(BigEnemy, 1);

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
    std::cout << "========== CLEAN GLOBAL OVERRIDE TEST ==========" << std::endl;

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


/*

int main()
{




    
    std::cout << "========== MEMORY MANAGER STRESS TESTS ==========" << std::endl;

    RunSameTest();
    RunBulkTest();
    RunReverseTest();
    RunButterflyTest();

    std::cout << "\n========== FINAL REPORT ==========" << std::endl;

    MM::PrintStats();
    MM::DumpLeaks();

    return 0;

    
}
    */