#ifndef GECON_POINT_HPP
#define GECON_POINT_HPP

namespace Gecon
{
    struct Point // TODO template - int, double, ...
    {
        Point(double x, double y);

        double x;
        double y;
    };

    double distance(const Point& first, const Point& second);
    bool isOver(const Point& first, const Point& second);
    bool isUnder(const Point& first, const Point& second);
    bool isOnTheLeftOf(const Point& first, const Point& second);
    bool isOnTheRightOf(const Point& first, const Point& second);
} // namespace Gecon

#endif // GECON_POINT_HPP
