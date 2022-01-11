#ifndef __EVENT_MANAGER_HPP
#define __EVENT_MANAGER_HPP

#include "EventWorker.hpp"
#include <queue>

namespace Bot {

    // TODO: deligate event queue managing to external class apart from
    // EventManager
    class EventQueue {
    public:
        std::shared_ptr<IEventExecutable> PopEvent() {
            if (!size()) return nullptr;
            std::lock_guard<std::mutex> _lock(m_events_lock);
            auto event = m_events.front();
            m_events.pop();
            return event;
        }

        void AddEvent(const std::shared_ptr<IEventExecutable>& event) {
            std::lock_guard<std::mutex> _lock(m_events_lock);
            m_events.push(event);
        }

        std::size_t size() {
            std::lock_guard<std::mutex> _lock(m_events_lock);
            return m_events.size();
        }

    private:
        std::mutex m_events_lock;
        std::queue<std::shared_ptr<IEventExecutable>> m_events;
    };


    /**
     * EventManager is worker's master who regulates events execution.
     * It operates threads and queue which is initializing externally
     * by EventsHandler class
    */
    class EventManager {
    public:
        EventManager(const std::size_t& max_events, const std::chrono::seconds& max_timeout)
         : m_max_events(max_events), m_max_timeout(max_timeout) {
        }

        static std::shared_ptr<EventManager> make_manager(const std::size_t& max_events, const std::chrono::seconds& max_timeout) {
            return std::make_shared<EventManager>(max_events, max_timeout);
        }

        void AddEvent(const std::shared_ptr<IEventExecutable>& event) {
            // TRACE("Addevent. queue: {} workers: {}", m_events.size(), m_workers.size());
            m_events.AddEvent(event);
            UpdateWorkers();
        }

        void _bindBot(const std::shared_ptr<TgBot::Bot>& bot) {
            m_bot = bot;
        }

    private:
        void UpdateWorkers() {
            std::lock_guard<std::mutex> _lock(m_workers_lock);
            m_workers.erase(
                std::remove_if(m_workers.begin(), m_workers.end(), [&](std::shared_ptr<EventWorker>& worker) { return worker->IsDone(m_max_timeout); }),
                m_workers.end()
            );
            m_workers.shrink_to_fit();

            if (m_events.size()) {
                while (m_events.size() && m_workers.size() < m_max_events) {
                    m_workers.emplace_back(std::make_shared<EventWorker>(m_events.PopEvent(), m_bot, [&](){UpdateWorkers();}));
                }
            }
        }

    private:
        std::size_t m_max_events;
        std::chrono::seconds m_max_timeout;
        std::shared_ptr<TgBot::Bot> m_bot;

        EventQueue m_events;

        std::mutex m_workers_lock;
        std::vector<std::shared_ptr<EventWorker>> m_workers;

    };

} // namespace Bot


#endif // __EVENT_MANAGER_HPP
