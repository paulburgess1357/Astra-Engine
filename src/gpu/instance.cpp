#include "gpu/instance.hpp"

#include <vulkan/vk_platform.h>

#include <algorithm>
#include <cstdint>
#include <format>
#include <span>
#include <string>
#include <string_view>
#include <vector>
#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_raii.hpp>

#include "core/error.hpp"
#include "core/log.hpp"

namespace astra::gpu {

namespace {

constexpr auto kApiVersion = vk::ApiVersion13;
constexpr const char* kValidationLayer = "VK_LAYER_KHRONOS_validation";

VKAPI_ATTR auto VKAPI_CALL onDebugMessage(
    vk::DebugUtilsMessageSeverityFlagBitsEXT severity,
    vk::DebugUtilsMessageTypeFlagsEXT /*type*/,
    const vk::DebugUtilsMessengerCallbackDataEXT* data,
    void* /*userData*/
) -> vk::Bool32 {
  using Severity = vk::DebugUtilsMessageSeverityFlagBitsEXT;

  const auto* message = (data != nullptr && data->pMessage != nullptr) ? data->pMessage : "(no message)";
  switch (severity) {
    case Severity::eError:
      ASTRA_ERROR("[vulkan] {}", message);
      break;
    case Severity::eWarning:
      ASTRA_WARN("[vulkan] {}", message);
      break;
    case Severity::eInfo:
      ASTRA_DEBUG("[vulkan] {}", message);
      break;
    case Severity::eVerbose:
      ASTRA_TRACE("[vulkan] {}", message);
      break;
  }
  return vk::False;
}

auto makeMessengerInfo() -> vk::DebugUtilsMessengerCreateInfoEXT {
  using Severity = vk::DebugUtilsMessageSeverityFlagBitsEXT;
  using Type = vk::DebugUtilsMessageTypeFlagBitsEXT;

  return vk::DebugUtilsMessengerCreateInfoEXT{}
      .setMessageSeverity(Severity::eWarning | Severity::eError)
      .setMessageType(Type::eGeneral | Type::eValidation | Type::ePerformance)
      .setPfnUserCallback(onDebugMessage);
}

auto hasLayer(std::span<const vk::LayerProperties> available, std::string_view name) -> bool {
  return std::ranges::any_of(available, [name](const auto& layer) -> bool { return std::string_view{layer.layerName.data()} == name; });
}

auto hasExtension(std::span<const vk::ExtensionProperties> available, std::string_view name) -> bool {
  return std::ranges::any_of(available, [name](const auto& ext) -> bool { return std::string_view{ext.extensionName.data()} == name; });
}

auto joinNames(std::span<const char* const> names) -> std::string {
  std::string joined;
  for (const auto* name : names) {
    joined += joined.empty() ? "" : ", ";
    joined += name;
  }
  return joined;
}

auto versionString(std::uint32_t version) -> std::string {
  return std::format("{}.{}.{}", vk::apiVersionMajor(version), vk::apiVersionMinor(version), vk::apiVersionPatch(version));
}

// Instance layers/extensions chosen for this run, resolved against what the loader offers.
struct InstanceSetup {
  std::vector<const char*> layers;
  std::vector<const char*> extensions;
  vk::InstanceCreateFlags flags;
  bool validation{false};
};

auto requireLoaderVersion(const vk::raii::Context& context) -> std::uint32_t {
  const auto loaderVersion = context.enumerateInstanceVersion();
  if (loaderVersion < kApiVersion) {
    throw core::Error("Vulkan loader supports {} but {} is required", versionString(loaderVersion), versionString(kApiVersion));
  }
  return loaderVersion;
}

auto validationAvailable(std::span<const vk::LayerProperties> layers, std::span<const vk::ExtensionProperties> extensions) -> bool {
  if (!hasLayer(layers, kValidationLayer)) {
    ASTRA_WARN("{} not available; validation disabled", kValidationLayer);
    return false;
  }
  if (!hasExtension(extensions, vk::EXTDebugUtilsExtensionName)) {
    ASTRA_WARN("{} not available; validation disabled", vk::EXTDebugUtilsExtensionName);
    return false;
  }
  return true;
}

auto resolveSetup(const vk::raii::Context& context, const InstanceConfig& config) -> InstanceSetup {
  const auto availableLayers = context.enumerateInstanceLayerProperties();
  const auto availableExtensions = context.enumerateInstanceExtensionProperties();

  InstanceSetup setup;
  setup.extensions.assign(config.requiredExtensions.begin(), config.requiredExtensions.end());

  setup.validation = config.enableValidation && validationAvailable(availableLayers, availableExtensions);
  if (setup.validation) {
    setup.layers.push_back(kValidationLayer);
    setup.extensions.push_back(vk::EXTDebugUtilsExtensionName);
  }

  // MoltenVK (macOS) is a non-conformant portability implementation and is only enumerated when asked.
  if (hasExtension(availableExtensions, vk::KHRPortabilityEnumerationExtensionName)) {
    setup.extensions.push_back(vk::KHRPortabilityEnumerationExtensionName);
    setup.flags |= vk::InstanceCreateFlagBits::eEnumeratePortabilityKHR;
  }

  for (const auto* extension : setup.extensions) {
    if (!hasExtension(availableExtensions, extension)) {
      throw core::Error("Required Vulkan instance extension {} is not available", extension);
    }
  }
  return setup;
}

auto createInstance(const vk::raii::Context& context, const InstanceConfig& config, const InstanceSetup& setup) -> vk::raii::Instance {
  const auto appInfo = vk::ApplicationInfo{}
                           .setPApplicationName(config.appName.c_str())
                           .setApplicationVersion(vk::makeApiVersion(0U, 0U, 1U, 0U))
                           .setPEngineName("Astra")
                           .setEngineVersion(vk::makeApiVersion(0U, 0U, 1U, 0U))
                           .setApiVersion(kApiVersion);

  // Chaining the messenger info covers vkCreateInstance/vkDestroyInstance themselves.
  const auto messengerInfo = makeMessengerInfo();
  const auto createInfo = vk::InstanceCreateInfo{}
                              .setPNext(setup.validation ? &messengerInfo : nullptr)
                              .setFlags(setup.flags)
                              .setPApplicationInfo(&appInfo)
                              .setPEnabledLayerNames(setup.layers)
                              .setPEnabledExtensionNames(setup.extensions);

  return vk::raii::Instance{context, createInfo};
}

}  // namespace

Instance::Instance(const InstanceConfig& config)
    : mInstance{nullptr} {
  const auto loaderVersion = requireLoaderVersion(mContext);
  const auto setup = resolveSetup(mContext, config);

  mInstance = createInstance(mContext, config, setup);
  if (setup.validation) {
    mMessenger.emplace(mInstance, makeMessengerInfo());
  }

  ASTRA_INFO(
      "Vulkan instance created (API {}, loader {}, validation {}, extensions: {})",
      versionString(kApiVersion),
      versionString(loaderVersion),
      setup.validation ? "on" : "off",
      joinNames(setup.extensions)
  );
}

}  // namespace astra::gpu
