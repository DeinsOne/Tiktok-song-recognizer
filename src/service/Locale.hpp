#ifndef __LOCALE_HPP
#define __LOCALE_HPP

#include <json/json.h>
#include "utils.hpp"

#include "Logger.h"

namespace Bot {

    using Phrases = Json::Value;

    class Locale {
    public:
        static void Init(const std::string& path);

        static Phrases GetPhrases();

    private:
        static Phrases m_phrases;
    };


    Phrases Locale::m_phrases;

    void Locale::Init(const std::string& path) {
        auto content = utils::readFile(path);
        if (content.empty()) CRITICAL("Locale -> bad locale file: {}", path);
        else m_phrases = utils::to_json(content);
    }

    Phrases Locale::GetPhrases() {
        return m_phrases;
    }

};


#endif // __LOCALE_HPP
