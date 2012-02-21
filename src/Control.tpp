#include "Control.hpp"

namespace Gecon
{
    template<
            typename DevicePolicy,
            typename ControlPolicy
            /*typename ObjectPolicy,
            typename GesturePolicy,
            typename ActionPolicy*/
    >
    Control<DevicePolicy, ControlPolicy>::Control():
        doControl_(false),
        isRunning_(false)
    {
    }

    template<
            typename DevicePolicy,
            typename ControlPolicy
            /*typename ObjectPolicy,
            typename GesturePolicy,
            typename ActionPolicy*/
    >
    void Control<DevicePolicy, ControlPolicy>::start()
    {
        if(! controlLoop_.isRunning())
        {
            controlLoop_ = *this;
            controlLoop_.doControl_ = true;

            boost::thread thread(&controlLoop_);
        }
    }

    template<
            typename DevicePolicy,
            typename ControlPolicy
            /*typename ObjectPolicy,
            typename GesturePolicy,
            typename ActionPolicy*/
    >
    void Control<DevicePolicy, ControlPolicy>::stop()
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

    template<
            typename DevicePolicy,
            typename ControlPolicy
            /*typename ObjectPolicy,
            typename GesturePolicy,
            typename ActionPolicy*/
    >
    bool Control<DevicePolicy, ControlPolicy>::isRunning() const
    {
        boost::lock_guard<boost::mutex> lock(isRunningMutex_);
        return isRunning_;
    }

    template<
            typename DevicePolicy,
            typename ControlPolicy
            /*typename ObjectPolicy,
            typename GesturePolicy,
            typename ActionPolicy*/
    >
    void Control<DevicePolicy, ControlPolicy>::operator()()
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

    template<
            typename DevicePolicy,
            typename ControlPolicy
            /*typename ObjectPolicy,
            typename GesturePolicy,
            typename ActionPolicy*/
    >
    DeviceManager& Control<DevicePolicy, ControlPolicy>::deviceManager();

    template<
            typename DevicePolicy,
            typename ControlPolicy
            /*typename ObjectPolicy,
            typename GesturePolicy,
            typename ActionPolicy*/
    >
    ObjectSet& Control<DevicePolicy, ControlPolicy>::objects();

    template<
            typename DevicePolicy,
            typename ControlPolicy
            /*typename ObjectPolicy,
            typename GesturePolicy,
            typename ActionPolicy*/
    >
    GestureSet& Control<DevicePolicy, ControlPolicy>::gestures();

    template<
            typename DevicePolicy,
            typename ControlPolicy
            /*typename ObjectPolicy,
            typename GesturePolicy,
            typename ActionPolicy*/
    >
    ActionSet& Control<DevicePolicy, ControlPolicy>::actions();
} // namespace Gecon
