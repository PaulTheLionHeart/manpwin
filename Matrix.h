#include    "Manp.h"

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
    private:
    };

