# ManpWIN

ManpWIN is a Windows fractal exploration and rendering application featuring advanced Mandelbrot and related fractal techniques including perturbation, BLA acceleration, slope shading, and a multithreaded formula parser.

This repository contains a fully reproducible CMake-based build system supporting consistent Debug and Release builds with Visual Studio 2022.

---

## ✨ Features

* Mandelbrot and related fractals
* Deep zoom using perturbation theory + BLA acceleration
* BLA (approximation methods) for massive speed improvements
* Multithreaded rendering engine
* Multithreaded formula parser
* Slope derivative rendering modes
* Fractint palette support
* PNG export
* Advanced plotting modes
* High-precision arithmetic (MPFR)
* Preservation of legacy algorithms with modern execution architecture
* True colour rendering
* Support for many fractal types including Mandelbrot, Julia, Burning Ship, and more

---

## 🔬 Use Cases

ManpWIN is designed for both visual exploration and mathematical experimentation:

- Deep zoom Mandelbrot and fractal structure analysis
- Perturbation-based extreme scale rendering
- Investigation of orbit behaviour and numerical stability
- Custom formula experimentation via VM-based parser
- Educational demonstrations of fractal dynamics

The system supports exploration across both floating-point and high-precision arithmetic domains.

---

## 🖼️ Example Output

### High-Precision Fractal Rendering

![Fractal](Docs/images/fractal.png)

### ManpWIN Interface

![ManpWIN UI](Docs/images/fractal_ui.jpg)

## 🎬 Animation

[![Watch Animation](Docs/images/jewels_preview.png)](Docs/videos/Jewels.webm)

👉 [Download animation file (WEBM, ~5 MB)](Docs/videos/Jewels.webm)

---

## 📊 Project Status

ManpWIN has reached a stable and reproducible build state with a fully functional multithreaded rendering pipeline.

### Current State

- ✔ Deterministic rendering across Mandelbrot, perturbation, and formula modes
- ✔ Stable multithreaded execution (worklist + parser)
- ✔ Verified Debug and Release builds via CMake + Visual Studio 2022
- ✔ Successful deep zoom exploration (including extreme perturbation ranges)
- ✔ Ongoing collaboration and interest from the mathematical community

### Known Characteristics

- Very deep zoom levels may require careful initialisation of perturbation state
- Some advanced modes (e.g. slope + bignum trig) are still under refinement
- Memory usage after large renders is not yet aggressively reduced

### Direction

The project is transitioning from a legacy fractal renderer into a:

> **high-precision, research-oriented fractal exploration platform**

with ongoing work in:

- Deterministic initialisation at extreme scales
- Memory lifecycle management
- Advanced numerical stability
- Further parser and VM evolution

---

## 🏗️ Build Requirements

* Windows 10/11  
* Visual Studio 2022 (with C++ tools)  
* CMake ≥ 3.23  
* vcpkg installed at:


C:\vcpkg


---

## ⚙️ Build Instructions

### 1. Clone repository


git clone https://github.com/PaulTheLionHeart/manpwin.git

cd manpwin


---

### 2. Configure (CMake + vcpkg)


cmake -B build -S . ^
-DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake ^
-DVCPKG_TARGET_TRIPLET=x64-windows


This step:
- installs required libraries (MPFR, GMP, libpng, zlib)
- generates Visual Studio build files

---

### 3. Build

#### Release

cmake --build build --config Release


#### Debug

cmake --build build --config Debug


---

### 4. Run

**Release**

build\Release\ManpWIN64.exe


**Debug**

build\Debug\ManpWIN64.exe


---

### 🟢 Alternative (recommended)

Use the provided batch files:


build_release.bat
build_debug.bat


These automatically configure and build the project using the correct settings.

---

## 📁 Project Structure


ManpWIN/
├─ ManpWIN64/ # Main application sources
├─ parser/ # Formula parser engine
├─ pnglib/ # PNG implementation
├─ ZLib/ # Compression support
├─ qdlib/ # Quad-double arithmetic
├─ MPEG/ # MPEG support
├─ CMakeLists.txt # Root build configuration


### External Dependencies (via vcpkg)

- MPFR (high precision math)
- GMP (arbitrary precision arithmetic)
- libpng
- zlib

---

## 🧠 Architecture Overview

```text
Git Repository
│
├─ ManpWIN64 (application)
├─ parser (formula VM)
├─ pnglib (PNG implementation)
├─ ZLib (compression)
├─ qdlib (quad-double arithmetic)
├─ MPEG (video support)
│
▼
External dependencies via vcpkg
(MPFR, GMP, libpng, zlib)
│
▼
Root CMakeLists.txt
│
├─ add_subdirectory(parser)
├─ add_subdirectory(pnglib)
├─ add_subdirectory(ZLib)
├─ add_subdirectory(qdlib)
├─ add_subdirectory(MPEG)
│
▼
ManpWIN64 executable target
│
▼
Debug / Release builds
│
▼
Visual Studio 2022 + CMake

The build uses a modular CMake architecture:

Each library compiled via add_subdirectory
Dependencies managed via vcpkg
Resource (.rc) file included in executable target to restore Windows menus
Runtime mismatch resolved via /NODEFAULTLIB:LIBCMTD
Debug and Release builds verified via both CMake and Visual Studio
🧯 Troubleshooting
Missing pnglib.lib

Reconfigure CMake and ensure pnglib CMakeLists builds a STATIC library.

MPFR / GMP errors

Ensure vcpkg is installed and the toolchain file is correctly specified during configure.

Blank screen

Ensure *.rc files are included in executable target.

Debug vs Release mismatch

Check runtime library consistency and avoid mixed CRT builds.

🐉 Dragon Slayer Timeline

A chronological record of major battles during the ManpWIN modernisation.

🐲 Repository archaeology — removed legacy and duplicate source files
⚔️ CMake resurrection — rebuilt modular build architecture
🧱 pnglib integration — fixed missing target + linker language issues
🔗 MPFR linking battle — resolved dependency integration via vcpkg
🪟 Resource restoration — fixed blank screen by restoring .rc compilation
🧠 Parser evolution — multithreaded formula parser stabilised
🎯 Plotting expansion — new slope rendering + plotting modes added
🐛 Debug infinite loop hunt — tracked worklist spin behaviour
🎨 Palette parser fix — vector migration introduced subtle indexing bug
🧩 Solid guessing initialization bug — uninitialised variable causing lock
⚙️ CRT conflict resolution — /NODEFAULTLIB:LIBCMTD investigation
🏰 First stable reproducible CMake build — Debug + Release verified
🏷 Milestone tagged — historic stabilisation snapshot captured
🧭 Stability phase reached — deterministic behaviour restored across rendering modes
🔬 Research interest — project now attracting mathematical exploration and extension
🤝 Contributing Notes
Never commit build directory
Tag stable milestones
Keep Debug and Release working
Prefer incremental commits
🏆 Milestone

This release represents the first stable, reproducible CMake-based build of ManpWIN with full Visual Studio 2022 compatibility.

🚀 Git Workflow Used
Stabilisation branch created
Legacy build artifacts removed
CMake modularized
Dependencies integrated via vcpkg
Debug and Release builds validated
Milestone tag created
Master branch verified via fresh clone
🔁 Reproducibility

A key goal of the modernisation effort is reproducibility:

Clean CMake-based builds
Controlled dependency handling
Verified Debug and Release parity
Deterministic rendering across runs (within current numerical limits)
Dependency management via vcpkg ensures consistent builds across systems
🙏 Credits
Paul the LionHeart — Author
ChatGPT — Development assistance