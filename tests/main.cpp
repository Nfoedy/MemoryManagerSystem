#include "MemoryManager/MemoryManager.h"


int main()
{
    void* ptr1 = MM::Malloc(64);    // Alloca 64 byte di memoria e ottiene un puntatore ad essa
    void* ptr2 = MM::Malloc(128);


    MM::Free(ptr1);    // Libera la memoria allocata in precedenza
    MM::Free(ptr2);    // Libera la memoria allocata in precedenza

    MM::PrintStats();
    
    return 0;   
}