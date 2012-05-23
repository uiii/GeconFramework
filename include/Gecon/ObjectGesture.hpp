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

#ifndef GECON_OBJECTGESTURE_HPP
#define GECON_OBJECTGESTURE_HPP

#include <set>
#include <string>

namespace Gecon
{
    template< typename Object >
    class ObjectGesture
    {
    public:
        class Event
        {
        };

        typedef std::set<Event*> Events;

        virtual ~ObjectGesture() {}
    };

    template< typename Object, typename PropertyType >
    using ObjectProperty = PropertyType(Object::*)() const;

    template< typename Object, typename PropertyType >
    bool needVisible(ObjectProperty<Object, PropertyType>)
    {
        return true;
    }

    template< typename Object >
    bool needVisible(ObjectProperty<Object, bool> property)
    {
        return property != &Object::isVisible;
    }
} // namespace Gecon

#endif // GECON_OBJECTGESTURE_HPP
