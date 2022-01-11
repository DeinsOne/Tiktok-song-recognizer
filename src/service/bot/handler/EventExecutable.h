#ifndef __EVENT_EXECUTABLE_HPP
#define __EVENT_EXECUTABLE_HPP

#include <memory>
#include <tgbot/Bot.h>

#include <tgbot/types/User.h>
#include <tgbot/types/Chat.h>

namespace Bot {

    class IEventExecutable {
    public:
        // Execute overridded task
        virtual void Handle(const std::shared_ptr<TgBot::Bot>& bot) = 0;

        virtual TgBot::User::Ptr GetOwner() = 0;

        virtual TgBot::Chat::Ptr GetChat() = 0;

    };

} // namespace Bot

#endif // __EVENT_EXECUTABLE_HPP
