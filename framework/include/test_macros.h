#ifndef ILRD_TEST_MACROS_H
#define ILRD_TEST_MACROS_H

#include <cstdlib>
#include <iostream>

#define RED   ""
#define GREEN ""
#define PASS  std::cout << "PASS\n"

#define TEST_ASSERT(cond)                                                      \
    do                                                                         \
    {                                                                          \
        if(!(cond))                                                            \
        {                                                                      \
            std::cerr << "FAIL: " << #cond << " at " << __FILE__ << ":"        \
                      << __LINE__ << "\n";                                     \
            std::exit(1);                                                      \
        }                                                                      \
    } while(0)

#endif
