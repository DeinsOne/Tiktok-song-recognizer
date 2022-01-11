#ifndef __BOT_THREADING_HPP
#define __BOT_THREADING_HPP

#include <thread>
#include <mutex>

#include <tgbot/Bot.h>


namespace Bot {

    class __IBotThreadingType {
    public:
        virtual void _stop() = 0;
        virtual void _start(const std::shared_ptr<TgBot::Bot>& bot) = 0;
    };

    /**
     * BotAsync is used to execute bot in separate thread from main program.
     * Provides nonblocking bot execution
    */
    class BotAsync : public __IBotThreadingType {
    public:
        BotAsync() = default;

        virtual void _stop() override;

        virtual void _start(const std::shared_ptr<TgBot::Bot>& bot) override;

    private:
        bool m_alive;
        std::mutex m_lock;
        std::thread m_worker;
    };

    /**
     * BotSingle executes bot in the executor thread and blocks it's flow
    */
    class BotSync : public __IBotThreadingType {
    public:
        BotSync() = default;

        virtual void _stop() override;

        virtual void _start(const std::shared_ptr<TgBot::Bot>& bot) override;

    private:
        bool m_alive;
        std::mutex m_lock;
    };



} // namespace Bot


#endif // __BOT_THREADING_HPP
