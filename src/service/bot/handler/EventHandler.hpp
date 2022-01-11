#ifndef __EVENT_HANDLER_HPP
#define __EVENT_HANDLER_HPP

#include <memory>
#include "EventObject.hpp"
#include "Event.hpp"
#include "../manager/EventManager.hpp"

#include <tgbot/Bot.h>

namespace Bot {

    class EventHandler {
    public:
        EventHandler(const std::shared_ptr<EventManager>& eventmanager) : m_eventmanager(eventmanager) {
        }

        static std::shared_ptr<EventHandler> make_handler(const std::shared_ptr<EventManager>& eventmanager) {
            return std::make_shared<EventHandler>(eventmanager);
        }

    public:
        template<typename TEvent>
        void AddEvent() {
            auto event = new TEvent;

            if (auto ptr = dynamic_cast<EventMessage*>(event))
                m_events_message.emplace_back(ptr);
            if (auto ptr = dynamic_cast<EventQuery*>(event))
                m_events_query.emplace_back(ptr);
        }

    public:
        void _processMessage(const std::shared_ptr<IEventObject<Message>>& object);

        void _processQuery(const std::shared_ptr<IEventObject<Query>>& object);


    private:
        std::shared_ptr<EventManager> m_eventmanager;

        std::vector<std::shared_ptr<EventMessage>> m_events_message;
        std::vector<std::shared_ptr<EventQuery>> m_events_query;

    };

} // namespace Bot

#endif // __EVENT_HANDLER_HPP
