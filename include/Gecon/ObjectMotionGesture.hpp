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
#include "ObjectGestureEvent.hpp"

#include <list>
#include <chrono>

namespace Gecon
{
    // TODO sdilene uloziste pro pohyby objektu aby se nemuselo pocitat nekolikrat
    // - preda se jako odkaz v konstruktoru

    template< typename Object >
    class ObjectMotionGesture : public ObjectGesture<Object>
    {
    public:
        typedef typename Object::Point Point;
        typedef std::list<Point> PointList;
        typedef PointList Motion;
        typedef std::list<std::size_t> MoveSequence;

        typedef typename ObjectGesture<Object>::ObjectSet ObjectSet;

        typedef ObjectGestureEvent<ObjectMotionGesture<Object> > Event;

        struct Size
        {
            double width;
            double height;
        };

        ObjectMotionGesture(Object* object, const Motion& motion);

        const Event& motionDoneEvent() const;

        ObjectSet objects() const;

        void check();
        bool needCheck() const;

    protected:
        typedef std::chrono::system_clock::time_point Time;
        typedef std::chrono::milliseconds Timeout;

        virtual void checkMotion_(const Motion &motion);

        Size getSize_(const Motion& motion);
        void normalize_(Motion& motion, const Size& size);
        void motionToMoves_(const Motion& motion, MoveSequence& moves);
        std::size_t distance_(const MoveSequence& left, const MoveSequence& right);

        Object* object_;

        Motion motion_;
        MoveSequence moves_;

        Motion recordedMotion_;
        Time lastRecordedMotionTime_;

        Timeout timeout_;

        Event motionDoneEvent_;

    public:
        typedef std::shared_ptr<ObjectMotionGesture<Object> > Ptr;
    };

    /*template< typename Object >
    typename ObjectMotionGesture<Object>::Ptr makeObjectMotionGesture(Object* object, const Motion& motion);*/
} // namespace Gecon

#include "private/ObjectMotionGesture.tpp"

#endif // GECON_OBJECTMOTIONGESTURE_HPP
