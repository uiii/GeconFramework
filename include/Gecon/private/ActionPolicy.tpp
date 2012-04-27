#include "../ActionPolicy.hpp"

#include <iostream>

namespace Gecon {
    
    template< typename Event >
    ActionPolicy<Event>::ActionPolicy()
    {
    }

    template< typename Event >
    void ActionPolicy<Event>::prepareActionTriggers(const ActionPolicy<Event>::ActionTriggers& triggers)
    {
        eventActionTriggers_.clear();

        for(ActionTrigger* trigger : triggers)
        {
            std::cout << "prepare trigger: " << trigger << std::endl;
            Events events = trigger->events();
            for(Event* event : events)
            {
                std::cout << "\tprepare event: " << event << std::endl;
                eventActionTriggers_[event].insert(trigger);
            }
        }
    }

    template< typename Event >
    void ActionPolicy<Event>::checkActionTriggers(const ActionPolicy<Event>::Events& events)
    {
        std::cout << "action check: " << events.size() << std::endl;
        ActionTriggers triggersToCheck;

        for(Event* event : events)
        {
            std::cout << "event: " << event << std::endl;
            ActionTriggers triggers = eventActionTriggers_[event];

            for(ActionTrigger* trigger : triggers)
            {
                std::cout << "\ttrigger: " << trigger << std::endl;
                trigger->eventOccured(event);
                if(trigger->needCheck())
                {
                    std::cout << "\t\tneed check" << std::endl;
                    triggersToCheck.insert(trigger);
                }
            }
        }

        for(ActionTrigger* trigger : triggersToCheck)
        {
            std::cout << "\tcheck trigger: " << trigger << std::endl;
            trigger->check();
        }
    }
} // namespace Gecon
