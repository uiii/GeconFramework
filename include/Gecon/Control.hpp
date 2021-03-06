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

#include "config_variable.hpp"

namespace Gecon
{
    template< typename DevicePolicy, typename ObjectPolicy, typename GesturePolicy, typename ActionPolicy >
    class Control : public ObjectPolicy, public GesturePolicy, public ActionPolicy
    {
    public:
        static config_variable<std::size_t> SLEEP_TIME;

        typedef typename DevicePolicy::DeviceAdapter DeviceAdapter;

        Control();

        /**
         * Start control loop.
         *
         * Every change performed on Control object has no effect
         * on the currently running control loop.
         * Need to be restarted to take their effect;
         */
        void start();

        /**
         * Restart control loop.
         *
         * Applay changes performed on Control object
         * on the currently running control loop.
         */
        void restart();

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
         * @warning Don't call it directly.
         * Instead of it call start() method to start control loop.
         *
         * @see start().
         */
        void operator()();

        /**
         * Set device to control.
         *
         * @note Control must be restarted to take effect.
         *
         * @param device
         *     Device to set.
         */
        void setDevice(DeviceAdapter device);

        /**
         * @returns
         *     Device set to control.
         */
        DeviceAdapter device() const;

    private:
        typedef Gecon::Control<DevicePolicy, ObjectPolicy, GesturePolicy, ActionPolicy> ControlLoop;
        ControlLoop* controlLoop_;

        void operator=(const ControlLoop& controlLoop);

        boost::mutex dataMutex_;

        boost::mutex doControlMutex_;
        bool doControl_;

        mutable boost::mutex isRunningMutex_;
        bool isRunning_;

        boost::condition_variable stopCond_;

        DeviceAdapter device_;
    };
} // namespace Gecon

#include "private/Control.tpp"

#endif // GECON_CONTROL_HPP
