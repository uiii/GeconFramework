#include "../ObjectStateGestureChecker.hpp"

namespace Gecon
{
    template< typename Object, typename ObjectContainer >
    ObjectStateGestureChecker<Object, ObjectContainer>::ObjectStateGestureChecker()
    {
    }

    template< typename Object, typename ObjectContainer >
    void ObjectStateGestureChecker<Object, ObjectContainer>::addGesture(typename ObjectStateGestureChecker<Object, ObjectContainer>::Gesture *gesture)
    {
        gestures_.insert(gesture);
        objectGestures_[gesture->object()].insert(gesture);

        gesture->reset();
    }

    template< typename Object, typename ObjectContainer >
    void ObjectStateGestureChecker<Object, ObjectContainer>::removeGesture(typename ObjectStateGestureChecker<Object, ObjectContainer>::Gesture *gesture)
    {
        gestures_.erase(gesture);
        objectGestures_[gesture->object()].erase(gesture);
    }

    template< typename Object, typename ObjectContainer >
    typename ObjectGestureChecker<Object, ObjectContainer>::Events ObjectStateGestureChecker<Object, ObjectContainer>::check(const typename ObjectGestureChecker<Object, ObjectContainer>::Objects &objects)
    {
        for(Object* object : objects)
        {
            Gestures gestures = objectGestures_[object];
            std::cout << "object: " << object << std::endl;
            std::cout << "gest: " << gestures.size() << std::endl;
            gesturesToCheck_.insert(gestures.begin(), gestures.end());
            std::cout << "gtch: " << gesturesToCheck_.size() << std::endl;
        }

        Events events;

        Gestures needCheck;
        for(Gesture* gesture : gesturesToCheck_)
        {
            std::cout << "gesture: " << gesture << std::endl;
            Events gestureEvents = gesture->check();
            events.insert(gestureEvents.begin(), gestureEvents.end());

            if(gesture->needCheck())
            {
                needCheck.insert(gesture);
            }
        }

        std::swap(gesturesToCheck_, needCheck);

        return events;
    }

    template< typename Object, typename ObjectContainer >
    void ObjectStateGestureChecker<Object, ObjectContainer>::clear()
    {
        gestures_.clear();
        gesturesToCheck_.clear();
        objectGestures_.clear();
    }

    template< typename Object, typename ObjectContainer >
    void ObjectStateGestureChecker<Object, ObjectContainer>::reset()
    {
        gesturesToCheck_.clear();

        gesturesToCheck_.insert(gestures_.begin(), gestures_.end()); // check all gestures for the first time

        for(Gesture* gesture : gestures_)
        {
            gesture->reset();
        }
    }
} // namespace Gecon
