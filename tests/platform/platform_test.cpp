#include "platform/platform.hpp"

#include <gtest/gtest.h>

#include "core/error.hpp"

using astra::platform::Backend;
using astra::platform::Platform;

TEST(Platform, HeadlessInitializes) {
  EXPECT_NO_THROW(Platform{Backend::Headless});
}

TEST(Platform, SecondInstanceThrows) {
  const Platform platform{Backend::Headless};
  EXPECT_THROW(Platform{Backend::Headless}, astra::core::Error);
}

TEST(Platform, CanBeRecreatedAfterDestruction) {
  {
    const Platform platform{Backend::Headless};
  }
  EXPECT_NO_THROW(Platform{Backend::Headless});
}
