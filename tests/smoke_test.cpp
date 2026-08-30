#include <gtest/gtest.h>

#include <cstdlib>
#include <string>

#ifndef ASTRA_ENGINE_PATH
#error "ASTRA_ENGINE_PATH must be provided by CMake"
#endif

TEST(AstraSmoke, ApplicationStartsSuccessfully) {
  const auto command = std::string{"\""} + ASTRA_ENGINE_PATH + "\"";
  EXPECT_EQ(std::system(command.c_str()), 0);
}
