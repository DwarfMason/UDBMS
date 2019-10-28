#include <iostream>
#include "Client.h"
#include <gtest/gtest.h>


#define PORT 2078


int main()
{
    Client cl_local;
    cl_local.ClientInit(PORT);
   // RUN_ALL_TESTS();
    return 0;
}