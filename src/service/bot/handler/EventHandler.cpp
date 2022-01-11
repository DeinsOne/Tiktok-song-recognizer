#include "EventHandler.hpp"

#include "Logger.h"

namespace Bot {

    void EventHandler::_processMessage(const std::shared_ptr<IEventObject<Message>>& object) {
        TRACE("EventHandler -> message usr[{}]: {} {}", object->GetImpl()->from->id, object->GetAttachmentType(), object->GetData());

        for (auto event : m_events_message) {
            // try {
                if (event->Check(object)) {
                    auto newevent = event->make_copy(object);
                    m_eventmanager->AddEvent(std::shared_ptr<IEventExecutable>(newevent));
                }
            // }
            // catch (const std::exception& e) {
                // ERROR("EventHandler -> message usr[{}] {}", event->GetOwner()->id, e.what() );
            // }
        }
    }

    void EventHandler::_processQuery(const std::shared_ptr<IEventObject<Query>>& object) {
        TRACE("EventHandler -> query usr[{}]: {}", object->GetImpl()->from->id, object->GetData());

        for (auto event : m_events_query) {
            // try {
                if (event->Check(object)) {
                    auto newevent = event->make_copy(object);
                    m_eventmanager->AddEvent(std::shared_ptr<IEventExecutable>(newevent));
                }
            // }
            // catch (const std::exception& e) {
                // ERROR("EventHandler -> query usr[{}] {}", event->GetOwner()->id, e.what() );
            // }
        }
    }

} // namespace Bot
