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

#ifndef GECON_V4L2VIDEODEVICECAPTURE_HPP
#define GECON_V4L2VIDEODEVICECAPTURE_HPP

#include <string>
#include <vector>
#include <memory>

#include <boost/filesystem.hpp>
#include <boost/thread.hpp>

#include <linux/videodev2.h>

#include "V4L2Error.hpp"
#include "V4L2DeviceDescriptor.hpp"
#include "Image.hpp"

namespace Gecon
{
    typedef __u32 BufferIndex;

    struct Buffer
    {
        BufferIndex index;
        unsigned char* pointer;
        std::size_t length;
    };

    class V4L2VideoDeviceCapture
    {
    public:
        typedef Image Snapshot; // TODO

        V4L2VideoDeviceCapture(const fs::path& file);

        const fs::path& file() const;

        void start();
        void stop();

        Snapshot getSnapshot();

    private:
        /**
         * Check if device file represents V4L2 video device.
         *
         * @throws std::system_error
         *     or device is not V4L2 video device.
         *     If device cannot be checked
         */
        void checkV4L2VideoDevice_(const fs::path& file) const;

        int openDevice_();
        void initialize_(V4L2DeviceDescriptor device);
        void deinitialize_();
        void startCaptureLoop_();
        void stopCaptureLoop_();

        void captureCrash_();

        void captureLoop_(V4L2DeviceDescriptor device);

        void controlDevice_(V4L2DeviceDescriptor device, int request, void* arg) const;
        void controlDevice_(V4L2DeviceDescriptor device, int request, void* arg, const v4l2_device_error &invalid_request_exception) const;

        void waitForData_(V4L2DeviceDescriptor device);

        boost::thread captureThread_;

        bool captureCrashed_;
        boost::exception_ptr captureError_;
        boost::mutex captureErrorMutex_;

        bool doCapture_;
        boost::mutex doCaptureMutex_;

        BufferIndex capturedBufferIndex_;
        boost::mutex capturedBufferMutex_;

        BufferIndex recentBufferIndex_;
        bool newRecentBuffer_;
        boost::mutex recentBufferMutex_;
        boost::condition_variable recentBufferChangedCond_;

        std::vector<Buffer> buffers_;

        fs::path file_;

        int startCount_;
        boost::mutex startStopMutex_;
    };
} // namespace Gecon

#endif // GECON_V4L2VIDEODEVICECAPTURE_HPP
