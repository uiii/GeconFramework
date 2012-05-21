#ifndef GECON_ACTIONTRIGGER_HPP
#define GECON_ACTIONTRIGGER_HPP

#include <set>
#include <map>
#include <functional>

namespace Gecon
{
    template< typename Event >
    class ActionTrigger
    {
    public:
        typedef std::function<void()> Action;
        typedef std::set<Event*> Events;

        ActionTrigger(Action action, bool repeted = false);
        virtual ~ActionTrigger();

        void addSwitch(Event* onEvent); // TODO remove - default offEvent
        void addSwitch(Event* onEvent, Event* offEvent);

        Events events();
        void eventOccured(Event* event);

        void check();
        bool needCheck() const;

        bool repeted() const;

        void reset();

    private:
        class Switch
        {
        public:
            Switch();

            void on();
            void off();

            bool isOn() const;

        private:
            bool isOn_;
        };

        typedef std::set<Switch*> Switches;
        typedef std::map<Event*, std::set<Switch*> > EventSwitches;

        Action action_;

        bool repeted_;

        Switches switches_;
        Events events_;

        EventSwitches onEventSwitches_;
        EventSwitches offEventSwitches_;
    };
} // namespace Gecon

#include "private/ActionTrigger.tpp"

#endif // GECON_ACTIONTRIGGER_HPP
