# Astra Engine

A cross-platform Vulkan renderer in C++23.

## Quick start

Prerequisites: Git, CMake 3.25+, a C++23 compiler, Ninja (Linux/macOS) or
Visual Studio 2022+ (Windows).

```sh
./scripts/bootstrap.sh                # once; Windows: .\scripts\bootstrap.ps1
cmake --preset default                # macOS: clang-debug   Windows: msvc-debug
cmake --build --preset build-default
ctest --preset test-default
./build/gcc-debug/bin/astra_engine
```

Bootstrap clones a pinned vcpkg into `.deps/`; nothing is installed globally.

## Presets

`<compiler>-<debug|release>` plus sanitizer and clang-tidy variants:

```sh
cmake --list-presets
cmake --preset clang-release
cmake --build --preset build-clang-release
ctest --preset test-clang-release
```

## Formatting

```sh
cmake --build --preset format
```

CI enforces it. `source scripts/aliases.sh` adds `astra_build`, `astra_test`,
`astra_run`, `astra_fmt`, `cda`.

## Validation layers

Enabled automatically in Debug when installed:

```sh
sudo apt install vulkan-validationlayers   # Linux
```

macOS/Windows: install the [LunarG Vulkan SDK](https://vulkan.lunarg.com/).

## Layout

```
src/
  core/        astra_core         Logging, shared utilities
  platform/    astra_platform     GLFW window, input, surface
  gpu/         astra_gpu          Vulkan: instance, device, swapchain, ...
  renderer/    astra_renderer     Frame loop, passes
  main.cpp     astra_engine       Executable
tests/
  <lib>/       astra_<lib>_tests  GoogleTest, one exe per library
```

Dependencies flow downward:

- `engine -> renderer -> gpu -> core`
- `platform -> core`.

Includes are relative to `src/`; namespaces mirror directories.

Style: `PascalCase` types, `camelCase` functions, `mPascalCase` members,
`kPascalCase` constants, `snake_case` files.

## Dependencies

vcpkg manifest (`vcpkg.json`): GLFW, spdlog, Vulkan loader + headers
(Vulkan-Hpp / `vk::raii`), Vulkan Memory Allocator, GoogleTest.

Optional: Vulkan validation layers (see above).
