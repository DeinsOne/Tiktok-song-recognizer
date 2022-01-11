#pragma once

#include <string>
#include <random>
#include <algorithm>
#include <chrono>
#include <sstream>
#include <iomanip>
#include <json/json.h>
#include <fstream>

namespace utils {

    inline std::string _toLower(const std::string& convert) {
        std::string rt;
        std::for_each(convert.begin(), convert.end(), [&](const char& c) { rt += ::tolower(c); } );
        return rt;
    }

    inline std::string _toUpper(const std::string& convert) {
        std::string rt;
        std::for_each(convert.begin(), convert.end(), [&](const char& c) { rt += ::toupper(c); } );
        return rt;
    }

    inline std::string eraseMatch(const std::string& str, const std::string& sub) {
        std::string rt = str;
        size_t pos = rt.find(sub);
        while (pos != std::string::npos) { rt.erase(pos, sub.length()); pos = rt.find(sub); }
        return rt;
    }

    inline std::string eraseAny(const std::string& str, const std::string& sub) {
        std::string rt = str;
        size_t pos = _toLower(rt).find(_toLower(sub));
        while (pos != std::string::npos) { rt.erase(pos, sub.length()); pos = _toLower(rt).find(_toLower(sub)); }
        return rt;
    }

    inline std::vector<std::size_t> findEvery(const std::string& str, const std::string& sub) {
        std::vector<std::size_t> positions;
        std::size_t pos = str.find(sub, 0);
        while(pos != std::string::npos) { positions.push_back(pos); pos = str.find(sub,pos+1); }
        return positions;
    }

    inline std::vector<std::string> splitString(const std::string& _str, const std::string& delimeter) {
        std::string str = _str;
        std::vector<std::string> splittedStrings = {};
        size_t pos = 0;
        while ((pos = str.find(delimeter)) != std::string::npos) {
            std::string token = str.substr(0, pos);
            if (token.length() > 0)
                splittedStrings.push_back(token);
            str.erase(0, pos + delimeter.length());
        }
        if (str.length() > 0)
            splittedStrings.push_back(str);
        return splittedStrings;
    }

    inline std::string generateRandomId(std::size_t length) {
        static const std::string chars("qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM1234567890");

        static const std::size_t charsLen = chars.length();
        std::string result;

        std::random_device randomDevice;
        std::mt19937 randomSeed(randomDevice());
        std::uniform_int_distribution<int> generator(0, charsLen - 1);

        for (std::size_t i = 0; i < length; ++i) {
            result += chars[generator(randomSeed)];
        }
        return result.c_str();
    }

    /**
     * @param timeString string with time format: %Y-%m-%d  %H:%M:%S
    */
    inline std::chrono::seconds getTimeSince(const std::string& timeString) {
        tm tm = {};
        std::stringstream ss(timeString);
        ss >> std::get_time(&tm, "%Y-%m-%d  %H:%M:%S");

        std::chrono::system_clock::time_point then = std::chrono::system_clock::from_time_t(mktime(&tm));

        return std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch() - then.time_since_epoch());
    }

    inline std::string getDateTime(const std::chrono::minutes& append = std::chrono::minutes(0)) {
        std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        now += std::chrono::duration_cast<std::chrono::seconds>(append).count();

        std::string s(30, '\0');
        std::strftime(&s[0], s.size(), "%Y-%m-%d %H:%M:%S", std::localtime(&now));
        return s;
    }


} // namespace utils

namespace utils {
    // Json utils
    inline std::string to_string(const Json::Value& value) {
        Json::FastWriter writer;
        auto result = writer.write(value);
        if (result.back() == '\n') result.erase(result.end()-1);
        return result;
    }

    inline Json::Value to_json(const std::string& str) {
        Json::Value value;
        std::stringstream sstr(str);
        sstr >> value;
        return value;
    }

    inline std::string readFile(const std::string& path) {
        std::stringstream str;
        std::ifstream stream(path);
        if (stream.is_open()) {
            while(stream.peek() != EOF) str << (char) stream.get();
            stream.close();
            return str.str();
        }
        return "";
    }

}