#pragma once

#include <vector>

namespace astra::platform {

enum class Backend {
  Native,    // OS windowing system
  Headless,  // GLFW null platform, for tests/CI
};

// RAII wrapper around glfw
class Platform {
 public:
  explicit Platform(Backend backend = Backend::Native);
  ~Platform();

  Platform(const Platform&) = delete;
  Platform(Platform&&) = delete;
  auto operator=(const Platform&) -> Platform& = delete;
  auto operator=(Platform&&) -> Platform& = delete;

  // Call once per frame.
  static auto pollEvents() -> void;

  // Vulkan instance extensions needed to create surfaces on this platform.
  // Throws if the platform has no Vulkan support (e.g. Headless).
  [[nodiscard]] static auto requiredVulkanExtensions() -> std::vector<const char*>;

 private:
  static bool mAlive;
};

}  // namespace astra::platform
