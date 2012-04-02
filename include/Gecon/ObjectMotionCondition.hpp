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

#ifndef GECON_OBJECTMOTIONCONDITION_HPP
#define GECON_OBJECTMOTIONCONDITION_HPP

#include "ObjectGesture.hpp"
#include "ObjectGestureEvent.hpp"

#include <list>
#include <chrono>

namespace Gecon
{
    // TODO sdilene uloziste pro pohyby objektu aby se nemuselo pocitat nekolikrat
    // - preda se jako odkaz v konstruktoru

    template< typename Object >
    class ObjectMotionCondition : public ObjectGesture<Object>
    {
    public:
        struct Point
        {
            double x;
            double y;
        };

        typedef std::list<Point> PointList;
        typedef PointList Motion;
        typedef std::list<std::size_t> MoveSequence;

        typedef ObjectGestureEvent<ObjectMotionCondition<Object> > Event;

        ObjectMotionCondition(Object* object, const Motion& motion, const std::string& description = "");

        ObjectSet objects() const;
        const std::string& description() const;

        const Event& motionDoneEvent() const;

        bool check() const;

    private:
        typedef std::chrono::system_clock::time_point Time;
        typedef std::chrono::system_clock::duration Duration;

        void normalize_(Motion& motion);
        void motionToMoves_(const Motion& motion, MoveSequence& moves);
        std::size_t distance_(const Motion& left, const Motion& right);

        Object* object_;

        std::string description_;

        Motion motion_;
        MoveSequence moves_;

        Motion recordedMotion_;
        Time lastRecordedMotionTime_;

        Duration timeout_;

        Event motionDoneEvent_;

    public:
        typedef std::shared_ptr<ObjectMotionCondition<Object> > Ptr;
    };

    template< typename Object >
    typename ObjectMotionCondition<Object>::Ptr makeGestureMotionCondition(Object* object, const Motion& motion);
} // namespace Gecon

#include "ObjectMotionCondition.tpp"

#endif // GECON_OBJECTMOTIONCONDITION_HPP
