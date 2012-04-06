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

#ifndef GECON_CONTROL_HPP
#define GECON_CONTROL_HPP

#include <memory>

#include <boost/thread.hpp>

namespace Gecon
{
    template< typename DevicePolicy, typename ObjectPolicy, typename GesturePolicy >
    class Control : public ObjectPolicy, public GesturePolicy
    {
    public:
        typedef typename DevicePolicy::DeviceAdapter DeviceAdapter;

        /*typedef std::set< typename ObjectPolicy::Object > ObjectSet;
        typedef std::set< typename GesturePolicy::Gesture > GestureSet;*/

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

        void setDevice(DeviceAdapter device);
        DeviceAdapter device() const;

        /*ObjectSet& objects();
        GestureSet& gestures();*/

    private:
        Gecon::Control<DevicePolicy, ObjectPolicy, GesturePolicy>* controlLoop_;

        boost::mutex doControlMutex_;
        bool doControl_;

        boost::mutex isRunningMutex_;
        bool isRunning_;

        boost::condition_variable stopCond_;

        DeviceAdapter device_;

        /*ObjectSet objects_;
        GestureSet gestures_;*/
    };
} // namespace Gecon

#include "private/Control.tpp"

#endif // GECON_CONTROL_HPP
