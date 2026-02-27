# ManpWIN

**ManpWIN** is a Windows fractal exploration and rendering application featuring advanced Mandelbrot and related fractal techniques including perturbation, BLA acceleration, slope shading, and a multithreaded formula parser.

This repository contains a modernized CMake build system enabling reproducible Debug and Release builds with Visual Studio 2022.

---

# ✨ Features

- Mandelbrot and related fractals
- Perturbation + BLA acceleration
- Multithreaded rendering engine
- Multithreaded formula parser
- Slope derivative rendering modes
- Fractint palette support
- PNG export
- Advanced plotting modes
- High-precision arithmetic (MPFR)
- Legacy algorithm preservation with modernization

---

# 🏗️ Build Requirements

- Windows 10/11
- Visual Studio 2022
- CMake ≥ 3.23
- MPFR library (included as imported dependency)

---

# ⚙️ Build Instructions

## Clone repository

```bash
git clone https://github.com/PaulTheLionHeart/manpwin.git
cd manpwin

##Configure build

mkdir build
cd build
cmake ..

## Build Debug

cmake --build . --config Debug

## Build Release

cmake --build . --config Release

## Run

Debug:

build\Debug\ManpWIN64.exe

Release:

build/Release/ManpWIN64.exe

## Project Structure:

ManpWIN/
 ├─ ManpWIN64/      # Main application sources
 ├─ parser/         # Formula parser engine
 ├─ pnglib/         # PNG implementation
 ├─ ZLib/           # Compression support
 ├─ qdlib/          # Quad-double arithmetic
 ├─ MPEG/           # MPEG support
 ├─ mpfr/           # High precision math library
 ├─ CMakeLists.txt  # Root build configuration

🧠 Architecture Notes

The build uses a modular CMake architecture:

Each library compiled via add_subdirectory

MPFR imported as external static library

Resource file compiled to restore Windows menus

Resource (.rc) file included in executable target to restore Windows menus

Runtime mismatch resolved via /NODEFAULTLIB:LIBCMTD

Debug and Release builds verified via both CMake and Visual Studio

🧯 Troubleshooting

Blank UI

Ensure resource .rc file is included in executable sources.

pnglib not found

Verify add_subdirectory(pnglib) and linker language set to C.

set_target_properties(pnglib PROPERTIES LINKER_LANGUAGE C)

Runtime conflict warning - cause = mpfr compiled with different runtime

Resolved with:

target_link_options(ManpWIN64 PRIVATE /NODEFAULTLIB:LIBCMTD)
Infinite rendering loop

Usually caused by:

uninitialised variables

worklist logic errors

palette parser bugs

🚀 Git Workflow Used

Stabilisation branch created

Legacy build artifacts removed

CMake modularized

Dependencies integrated

Debug and Release builds validated

Milestone tag created

Master branch verified via fresh clone

🏆 Milestone

This release represents the first stable reproducible CMake build of ManpWIN with full Visual Studio 2022 compatibility.

🙏 Credits

Paul de Leeuw (LionHeart) — Author and architect of ManpWIN
ChatGPT (OpenAI) — Development partner, debugging assistance, and documentation support

---

# ✅ Next step (commit)

```bash
git add README.md
git commit -m "Add project README with build guide"
git push

