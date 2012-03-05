#include "ColorObjectRecognition.hpp"

namespace Gecon
{
    template< typename Snapshot >
    ColorObjectRecognition::ObjectSet ColorObjectRecognition::recognizeObjects(const Snapshot &snapshot)
    {
        if(image_.width() != snapshot.width() || image_.height() != snapshot.height())
        {
            image_ = OutputImage(snapshot.width(), snapshot.height());
        }

        AreaBlockList lastRowBlocks;
        AreaBlockList currentRowBlocks;

        for(std::size_t row = 0; row < snapshot.height(); ++row)
        {
            createBlocks_(snapshot, row, currentRowBlocks);
            mergeBlocks_(lastRowBlocks, currentRowBlocks);

            std::swap(lastRowBlocks, currentRowBlocks);
        }

        return ObjectSet();
    }

    template< typename Snapshot >
    void ColorObjectRecognition::createBlocks_(Snapshot snapshot, std::size_t row, AreaBlockList &currentRowBlocks)
    {
        currentRowBlocks.clear();

        Object* lastObject = 0;

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
                Object* object = objects_.at(pixelBitset.find_first());

                if(object == lastObject)
                {
                    currentRowBlocks.back().end = column;
                }
                else
                {
                    currentRowBlocks.push_back(AreaBlock{0, object, row, column, column});
                }

                lastObject = object;

                image_.setAt(column, row, object->color());
            }
            else
            {
                lastObject = 0;

                image_.setAt(column, row, Gecon::Color<RGB>());
            }
        }
    }
} // namespace Gecon
