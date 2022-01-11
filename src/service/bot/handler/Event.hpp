#ifndef __EVENT_HPP
#define __EVENT_HPP

#include <memory>
#include "EventObject.hpp"
#include "EventExecutable.h"

#include <tgbot/Bot.h>

namespace Bot {

    enum class EventType {
        EventType_None = 0,
        EventType_Message,
        EventType_Query
    };


    template<typename TEventObject>
    class IEvent : public IEventExecutable {
    public:
        IEvent() = default;
        IEvent(const std::shared_ptr<TEventObject>& eventobject);

        virtual std::shared_ptr<TEventObject> GetEventObject() = 0;
        // virtual EventType GetEventType() = 0;

        virtual std::shared_ptr<IEvent<TEventObject>> make_copy(const std::shared_ptr<TEventObject>& eventobject) = 0;

    public:
        // Check whater event compatible with given event object
        virtual bool Check(const std::shared_ptr<TEventObject>& eventobject) = 0;
    };


    class EventMessage : public IEvent<IEventObject<Message>> {
    public:
        EventMessage() = default;
        EventMessage(const std::shared_ptr<IEventObject<Message>>& eventobject) : m_eventobject(eventobject) {
        }

        virtual std::shared_ptr<IEventObject<Message>> GetEventObject() override { return m_eventobject; }
        // virtual EventType GetEventType() override { return EventType::EventType_Message; };

        virtual TgBot::User::Ptr GetOwner() override {
            return m_eventobject->GetOwner();
        }

        virtual TgBot::Chat::Ptr GetChat() override {
            return m_eventobject->GetChat();
        }

    protected:
        std::shared_ptr<IEventObject<Message>> m_eventobject;
    };


    class EventQuery : public IEvent<IEventObject<Query>> {
    public:
        EventQuery() = default;
        EventQuery(const std::shared_ptr<IEventObject<Query>>& eventobject) : m_eventobject(eventobject) {
        }

        virtual std::shared_ptr<IEventObject<Query>> GetEventObject() override { return m_eventobject; }
        // virtual EventType GetEventType() override { return EventType::EventType_Message; };

        virtual TgBot::User::Ptr GetOwner() override {
            return m_eventobject->GetOwner();
        }

        virtual TgBot::Chat::Ptr GetChat() override {
            return m_eventobject->GetChat();
        }

    protected:
        std::shared_ptr<IEventObject<Query>> m_eventobject;
    };


} // namespace Bot


#endif // __EVENT_HPP
