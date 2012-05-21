/**
 * This file is part of the Gecon Framework project (https://github.com/uiii/GeconFramework).
 *
 * Copyright (C) 2012 by Richard Jedlička <uiii.dev@gmail.com>
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

#include "../ColorObject.hpp"

#include <cassert>

#include "math_constants.hpp"

namespace Gecon
{
    template< typename ColorSpace >
    ColorObject<ColorSpace>::ColorObject(Color color):
        color_(color),
        maxPosition_(1,1),
        isVisible_(false)
    {
    }

    template< typename ColorSpace >
    typename ColorObject<ColorSpace>::Color ColorObject<ColorSpace>::color() const
    {
        return color_;
    }

    template< typename ColorSpace >
    void ColorObject<ColorSpace>::setColor(ColorObject<ColorSpace>::Color color)
    {
        color_ = color;
    }

    template< typename ColorSpace >
    void ColorObject<ColorSpace>::update(ColorArea<ColorSpace>* area, Point maxPosition)
    {
        if(area)
        {
            updateConvexHull_(area);
            updateMinimalBoundingBox_(convexHull_);
        }

        maxPosition_ = maxPosition;

        updateTime_ = std::chrono::system_clock::now();
    }

    template< typename ColorSpace >
    typename ColorObject<ColorSpace>::Time ColorObject<ColorSpace>::updateTime()
    {
        return updateTime_;
    }

    template< typename ColorSpace >
    bool ColorObject<ColorSpace>::isVisible() const
    {
        return isVisible_;
    }

    template< typename ColorSpace >
    void ColorObject<ColorSpace>::setVisible(bool visible)
    {
        isVisible_ = visible;
    }

    template< typename ColorSpace >
    Point ColorObject<ColorSpace>::position() const
    {
        return Point(
            boundingBox_.position.x / maxPosition_.x,
            boundingBox_.position.y / maxPosition_.y
        );
    }

    template< typename ColorSpace >
    int ColorObject<ColorSpace>::angle() const
    {
        return boundingBox_.angle;
    }

    template< typename ColorSpace >
    double ColorObject<ColorSpace>::aspectRatio() const
    {
        return boundingBox_.width / boundingBox_.height;
    }

    template< typename ColorSpace >
    Fraction ColorObject<ColorSpace>::areaSize() const
    {
        return Fraction(1, int((maxPosition_.x * maxPosition_.y) / (boundingBox_.width * boundingBox_.height)));
    }

    template< typename ColorSpace >
    const typename ColorObject<ColorSpace>::Border& ColorObject<ColorSpace>::border() const
    {
        return border_;
    }

    template< typename ColorSpace >
    const typename ColorObject<ColorSpace>::BoundingBox& ColorObject<ColorSpace>::boundingBox() const
    {
        return boundingBox_;
    }

    template< typename ColorSpace >
    const typename ColorObject<ColorSpace>::ConvexHull& ColorObject<ColorSpace>::convexHull() const
    {
        return convexHull_;
    }

    template< typename ColorSpace >
    void ColorObject<ColorSpace>::updateConvexHull_(ColorArea<ColorSpace> *area)
    {
        typedef typename ColorArea<ColorSpace>::BlockList BlockList;
        const BlockList& blocks = area->blocks();

        enum struct Orientation
        {
            LEFT,
            RIGHT,
            STRAIGHT
        };

        auto getAngleOrientation = [](const Point& first, const Point& second, const Point& third) -> Orientation
        {
            int determinant = (second.x - first.x) * (third.y - second.y) - (third.x - second.x) * (second.y - first.y);

            if(determinant == 0)
            {
                return Orientation::STRAIGHT;
            }
            else if(determinant < 0)
            {
                return Orientation::LEFT;
            }
            else // determinant > 0
            {
                return Orientation::RIGHT;
            }
        };

        auto addNextPoint = [&getAngleOrientation](const Point& nextPoint, ConvexHull& points, Orientation orientation) -> void
        {
            if(points.size() > 1)
            {
                auto firstPointBack = std::prev(points.end(), 1);
                auto secondPointBack = std::prev(points.end(), 2);

                Orientation angleOrientation = getAngleOrientation(*secondPointBack, *firstPointBack, nextPoint);
                while(firstPointBack != points.begin() && angleOrientation != orientation)
                {
                    if(secondPointBack != points.begin())
                    {
                        --secondPointBack;
                    }

                    --firstPointBack;

                    points.pop_back();

                    if(angleOrientation == Orientation::STRAIGHT)
                    {
                        angleOrientation = orientation;
                    }
                    else
                    {
                        angleOrientation = getAngleOrientation(*secondPointBack, *firstPointBack, nextPoint);
                    }
                }
            }

            points.push_back(nextPoint);
        };

        ConvexHull leftPoints;
        ConvexHull rightPoints;

        border_.clear();

        typename BlockList::const_iterator block = blocks.begin();
        while(block != blocks.end())
        {
            Point leftPoint = {(double)block->begin, (double)block->row};
            Point rightPoint = {(double)block->end, (double)block->row};

            border_.push_front(leftPoint);
            border_.push_back(rightPoint);

            addNextPoint(leftPoint, leftPoints, Orientation::LEFT);
            addNextPoint(rightPoint, rightPoints, Orientation::RIGHT);

            ++block;
        }

        leftPoints.reverse();

        if(rightPoints.front().x == leftPoints.back().x)
        {
            leftPoints.pop_back();
        }

        if(rightPoints.back().x == leftPoints.front().x)
        {
            rightPoints.pop_back();
        }

        convexHull_.clear();
        convexHull_.splice(convexHull_.end(), rightPoints);
        convexHull_.splice(convexHull_.end(), leftPoints);
    }

    template< typename ColorSpace >
    void ColorObject<ColorSpace>::updateMinimalBoundingBox_(const ConvexHull& convexHull)
    {
        // TODO krajni pripady

        typedef Point Vector;
        typedef typename ConvexHull::const_iterator PointIterator;

        // find significant point iterators
        PointIterator top = convexHull.begin();
        PointIterator bottom = convexHull.begin();
        PointIterator left = convexHull.begin();
        PointIterator right = convexHull.begin();

        PointIterator point = convexHull.begin();
        while(point != convexHull.end())
        {
            if(point->y >= bottom->y)
            {
                bottom = point;
            }

            if(point->x <= left->x)
            {
                left = point;
            }

            if(point->x >= right->x)
            {
                right = point;
            }

            ++point;
        }

        struct CaliperLine
        {
            PointIterator rotationPoint;
            Vector line;
        };

        typedef std::vector<CaliperLine> CaliperLineList;

        struct CaliperConfiguration
        {
            double area;
            CaliperLineList caliperLines;
        };

        auto getAdjacentPoint = [&convexHull](const PointIterator& point) -> PointIterator
        {
            PointIterator adjacentPoint = std::next(point, 1);

            if(adjacentPoint == convexHull.end())
            {
                return convexHull.begin();
            }
            else
            {
                return adjacentPoint;
            }
        };

        auto getAngleCosine = [](const Vector& normalizedVector, const Vector& edge) -> double
        {
            double edgeNorm = std::sqrt(std::pow(edge.x, 2) + std::pow(edge.y, 2));
            double dotProduct = ((normalizedVector.x * edge.x) + (normalizedVector.y * edge.y));

            assert(edgeNorm != 0); // TODO remove

            return dotProduct / edgeNorm;
        };

        auto rotate = [](CaliperLine& caliperLine, double angleCosine) -> void
        {
            double angleSineSquare = 1 - std::pow(angleCosine, 2);
            double angleSine = angleSineSquare < 0 ? 0 : std::sqrt(angleSineSquare);

            Vector& oldLine = caliperLine.line;
            Vector newLine = {
                oldLine.x * angleCosine - oldLine.y * angleSine,
                oldLine.x * angleSine + oldLine.y * angleCosine
            };

            double newLineNorm = std::sqrt(std::pow(newLine.x, 2) + std::pow(newLine.y, 2));
            newLine.x = newLine.x / newLineNorm;
            newLine.y = newLine.y / newLineNorm;

            //std::cout << "old: " << oldLine.x << " " << oldLine.y << std::endl;
            //std::cout << "new: " << newLine.x << " " << newLine.y << std::endl;

            caliperLine.line = newLine;
        };

        auto getDistance = [](const CaliperLine& first, const CaliperLine& second) -> double
        {
            PointIterator firstPoint = first.rotationPoint;
            PointIterator secondPoint = second.rotationPoint;

            double multiple = first.line.x * (secondPoint->x - firstPoint->x) + first.line.y * (secondPoint->y - firstPoint->y);
            Point oppositePoint = {
                firstPoint->x + multiple * first.line.x,
                firstPoint->y + multiple * first.line.y,
            };

            //std::cout << "first: " << first.line.x << " " << first.line.y << std::endl;
            //std::cout << "second: " << second.line.x << " " << second.line.y << std::endl;
            //std::cout << multiple << std::endl;
            return std::sqrt(std::pow(secondPoint->x - oppositePoint.x, 2) + std::pow(secondPoint->y - oppositePoint.y, 2));
        };

        CaliperConfiguration configuration = {
            (right->x - left->x) * (bottom->y - top->y),
            {
                {top, {1,0}},
                {right, {0,1}},
                {bottom, {-1,0}},
                {left, {0,-1}}
            }
        };

        CaliperConfiguration minAreaConfiguration = configuration;

        double angleSum = 0;
        while(angleSum < 1)
        {
            // get max angle cosine (= min angle)
            double maxAngleCosine = -1;
            CaliperLine* leadCaliperLinePtr = 0;
            for(CaliperLine& caliperLine : configuration.caliperLines)
            {
                PointIterator adjacentPoint = getAdjacentPoint(caliperLine.rotationPoint);
                Vector incidentEdge = {
                    adjacentPoint->x - caliperLine.rotationPoint->x,
                    adjacentPoint->y - caliperLine.rotationPoint->y,
                };

                double angleCosine = getAngleCosine(caliperLine.line, incidentEdge);

                if(angleCosine > maxAngleCosine)
                {
                    maxAngleCosine = angleCosine;
                    leadCaliperLinePtr = &caliperLine;
                }
            }

            // rotate all caliper lines
            rotate(configuration.caliperLines[0], maxAngleCosine);
            double x = configuration.caliperLines[0].line.x;
            double y = configuration.caliperLines[0].line.y;
            for(CaliperLine& caliperLine : configuration.caliperLines)
            {
                //rotate(caliperLine, maxAngleCosine);
                caliperLine.line.x = x;
                caliperLine.line.y = y;

                std::swap(x, y);
                x = -x;
            }

            // move rotation point of the lead caliper line
            leadCaliperLinePtr->rotationPoint = getAdjacentPoint(leadCaliperLinePtr->rotationPoint);

            // TODO rect area
            double height = getDistance(configuration.caliperLines[0], configuration.caliperLines[2]);
            double width = getDistance(configuration.caliperLines[1], configuration.caliperLines[3]);

            configuration.area = height * width;

            if(configuration.area < minAreaConfiguration.area)
            {
                minAreaConfiguration = configuration;
            }

            angleSum += 1 - maxAngleCosine;
        }

        auto getLineIntersection = [](const CaliperLine& first, const CaliperLine& second) -> Point
        {
            PointIterator firstPoint = first.rotationPoint;
            PointIterator secondPoint = second.rotationPoint;

            double multiple =
                (second.line.x * (firstPoint->y - secondPoint->y) - second.line.y * (firstPoint->x - secondPoint->x))
                / (second.line.y * first.line.x - second.line.x * first.line.y);

            return { firstPoint->x + multiple * first.line.x, firstPoint->y + multiple * first.line.y };
        };

        auto getLineAngle = [](const CaliperLine& caliperLine) -> double
        {
            double x = caliperLine.line.x;
            if(caliperLine.line.y == 0)
            {
                x = 1.0;
            }
            if(caliperLine.line.y > 0)
            {
                x = -x;
            }

            return std::acos(x) * 180 / PI;
        };

        CaliperLineList& caliperLines = minAreaConfiguration.caliperLines;

        double firstSizeDimension = getDistance(caliperLines[0], caliperLines[2]);
        double secondSizeDimension = getDistance(caliperLines[1], caliperLines[3]);

        boundingBox_.width = std::max(firstSizeDimension, secondSizeDimension);
        boundingBox_.height = std::min(firstSizeDimension, secondSizeDimension);

        Point topLeft = getLineIntersection(caliperLines[0], caliperLines[3]);

        boundingBox_.position = {
            topLeft.x + (secondSizeDimension / 2.0) * caliperLines[0].line.x + (firstSizeDimension / 2.0) * caliperLines[1].line.x,
            topLeft.y + (secondSizeDimension / 2.0) * caliperLines[0].line.y + (firstSizeDimension / 2.0) * caliperLines[1].line.y
        };

        boundingBox_.angle = firstSizeDimension > secondSizeDimension ? getLineAngle(caliperLines[1]) : getLineAngle(caliperLines[0]);
    }
} // namespace Gecon
