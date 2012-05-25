#include "../ActionPolicy.hpp"

#include <iostream>

namespace Gecon {
    
    template< typename Event >
    ActionPolicy<Event>::ActionPolicy()
    {
    }

    template< typename Event >
    ActionPolicy<Event>::ActionPolicy(const ActionPolicy<Event> &another)
    {
        prepareActionTriggers(another.triggers_);
    }

    template< typename Event >
    ActionPolicy<Event>& ActionPolicy<Event>::operator=(const ActionPolicy<Event>& another)
    {
        prepareActionTriggers(another.triggers_);

        return *this;
    }

    template< typename Event >
    void ActionPolicy<Event>::prepareActionTriggers(const ActionPolicy<Event>::ActionTriggers& triggers)
    {
        triggers_ = triggers;

        eventActionTriggers_.clear();
        triggersToCheck_.clear();

        for(ActionTrigger* trigger : triggers_)
        {
            trigger->reset();

            for(Event* event : trigger->events())
            {
                eventActionTriggers_[event].insert(trigger);
            }
        }
    }

    template< typename Event >
    void ActionPolicy<Event>::checkActionTriggers(const ActionPolicy<Event>::Events& events)
    {
        for(Event* event : events)
        {
            ActionTriggers triggers = eventActionTriggers_[event];

            for(ActionTrigger* trigger : triggers)
            {
                trigger->eventOccured(event);
                triggersToCheck_.insert(trigger);
            }
        }

        for(ActionTrigger* trigger : triggersToCheck_)
        {
            trigger->check();
        }

        ActionTriggers needCheck;
        for(ActionTrigger* trigger : triggersToCheck_)
        {
            if(trigger->needCheck())
            {
                needCheck.insert(trigger);
            }
        }

        std::swap(triggersToCheck_, needCheck);
    }
} // namespace Gecon
