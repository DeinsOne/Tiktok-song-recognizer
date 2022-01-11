#include "BotThreading.hpp"
#include "Logger.h"

#include <tgbot/tgbot.h>

namespace Bot {

    void BotAsync::_stop() {
        m_alive = false;
        std::lock_guard<std::mutex> _lock(m_lock);
        INFO("BotAsync -> stop bot. exit longpool");
    }

    void BotAsync::_start(const std::shared_ptr<TgBot::Bot>& bot) {
        m_worker = std::thread([&](const std::shared_ptr<TgBot::Bot>& _bot) {
            INFO("BotAsync -> start bot in asynchronous threading");
            INFO("BotAsync -> {} {}", bot->getApi().getMe()->username, bot->getApi().getMe()->id);
            m_alive = true;

            try {
                TgBot::TgLongPoll pool(*_bot, 1, 1);
                while (m_alive) {
                    std::lock_guard<std::mutex> _lock(m_lock);
                    pool.start();
                }
            }
            catch (const TgBot::TgException& e) {
                ERROR("BotAsync -> {}", e.what());
            }

        }, bot);
        m_worker.detach();
    }

} // namespace Bot
