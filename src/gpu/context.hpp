#pragma once

#include <cstdint>

namespace astra::gpu {

// Vulkan header version this build was compiled against (VK_HEADER_VERSION_COMPLETE).
[[nodiscard]] std::uint32_t headerVersion();

}  // namespace astra::gpu
