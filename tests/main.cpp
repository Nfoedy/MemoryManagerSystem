#include "MemoryManager/MemoryManager.h"

#include <iostream>


// TEST CLASS

class Player
{
private:
    int m_Hp;

public: 
    Player(int hp) : m_Hp(hp)
    {
        std::cout << "[Player] Creato con HP: " << m_Hp << std::endl;
    }

    ~Player()
    {
        std::cout << "[Player] Distrutto" << std::endl;
    }

};


// TEST 1 - NO LEAK

void Test_NoLeaks()
{
    std::cout << " ===== TEST NO LEAKS =====" << std::endl;

    // Allocazione raw
    void* a = MM_MALLOC(32);

    // Allocazione oggetto
    Player* p = MM_NEW(Player,100);

    // Deallocazione
    MM_FREE(a);
    MM_DELETE(p);

    MM::PrintStats();
    MM::DumpLeaks();
}


// TEST 2 - LEAK


void Test_Leaks()
{
    std::cout << " ===== TEST LEAKS =====" << std::endl;

    // Allozacione raw
    int* a = (int*)MM_MALLOC(4);

    // Allocazione oggetto
    Player* p = MM_NEW(Player,100);
    
    //Deallocazione (ne libero solo 1)
    MM_FREE(a);

    MM::PrintStats();
    MM::DumpLeaks();
}








int main()
{
    Test_NoLeaks();

    Test_Leaks();

    return 0;
}