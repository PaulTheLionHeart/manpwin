# ManpWIN CMake script
# Copyright 2025 Paul de Leeuw
#

cmake_minimum_required(VERSION 3.23)

set(MANPWIN_PROJECT_NAME "manpwin" CACHE STRING "Project name for IDEs")

project("${MANPWIN_PROJECT_NAME}")

add_executable(manpwin WIN32

AniInvert.cpp
AniJulia.cpp
anim.cpp
anim.h
ant.cpp
Apollonius.cpp
Approximation.cpp
Arithmetic.cpp
Arithmetic.h
Bif.cpp
Bif.h
Big.h
BigComplex.cpp
BigComplex.h
BigDouble.cpp
BigDouble.h
BigFN.cpp
BigFunctions.cpp
BigMandelDerivatives.cpp
BigManDerivatives.cpp
BigMatrix.cpp
BigMatrix.h
BigPixel.cpp
BigTrig.cpp
BigTrig.h
BigTZ.cpp
Bt.cpp
BuddhaBrot.cpp
CalcMode.cpp
Circle.cpp
Clipbord.cpp
CodeVersion.h
Colour.cpp
colour.h
Colour1.cpp
ColourMethod.cpp
Complex.cpp
Complex.h
Config.cpp
Cross.cpp
Curves.cpp
DDComplex.cpp
DDComplex.h
DDFractintFunctions.cpp
DDFractintTrig.cpp
DDFunctions.cpp
DDMandelDerivativesFN.cpp
DDMatrix.cpp
DDMatrix.h
DDPixel.cpp
DDTZFunctions.cpp
Decomp.cpp
Default.cpp
DerivSlope.cpp
Dib.cpp
Dib.h
Dither.cpp
Dl3quant.cpp
Dostereo.cpp
EditPal.cpp
EditPal.h
EditPalDlg.cpp
ExpComplex.cpp
ExpComplex.h
FDimension.cpp
FFT.cpp
fibonaci.cpp
filter.cpp
Filter.h
FilterData.cpp
floatexp.h
fourier.cpp
fpu087.cpp
Fract.cpp
fract.h
FractalMaps.cpp
fractalp.cpp
fractalp.h
FractintFilters.cpp
FractintFunctions.cpp
FractintTrig.cpp
Fractype.h
FracZoom.cpp
FrameCalculator.cpp
FrameCalculator.h
FrantPAR.cpp
Functions.cpp
FwdDiff.cpp
FwdDiffFn.cpp
Genline.cpp
Geometry.cpp
GIFwrite.cpp
Guess.cpp
Henon.cpp
htmlhelp.cpp
icon1.ico
KallesPAR.cpp
Keyboard.cpp
Kleinian.cpp
KnotOsc.cpp
Knots.cpp
Line3d.cpp
Lorenz.cpp
Lsys.cpp
Malthus.cpp
MandelDerivatives.cpp
MandelDerivativesFN.cpp
Maninit.cpp
Manp.cpp
Manp.h
Manpdlg.cpp
Manpfile.cpp
Manpmain.cpp
Manpwin.cpp
Manpwin.h
MANPWIN.ICO
Manpwin.rc
Matrix.cpp
Matrix.h
menu.h
Miscfrac.cpp
Mountain.cpp
MPEGWrite.cpp
mpfr.h
Newton.cpp
OscAnim.cpp
OscDatabase.cpp
Oscillators.cpp
OscMorph.cpp
OscProcess.cpp
OscProcess.h
OtherFunctions.cpp
OtherFunctions.h
ParamAnim.cpp
Pascal.cpp
PertFN.cpp
PertFNFloatExp.cpp
Perturbation.cpp
Pixel.cpp
Pixel.h
Plot.cpp
Plot.h
png.cpp
Polygon.cpp
Polygon.h
Popcorn.cpp
Potential.cpp
Potential.h
preview.cpp
Preview.h
QDComplex.cpp
QDComplex.h
QDFractintFunctions.cpp
QDFractintTrig.cpp
QDFunctions.cpp
QDMandelDerivativesFN.cpp
QDMatrix.cpp
QDMatrix.h
QDPixel.cpp
QDTZFunctions.cpp
resource.h
Rotate.cpp
RTJulia.cpp
Savefile.cpp
SaveVars.cpp
Script.cpp
SierpinskiFlowers.cpp
Slope.cpp
Slope.h
SlopeMT.cpp
SprottLinz.cpp
SprottMaps.cpp
SprottOsc.cpp
Stereo.cpp
Surfaces.cpp
Sym.cpp
SymPlot.cpp
Tess.cpp
Tetration.cpp
Tetration.h
tierazon.cpp
TierazonFunctions.cpp
Triangles.cpp
trig.cpp
TZFilterDLG.cpp
undo.cpp
Update.cpp
User.cpp
Zigzag.cpp
Zoom.cpp
Zoom.h
)

add_subdirectory(mpeg)
add_subdirectory(parser)
add_subdirectory(pnglib)
add_subdirectory(qdlib)
add_subdirectory(zlib)

target_link_libraries(manpwin PUBLIC mpeg)
target_link_libraries(manpwin PUBLIC parser)
target_link_libraries(manpwin PUBLIC pnglib)
target_link_libraries(manpwin PUBLIC qdlib)
target_link_libraries(manpwin PUBLIC zlib)
target_link_libraries(manpwin PUBLIC htmlhelp)
#target_link_libraries(manpwin PUBLIC mpfr)

add_library(mpfr STATIC IMPORTED)
set_target_properties(mpfr PROPERTIES IMPORTED_LOCATION_DEBUG C:/Github/manpwin/mpfr/build.vc15/lib/x64/Debug/mpfr.lib)
set_target_properties(mpfr PROPERTIES IMPORTED_LOCATION_RELEASE C:/Github/manpwin/mpfr/build.vc15/lib/x64/Release/mpfr.lib)
target_link_libraries(manpwin PUBLIC mpfr)

