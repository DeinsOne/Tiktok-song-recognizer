#ifndef __SERVICES_CONFIG_HPP
#define __SERVICES_CONFIG_HPP

#include "Config.hpp"

namespace Bot {

    class TtDownloaderService : public IExternalService {
    public:
        virtual Json::Value Get(const std::shared_ptr<Json::Value>& rootconfig) override {
            std::lock_guard<std::mutex> _lock(*m_mutext);
            return (*m_rootconfig)["tt-downloader"];
        }

        const std::string GetHost() {
            return Get(m_rootconfig)["host"].asString();
        }

    };

    class RecognizerService : public IExternalService {
    public:
        virtual Json::Value Get(const std::shared_ptr<Json::Value>& rootconfig) override {
            std::lock_guard<std::mutex> _lock(*m_mutext);
            return (*m_rootconfig)["song-recognizer"];
        }

        const std::string GetHost() {
            return Get(m_rootconfig)["host"].asString();
        }

    };

} // namespace Bot


#endif // __SERVICES_CONFIG_HPP
