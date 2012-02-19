#include "ManualTester.hpp"

#include <iostream>

ManualTester::ManualTester()
{
}

int ManualTester::argc = 0;
char** ManualTester::argv = NULL;

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
        std::cout << "[" << (it - tester->testSuiteList_.begin()) << "] " << (*it)->name() << std::endl;
    }

    std::cout << "\nWhat do you want to test?: ";

    int choice;
    std::cin >> choice;

    std::cout << "--------------------------------------------------" << std::endl;

    tester->testSuiteList_.at(choice)->run();
}

void ManualTester::registerTestSuite(ManualTestSuite *test)
{
    ManualTester* tester = getInstance_();

    tester->testSuiteList_.push_back(test);
}

ManualTester * ManualTester::getInstance_()
{
    static ManualTester tester;
    return &tester;
}
