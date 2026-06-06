# ManpWIN

ManpWIN is a Windows fractal exploration and rendering application featuring advanced Mandelbrot and related fractal techniques including perturbation, BLA acceleration, slope shading, and a multithreaded formula parser.

This repository contains a fully reproducible CMake-based build system supporting consistent Debug and Release builds with Visual Studio 2022.

ManpWIN is evolving toward a unified mathematical experimentation platform supporting deep zoom perturbation, high precision arithmetic, symbolic formula evaluation, and future transcendental perturbation research.

---

## Version 4.03b

Maintenance, usability, and stability release focused on Status Information modernisation, improved rendering progress reporting, documentation updates, and ongoing infrastructure improvements.

---

## Recent Improvements (4.03b)

### Status Information Modernisation
* Modernised Status Information window
* Added Thread Count reporting
* Added Plot Mode reporting
* Improved raster pass reporting
* Improved perturbation progress reporting
* Added Copy button
* Added dedicated Status Information help page

### Documentation
* Updated Help system
* Added Status Information documentation
* Updated keyboard command documentation
* Updated revision history

### General Stability
* Continued infrastructure cleanup
* Repository synchronisation and cleanup
* Numerous maintenance and reliability improvements

## Previous Major Release (4.03a)

### Perturbation and Rendering
* Extended perturbation support for additional transcendental and fractional-power functions
* Extended derivative slope support for additional perturbation function families
* Fixed perturbation cubic distortion issues
* Improved high-precision rendering stability

### File System and Metadata
* Complete rewrite and stabilisation of file read/write systems
* Improved management and separation of fractal metadata and rendering data
* Fixed Fractint PAR loading crash
* Improved PNG and long filename handling

### Colour and Palette System
* Corrected RGB/BGR handling affecting:
  * Kalles Fraktaler (KFR)
  * MAP palette files
  * COL true colour palette files
  * Fractint PAR inline palettes

* Improved palette source handling and rendering consistency

### Animation and Export
* Fixed animated GIF writing
* Fixed animated MPEG writing
* Improved animation frame and DIB handling

### General Stability
* Fixed intermittent derivative slope rendering artefacts
* Fixed clipboard handling and shutdown issues
* Numerous rendering and infrastructure stability improvements

Recent development has also focused on modernising legacy rendering, palette, metadata, and export systems while preserving compatibility with classic ManpWIN workflows and external fractal formats.

---

## ✨ Features

* Mandelbrot and related fractals
* Deep zoom using perturbation theory + BLA acceleration (with improved stability)
* BLA (approximation methods) for massive speed improvements
* Multithreaded rendering engine
* Multithreaded formula parser
* Slope derivative rendering modes
* Fractint palette support
* PNG export
* Advanced plotting modes
* Hypercomplex fractal support
* High-precision arithmetic (MPFR, double-double, quad-double)
* Template-based parser and arithmetic framework supporting unified fractal semantics
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
- ✔ Successful deep zoom exploration
- ✔ Ongoing collaboration and interest from the mathematical community
- ✔ Improved numerical stability across multiple arithmetic types (MPFR, DD, QD)

---

## 🆕 Recent Improvements

- Fixed perturbation initialisation issue when reference generation occurs outside the perturbation engine
- Improved rendering stability after toggling and recomputation
- Stabilised trigonometric functions across multiple arithmetic types
- Improved error handling in double-double (DD) and quad-double (QD) modes
- Introduced template-based framework to support future extensibility of fractal calculations
- Unified parser semantic architecture using templates
- Reduced arithmetic-specific duplication across parser and trig systems
- Introduced unified dispatch framework for DD/QD/MPFR arithmetic
- Modernised hypercomplex trig implementation

---

## 🏗️ Build Requirements

- Windows 10/11  
- Visual Studio 2022 (with C++ tools)  
- CMake ≥ 3.23  
- vcpkg installed at:

```text
C:\vcpkg
```
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

---

### 3. Build

Release:
cmake --build build --config Release

Debug:
cmake --build build --config Debug

---

### 4. Run

Release:
build\Release\ManpWIN64.exe

Debug:
build\Debug\ManpWIN64.exe

---

### Alternative (recommended)

build_release.bat
build_debug.bat

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
├─ CMakeLists.txt # Root build configuration
```

### External Dependencies (via vcpkg)

- MPFR
- GMP
- libpng
- zlib

---

## 🧯 Troubleshooting

Missing pnglib.lib:
Reconfigure CMake and ensure pnglib builds as STATIC.

MPFR / GMP errors:
Ensure vcpkg is installed and toolchain is set.

Blank screen:
Ensure .rc files are included.

Debug vs Release mismatch:
Check runtime library consistency.

---

## 🐉 Dragon Slayer Timeline

A chronological record of major battles during the ManpWIN modernisation.

- 🐲 Repository archaeology — removed legacy and duplicate source files  
- ⚔️ CMake resurrection — rebuilt modular build architecture  
- 🧱 pnglib integration — fixed missing target + linker language issues  
- 🔗 MPFR linking battle — resolved dependency integration via vcpkg  
- 🪟 Resource restoration — fixed blank screen by restoring `.rc` compilation  
- 🧠 Parser evolution — multithreaded formula parser stabilised  
- 🎯 Plotting expansion — new slope rendering + plotting modes added  
- 🐛 Debug infinite loop hunt — tracked worklist spin behaviour  
- 🎨 Palette parser fix — vector migration introduced subtle indexing bug  
- 🧩 Solid guessing initialization bug — uninitialised variable causing lock  
- ⚙️ CRT conflict resolution — `/NODEFAULTLIB:LIBCMTD` investigation  
- 🏰 First stable reproducible CMake build — Debug + Release verified  
- 🏷 Milestone tagged — historic stabilisation snapshot captured  
- 🧭 Stability phase reached — deterministic behaviour restored  
- 🔬 Research interest — project now attracting mathematical exploration  
- 🐉 Perturbation dragon — fixed uninitialised power causing incorrect rendering after toggling  
- 🧮 Arithmetic stabilisation — improved trig behaviour across DD/QD/MPFR  
- 🧱 Template foundation — introduced templated framework for future fractal expansion  
- 🧠 Semantic unification — parser/trig arithmetic consolidated via templates
- 🐉 Layer discipline restored — VM, dispatch, and semantic layers separated cleanly
- 🔮 Transcendental perturbation groundwork — infrastructure prepared for future symbolic perturbation research
-   📊 Status Information modernisation — improved progress reporting and diagnostics
-   🧵 Thread progress overhaul — status reporting now reflects the slowest active worker thread
-   📚 Documentation refresh — Status Information help page and keyboard documentation updated

---

## 🤝 Contributing Notes

- Never commit build directory
- Tag stable milestones
- Keep Debug and Release working
- Prefer incremental commits

---

## 🏆 Milestone

First stable reproducible CMake build achieved.

## 🔁 Reproducibility

---

A key goal of the modernisation effort is reproducibility:

- Clean CMake-based builds
- Controlled dependency handling via vcpkg
- Verified Debug and Release parity
- Deterministic rendering across runs (within current numerical limits)

---

## 🙏 Credits

<table>
<tr>
<td align="center">
<br>
<b>Paul the LionHeart</b><br>
<img src="Docs/images/lionheart.png" width="120"><br>
<sub>Author</sub>
</td>

<td align="center">
<br>
<b>ChatGPT</b><br>
<img src="Docs/images/chatty.png" width="120"><br>
<sub>Workshop Assistant</sub>
</td>
</tr>
</table>
