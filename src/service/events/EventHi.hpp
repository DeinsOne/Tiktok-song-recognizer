#ifndef __EVENT_HI_HPP
#define __EVENT_HI_HPP

#include "../bot/handler/Event.hpp"
#include "../utils-bot.hpp"
#include "../utils.hpp"

#include "../Users.h"
#include "../Locale.hpp"

namespace Bot {
namespace Events {

    class EventHi : public EventMessage {
    public:
        EventHi() = default;
        EventHi(const std::shared_ptr<IEventObject<Message>>& eventobject) : EventMessage(eventobject) {}  

        virtual bool Check(const std::shared_ptr<IEventObject<Message>>& eventobject) override {
            return utils::_toLower(eventobject->GetData()) == "hi" || eventobject->GetData() == "Привет" || utils::_toLower(eventobject->GetData()) == "привет";
        }

        virtual void Handle(const std::shared_ptr<TgBot::Bot>& bot) override {
            auto user = UsersDatabase::GetUser(User(m_eventobject->GetOwner()));

            TgBot::InlineKeyboardMarkup::Ptr kb(new TgBot::InlineKeyboardMarkup);
            std::vector<TgBot::InlineKeyboardButton::Ptr> row;

            TgBot::InlineKeyboardButton::Ptr button(new TgBot::InlineKeyboardButton);
            button->text = Locale::GetPhrases()[user._language]["dev"].asString();
            button->url = "https://t.me/deinsone";

            row.emplace_back(button);
            kb->inlineKeyboard.emplace_back(row);

            bot->getApi().sendMessage(m_eventobject->GetImpl()->from->id, Locale::GetPhrases()[user._language]["secret"].asString(), false, 0, kb);
        }

        virtual std::shared_ptr<IEvent<IEventObject<Message>>> make_copy(const std::shared_ptr<IEventObject<Message>>& eventobject) override {
            return std::make_shared<EventHi>(eventobject);
        }

    };

} // namespace Events
} // namespace Bot


#endif // __EVENT_HI_HPP
