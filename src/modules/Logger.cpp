#include "Logger.h"

#include <vector>

#include "spdlog/async.h"
#include "spdlog/sinks/basic_file_sink.h"
#include <spdlog/sinks/stdout_color_sinks.h>
#include "spdlog/pattern_formatter.h"


namespace Logger {

	std::shared_ptr<spdlog::logger> Logger::_consoleLogger;
    std::shared_ptr<spdlog::logger> Logger::_fileLogger;

    void Logger::Init() {
		std::vector<spdlog::sink_ptr> logSinks;
		logSinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
		logSinks.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("tmp/runtime.log", true));

		logSinks[0]->set_pattern("%^[%T]  %v%$");
		logSinks[1]->set_pattern("[%T] [%l]  %v");

		_consoleLogger = std::make_shared<spdlog::logger>("core", begin(logSinks), end(logSinks));
		spdlog::register_logger(_consoleLogger);
		_consoleLogger->set_level(spdlog::level::trace);
		_consoleLogger->flush_on(spdlog::level::trace);

		_fileLogger = std::make_shared<spdlog::logger>("log", begin(logSinks), end(logSinks));
		spdlog::register_logger(_fileLogger);
		_fileLogger->set_level(spdlog::level::trace);
		_fileLogger->flush_on(spdlog::level::trace);
    }

}

