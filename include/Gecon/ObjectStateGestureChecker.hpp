#ifndef GECON_OBJECTSTATEGESTURECHECKER_HPP
#define GECON_OBJECTSTATEGESTURECHECKER_HPP

#include "ObjectGestureChecker.hpp"
#include "ObjectStateGesture.hpp"

namespace Gecon
{
    template< typename Object, typename ObjectContainer = std::set<Object*> >
    class ObjectStateGestureChecker : public ObjectGestureChecker<Object, ObjectContainer>
    {
    public:
        typedef ObjectStateGesture<Object> Gesture;
        typedef std::set<Gesture*> Gestures;

        typedef typename ObjectGestureChecker<Object, ObjectContainer>::Events Events;
        typedef typename ObjectGestureChecker<Object, ObjectContainer>::Objects Objects;

        ObjectStateGestureChecker();

        void addGesture(Gesture* gesture);
        void removeGesture(Gesture* gesture);

        Events check(const Objects& objects);

        void clear();
        void reset();

    private:
        Gestures gestures_;
        Gestures gesturesToCheck_;

        std::map<Object*, Gestures> objectGestures_;
    };
} // namespace Gecon

#include "private/ObjectStateGestureChecker.tpp"

#endif // GECON_OBJECTSTATEGESTURECHECKER_HPP
