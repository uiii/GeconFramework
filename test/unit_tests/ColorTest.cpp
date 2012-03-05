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

    typedef std::tuple<unsigned char, unsigned char, unsigned char> compTriple;

    template<>
    template<>
    void ColorTest::test<1>()
    {
        set_test_name("Color construction");

        Gecon::Color<Gecon::RGB> emptyRGB;
        Gecon::Color<Gecon::RGB> nonemptyRGB({10,100,255});

        ensure("empty RGB color",
            compTriple(emptyRGB.r, emptyRGB.g, emptyRGB.b) == compTriple(0, 0, 0));

        ensure("non-empty RGB color",
            compTriple(nonemptyRGB.r, nonemptyRGB.g, nonemptyRGB.b) == compTriple(10, 100, 255));

        Gecon::Color<Gecon::YCbCr> emptyYCbCr;
        Gecon::Color<Gecon::YCbCr> nonemptyYCbCr({10,100,255});

        ensure("empty YCbCr color",
            compTriple(emptyYCbCr.y, emptyYCbCr.cb, emptyYCbCr.cr) == compTriple(0, 128, 128));

        ensure("non-empty YCbCr color",
            compTriple(nonemptyYCbCr.y, nonemptyYCbCr.cb, nonemptyYCbCr.cr) == compTriple(10, 100, 255));
    }

    template<>
    template<>
    void ColorTest::test<2>()
    {
        /*set_test_name("Color comparison");

        Gecon::Color color1(10,20,30);
        Gecon::Color color2(40,50,60);

        ensure("different colors are not equal", color1 != color2);

        Gecon::Color color3(10,20,30);
        Gecon::Color color4(10,20,30);
        ensure("same colors are equal", color3 == color4);*/
    }
}
