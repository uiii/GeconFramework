#ifndef GECON_OBJECTRELATIONGESTURECHECKER_HPP
#define GECON_OBJECTRELATIONGESTURECHECKER_HPP

#include "ObjectGestureChecker.hpp"
#include "ObjectRelationGesture.hpp"

namespace Gecon
{
    template< typename Object, typename ObjectContainer = std::set<Object*> >
    class ObjectRelationGestureChecker : public ObjectGestureChecker<Object, ObjectContainer>
    {
    public:
        typedef ObjectRelationGesture<Object> Gesture;
        typedef std::set<Gesture*> Gestures;

        typedef typename ObjectGestureChecker<Object, ObjectContainer>::Events Events;
        typedef typename ObjectGestureChecker<Object, ObjectContainer>::Objects Objects;

        ObjectRelationGestureChecker();

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

#include "private/ObjectRelationGestureChecker.tpp"

#endif // GECON_OBJECTRELATIONGESTURECHECKER_HPP
