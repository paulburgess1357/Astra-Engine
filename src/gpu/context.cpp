#include "gpu/context.hpp"

#include <cstdint>
#include <vulkan/vulkan.hpp>

namespace astra::gpu {

auto headerVersion() -> std::uint32_t {
  return vk::HeaderVersionComplete;
}

}  // namespace astra::gpu
