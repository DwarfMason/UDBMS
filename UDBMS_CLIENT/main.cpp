//#include "Client.h"
#include "tests.cpp"
#include <gtest/gtest.h>

/// TO START TESTING CHANGE TESTING DEFINE IN CLIENT.h TO TRUE
#define PORT 2081


int main(int argc, char **argv)
{
    Client cl_local;
    cl_local.ClientInit(PORT);
    if (TESTING) {
        env = MyTestEnvironment(&cl_local);
        ::testing::InitGoogleTest(&argc, argv);
        return RUN_ALL_TESTS();
    }
    return 0;
}