#include <exception>

#include "core/log.hpp"
#include "gpu/instance.hpp"
#include "platform/platform.hpp"
#include "platform/window.hpp"
#include "renderer/renderer.hpp"

namespace astra {
namespace {

auto run() -> void {
  const platform::Platform platform;
  platform::Window window({});

  const auto extensions = platform::Platform::requiredVulkanExtensions();
  const gpu::Instance instance({.requiredExtensions = extensions});

  while (!window.shouldClose()) {
    platform::Platform::pollEvents();
    if (window.isKeyDown(platform::Key::Escape) || window.isKeyDown(platform::Key::Q)) {
      window.requestClose();
    }
    renderer::renderFrame();
  }
}

}  // namespace
}  // namespace astra

auto main() -> int {
  astra::core::initLogging();
  ASTRA_INFO("Astra Engine starting");

  try {
    astra::run();
  } catch (const std::exception& e) {
    ASTRA_CRITICAL("{}", e.what());
    return 1;
  }
  return 0;
}
