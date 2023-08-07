#include "key_generator.h"
#include "Server.h"
#include <time.h>

int main()
{
    try
    {
        Server mainServer;
        mainServer.serve();
    }
    catch (std::exception& e)
    {
        std::cout << "Error occured: " << e.what() << std::endl;
    }
    return 0;
}