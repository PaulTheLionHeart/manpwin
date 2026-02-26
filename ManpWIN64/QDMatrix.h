#include <math.h>
#include "QDComplex.h"
typedef qd_real	QDMATRIX[4][4];		// matrix of quad-double 
typedef qd_real	QDVECTOR[3];		// vector of quad-double 
typedef qd_real	QDMATRIXPTR[][4];	// fussy array warnings
typedef qd_real	QDVECTORPTR[];		// ditto

#pragma once
class CQDMatrix
    {
    public:
	void	xrot(double, QDMATRIXPTR);
	void	yrot(double, QDMATRIXPTR);
	void	zrot(double, QDMATRIXPTR);
	void	trans(qd_real, qd_real, qd_real, QDMATRIXPTR);
	void	identity(QDMATRIXPTR);
	int	vmult(QDVECTORPTR, QDMATRIXPTR, QDVECTORPTR);
	void	mat_mul(QDMATRIXPTR mat1, QDMATRIXPTR mat2, QDMATRIXPTR mat3);
	void	scale(double sx, double sy, double sz, QDMATRIXPTR m);
	void	InitTransformation(qd_real tx, qd_real ty, qd_real tz, double x_rot, double y_rot, double z_rot);
	void	DoTransformation(qd_real *x1, qd_real *y1, qd_real *z1, qd_real x, qd_real y, qd_real z);

    private:
	QDMATRIX	m;
    };

