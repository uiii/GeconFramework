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

#ifndef GECON_CAMERA_HPP
#define GECON_CAMERA_HPP

#include <list>

#include "CameraDevice.hpp"

namespace Gecon
{
    class Camera
    {
    public:
        Camera(CameraDevicePtr cameraDevice);

        void open();
        void close();

        /** Checks if camera capture is opened.
         *
         * @retval True
         *     If capture is opened.
         * @retval False
         *     Otherwise.
         */
        bool isOpened() const;

        /** Gives image from the camera.
         *
         * @throws std::logic_error
         *     If camera capture is not opened.
         *
         * @returns
         *     Image object captured from the camera.
         */
        Image getFrame();

        CameraDeviceConstPtr device() const;

    private:
        CameraDevicePtr cameraDevice_;

        std::shared_ptr<int> openingsCount_; // TODO presunout do camera device
    };

    typedef std::list<Camera> CameraList;
}

#endif // GECON_CAMERA_HPP
