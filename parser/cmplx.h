// Hyper (4D) complex number defs
#ifndef _CMPLX_DEFINED
#define _CMPLX_DEFINED

#include "..\qdlib\dd_real.h"
#include "..\qdlib\qd_real.h"
struct DHyperComplex
    {
    double x, y;
    double z, t;
    };

typedef struct  DHyperComplex    _HCMPLX;

struct DDHyperComplex
    {
    dd_real x, y;
    dd_real z, t;
    };

typedef struct  DDHyperComplex    _DDHCMPLX;

struct QDHyperComplex
    {
    qd_real x, y;
    qd_real z, t;
    };

typedef struct  QDHyperComplex    _QDHCMPLX;

#endif
