#include "Bot.hpp"
#include "Logger.h"

namespace Bot {

    Bot::Bot(const std::string& token,
        const std::shared_ptr<EventManager>& eventmanage,
        const std::shared_ptr<EventHandler>& eventhandler
    ) : m_eventmanage(eventmanage), m_eventhandler(eventhandler) {
        m_bot = std::make_shared<TgBot::Bot>(token);
        m_eventmanage->_bindBot(m_bot);

        m_bot->getEvents().onAnyMessage([&](TgBot::Message::Ptr message) {
            try {
                auto object = EventObjectMessage::make_object(message);
                m_eventhandler->_processMessage(object);
            }
            catch (const std::exception& e) {
                ERROR("While processing m_bot->getEvents().onAnyMessage() Error: usr[{}] {}", message->from->id, e.what());
            }
        });

        m_bot->getEvents().onCallbackQuery([&](TgBot::CallbackQuery::Ptr query) {
            try {
                auto object = EventObjectQuery::make_object(query);
                m_eventhandler->_processQuery(object);
            }
            catch (const std::exception& e) {
                ERROR("While processing m_bot->getEvents().onCallbackQuery() Error: usr[{}] {}", query->from->id, e.what());
            }
        });

    }

} // namespace Bot
