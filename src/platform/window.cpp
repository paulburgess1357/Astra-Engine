#include "platform/window.hpp"

#include <GLFW/glfw3.h>

#include <cstdint>

#include "core/error.hpp"

namespace astra::platform {

auto Window::Deleter::operator()(GLFWwindow* window) const -> void {
  glfwDestroyWindow(window);
}

Window::Window(const WindowConfig& config) {
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, config.resizable ? GLFW_TRUE : GLFW_FALSE);

  mWindow.reset(glfwCreateWindow(static_cast<int>(config.width), static_cast<int>(config.height), config.title.c_str(), nullptr, nullptr));
  if (!mWindow) {
    throw core::Error("Failed to create window \"{}\" ({}x{})", config.title, config.width, config.height);
  }
}

auto Window::shouldClose() const -> bool {
  return glfwWindowShouldClose(mWindow.get()) == GLFW_TRUE;
}

auto Window::requestClose() -> void {
  glfwSetWindowShouldClose(mWindow.get(), GLFW_TRUE);
}

auto Window::framebufferSize() const -> Extent2D {
  int width = 0;
  int height = 0;
  glfwGetFramebufferSize(mWindow.get(), &width, &height);
  return {.width = static_cast<std::uint32_t>(width), .height = static_cast<std::uint32_t>(height)};
}

}  // namespace astra::platform
