#include <spdlog/spdlog.h>

#include "application.hpp"

auto main() -> int {
  spdlog::info(astra::kGreeting);
  return 0;
}
