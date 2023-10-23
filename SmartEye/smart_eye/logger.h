#pragma once
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <memory>

class Logging
{
private:
    static std::shared_ptr<spdlog::logger> logger;

public:
    Logging(Logging &other) = delete;
    void operator=(const Logging &) = delete;
    static std::shared_ptr<spdlog::logger> getFile();
};
