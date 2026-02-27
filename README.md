# ManpWIN

ManpWIN is a Windows fractal exploration and rendering application featuring advanced Mandelbrot and related fractal techniques including perturbation, BLA acceleration, slope shading, and a multithreaded formula parser.

This repository contains a modernized CMake build system enabling reproducible Debug and Release builds with Visual Studio 2022.

---

## ✨ Features

* Mandelbrot and related fractals
* Perturbation + BLA acceleration
* Multithreaded rendering engine
* Multithreaded formula parser
* Slope derivative rendering modes
* Fractint palette support
* PNG export
* Advanced plotting modes
* High-precision arithmetic (MPFR)
* Legacy algorithm preservation with modernization

---

## 🏗️ Build Requirements

* Windows 10/11
* Visual Studio 2022
* CMake ≥ 3.23
* MPFR library (included as imported dependency)

---

## ⚙️ Build Instructions

### Clone repository

```
git clone https://github.com/PaulTheLionHeart/manpwin.git
cd manpwin
```

### Configure build

```
mkdir build
cd build
cmake ..
```

### Build Debug

```
cmake --build . --config Debug
```

### Build Release

```
cmake --build . --config Release
```

### Run

**Debug**

```
build\Debug\ManpWIN64.exe
```

**Release**

```
build\Release\ManpWIN64.exe
```

---

## 📁 Project Structure

```
ManpWIN/
├─ ManpWIN64/     # Main application sources
├─ parser/        # Formula parser engine
├─ pnglib/        # PNG implementation
├─ ZLib/          # Compression support
├─ qdlib/         # Quad-double arithmetic
├─ MPEG/          # MPEG support
├─ mpfr/          # High precision math library
└─ CMakeLists.txt # Root build configuration
```

---

## 🧠 Architecture Notes

The build uses a modular CMake architecture:

* Each library compiled via `add_subdirectory`
* MPFR imported as external static library
* Resource (.rc) file included in executable target to restore Windows menus
* Runtime mismatch resolved via `/NODEFAULTLIB:LIBCMTD`
* Debug and Release builds verified via both CMake and Visual Studio

---

## 🧯 Troubleshooting

### Missing pnglib.lib

Reconfigure CMake and ensure pnglib CMakeLists builds a STATIC library.

### MPFR linker errors

Ensure mpfr imported library path is correct.

### Blank screen

Ensure *.rc files are included in executable target.

### Debug vs Release mismatch

Check runtime library consistency and avoid mixed CRT builds.

---

## 🐉 Dragon Slayer Log

* Repository recovery
* Parser modernization
* PNG library integration
* MPFR linking fixes
* Resource compilation restoration
* Debug infinite loop bug hunt
* Palette parser fix
* Solid guessing initialization bug
* CMake stabilization

---

## 🤝 Contributing Notes

* Never commit build directory
* Tag stable milestones
* Keep Debug and Release working
* Prefer incremental commits

---

## 🏆 Milestone

This release represents the first stable reproducible CMake build of ManpWIN with full Visual Studio 2022 compatibility.

---

## 🚀 Git Workflow Used

* Stabilisation branch created
* Legacy build artifacts removed
* CMake modularized
* Dependencies integrated
* Debug and Release builds validated
* Milestone tag created
* Master branch verified via fresh clone

---

## 🙏 Credits

- Paul the LionHeart — Author
- ChatGPT — Development assistance