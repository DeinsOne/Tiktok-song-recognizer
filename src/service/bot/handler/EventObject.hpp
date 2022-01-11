#ifndef __EVENT_OBJECT_HPP
#define __EVENT_OBJECT_HPP

#include <tgbot/types/Message.h>
#include <tgbot/types/CallbackQuery.h>

#include "EventHandler.hpp"
#include "Logger.h"

#include <tgbot/types/User.h>
#include <tgbot/types/Chat.h>

namespace Bot {

    /**
     * Wrapper for TgBot::MessagePtr & TgBot::InlineQuery::Ptr
     * T is wrapper type: std::shared_ptr<some type>
    */
    template<typename TShared>
    class IEventObject {
    public:
        virtual TShared GetImpl() = 0;

        /** 
         * @return String representation of object's text. Message returns text etc.
        */
        virtual std::string GetData() = 0;

        virtual std::string GetAttachmentType() = 0;

        virtual TgBot::User::Ptr GetOwner() = 0;

        virtual TgBot::Chat::Ptr GetChat() = 0;

    };

    using Message = TgBot::Message::Ptr;
    using Query = TgBot::CallbackQuery::Ptr;



    // Message event object
    class EventObjectMessage : public IEventObject<Message> {
    public:
        static std::shared_ptr<IEventObject<Message>> make_object(const Message& o) {
            auto impl = std::make_shared<EventObjectMessage>();
            impl->m_message = o;

            // Detect attachment
            if (impl->m_message->sticker) impl->m_attachmenttype = "[sticker]";
            if (impl->m_message->audio) impl->m_attachmenttype = "[audio]";
            if (impl->m_message->voice) impl->m_attachmenttype = "[voice]";
            if (impl->m_message->video) impl->m_attachmenttype = "[video]";
            if (impl->m_message->photo.size()) impl->m_attachmenttype = "[photo]";
            if (impl->m_message->document) impl->m_attachmenttype = "[document]";

            return impl;
        }

        virtual Message GetImpl() override {
            return m_message;
        }

        virtual std::string GetData() override {
            if (
                m_message->audio || m_message->video || 
                m_message->voice || m_message->document ||
                m_message->photo.size()
            ) return m_message->caption;

            return m_message->text;
        }

        virtual std::string GetAttachmentType() override {
            return m_attachmenttype;
        }

        virtual TgBot::User::Ptr GetOwner() override {
            return m_message->from;
        }

        virtual TgBot::Chat::Ptr GetChat() override {
            return m_message->chat;
        }


    private:
        Message m_message;
        std::string m_attachmenttype;
    };

    // Query event object
    class EventObjectQuery : public IEventObject<Query> {
    public:
        static std::shared_ptr<IEventObject<Query>> make_object(const Query& o) {
            auto impl = std::make_shared<EventObjectQuery>();
            impl->m_query = o;
            return impl;
        }

        virtual Query GetImpl() override {
            return m_query;
        }

        virtual std::string GetData() override {
            return m_query->data;
        }

        virtual std::string GetAttachmentType() override {
            return "";
        }

        virtual TgBot::User::Ptr GetOwner() override {
            return m_query->from;
        }

        virtual TgBot::Chat::Ptr GetChat() override {
            return nullptr;
        }

    private:
        Query m_query;
    };

} // namespace Bot

#endif // __EVENT_OBJECT_HPP
