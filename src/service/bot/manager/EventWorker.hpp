#ifndef __EVENT_WORKER_HPP
#define __EVENT_WORKER_HPP

#include <memory>
#include <chrono>
#include <thread>

#include "../handler/EventExecutable.h"
#include "Logger.h"

namespace Bot {

    // This callback will be executing every time EventWorker get exception. Indicates whether given event has runtime hanling error 
    static inline void __onBadWorker(const std::exception& e, const std::shared_ptr<IEventExecutable>& m_event, const std::shared_ptr<TgBot::Bot>& bot) {
        try {
            // Send sticker via file_id
            auto sticker = bot->getApi().sendSticker(m_event->GetOwner()->id, "CAACAgIAAxkDAAIBIGHcAR55RsRQrmosELf4sxJkBfAUAAJeFQACf3rhSqPlqFeHamgtIwQ");
            bot->getApi().sendMessage(m_event->GetOwner()->id, "Some error ocurred. Please contact developer @deinsone 🥺");
            // To get file_id
            // auto sticker = bot->getApi().sendSticker(m_event->GetOwner()->id, TgBot::InputFile::fromFile("../resources/InternallError.tgs", "image/webp") );
            // printf("  sticker file_id: %s\n", sticker->sticker->fileId.c_str());
        }
        catch (const std::exception& ie) {
            ERROR("EventWorker -> sending sorry message usr[{}] {}", m_event->GetOwner()->id, e.what());  
        }
    }

    /**
     * Class doing event handling
    */
    class EventWorker {
    public:
        EventWorker(const std::shared_ptr<IEventExecutable>& event, const std::shared_ptr<TgBot::Bot>& bot, std::function<void()> callback);

        template<typename _Clock, typename _Dur>
        bool IsDone(const std::chrono::duration<_Clock, _Dur>& max_lifetime) {
            return std::chrono::high_resolution_clock::now() - m_initiated > max_lifetime || m_ready;
        }

    private:
        std::thread m_thread;
        std::chrono::high_resolution_clock::time_point m_initiated;
        bool m_ready;
        std::shared_ptr<IEventExecutable> m_event;
    };


    inline EventWorker::EventWorker(const std::shared_ptr<IEventExecutable>& event, const std::shared_ptr<TgBot::Bot>& bot, std::function<void()> callback) : m_event(event) {
        m_ready = false;
        m_thread = std::thread([&](const std::shared_ptr<TgBot::Bot>& _bot, std::function<void()> _callback) {
            m_initiated = std::chrono::high_resolution_clock::now();
            try {
                m_event->Handle(_bot);
            }
            catch (const std::exception& e) {
                ERROR("EventWorker -> usr[{}] {}", m_event->GetOwner()->id, e.what());

                #ifdef NOTIFY_USER_ON_ERROR
                  __onBadWorker(e, m_event, _bot);
                #endif // NOTIFY_USER_ON_ERROR
            }

            m_ready = true;
            _callback();
        }, bot, callback);
        m_thread.detach();
    }

} // namespace Bot


#endif // __EVENT_WORKER_HPP
