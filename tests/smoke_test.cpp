#include <gtest/gtest.h>

#include "application.hpp"

TEST(AstraSmoke, ApplicationGreetingIsCorrect) {
  EXPECT_EQ(astra::kGreeting, "Hello from Astra.");
}
