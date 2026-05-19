#include "MemoryManager/MemoryManager.h"

#include <iostream>

class SilentPlayer
{
public:
    SilentPlayer(int hp)
        : m_Hp(hp)
    {
    }

    ~SilentPlayer()
    {
    }

private:
    int m_Hp;
};

int main()
{
    std::cout << "===== CLEAN GLOBAL OVERRIDE TEST =====" << std::endl;

    MM::Initialize();

    SilentPlayer* player = new SilentPlayer(100);
    delete player;

    MM::Shutdown();

    MM::PrintStats();
    MM::DumpLeaks();

    return 0;
}