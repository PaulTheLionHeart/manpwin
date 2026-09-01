# ManpWIN 4.05 Release Notes

ManpWIN 4.05 is a major perturbation, slope-rendering, and parameter architecture update focused on improving rendering consistency, fractal interoperability, colouring behaviour, and long-term maintainability.

## Rendering and Colouring

- Unified ordinary filtering behaviour across pixel, perturbation, and derivative slope rendering.
- Added filter and biomorph support to derivative slope rendering while retaining slope lighting.
- Fixed several forward-difference slope rendering issues, including subtype 13 calculations, special-colour indexing, and pixel-state handling.
- Corrected palette start, offset, and shift handling, and added configurable pre-palette colouring for iterations below the selected Start Palette value.

## Perturbation and Parameter Handling

- Expanded and reorganised perturbation and slope parameter handling, including updated dialogue boxes, parameter animation, and parameter-file save/load support.
- Improved conversion between equivalent pixel and perturbation fractals, including Polynomial fractals.
- Extended Perturbation Polynomial support to eighth-order polynomials.

## Stability and Correctness

- Fixed a long-standing perturbation power distortion caused by reference orbits being generated before the correct fractal degree was established.
- Improved rendering consistency across perturbation, slope, and standard pixel modes.
- General stability and internal architecture improvements.

## Compatibility Note

Perturbation and slope parameter layouts were revised in ManpWIN 4.05.

Older parameter files and parameter-animation scripts using the previous layouts may require adjustment before reuse.

## Documentation

- Updated help documentation for ManpWIN 4.05.
- Added documentation for Start Palette and pre-palette colouring.
- Updated keyboard-command, parameter-file, animation, and known-limitations documentation.