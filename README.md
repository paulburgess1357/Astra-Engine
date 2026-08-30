# Astra Engine

A cross-platform Vulkan Renderer

## Bootstrap dependencies

Prerequisites are Git, CMake 3.25+, a compiler with C++23 support, and the
platform build tools: Ninja on Linux/macOS (`apt install ninja-build` or
`brew install ninja`) or Visual Studio 2022 or newer on Windows. On Linux and macOS,
bootstrap the pinned vcpkg checkout with:

```sh
./scripts/bootstrap.sh
```

On Windows, run:

```powershell
powershell -ExecutionPolicy Bypass -File .\scripts\bootstrap.ps1
```

The scripts keep vcpkg, downloaded sources, and its binary cache under the
Git-ignored `.deps/` directory. They do not install anything globally or modify
the shell environment. Running a configure preset then installs the manifest
dependencies for that platform automatically.

## Build locally

The default preset is a GCC Debug build for Linux:

```sh
cmake --preset default
cmake --build --preset build-default
ctest --preset test-default
```

On macOS, use the Apple Clang preset:

```sh
cmake --preset clang-debug
cmake --build --preset build-clang-debug
ctest --preset test-clang-debug
```

On Windows, use the MSVC preset:

```powershell
cmake --preset msvc-debug
cmake --build --preset build-msvc-debug
ctest --preset test-msvc-debug
```

`default` means GCC Debug: fast compilation, debug information, and assertions
enabled. Build an optimized release locally with:

```sh
cmake --preset gcc-release
cmake --build --preset build-gcc-release
ctest --preset test-gcc-release
```

Run the required formatting check with:

```sh
cmake --build --preset format-check
```

The `clang-*`, `clang-tidy`, and `msvc-*` presets provide equivalent Clang,
static-analysis, and Visual Studio builds. The CI pipeline checks formatting,
runs clang-tidy, and builds/tests Debug and Release with GCC/Clang on Linux,
Apple Clang on macOS, and MSVC on Windows.

## Runtime sanitizers

Sanitizer builds instrument the executable and report bugs while tests run. Use
AddressSanitizer and UndefinedBehaviorSanitizer together for memory and
undefined-behavior errors:

```sh
cmake --preset gcc-asan-ubsan
cmake --build --preset build-gcc-asan-ubsan
ctest --preset test-gcc-asan-ubsan
```

Use the corresponding `clang-asan-ubsan` preset with Clang or Apple Clang.
ThreadSanitizer is a separate build because it cannot be combined with
AddressSanitizer:

```sh
cmake --preset gcc-tsan
cmake --build --preset build-gcc-tsan
ctest --preset test-gcc-tsan
```

GCC, Clang, and Apple Clang support all three sanitizers. MSVC supports
AddressSanitizer only; use `msvc-asan`, `build-msvc-asan`, and `test-msvc-asan`.

## Style

- Types, classes, and enums: `PascalCase`
- Functions, methods, and variables: `camelCase`
- Constants: `kPascalCase`
- Files: `snake_case`
- Namespaces: `lowercase`

Formatting is required and defined by `.clang-format`. `.clangd` configures
editor diagnostics; `.clang-tidy` is an opt-in static-analysis preset. clangd
reads `build/compile_commands.json`, a symlink that always points at the most
recently configured preset's compilation database.

## Dependencies

This repository uses vcpkg manifest mode with a pinned registry baseline:

- GLFW for windows, input, and Vulkan surface creation
- spdlog for logging
- Vulkan loader and headers, including Vulkan-Hpp and its `vk::raii` API
- Vulkan Memory Allocator (VMA) for GPU memory management
