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

#ifndef GECON_GESTURERELATIONCONDITION_HPP
#define GECON_GESTURERELATIONCONDITION_HPP

#include "GestureCondition.hpp"

#include <functional>

namespace Gecon
{
    template< typename Object, typename PropertyType >
    class GestureRelationCondition : public GestureCondition<Object>
    {
    public:
        typedef std::function<PropertyType(const Object&)> Property;
        typedef std::function<bool(const PropertyType&, const PropertyType&)> Condition;

        GestureRelationCondition(Object* left, Object* right, Property leftProperty, Property rightProperty, Condition condition);

        ObjectList objects() const;

        bool check() const;

    private:
        Object* left_;
        Object* right_;

        Property leftProperty_;
        Property rightProperty_;
        Condition condition_;
    };
} // namespace Gecon

#include "GestureRelationCondition.tpp"

#endif // GECON_GESTURERELATIONCONDITION_HPP
