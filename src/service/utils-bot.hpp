#pragma once

#include "Logger.h"
#include <tgbot/tgbot.h>

namespace utils {

    inline void createReplyKeyboard(TgBot::ReplyKeyboardMarkup::Ptr& kb, const std::vector<std::string>& buttonLayout) {
        for (size_t i = 0; i < buttonLayout.size(); ++i) {
            TgBot::KeyboardButton::Ptr button(new TgBot::KeyboardButton);
            button->text = buttonLayout[i];
            kb->keyboard.push_back({button});
        }
    }

    inline void createReplyKeyboard(TgBot::ReplyKeyboardMarkup::Ptr& kb, const std::vector<std::vector<std::string>>& buttonLayout) {
        for (size_t i = 0; i < buttonLayout.size(); ++i) {
            std::vector<TgBot::KeyboardButton::Ptr> row;
            for (size_t j = 0; j < buttonLayout[i].size(); ++j) {
                TgBot::KeyboardButton::Ptr button(new TgBot::KeyboardButton);
                button->text = buttonLayout[i][j];
                row.push_back(button);
            }
            kb->keyboard.push_back(row);
        }
    }

    inline void createInlineKeyboard(TgBot::InlineKeyboardMarkup::Ptr& kb, const std::vector<std::vector<std::pair<std::string,std::string>>>& layout) {
        for (auto i : layout) {
            std::vector<TgBot::InlineKeyboardButton::Ptr> row;
            for (auto j : i) {
                TgBot::InlineKeyboardButton::Ptr button(new TgBot::InlineKeyboardButton);
                button->text = j.first;
                button->callbackData = j.second;
                row.emplace_back(button);
            }
            kb->inlineKeyboard.emplace_back(row);
        }
    }


    inline std::string formatBold(const std::string& str, const std::string parse_mode = "html") {
        auto mode = utils::_toLower(parse_mode);
        if (StringTools::startsWith(mode, "html")) {
            return "<b>" + str + "</b>";
        }
        else if (StringTools::startsWith(mode, "markdown")) {
            return "*" + str + "*";
        }

        return str;
    }

} // namespace Endpoint

