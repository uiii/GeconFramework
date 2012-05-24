#include "../ObjectMotionGestureChecker.hpp"

namespace Gecon
{
    template< typename Object, typename ObjectContainer >
    config_variable<std::chrono::milliseconds::rep> ObjectMotionGestureChecker<Object, ObjectContainer>::MOTION_TIMEOUT = 1000;
    template< typename Object, typename ObjectContainer >
    config_variable<std::size_t> ObjectMotionGestureChecker<Object, ObjectContainer>::MINIMAL_GESTURE_SIDE_FRACTION = 4;
    template< typename Object, typename ObjectContainer >
    config_variable<std::size_t> ObjectMotionGestureChecker<Object, ObjectContainer>::NOT_MOTION_TOLERANCE = 10;
    template< typename Object, typename ObjectContainer >
    config_variable<std::size_t> ObjectMotionGestureChecker<Object, ObjectContainer>::MINIMAL_DIFFERENT_GESTURE_DISTANCE = 50;
    template< typename Object, typename ObjectContainer >
    config_variable<std::size_t> ObjectMotionGestureChecker<Object, ObjectContainer>::MAXIMAL_SAME_GESTURE_DISTANCE = 30;

    template< typename Object, typename ObjectContainer >
    ObjectMotionGestureChecker<Object, ObjectContainer>::ObjectMotionGestureChecker():
        timeout_(std::chrono::milliseconds(MOTION_TIMEOUT))
    {
    }

    template< typename Object, typename ObjectContainer >
    void ObjectMotionGestureChecker<Object, ObjectContainer>::addGesture(Gesture* motionGesture)
    {
        Gesture* tooSimilarGesture = 0;
        for(Gesture* gesture : gestures_)
        {
            std::size_t minDistance = MINIMAL_DIFFERENT_GESTURE_DISTANCE;
            std::size_t distance = levenshteinDistance(gesture->moves(), motionGesture->moves(), minDistance);
            if(distance < minDistance)
            {
                std::cout << "too similar distance: " << distance << std::endl;
                tooSimilarGesture = gesture;
                break;
            }
        }

        if(tooSimilarGesture)
        {
            throw std::logic_error("Too similar gesture already exists.");
        }

        gestures_.insert(motionGesture);
        motionStorage_[motionGesture->object()].gestures.insert(motionGesture);
    }

    template< typename Object, typename ObjectContainer >
    void ObjectMotionGestureChecker<Object, ObjectContainer>::removeGesture(ObjectMotionGestureChecker<Object, ObjectContainer>::Gesture *gesture)
    {
        gestures_.erase(gesture);
        motionStorage_[gesture->object()].gestures.erase(gesture);
    }

    template< typename Object, typename ObjectContainer >
    typename ObjectGestureChecker<Object, ObjectContainer>::Events ObjectMotionGestureChecker<Object, ObjectContainer>::check(const typename ObjectGestureChecker<Object, ObjectContainer>::Objects& objects)
    {
        Events events;

        needCheck_.insert(objects.begin(), objects.end());
        for(Object* object : needCheck_)
        {
            recordMotion_(object);

            MoveSequence& moves = motionStorage_[object].moves;
            if(! moves.empty())
            {
                std::size_t minDistance = MAXIMAL_SAME_GESTURE_DISTANCE;
                Gesture* closestGesture = 0;
                for(Gesture* gesture : motionStorage_[object].gestures)
                {
                    std::size_t distance = levenshteinDistance(gesture->moves(), moves, 10000);//MAXIMAL_SAME_GESTURE_DISTANCE); //TODO
                    std::cout << "distance: " << distance << std::endl;
                    if(distance < minDistance)
                    {
                        minDistance = distance;
                        closestGesture = gesture;
                    }
                }

                if(minDistance < MAXIMAL_SAME_GESTURE_DISTANCE)
                {
                    events.insert(closestGesture->motionDoneEvent());
                }

                motionStorage_[object].moves.clear();
            }

            if(! motionStorage_[object].motion.empty())
            {
                needCheck_.insert(object);
            }
        }

        return events;
    }

    template< typename Object, typename ObjectContainer >
    void ObjectMotionGestureChecker<Object, ObjectContainer>::clear()
    {
        gestures_.clear();
        needCheck_.clear();
        motionStorage_.clear();
    }

    template< typename Object, typename ObjectContainer >
    void ObjectMotionGestureChecker<Object, ObjectContainer>::reset()
    {
        for(auto objectRecord : motionStorage_)
        {
            MotionRecord& record = objectRecord.second;

            record.motion.clear();
            record.moves.clear();
            record.lastRecordedMotionTime = Time();
        }
    }

    template< typename Object, typename ObjectContainer >
    void ObjectMotionGestureChecker<Object, ObjectContainer>::recordMotion_(Object* object)
    {
        MotionRecord& record = motionStorage_[object];

        Time now = object->updateTime();
        if(! record.motion.empty() && now - record.lastRecordedMotionTime > timeout_)
        {
            Gesture gesture(object, record.motion);
            typename Gesture::Size gestureSize = gesture.originalSize();

            std::cout << gestureSize.width << " " << gestureSize.height << " " << MINIMAL_GESTURE_SIDE_FRACTION << std::endl;

            std::size_t minimalGestureSide = std::max(object->maxPosition().x, object->maxPosition().y) / MINIMAL_GESTURE_SIDE_FRACTION;
            if(std::max(gestureSize.width, gestureSize.height) > minimalGestureSide)
            {
                record.moves = gesture.moves();
                motionDone_(record);
            }

            record.motion.clear();
        }

        if(object->isVisible())
        {
            if(record.motion.empty() || distance(object->absolutePosition(), record.motion.back()) > NOT_MOTION_TOLERANCE)
            {
                record.motion.push_back(object->absolutePosition());
                record.lastRecordedMotionTime = now;
            }
        }
    }

    template< typename Object, typename ObjectContainer >
    void ObjectMotionGestureChecker<Object, ObjectContainer>::motionDone_(MotionRecord&)
    {
    }

    template< typename T >
    std::size_t levenshteinDistance(const std::vector<T>& left, const std::vector<T>& right, std::size_t maxDistance)
    {
        std::size_t width = right.size();
        std::size_t height = left.size();

        if((std::size_t)std::abs((int)width - (int)height) > maxDistance)
        {
            // distance is at least maxDistance + 1
            return maxDistance + 1;
        }

        std::size_t table[height + 1][width + 1];

        std::size_t col = 0;
        std::size_t row = 0;

        for(col = 0; col < std::min(maxDistance + 1, width + 1); ++col)
        {
            table[0][col] = col;
        }

        for(row = 0; row < std::min(maxDistance + 1, height + 1); ++row)
        {
            table[row][0] = row;
        }

        for(row = 1; row < height + 1; ++row)
        {
            std::size_t colBegin = std::max((int)row - (int)maxDistance, 1);
            std::size_t colEnd = std::min(row + maxDistance + 1, width + 1);

            //std::cout << std::endl;

            if(colBegin > 1)
            {
                table[row][colBegin - 1] = -1;
            }

            if(colEnd < width + 1)
            {
                table[row][colEnd] = -1;
            }

            for(col = colBegin; col < colEnd; ++col)
            {
                if(left[row - 1] == right[col - 1])
                {
                    table[row][col] = table[row - 1][col - 1];
                }
                else
                {
                    table[row][col] = std::min(
                            table[row - 1][col - 1],
                            std::min(
                                table[row - 1][col],
                                table[row][col - 1]
                            )
                    ) + 1;
                }

                //std::cout << table[row][col] << " ";
            }
            //std::cout << std::endl;
        }

        return table[height][width];
    }
} // namespace Gecon
