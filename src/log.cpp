// Copyright (C) Tubbles github.com/Tubbles

#include "log.hpp"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace lg {
void setup(fs::path &my_dir) {
    static const std::string log_filename{my_dir.string() + std::string{"/log.txt"}};

    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    // console_sink->set_level(spdlog::level::debug);
    console_sink->set_level(spdlog::level::info);

    auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(log_filename.c_str(), true);
    file_sink->set_level(spdlog::level::trace);
    spdlog::sinks_init_list sinks{console_sink, file_sink};

    auto logger = std::make_shared<spdlog::logger>("main", sinks);
    logger->set_level(spdlog::level::debug);
    spdlog::set_default_logger(logger);
}
} // namespace lg
