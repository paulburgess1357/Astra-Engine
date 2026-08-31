#include <gtest/gtest.h>

#include "core/version.hpp"

TEST(Core, EngineName) {
  EXPECT_EQ(astra::core::kEngineName, "Astra");
}
