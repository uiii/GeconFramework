#ifndef GECON_ACTIONPOLICY_HPP
#define GECON_ACTIONPOLICY_HPP

#include <set>
#include <map>

#include "ActionTrigger.hpp"

namespace Gecon
{
    /**
     * Standard action policy
     */
    template< typename Event >
    class ActionPolicy
    {
    public:
        typedef std::set<Event*> Events;

        typedef Gecon::ActionTrigger<Event> ActionTrigger;
        typedef std::set<ActionTrigger*> ActionTriggers;

        ActionPolicy();
        ActionPolicy(const ActionPolicy<Event>& another);

        ActionPolicy<Event>& operator=(const ActionPolicy<Event>& another);

        /**
         * Prepare action triggers to control
         */
        void prepareActionTriggers(const ActionTriggers& triggers);

        /**
         * Check if some triggers have all switches.
         * And perform appropriate actions.
         *
         * @param events
         *     Events to switch triggers' switches
         */
        void checkActionTriggers(const Events& events);

    private:
        ActionTriggers triggers_;
        std::map<Event*, ActionTriggers> eventActionTriggers_;

        ActionTriggers triggersToCheck_;
    };
} // namespace Gecon

#include "private/ActionPolicy.tpp"

#endif // GECON_ACTIONPOLICY_HPP
