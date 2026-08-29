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
