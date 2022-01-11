#ifndef __EVENT_START_HPP
#define __EVENT_START_HPP

#include "../bot/handler/Event.hpp"
#include "../utils-bot.hpp"

namespace Bot {
namespace Events {

    class EventStart : public EventMessage {
    public:
        EventStart() = default;
        EventStart(const std::shared_ptr<IEventObject<Message>>& eventobject) : EventMessage(eventobject) {}  

        virtual bool Check(const std::shared_ptr<IEventObject<Message>>& eventobject) override {
            return eventobject->GetData() == "/start" ? true : false;
        }

        virtual void Handle(const std::shared_ptr<TgBot::Bot>& bot) override {
            std::vector<std::vector<std::pair<std::string,std::string>>> _opts;
            TgBot::InlineKeyboardMarkup::Ptr kb(new TgBot::InlineKeyboardMarkup);

            // TODO: Adding language options will make changes globally
            _opts.push_back( { { "🇬🇧 English", "language_en" } } );
            _opts.push_back( { { "🇷🇺 Русский", "language_ru" } } );

            utils::createInlineKeyboard(kb, _opts);

            bot->getApi().sendMessage(m_eventobject->GetImpl()->from->id, "Hi! Please select your preferred language", true, 0, kb, "HTML", true);
        }

        virtual std::shared_ptr<IEvent<IEventObject<Message>>> make_copy(const std::shared_ptr<IEventObject<Message>>& eventobject) override {
            return std::make_shared<EventStart>(eventobject);
        }

    };

} // namespace Events
} // namespace Bot


#endif // __EVENT_START_HPP
