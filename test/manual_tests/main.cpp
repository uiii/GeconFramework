/**
 * This file is part of the Gecon Framework project (https://github.com/uiii/Gecon-Framework).
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

#include <iostream>

#include "ManualTester.hpp"

#include "V4L2VideoDevicePolicyTest.hpp"
#include "ColorObjectPolicyTest.hpp"
#include "GesturePolicyTest.hpp"

int main(int argc, char* argv[])
{
    ManualTester::argc = argc;
    ManualTester::argv = argv;

    ManualTester::registerTestSuite(new V4L2VideoDevicePolicyTest);
    ManualTester::registerTestSuite(new ObjectPolicyTest);
    ManualTester::registerTestSuite(new GesturePolicyTest);

    ManualTester::runTests();

    return 0;
}
