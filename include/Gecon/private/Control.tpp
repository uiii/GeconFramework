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
    template< typename DevicePolicy, typename ObjectPolicy, typename GesturePolicy, typename ActionPolicy>
    config_variable<std::size_t> Control<DevicePolicy, ObjectPolicy, GesturePolicy, ActionPolicy>::SLEEP_TIME = 30;

    template< typename DevicePolicy, typename ObjectPolicy, typename GesturePolicy, typename ActionPolicy>
    Control<DevicePolicy, ObjectPolicy, GesturePolicy, ActionPolicy>::Control():
        controlLoop_(0),
        doControl_(false),
        isRunning_(false)
    {
    }

    template< typename DevicePolicy, typename ObjectPolicy, typename GesturePolicy, typename ActionPolicy>
    void Control<DevicePolicy, ObjectPolicy, GesturePolicy, ActionPolicy>::start()
    {
        if(controlLoop_ == 0)
        {
            controlLoop_ = new Control<DevicePolicy, ObjectPolicy, GesturePolicy, ActionPolicy>;
        }

        if(! controlLoop_->isRunning())
        {
            *controlLoop_ = *this;
            controlLoop_->doControl_ = true;

            boost::thread thread(std::ref(*controlLoop_));
        }
    }

    template< typename DevicePolicy, typename ObjectPolicy, typename GesturePolicy, typename ActionPolicy>
    void Control<DevicePolicy, ObjectPolicy, GesturePolicy, ActionPolicy>::restart()
    {
        if(! controlLoop_ || ! controlLoop_->isRunning())
        {
            return;
        }

        boost::unique_lock<boost::mutex> dataLock(controlLoop_->dataMutex_);

        ObjectPolicy& objectPolicy = *controlLoop_;
        objectPolicy = *this;

        GesturePolicy& gesturePolicy = *controlLoop_;
        gesturePolicy = *this;

        ActionPolicy& actionPolicy = *controlLoop_;
        actionPolicy = *this;

        dataLock.unlock();
    }

    template< typename DevicePolicy, typename ObjectPolicy, typename GesturePolicy, typename ActionPolicy>
    void Control<DevicePolicy, ObjectPolicy, GesturePolicy, ActionPolicy>::stop()
    {
        if(controlLoop_ && controlLoop_->isRunning())
        {
            controlLoop_->stop();
        }

        boost::unique_lock<boost::mutex> doControlLock(doControlMutex_);
        doControl_ = false;
        doControlLock.unlock();

        boost::unique_lock<boost::mutex> isRunningLock(isRunningMutex_);
        while(isRunning_)
        {
            stopCond_.wait(isRunningLock);
        }
        isRunningLock.unlock();
    }

    template< typename DevicePolicy, typename ObjectPolicy, typename GesturePolicy, typename ActionPolicy>
    bool Control<DevicePolicy, ObjectPolicy, GesturePolicy, ActionPolicy>::isRunning() const
    {
        boost::lock_guard<boost::mutex> lock(isRunningMutex_);
        if(controlLoop_) // this is not control loop
        {
            return controlLoop_->isRunning();
        }
        else // this is control loop
        {
            return isRunning_;
        }
    }

    template< typename DevicePolicy, typename ObjectPolicy, typename GesturePolicy, typename ActionPolicy>
    void Control<DevicePolicy, ObjectPolicy, GesturePolicy, ActionPolicy>::operator()()
    {
        if(controlLoop_) // this is not control loop
        {
            return;
        }

        boost::unique_lock<boost::mutex> isRunningLock(isRunningMutex_);
        isRunning_ = true;
        isRunningLock.unlock();

        try
        {
            device_.open();
        }
        catch(const std::exception& e)
        {
            doControl_ = false;
        }

        boost::unique_lock<boost::mutex> doControlLock(doControlMutex_);
        while(doControl_)
        {
            doControlLock.unlock();

            boost::unique_lock<boost::mutex> dataLock(dataMutex_);
            try
            {
                ActionPolicy::checkActionTriggers(GesturePolicy::checkGestures(ObjectPolicy::recognizeObjects(device_.getSnapshot())));
            }
            catch(const std::exception& e)
            {
                doControl_ = false;
            }
            dataLock.unlock();

            boost::this_thread::sleep(boost::posix_time::milliseconds(SLEEP_TIME));

            doControlLock.lock();
        }
        doControlLock.unlock();

        device_.close();

        isRunningLock.lock();
        isRunning_ = false;
        isRunningLock.unlock();

        stopCond_.notify_all();
    }

    template< typename DevicePolicy, typename ObjectPolicy, typename GesturePolicy, typename ActionPolicy>
    void Control<DevicePolicy, ObjectPolicy, GesturePolicy, ActionPolicy>::setDevice(Control::DeviceAdapter device)
    {
        device_ = device;
    }

    template< typename DevicePolicy, typename ObjectPolicy, typename GesturePolicy, typename ActionPolicy>
    typename Control<DevicePolicy, ObjectPolicy, GesturePolicy, ActionPolicy>::DeviceAdapter Control<DevicePolicy, ObjectPolicy, GesturePolicy, ActionPolicy>::device() const
    {
        return device_;
    }

    template< typename DevicePolicy, typename ObjectPolicy, typename GesturePolicy, typename ActionPolicy>
    void Control<DevicePolicy, ObjectPolicy, GesturePolicy, ActionPolicy>::operator=(const Control::ControlLoop& controlLoop)
    {
        ObjectPolicy::operator=(controlLoop);
        GesturePolicy::operator=(controlLoop);
        ActionPolicy::operator=(controlLoop);

        device_ = controlLoop.device_;
    }
} // namespace Gecon
