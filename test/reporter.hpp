/**
 * This file is part of the Gecon Framework project.
 *
 * Copyright (C) 2011 by Richard Jedlička <uiii.dev@gmail.com>
 *
 * Gecon Framework is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Gecon Framework is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with Gecon Framework. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef TUT_CONSOLE_REPORTER
#define TUT_CONSOLE_REPORTER

#include <tut/tut.hpp>
#include <cassert>

#include <string>

// COLORS

enum Colors
{
    normal,
    red,
    green,
    orange,
    blue,
    magenta,
    cyan,
};

#if defined(WINDOWS)
    std::string color(Colors clr)
    {
        return "";
    }
#elif defined(UNIX)
    std::string color(Colors clr)
    {
        switch(clr)
        {
            case normal: return "\033[22;0m"; break;
            case red: return "\033[22;31m"; break;
            case green: return "\033[22;32m"; break;
            case orange: return "\033[22;33m"; break;
            case blue: return "\033[22;34m"; break;
            case magenta: return "\033[22;35m"; break;
            case cyan: return "\033[22;36m"; break;
            default: return ""; break;
        }
    }
#endif

namespace
{

std::ostream& operator<<(std::ostream& os, const tut::test_result& tr)
{
    os << "[" << tr.test << "] " << tr.name << ": ";
    switch(tr.result)
    {
    case tut::test_result::ok:
        os << color(green) << "OK" << color(normal);
        break;
    case tut::test_result::fail:
        os << color(red) << "FAIL" << color(normal);
        break;
    case tut::test_result::ex_ctor:
        os << color(orange) << "CONSTRUCTOR EXEPTION" << color(normal);
        break;
    case tut::test_result::ex:
        os << color(orange) << "EXCEPTION" << color(normal);
        break;
    case tut::test_result::warn:
        os << color(magenta) << "WARNING" << color(normal);
        break;
    case tut::test_result::term:
        os << color(blue) << "TERMINATED" << color(normal);
        break;
    case tut::test_result::rethrown:
        os << color(cyan) << "RETHROWN" << color(normal);
        break;
    case tut::test_result::dummy:
        assert(!"Should never be called");
    }

    return os;
}

} // end of namespace

namespace tut
{

/**
 * Default TUT callback handler.
 */
class reporter : public tut::callback
{
    std::string current_group;
    typedef std::vector<tut::test_result> not_passed_list;
    not_passed_list not_passed;
    std::ostream& os;

public:

    int ok_count;
    int exceptions_count;
    int failures_count;
    int terminations_count;
    int warnings_count;

    reporter()
        : os(std::cout)
    {
        init();
    }

    reporter(std::ostream& out)
        : os(out)
    {
        init();
    }

    void run_started()
    {
        init();
    }

    void test_completed(const tut::test_result& tr)
    {
        if (tr.group != current_group)
        {
            os << std::endl << tr.group << ": " << std::flush;
            current_group = tr.group;
        }

        os << "\n     " << tr << std::flush;

        // update global statistics
        switch (tr.result) {
            case test_result::ok:
                ok_count++;
                break;
            case test_result::fail:
            case test_result::rethrown:
                failures_count++;
                break;
            case test_result::ex:
            case test_result::ex_ctor:
                exceptions_count++;
                break;
            case test_result::warn:
                warnings_count++;
                break;
            case test_result::term:
                terminations_count++;
                break;
            case tut::test_result::dummy:
                assert(!"Should never be called");
        } // switch

        if (tr.result != tut::test_result::ok)
        {
            not_passed.push_back(tr);
        }
    }

    void run_completed()
    {
        os << std::endl;

        if (not_passed.size() > 0)
        {
            not_passed_list::const_iterator i = not_passed.begin();
            while (i != not_passed.end())
            {
                tut::test_result tr = *i;

                os << std::endl;

                os << "---> " << "group: " << tr.group
                << "\n     test: [" << tr.test << "] " << tr.name
                << std::endl;

#if defined(TUT_USE_POSIX)
                if(tr.pid != getpid())
                {
                    os << "     child pid: " << tr.pid << std::endl;
                }
#endif
                os << "     problem: ";
                switch(tr.result)
                {
                case test_result::rethrown:
                    os << "assertion failed in child" << std::endl;
                    break;
                case test_result::fail:
                    os << "assertion failed" << std::endl;
                    break;
                case test_result::ex:
                case test_result::ex_ctor:
                    os << "unexpected exception" << std::endl;
                    if( tr.exception_typeid != "" )
                    {
                        os << "     exception typeid: "
                        << tr.exception_typeid << std::endl;
                    }
                    break;
                case test_result::term:
                    os << "would be terminated" << std::endl;
                    break;
                case test_result::warn:
                    os << "test passed, but cleanup code (destructor) raised"
                        " an exception" << std::endl;
                    break;
                default:
                    break;
                }

                if (!tr.message.empty())
                {
                    if (tr.result == test_result::fail)
                    {
                        os << "     failed assertion: \"" << tr.message << "\""
                            << std::endl;
                    }
                    else
                    {
                        os << "     message: \"" << tr.message << "\""
                            << std::endl;
                    }
                }

                ++i;
            }
        }

        os << std::endl;

        os << "tests summary:\n";
        os << color(green) << "\n     Ok: " << ok_count << color(normal);
        os << color(red) << "\n     Failures: " << failures_count << color(normal);
        os << color(orange) << "\n     Exceptions: " << exceptions_count << color(normal);
        os << color(magenta) << "\n     Warnings: " << warnings_count << color(normal);
        os << color(blue) << "\n     Terminations: " << terminations_count << color(normal);
        os << std::endl;
    }

    bool all_ok() const
    {
        return not_passed.empty();
    }

private:

    void init()
    {
        ok_count = 0;
        exceptions_count = 0;
        failures_count = 0;
        terminations_count = 0;
        warnings_count = 0;
        not_passed.clear();
    }
};

}

#endif
