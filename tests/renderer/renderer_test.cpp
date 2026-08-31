#include "renderer/renderer.hpp"

#include <gtest/gtest.h>

TEST(Renderer, RenderFrameDoesNotThrow) {
  EXPECT_NO_THROW(astra::renderer::renderFrame());
}
