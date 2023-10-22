#include "Logger.h"


using namespace std;
shared_ptr<spdlog::logger> Logging::logger = nullptr;
shared_ptr<spdlog::logger> Logging::getFile()
{
	if (logger == nullptr) {
		logger = spdlog::basic_logger_mt("logger", "log.txt");
		spdlog::set_default_logger(logger);
	}
	return logger;
}