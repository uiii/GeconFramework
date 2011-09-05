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

#include <tut/tut.hpp>

#define ensure_throws(msg, code, exception) \
    try \
    { \
        code; \
        fail(msg); \
    } \
    catch(const exception& e) \
    {}

#define ensure_throws_nothing(msg, code) \
    try \
    { \
        code; \
    } \
    catch(...) \
    { \
        fail(msg); \
    }

namespace tut
{
    template <typename M, typename LHS, typename RHS>
    void ensure_differs(const M& msg, const LHS& actual, const RHS& not_expected)
    {
        if (not_expected == actual)
        {
            std::stringstream ss;
            detail::msg_prefix(ss,msg)
               << "not expected '"
               << not_expected
               << "' actual '"
               << actual
               << '\'';
            throw failure(ss.str());
        }
    }

    template <typename LHS, typename RHS>
    void ensure_differs(const LHS& actual, const RHS& not_expected)
    {
        ensure_differs("Values do not differ", actual, not_expected);
    }
}
