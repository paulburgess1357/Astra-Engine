#pragma once

#include <cstdint>
#include <memory>
#include <string>

struct GLFWwindow;

namespace astra::platform {

inline constexpr std::uint32_t kDefaultWidth = 1920;
inline constexpr std::uint32_t kDefaultHeight = 1080;

struct Extent2D {
  std::uint32_t width{};
  std::uint32_t height{};
};

struct WindowConfig {
  std::uint32_t width{kDefaultWidth};
  std::uint32_t height{kDefaultHeight};
  std::string title{"Astra Engine"};
  bool resizable{true};
};

// RAII wrapper around a GLFWwindow (no client API, for Vulkan). Needs a live Platform.
class Window {
 public:
  explicit Window(const WindowConfig& config);

  [[nodiscard]] auto shouldClose() const -> bool;
  auto requestClose() -> void;

  // Drawable size in pixels; zero while minimized.
  [[nodiscard]] auto framebufferSize() const -> Extent2D;

  [[nodiscard]] auto handle() const -> GLFWwindow* {
    return mWindow.get();
  }

 private:
  struct Deleter {
    auto operator()(GLFWwindow* window) const -> void;
  };

  std::unique_ptr<GLFWwindow, Deleter> mWindow;
};

}  // namespace astra::platform
