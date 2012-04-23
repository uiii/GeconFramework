#include "Event.hpp"

#include <iostream>

namespace Gecon
{
    Event::Trigger::Switch::Switch(Event *onEvent, Event *offEvent, Event::Trigger* trigger):
        onEvent_(onEvent),
        offEvent_(offEvent),
        trigger_(trigger)
    {
        if(onEvent_)
        {
            onEvent_->onSwitches_.push_back(this);
        }

        if(offEvent_)
        {
            offEvent_->offSwitches_.push_back(this);
        }

        if(trigger_)
        {
            trigger_->switches_.push_back(this);
        }
    }

    Event::Trigger::Switch::~Switch()
    {
        if(onEvent_)
        {
            onEvent_->onSwitches_.remove(this);
        }

        if(offEvent_)
        {
            offEvent_->offSwitches_.remove(this);
        }

        if(trigger_)
        {
            trigger_->switches_.remove(this);
        }
    }

    void Event::Trigger::Switch::on()
    {
        isOn_ = true;

        if(trigger_)
        {
            trigger_->check_();
        }
    }

    void Event::Trigger::Switch::off()
    {
        isOn_ = false;
    }

    bool Event::Trigger::Switch::isOn() const
    {
        return isOn_;
    }

    Event::Trigger::~Trigger()
    {
        // copy pointers to swichtes, because deletion of switch
        // removes it from switches_ list
        std::list<Switch*> deleteList(switches_);

        for(Switch* s : deleteList)
        {
            delete s;
        }
    }

    void Event::Trigger::addSwitch(Event *onEvent)
    {
        std::cout << "before: " << switches_.size() << std::endl;
        new Switch(onEvent, 0, this);
        std::cout << "after: " << switches_.size() << std::endl;
    }

    void Event::Trigger::addSwitch(Event *onEvent, Event *offEvent)
    {
        new Switch(onEvent, offEvent, this);
    }

    void Event::Trigger::check_()
    {
        bool allOn = true;

        for(Switch* s : switches_)
        {
            allOn = allOn && s->isOn();
        }

        if(allOn)
        {
            action_();
            reset_();
        }
    }

    void Event::Trigger::reset_()
    {
        for(Switch* s : switches_)
        {
            s->off();
        }
    }

    Event::Event()
    {
    }

    Event::Event(const Event &another)
    {
    }

    Event::~Event()
    {
        // copy pointers to swichtes, because deletion of switch
        // may remove it from onSwitches_ or offSwitches_ list
        std::list<Trigger::Switch*> deleteList;
        deleteList.insert(deleteList.end(), onSwitches_.begin(), onSwitches_.end());
        deleteList.insert(deleteList.end(), offSwitches_.begin(), offSwitches_.end());

        for(Trigger::Switch* s : deleteList)
        {
            delete s;
        }
    }

    void Event::raise() const
    {
        for(Trigger::Switch* s : onSwitches_)
        {
            s->on();
        }

        for(Trigger::Switch* s : offSwitches_)
        {
            s->on();
        }
    }
} // namespace Gecon
