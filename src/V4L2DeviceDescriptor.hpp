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

#ifndef V4L2DEVICEDESCRIPTOR_HPP
#define V4L2DEVICEDESCRIPTOR_HPP

#include <memory>
#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

namespace Gecon
{
    class V4L2FileDescriptor
    {
    public:
        V4L2FileDescriptor(int fd);
        ~V4L2FileDescriptor();

        operator int() const;

    private:
        int fd_;
    };
}

namespace Gecon
{
    class V4L2DeviceDescriptor
    {
    public:
        V4L2DeviceDescriptor();
        V4L2DeviceDescriptor(const fs::path& file);

        operator int() const;
        fs::path file() const;

        void close(); // TODO mozna odstranit

    private:
        std::shared_ptr<V4L2FileDescriptor> fd_;
        fs::path file_;
    };
}

#endif //V4L2DEVICEDESCRIPTOR_HPP
