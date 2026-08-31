#include "gpu/context.hpp"

#include <vulkan/vulkan.hpp>

namespace astra::gpu {

std::uint32_t headerVersion() {
  return VK_HEADER_VERSION_COMPLETE;
}

}  // namespace astra::gpu
