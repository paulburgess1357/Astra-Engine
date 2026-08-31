#pragma once

#include <cstdint>

namespace astra::gpu {

// Vulkan header version this build was compiled against (VK_HEADER_VERSION_COMPLETE).
[[nodiscard]] auto headerVersion() -> std::uint32_t;

}  // namespace astra::gpu
