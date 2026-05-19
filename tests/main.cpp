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
    std::cout << "===== TEST GLOBAL NEW DELETE OVERRIDE =====\n";

    Player* player = new Player(100);

    delete player;

    MM::PrintStats();
    MM::DumpLeaks();

    return 0;
}