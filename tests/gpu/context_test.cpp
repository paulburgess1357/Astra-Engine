#include "gpu/context.hpp"

#include <gtest/gtest.h>

TEST(Gpu, HeaderVersionIsSet) {
  EXPECT_GT(astra::gpu::headerVersion(), 0U);
}
