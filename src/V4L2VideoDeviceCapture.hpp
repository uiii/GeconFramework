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

#define SNAPSHOT_WIDTH 640
#define SNAPSHOT_HEIGHT 480

#define BUFFER_COUNT 5

#define WAIT_FOR_DATA_TIMEOUT 2

namespace Gecon
{
    typedef __u32 BufferIndex;

    struct Buffer
    {
        BufferIndex index;
        unsigned char* data;
        std::size_t length;
    };

    struct RawSnapshot
    {
        std::size_t widht;
        std::size_t height;
        unsigned char* data;
    };

    /**
     * Represents V4L2 device capture.
     *
     * When started, captures snapshots from the device.
     * Should be created only once per device file.
     */
    class V4L2VideoDeviceCapture
    {
    public:
        /**
         * Construct a device capture.
         *
         * @param file
         *     File representing V4L2 device.
         *
         * @throws v4l2_device_error
         *     If file doesn't represent V4L2 device.
         * @throws v4l2_unsupported_requirement
         *     If the device doesn't support video capture.
         */
        V4L2VideoDeviceCapture(const fs::path& file);

        /**
         * Get path to the system file representing the device.
         *
         * @returns
         *     Path to the device file.
         */
        const fs::path& file() const;

        /**
         * Start capture.
         *
         * @throws v4l2_unsupported_requirement
         *     When the device doesn't meet the requirements needed for capture.
         * @throws v4l2_device_error
         *     When some error related to the device occured while capture starts
         */
        void start();

        /**
         * Stop capture.
         */
        void stop();

        /**
         * Get the most recently captured snapshot from the device.
         *
         * @returns
         *     Device snapshot.
         *
         * @throws v4l2_device_error
         *     When capture is not started.
         *     When capture crashed.
         */
        RawSnapshot getSnapshot();

    private:
        /**
         * Checks is the device file is V4L2 device.
         *
         * If not throws the exception.
         *
         * @throws v4l2_device_error
         *     When the device is not V4L2 device.
         * @throws v4l2_unsupported_requirement
         *     When the device doesn't support video capture.
         */
        void checkV4L2VideoDevice_(const fs::path& file) const;

        /**
         * Initialize the device capture.
         *
         * @param device
         *     V4L2 device descriptor.
         *
         * @throws v4l2_device_error
         *     When cannot control device. @see controlDevice_.
         *     When device hasn't enough free memory.
         *     When memory mapping failed.
         * @throws v4l2_unsupported_requirement
         *     When cannot set required pixel format.
         *     When cannot set memory mapping I/O method.
         */
        void initialize_(V4L2DeviceDescriptor device);

        /**
         * Deinitialize the device capture.
         */
        void deinitialize_();

        /**
         * Start capture loop.
         *
         * Initialize the capture.
         * Then start capture loop thread and wait for the first filled buffer.
         *
         * @throws v4l2_device_error
         *     When capture loop crashes immediately after start. @see captureLoop_.
         *     When capture loop is not responding. @see captureLoop_.
         *     When capture initialization failed. @see initialize_.
         * @throws v4l2_unsupported_requirement
         *     When capture initialization failed. @see initialize_.
         */
        void startCaptureLoop_();

        /**
         * Stop capture loop.
         *
         * Tell the capture loop thread to stop and wait until its end.
         * Then deinitalize the capture.
         */
        void stopCaptureLoop_();

        /**
         * Capture crash.
         *
         * Is called when capture loop crashes.
         */
        void captureCrash_();

        /**
         * Capture loop.
         *
         * Repeatedly fill and rotate the buffers with data from the device
         * and save the most recently filled buffer index.
         *
         * When capture crashes, saves the exception (v4l2_device_error) to captureError_.
         *
         * @param device
         *     V4L2 device descriptor.
         *
         * @throws v4l2_device_error
         *     When cannot start or stop streaming.
         */
        void captureLoop_(V4L2DeviceDescriptor device);

        /**
         * Contol the device.
         *
         * @param device
         *     V4L2 device descriptor.
         * @param request
         *     Request to the device.
         *     // TODO odkaz
         * @param arg
         *     Request argument.
         *
         * @throws v4l2_invalid_request
         *     When the request is invalid.
         * @throws v4l2_device_error
         *     When internal error occures.
         */
        void controlDevice_(V4L2DeviceDescriptor device, int request, void* arg) const;

        /**
         * Contol the device.
         *
         * @param device
         *     V4L2 device descriptor.
         * @param request
         *     Request to the device.
         *     // TODO odkaz
         * @param arg
         *     Request argument.
         * @param invalid_request_exception
         *     The exception to throw when the request is invalid.
         *
         * @throws invalid_request_exception
         *     When the request is invalid.
         * @throws v4l2_device_error
         *     When internal error occures.
         */
        void controlDevice_(V4L2DeviceDescriptor device, int request, void* arg, const v4l2_device_error &invalid_request_exception) const;

        /**
         * Wait until some new data from device are ready.
         *
         * @param device
         *     V4L2 device descriptor.
         *
         * @throws v4l2_device_error
         *     When internal error occures.
         *     When the time is out.
         */
        void waitForData_(V4L2DeviceDescriptor device);

        boost::thread captureThread_;

        bool doCapture_;
        boost::mutex doCaptureMutex_;

        bool captureCrashed_;
        boost::exception_ptr captureError_;
        boost::mutex captureErrorMutex_;

        std::size_t snapshotWidht_;
        std::size_t snapshotHeight_;

        std::size_t bufferCount_;
        std::vector<Buffer> buffers_;

        BufferIndex capturedBufferIndex_;
        boost::mutex capturedBufferMutex_;

        BufferIndex recentBufferIndex_;
        bool newRecentBuffer_;
        boost::mutex recentBufferMutex_;
        boost::condition_variable recentBufferChangedCond_;

        fs::path file_;

        int startCount_;
        boost::mutex startStopMutex_;
    };
} // namespace Gecon

#endif // GECON_V4L2VIDEODEVICECAPTURE_HPP
