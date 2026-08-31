# Astra Engine

A cross-platform Vulkan renderer in C++23.

## Quick start

Prerequisites: Git, CMake 3.25+, a C++23 compiler, and Ninja (Linux/macOS) or
Visual Studio 2022+ (Windows; VS 2026 needs CMake 4.2+).

```sh
./scripts/bootstrap.sh          # Windows: powershell -ExecutionPolicy Bypass -File .\scripts\bootstrap.ps1
cmake --preset default          # macOS: clang-debug   Windows: msvc-debug
cmake --build --preset build-default
ctest --preset test-default
```

Bootstrap clones a pinned vcpkg into the git-ignored `.deps/` directory. Nothing
is installed globally; the first configure builds the manifest dependencies.

## Presets

Every preset `<name>` has matching `build-<name>` and `test-<name>` presets.

| Preset            | Compiler    | Notes                          |
| ----------------- | ----------- | ------------------------------ |
| `default`         | GCC         | Alias for `gcc-debug`          |
| `gcc-debug`       | GCC         |                                |
| `gcc-release`     | GCC         |                                |
| `clang-debug`     | Clang       | Use on macOS                   |
| `clang-release`   | Clang       |                                |
| `clang-tidy`      | Clang       | Static analysis (`.clang-tidy`) |
| `gcc-asan-ubsan`  | GCC         | Address + UB sanitizers        |
| `gcc-tsan`        | GCC         | Thread sanitizer               |
| `clang-asan-ubsan`| Clang       | Address + UB sanitizers        |
| `clang-tsan`      | Clang       | Thread sanitizer               |
| `msvc-debug`      | MSVC        |                                |
| `msvc-release`    | MSVC        |                                |
| `msvc-asan`       | MSVC        | Address sanitizer only         |

ASan and TSan cannot be combined, hence separate presets. Warnings are errors
on every preset.

Formatting is enforced in CI:

```sh
cmake --build --preset format        # apply
cmake --build --preset format-check  # verify
```

CI runs the full matrix on Ubuntu, macOS, and Windows plus the format check.

### Shell shortcuts (optional)

`scripts/aliases.sh` defines `astra_build`, `astra_test`, `astra_run`,
`astra_fmt`, and `cda` for the default preset; they work from any directory.
Add to your `~/.bashrc` or `~/.zshrc`:

```sh
source /path/to/Astra-Engine/scripts/aliases.sh
```

## Layout

```
src/
  core/       astra_core      logging setup, shared utilities (no engine deps)
  platform/   astra_platform  GLFW window, input, surface creation
  gpu/        astra_gpu       Vulkan: device, swapchain, memory, pipelines, commands
  renderer/   astra_renderer  frame loop, passes, what actually gets drawn
  main.cpp    astra_engine    executable wiring the above together
tests/<lib>/  astra_<lib>_tests  GoogleTest, one executable per library
```

Dependencies flow downward only: `engine -> renderer -> gpu -> core`,
`platform -> core`. Headers are included relative to `src/`
(`#include "gpu/context.hpp"`); namespaces mirror directories (`astra::gpu`).

Log with `SPDLOG_INFO`/`SPDLOG_WARN`/etc. on the default logger. Macros below
`SPDLOG_ACTIVE_LEVEL` (trace in Debug, info otherwise) compile out.

## Style

`PascalCase` types · `camelCase` functions/variables · `kPascalCase` constants ·
`snake_case` files · `lowercase` namespaces. Formatting is defined by
`.clang-format`. clangd reads `build/compile_commands.json`, a symlink to the
most recently configured preset's database.

## Dependencies

vcpkg manifest mode (`vcpkg.json`), pinned to a baseline commit:

- **GLFW** — windows, input, Vulkan surface creation
- **spdlog** — logging
- **Vulkan** loader + headers, including Vulkan-Hpp / `vk::raii`
- **Vulkan Memory Allocator** — GPU memory management
- **GoogleTest** — tests
