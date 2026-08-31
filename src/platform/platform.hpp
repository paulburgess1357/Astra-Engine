#pragma once

namespace astra::platform {

// Initializes the windowing/input backend (GLFW). Returns false on failure.
[[nodiscard]] auto initialize() -> bool;
auto shutdown() -> void;

}  // namespace astra::platform
