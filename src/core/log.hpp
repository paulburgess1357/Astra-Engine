#pragma once

namespace astra::core {

// Configures the process-wide spdlog default logger. Call once at startup;
// afterwards use SPDLOG_INFO / SPDLOG_WARN / ... from any thread.
auto initLogging() -> void;

}  // namespace astra::core
