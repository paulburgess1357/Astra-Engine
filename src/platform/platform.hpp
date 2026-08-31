#pragma once

namespace astra::platform {

// Initializes the windowing/input backend (GLFW). Returns false on failure.
[[nodiscard]] bool initialize();
void shutdown();

}  // namespace astra::platform
