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

#include <functional>

namespace Gecon
{
    template< typename Object>
    class ObjectStateGesture : public ObjectGesture<Object>
    {
    public:
        template< typename PropertyType >
        using Property = ObjectProperty<Object, PropertyType>;

        template< typename PropertyType >
        using Relation = std::function<bool(const PropertyType&, const PropertyType&)>;

        typedef std::function<bool(const Object&)> Condition;

        typedef typename ObjectGesture<Object>::Event Event;
        typedef typename ObjectGesture<Object>::Events Events;
        typedef typename ObjectGesture<Object>::Objects Objects;

        template< typename PropertyType >
        ObjectStateGesture(Object* object, Property<PropertyType> property, Relation<PropertyType> relation, PropertyType value);

        const Object& objectState() const; // TODO remove

        Event* stateEnterEvent();
        Event* stateLeaveEvent();

        Objects objects() const;

        Events check();
        bool needCheck() const;

        void reset();

    private:
        Object* object_;

        Object objectState_; // TODO remove
        bool mustBeVisible_;

        Condition condition_;

        bool inState_;

        Event stateEnterEvent_;
        Event stateLeaveEvent_;
    };
} // namespace Gecon

#include "private/ObjectStateGesture.tpp"

#endif // GECON_OBJECTSTATEGESTURE_HPP
