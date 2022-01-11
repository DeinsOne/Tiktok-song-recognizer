#include "BotThreading.hpp"
#include "Logger.h"

#include <tgbot/tgbot.h>

namespace Bot {

    void BotSync::_stop() {
        m_alive = false;
        std::lock_guard<std::mutex> _lock(m_lock);
        INFO("BotSync -> stop bot. exit longpool");
    }

    void BotSync::_start(const std::shared_ptr<TgBot::Bot>& bot) {
        INFO("BotSync -> start bot in synchronous threading");
        INFO("BotSync -> {} {}", bot->getApi().getMe()->username, bot->getApi().getMe()->id);
        m_alive = true;

        try {
            TgBot::TgLongPoll pool(*bot, 1, 1);

            while (m_alive) {
                std::lock_guard<std::mutex> _lock(m_lock);
                pool.start();
            }
        }
        catch (const TgBot::TgException& e) {
            ERROR("BotSync -> {}", e.what());
        }
    }

} // namespace Bot
