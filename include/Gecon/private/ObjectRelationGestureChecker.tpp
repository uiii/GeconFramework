#include "../ObjectRelationGestureChecker.hpp"

namespace Gecon
{
    template< typename Object, typename ObjectContainer >
    ObjectRelationGestureChecker<Object, ObjectContainer>::ObjectRelationGestureChecker()
    {
    }

    template< typename Object, typename ObjectContainer >
    void ObjectRelationGestureChecker<Object, ObjectContainer>::addGesture(ObjectRelationGestureChecker<Object, ObjectContainer>::Gesture *gesture)
    {
        gestures_.insert(gesture);
        objectGestures_[gesture->leftObject()].insert(gesture);
        objectGestures_[gesture->rightObject()].insert(gesture);

        gesture->reset();
    }

    template< typename Object, typename ObjectContainer >
    void ObjectRelationGestureChecker<Object, ObjectContainer>::removeGesture(ObjectRelationGestureChecker<Object, ObjectContainer>::Gesture *gesture)
    {
        gestures_.erase(gesture);
        objectGestures_[gesture->leftObject()].erase(gesture);
        objectGestures_[gesture->rightObject()].erase(gesture);
    }

    template< typename Object, typename ObjectContainer >
    typename ObjectGestureChecker<Object, ObjectContainer>::Events ObjectRelationGestureChecker<Object, ObjectContainer>::check(const typename ObjectGestureChecker<Object, ObjectContainer>::Objects &objects)
    {
        for(Object* object : objects)
        {
            Gestures gestures = objectGestures_[object];
            gesturesToCheck_.insert(gestures.begin(), gestures.end());
        }

        Events events;

        Gestures needCheck;
        for(Gesture* gesture : gesturesToCheck_)
        {
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
    void ObjectRelationGestureChecker<Object, ObjectContainer>::clear()
    {
        gestures_.clear();
        gesturesToCheck_.clear();
        objectGestures_.clear();
    }

    template< typename Object, typename ObjectContainer >
    void ObjectRelationGestureChecker<Object, ObjectContainer>::reset()
    {
        gesturesToCheck_.clear();

        gesturesToCheck_.insert(gestures_.begin(), gestures_.end()); // check all gestures for the first time

        for(Gesture* gesture : gestures_)
        {
            gesture->reset();
        }
    }
} // namespace Gecon
