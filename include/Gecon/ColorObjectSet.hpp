#ifndef GECON_COLOROBJECTSET_HPP
#define GECON_COLOROBJECTSET_HPP

#include <set>

#include <boost/dynamic_bitset.hpp>

#include "config_variable.hpp"
#include "ColorObject.hpp"

namespace Gecon
{
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

        Object* find(Color color) const;

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
        */
        void update(Object* object);

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

        ComponentRange applyRange_(Component component, std::size_t range);
        void checkCollision(Object* object);
        void clearObjectInMap(std::size_t index);
        void setObjectInMap_(std::size_t index, Color color);


        void reserve_();
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
