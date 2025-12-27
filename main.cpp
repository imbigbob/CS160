

#include <fstream>
#include <iostream>
#include <string>

#include "src/Program/Program.hpp"
#include "src/core/RecurringManager/RecurringManager.hpp"
int main()
{
    // try
    // {
    //     Program program;
    //     program.run();
    // }
    // catch (std::exception &e)
    // {
    //     std::cout << "EXCEPTION bug: " << e.what() << '\n';
    // }
    RecurringManager recurringManager;
    recurringManager.processRecurring();

    return 0;
}
