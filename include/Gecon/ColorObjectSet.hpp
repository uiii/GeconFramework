#ifndef GECON_COLOROBJECTSET_HPP
#define GECON_COLOROBJECTSET_HPP

#include <set>

#include <boost/dynamic_bitset.hpp>

#include "config_variable.hpp"
#include "ColorObject.hpp"

namespace Gecon
{
    /**
     * Container representing set of color objects.
     *
     * Provides fast lookup by color. 
     *
     * Can work with objects of any color space.
     * But stores them using YCbCr color space.
     */
    template< typename ColorSpace >
    class ColorObjectSet
    {
    public:
        static config_variable<std::size_t> Y_RANGE;
        static config_variable<std::size_t> CB_RANGE;
        static config_variable<std::size_t> CR_RANGE;

        typedef ColorObject<ColorSpace> Object;
        typedef std::vector<Object*> ObjectVector;

        typedef typename Object::Color Color;

        typedef typename ObjectVector::const_iterator const_iterator;

        ColorObjectSet();
        virtual ~ColorObjectSet();

        /**
         * Find object by color.
         *
         * @returns
         *    Object if found, otherwise NULL.
         */
        Object* find(Color color) const;

        /**
         * Insert new object into the set.
         *
         * For one color zone can be inserted only one object.
         * If too similar object (their color zones intersect)
         * is already in the set, throws an exception.
         *
         * @trows std::logic_error
         *     If too similar object already inserted.
         */
        void insert(Object* object);

        /**
         * Update object in the set.
         *
         * @warning Must be called when object's
         * color has been changed. Otherwise
         * find method will return wrong object.
         *
         * @param object
         *     Object to update.
         *
         * @trows std::logic_error
         *     If too similar object already inserted.
         */
        void update(Object* object);

        /**
         * Remove object from the set.
         *
         * If no such object is in the set,
         * nothing happens.
         */
        void remove(Object* object);

        const_iterator begin() const;
        const_iterator end() const;

        std::size_t size() const;

        void clear();

    private:
        typedef boost::dynamic_bitset<> Bitset;
        typedef std::vector<Bitset> ComponentMap;

        typedef typename ColorSpace::Component Component;
        typedef std::pair<Component, Component> ComponentRange;

        /**
         * Compute color zone range for specifed color component.
         */
        ComponentRange applyRange_(Component component, std::size_t range);

        /**
         * Check if there is no too similar object.
         *
         * @throws std::logic_error
         *     If there is such object.
         */
        void checkCollision_(Object* object);

        /**
         * Remove object indication from component maps.
         *
         * Unset bit with speecified index in indicators
         * at each level of component maps.
         *
         * @param index
         *     Index of bit in the indicators.
         */
        void clearObjectInMap_(std::size_t index);

        /**
         * Include object indication in component maps.
         *
         * Set bit with speecified index in indicators
         * at appropriate levels (corresponding to color zone ranges)
         * of component maps.
         *
         * @param index
         *     Index of bit in the indicators.
         *
         * @param color
         *     Color to compute color zone ranges.
         */
        void setObjectInMap_(std::size_t index, Color color);

        /**
         * Increase capacity of the indicators.
         */
        void reserve_();

        /**
         * Clear component maps and include again all objects.
         */
        void rebuild_();

        ComponentMap yMap_;
        ComponentMap cbMap_;
        ComponentMap crMap_;

        std::size_t capacity_;

        ObjectVector objects_;
    };
} // namespace Gecon

#include "private/ColorObjectSet.tpp"

#endif // GECON_COLOROBJECTSET_HPP
