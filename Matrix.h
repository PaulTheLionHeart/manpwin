#include <math.h>

typedef double	MATRIX[4][4];		// matrix of floats 
typedef double	VECTOR[3];		// vector of floats 
typedef double	MATRIXPTR[][4];		// fussy array warnings from HI_TECH 
typedef double	VECTORPTR[];		// ditto 

#pragma once
class CMatrix
    {
    public:
	void	xrot(double, MATRIXPTR);
	void	yrot(double, MATRIXPTR);
	void	zrot(double, MATRIXPTR);
	void	trans(double, double, double, MATRIXPTR);
	void	identity(MATRIXPTR);
	int	vmult(VECTORPTR, MATRIXPTR, VECTORPTR);
	void	mat_mul(MATRIXPTR mat1, MATRIXPTR mat2, MATRIXPTR mat3);
	void	scale(double sx, double sy, double sz, MATRIXPTR m);
	void	InitTransformation(double tx, double ty, double tz, double x_rot, double y_rot, double z_rot);
	void	DoTransformation(double *x1, double *y1, double *z1, double x, double y, double z);

    private:
	MATRIX	m;
    };

