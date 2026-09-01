#include <exception>

#include "core/log.hpp"
#include "gpu/instance.hpp"
#include "platform/platform.hpp"
#include "platform/window.hpp"
#include "renderer/renderer.hpp"

namespace {

auto run() -> void {
  const astra::platform::Platform platform;
  const astra::platform::Window window({});

  const auto extensions = astra::platform::Platform::requiredVulkanExtensions();
  const astra::gpu::Instance instance({.requiredExtensions = extensions});

  while (!window.shouldClose()) {
    astra::platform::Platform::pollEvents();
    astra::renderer::renderFrame();
  }
}

}  // namespace

auto main() -> int {
  astra::core::initLogging();
  ASTRA_INFO("Astra Engine starting");

  try {
    run();
  } catch (const std::exception& e) {
    ASTRA_CRITICAL("{}", e.what());
    return 1;
  }
  return 0;
}
