#include "../ActionTrigger.hpp"

#include <algorithm>

#include <iostream>

namespace Gecon
{
    template< typename Event >
    ActionTrigger<Event>::ActionTrigger(ActionTrigger<Event>::Action action, bool repeted):
        action_(action),
        repeted_(repeted)
    {
        std::cout << "con repeted: " << repeted_ << std::endl;
    }

    template< typename Event >
    ActionTrigger<Event>::~ActionTrigger()
    {
        for(Switch* s : switches_)
        {
            delete s;
        }
    }

    template< typename Event >
    void ActionTrigger<Event>::addSwitch(Event *onEvent, Event *offEvent)
    {
        bool switchOffAfterCheck = (offEvent == 0);

        Switch* s = new Switch(switchOffAfterCheck);

        switches_.insert(s);

        events_.insert(onEvent);
        onEventSwitches_[onEvent].insert(s);

        if(offEvent)
        {
            events_.insert(offEvent);
            offEventSwitches_[offEvent].insert(s);
        }
    }

    template< typename Event >
    typename ActionTrigger<Event>::Events ActionTrigger<Event>::events()
    {
        return events_;
    }

    template< typename Event >
    void ActionTrigger<Event>::eventOccured(Event *event)
    {
        for(Switch* s : onEventSwitches_[event])
        {
            s->on();
        }

        for(Switch* s : offEventSwitches_[event])
        {
            s->off();
        }
    }

    template< typename Event >
    void ActionTrigger<Event>::check()
    {
        bool allOn = true;

        for(Switch* s : switches_)
        {
            allOn = allOn && s->isOn();

            if(s->switchOffAfterCheck())
            {
                s->off();
            }
        }

        if(allOn)
        {
            std::cout << "perform action" << std::endl;
            action_();
        }
    }

    template< typename Event >
    bool ActionTrigger<Event>::needCheck() const
    {
        std::cout << "repeted: " << repeted_ << std::endl;
        return repeted_;
    }

    template< typename Event >
    bool ActionTrigger<Event>::repeted() const
    {
        return repeted_;
    }

    template< typename Event >
    void ActionTrigger<Event>::reset()
    {
        for(Switch* s : switches_)
        {
            s->off();
        }
    }


    template< typename Event >
    ActionTrigger<Event>::Switch::Switch(bool switchOffAfterCheck):
        isOn_(false),
        switchOffAfterCheck_(switchOffAfterCheck)
    {
    }

    template< typename Event >
    void ActionTrigger<Event>::Switch::on()
    {
        isOn_ = true;
    }

    template< typename Event >
    void ActionTrigger<Event>::Switch::off()
    {
        isOn_ = false;
    }

    template< typename Event >
    bool ActionTrigger<Event>::Switch::isOn() const
    {
        return isOn_;
    }

    template< typename Event >
    bool ActionTrigger<Event>::Switch::switchOffAfterCheck() const
    {
        return switchOffAfterCheck_;
    }
} // namespace Gecon
