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

#ifndef GECON_GESTURESTATECONDITION_HPP
#define GECON_GESTURESTATECONDITION_HPP

#include "GestureCondition.hpp"

#include <functional>

namespace Gecon
{
    template< typename Object, typename PropertyType >
    class GestureStateCondition : public GestureCondition<Object>
    {
    public:
        typedef std::function<PropertyType(const Object&)> Property;
        typedef std::function<bool(const PropertyType&)> Condition;

        GestureStateCondition(Object* object, Property property, Condition condition);

        ObjectList objects() const;

        bool check() const;

    private:
        Object* object_;
        Property property_;
        Condition condition_;
    };
} // namespace Gecon

#include "GestureStateCondition.tpp"

#endif // GECON_GESTURESTATECONDITION_HPP
