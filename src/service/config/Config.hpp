#ifndef __CONFIG_HPP
#define __CONFIG_HPP

#include <string>
#include <chrono>
#include <json/json.h>

#include <thread>
#include <mutex>

#include "../utils.hpp"
#include "Logger.h"

namespace Bot {

    /**
     * Interface for building service configurations. User can specify json node of service
     * and add methods to interact with proporties
    */
    class IExternalService {
    public:
        virtual void Init(
            const std::shared_ptr<Json::Value>& rootconfig,
            const std::shared_ptr<std::mutex>& mutext
        ) {
            m_rootconfig = rootconfig;
            m_mutext = mutext;
        }

        /**
         * FIXME: Thread unsafe 
         * Overloads Get method to simplify json access
        */
        virtual Json::Value operator[](const std::string& key)  {
            return Get(m_rootconfig)[key];
        }

        /**
         * @param root Is a main node - config file where api configuration located 
        */
        virtual Json::Value Get(const std::shared_ptr<Json::Value>& rootconfig) = 0;

    protected:
        std::shared_ptr<Json::Value> m_rootconfig; 
        std::shared_ptr<std::mutex> m_mutext;
    };


    /**
     * Singleton to control external apis. Provides configuration update
     * via worker thread with given interval
    */
    class Config {
    public:
        static Config* Get() {
            static Config* c = new Config;
            return c;
        }

        template<typename _Clock, typename _Dur>
        void Init(
            const std::string& path,
            const std::chrono::duration<_Clock, _Dur>& interval
        ) {
            std::string file = utils::readFile(path);
            auto json = utils::to_json(file.size() ? file : "{}");
            if (file.empty()) CRITICAL("Config -> configuration file is empty or has wrong path: {}", path);

            m_config = std::make_shared<Json::Value>(json);
            m_lock = std::make_shared<std::mutex>();
            m_interval = std::chrono::duration_cast<std::chrono::minutes>(interval);
            m_path = path;

            m_worker = std::thread([&]() {
                TRACE("Config -> enter worker");
                auto _next_tick = std::chrono::high_resolution_clock::now() + m_interval;

                while (m_alive) {
                    std::lock_guard<std::mutex> _lock(m_alive_mtx);  // Lock to avoid distructor
                    if (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::high_resolution_clock::now() - _next_tick).count() >= 0) {
                        _next_tick = std::chrono::high_resolution_clock::now() + m_interval;
                        Load();
                        TRACE("Config -> config updated");
                    }
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                }

                TRACE("Config -> exit worker");
            });
            m_worker.detach();

        }

        template<typename TApi>
        std::shared_ptr<TApi> GetService() {
            std::shared_ptr<TApi> api = std::make_shared<TApi>();
            api->Init(m_config, m_lock);
            return api;
        }

        void Destroy() {
            m_alive = false;
            std::lock_guard<std::mutex> _lock(m_alive_mtx);
        }

    private:
        Config() { }

        void Load() {
            std::lock_guard<std::mutex> _lock(*m_lock);
            std::string file = utils::readFile(m_path);
            auto json = utils::to_json(file.size() ? file : "{}");
            if (file.empty()) CRITICAL("Config -> configuration file is empty or has wrong path: {}", m_path);
            *m_config = json;
        }

    private:
        std::thread m_worker;
        std::chrono::minutes m_interval;
        std::string m_path;

        std::shared_ptr<Json::Value> m_config;
        std::shared_ptr<std::mutex> m_lock;

    private:
        bool m_alive = true;
        std::mutex m_alive_mtx;
    };

} // namespace Bot


#endif // __CONFIG_HPP
