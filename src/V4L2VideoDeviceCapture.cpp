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

#include "V4L2VideoDeviceCapture.hpp"

#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <libv4l2.h>

#include <iostream>

#define CLEAR(x) memset(&(x), 0, sizeof(x))

namespace Gecon
{
    V4L2VideoDeviceCapture::V4L2VideoDeviceCapture(const fs::path &file):
        captureCrashed_(false),
        doCapture_(false),
        capturedBufferIndex_(0),
        recentBufferIndex_(0),
        newRecentBuffer_(false),
        file_(file),
        startCount_(0)
    {
    }

    const fs::path& V4L2VideoDeviceCapture::file() const
    {
        return file_;
    }

    /**
     * Start capture.
     *
     * @throws // TODO
     */
    void V4L2VideoDeviceCapture::start()
    {
        boost::lock_guard<boost::mutex> lock(startStopMutex_);

        if(startCount_ == 0)
        {
            startCaptureLoop_();
        }

        ++startCount_;
    }

    /**
     * Stop capture.
     */
    void V4L2VideoDeviceCapture::stop()
    {
        boost::lock_guard<boost::mutex> lock(startStopMutex_);

        if(startCount_ == 1)
        {
            stopCaptureLoop_();
        }

        if(startCount_ > 0)
        {
            --startCount_;
        }
    }

    V4L2VideoDeviceCapture::Snapshot V4L2VideoDeviceCapture::getSnapshot()
    {
        boost::lock_guard<boost::mutex> lock(startStopMutex_);

        if(startCount_ == 0)
        {
            if(captureCrashed_)
            {
                boost::rethrow_exception(captureError_);
            }
            else
            {
                throw v4l2_device_error(file_, "%device% is not opened");
            }
        }

        boost::unique_lock<boost::mutex> recentBufferLock(recentBufferMutex_);
        if(newRecentBuffer_)
        {
            std::swap(capturedBufferIndex_, recentBufferIndex_);
            newRecentBuffer_ = false;
        }
        recentBufferLock.unlock();

        return Image(640, 480, buffers_[capturedBufferIndex_].pointer); // TODO magic
    }

    void V4L2VideoDeviceCapture::checkV4L2VideoDevice_(const fs::path& file) const
    {
        V4L2DeviceDescriptor device(file);

        v4l2_capability cap;

        controlDevice_(device, VIDIOC_QUERYCAP, &cap,
            v4l2_device_error(file, "%device% is not V4L2 device"));

        if(! (cap.capabilities & V4L2_CAP_VIDEO_CAPTURE))
        {
            throw v4l2_unsupported_requirement(file, "video capture");
        }
    }

    void V4L2VideoDeviceCapture::initialize_(V4L2DeviceDescriptor device)
    {
        v4l2_format format;
        CLEAR(format);

        format.fmt.pix.width = 640; // TODO magic
        format.fmt.pix.width = 480; // TODO magic
        format.fmt.pix.pixelformat = V4L2_PIX_FMT_RGB24;
        format.fmt.pix.field = V4L2_FIELD_INTERLACED;
        format.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

        controlDevice_(device, VIDIOC_S_FMT, &format);

        if(format.fmt.pix.pixelformat != V4L2_PIX_FMT_RGB24)
        {
            throw v4l2_unsupported_requirement(device.file(), "RGB24 pixel format");
        }

        if(format.fmt.pix.width != 640 || format.fmt.pix.height != 480)
        {
            // TODO log warning
        }

        v4l2_requestbuffers bufferRequest;
        CLEAR(bufferRequest);

        bufferRequest.count = 3; // TODO magic
        bufferRequest.memory = V4L2_MEMORY_MMAP;
        bufferRequest.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

        controlDevice_(device, VIDIOC_REQBUFS, &bufferRequest,
            v4l2_unsupported_requirement(device.file(), "memory mapping I/O"));

        if(bufferRequest.count < 3) // TODO magic
        {
            // free buffers
            bufferRequest.count = 0;
            controlDevice_(device, VIDIOC_REQBUFS, &bufferRequest);

            throw v4l2_device_error(device.file(), "%device% hasn't enough free memory");
        }

        v4l2_buffer deviceBuffer;

        for(int i = 0; i < 3; ++i) // TODO magic
        {
            CLEAR(deviceBuffer);

            deviceBuffer.index = i;
            deviceBuffer.memory = V4L2_MEMORY_MMAP;
            deviceBuffer.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

            controlDevice_(device, VIDIOC_QUERYBUF, &deviceBuffer);

            void* mapped_memory = v4l2_mmap(NULL, deviceBuffer.length, PROT_READ | PROT_WRITE, MAP_SHARED, device, deviceBuffer.m.offset);
            if(mapped_memory == MAP_FAILED)
            {
                throw v4l2_device_error(device.file(), "%device% memory mapping failed", errno_message());
            }

            Buffer buffer = { i, (unsigned char*)mapped_memory, deviceBuffer.length };
            buffers_.push_back(buffer);
        }

        // enque all buffers except last two
        for(int i = 0; i < 3 - 2; ++i) // TODO magic
        {
            CLEAR(deviceBuffer);

            deviceBuffer.index = 1;
            deviceBuffer.memory = V4L2_MEMORY_MMAP;
            deviceBuffer.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

            controlDevice_(device, VIDIOC_QBUF, &deviceBuffer);
        }

        // last two buffer reserve for exchange
        recentBufferIndex_ = 3 - 2; // TODO magic
        capturedBufferIndex_ = 3 - 1; // TODO magic
    }

    void V4L2VideoDeviceCapture::deinitialize_()
    {
        for(int i = 0; i < 3; ++i)
        {
            v4l2_munmap(buffers_[i].pointer, buffers_[i].length);
        }

        buffers_.clear();
    }

    void V4L2VideoDeviceCapture::startCaptureLoop_()
    {
        V4L2DeviceDescriptor device(file_);
        initialize_(device);

        doCapture_ = true;

        // start capture loop
        captureThread_ = boost::thread(&V4L2VideoDeviceCapture::captureLoop_, this, device);

        // wait for the first captured buffer
        boost::unique_lock<boost::mutex> lock(recentBufferMutex_);
        if(recentBufferChangedCond_.timed_wait(lock, boost::posix_time::seconds(2)))
        {
            captureCrashed_ = false;
        }
        else
        {
            stopCaptureLoop_();

            if(captureError_)
            {
                boost::rethrow_exception(captureError_);
            }
            else
            {
                throw v4l2_device_error(device.file(), "response time of %device% has expired");
            }
        }
    }

    void V4L2VideoDeviceCapture::stopCaptureLoop_()
    {
        boost::unique_lock<boost::mutex> doCaptureLock(doCaptureMutex_);
        doCapture_ = false;
        doCaptureLock.unlock();

        captureThread_.join();
    }

    void V4L2VideoDeviceCapture::captureCrash_()
    {
        boost::lock_guard<boost::mutex> lock(startStopMutex_);

        if(startCount_ > 0)
        {
            stopCaptureLoop_();

            startCount_ = 0;
        }

        captureCrashed_ = true;
    }

    void V4L2VideoDeviceCapture::captureLoop_(V4L2DeviceDescriptor device)
    {
        int type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        controlDevice_(device, VIDIOC_STREAMON, &type);

        v4l2_buffer deviceBuffer;

        boost::unique_lock<boost::mutex> doCaptureLock(doCaptureMutex_);
        while(doCapture_)
        {
            doCaptureLock.unlock();

            try
            {
                waitForData_(device);

                CLEAR(deviceBuffer);
                deviceBuffer.memory = V4L2_MEMORY_MMAP;
                deviceBuffer.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
                controlDevice_(device, VIDIOC_DQBUF, &deviceBuffer);

                boost::unique_lock<boost::mutex> lock(recentBufferMutex_);
                std::swap(deviceBuffer.index, recentBufferIndex_);
                newRecentBuffer_ = true;
                lock.unlock();

                recentBufferChangedCond_.notify_all();

                controlDevice_(device, VIDIOC_QBUF, &deviceBuffer);
            }
            catch(const std::exception& e)
            {
                captureError_ = boost::copy_exception(v4l2_capture_crashed<std::exception>(e, "Try to re-open the device."));

                doCaptureLock.lock();
                doCapture_ = false;
                doCaptureLock.unlock();

                // start capture termination thread
                boost::thread(&V4L2VideoDeviceCapture::captureCrash_, this);
            }

            doCaptureLock.lock();
        }
        doCaptureLock.unlock();

        type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        controlDevice_(device, VIDIOC_STREAMOFF, &type);

        deinitialize_();
    }

    void V4L2VideoDeviceCapture::controlDevice_(V4L2DeviceDescriptor device, int request, void *arg) const
    {
        controlDevice_(device, request, arg, v4l2_invalid_request(device.file()));
    }

    void V4L2VideoDeviceCapture::controlDevice_(V4L2DeviceDescriptor device, int request, void *arg, const v4l2_device_error& invalid_request_exception) const
    {
        int result;
        do
        {
            result = v4l2_ioctl(device, request, arg);
        } while(result == -1 && (errno == EINTR || errno == EAGAIN));

        if(result == -1)
        {
            if(errno == EINVAL)
            {
                invalid_request_exception.raise();
            }
            else
            {
                throw v4l2_device_error(device.file(), "cannot control device %device%", errno_message());
            }
        }
    }

    void V4L2VideoDeviceCapture::waitForData_(V4L2DeviceDescriptor device)
    {
        int result;
        do
        {
            fd_set fds;
            FD_ZERO(&fds);
            FD_SET(device, &fds);

            timeval tv;
            tv.tv_sec = 2;
            tv.tv_usec = 0;

            result = select(device + 1, &fds, NULL, NULL, &tv);
        } while(result == -1 && errno == EINTR);

        if(result == -1)
        {
            throw v4l2_device_error(device.file(), "select on %device%", errno_message());
        }
        else if(result == 0)
        {
            throw v4l2_device_error(device.file(), "cannot get data from %device%");
        }
    }
} // namespace Gecon
