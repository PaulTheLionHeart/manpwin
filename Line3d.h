/*
    LINE3D.H a program to investigate the Mandelbrot set. This version reads
    values of count from file and displays the data transformed into
    a 3 dimensional image. For more information refer to Scientific
    American August 1985.

    Written in Microsoft Visual 'C++' by Paul de Leeuw.

    This program is written in "standard" C. Hardware dependant code
    (console drivers & serial I/O) is in separate machine libraries.
*/

#include "manp.h"
#include "colour.h"

//#define	abs(x)		(x<0)?-x:x
#define	sign(x)		((x)>0)?1:((x)<0)?-1:0

#pragma once
class CLine3d
    {
    public:
	void	projection(int x, int y, long col);
	void	xrot(double, MATRIXPTR);
	void	yrot(double, MATRIXPTR);
	void	zrot(double, MATRIXPTR);
	void	trans(double, double, double, MATRIXPTR);
	void	init3d(int xdots, int ydots, double x_rot, double y_rot, double z_rot, double sclx, double scly, double sclz, long threshold, double hor, double vert);
	void	identity(MATRIXPTR);
	int	vmult(VECTORPTR, MATRIXPTR, VECTORPTR);

    private:
	void	do_line(short, short, short, short, short, short);
	void	interpcolour(short, short, short, short, int, int, int, int);
	//void	do_3d(void);
	void	mat_mul(MATRIXPTR, MATRIXPTR, MATRIXPTR);
	void	scale(double, double, double, MATRIXPTR);

	double	x_rot;			// angle display plane to x axis
	double	y_rot;			// angle display plane to y axis
	double	z_rot;			// angle display plane to z axis
	double	sclx, scly, sclz;	// scale 
	double	xval, yval, zval;	// rotate 

	long	threshold;		// maximum iterations
	double	hor;			// horizontal address
	double	vert;			// vertical address 

	MATRIX	m;			// transformation matrice

	struct PointInfo
	    {
	    int x;
	    int y;
	    int colour;
	    };

	struct PointInfo oldline[MAXHORIZONTAL];	// old pixels 
	struct PointInfo oldpoint;			// old pixels 
    };

