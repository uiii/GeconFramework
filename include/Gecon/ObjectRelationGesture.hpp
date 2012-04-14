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

#ifndef GECON_OBJECTRELATIONGESTURE_HPP
#define GECON_OBJECTRELATIONGESTURE_HPP

#include "ObjectGesture.hpp"
#include "ObjectGestureEvent.hpp"

#include <functional>

namespace Gecon
{
    template< typename Object, typename PropertyType >
    class ObjectRelationGesture : public ObjectGesture<Object>
    {
    public:
        typedef std::function<PropertyType(const Object&)> Property;
        typedef std::function<bool(const PropertyType&, const PropertyType&)> Condition;

        typedef ObjectGestureEvent<ObjectRelationGesture<Object, PropertyType> > Event;

        ObjectRelationGesture(
                Object* left,
                Object* right,
                Property leftProperty,
                Property rightProperty,
                Condition condition,
                const std::string& description = ""
        );

        const Event& relationEnterEvent() const;
        const Event& relationLeaveEvent() const;

        ObjectSet objects() const;

        void check();
        bool needCheck() const;

    private:
        Object* left_;
        Object* right_;

        Property leftProperty_;
        Property rightProperty_;
        Condition condition_;

        Event relationEnterEvent_;
        Event relationLeaveEvent_;

    public:
        typedef std::shared_ptr<ObjectRelationGesture<Object> > Ptr;
    };

    template< typename Object, typename PropertyType >
    typename ObjectRelationGesture<Object, PropertyType>::Ptr makeObjectRelationGesture(
            Object* left,
            Object* right,
            typename ObjectRelationGesture<Object, PropertyType>::Property leftProperty,
            typename ObjectRelationGesture<Object, PropertyType>::Property rightProperty,
            typename ObjectRelationGesture<Object, PropertyType>::Condition condition
    );
} // namespace Gecon

#include "private/ObjectRelationGesture.tpp"

#endif // GECON_OBJECTRELATIONGESTURE_HPP
