#include "MemoryManager/MemoryManager.h"

#include <iostream>


// TEST CLASS
/*
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
*/




int main(){

    std::cout << "\n =======TEST=========" << std::endl;
    
    void* small = MM_MALLOC(32);   //Small obj
    void* big = MM_MALLOC(128);    // General

    MM_FREE(small);
    

    MM::PrintStats();
    MM::DumpLeaks();


    return 0;

}