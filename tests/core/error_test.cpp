#include "core/error.hpp"

#include <gtest/gtest.h>

#include <stdexcept>
#include <string>

TEST(Error, CarriesMessage) {
  const astra::core::Error error("something broke");
  EXPECT_EQ(std::string{error.what()}, "something broke");
}

TEST(Error, IsCatchableAsRuntimeError) {
  EXPECT_THROW(throw astra::core::Error("boom"), std::runtime_error);
}

TEST(Error, FormatsMessage) {
  const astra::core::Error error("{} x {}", 4, "y");
  EXPECT_EQ(std::string{error.what()}, "4 x y");
}
