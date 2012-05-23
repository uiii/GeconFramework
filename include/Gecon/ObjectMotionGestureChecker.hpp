#ifndef GECON_OBJECTMOTIONGESTURECHECKER_HPP
#define GECON_OBJECTMOTIONGESTURECHECKER_HPP

#include "ObjectGestureChecker.hpp"
#include "ObjectMotionGesture.hpp"

namespace Gecon
{
    template< typename Object, typename ObjectContainer = std::set<Object*> >
    class ObjectMotionGestureChecker : public ObjectGestureChecker<Object, ObjectContainer>
    {
    public:
        static config_variable<std::chrono::milliseconds::rep> MOTION_TIMEOUT;
        static config_variable<std::size_t> MINIMAL_GESTURE_SIDE;
        static config_variable<std::size_t> NOT_MOTION_TOLERANCE;
        static config_variable<std::size_t> MINIMAL_DIFFERENT_GESTURE_DISTANCE;
        static config_variable<std::size_t> MAXIMAL_SAME_GESTURE_DISTANCE;

        typedef ObjectMotionGesture<Object> Gesture;
        typedef std::set<Gesture*> Gestures;

        typedef typename ObjectGestureChecker<Object, ObjectContainer>::Events Events;
        typedef typename ObjectGestureChecker<Object, ObjectContainer>::Objects Objects;

        typedef std::set<Object*> ObjectSet;

        typedef typename ObjectMotionGesture<Object>::Motion Motion;
        typedef typename ObjectMotionGesture<Object>::MoveSequence MoveSequence;

        typedef std::chrono::system_clock::time_point Time;

        struct MotionRecord
        {
            Motion motion;
            MoveSequence moves;
            Time lastRecordedMotionTime;

            Gestures gestures;
        };

        typedef std::map<Object*, MotionRecord> MotionStorage;

        ObjectMotionGestureChecker();

        void addGesture(Gesture* gesture);
        void removeGesture(Gesture* gesture);

        Events check(const Objects& objects);

        void clear();
        void reset();

    protected:
        typedef std::chrono::milliseconds Timeout;

        void recordMotion_(Object* object);
        virtual void motionDone_(MotionRecord& record);

        Gestures gestures_;
        MotionStorage motionStorage_;

        ObjectSet needCheck_;

        Timeout timeout_;
    };

    template< typename T >
    std::size_t levenshteinDistance(const std::vector<T>& left, const std::vector<T>& right, std::size_t maxDistance);
} // namespace Gecon

#include "private/ObjectMotionGestureChecker.tpp"

#endif // GECON_OBJECTMOTIONGESTURECHECKER_HPP
