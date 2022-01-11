#pragma once

#include <tgbot/types/User.h>
#include <SQLiteCpp/SQLiteCpp.h>

namespace Bot {

    struct User {
        std::string _user_id;
        std::string _username;
        std::string _language;
        std::string _added;

        User() {}

        User(
            TgBot::User::Ptr user,
            const std::string& added = ""
        );
    };

    class UsersDatabase {
    public:
        static void Init(const std::string& path);

    public:
        static bool CheckUser(const User& user);

        static void AddUser(const User& user);
        static User GetUser(const User& user);

        static void SetUserLang(const User& user, const std::string lang);

    private:
        static std::shared_ptr<SQLite::Database> _db;
    };

}
