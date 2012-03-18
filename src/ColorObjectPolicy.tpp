#include "ColorObjectPolicy.hpp"

#include <map>

namespace Gecon
{
    template< typename Snapshot >
    ColorObjectPolicy::ObjectList ColorObjectPolicy::recognizeObjects(const Snapshot &snapshot)
    {
        if(image_.width() != snapshot.width() || image_.height() != snapshot.height())
        {
            image_ = OutputImage(snapshot.width(), snapshot.height());
        }

        AreaBlockList lastRowBlocks;
        AreaBlockList currentRowBlocks;

        // create areas
        for(std::size_t row = 0; row < snapshot.height(); ++row)
        {
            createBlocks_(snapshot, row, currentRowBlocks);
            connectBlocks_(lastRowBlocks, currentRowBlocks);

            std::swap(lastRowBlocks, currentRowBlocks);
        }

        ObjectList visibleObjects;
        selectVisibleObjects_(visibleObjects);

        // remove all areas
        while(! areas_.empty())
        {
            delete areas_.front();
            areas_.pop_front();
        }

        return visibleObjects;
    }

    template< typename Snapshot >
    void ColorObjectPolicy::createBlocks_(Snapshot snapshot, std::size_t row, AreaBlockList &currentRowBlocks)
    {
        currentRowBlocks.clear();

        ObjectPtr lastObject = 0;

        for(std::size_t column = 0; column < snapshot.width(); ++column)
        {
            Color pixelColor = snapshot.at(column, row);

            Bitset pixelBitset(objects_.size());
            pixelBitset.set();
            pixelBitset &= yMap_[pixelColor.y];
            pixelBitset &= cbMap_[pixelColor.cb];
            pixelBitset &= crMap_[pixelColor.cr];

            if(pixelBitset.any())
            {
                ObjectPtr object = objects_.at(pixelBitset.find_first());

                if(object == lastObject)
                {
                    currentRowBlocks.back().end = column + 1;
                }
                else
                {
                    currentRowBlocks.push_back(AreaBlock{0, object, row, column, column + 1});
                }

                lastObject = object;

                image_.setAt(column, row, Gecon::Color<RGB>({255,255,255}));
            }
            else
            {
                lastObject = 0;

                image_.setAt(column, row, Gecon::Color<RGB>());
            }
        }
    }
} // namespace Gecon
