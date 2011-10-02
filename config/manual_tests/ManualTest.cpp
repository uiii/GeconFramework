#include "ManualTest.hpp"

ManualTest::ManualTest(const std::string& name):
    name_(name)
{
}

ManualTest::~ManualTest()
{
}

const std::string & ManualTest::name() const
{
    return name_;
}
