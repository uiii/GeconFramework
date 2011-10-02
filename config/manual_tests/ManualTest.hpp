#ifndef MANUALTEST_HPP
#define MANUALTEST_HPP

#include <string>

class ManualTest
{
public:
    ManualTest(const std::string& name);
    virtual ~ManualTest();

    virtual const std::string& name() const;

    virtual void run() = 0;

private:
    std::string name_;
};

#endif // MANUALTEST_HPP
