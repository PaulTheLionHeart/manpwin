ManpWIN + vcpkg Strategy

(Toolchain, Dependency & Repository Policy)

1. Purpose

This document defines how ManpWIN manages third-party libraries using vcpkg, while supporting:

Visual Studio 2017 (v141) — primary development environment

Visual Studio 2022 (v143) — modern CMake + release environment

The goal is:

Minimal backup footprint

Clean repository

Stable legacy workflow

Modern reproducible builds

No duplication of third-party source trees

2. High-Level Architecture
Desktop (Primary Build Machine)

Contains:

C:\vcpkg\

Built with two custom triplets:

x64-windows-static-v141

x64-windows-static-v143

These produce:

installed\
  x64-windows-static-v141\
  x64-windows-static-v143\

Both coexist safely.

Desktop responsibilities:

Build/update third-party libraries

Test new dependency versions

Generate release builds

Laptop (Lightweight Dev Machine)

Contains only:

include\
lib\

Copied from:

C:\vcpkg\installed\x64-windows-static-v141\
C:\vcpkg\installed\x64-windows-static-v143\

Laptop does NOT contain:

buildtrees

ports

downloads

full vcpkg installation

This reduces footprint from ~5GB to a few hundred MB.

3. Triplet Configuration

Two custom triplets exist.

x64-windows-static-v141.cmake
set(VCPKG_TARGET_ARCHITECTURE x64)
set(VCPKG_CRT_LINKAGE static)
set(VCPKG_LIBRARY_LINKAGE static)
set(VCPKG_PLATFORM_TOOLSET v141)
x64-windows-static-v143.cmake
set(VCPKG_TARGET_ARCHITECTURE x64)
set(VCPKG_CRT_LINKAGE static)
set(VCPKG_LIBRARY_LINKAGE static)
set(VCPKG_PLATFORM_TOOLSET v143)

This guarantees:

No ABI mixing

Clean separation of compiler toolsets

Safe linking in both environments

4. Manifest (vcpkg.json)

Single shared manifest in repository root:

{
  "name": "manpwin",
  "version-string": "dev",
  "dependencies": [
    "zlib",
    "libpng",
    "gmp",
    "mpfr"
  ]
}

Important:

Only one vcpkg.json is maintained.

Toolset differences are handled via triplets.

No per-compiler manifests.

5. Visual Studio 2017 Policy

VS2017 remains the primary development editor.

It:

Does NOT require CMake

Does NOT require vcpkg integration

Simply points to include/lib folders of v141 build

Settings:

Include:

...\x64-windows-static-v141\include

Library:

...\x64-windows-static-v141\lib

VS2017 remains comfortable and stable.

6. Visual Studio 2022 Policy

VS2022 uses:

CMakeLists.txt

vcpkg toolchain integration

v143 triplet

CMakePresets.json (recommended):

{
  "version": 3,
  "configurePresets": [
    {
      "name": "vs2022-v143",
      "generator": "Visual Studio 17 2022",
      "toolchainFile": "C:/vcpkg/scripts/buildsystems/vcpkg.cmake",
      "cacheVariables": {
        "VCPKG_TARGET_TRIPLET": "x64-windows-static-v143"
      }
    }
  ]
}

This enables:

Automatic dependency resolution

Clean CI integration

Reproducible builds

7. Repository Policy

The following are committed to GitHub:

✔ Source code
✔ vcpkg.json
✔ CMakeLists.txt
✔ CMakePresets.json
✔ Strategy documentation

The following are NEVER committed:

✘ installed/
✘ buildtrees/
✘ packages/
✘ third-party .lib files
✘ downloaded sources

Repository remains clean and lightweight.

8. Backup Strategy

Back up:

Entire ManpWIN source tree

vcpkg.json

Custom triplets

Do NOT back up:

Full vcpkg directory (rebuildable)

Third-party binaries (rebuildable)

If needed, desktop can regenerate:

vcpkg install --triplet x64-windows-static-v141
vcpkg install --triplet x64-windows-static-v143
9. Update Policy

When updating libraries:

Update vcpkg on desktop.

Rebuild both triplets.

Run regression tests.

Benchmark performance.

Commit updated vcpkg.json if version changes.

Copy include/lib folders to laptop.

No blind updates.

10. Long-Term Goal

Eventually:

VS2022 + CMake may become primary.

VS2017 may remain legacy reference.

Both continue to coexist cleanly.

Decision deferred — not forced.

11. Guiding Principles

Stability before novelty.

Single source of truth.

No duplicated third-party code in repo.

No ABI mixing.

Clear separation between toolsets.

Comfort for development, modernity for release.