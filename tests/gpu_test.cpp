#include <gtest/gtest.h>

#include "gpu/context.hpp"

TEST(Gpu, HeaderVersionIsSet) {
  EXPECT_GT(astra::gpu::headerVersion(), 0U);
}
