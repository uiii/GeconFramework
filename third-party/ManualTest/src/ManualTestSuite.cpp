#include "ManualTestSuite.hpp"

#include <iostream>
#include <vector>

ManualTestSuite::ManualTestSuite(const std::string& name):
    name_(name)
{
}

ManualTestSuite::~ManualTestSuite()
{
}

const std::string & ManualTestSuite::name() const
{
    return name_;
}

void ManualTestSuite::addTest(const std::string& testName, ManualTestSuite::GenericTestPtr test)
{
    tests_[testName] = test;
}

void ManualTestSuite::run()
{
    if(tests_.empty())
    {
        std::cout << "Nothing to test" << std::endl;
        return;
    }

    std::vector<GenericTestPtr> testList;
    int i = 0;
    for(auto it = tests_.begin(); it != tests_.end(); ++it, ++i)
    {
        std::cout << "[" << i + 1 << "] " << it->first << std::endl;
        testList.push_back(it->second);
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

    testList[choice - 1]();
}
