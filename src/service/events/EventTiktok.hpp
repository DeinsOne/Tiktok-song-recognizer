#ifndef __EVENT_TIKTOK_HPP
#define __EVENT_TIKTOK_HPP

#include "../bot/handler/Event.hpp"
#include "../utils-bot.hpp"

#include "../config/Config.hpp"
#include <httplib.h>

#include "Logger.h"
#include "url.c"


namespace Bot {
namespace Events {

    class EventTiktok : public EventMessage {
    public:
        EventTiktok() = default;
        EventTiktok(const std::shared_ptr<IEventObject<Message>>& eventobject) : EventMessage(eventobject) {}  

        virtual bool Check(const std::shared_ptr<IEventObject<Message>>& eventobject) override {
            for (auto entt : eventobject->GetImpl()->entities) {
                if (entt->type == "url") {
                    auto urltext = eventobject->GetData().substr(entt->offset, entt->length);
                    if (urltext.find("tiktok.com") != std::string::npos) return true;
                }
            }
            return false;
        }

        virtual void Handle(const std::shared_ptr<TgBot::Bot>& bot) override {
            std::vector<std::string> m_urls;

            // Shape urls list
            for (auto entt : m_eventobject->GetImpl()->entities) {
                if (entt->type == "url") {
                    auto urltext = m_eventobject->GetData().substr(entt->offset, entt->length);
                    if (urltext.find("tiktok.com") != std::string::npos) m_urls.emplace_back(urltext);
                }
            }

            // Unify urls
            std::sort(m_urls.begin(), m_urls.end());
            auto it = std::unique(m_urls.begin(), m_urls.end());
            m_urls.resize(std::distance(m_urls.begin(), it));

            // Process
            std::vector<Json::Value> results;
            for (auto url : m_urls) {
                results.emplace_back(recognizeSong(url));
            }

            // Shape responce
            std::size_t bad_result = 0;
            for (auto result : results) {
                if (result["status"]["msg"].asString() != "Success") { bad_result++; continue; }

                std::string text = "Song: " + result["metadata"]["music"][0]["title"].asString() + " " + result["metadata"]["music"][0]["label"].asString() + "\n\n";

                if (!result["metadata"]["music"][0]["external_metadata"].empty())
                    text += "https://youtu.be/" + result["metadata"]["music"][0]["external_metadata"]["youtube"]["vid"].asString();
                bot->getApi().sendMessage(m_eventobject->GetOwner()->id, text);
            }


            // last message
            auto user = UsersDatabase::GetUser(User(m_eventobject->GetOwner()));
            if (bad_result < results.size())
                bot->getApi().sendMessage(m_eventobject->GetOwner()->id, Locale::GetPhrases()[user._language]["result"].asString());
            else
                bot->getApi().sendMessage(m_eventobject->GetOwner()->id, Locale::GetPhrases()[user._language]["bad_result"].asString());

        }

        virtual std::shared_ptr<IEvent<IEventObject<Message>>> make_copy(const std::shared_ptr<IEventObject<Message>>& eventobject) override {
            return std::make_shared<EventTiktok>(eventobject);
        }

    private:
        Json::Value recognizeSong(const std::string& url);

    };


    Json::Value EventTiktok::recognizeSong(const std::string& url) {
        auto ttdownloader = Config::Get()->GetService<TtDownloaderService>();
        httplib::Client _ttclient(ttdownloader->GetHost().c_str());

        auto audio = _ttclient.Get("/v1/audio", httplib::Params{{"url",url}}, httplib::Headers{{}});

        if (audio.error() == httplib::Error::Connection) {
            ERROR("TtDownloaderService -> is unavailable 404");
            throw std::runtime_error("TtDownloaderService -> is unavailable 404");
        }
        else if ((int)audio.error() > 0) {
            ERROR("TtDownloaderService -> internall error");
            throw std::runtime_error("TtDownloaderService -> internall error");
        }

        auto audiourl = utils::to_json(audio->body);
        printf("  audio: %s\n", audiourl["audiourl"].asString().c_str());


        // Download audio
        auto parsedurl = url_parse((char*)audiourl["audiourl"].asCString());

        httplib::Client _downloader((std::string(parsedurl->protocol) + "://" + std::string(parsedurl->hostname)).c_str());
        _downloader.set_follow_location(true);
        auto audioresponce = _downloader.Get(parsedurl->path);

        if (audioresponce->get_header_value("content-type") != "audio/mpeg") throw std::runtime_error("EventTiktok -> audio downloading failed: " + httplib::to_string(audioresponce.error()));


        // Recognize
        auto recognizerservice = Config::Get()->GetService<RecognizerService>();
        httplib::Client _recognizer(recognizerservice->GetHost().c_str());
        _recognizer.set_follow_location(true);
        _recognizer.set_read_timeout(std::chrono::minutes(1));
        _recognizer.set_connection_timeout(std::chrono::minutes(1));


        auto form = httplib::MultipartFormDataItems{
            { "sample", audioresponce->body, "sound.mp3", "audio/mpeg" }
        };
        auto recognizerresponce = _recognizer.Post("/v1/identify", form);

        if ((int)recognizerresponce.error() > 0 || recognizerresponce->get_header_value("content-type") != "application/json") throw std::runtime_error("RecognizerService -> " + httplib::to_string(recognizerresponce.error()));

        std::ofstream recognized("tmp/recognized.json");
        recognized << recognizerresponce->body;
        recognized.close();

        return utils::to_json(recognizerresponce->body);
    }


} // namespace Events
} // namespace Bot


#endif // __EVENT_TIKTOK_HPP
