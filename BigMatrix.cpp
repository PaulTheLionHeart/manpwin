/*
    BIGMATRIX.CPP a program to do matrix manipulations on Bignum
    
    Written in Microsoft Visual 'C++' by Paul de Leeuw.

    This program is written in "standard" C. Hardware dependant code
    (console drivers & serial I/O) is in separate machine libraries.
*/

#include "BigMatrix.h"

/***************************************************************************
	Initialise a matrix and set to identity matrix
***************************************************************************/

void	CBigMatrix::identity(BIGMATRIXPTR m)

    {
    int	i, j;

    for (i = 0; i < 4; i++)
	for (j = 0; j < 4; j++)
	    if (i == j)
		m[j][i] = 1.0;
	    else
		m[j][i] = 0.0;
    }

/***************************************************************************
	Multiply two matrices 	Result stored in MATRIX new to avoid problems
	in case parameter mat3 == mat2 or mat 1 
***************************************************************************/

void	CBigMatrix::mat_mul(BIGMATRIXPTR mat1, BIGMATRIXPTR mat2, BIGMATRIXPTR mat3)

    {
    BIGMATRIX	New;
    int	i, j;

    for (i = 0; i < 4; i++)
	for (j = 0; j < 4; j++)
	    New[j][i] = mat1[j][0] * mat2[0][i] + mat1[j][1] * mat2[1][i] 
			    + mat1[j][2] * mat2[2][i] + mat1[j][3]*mat2[3][i];

     // should replace this with MPFR calls
    for (i = 0; i < 4; i++)
	for (j = 0; j < 4; j++)
	    mat3[j][i] = New[j][i];
    }

/***************************************************************************
	Multiply a matrix by a scalar
***************************************************************************/

void	CBigMatrix::scale(double sx, double sy, double sz, BIGMATRIXPTR m)

    {
    BIGMATRIX	scale;

    identity(scale);
    scale[0][0] = sx;
    scale[1][1] = sy;
    scale[2][2] = sz;
    mat_mul(m, scale, m);
    }

/***************************************************************************
	Rotate about X axis
***************************************************************************/

void	CBigMatrix::xrot(double theta, BIGMATRIXPTR m)

    {
    BIGMATRIX	rot;
    double	sintheta, costheta;

    sintheta = sin(theta);
    costheta = cos(theta);
    identity(rot);
    rot[1][1] = costheta;
    rot[1][2] = -sintheta;
    rot[2][1] = sintheta;
    rot[2][2] = costheta;
    mat_mul(m, rot, m);
    }

/***************************************************************************
	Rotate about Y axis
***************************************************************************/

void	CBigMatrix::yrot(double theta, BIGMATRIXPTR m)

    {
    BIGMATRIX	rot;
    double	sintheta, costheta;

    sintheta = sin(theta);
    costheta = cos(theta);
    identity(rot);
    rot[0][0] = costheta;
    rot[0][2] = sintheta;
    rot[2][0] = -sintheta;
    rot[2][2] = costheta;
    mat_mul(m, rot, m);
    }

/***************************************************************************
	Rotate about Z axis
***************************************************************************/

void	CBigMatrix::zrot(double theta, BIGMATRIXPTR m)

    {
    BIGMATRIX rot;
    double sintheta, costheta;

    sintheta = sin(theta);
    costheta = cos(theta);
    identity(rot);
    rot[0][0] = costheta;
    rot[0][1] = -sintheta;
    rot[1][0] = sintheta;
    rot[1][1] = costheta;
    mat_mul(m, rot, m);
    }

/***************************************************************************
	Translate matrix
***************************************************************************/

void	CBigMatrix::trans(BigDouble tx, BigDouble ty, BigDouble tz, BIGMATRIXPTR m)

    {
    BIGMATRIX	trans;

    identity(trans);
    trans[3][0] = tx;
    trans[3][1] = ty;
    trans[3][2] = tz;
    mat_mul(m, trans, m);
    }

/***************************************************************************
	Multiply source vector s by matrix m, result in target t
***************************************************************************/

int	CBigMatrix::vmult(BIGVECTORPTR s, BIGMATRIXPTR m, BIGVECTORPTR t)

    {
    BIGVECTOR	tmp;
    int	i, j;

    for (j = 0; j < 3; j++)
	{
	tmp[j] = 0.0;
	for (i = 0; i < 3; i++)
	    tmp[j] += s[i] * m[i][j];
	// vector is really four dimensional with last component always 1
	tmp[j] += m[3][j];
	}
	// set target = tmp. Necessary to use tmp in case source = target
    for (i = 0; i < 3; i++)
	t[i] = tmp[i];
    return (0);
    }                                                                        

/**************************************************************************
	Initialise transformation Matrix
***************************************************************************/

void	CBigMatrix::InitTransformation(BigDouble tx, BigDouble ty, BigDouble tz, double x_rot, double y_rot, double z_rot)

    {
    double	xval, yval, zval;

    if (z_rot > 360.0)  z_rot -= 360.0;
    if (y_rot > 360.0)  y_rot -= 360.0;
    if (x_rot > 360.0)  x_rot -= 360.0;

    xval = x_rot / 57.29577;					// rot values - convert degrees to radians
    yval = y_rot / 57.29577;
    zval = z_rot / 57.29577;
    identity(m);						// start with identity 
    trans(-tx, -ty, -tz, m);					// translate so origin is in centre, so that we rotate through the centre
    xrot(xval, m);						// rotate in each axis
    yrot(yval, m);
    zrot(zval, m);
    trans(tx, ty, tz, m);					// translate back
    }

/**************************************************************************
	Implement transformation Matrix
***************************************************************************/

void	CBigMatrix::DoTransformation(BigDouble *x1, BigDouble *y1, BigDouble *z1, BigDouble x, BigDouble y, BigDouble z)

    {
    BIGVECTOR	w;

    w[0] = x;
    w[1] = y;
    w[2] = z;
    vmult(w, m, w);
    *x1 = w[0];
    *y1 = w[1];
    *z1 = w[2];
    }

