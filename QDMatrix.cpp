/*
    QDMATRIX.CPP a program to do matrix manipulations on quad-double
    
    Written in Microsoft Visual 'C++' by Paul de Leeuw.

    This program is written in "standard" C. Hardware dependant code
    (console drivers & serial I/O) is in separate machine libraries.
*/

#include "QDMatrix.h"

/***************************************************************************
	Initialise a matrix and set to identity matrix
***************************************************************************/

void	CQDMatrix::identity(QDMATRIXPTR m)

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

void	CQDMatrix::mat_mul(QDMATRIXPTR mat1, QDMATRIXPTR mat2, QDMATRIXPTR mat3)

    {
    QDMATRIX	New;
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

void	CQDMatrix::scale(double sx, double sy, double sz, QDMATRIXPTR m)

    {
    QDMATRIX	scale;

    identity(scale);
    scale[0][0] = sx;
    scale[1][1] = sy;
    scale[2][2] = sz;
    mat_mul(m, scale, m);
    }

/***************************************************************************
	Rotate about X axis
***************************************************************************/

void	CQDMatrix::xrot(double theta, QDMATRIXPTR m)

    {
    QDMATRIX	rot;
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

void	CQDMatrix::yrot(double theta, QDMATRIXPTR m)

    {
    QDMATRIX	rot;
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

void	CQDMatrix::zrot(double theta, QDMATRIXPTR m)

    {
    QDMATRIX rot;
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

void	CQDMatrix::trans(qd_real tx, qd_real ty, qd_real tz, QDMATRIXPTR m)

    {
    QDMATRIX	trans;

    identity(trans);
    trans[3][0] = tx;
    trans[3][1] = ty;
    trans[3][2] = tz;
    mat_mul(m, trans, m);
    }

/***************************************************************************
	Multiply source vector s by matrix m, result in target t
***************************************************************************/

int	CQDMatrix::vmult(QDVECTORPTR s, QDMATRIXPTR m, QDVECTORPTR t)

    {
    QDVECTOR	tmp;
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

void	CQDMatrix::InitTransformation(qd_real tx, qd_real ty, qd_real tz, double x_rot, double y_rot, double z_rot)

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

void	CQDMatrix::DoTransformation(qd_real *x1, qd_real *y1, qd_real *z1, qd_real x, qd_real y, qd_real z)

    {
    QDVECTOR	w;

    w[0] = x;
    w[1] = y;
    w[2] = z;
    vmult(w, m, w);
    *x1 = w[0];
    *y1 = w[1];
    *z1 = w[2];
    }

