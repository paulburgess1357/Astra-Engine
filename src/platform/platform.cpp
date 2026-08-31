#include "platform/platform.hpp"

#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>

namespace astra::platform {

bool initialize() {
  if (glfwInit() != GLFW_TRUE) {
    SPDLOG_ERROR("glfwInit failed");
    return false;
  }
  return true;
}

void shutdown() {
  glfwTerminate();
}

}  // namespace astra::platform
