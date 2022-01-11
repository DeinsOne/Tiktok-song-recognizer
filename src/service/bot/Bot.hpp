#ifndef __BOT_HPP
#define __BOT_HPP

#include <string>

#include "BotThreading.hpp"
#include "manager/EventManager.hpp"
#include "handler/EventHandler.hpp"

namespace Bot {

    class Bot {
    public:
        Bot(const std::string& token,
            const std::shared_ptr<EventManager>& eventmanage,
            const std::shared_ptr<EventHandler>& eventhandler
        );

        static std::shared_ptr<Bot> make_bot(
            const std::shared_ptr<EventManager>& eventmanage,
            const std::shared_ptr<EventHandler>& eventhandler,
            const std::string& token
        ) {
            return std::make_shared<Bot>(token, eventmanage, eventhandler);
        }

        template<typename ThreadingType>
        void start() {
            if (m_executor) {
                m_executor->_stop();
                m_executor.reset();
            }
            m_executor = std::make_shared<ThreadingType>();
            m_executor->_start(m_bot);
        }

        void stop() { m_executor->_stop(); }

    protected:
        std::shared_ptr<EventManager> m_eventmanage;
        std::shared_ptr<EventHandler> m_eventhandler;

        std::shared_ptr<TgBot::Bot> m_bot;

    private:
        std::shared_ptr<__IBotThreadingType> m_executor;
    };

} // namespace Bot

#endif // __BOT_HPP
