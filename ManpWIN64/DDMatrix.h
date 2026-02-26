#include <math.h>
#include "DDComplex.h"
typedef dd_real	DDMATRIX[4][4];		// matrix of double-double 
typedef dd_real	DDVECTOR[3];		// vector of double-double 
typedef dd_real	DDMATRIXPTR[][4];	// fussy array warnings
typedef dd_real	DDVECTORPTR[];		// ditto

#pragma once
class CDDMatrix
    {
    public:
	void	xrot(double, DDMATRIXPTR);
	void	yrot(double, DDMATRIXPTR);
	void	zrot(double, DDMATRIXPTR);
	void	trans(dd_real, dd_real, dd_real, DDMATRIXPTR);
	void	identity(DDMATRIXPTR);
	int	vmult(DDVECTORPTR, DDMATRIXPTR, DDVECTORPTR);
	void	mat_mul(DDMATRIXPTR mat1, DDMATRIXPTR mat2, DDMATRIXPTR mat3);
	void	scale(double sx, double sy, double sz, DDMATRIXPTR m);
	void	InitTransformation(dd_real tx, dd_real ty, dd_real tz, double x_rot, double y_rot, double z_rot);
	void	DoTransformation(dd_real *x1, dd_real *y1, dd_real *z1, dd_real x, dd_real y, dd_real z);

    private:
	DDMATRIX	m;
    };

