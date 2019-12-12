//#include "Client.h"
#include "tests.cpp"
#include <gtest/gtest.h>

/// TO START TESTING CHANGE TESTING DEFINE IN CLIENT.h TO TRUE
#define PORT 2080


int main(int argc, char **argv)
{
    Generator a;
    std::cout << a.GenerateRequest();
    Client cl_local;
    cl_local.ClientInit(PORT);
    if (TESTING) {
        env = MyTestEnvironment(&cl_local);
        ::testing::InitGoogleTest(&argc, argv);
        return RUN_ALL_TESTS();
    }
    return 0;
}