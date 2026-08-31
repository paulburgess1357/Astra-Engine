#include <spdlog/spdlog.h>

#include "core/log.hpp"
#include "core/version.hpp"
#include "platform/platform.hpp"
#include "renderer/renderer.hpp"

auto main() -> int {
  astra::core::initLogging();
  SPDLOG_INFO("{} starting", astra::core::kEngineName);

  if (!astra::platform::initialize()) {
    return 1;
  }
  astra::renderer::renderFrame();
  astra::platform::shutdown();
  return 0;
}
