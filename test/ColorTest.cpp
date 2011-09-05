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

#include "tut_extended.hpp"

#include "Color.hpp"

namespace tut
{
    struct ColorTestData
    {
    };

    typedef test_group<ColorTestData> ColorTestGroup;
    typedef ColorTestGroup::object ColorTest;
    ColorTestGroup colorTestGroup("Color test group");

    template<>
    template<>
    void ColorTest::test<1>()
    {
        set_test_name("Color construction");

        Gecon::Color color;
        ensure_equals("empty color red component", color.red(), 0);
        ensure_equals("empty color green component", color.green(), 0);
        ensure_equals("empty color blue component", color.blue(), 0);

        Gecon::Color color1(10,100,255);
        ensure_equals("non-empty color red component", color1.red(), 10);
        ensure_equals("non-empty color green component", color1.green(), 100);
        ensure_equals("non-empty color blue component", color1.blue(), 255);
    }

    template<>
    template<>
    void ColorTest::test<2>()
    {
        set_test_name("Color comparison");

        Gecon::Color color1(10,20,30);
        Gecon::Color color2(40,50,60);

        ensure("different colors are not equal", color1 != color2);

        Gecon::Color color3(10,20,30);
        Gecon::Color color4(10,20,30);
        ensure("same colors are equal", color3 == color4);
    }
}
