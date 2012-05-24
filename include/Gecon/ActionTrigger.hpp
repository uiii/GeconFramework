#ifndef GECON_ACTIONTRIGGER_HPP
#define GECON_ACTIONTRIGGER_HPP

#include <set>
#include <map>
#include <functional>

namespace Gecon
{
    /**
     * Represents action trigger
     */
    template< typename Event >
    class ActionTrigger
    {
    public:
        typedef std::function<void()> Action;
        typedef std::set<Event*> Events;

        /**
         * Contruct action trigger.
         *
         * @param action
         *     Action to perform if all switches are on.
         *
         * @param repeted
         *     Tells if action is repeted.
         */
        ActionTrigger(Action action, bool repeted = false);
        virtual ~ActionTrigger();

        /**
         * Add switch.
         *
         * Switch has one event that switches it on
         * and may have one event thet switches it off.
         *
         * @param onEvent
         *    Event that switches switch on.
         *
         * @param offEvent
         *    Event that switches switch off. (Optional)
         */
        void addSwitch(Event* onEvent, Event* offEvent = 0);

        /**
         * Get all events incidenting with this switch.
         */
        Events events();
        
        /**
         * Apply event on trigger's switches.
         *
         * @param event
         *     Event to apply.
         */
        void eventOccured(Event* event);

        /**
         * Check if all switches are on
         * and perform action.
         */
        void check();

        /**
         * Return true if trigger needs check.
         * It is when action is repeted.
         */
        bool needCheck() const;

        /**
         * Return true if action is repeted.
         */
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
