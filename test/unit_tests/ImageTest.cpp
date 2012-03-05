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
/*
#include "tut_extended.hpp"

#include <stdexcept>

#include "Image.hpp"

namespace tut
{
    struct ImageTestData
    {
    };

    typedef test_group<ImageTestData> ImageTestGroup;
    typedef ImageTestGroup::object ImageTest;
    ImageTestGroup imageTestGroup("Image test group");

    template<>
    template<>
    void ImageTest::test<1>()
    {
        set_test_name("Image construction");
        
        Gecon::Image img1;
        ensure_equals("empty image width", img1.width(), (std::size_t) 0);
        ensure_equals("empty image height", img1.height(), (std::size_t) 0);
        ensure("empty image data", img1.rawData().empty());

        Gecon::Image img2(0, 0);
        ensure_equals("explicitely empty image width", img2.width(), (std::size_t) 0);
        ensure_equals("explicitely empty image height", img2.height(), (std::size_t) 0);
        ensure("explicitely empty image data", img2.rawData().empty());

        Gecon::Image img3(10, 20);
        ensure_equals("non-empty image width", img3.width(), (std::size_t) 10);
        ensure_equals("non-empty image height", img3.height(), (std::size_t) 20);

        for(auto& i : img3.rawData())
        {
            ensure_equals(i, 0);
        }

        ensure_throws("negative image size", Gecon::Image img(-1, -1), std::length_error);
    }

    template<>
    template<>
    void ImageTest::test<2>()
    {
        set_test_name("Pixel access");

        Gecon::Image img1;
        ensure_throws("empty image read access", img1.at(1, 1), std::out_of_range);
        ensure_throws("empty image write access", img1.setAt(1, 1, Gecon::Color()), std::out_of_range);

        Gecon::Image img2(10,20);
        ensure_throws("non-empty image negative position read access", img2.at(-1, -1), std::out_of_range);
        ensure_throws("non-empty image big position read access", img2.at(11, 21), std::out_of_range);
        ensure_throws("non-empty image negative position write access", img2.setAt(-1, -1, Gecon::Color()), std::out_of_range);
        ensure_throws("non-empty image big position write access", img2.setAt(11, 21, Gecon::Color()), std::out_of_range);

        Gecon::Color color;
        ensure_throws_nothing("non-empty image correct position read access", color = img2.at(5, 15));
        ensure("image's pixels should be of \"empty\" color", color == Gecon::Color());

        Gecon::Color newColor(10, 100, 255);
        ensure_throws_nothing("non-empty image correct position write access", img2.setAt(5, 15, newColor));
        ensure("color of pixel isn't changed", img2.at(5, 15) == newColor);
    }
}*/
