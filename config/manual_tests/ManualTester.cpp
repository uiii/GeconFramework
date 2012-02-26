#include "ManualTester.hpp"

#include <iostream>

int ManualTester::argc = 0;
char** ManualTester::argv = NULL;

ManualTester::~ManualTester()
{
    std::for_each(testSuiteList_.begin(), testSuiteList_.end(), [](ManualTestSuite* suite){ delete suite; });
}

void ManualTester::registerTestSuite(ManualTestSuite *test)
{
    ManualTester* tester = getInstance_();

    tester->testSuiteList_.push_back(test);
}

void ManualTester::runTests()
{
    ManualTester* tester = getInstance_();

    if(tester->testSuiteList_.empty())
    {
        std::cout << "Nothing to test." << std::endl;
        return;
    }

    for(auto it = tester->testSuiteList_.begin(); it != tester->testSuiteList_.end(); ++it)
    {
        std::cout << "[" << (it - tester->testSuiteList_.begin()) + 1 << "] " << (*it)->name() << std::endl;
    }

    std::cout << "\n[0] Quit" << std::endl;
    std::cout << "\nWhat do you want to test?: ";

    int choice;
    std::cin >> choice;

    if(! choice)
    {
        return;
    }

    std::cout << "--------------------------------------------------" << std::endl;

    tester->testSuiteList_.at(choice - 1)->run();
}

ManualTester::ManualTester()
{
}

ManualTester * ManualTester::getInstance_()
{
    static ManualTester tester;
    return &tester;
}
