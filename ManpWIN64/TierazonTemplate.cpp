#include "TierazonTemplate.h"

// TierazonTemplate.cpp
//
// Intentionally empty.
// Template implementations are defined in TierazonTemplate.h
// because templates must be visible at the point of instantiation.

// When WOULD you need it?
// Only if you later do something like :
// * Explicit template instantiation
// template void TierazonIter_Generic<BigComplex>(...);
//
// Then a.cpp can be useful to :
// * reduce compile times
// * centralise instantiations
