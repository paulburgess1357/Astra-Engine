#include "core/log.hpp"

#include <spdlog/common.h>
#include <spdlog/logger.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#include <memory>
#include <string>
#include <vector>

namespace astra::core {

namespace {

// Relative to the working directory; overwritten on each run.
constexpr const char* kLogFile = "astra.log";

}  // namespace

auto initLogging() -> void {
  std::vector<spdlog::sink_ptr> sinks{std::make_shared<spdlog::sinks::stdout_color_sink_mt>()};

  std::string fileError;
  try {
    sinks.push_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>(kLogFile, true));
  } catch (const spdlog::spdlog_ex& e) {
    fileError = e.what();
  }

  spdlog::set_default_logger(std::make_shared<spdlog::logger>("astra", sinks.begin(), sinks.end()));
  spdlog::set_pattern("[%H:%M:%S.%e] [%^%-8l%$] [id %t] %v");
  spdlog::set_level(static_cast<spdlog::level::level_enum>(SPDLOG_ACTIVE_LEVEL));
  // File output is buffered; flush on warnings so a crash keeps the lines that matter.
  spdlog::flush_on(spdlog::level::warn);

  if (!fileError.empty()) {
    ASTRA_WARN("Logging to console only: {}", fileError);
  }
}

}  // namespace astra::core
