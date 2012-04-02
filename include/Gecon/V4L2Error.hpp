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

#ifndef V4L2ERROR_HPP
#define V4L2ERROR_HPP

#include <string>
#include <sstream>
#include <system_error>
#include <stdexcept>

#include <boost/filesystem.hpp>
#include <boost/format.hpp>

namespace fs = boost::filesystem;

namespace Gecon
{
    enum class v4l2_error
    {
        ok = 0,
        invalid_device,
        no_video_capture
    };

    class v4l2_category_type : public std::error_category
    {
    public:
        virtual const char* name() const;
        virtual std::string message(int code) const;
    };

    const v4l2_category_type& v4l2_category();

    /*std::error_code make_error_code(v4l2_error err);
    std::error_code make_error_code_from_errno();

    std::runtime_error v4l2_device_error(const fs::path& device, const std::string& what, const std::string& why = "");
    std::system_error v4l2_device_error(const fs::path& device, const std::string& what, std::error_code why);*/

    std::string errno_message();

    class v4l2_device_error : public std::runtime_error
    {
    public:
        v4l2_device_error(const fs::path& file, const std::string& message, const std::string& error_message = "", const std::string& delimiter = ": ");
        virtual ~v4l2_device_error() throw();

        virtual const fs::path& device() const throw();
        virtual const char* what() const throw();
        virtual void raise() const throw();

    private:
        fs::path device_;
        std::string what_;
    };

    class v4l2_unsupported_requirement : public v4l2_device_error
    {
    public:
        v4l2_unsupported_requirement(const fs::path& file, const std::string& requirement);

        virtual void raise() const throw();
    };

    class v4l2_invalid_request : public v4l2_device_error
    {
    public:
        v4l2_invalid_request(const fs::path& file);

        virtual void raise() const throw();
    };

    template< typename ExceptionType >
    class v4l2_capture_crashed : public std::runtime_error
    {
    public:
        v4l2_capture_crashed(const ExceptionType& why, const std::string& additional_info = ""):
            std::runtime_error(""),
            what_("video capture has crashed: "),
            info_(additional_info),
            why_(why)
        {
        }
        virtual ~v4l2_capture_crashed() throw() {}

        virtual const char* what() const throw()
        {
            std::stringstream ss;
            ss << what_ << why_.what() << "\n" << info_;
            return ss.str().c_str();
        }

    private:
        std::string what_;
        std::string info_;
        ExceptionType why_;
    };
}

#endif // V4L2ERROR_HPP
