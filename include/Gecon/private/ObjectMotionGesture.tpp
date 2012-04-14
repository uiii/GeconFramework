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
    ObjectMotionGesture<Object>::ObjectMotionGesture(Object *object, const Motion& motion):
        object_(object),
        motion_(motion),
        timeout_(std::chrono::milliseconds(1000))
    {
        normalize_(motion_, getSize_(motion_));
        motionToMoves_(motion_, moves_);
    }

    template< typename Object >
    typename ObjectMotionGesture<Object>::ObjectSet ObjectMotionGesture<Object>::objects() const
    {
        return { object_ };
    }

    template< typename Object >
    const typename ObjectMotionGesture<Object>::Event& ObjectMotionGesture<Object>::motionDoneEvent() const
    {
        return motionDoneEvent_;
    }

    template< typename Object >
    void ObjectMotionGesture<Object>::check()
    {
        Time now = std::chrono::system_clock::now();
        if(! recordedMotion_.empty() && now - lastRecordedMotionTime_ > timeout_)
        {
            std::cout << "timeout" << std::endl;
            Size motionSize = getSize_(recordedMotion_);

            if(std::max(motionSize.width, motionSize.height) > 20) // TODO magic
            {
                std::cout << "check motion" << std::endl;
                normalize_(recordedMotion_, motionSize);
                checkMotion_(recordedMotion_);
            }

            recordedMotion_.clear();
        }

        if(object_->isVisible())
        {
            if(recordedMotion_.empty() || object_->position().distance(recordedMotion_.back()) > 20) // TODO magic
            {
                recordedMotion_.push_back(object_->position());
                lastRecordedMotionTime_ = now;
            }
        }
    }

    template< typename Object >
    bool ObjectMotionGesture<Object>::needCheck() const
    {
        return ! recordedMotion_.empty();
    }

    template< typename Object >
    void ObjectMotionGesture<Object>::checkMotion_(const Motion &motion)
    {
        std::cout << "check motion base" << std::endl;
        MoveSequence moves;
        motionToMoves_(motion, moves);
        if(distance_(moves_, moves) < 20) // TODO magic
        {
            motionDoneEvent_.raise(*this);
        }
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
            ratio = 100.0 / size.height; // TODO magic
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
            }
        }
    }

    template< typename Object >
    std::size_t ObjectMotionGesture<Object>::distance_(const ObjectMotionGesture<Object>::MoveSequence& left, const ObjectMotionGesture<Object>::MoveSequence& right)
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

        typename MoveSequence::const_iterator rowIt;
        typename MoveSequence::const_iterator colIt;
        for(rowIt = right.begin(), row = 1; rowIt != right.end(); ++rowIt, ++row)
        {
            for(colIt = left.begin(), col = 1; colIt != left.end(); ++colIt, ++col)
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

    /*template< typename Object >
    typename ObjectMotionGesture<Object>::Ptr makeObjectMotionGesture(Object* object, const Motion& motion)
    {
        return std::make_shared<ObjectMotionGesture<Object>>(object, motion);
    }*/
} // namespace Gecon
