#include "../ColorObjectSet.hpp"

namespace Gecon
{
    template< typename ColorSpace > config_variable<std::size_t> ColorObjectSet<ColorSpace>::Y_RANGE = 80;
    template< typename ColorSpace > config_variable<std::size_t> ColorObjectSet<ColorSpace>::CB_RANGE = 20;
    template< typename ColorSpace > config_variable<std::size_t> ColorObjectSet<ColorSpace>::CR_RANGE = 20;

    template< typename ColorSpace >
    ColorObjectSet<ColorSpace>::ColorObjectSet():
        yMap_(256, Bitset()),
        cbMap_(256, Bitset()),
        crMap_(256, Bitset()),
        capacity_(0)
    {
    }

    template< typename ColorSpace >
    ColorObjectSet<ColorSpace>::~ColorObjectSet()
    {
    }

    template< typename ColorSpace >
    typename ColorObjectSet<ColorSpace>::Object* ColorObjectSet<ColorSpace>::find(typename ColorObjectSet<ColorSpace>::Color color) const
    {
        Bitset bitset(capacity_);
        bitset.set();
        bitset &= yMap_[color.y];
        bitset &= cbMap_[color.cb];
        bitset &= crMap_[color.cr];

        Object* object = 0;
        if(bitset.any())
        {
            object = objects_.at(bitset.find_first());
        }

        return object;
    }

    template< typename ColorSpace >
    void ColorObjectSet<ColorSpace>::insert(ColorObjectSet<ColorSpace>::Object *object)
     {
        checkCollision_(object);

        if(size() + 1 > capacity_)
        {
            reserve_();
        }

        setObjectInMap_(size(), object->color());

        objects_.push_back(object);
    }

    template< typename ColorSpace >
    void ColorObjectSet<ColorSpace>::update(ColorObjectSet<ColorSpace>::Object *object)
    {
        checkCollision_(object);

        typename ObjectVector::iterator it = std::find(objects_.begin(), objects_.end(), object);
        if(it == objects_.end())
        {
            return;
        }

        clearObjectInMap_(it - objects_.begin());
        setObjectInMap_(it - objects_.begin(), object->color());
    }

    template< typename ColorSpace >
    void ColorObjectSet<ColorSpace>::remove(ColorObjectSet<ColorSpace>::Object* object)
    {
        typename ObjectVector::iterator it = std::find(objects_.begin(), objects_.end(), object);
        if(it == objects_.end())
        {
            return;
        }

        objects_.erase(it);

        rebuild_();
    }

    template< typename ColorSpace >
    typename ColorObjectSet<ColorSpace>::const_iterator ColorObjectSet<ColorSpace>::begin() const
    {
        return objects_.begin();
    }

    template< typename ColorSpace >
    typename ColorObjectSet<ColorSpace>::const_iterator ColorObjectSet<ColorSpace>::end() const
    {
        return objects_.end();
    }

    template< typename ColorSpace >
    std::size_t ColorObjectSet<ColorSpace>::size() const
    {
        return objects_.size();
    }

    template< typename ColorSpace >
    void ColorObjectSet<ColorSpace>::clear()
    {
        capacity_ = 0;
        yMap_ = ComponentMap(256, Bitset());
        cbMap_ = ComponentMap(256, Bitset());
        crMap_ = ComponentMap(256, Bitset());

        objects_.clear();
    }

    template< typename ColorSpace >
    typename ColorObjectSet<ColorSpace>::ComponentRange ColorObjectSet<ColorSpace>::applyRange_(typename ColorObjectSet<ColorSpace>::Component component, std::size_t range)
    {
        int rangeBegin = component - range;
        int rangeEnd = component + range;

        if(rangeBegin < 0) rangeBegin = 0;
        if(rangeEnd > 255) rangeEnd = 255;

        return std::make_pair<Component, Component>(rangeBegin, rangeEnd);
    }

    template< typename ColorSpace >
    void ColorObjectSet<ColorSpace>::checkCollision_(ColorObjectSet<ColorSpace>::Object *object)
    {
        Color color = object->color();
        auto yRange = applyRange_(color.y, Y_RANGE);
        auto cbRange = applyRange_(color.cb, CB_RANGE);
        auto crRange = applyRange_(color.cr, CR_RANGE);

        for(std::size_t i = 0; i < 8; ++i)
        {
            // check all corners of color cuboid neighbourhood in the color map
            Component y = i % 2 ? yRange.first : yRange.second;
            Component cb = i/2 % 2 ? cbRange.first : cbRange.second;
            Component cr = i/4 % 2 ? crRange.first : crRange.second;

            Object* foundObject = find(Color({y, cb, cr}));
            if(foundObject && foundObject != object) // second condition used when updating object
            {
                throw std::logic_error("Too similar object already exists.");
            }
        }
    }

    template< typename ColorSpace >
    void ColorObjectSet<ColorSpace>::clearObjectInMap_(std::size_t index)
    {
        Bitset bitset(capacity_);
        bitset.set();
        bitset.set(index, false);

        auto clearObject = [&bitset](Bitset& set){ set &= bitset; };

        std::for_each(yMap_.begin(), yMap_.end(), clearObject);
        std::for_each(cbMap_.begin(), cbMap_.end(), clearObject);
        std::for_each(crMap_.begin(), crMap_.end(), clearObject);
    }

    template< typename ColorSpace >
    void ColorObjectSet<ColorSpace>::setObjectInMap_(std::size_t index, ColorObjectSet<ColorSpace>::Color color)
    {
        Bitset bitset(capacity_);
        bitset.set(index, true);

        auto yMapRange = applyRange_(color.y, Y_RANGE);
        auto cbMapRange = applyRange_(color.cb, CB_RANGE);
        auto crMapRange = applyRange_(color.cr, CR_RANGE);

        auto addObject = [&bitset](Bitset& set){ set |= bitset; };

        std::for_each(yMap_.begin() + yMapRange.first, yMap_.begin() + yMapRange.second, addObject);
        std::for_each(cbMap_.begin() + cbMapRange.first, cbMap_.begin() + cbMapRange.second, addObject);
        std::for_each(crMap_.begin() + crMapRange.first, crMap_.begin() + crMapRange.second, addObject);
    }

    template< typename ColorSpace >
    void ColorObjectSet<ColorSpace>::reserve_()
    {
        auto extendBitset = [](Bitset& set){ set.append(0); };

        std::for_each(yMap_.begin(), yMap_.end(), extendBitset);
        std::for_each(cbMap_.begin(), cbMap_.end(), extendBitset);
        std::for_each(crMap_.begin(), crMap_.end(), extendBitset);

        capacity_ += Bitset::bits_per_block;
    }

    template< typename ColorSpace >
    void ColorObjectSet<ColorSpace>::rebuild_()
    {
        capacity_ = objects_.size() / Bitset::bits_per_block + 1;
        yMap_ = ComponentMap(256, Bitset(capacity_));
        cbMap_ = ComponentMap(256, Bitset(capacity_));
        crMap_ = ComponentMap(256, Bitset(capacity_));

        for(std::size_t i = 0; i < objects_.size(); ++i)
        {
            setObjectInMap_(i, objects_.at(i)->color());
        }
    }
} // namespace Gecon
