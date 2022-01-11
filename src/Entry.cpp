#include <cstdio>

#include "Logger.h"
#include "service/config/Config.hpp"
#include "service/config/ServicesConfig.hpp"

#include "service/bot/Bot.hpp"
#include "service/bot/manager/EventManager.hpp"
#include "service/bot/handler/EventHandler.hpp"

#include "service/events/EventStart.hpp"
#include "service/events/EventLanguage.hpp"
#include "service/events/EventTiktok.hpp"


using namespace Bot;

int main(int argc, char** argv) {
    Logger::Logger::Init();

    Config::Get()->Init("../resources/config.json", std::chrono::minutes(60));
    UsersDatabase::Init("../resources/users.db");
    Locale::Init("../resources/locales.json");

    // Wait all threads deployment
    std::this_thread::sleep_for(std::chrono::seconds(1));

    // Build utility objects
    auto eventmanager = EventManager::make_manager(10, std::chrono::minutes(2));
    auto eventhandler = EventHandler::make_handler(eventmanager);

    // Add events
    eventhandler->AddEvent<Events::EventStart>();
    eventhandler->AddEvent<Events::EventLanguage>();
    eventhandler->AddEvent<Events::EventTiktok>();

    // Create bot
    auto bot = Bot::Bot::make_bot(eventmanager, eventhandler,
        "5038416978:AAHsU1LJzs4m3k645cqr3fd0GOrh04QHx4k"
    );

    // Start bot in the same thread
    bot->start<BotSync>();

    Config::Get()->Destroy();
    return 0;
}
