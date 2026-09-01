#include "platform/window.hpp"

#include <gtest/gtest.h>

#include <cstdint>
#include <utility>

#include "platform/platform.hpp"

using astra::platform::Backend;
using astra::platform::Platform;
using astra::platform::Window;

namespace {

constexpr std::uint32_t kWidth = 640;
constexpr std::uint32_t kHeight = 480;

}  // namespace

namespace {

class WindowTest : public ::testing::Test {
 protected:
  Platform mPlatform{Backend::Headless};
};

}  // namespace

TEST_F(WindowTest, CreatesWithRequestedSize) {
  const Window window({.width = kWidth, .height = kHeight, .title = "test"});
  ASSERT_NE(window.handle(), nullptr);

  const auto size = window.framebufferSize();
  EXPECT_EQ(size.width, kWidth);
  EXPECT_EQ(size.height, kHeight);
}

TEST_F(WindowTest, CloseRequestIsObservable) {
  Window window({});
  EXPECT_FALSE(window.shouldClose());
  window.requestClose();
  EXPECT_TRUE(window.shouldClose());
}

TEST_F(WindowTest, MoveTransfersOwnership) {
  Window original({});
  const auto* const handle = original.handle();

  const auto moved = std::move(original);
  EXPECT_EQ(moved.handle(), handle);
}

TEST_F(WindowTest, MultipleWindowsCoexist) {
  const Window first({.title = "first"});
  const Window second({.title = "second"});
  EXPECT_NE(first.handle(), second.handle());
}
