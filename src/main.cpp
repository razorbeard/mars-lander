#include "application.hpp"

#include <stdexcept>
#include <iostream>

int main()
{
    try
    {
        Application app;
        app.run(); 
    }
    catch (const std::exception& e)
    {
        std::cout << "\nEXCEPTION: " << e.what() << std::endl;
    }

    return 0;
}
