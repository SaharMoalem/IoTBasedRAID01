#include <iostream>

#include "StripeSplitter.hpp"

#define TEST_ASSERT(cond)                                                      \
    do                                                                         \
    {                                                                          \
        if(!(cond))                                                            \
        {                                                                      \
            std::cerr << "FAIL: " << #cond << " at " << __FILE__ << ":"        \
                      << __LINE__ << "\n";                                     \
            return 1;                                                          \
        }                                                                      \
    } while(0)

int main()
{
    const size_t chunk = 4 * 1024 * 1024;
    const size_t numMinions = 3;

    TEST_ASSERT(ilrd::StripeSplitter::GetPrimaryIndex(0, chunk) == 0);
    TEST_ASSERT(ilrd::StripeSplitter::GetPrimaryIndex(chunk, chunk) == 1);
    TEST_ASSERT(ilrd::StripeSplitter::GetBackupIndex(0, numMinions) == 1);
    TEST_ASSERT(ilrd::StripeSplitter::GetBackupIndex(2, numMinions) == 0);

    const auto single = ilrd::StripeSplitter::Split(100, 50, chunk, numMinions);
    TEST_ASSERT(single.size() == 1);
    TEST_ASSERT(single[0].localOffset == 100);
    TEST_ASSERT(single[0].length == 50);
    TEST_ASSERT(single[0].primaryMinionIndex == 0);
    TEST_ASSERT(single[0].backupMinionIndex == 1);

    const size_t crossOffset = chunk - 10;
    const auto split = ilrd::StripeSplitter::Split(crossOffset, 20, chunk, numMinions);
    TEST_ASSERT(split.size() == 2);
    TEST_ASSERT(split[0].length == 10);
    TEST_ASSERT(split[1].length == 10);
    TEST_ASSERT(split[0].primaryMinionIndex == 0);
    TEST_ASSERT(split[1].primaryMinionIndex == 1);
    TEST_ASSERT(split[0].backupMinionIndex == 1);
    TEST_ASSERT(split[1].backupMinionIndex == 2);

    std::cout << "stripe splitter ok\n";
    return 0;
}
