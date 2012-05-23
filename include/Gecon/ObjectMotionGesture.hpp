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
    template< typename Object >
    class ObjectMotionGesture : public ObjectGesture<Object>
    {
    public:
        static config_variable<std::size_t> MOVE_SEGMENT_LENGTH;

        typedef std::list<Point> PointList;
        typedef PointList Motion;
        typedef std::vector<std::size_t> MoveSequence;

        typedef typename ObjectGesture<Object>::Event Event;

        struct Size
        {
            double width;
            double height;
        };

        ObjectMotionGesture(Object* object, const Motion& motion);
        virtual ~ObjectMotionGesture();

        ObjectMotionGesture& operator=(const ObjectMotionGesture& another);

        Event* motionDoneEvent();

        Object* object() const;
        const MoveSequence& moves() const;

        Size originalSize() const;

    protected:
        Size getSize_(const Motion& motion);
        void normalize_(Motion& motion, const Size& size);
        void motionToMoves_(const Motion& motion, MoveSequence& moves);

        Object* object_;

        Motion motion_;
        MoveSequence moves_;

        Size originalSize_;

        Event motionDoneEvent_;
    };
} // namespace Gecon

#include "private/ObjectMotionGesture.tpp"

#endif // GECON_OBJECTMOTIONGESTURE_HPP
