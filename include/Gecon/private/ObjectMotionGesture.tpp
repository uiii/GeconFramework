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

#include "../ObjectMotionGesture.hpp"

#include <cmath>

#include "math_constants.hpp"

namespace Gecon
{
    template< typename Object > config_variable<std::size_t> ObjectMotionGesture<Object>::MOVE_SEGMENT_LENGTH = 3;

    template< typename Object >
    ObjectMotionGesture<Object>::ObjectMotionGesture(Object *object, const Motion& motion):
        object_(object),
        motion_(motion)
    {
        originalSize_ = getSize_(motion_);
        normalize_(motion_, originalSize_);
        motionToMoves_(motion_, moves_);
    }

    template< typename Object >
    ObjectMotionGesture<Object>::~ObjectMotionGesture()
    {
    }

    template< typename Object >
    ObjectMotionGesture<Object>& ObjectMotionGesture<Object>::operator=(const ObjectMotionGesture<Object>& another)
    {
        originalSize_ = another.originalSize_;
        object_ = another.object_;
        motion_ = another.motion_;

        normalize_(motion_, originalSize_);
        motionToMoves_(motion_, moves_);

        return *this;
    }

    template< typename Object >
    const typename ObjectMotionGesture<Object>::MoveSequence& ObjectMotionGesture<Object>::moves() const
    {
        return moves_;
    }

    template< typename Object >
    typename ObjectMotionGesture<Object>::Event* ObjectMotionGesture<Object>::motionDoneEvent()
    {
        return &motionDoneEvent_;
    }

    template< typename Object >
    Object* ObjectMotionGesture<Object>::object() const
    {
        return object_;
    }

    template< typename Object >
    typename ObjectMotionGesture<Object>::Size ObjectMotionGesture<Object>::getSize_(const Motion& motion)
    {
        if(motion.empty())
        {
            return { 0.0, 0.0 };
        }

        double minX = motion.front().x;
        double maxX = motion.front().x;
        double minY = motion.front().y;
        double maxY = motion.front().y;

        for(const Point& position : motion)
        {
            if(position.x < minX)
            {
                minX = position.x;
            }
            else if(position.x > maxX)
            {
                maxX = position.x;
            }

            if(position.y < minY)
            {
                minY = position.y;
            }
            else if(position.y > maxY)
            {
                maxY = position.y;
            }
        }

        double motionWidth = maxX - minX;
        double motionHeight = maxY - minY;

        return { motionWidth, motionHeight };
    }

    template< typename Object >
    void ObjectMotionGesture<Object>::normalize_(ObjectMotionGesture<Object>::Motion& motion, const Size& size)
    {
        double ratio = 1.0;
        if(size.height > size.width)
        {
            ratio = 100.0 / size.height;
        }
        else
        {
            ratio = 100.0 / size.width;
        }

        for(Point& position : motion)
        {
            position.x = ratio * position.x;
            position.y = ratio * position.y;
        }
    }

    template< typename Object >
    void ObjectMotionGesture<Object>::motionToMoves_(const ObjectMotionGesture<Object>::Motion& motion, ObjectMotionGesture<Object>::MoveSequence& moves)
    {
        moves.clear();

        if(motion.empty())
        {
            return;
        }

        struct Segment
        {
            typename Motion::const_iterator begin;
            typename Motion::const_iterator end;
        };

        struct Circle
        {
            Point center;
            double radius;
        };

        typedef Point Vector;

        auto isInsideCircle = [](const Circle& circle, const Point& point) -> bool
        {
            return std::pow(point.x - circle.center.x, 2) + std::pow(point.y - circle.center.y, 2) <= std::pow(circle.radius, 2);
        };

        auto getCircleSegmentIntersection = [](const Circle& circle, const Segment& segment) -> Point
        {
            Vector segmentVector = { segment.end->x - segment.begin->x, segment.end->y - segment.begin->y };
            Vector centerBeginVector = { segment.begin->x - circle.center.x, segment.begin->y - circle.center.y };

            double a = std::pow(segmentVector.x, 2) + std::pow(segmentVector.y, 2);
            double b = 2 * (centerBeginVector.x * segmentVector.x + centerBeginVector.y * segmentVector.y);
            double c = std::pow(centerBeginVector.x, 2) + std::pow(centerBeginVector.y, 2) - std::pow(circle.radius, 2);

            double discriminant = std::pow(b, 2) - 4 * a * c;

            // The bigger result (multiple) is always needed, because
            // intersection is closer to the end of the segment.
            // So there is no need to count (-b - std::sqrt(discriminant)) / (2 * a),
            // 'a' is always positive and (-b - std::sqrt(discriminant))
            // is less than (-b + std::sqrt(discriminant)).
            double multiple = (-b + std::sqrt(discriminant)) / (2 * a);

            return { segment.begin->x + multiple * segmentVector.x, segment.begin->y + multiple * segmentVector.y };
        };

        auto getVectorAngle = [](const Vector& vector) -> double
        {
            if(vector.y <= 0)
            {
                return std::acos(vector.x) * 180 / PI;
            }
            else
            {
                return std::acos(-vector.x) * 180 / PI + 180;
            }
        };

        Segment currentSegment = {
            motion.begin(),
            std::next(motion.begin(), 1),
        };

        Circle currentCircle = { motion.front(), MOVE_SEGMENT_LENGTH };

        while(currentSegment.end != motion.end())
        {
            if(isInsideCircle(currentCircle, *currentSegment.end))
            {
                // move segment
                ++currentSegment.begin;
                ++currentSegment.end;
            }
            else
            {
                Point intersection = getCircleSegmentIntersection(currentCircle, currentSegment);

                //std::cout << ">>> intersection: " << intersection.x << " " << intersection.y << std::endl;

                Vector moveVector = {
                    (intersection.x - currentCircle.center.x) / currentCircle.radius,
                    (intersection.y - currentCircle.center.y) / currentCircle.radius
                };

                //std::cout << ">>> moveVector: " << moveVector.x << " " << moveVector.y << std::endl;
                //std::cout << ">>> angle: " << getVectorAngle(moveVector) << std::endl;

                std::size_t move = int(std::round(getVectorAngle(moveVector) / 45.0)) % 8;

                //std::cout << ">>> move: " << move << std::endl;
                moves.push_back(move);

                // move circle
                //currentCircle.center = intersection; // TODO neni pravda - musim posunout ve smeru move (ale mozna ne)
                currentCircle.center = Point(
                    currentCircle.center.x + std::cos(move * PI/4.0) * currentCircle.radius,
                    currentCircle.center.y - std::sin(move * PI/4.0) * currentCircle.radius
                );
            }
        }
    }

    template< typename Object >
    typename ObjectMotionGesture<Object>::Size ObjectMotionGesture<Object>::originalSize() const
    {
        return originalSize_;
    }
} // namespace Gecon
