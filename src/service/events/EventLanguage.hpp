#ifndef __EVENT_LANGUAGE_HPP
#define __EVENT_LANGUAGE_HPP

#include "../bot/handler/Event.hpp"
#include "../utils-bot.hpp"
#include "../Users.h"

#include <tgbot/tgbot.h>
#include "../Locale.hpp"

namespace Bot {
namespace Events {

    class EventLanguage : public EventQuery {
    public:
        EventLanguage() = default;
        EventLanguage(const std::shared_ptr<IEventObject<Query>>& eventobject) : EventQuery(eventobject) {}  

        virtual bool Check(const std::shared_ptr<IEventObject<Query>>& eventobject) override {
            return StringTools::startsWith(eventobject->GetData(), "language_");
        }

        virtual void Handle(const std::shared_ptr<TgBot::Bot>& bot) override {
            std::string newlang = utils::eraseAny(m_eventobject->GetData(), "language_");

            if (!UsersDatabase::CheckUser(User(m_eventobject->GetImpl()->from))) {
                auto usr = User(m_eventobject->GetImpl()->from);
                usr._language = newlang;
                UsersDatabase::AddUser(usr);
            }
            else {
                UsersDatabase::SetUserLang(User(m_eventobject->GetImpl()->from), newlang);
            }

            bot->getApi().deleteMessage(m_eventobject->GetImpl()->from->id, m_eventobject->GetImpl()->message->messageId);
            bot->getApi().sendMessage(m_eventobject->GetImpl()->from->id, Locale::GetPhrases()[newlang]["bot_can"].asString());
        }

        virtual std::shared_ptr<IEvent<IEventObject<Query>>> make_copy(const std::shared_ptr<IEventObject<Query>>& eventobject) override {
            return std::make_shared<EventLanguage>(eventobject);
        }

    };

} // namespace Events
} // namespace Bot


#endif // __EVENT_LANGUAGE_HPP
