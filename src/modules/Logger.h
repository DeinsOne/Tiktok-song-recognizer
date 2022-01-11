#ifndef _LOGGER
#define _LOGGER

#include <spdlog/spdlog.h>
#include <memory>

namespace Logger {

    class Logger {
    public:
        static void Init();

        static std::shared_ptr<spdlog::logger>& GetConsoleLogger() { return _consoleLogger; }
        static std::shared_ptr<spdlog::logger>& GetFileLogger() { return _fileLogger; }
    private:
        static std::shared_ptr<spdlog::logger> _consoleLogger;
        static std::shared_ptr<spdlog::logger> _fileLogger;
    };


}

#define CONSOLE_TRACE(...)    ::Logger::Logger::GetConsoleLogger()->trace(__VA_ARGS__)
#define CONSOLE_INFO(...)     ::Logger::Logger::GetConsoleLogger()->info(__VA_ARGS__)
#define CONSOLE_WARN(...)     ::Logger::Logger::GetConsoleLogger()->warn(__VA_ARGS__)
#define CONSOLE_ERROR(...)    ::Logger::Logger::GetConsoleLogger()->error(__VA_ARGS__)
#define CONSOLE_CRITICAL(...) ::Logger::Logger::GetConsoleLogger()->critical(__VA_ARGS__)

// Client log macros
#define TRACE(...)         ::Logger::Logger::GetFileLogger()->trace(__VA_ARGS__)
#define INFO(...)          ::Logger::Logger::GetFileLogger()->info(__VA_ARGS__)
#define WARN(...)          ::Logger::Logger::GetFileLogger()->warn(__VA_ARGS__)
#define ERROR(...)         ::Logger::Logger::GetFileLogger()->error(__VA_ARGS__)
#define CRITICAL(...)      ::Logger::Logger::GetFileLogger()->critical(__VA_ARGS__)

#endif // _PARSER_LOGGER
