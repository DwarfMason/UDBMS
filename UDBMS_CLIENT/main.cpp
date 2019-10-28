//#include "Client.h"
#include "tests.cpp"
#include <gtest/gtest.h>

/// TO START TESTING CHANGE TESTING DEFINE IN CLIENT.h TO TRUE
#define PORT 2078


int main()
{

    Client cl_local;
    cl_local.ClientInit(PORT);
    if (TESTING) {
        env = MyTestEnvironment(&cl_local);
        RUN_ALL_TESTS();
    }
    return 0;
}