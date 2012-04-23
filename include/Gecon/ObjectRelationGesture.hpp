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
#include "Event.hpp"

#include <functional>

namespace Gecon
{
    template< typename Object >
    class ObjectRelationGesture : public ObjectGesture<Object>
    {
    public:
        template< typename PropertyType >
        using Property = ObjectProperty<Object, PropertyType>;

        template< typename PropertyType >
        using Relation = std::function<bool(const PropertyType&, const PropertyType&)>;

        typedef std::function<bool(const Object&, const Object&)> Condition;

        typedef typename ObjectGesture<Object>::ObjectSet ObjectSet;

        template< typename PropertyType >
        ObjectRelationGesture(
                Object* left,
                Property<PropertyType> leftProperty,
                Relation<PropertyType> relation,
                Object* right,
                Property<PropertyType> rightProperty
        );

        Event* relationEnterEvent();
        Event* relationLeaveEvent();

        ObjectSet objects() const;

        void check();
        bool needCheck() const;

    private:
        Object* left_;
        Object* right_;

        Object leftState_;
        Object rightState_;

        bool leftMustBeVisible_;
        bool rightMustBeVisible_;

        Condition condition_;

        bool inRelation_;

        Event relationEnterEvent_;
        Event relationLeaveEvent_;
    };
} // namespace Gecon

#include "private/ObjectRelationGesture.tpp"

#endif // GECON_OBJECTRELATIONGESTURE_HPP
