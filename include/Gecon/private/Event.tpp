#include "../Event.hpp"

namespace Gecon
{
    template< typename Action, typename... Objects >
    Event::Trigger::Trigger(Action action, Objects*... objects):
        action_(
            std::bind(action,
                std::bind(&Trigger::copy_dereference_<Objects>, this, objects)...
            )
        )
    {
    }

    template< typename Object >
    Object Event::Trigger::copy_dereference_(Object* object) const
    {
        return Object(*object);
    }
}
