# ManpWIN 4.03a – Stability Update

## Overview

ManpWIN 4.03a is a major stability and infrastructure update focused on modernising file handling, improving perturbation rendering, correcting long-standing colour and palette issues, and stabilising animation export systems.

Significant internal work was also completed to improve the management and separation of fractal metadata, rendering data, palette systems, and animation frame handling. Many long-standing legacy issues relating to RGB/BGR colour handling, DIB management, and file interoperability were identified and corrected.

This release represents a substantial stabilisation milestone for the ManpWIN rendering engine.

---

# Fractal Engine Improvements

* Added perturbation support for additional transcendental functions:

  * Exp
  * Sin
  * Cos
  * Sinh
  * z^(n + 0.5)

* Added derivative slope support to perturbation rendering.

* Fixed distortion bug affecting perturbation cubic rendering.

* Improved stability and interoperability between perturbation, derivative slope, and high precision arithmetic systems.

---

# File System and Metadata Improvements

* Complete rewrite and stabilisation of file read/write systems.

* Overhauled management and separation of fractal metadata and rendering data.

* Fixed crash when reading Fractint PAR files.

* Fixed filename offset issues in file dialogue boxes when handling long filenames.

* Improved PNG file status message handling.

---

# Colour and Palette System Fixes

* Rationalised palette and colour source handling.

* Improved separation of:

  * palette source systems
  * runtime palette lookup systems
  * rendering systems
  * metadata systems

* Fixed RGB/BGR channel swapping issues affecting:

  * Kalles Fraktaler files (KFR)
  * MAP palette files
  * COL true colour palette files
  * Fractint PAR inline palette decoding

* Corrected procedural COL palette generation semantics.

* Improved compatibility between imported palette formats and internal runtime palette handling.

---

# Rendering and Display Fixes

* Fixed intermittent horizontal and vertical line artifacts appearing in derivative slope rendering.

* Fixed clipboard handling issues that could:

  * prevent clipboard loading
  * interfere with normal program shutdown

* Improved DIB handling and rendering consistency.

---

# Animation and Export Improvements

* Fixed animated GIF writing.

* Fixed animated MPEG writing.

* Improved animation frame management and DIB packing behaviour.

* Modernised animation frame handling and export stability.

---

# Internal Improvements

* Improved separation between:

  * fractal metadata
  * rendering data
  * palette generation
  * runtime colour lookup systems

* Modernised DIB handling infrastructure.

* Reduced legacy RGB/BGR inconsistencies throughout the rendering and export pipeline.

* Improved maintainability of colour and palette systems.

* Rationalised several long-standing legacy code paths related to animation, palette handling, and file interoperability.

---

# Notes

This release focuses heavily on stability, compatibility, infrastructure modernisation, and long-standing bug fixes across rendering, colour management, perturbation systems, and animation export functionality.

Further testing and incremental stabilisation work will continue in future releases.
