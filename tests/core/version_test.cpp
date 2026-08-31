#include "core/version.hpp"

#include <gtest/gtest.h>

TEST(Core, EngineName) {
  EXPECT_EQ(astra::core::kEngineName, "Astra");
}
