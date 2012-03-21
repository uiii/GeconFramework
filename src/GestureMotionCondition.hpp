/**
 * This file is part of the Gecon Framework project (https://github.com/uiii/Gecon-Framework).
 *
 * Copyright (C) 2011 by Richard Jedlička <uiii.dev@gmail.com>
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

#ifndef GECON_GESTUREMOTIONCONDITION_HPP
#define GECON_GESTUREMOTIONCONDITION_HPP

#include "GestureCondition.hpp"

#include <list>
#include <chrono>

namespace Gecon
{
    // TODO sdilene uloziste pro pohyby objektu aby se nemuselo pocitat nekolikrat
    // - preda se jako odkaz v konstruktoru

    template< typename Object >
    class GestureMotionCondition : public GestureCondition<Object>
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

        GestureMotionCondition(Object* object, const Motion& motion);

        ObjectList objects() const;

        bool check() const;

    private:
        typedef std::chrono::system_clock::time_point Time;
        typedef std::chrono::system_clock::duration Duration;

        void normalize_(Motion& motion);
        void motionToMoves(const Motion& motion, MoveSequence& moves);
        std::size_t distance_(const Motion& left, const Motion& right);

        Object* object_;
        Motion motion_;
        MoveSequence moves_;

        Motion recordedMotion_;
        Time lastRecordedMotionTime_;

        Duration timeout_;
    };
} // namespace Gecon

#include "GestureMotionCondition.tpp"

#endif // GECON_GESTUREMOTIONCONDITION_HPP
