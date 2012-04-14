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

#ifndef GECON_OBJECTSTATEGESTURE_HPP
#define GECON_OBJECTSTATEGESTURE_HPP

#include "ObjectGesture.hpp"
#include "ObjectGestureEvent.hpp"

#include <functional>

namespace Gecon
{
    template< typename Object, typename PropertyType >
    class ObjectStateGesture : public ObjectGesture<Object>
    {
    public:
        typedef PropertyType (Object::*Property)() const;
        typedef std::function<bool(const PropertyType&)> Condition;

        typedef typename ObjectGesture<Object>::ObjectSet ObjectSet;

        typedef ObjectGestureEvent<ObjectStateGesture<Object, PropertyType> > Event;

        ObjectStateGesture();
        ObjectStateGesture(Object* object, Property property, Condition condition);

        const Object& objectState() const;

        const Event& stateEnterEvent() const;
        const Event& stateLeaveEvent() const;

        ObjectSet objects() const;

        void check();
        bool needCheck() const;

    private:
        Object* object_;
        Property property_;
        Condition condition_;

        bool inState_;
        Object objectState_;

        Event stateEnterEvent_;
        Event stateLeaveEvent_;

    public:
        typedef std::shared_ptr<ObjectStateGesture> Ptr;
    };

    template< typename Object, typename PropertyType >
    typename ObjectStateGesture<Object, PropertyType>::Ptr makeObjectStateGesture(
            Object* object,
            PropertyType (Object::*property)() const,
            std::function<bool(const PropertyType&)> condition
    );
} // namespace Gecon

#include "private/ObjectStateGesture.tpp"

#endif // GECON_OBJECTSTATEGESTURE_HPP
