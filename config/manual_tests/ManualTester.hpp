#ifndef MANUALTESTER_HPP
#define MANUALTESTER_HPP

#include <vector>

#include "ManualTestSuite.hpp"

class ManualTester
{
public:
    virtual ~ManualTester();

    static void registerTestSuite(ManualTestSuite* test);

    static void runTests();

    static int argc;
    static char** argv;

private:
    ManualTester();

    static ManualTester* getInstance_();

    std::vector<ManualTestSuite*> testSuiteList_;
};

#endif // MANUALTESTER_HPP
