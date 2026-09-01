#pragma once

#include <spdlog/spdlog.h>

#define ASTRA_TRACE(...) SPDLOG_TRACE(__VA_ARGS__)
#define ASTRA_DEBUG(...) SPDLOG_DEBUG(__VA_ARGS__)
#define ASTRA_INFO(...) SPDLOG_INFO(__VA_ARGS__)
#define ASTRA_WARN(...) SPDLOG_WARN(__VA_ARGS__)
#define ASTRA_ERROR(...) SPDLOG_ERROR(__VA_ARGS__)
#define ASTRA_CRITICAL(...) SPDLOG_CRITICAL(__VA_ARGS__)

namespace astra::core {

// Set logging pattern and level for spdlog
auto initLogging() -> void;

}  // namespace astra::core
