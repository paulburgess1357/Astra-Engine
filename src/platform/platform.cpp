#include "platform/platform.hpp"

#include <GLFW/glfw3.h>

#include <cstdint>
#include <span>
#include <vector>

#include "core/error.hpp"
#include "core/log.hpp"

namespace astra::platform {

namespace {

auto onGlfwError(int code, const char* description) -> void {
  ASTRA_ERROR("GLFW error {:#x}: {}", code, description);
}

}  // namespace

bool Platform::mAlive = false;

Platform::Platform(Backend backend) {
  if (mAlive) {
    throw core::Error("Only one platform::Platform may exist at a time");
  }

  glfwSetErrorCallback(onGlfwError);
  if (backend == Backend::Headless) {
    glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_NULL);
  }
  if (glfwInit() != GLFW_TRUE) {
    throw core::Error("glfwInit failed");
  }
  mAlive = true;
  ASTRA_INFO("GLFW {} initialized ({})", glfwGetVersionString(), backend == Backend::Headless ? "headless" : "native");
}

Platform::~Platform() {
  glfwTerminate();
  mAlive = false;
}

auto Platform::pollEvents() -> void {
  glfwPollEvents();
}

auto Platform::requiredVulkanExtensions() -> std::vector<const char*> {
  std::uint32_t count = 0;
  auto* names = glfwGetRequiredInstanceExtensions(&count);
  if (names == nullptr) {
    throw core::Error("Platform does not support Vulkan surface creation");
  }
  const std::span<const char*> span{names, count};
  return {span.begin(), span.end()};
}

}  // namespace astra::platform
