#include "core/log.hpp"

#include <spdlog/common.h>
#include <spdlog/spdlog.h>

namespace astra::core {

auto initLogging() -> void {
  spdlog::set_pattern("[%H:%M:%S.%e] [%^%l%$] [%t] %v");
  spdlog::set_level(static_cast<spdlog::level::level_enum>(SPDLOG_ACTIVE_LEVEL));
}

}  // namespace astra::core
