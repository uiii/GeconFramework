#include "Point.hpp"

#include <cmath>

#include <iostream>

namespace Gecon
{
    Point::Point(double x_, double y_):
        x(x_), y(y_)
    {
    }

    double distance(const Point& first, const Point& second)
    {
        return std::sqrt(std::pow(first.x - second.x, 2) + std::pow(first.y - second.y, 2));
    }

    bool isOver(const Point& first, const Point& second)
    {
        std::cout << "is over?" << std::endl;
        std::cout << "\t" << first.x << " " << first.y << std::endl;
        std::cout << "\t" << second.x << " " << second.y << std::endl;
        return first.y < second.y;
    }

    bool isUnder(const Point& first, const Point& second)
    {
        return first.y > second.y;
    }

    bool isOnTheLeftOf(const Point& first, const Point& second)
    {
        return first.x < second.x;
    }

    bool isOnTheRightOf(const Point& first, const Point& second)
    {
        return first.x > second.x;
    }
} // namespace Gecon
