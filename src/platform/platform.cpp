#include "platform/platform.hpp"

#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>

#include "core/error.hpp"

namespace astra::platform {

namespace {

void onGlfwError(int code, const char* description) {
  SPDLOG_ERROR("GLFW error {:#x}: {}", code, description);
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
}

Platform::~Platform() {
  glfwTerminate();
  mAlive = false;
}

auto Platform::pollEvents() -> void {
  glfwPollEvents();
}

}  // namespace astra::platform
