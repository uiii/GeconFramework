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

#ifndef GECON_OBJECTMOTIONGESTURE_HPP
#define GECON_OBJECTMOTIONGESTURE_HPP

#include "ObjectGesture.hpp"
#include "Point.hpp"

#include <list>
#include <chrono>

#include "config_variable.hpp"

namespace Gecon
{
    template< typename T >
    std::size_t levenshteinDistance(const std::vector<T>& left, const std::vector<T>& right, std::size_t maxDistance);

    template< typename Object >
    class ObjectMotionGesture : public ObjectGesture<Object>
    {
    public:
        static config_variable<std::chrono::milliseconds::rep> MOTION_TIMEOUT;
        static config_variable<std::size_t> MINIMAL_GESTURE_SIDE;
        static config_variable<std::size_t> NOT_MOTION_TOLERANCE;
        static config_variable<std::size_t> MAXIMAL_SAME_GESTURE_DISTANCE;
        static config_variable<std::size_t> MOVE_SEGMENT_LENGTH;

        typedef std::list<Point> PointList;
        typedef PointList Motion;
        typedef std::vector<std::size_t> MoveSequence;

        typedef typename ObjectGesture<Object>::Event Event;
        typedef typename ObjectGesture<Object>::Events Events;
        typedef typename ObjectGesture<Object>::Objects Objects;

        typedef std::chrono::system_clock::time_point Time;

        struct MotionRecord
        {
            Motion motion;
            MoveSequence moves;
            Time lastRecordedMotionTime;
            Time movesGenerationTime;
        };

        typedef std::map<Object*, MotionRecord> MotionStorage;

        ObjectMotionGesture(Object* object, const Motion& motion, MotionStorage* motionStorage);
        virtual ~ObjectMotionGesture();

        ObjectMotionGesture& operator=(const ObjectMotionGesture& another);

        Event* motionDoneEvent();

        Objects objects() const;
        const MoveSequence& moves() const;

        Events check();
        bool needCheck() const;

        void reset();

    protected:
        typedef std::chrono::milliseconds Timeout;

        struct Size
        {
            double width;
            double height;
        };

        void recordMotion_();
        virtual void processRecord_(MotionRecord& record, const Size& size);

        Size getSize_(const Motion& motion);
        void normalize_(Motion& motion, const Size& size);
        void motionToMoves_(const Motion& motion, MoveSequence& moves);

        Object* object_;

        Motion motion_;
        MoveSequence moves_;

        MotionStorage* motionStorage_;

        Timeout timeout_;

        Event motionDoneEvent_;
    };

    template< typename Object >
    std::size_t gestureDistance(ObjectMotionGesture<Object>* first, ObjectMotionGesture<Object>* second, std::size_t maxDistance);
} // namespace Gecon

#include "private/ObjectMotionGesture.tpp"

#endif // GECON_OBJECTMOTIONGESTURE_HPP
