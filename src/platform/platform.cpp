#include "platform/platform.hpp"

#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>

namespace astra::platform {

auto initialize() -> bool {
  if (glfwInit() != GLFW_TRUE) {
    SPDLOG_ERROR("glfwInit failed");
    return false;
  }
  return true;
}

auto shutdown() -> void {
  glfwTerminate();
}

}  // namespace astra::platform
