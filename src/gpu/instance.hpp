#pragma once

#include <optional>
#include <span>
#include <string>
#include <vulkan/vulkan_raii.hpp>

namespace astra::gpu {

#ifdef NDEBUG
inline constexpr bool kValidationDefault = false;
#else
inline constexpr bool kValidationDefault = true;
#endif

struct InstanceConfig {
  std::string appName{"Astra Engine"};
  // Instance extensions the window system needs (e.g. from glfwGetRequiredInstanceExtensions).
  std::span<const char* const> requiredExtensions;
  // Enables VK_LAYER_KHRONOS_validation and a debug messenger routed to the engine log.
  bool enableValidation{kValidationDefault};
};

// RAII wrapper around a vk::Instance plus its optional debug messenger.
class Instance {
 public:
  explicit Instance(const InstanceConfig& config);

  [[nodiscard]] auto handle() const -> const vk::raii::Instance& {
    return mInstance;
  }

  [[nodiscard]] auto validationEnabled() const -> bool {
    return mMessenger.has_value();
  }

 private:
  vk::raii::Context mContext;
  vk::raii::Instance mInstance;
  std::optional<vk::raii::DebugUtilsMessengerEXT> mMessenger;
};

}  // namespace astra::gpu
