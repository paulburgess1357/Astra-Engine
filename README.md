# Astra Engine

A cross-platform Vulkan renderer, beginning with a strict C++/CMake foundation.

## Build locally (GCC)

Prerequisites: CMake 3.25+, Make, GCC with C++23 support, and `clang-format`.

```sh
cmake --preset default
cmake --build --preset build-default
ctest --preset test-default
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
optional static-analysis, and Visual Studio builds. The CI pipeline builds and
tests Debug and Release with GCC/Clang on Linux, Apple Clang on macOS, and MSVC
on Windows.

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
editor diagnostics; `.clang-tidy` is an opt-in static-analysis preset.

## Dependencies

This repository uses vcpkg manifest mode. There are no dependencies yet; Vulkan
and windowing libraries will be added to `vcpkg.json` when the renderer begins.
