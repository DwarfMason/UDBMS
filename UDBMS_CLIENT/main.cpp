//#include "Client.h"
#include "tests.cpp"

#define PORT 2078


int main()
{

    Client cl_local;
    cl_local.ClientInit(PORT);
    if (TESTS) {
        env = MyTestEnvironment(&cl_local);
        RUN_ALL_TESTS();
    }
    return 0;
}