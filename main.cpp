

#include <fstream>
#include <iostream>
#include <string>

#include "src/Program/Program.hpp"
#include "src/core/RecurringManager/RecurringManager.hpp"
int main()
{
    RecurringManager recurringManager;
    recurringManager.processRecurring();
    try
    {
        Program program;
        program.run();
    }
    catch (std::exception &e)
    {
        std::cout << "EXCEPTION bug: " << e.what() << '\n';
    }

    return 0;
}
