#ifndef GECON_ACTIONPOLICY_HPP
#define GECON_ACTIONPOLICY_HPP

#include <set>
#include <map>

#include "ActionTrigger.hpp"

namespace Gecon
{
    template< typename Event >
    class ActionPolicy
    {
    public:
        typedef std::set<Event*> Events;

        typedef Gecon::ActionTrigger<Event> ActionTrigger;
        typedef std::set<ActionTrigger*> ActionTriggers;

        ActionPolicy();

        void prepareActionTriggers(const ActionTriggers& triggers);
        void checkActionTriggers(const Events& events);

    private:
        std::map<Event*, ActionTriggers> eventActionTriggers_;
    };
} // namespace Gecon

#include "private/ActionPolicy.tpp"

#endif // GECON_ACTIONPOLICY_HPP
