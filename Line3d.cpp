/*
    LINE3D.CPP a program to investigate the Mandelbrot set. This version reads
    values of count from file and displays the data transformed into 
    a 3 dimensional image. For more information refer to Scientific 
    American August 1985.
    
    Written in Microsoft Visual 'C++' by Paul de Leeuw.

    This program is written in "standard" C. Hardware dependant code
    (console drivers & serial I/O) is in separate machine libraries.
*/

#include "manp.h"
#include "colour.h"
#include "Matrix.h"
#include "pixel.h"

/**************************************************************************
	Initialise 3D parameters
**************************************************************************/

void	CPixel::init3d(int xdots, int ydots, double x_rot, double y_rot, double z_rot, double sclx, double scly, double sclz, long threshold, double hor, double vert)
	
    {
    CMatrix Mat;

    Mat.identity(m);				/* start with identity */

/* translate so origin is in centre, so that we rotate through the centre */

    Mat.trans((double) xdots / (-2.0), (double) ydots / (-2.0), 0.0, m);

    Mat.scale(sclx, scly, sclz, m);		/* apply scale factors */

    xval = x_rot / 57.29577;	/* rot values - convert degrees to radians */
    yval = y_rot / 57.29577;
    zval = z_rot / 57.29577;

    Mat.xrot(xval, m);
    Mat.yrot(yval, m);
    Mat.zrot(zval, m);

    Mat.trans((double) xdots / (2.0), (double) ydots / (2.0), 0.0, m);
    }

/**************************************************************************
	Project 3 dimensions onto 2
**************************************************************************/

void	CPixel::projection(int x, int y, long col)

    {
    VECTOR	w;
    struct	PointInfo	cur;				// current pixels
    int	colour;
    CMatrix Mat;

    colour = (col >= MAXTHRESHOLD) ? MAXTHRESHOLD : (int)col;	// reduce to 15 bits
    cur.colour = colour;

    w[0] = (double) x;
    w[1] = (double) y;
    w[2] = (double) colour;
    Mat.vmult(w, m, w);
    cur.x = (int) (w[0]);
    cur.y = (int) (w[1]);
    if (y > 0 && x > 0)
	do_line((short)cur.x, (short)cur.y, (short)oldpoint.x, (short)oldpoint.y, (short)oldpoint.colour, (short)cur.colour);
    oldpoint.x = cur.x;
    oldpoint.y = cur.y;
    oldpoint.colour = cur.colour;
    if (y > 0 && x > 0)
	do_line((short)cur.x, (short)cur.y, (short)oldline[x].x, (short)oldline[x].y, (short)oldline[x].colour, (short)cur.colour);
    oldline[x].x = cur.x;
    oldline[x].y = cur.y;
    oldline[x].colour = cur.colour;
    }	

#ifdef SPLAT
/***************************************************************************
	Initialise a matrix and set to identity matrix
***************************************************************************/

void	CPixel::identity(MATRIXPTR m)

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

void	CPixel::mat_mul(MATRIXPTR mat1, MATRIXPTR mat2, MATRIXPTR mat3)

    {
    MATRIX	New;
    int	i, j;

    for (i = 0; i < 4; i++)
	for (j = 0; j < 4; j++)
	    New[j][i] = mat1[j][0] * mat2[0][i] + mat1[j][1] * mat2[1][i] 
			    + mat1[j][2] * mat2[2][i] + mat1[j][3]*mat2[3][i];

     /* should replace this with memcpy */
    for (i = 0; i < 4; i++)
	for (j = 0; j < 4; j++)
	    mat3[j][i] = New[j][i];
    }

/***************************************************************************
	Multiply a matrix by a scalar
***************************************************************************/

void	CPixel::scale(double sx, double sy, double sz, MATRIXPTR m)

    {
    MATRIX	scale;

    identity(scale);
    scale[0][0] = sx;
    scale[1][1] = sy;
    scale[2][2] = sz;
    mat_mul(m, scale, m);
    }

/***************************************************************************
	Rotate about X axis
***************************************************************************/

void	CPixel::xrot(double theta, MATRIXPTR m)

    {
    MATRIX	rot;
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

void	CPixel::yrot(double theta, MATRIXPTR m)

    {
    MATRIX	rot;
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

void	CPixel::zrot(double theta, MATRIXPTR m)

    {
    MATRIX rot;
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

void	CPixel::trans(double tx, double ty, double tz, MATRIXPTR m)

    {
    MATRIX	trans;

    identity(trans);
    trans[3][0] = tx;
    trans[3][1] = ty;
    trans[3][2] = tz;
    mat_mul(m, trans, m);
    }

/***************************************************************************
	Multiply source vector s by matrix m, result in target t
***************************************************************************/

int	CPixel::vmult(VECTORPTR s, MATRIXPTR m, VECTORPTR t)

    {
    VECTOR	tmp;
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

#endif
/***************************************************************************
	Interpolate colours
***************************************************************************/

void	CPixel::interpcolour(short x0, short y0, short x1, short y1,
					int x, int y, int col0, int col1)

    {
    int	colour;
    long	d1, d2, col;

    if ((abs(x1 - x0)) > (abs(y1 - y0)) && (x1 - x0) != 0)
	{
	d1 = (long) (x1 - x);
	d2 = (long) (x1 - x0);
	}
    else if (y1 - y0 != 0)
	{
	d1 = (long) (y1 - y);
	d2 = (long) (y1 - y0);
	}
    else
	d1 = d2 = 1l;
    col = (long) (col1 - col0);
    colour = abs(col0 + (int) ((col * d1) / d2));

    if (2 <= x  && x <= (int)xdots - 3 && 2 <= y && y <= (int)ydots - 3)
	if (0 < colour)
	    PlotPixel((WORD)x, (WORD)y, colour);
    }

/**************************************************************************
	Draw a line from raw x0,y0 to x1,y1 and interp colour
**************************************************************************/

void	CPixel::do_line(short x0, short y0, short x1, short y1, short col0, short col1)

    {
    register short xlo, ylo;
    register short c;
    short xhi, yhi;
    long a, b;

    ylo = min(y0, y1);
    yhi = max(y0, y1);
    xlo = min(x0, x1);
    xhi = max(x0, x1);

    if (x0 == x1)
	{
	while (ylo <= yhi)
	    interpcolour(x0, y0, x1, y1, x0, ylo++, col0, col1);
	}
    else if (y0 == y1)
	{
	while (xlo <= xhi)
	    interpcolour(x0, y0, x1, y1, xlo++, y0, col0, col1);
	}
    else 
	{ 
	a = (x1 - x0); b = (y1 - y0); c = 0;
	if (xhi-xlo > yhi-ylo) 
	    {
	    y0 = (xlo == x0) ? y0 : y1;
	    while (xlo <= xhi)
		interpcolour(x0, y0, x1, y1, 
				    xlo++, (short)(y0 + (b * c++) / a), col0, col1);
	    } 
	else 
	    {
	    x0 = (ylo == y0) ? x0 : x1;
	    while (ylo <= yhi)
		interpcolour(x0, y0, x1, y1, 
				    (short)(x0 + (a * c++) / b), ylo++, col0, col1);
	    }
	}
    }
