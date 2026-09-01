#include <spdlog/spdlog.h>

#include <exception>

#include "core/log.hpp"
#include "platform/platform.hpp"
#include "platform/window.hpp"
#include "renderer/renderer.hpp"

namespace {

auto run() -> void {
  const astra::platform::Platform platform;
  const astra::platform::Window window({});

  while (!window.shouldClose()) {
    astra::platform::Platform::pollEvents();
    astra::renderer::renderFrame();
  }
}

}  // namespace

auto main() -> int {
  astra::core::initLogging();
  SPDLOG_INFO("Astra Engine starting");

  try {
    run();
  } catch (const std::exception& e) {
    SPDLOG_CRITICAL("{}", e.what());
    return 1;
  }
  return 0;
}
