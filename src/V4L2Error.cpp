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

#include "V4L2Error.hpp"

#include <sstream>
#include <cerrno>

#include <boost/algorithm/string.hpp>

namespace Gecon
{
    const char* v4l2_category_type::name() const
    {
        return "v4l2";
    }

    std::string v4l2_category_type::message(int code) const
    {
        std::string msg;

        switch(static_cast<v4l2_error>(code))
        {
            case v4l2_error::invalid_device:
                msg = "this is not V4L2 device";
            break;
            case v4l2_error::no_video_capture:
                msg = "this is not video capture device";
            break;
            default:
            break;
        }

        return msg;
    }

    const v4l2_category_type& v4l2_category()
    {
        static v4l2_category_type category;
        return category;
    }

    std::error_code make_error_code(v4l2_error err)
    {
        return std::error_code(static_cast<int>(err), v4l2_category());
    }

    std::error_code make_error_code_from_errno()
    {
        return std::error_code(errno, std::system_category());
    }

    /*std::runtime_error v4l2_device_error(const fs::path& device, const std::string& what, const std::string& why)
    {
        std::stringstream ss;
        ss << "[" << device.c_str() << "] " << what;
        if(! why.empty())
        {
            ss << ":" << why;
        }

        return std::runtime_error(ss.str());
    }

    std::system_error v4l2_device_error(const fs::path& device, const std::string& what, std::error_code why)
    {
        return std::system_error(why, what);
    }*/

    std::string errno_message()
    {
        return std::system_category().message(errno);
    }

    v4l2_device_error::v4l2_device_error(const fs::path &file, const std::string &message, const std::string &error_message, const std::string &delimiter):
        runtime_error(""),
        what_(message)
    {
        boost::replace_first(what_, "%device%", file.string());

        if(! error_message.empty())
        {
            what_.append(delimiter).append(error_message);
        }
    }

    v4l2_device_error::~v4l2_device_error() throw()
    {
    }

    const fs::path &v4l2_device_error::device() const throw()
    {
        return device_;
    }

    const char* v4l2_device_error::what() const throw()
    {
        return what_.c_str();
    }

    void v4l2_device_error::raise() const throw()
    {
        throw *this;
    }

    v4l2_unsupported_requirement::v4l2_unsupported_requirement(const fs::path &file, const std::string &requirement):
        v4l2_device_error(file, "%device% doesn't support", requirement, " ")
    {
    }

    void v4l2_unsupported_requirement::raise() const throw()
    {
        throw *this;
    }

    v4l2_invalid_request::v4l2_invalid_request(const fs::path &file):
        v4l2_device_error(file, "invalid request to %device%")
    {
    }

    void v4l2_invalid_request::raise() const throw()
    {
        throw *this;
    }
}
