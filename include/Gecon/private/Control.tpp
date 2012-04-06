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

#include "../Control.hpp"

namespace Gecon
{
    template< typename DevicePolicy, typename ObjectPolicy, typename GesturePolicy>
    Control<DevicePolicy, ObjectPolicy, GesturePolicy>::Control():
        controlLoop_(0),
        doControl_(false),
        isRunning_(false)
    {
    }

    template< typename DevicePolicy, typename ObjectPolicy, typename GesturePolicy>
    void Control<DevicePolicy, ObjectPolicy, GesturePolicy>::start()
    {
        if(controlLoop_ == 0)
        {
            controlLoop_ = new Control<DevicePolicy, ObjectPolicy, GesturePolicy>;
        }

        if(! controlLoop_->isRunning())
        {
            if(! device_)
            {
                throw; // TODO
            }

            controlLoop_ = *this;
            controlLoop_->doControl_ = true;

            boost::thread thread(controlLoop_);
        }
    }

    template< typename DevicePolicy, typename ObjectPolicy, typename GesturePolicy>
    void Control<DevicePolicy, ObjectPolicy, GesturePolicy>::stop()
    {
        if(! controlLoop_) // control loop hasn't been initialized yet
        {
            return;
        }

        if(controlLoop_->isRunning())
        {
            controlLoop_->stop();
        }

        boost::unique_lock<boost::mutex> doControlLock(doControlMutex_);
        doControl_ = false;
        doControlLock.unlock();

        boost::unique_lock<boost::mutex> isRunningLock(doControlMutex_);
        while(isRunning_)
        {
            stopCond_.wait(isRunningLock);
        }
        isRunningLock.unlock();
    }

    template< typename DevicePolicy, typename ObjectPolicy, typename GesturePolicy>
    bool Control<DevicePolicy, ObjectPolicy, GesturePolicy>::isRunning() const
    {
        boost::lock_guard<boost::mutex> lock(isRunningMutex_);
        return isRunning_;
    }

    template< typename DevicePolicy, typename ObjectPolicy, typename GesturePolicy>
    void Control<DevicePolicy, ObjectPolicy, GesturePolicy>::operator()()
    {
        boost::unique_lock<boost::mutex> isRunningLock(isRunningMutex_);
        isRunning_ = true;
        isRunningLock.unlock();

        device_.open();

        boost::unique_lock<boost::mutex> doControlLock(doControlMutex_);
        while(doControl_)
        {
            doControlLock.unlock();

            executeActions(checkGestures(recognizeObjects(device.getSnapshot())));

            doControlLock.lock();
        }
        doControlLock.unlock();

        device_.close();

        isRunningLock.lock();
        isRunning_ = false;
        isRunningLock.unlock();

        stopCond_.notify_one();
    }

    template< typename DevicePolicy, typename ObjectPolicy, typename GesturePolicy>
    void Control<DevicePolicy, ObjectPolicy, GesturePolicy>::setDevice(Control::DeviceAdapter device)
    {
        device_ = device;
    }

    template< typename DevicePolicy, typename ObjectPolicy, typename GesturePolicy>
    typename Control<DevicePolicy, ObjectPolicy, GesturePolicy>::DeviceAdapter Control<DevicePolicy, ObjectPolicy, GesturePolicy>::device() const
    {
        return device_;
    }

    /*template< typename DevicePolicy, typename ObjectPolicy, typename GesturePolicy>
    ObjectSet& Control<Device, ObjectPolicy, GesturePolicy>::objects();

    template< typename DevicePolicy, typename ObjectPolicy, typename GesturePolicy>
    GestureSet& Control<Device, ObjectPolicy, GesturePolicy>::gestures();*/
} // namespace Gecon
