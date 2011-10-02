#ifndef MANUALTESTER_HPP
#define MANUALTESTER_HPP

#include <vector>

#include "ManualTest.hpp"

class ManualTester
{
public:
    static void registerTest(ManualTest* test);

    static void runTests();

private:
    ManualTester();

    static ManualTester* getInstance_();

    std::vector<ManualTest*> testList_;
};

#endif // MANUALTESTER_HPP
