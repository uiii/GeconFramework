#ifndef GECON_EVENT_HPP
#define GECON_EVENT_HPP

#include <list>
#include <functional>

namespace Gecon
{
    class Event
    {
    public:
        class Trigger
        {
        public:
            class Switch
            {
            public:
                Switch(Event* onEvent, Event* offEvent, Trigger *trigger);
                virtual ~Switch();

                void on();
                void off();

                bool isOn() const;

            private:
                bool isOn_;

                Event* onEvent_;
                Event* offEvent_;

                Trigger* trigger_;
            };

            /*template< typename... Objects >
            using Action = std::function<void(const Objects&...)>;*/

            template< typename Action, typename... Objects >
            Trigger(Action, Objects*...); // TODO object type - takhle prijme cokoli

            ~Trigger();

            void addSwitch(Event* onEvent);
            void addSwitch(Event* onEvent, Event* offEvent);

        private:
            template< typename Object >
            Object copy_dereference_(Object* object) const;

            void check_();
            void reset_();

            std::function<void()> action_;

            std::list<Switch*> switches_;
        };

        Event();
        Event(const Event& another);

        virtual ~Event();

        void raise() const;

    private:
        std::list<Trigger::Switch*> onSwitches_;
        std::list<Trigger::Switch*> offSwitches_;
    };
} // namespace Gecon

#include "private/Event.tpp"

#endif // GECON_EVENT_HPP
