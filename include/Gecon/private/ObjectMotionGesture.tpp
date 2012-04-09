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

namespace Gecon
{
    template< typename Object >
    ObjectMotionGesture<Object>::ObjectMotionGesture(Gecon::Object *object, const Motion& motion, const std::string& description):
        object_(object),
        description_(description),
        motion_(motion),
        lastRecordedMotionTime_(0),
        timeout_(std::chrono::milliseconds(1000)),
        motionDoneEvent_(object)
    {
    }

    template< typename Object >
    Gesture::ObjectSet ObjectMotionGesture<Object>::objects() const
    {
        //return { object_ }; TODO
    }

    template< typename Object >
    const std::string& ObjectMotionGesture<Object>::description() const
    {
        return description_;
    }

    template< typename Object >
    const Event& ObjectMotionGesture<Object>::motionDoneEvent() const
    {
        return motionDoneEvent_;
    }

    template< typename Object >
    bool ObjectMotionGesture<Object>::check() const
    {
        bool motionDone = false;

        Time now = std::chrono::system_clock::now;
        if(! recordedMotion_.empty() && now - lastRecordedMotionTime_ > timeout_)
        {
            // TODO check motion
            if(distance_(motion_, recordedMotion_) < 20) // TODO magic
            {
                motionDone = true;
                recordedMotion_.clear();
            }
        }

        if(object_.isVisible())
        {
            recordedMotion_.push_back(object_.position());
            lastRecordedMotionTime_ = now;
        }

        return motionDone;
    }

    template< typename Object >
    void ObjectMotionGesture<Object>::normalize_(ObjectMotionGesture<Object>::Motion& motion)
    {
        if(motion.empty())
        {
            return;
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

        double motionHeight = maxY - minY;
        double motionWidth = maxX - minX;

        double ratio = 1.0;
        if(motionHeight > motionWidth)
        {
            ratio = 100.0 / motionHeight; // TODO magic
        }
        else
        {
            ratio = 100.0 / motionWidth;
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
            Motion::const_iterator begin;
            Motion::const_iterator end;
        };

        struct Circle
        {
            Point center;
            double radius;
        };

        typedef Point Vector;

        auto isInsideCircle = [](const Circle& circle, const Point& point)
        {
            return std::pow(point.x - circle.center.x, 2) + std::pow(point.y - circle.center.y, 2) <= std::pow(circle.radius, 2);
        };

        auto getCircleSegmentIntersection = [](const Circle& circle, const Segment& segment) -> Point
        {
            Vector segmentVector = { segment.end.x - segment.begin.x, segment.end.y - segment.begin.y };
            Vector centerBeginVector = { segment.begin.x - circle.center.x, segment.begin.y - circle.center.y };

            double a = std::pow(segmentVector.x, 2) + std::pow(segmentVector.y, 2);
            double b = 2 * (centerBeginVector.x * segmentVector.x + centerBeginVector.y * segmentVector.y);
            double c = std::pow(centerBeginVector.x, 2) + std::pow(centerBeginVector.y, 2) - std::pow(circle.radius, 2);

            double discriminant = std::pow(b, 2) - 4 * a * c;

            double multiple = (-b + std::sqrt(discriminant)) / (2 * a);

            return { segment.begin.x + multiple * segmentVector.x, segment.begin.y + multiple * segmentVector.y };
        };

        auto getVectorAngle = [](const Vector& vector) -> double
        {
            if(vector.y <= 0)
            {
                return std::acos(vector.x) * 180 / PI;
            }
            if(vector.y > 0)
            {
                return std::acos(-vector.x) * 180 / PI + 180;
            }
        };

        Segment currentSegment = {
            motion.begin(),
            std::next(motion.begin(), 1),
        };

        Circle currentCircle = { motion.front(), 10 }; // TODO magic

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

                Vector moveVector = {
                    intersection.x - currentCircle.center.x,
                    intersection.y - currentCircle.center.y
                };

                std::size_t move = std::round(getVectorAngle(moveVector) / 45.0);

                moves.push_back(move);

                // move circle
                currentCircle.center = intersection; // TODO neni pravda - musim posunout ve smeru move (ale mozna ne)
                currentSegment.begin = intersection;
            }
        }
    }

    template< typename Object >
    std::size_t ObjectMotionGesture<Object>::distance_(const ObjectMotionGesture<Object>::Motion& left, const ObjectMotionGesture<Object>::Motion& right)
    {
        std::size_t width = left.size();
        std::size_t height = right.size();

        std::size_t table[width + 1][height + 1];

        std::size_t col = 0;
        std::size_t row = 0;

        for(col = 0; col < width; ++col)
        {
            table[col][0] = col;
        }

        for(row = 0; row < height; ++row)
        {
            table[0][row] = row;
        }

        Motion::const_iterator rowIt;
        Motion::const_iterator colIt;
        for(rowIt = moves2.begin(), row = 1; rowIt != moves2.end(); ++rowIt, ++row)
        {
            for(colIt = moves1.begin(), col = 1; colIt != moves1.end(); ++colIt, ++col)
            {
                if((*rowIt) == (*colIt))
                {
                    table[col][row] = table[col - 1][row - 1];
                }
                else
                {
                    table[col][row] = std::min(
                            table[col - 1][row - 1] + 1,
                            std::min(
                                table[col - 1][row] + 1,
                                table[col][row - 1] + 1
                            )
                    );
                }
            }
        }

        return table[width - 1][height - 1];
    }

    template< typename Object >
    typename ObjectMotionGesture<Object>::Ptr makeGestureMotionCondition(Object* object, const Motion& motion)
    {
        return std::make_shared<ObjectMotionGesture<Object>>(object, motion);
    }
} // namespace Gecon
