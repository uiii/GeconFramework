#include "ManualTester.hpp"

#include <iostream>

ManualTester::ManualTester()
{
}

void ManualTester::runTests()
{
    ManualTester* tester = getInstance_();

    if(tester->testList_.empty())
    {
        std::cout << "There are no manual test" << std::endl;
        return;
    }

    for(auto it = tester->testList_.begin(); it != tester->testList_.end(); ++it)
    {
        std::cout << "[" << (it - tester->testList_.begin()) << "] " << (*it)->name() << std::endl;
    }

    std::cout << "\nWhat do you want to test?: ";

    int choice;
    std::cin >> choice;

    std::cout << "--------------------------------------------------" << std::endl;

    tester->testList_.at(choice)->run();
}

void ManualTester::registerTest(ManualTest *test)
{
    ManualTester* tester = getInstance_();

    tester->testList_.push_back(test);
}

ManualTester * ManualTester::getInstance_()
{
    static ManualTester tester;
    return &tester;
}
