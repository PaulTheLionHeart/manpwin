# ManpWIN

ManpWIN is a Windows fractal exploration and rendering application featuring advanced Mandelbrot and related fractal techniques including perturbation theory, optional Bilinear Approximation (BLA) acceleration, high-precision arithmetic, slope rendering, and a multithreaded formula parser.

The project combines decades of fractal development with a modern CMake-based build system, providing a powerful platform for both visual exploration and mathematical research into deep-zoom fractals and experimental formulae.

This repository contains a fully reproducible CMake-based build system supporting consistent Debug and Release builds with Visual Studio 2022.

---

## Version 4.06

ManpWIN 4.06 is a major feature and compatibility release introducing a new
Inverse Julia implementation, a substantially expanded fractal transformation
system, and major improvements to Fractint PAR compatibility, rendering, and
reliability.

---

## Recent Improvements (4.06)

### Inverse Julia

* Added a new Inverse Julia implementation supporting powers from 2 through 20
* Added Breadth First, Depth First, Random Walk, and Random Run generation methods
* Added Inverse Julia animation across all supported powers
* Added Real-Time Inverse Julia exploration for Mandelbrot and Power fractals
* Added support for rotated source views and improved coordinate handling
* Restores the original Mandelbrot or Power fractal after an Inverse Julia animation
* Added progress reporting, cancellation, and safeguards against runaway calculations
* Improved compatibility with Fractint `julia_inverse` and `miim=` parameters

### Fractal Transformations

* Added a dedicated Fractal Transformations system separate from ordinary Fractal Options
* Added multiple coordinate systems and standard geometric transformations
* Added Affine transformations with scaling, stretching, shearing, reflection, rotation, and translation
* Added Möbius, Joukowski, and Twist transformations
* Improved arbitrary-angle and quarter-turn rotation handling
* Improved rotated zoom-in and zoom-out behaviour
* Added transformation persistence in parameter files

### Fractint PAR Compatibility

* Greatly improved Fractint viewport interpretation
* Added support for six-corner geometry, rotation, skew, stretched axes, and horizontal magnification
* Improved loading of rotated and transformed Fractint views
* Restored the historical Fractint default inside colour for imported PAR files
* Corrected logmap and potential colouring interactions with filters and inside colours
* Improved symmetry and block plotting for affected Fractint examples
* Repaired a number of long-standing compatibility problems in individual PAR examples
* Improved Inverse Julia compatibility for examples including SeaHorse and SeaShell

### Fractal and Rendering Improvements

* Added Magnet 1 and Magnet 2 support including finite-attractor handling
* Added TieraZon Blob / Convergence bailout behaviour
* Corrected Frothy Basin colouring so valid palette indices are preserved
* Increased IFS point-generation density, restoring detail in examples such as Autumn
* Fixed several long-standing rendering defects affecting legacy fractals
* Improved status information and rendering progress reporting
* Improved cancellation and loop protection for long-running calculations

---

## Previous Release Highlights (4.05b)

### TieraZon and Flarium Filters

* Fixed TieraZon filter behaviour across pixel-based fractals
* Added a number of additional filters adapted from Stephen C. Ferguson's Flarium program
* Fixed incorrect red/blue channel ordering in TieraZon and Flarium filters
* Repaired several legacy TieraZon and Flarium filters which were not producing correct results
* Corrected TieraZon filter precedence so TieraZon colouring takes priority over ordinary inside filters when appropriate

### Clouds Orbit-Density Rendering

* Added Clouds, an orbit-density rendering method based on an idea from Flarium
* Added Clouds support to both ordinary and perturbation rendering
* Added Linear Greyscale, Log Greyscale, Linear Palette, and Log Palette rendering modes
* Added a Clouds colour selector to the Fractal Options dialog
* Added palette colour support to Clouds rendering
* Automatically disables Bilinear Approximation (BLA) acceleration while Clouds is active so that all orbit visits are preserved in the density calculation
* Added saving and restoration of Clouds rendering settings in PAR files and animation scripts
* Added Clouds density-map merge progress reporting to the status bar

### Rendering and Documentation

* Fixed the status-bar pass display which could incorrectly report `Pass 0 of n` during multi-pass rendering
* Added new Help documentation for TieraZon, Flarium, and Clouds filters

---

## ✨ Features

* Mandelbrot and related fractals
* Extreme deep-zoom rendering using perturbation theory, with optional Bilinear Approximation (BLA) acceleration for improved performance
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
* TieraZon and Flarium orbit-based colouring filters
* Clouds orbit-density rendering with greyscale and palette-based display modes

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

### Inverse Julia Animation

[![Watch Inverse Julia Animation](Docs/images/InverseJulia_preview.png)](Docs/videos/InverseJulia.webm)

👉 [Download animation file (WEBM)](Docs/videos/InverseJulia.webm)

---

## 📊 Project Status

ManpWIN has reached a stable and reproducible build state with a fully functional multithreaded rendering pipeline.

### Current State

- ✔ Deterministic rendering across Mandelbrot, perturbation, and formula modes
- ✔ Stable multithreaded execution (worklist + parser)
- ✔ Verified Debug and Release builds via CMake + Visual Studio 2022
- ✔ Verified extreme deep zoom rendering using perturbation theory and high-precision arithmetic
- ✔ Ongoing collaboration and interest from the mathematical community
- ✔ Improved numerical stability across multiple arithmetic types (MPFR, DD, QD)

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

```bat
git clone https://github.com/PaulTheLionHeart/manpwin.git
cd manpwin
```

---

### 2. Configure (CMake + vcpkg)

```bat
  cmake -B build -S . ^
  -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake ^
  -DVCPKG_TARGET_TRIPLET=x64-windows-static
```


The `x64-windows-static` triplet is recommended because it produces a self-contained executable without requiring MPFR or other third-party runtime DLLs.

---

### 3. Build

Release:
```bat
cmake --build build --config Release
```

Debug:
```bat
cmake --build build --config Debug
```

---

### 4. Run

Release:
```bat
build\Release\ManpWIN64.exe
```

Debug:
```bat
build\Debug\ManpWIN64.exe
```

---

### Alternative (recommended)

```bat
build_release.bat
build_debug.bat
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
├─ CMakeLists.txt # Root build configuration
```

### External Dependencies (via vcpkg)

- MPFR
- GMP
- libpng
- zlib

---

## 🧯 Troubleshooting

**Missing pnglib.lib:**  
Reconfigure CMake and ensure pnglib builds as STATIC.

**MPFR / GMP errors:**  
Ensure vcpkg is installed and toolchain is set.

**Blank screen:**  
Ensure .rc files are included.

**Debug vs Release mismatch:**  
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
- 📊 Status Information modernisation — improved progress reporting and diagnostics
- 🧵 Thread progress overhaul — status reporting now reflects the slowest active worker thread
- 📚 Documentation refresh — Status Information help page and keyboard documentation updated
- 🧭 Deep zoom expedition — tracked and eliminated long-standing perturbation teleporting
- 🛰 Reference orbit refinement — improved perturbation reference generation and rebasing
- 🧩 Hidden missing `else` — restored correct rebasing when BLA is disabled
- 🌊 KFR smoothing restored — preserved smoothing values when reading and writing `.KFR` files
- 📋 Metadata consistency — parameter dialogues now correctly rebuild fractal metadata across all new fractal families
- 🧮 DD/QD trig repair — fixed transcendental arithmetic issues affecting TieraZon fractals
- 🧊 3D rendering restored — corrected effective thread handling in pixel and perturbation modes
- 🧵 Thread lifecycle cleanup — removed completion races and simplified worker startup/shutdown handling
- 🐉 CPixel audit — verified strip ownership, symmetry, worklists, arithmetic initialisation, and rendering lifecycle
- 🎨 Filter unification — shared ordinary filter behaviour across pixel, perturbation, and derivative slope rendering
- 💡 Slope lighting preserved — filters and biomorphs now work with derivative slope without replacing slope illumination
- 🎨 Palette cleanup — corrected palette start, offset, and shift handling and added configurable pre-palette colouring
- 🔄 Fractal interoperability — improved conversion between equivalent pixel and perturbation fractals
- 🧮 Polynomial expansion — Perturbation Polynomial extended to eighth order
- 🐉 Power distortion slain — fixed long-standing reference-orbit distortion caused by fractal degree being established too late
- 🧩 Parameter architecture — expanded perturbation and slope parameters, updated dialogue and animation handling, and simplified parameter-file persistence
- 🎞 Parameter animation repair — restored normal parameter, Start Palette, and bailout animation after the 4.05 parameter architecture changes
- 📋 Parameter persistence repair — corrected TieraZon `FDOptions` and `dStrands` PAR handling and restored perturbation/slope runtime state from PAR and animation files
- 🎨 Pre-palette colour preservation — ensured the selected start colour survives parameter animation and parameter-file loading
- 🎨 TieraZon filter repair — corrected colouring behaviour, channel ordering, precedence, and several legacy filters
- 🧪 Flarium expansion — added additional Stephen C. Ferguson filter algorithms to ManpWIN
- ☁️ Clouds arrive — introduced orbit-density rendering based on the Flarium Clouds idea
- 🎨 Clouds colour modes — added linear/logarithmic greyscale and palette rendering
- 🧮 Perturbation Clouds — extended orbit-density rendering to the perturbation engine
- 🚀 BLA safety — automatically bypassed Bilinear Approximation when Clouds requires every orbit visit
- 🧵 Density merge reporting — added progress reporting while per-thread Clouds density maps are combined
- 📊 Pass display repair — fixed the long-standing `Pass 0 of n` status display
- 📚 Clouds documentation — added dedicated TieraZon, Flarium, and Clouds Help documentation
- 🐉 Inverse Julia dragon — built a new Inverse Julia engine from scratch and extended it from quadratic-only behaviour to powers 2–20
- 🌳 Inverse tree tamed — added Breadth First, Depth First, Random Walk, and Random Run generation methods
- 🎞 Inverse Julia motion — added higher-power animation and Real-Time Inverse Julia exploration
- 🧭 Transformation workshop — introduced Affine, Möbius, Joukowski, Twist, coordinate-system transforms, and corrected rotated zoom behaviour
- 🗺 Fractint cartography — reconstructed six-corner geometry, rotation, skew, stretched axes, and horizontal magnification from legacy PAR files
- 🐸 Evil Frog palette rescue — traced colour through multiple clamping paths and restored the missing palette range
- 🌳 Autumn regrown — restored missing IFS foliage by increasing point-generation density
- 🧮 Newton finally cornered — resolved a long-standing Fractint compatibility problem
- 🐚 SeaHorse and SeaShell expedition — greatly improved six-corner Inverse Julia compatibility
- 🎨 Inside colour archaeology — verified the original DOS Fractint source and restored its historical default inside colour

---

## 🤝 Contributing Notes

- Never commit build directory
- Tag stable milestones
- Keep Debug and Release working
- Prefer incremental commits

---

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
