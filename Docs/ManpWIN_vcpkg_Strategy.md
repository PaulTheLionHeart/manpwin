# ManpWIN + vcpkg Strategy

## Purpose

ManpWIN uses vcpkg to provide the external libraries required by the modern CMake build while keeping third-party source code and binaries out of the repository.

The current release build uses:

- Visual Studio 2022
- CMake 3.23 or later
- vcpkg manifest mode
- x64-windows-static
- Static MSVC runtime

## Dependencies

The repository contains a single `vcpkg.json` manifest declaring:

- zlib
- libpng
- GMP
- MPFR

Third-party dependency source trees and compiled libraries are not stored in the ManpWIN repository.

## Build Configuration

The recommended vcpkg target triplet is:

    x64-windows-static

This provides statically linked third-party libraries and supports creation of a self-contained ManpWIN executable.

The root `CMakeLists.txt` selects the static MSVC runtime.

GMP and MPFR are located through ManpWIN's custom CMake Find modules because the current vcpkg packages do not provide the required CMake package configuration used by ManpWIN.

## Visual Studio 2022

Visual Studio 2022 is the supported modern CMake and release-build environment.

Typical configuration:

    cmake -B build -S . ^
      -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake ^
      -DVCPKG_TARGET_TRIPLET=x64-windows-static

Build:

    cmake --build build --config Release

or:

    cmake --build build --config Debug

The supplied build batch files may also be used.

## Visual Studio 2017

Visual Studio 2017 may still be used for legacy development where required.

It is not the documented reproducible CMake release environment.

Any libraries used by a legacy Visual Studio 2017 project must remain ABI-compatible with the compiler and runtime configuration used by that project.

## Repository Policy

Commit:

- Source code
- CMake build files
- vcpkg.json
- ManpWIN CMake helper modules
- Build and strategy documentation

Do not commit:

- vcpkg installed directories
- buildtrees
- packages
- downloaded dependency sources
- third-party binary libraries
- generated CMake build directories

## Dependency Updates

Dependency updates should be deliberate rather than automatic.

When updating vcpkg libraries:

1. Update or rebuild the required dependencies.
2. Build ManpWIN in Debug and Release configurations.
3. Run regression tests.
4. Verify rendering and high-precision arithmetic.
5. Commit manifest changes only when required.

## Guiding Principles

- Stability before novelty
- Reproducible release builds
- Static third-party dependencies
- No duplicated third-party source trees
- No ABI mixing
- Keep the repository clean and lightweight