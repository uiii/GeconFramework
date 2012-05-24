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

            std::cout << "prepare trigger: " << trigger << std::endl;
            for(Event* event : trigger->events())
            {
                std::cout << "\tprepare event: " << event << std::endl;
                eventActionTriggers_[event].insert(trigger);
            }
        }
    }

    template< typename Event >
    void ActionPolicy<Event>::checkActionTriggers(const ActionPolicy<Event>::Events& events)
    {
        //std::cout << "action check: " << events.size() << std::endl;

        for(Event* event : events)
        {
            std::cout << "event: " << event << std::endl;
            ActionTriggers triggers = eventActionTriggers_[event];

            for(ActionTrigger* trigger : triggers)
            {
                std::cout << "\ttrigger: " << trigger << std::endl;
                trigger->eventOccured(event);
                triggersToCheck_.insert(trigger);
            }
        }

        for(ActionTrigger* trigger : triggersToCheck_)
        {
            std::cout << "\tcheck trigger: " << trigger << std::endl;
            trigger->check();
        }

        ActionTriggers needCheck;
        for(ActionTrigger* trigger : triggersToCheck_)
        {
            if(trigger->needCheck())
            {
                std::cout << "\t\tneed check" << std::endl;
                needCheck.insert(trigger);
            }
        }

        std::swap(triggersToCheck_, needCheck);
    }
} // namespace Gecon
