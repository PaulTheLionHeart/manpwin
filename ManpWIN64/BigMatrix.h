#include <math.h>
#include "BigDouble.h"
typedef BigDouble	BIGMATRIX[4][4];	// matrix of BigDoubles 
typedef BigDouble	BIGVECTOR[3];		// vector of BigDoubles 
typedef BigDouble	BIGMATRIXPTR[][4];	// fussy array warnings
typedef BigDouble	BIGVECTORPTR[];		// ditto

#pragma once
class CBigMatrix
    {
    public:
	void	xrot(double, BIGMATRIXPTR);
	void	yrot(double, BIGMATRIXPTR);
	void	zrot(double, BIGMATRIXPTR);
	void	trans(BigDouble, BigDouble, BigDouble, BIGMATRIXPTR);
	void	identity(BIGMATRIXPTR);
	int	vmult(BIGVECTORPTR, BIGMATRIXPTR, BIGVECTORPTR);
	void	mat_mul(BIGMATRIXPTR mat1, BIGMATRIXPTR mat2, BIGMATRIXPTR mat3);
	void	scale(double sx, double sy, double sz, BIGMATRIXPTR m);
	void	InitTransformation(BigDouble tx, BigDouble ty, BigDouble tz, double x_rot, double y_rot, double z_rot);
	void	DoTransformation(BigDouble *x1, BigDouble *y1, BigDouble *z1, BigDouble x, BigDouble y, BigDouble z);

    private:
	BIGMATRIX	m;
    };

