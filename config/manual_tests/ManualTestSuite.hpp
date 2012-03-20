#ifndef MANUALTESTSUITE_HPP
#define MANUALTESTSUITE_HPP

#include <string>
#include <map>
#include <functional>

class ManualTestSuite
{
public:

    ManualTestSuite(const std::string& name);
    virtual ~ManualTestSuite();

    virtual const std::string& name() const;

    virtual void run();

protected:
    typedef std::function<void()> GenericTestPtr;

    virtual void addTest(const std::string& testName, GenericTestPtr test);

private:

    std::string name_;
    std::map<std::string, GenericTestPtr> tests_;
};

#endif // MANUALTESTSUITE_HPP
