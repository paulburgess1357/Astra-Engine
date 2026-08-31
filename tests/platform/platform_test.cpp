#include "platform/platform.hpp"

#include <gtest/gtest.h>

TEST(Platform, ShutdownWithoutInitializeIsSafe) {
  // glfwTerminate is documented as a no-op if GLFW is not initialized.
  astra::platform::shutdown();
}
