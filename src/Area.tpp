#include "Area.hpp"

#include <algorithm>

namespace Gecon
{
    template< typename Object >
    Area<Object>::Area(const AreaBlock &block):
        parentArea_(0),
        unionRank_(0),
        size_(0)
    {
        addBlock(block);
    }

    template< typename Object >
    void Area<Object>::addBlock(AreaBlock block)
    {
        block.area = this;

        if(blocks_.empty() || blocks_.back().row != block.row)
        {
            size_ += block.end - block.begin;
            blocks_.push_back(block);
        }
        else
        {
            size_ += block.end - blocks_.back().end;
            blocks_.back().end = block.end;
        }
    }

    template< typename Object >
    bool Area<Object>::nested() const
    {
        return parentArea_ != 0;
    }

    template< typename Object >
    std::size_t Area<Object>::size() const
    {
        includeSubAreas_();

        return size_;
    }

    template< typename Object >
    void Area<Object>::draw(Image<RGB>& image)
    {
        if(nested())
        {
            return;
        }

        includeSubAreas_();

        for(AreaBlock& block : blocks_)
        {
            for(std::size_t i = block.begin; i < block.end; ++i)
            {
                if(image.at(i, block.row) != Color<RGB>())
                {
                    image.setAt(i, block.row, Color<RGB>({255,255,0}));
                }
                else
                {
                    image.setAt(i, block.row, Color<RGB>({0,255,255}));
                }
            }
        }
    }

    template< typename Object >
    void Area<Object>::includeSubAreas_()
    {
        auto rowComparison = [](const AreaBlock& left, const AreaBlock& right) -> bool { return left.row > right.row; };

        AreaBlockHeap blockHeap;
        makeBlockHeap_(blockHeap, rowComparison);

        AreaBlockList newBlocks;
        std::size_t newSize = 0;

        while(! blockHeap.empty())
        {
            AreaBlock newBlock = blockHeap.front(); // currentBlock with minimal row in the heap

            if(newBlocks.empty() || newBlocks.back().row < newBlock.row)
            {
                newBlock.area = this;

                newBlocks.push_back(newBlock);
                newSize += newBlock.end - newBlock.begin;
            }
            else // blocks' rows are equal
            {
                AreaBlock& lastAddedBlock = newBlocks.back();

                std::size_t newBegin = std::min(lastAddedBlock.begin, newBlock.begin);
                std::size_t newEnd = std::max(lastAddedBlock.end, newBlock.end);

                newSize += lastAddedBlock.begin - newBegin;
                newSize += newEnd - lastAddedBlock.end;

                lastAddedBlock.begin = newBegin;
                lastAddedBlock.end = newEnd;
            }

            popBlockHeap_(blockHeap, rowComparison);
        }

        std::swap(blocks_, newBlocks);
        size_ = newSize;
    }

    template< typename Object >
    void Area<Object>::makeBlockHeap_(Area<Object>::AreaBlockHeap &blockHeap, const Area<Object>::AreaBlockComparison& comparison)
    {
        blockHeap.reserve(subAreas_.size() + 1); // allocate place for every subarea and one for this area

        for(AreaPtr subArea : subAreas_)
        {
            subArea->includeSubAreas_();

            if(! subArea->blocks_.empty())
            {
                blockHeap.push_back(subArea->blocks_.front());
                subArea->blocks_.pop_front();
            }
        }

        if(! blocks_.empty())
        {
            blockHeap.push_back(blocks_.front());
            blocks_.pop_front();
        }

        std::make_heap(blockHeap.begin(), blockHeap.end(), comparison);
    }

    template< typename Object >
    void Area<Object>::popBlockHeap_(Area<Object>::AreaBlockHeap &blockHeap, const Area<Object>::AreaBlockComparison& comparison)
    {
        std::pop_heap(blockHeap.begin(), blockHeap.end(), comparison);

        // add subarea's next block to heap if any
        AreaBlockList& areaBlocks = blockHeap.back().area->blocks_;
        if(! areaBlocks.empty())
        {
            blockHeap.back() = areaBlocks.front();
            areaBlocks.pop_front();

            std::push_heap(blockHeap.begin(), blockHeap.end(), comparison);
        }
        else
        {
            blockHeap.pop_back();
        }
    }

    template< typename Object >
    void joinAreas(Area<Object>* first, Area<Object>* second)
    {
        typename Area<Object>::AreaList compressList;

        // find first's root area
        Area<Object>* firstRoot = first;
        while(firstRoot->parentArea_)
        {
            compressList.push_back(firstRoot);
            firstRoot = firstRoot->parentArea_;
            assert(firstRoot != firstRoot->parentArea_);
        }

        // compress first's root successors
        while(! compressList.empty()) // TODO
        {
            Area<Object>* area = compressList.back();
            compressList.pop_back();

            if(area->parentArea_ != firstRoot) // last area in the list is already compressed
            {
                area->parentArea_ = firstRoot;
                firstRoot->subAreas_.push_back(area);
            }
        }

        // find second's root area
        Area<Object>* secondRoot = second;
        while(secondRoot->parentArea_)
        {
            compressList.push_back(secondRoot);
            secondRoot = secondRoot->parentArea_;
            assert(secondRoot != secondRoot->parentArea_);
        }

        // compress second's root successors
        while(! compressList.empty()) // TODO
        {
            Area<Object>* area = compressList.back();
            compressList.pop_back();

            if(area->parentArea_ != secondRoot) // last area in the list is already compressed
            {
                area->parentArea_ = secondRoot;
                secondRoot->subAreas_.push_back(area);
            }
        }

        // join roots if needed
        if(firstRoot != secondRoot) // first and second aren't subareas of the same area
        {
            if(firstRoot->unionRank_ < secondRoot->unionRank_)
            {
                secondRoot->subAreas_.push_back(firstRoot);
                firstRoot->parentArea_ = secondRoot;
            }
            else
            {
                firstRoot->subAreas_.push_back(secondRoot);
                secondRoot->parentArea_ = firstRoot;

                if(firstRoot->unionRank_ == secondRoot->unionRank_)
                {
                    ++(firstRoot->unionRank_);
                }
            }
        }
    }
} // namespace Gecon
