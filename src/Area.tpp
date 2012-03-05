#include "Area.hpp"

namespace Gecon
{
    template< typename Object >
    Area<Object>::Area(AreaBlock block):
        parentArea_(0),
        unionRank_(0)
    {
        addBlock(block);
    }

    template< typename Object >
    void Area<Object>::addBlock(AreaBlock block)
    {
        if(blocks_.empty() || blocks_.back().row != block.row)
        {
            blocks_.push_back(block);
        }
        else
        {
            blocks_.back().end = block.end;
        }
    }

    template< typename Object >
    void joinAreas(Area<Object>* first, Area<Object>* second)
    {
        // find first's root area
        Area<Object>* firstParent = first->parentArea_;
        while(firstParent)
        {
            firstParent = firstParent->parentArea_;
        }

        // find second's root area
        Area<Object>* secondParent = second->parentArea_;
        while(secondParent)
        {
            secondParent = secondParent->parentArea_;
        }

        if(firstParent != secondParent) // first and second aren't subareas of the same area
        {
            if(first->unionRank_ < second->unionRank_)
            {
                second->subAreas_.push_back(first);
                first->parentArea_ = second;
            }
            else
            {
                first->subAreas_.push_back(second);
                second->parentArea_ = first;

                if(first->unionRank_ == second->unionRank_)
                {
                    ++(first->unionRank_);
                }
            }
        }
    }
} // namespace Gecon
