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

        Control();

        /**
         * Start control loop.
         *
         * Every change performed on Control object has no effect
         * on the currently running control loop.
         * All changes will take effect on the next run.
         */
        void start();

        /**
         * Stop control loop.
         */
        void stop();

        /**
         * Tells if control is currently running.
         *
         * @returns True/False.
         */
        bool isRunning() const;

        /**
         * Main control loop.
         *
         * WARNING: Don't call it directly.
         * Instead of it call start() method to start control loop.
         *
         * @see start().
         */
        void operator()();

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

#include "Control.tpp"

#endif // GECON_CONTROL_HPP
