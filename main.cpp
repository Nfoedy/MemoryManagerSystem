#include "MemoryManager/MemoryManager.h"

#include "tests/SameTest.h"
#include "tests/BulkTest.h"
#include "tests/ReverseTest.h"
#include "tests/ButterflyTest.h"

#include <iostream>


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