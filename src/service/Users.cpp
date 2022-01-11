#include "Users.h"

#ifdef __linux__
    #include <experimental/filesystem>
    namespace fs = std::experimental::filesystem;
#endif

#include "utils.hpp"

namespace Bot {

    std::shared_ptr<SQLite::Database> UsersDatabase::_db;

    User::User(
        TgBot::User::Ptr user,
        const std::string& added
    ) {
        _user_id = std::to_string(user->id);
        _username = user->username;
        _language = "en";
        _added = added;
    }

    void UsersDatabase::Init(const std::string& path) {
        if (!fs::is_directory(fs::path(path).parent_path()))
            fs::create_directories(fs::path(path).parent_path());

        _db = std::make_shared<SQLite::Database>(path, SQLite::OPEN_CREATE | SQLite::OPEN_READWRITE | SQLite::OPEN_FULLMUTEX);

        if (!_db->tableExists("users")) {
            _db->exec(R"(
                CREATE TABLE users (
                    user_id         TEXT            PRIMARY KEY,
                    username        TEXT,
                    language        TEXT            DEFAULT('en'),
                    added           DATETIME        DEFAULT (CURRENT_TIMESTAMP)
                );
            )");
        }
    }



    void UsersDatabase::AddUser(const User& user) {
        SQLite::Statement insert(*_db, R"(
            INSERT INTO users (
                user_id,
                username,
                language,
                added
            )
            VALUES (?,?,?,?)
            ON CONFLICT(user_id) DO
            UPDATE SET
                username = excluded.username,
                added = excluded.added;
        )");

        insert.bind(1, user._user_id);
        insert.bind(2, user._username);
        insert.bind(3, user._language);
        insert.bind(4, utils::getDateTime());

        insert.exec();
    }

    bool UsersDatabase::CheckUser(const User& user) {
        SQLite::Statement select(*_db, "SELECT * FROM users WHERE user_id == ? OR username == ?;");
        select.bind(1, user._user_id);
        select.bind(2, user._username);

        if (select.executeStep()) return true;
        return false;
    }

    User UsersDatabase::GetUser(const User& user) {
        SQLite::Statement select(*_db, "SELECT * FROM users WHERE user_id == ? OR username == ?;");
        select.bind(1, user._user_id);
        select.bind(2, user._username);

        if (select.executeStep()) {
            User usr;
            usr._username = select.getColumn("username").getString();
            usr._user_id = select.getColumn("user_id").getUInt();
            usr._language = select.getColumn("language").getString();
            usr._added = select.getColumn("added").getString();
            return usr;
        }

        return User();
    }

    void UsersDatabase::SetUserLang(const User& user, const std::string language) {
        SQLite::Statement setLang(*_db, R"(
            UPDATE users SET language = ? WHERE user_id = ?;)");
        setLang.bind(1, language);
        setLang.bind(2, user._user_id);
        setLang.exec();
    }

} // namespace Endpoint

