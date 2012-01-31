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

#ifndef GECON_CONTROL_HPP
#define GECON_CONTROL_HPP

#include <memory>

#include "DeviceManager.hpp"

namespace Gecon
{
    template<
            typename DevicePolicy,
            typename ControlPolicy
            /*typename ObjectPolicy,
            typename GesturePolicy,
            typename ActionPolicy*/
    >
    class Control :
        public DevicePolicy,
        public ControlPolicy
        /*public ObjectPolicy,
        public GesturePolicy,
        public ActionPolicy*/
    {
    public:
        using ControlPolicy::beforeRun;
        using ControlPolicy::afterRun;
        using ControlPolicy::recognizeObjects;
        using ControlPolicy::checkGestures;
        using ControlPolicy::executeActions;

        typedef Gecon::DeviceManager< DevicePolicy > DeviceManager;

        /*typedef std::set< typename ObjectPolicy::Object > ObjectSet;
        typedef std::set< typename GesturePolicy::Gesture > GestureSet;
        typedef std::set< typename ActionPolicy::Action > ActionSet;*/

        typedef std::set< typename ControlPolicy::Object > ObjectSet;
        typedef std::set< typename ControlPolicy::Control > GestureSet;
        typedef std::set< typename ControlPolicy::Action > ActionSet;

        Control():
            doControl_(false),
            isRunning_(false)
        {
        }

        /**
         * Start control loop.
         *
         * Every change performed on Control object has no effect
         * on the currently running control loop.
         * All changes will take effect on the next run.
         */
        void start()
        {
            if(! controlLoop_.isRunning())
            {
                controlLoop_ = *this;
                controlLoop_.doControl_ = true;

                boost::thread thread(&controlLoop_);
            }
        }

        /**
         * Stop control loop.
         */
        void stop()
        {
            if(controlLoop_.isRunning())
            {
                controlLoop_.stop();
            }

            boost::unique_lock<boost::mutex> doControlLock(doControlMutex_);
            doControl = false;
            doControlLock.unlock();

            boost::unique_lock<boost::mutex> isRunningLock(doControlMutex_);
            while(isRunning_)
            {
                stopCond_.wait(isRunningLock);
            }
            isRunningLock.unlock();
        }

        /**
         * Tells if control is currently running.
         *
         * @returns True/False.
         */
        bool isRunning() const
        {
            boost::lock_guard<boost::mutex> lock(isRunningMutex_);
            return isRunning_;
        }

        /**
         * Main control loop.
         *
         * WARNING: Don't call it directly.
         * Instead of it call start() method to start control loop.
         *
         * @see start().
         */
        void operator()()
        {
            boost::lock_guard<boost::mutex> isRunningLock(isRunningMutex_);
            isRunning_ = true;
            isRunningLock.unlock();

            beforeRun();

            DeviceManager::DeviceAdapter device = deviceManager_.selectedDevice();
            device.open();

            boost::unique_lock<boost::mutex> doControlLock(doControlMutex_);
            while(doControl_)
            {
                doControlLock.unlock();

                executeActions(checkGestures(recognizeObjects(device.getSnapshot())));

                doControlLock.lock();
            }
            doControlLock.unlock();

            device.close();

            afterRun();

            isRunningLock.lock();
            isRunning_ = false;
            isRunningLock.unlock();

            stopCond_.notify_one();
        }

        DeviceManager& deviceManager();

        ObjectSet& objects();
        GestureSet& gestures();
        ActionSet& actions();

    private:
        Control<DevicePolicy, ControlPolicy> controlLoop_;

        boost::mutex doControlMutex_;
        bool doControl_;

        boost::mutex isRunningMutex_;
        bool isRunning_;

        boost::condition_variable stopCond_;

        DeviceManager deviceManager_;

        ObjectSet objects_;
        GestureSet gestures_;
        ActionSet actions_;
    };
} // namespace Gecon

#endif // GECON_CONTROL_HPP
