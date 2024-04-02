//////////////////////////////////////////////////////////////////////////////////////////
//
// OSCPROCESS.CPP a module for oscillator, fractal map, surface, knots and curves functions.
//
//////////////////////////////////////////////////////////////////////////////////////////
// Author:-
//	Paul de Leeuw
//	pdeleeuw at deleeuw dot com dot au	( replace "at" "dot" by the normal characters.)
//	11/12/2007
//	This Class includes code to create a true palette and to modify it
//////////////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include <windowsx.h>
#include "manpwin.h"
#include "manp.h"
#include "fractalp.h"
#include "Dib.h"
#include "OscProcess.h"
#include "resource.h"
#include "fractype.h"
#include "colour.h"
#include "Anim.h"

// norty globals
extern	PAINTSTRUCT 	ps;
extern	int	xdots, ydots, width, height;
// some glabal routines that need to be passed in at initialisation
extern	int	user_data(HWND);
extern	void	ClearScreen(void);
extern	void	DisplayStatusBarInfo(int, char *);
extern	void	InitTransformation(double tx, double ty, double tz);
extern	double	GetNumFrames(void);
extern	void	DoTransformation(double *x1, double *y1, double *z1, double x, double y, double z);

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COscProcess::COscProcess()
    {
    }

COscProcess::~COscProcess()
    {
    }

/**************************************************************************
	Plot basal plane and vertical axis
***************************************************************************/

void	COscProcess::InitOscProc(int dimensions, double InMandel_width, double InScreenRatio, int InxDots, int InyDots, WORD InType, int InSubType, ProcessType OscAnimProc, int *InxAxis, int *InyAxis, int *InzAxis, int InFindCentre, 
	CDib *Dib, double c1[], BYTE PerspectiveFlag, double InIterations, RGBTRIPLE InOscBackGround, double *x_rot, double *y_rot, double *z_rot, BOOL *InRemoveHiddenPixels, HWND hwndIn, double *wpixelsIn, CTrueCol *TrueColIn)
    {
    int	    j;

    type = InType;
    subtype = InSubType;
    iterations = InIterations;
    FindCentre = InFindCentre;
    mandel_width = InMandel_width;
    ScreenRatio = InScreenRatio;
    xAxis = InxAxis;
    yAxis = InyAxis;
    zAxis = InzAxis;
    wpixels = wpixelsIn;
    TrueCol = TrueColIn;

    RemoveHiddenPixels = InRemoveHiddenPixels;
    OscBackGround.rgbtRed = InOscBackGround.rgbtRed;
    OscBackGround.rgbtGreen = InOscBackGround.rgbtGreen;
    OscBackGround.rgbtBlue = InOscBackGround.rgbtBlue;

    hwnd = hwndIn;

    if (dimensions >= MAXDIM)
	dimensions = MAXDIM - 1;

    Plot.InitPlot(100000L, TrueCol, wpixels, xdots, ydots, xdots, ydots, Dib->BitsPerPixel, Dib, USEPALETTE);
    xscale = (double)(xdots - 1) / (mandel_width * ScreenRatio);
    yscale = (double)(ydots - 1) / mandel_width;
    axes = dimensions;

    if (type == OSCILLATORS)
	axes *= 2;
    if (OscAnimProc == STANDARD)
	{
	xSum = 0.0;
	ySum = 0.0;
	zSum = 0.0;
	}
    if (OscAnimProc == INITANIM)
	{
	DisplayAxisImages = FALSE;
	AxisDisplay = OFF;
	FrameCounter = 0.0;
	for (j = 0; j < axes; j++)
	    {
	    cMax[j] = -1.0E30;
	    cMin[j] = 1.0E30;
	    }
	if (!PerspectiveFlag)
	    *x_rot = *y_rot = *z_rot = 0.0;
	}

    if (OscAnimProc == MORPHING || DisplayAxisImages)
	*RemoveHiddenPixels = FALSE;					// memory management is too difficult and hard to see

    else if (type == KNOTS || type == SURFACES || type == CURVES)
	{
	if (*RemoveHiddenPixels)
	    {
	    if ((zValuesKnots = new double[xdots * ydots]) == NULL)	// need a floating point value of z for each pixel
		{
		zValuesKnots = NULL;
		*RemoveHiddenPixels = FALSE;
		}
	    else
		for (j = 0; j < xdots * ydots; j++)
		    *(zValuesKnots + j) = -1.0E100;			// negative enough to be in the background
	    }
	}

    if (OscAnimProc == RUNANIM)
	{
	if (FindCentre == BOUNDARY)
	    InitTransformation((cMax[*xAxis] + cMin[*xAxis]) / 2, (cMax[*yAxis] + cMin[*yAxis]) / 2, (cMax[*zAxis] + cMin[*zAxis]) / 2);	// translate to the centre of the object
	else
	    InitTransformation(xSum / iterations, ySum / iterations, zSum / iterations);	// translate to the centre of the object
	}

    Dib->ClearDib(OscBackGround.rgbtRed, OscBackGround.rgbtGreen, OscBackGround.rgbtBlue);	// set background colour
    }

/**************************************************************************
	Common Display Routines
***************************************************************************/

int	COscProcess::DisplayOscillator(double c1[], double cn[], double dt, DWORD colour, double i, int dimensions, BOOL InDisplayLines, int xSign, int ySign, double hor, double vert, 
															    double InVertBias, double InzBias, ProcessType OscAnimProc, int InCoordSystem, long threshold, double ColourFactor, int KnotWidth)
    {
    double	x, y, z, x1, y1, z1, xRaw, yRaw, zRaw, delta;
    int		j;
    static	int u, v, uOld, vOld;
    BOOL	PlotLines = TRUE;
    BOOL	FractalMapColouring = DatabasePtr->flags & 1;
//    BOOL    RemoveHiddenPixels;		// used in Knots, Surfaces and Curves

//    RemoveHiddenPixels = DatabasePtr->flags & 1;
    DisplayLines = InDisplayLines;
    PlotLines = DisplayLines;
    VertBias = InVertBias;
    zBias = InzBias;
    CoordSystem = InCoordSystem;

    delta = iterations / GetNumFrames();
    if (type == OSCILLATORS)
	{
	for (j = 0; j < dimensions; j++)
	    {
	    c1[j] += cn[j] * dt;
	    cTot[j] = c1[j];
	    cTot[j + dimensions] = cn[j];	// allow for cn values as well as c1 values
	    }
	}
    else
	{
	for (j = 0; j < axes; j++)
	    cTot[j] = c1[j];
	}
    if (OscAnimProc == INITANIM)
	FindMinMax(cTot);

    xRaw = cTot[*xAxis];
    yRaw = cTot[*yAxis];
    zRaw = cTot[*zAxis]/* * zBias*/;
    ChangeCoordSystem(&x, &y, &z, xRaw, yRaw, zRaw, CoordSystem);						// Co-ordinate system transformations

    if (OscAnimProc == MORPHING || DisplayAxisImages)
	{
	cTot[*xAxis] = x;
	cTot[*yAxis] = y;
	cTot[*zAxis] = z;
	if (type == KNOTS || type == CURVES) 
	    *RemoveHiddenPixels = FALSE;									// too fast to see and there's no need
	if (PointInfo)												// don't crash if we haven't initialised PointInfo
	    LoadRawMorphData(dimensions, (long)iterations, (long)i, cTot, KnotWidth, xAxis, yAxis, zAxis);
	return 0;
	}

    if (OscAnimProc == RUNANIM)
	{
	DoTransformation(&x1, &y1, &z1, x, y, z * zBias);							// 3D rotation transformations
	if (FindCentre == BOUNDARY)										// all points are close to centre
	    {
	    if (xSign > 0)											// axis sign
		u = (int)((x1 - (cMax[*xAxis] + cMin[*xAxis] - mandel_width * ScreenRatio) / 2) * xscale);
	    else
		u = (int)((((cMax[*xAxis] + cMin[*xAxis]) + mandel_width * ScreenRatio) / 2 - x1) * xscale);
	    if (ySign > 0)											// axis sign
		v = (int)((((cMax[*yAxis] + cMin[*yAxis]) * VertBias + mandel_width) / 2 - y1 * VertBias) * yscale);
	    else
		v = (int)((y1 - (cMax[*yAxis] + cMin[*yAxis] - mandel_width) / 2) * VertBias * yscale);
	    }
	else													// too many points off in the distance
	    {
	    if (xSign > 0)											// axis sign
		u = (int)((x1 - xSum / iterations + (mandel_width * ScreenRatio) / 2) * xscale);
	    else
		u = (int)((xSum / iterations + (mandel_width * ScreenRatio) / 2 - x1) * xscale);
	    if (ySign > 0)											// axis sign
		v = (int)((ySum / iterations * VertBias + mandel_width / 2 - y1 * VertBias) * yscale);
	    else
		v = (int)((y1 - ySum / iterations + mandel_width / 2) * VertBias * yscale);
	    }
	}

    if (OscAnimProc == STANDARD || OscAnimProc == EVOLUTION)
	{
	xSum += x;
	ySum += y;
	zSum += z;
	if (xSign > 0)							    // axis sign
	    u = (long)((x - hor) * xscale);
	else
	    u = (long)((hor + mandel_width * ScreenRatio - x) * xscale);
	if (ySign > 0)							    // axis sign
	    v = (long)((vert + mandel_width - y * VertBias) * yscale);
	else
	    v = (long)((y * VertBias - vert) * yscale);
	z1 = z;								    // this is the value to test for hidden pixel
	}
    if (OscAnimProc != INITANIM)
	{
	if (OscAnimProc == EVOLUTION)
	    if (i > FrameCounter * delta)	// evolution done up to this point
		{
		FrameCounter++;
		return -1;
		}
	if (PlotLines)
	    {
	    if (i > 0)
		Plot.genline(u, v, uOld, vOld, colour);
	    uOld = u;
	    vOld = v;
	    }
	else
	    {
	    if ((type == FRACTALMAPS || type == SPROTTMAPS) && FractalMapColouring)
		FractalMapColour(c1[0], c1[1], c1[2], u, v, (dimensions > 2), ColourFactor, threshold, i);
	    else
		{
		if (type == KNOTS || type == CURVES)
		    PlotKnot(u, v, colour, KnotWidth, z1, Plot);
		else if (type == SURFACES && *RemoveHiddenPixels)
		    {
		    if (CheckForHiddenPixels(z1, u, v))
			Plot.PlotPoint(u, v, colour);		// only plot the pixels in the foreground
		    }
		else
		    Plot.PlotPoint(u, v, colour);
		}
	    }
	}
    return 0;
    }

/**************************************************************************
	Implement Co-ordinate System Transformation
***************************************************************************/

void	COscProcess::ChangeCoordSystem(double *x1, double *y1, double *z1, double x, double y, double z, int CoordSystem)

    {
    double  a = 1.0;
    double  b = 2.0;
    double  CoshMinusCos, theta, phi, nu, temp;
    int	    sign;

    switch (CoordSystem)
	{
	case	CARTESIAN:		// no need to change, just copy
	    *x1 = x; *y1 = y; *z1 = z;
	    break;
	case	SPHERICAL:		// these new variables are the conversion from spherical to cartesian coordinates for plotting
	    theta = fmod(y, PI); phi = fmod(z, TWO_PI);				// ensure angles are within range
	    *x1 = x * sin(theta) * cos(phi); *y1 = x * sin(theta) * sin(phi); *z1 = x * cos(theta);
	    break;
	case	CYLINDRICAL:		// these new variables are the conversion from spherical to cylindrical coordinates for plotting
	    phi = fmod(x, TWO_PI);
	    *x1 = y * cos(phi); *y1 = y * sin(phi); *z1 = z;
	    break;
	case	PARABOLIC:		// etc
	    *x1 = (x*x - y * y) / 2.0; *y1 = x * y; *z1 = z;
	    break;
	case	PARABOLOIDAL:
	    phi = fmod(z, TWO_PI);
	    *x1 = x * y * cos(phi); *y1 = x * y * sin(phi); *z1 = (x*x - y * y) / 2.0;
	    break;
	case	ELLIPTICAL:
	    phi = fmod(y, TWO_PI);
	    *x1 = a * cosh(x) * cos(phi); *y1 = a * sinh(x) * sin(phi); *z1 = z;
	    break;
	case	BIPOLAR:
	    phi = fmod(x, TWO_PI);
	    CoshMinusCos = cosh(y) - cos(phi);
	    *x1 = a * sinh(y) / CoshMinusCos; *y1 = a * sin(phi) / CoshMinusCos; *z1 = z;
	    break;
	case	TOROIDAL:
	    sign = (x >= 0.0) ? 1 : -1; theta = fmod(fabs(x), PI) * sign; phi = fmod(z, TWO_PI);
	    CoshMinusCos = cosh(y) - cos(theta);
	    *x1 = a * sinh(y) * cos(phi) / CoshMinusCos; *y1 = a * sinh(y) * sin(phi) / CoshMinusCos; *z1 = a * sin(theta) / CoshMinusCos;
	    break;
	case	PROLATE_SPHERE:				// prolate spheroidal
	    nu = fmod(y, PI); phi = fmod(z, TWO_PI);
	    *x1 = a * sinh(x) * sin(nu) * cos(phi); *y1 = a * sinh(x) * sin(nu) * sin(phi); *z1 = a * cosh(x) * cos(nu);
	    break;
	case	OBLATE_SPHERE:				// oblate spheroidal
	    nu = fmod(y, PI); phi = fmod(z, TWO_PI);
	    *x1 = a * cosh(x) * cos(nu) * cos(phi); *y1 = a * cosh(x) * cos(nu) * sin(phi); *z1 = a * sinh(x) * sin(nu);
	    break;
	case	BISPHERICAL:
	    sign = (x >= 0.0) ? 1 : -1; theta = fmod(fabs(x), PI) * sign; phi = fmod(z, TWO_PI);
	    CoshMinusCos = cosh(y) - cos(theta);
	    *x1 = a * sin(theta) * cos(phi) / CoshMinusCos; *y1 = a * sin(theta) * sin(phi) / CoshMinusCos; *z1 = a * sinh(y) / CoshMinusCos;
	    break;
	case	CONICAL:
	    temp = a * a - b * b;
	    *x1 = x * y * z / a * b;
	    *y1 = x / a * sqrt(fabs((y*y - a * a) * (z*z - a * a) / temp));
	    *z1 = x / b * sqrt(fabs((y*y - b * b) * (z*z - b * b) / temp)); ;
	    break;
	    /*
		    case	BIPOLAR_CYL:				// bipolar cylindrical
			sign = (x >= 0.0) ? 1 : -1; theta = fmod(fabs(x), PI) * sign; phi = fmod(z, TWO_PI);
			CoshMinusCos = cosh(y) - cos(theta);
			*x1 = a * sinh(y) / CoshMinusCos; *y1 = a * sin(theta) / CoshMinusCos; *z1 = z;
			break;
	    */
	}
    }

/**************************************************************************
	Point to the correct database
***************************************************************************/

struct	OscillatorSpecificStuff	*(COscProcess::LoadDatabasePointer(WORD InType, int InSubType))
    {
    type = InType;
    subtype = InSubType;

    switch (type)
	{
	case OSCILLATORS:
	    DatabasePtr = &OscillatorSpecific[subtype];
	    break;
	case FRACTALMAPS:
	    DatabasePtr = &FractalMapSpecific[subtype];
	    break;
	case SPROTTMAPS:
	    DatabasePtr = &SprottMapSpecific[subtype];
	    break;
	case SURFACES:
	    DatabasePtr = &SurfaceSpecific[subtype];
	    break;
	case KNOTS:
	    DatabasePtr = &KnotSpecific[subtype];
	    break;
	case CURVES:
	    DatabasePtr = &CurveSpecific[subtype];
	    break;
	default:
	    DatabasePtr = &OscillatorSpecific[0];
	    break;
	}
    return DatabasePtr;
    }

/**************************************************************************
	Find max and min values of points in each axis
***************************************************************************/

void	COscProcess::FindMinMax(double c1[])
    {
    int i;

    for (i = 0; i < axes; i++)
	{
	if (c1[i] > cMax[i])
	    cMax[i] = c1[i];
	if (c1[i] < cMin[i])
	    cMin[i] = c1[i];
	}
    }

/**************************************************************************
	Plot circle at the centre
***************************************************************************/

void	COscProcess::PlotCircle(int CentrePixels)
    {
    double	xCentre, yCentre, zCentre;
    int		u0, v0, i;
    DWORD	colour = 60;

    Plot.flags = 0;
    if (FindCentre == BOUNDARY)						// all points are close to centre
	{
	xCentre = (cMax[*xAxis] + cMin[*xAxis]) / 2.0;
	yCentre = (cMax[*yAxis] + cMin[*yAxis]) / 2.0;
	zCentre = (cMax[*zAxis] + cMin[*zAxis]) / 2.0; 
	}
    else
	{
	xCentre = xSum / iterations;
	yCentre = ySum / iterations;
	zCentre = zSum / iterations;
	}
    u0 = (int)(xCentre + (mandel_width * ScreenRatio / 2) * xscale);
    v0 = (int)(yCentre * VertBias + (mandel_width / 2) * yscale);
    for (i = 0; i < CentrePixels; i++)
	Plot.DisplayCircle(u0, v0, i, 0x00ffffff);
    }

/**************************************************************************
	Plot basal plane and vertical axis
***************************************************************************/

void	COscProcess::PlotAxes(void)
    {
    double	xCentre, yCentre, zCentre, x0[3], y0[3], z0[3], x1[3], y1[3], z1[3];
    int		u0, v0, u1, v1;
    DWORD	colour = 60;

    Plot.flags = 0;
    if (FindCentre == BOUNDARY)						// all points are close to centre
	{
	xCentre = (cMax[*xAxis] + cMin[*xAxis]) / 2.0;
	yCentre = (cMax[*yAxis] + cMin[*yAxis]) / 2.0;
	zCentre = (cMax[*zAxis] + cMin[*zAxis]) / 2.0;
	}
    else
	{
	xCentre = xSum / iterations;
	yCentre = ySum / iterations;
	zCentre = zSum / iterations;
	}
    DoTransformation(&x0[0], &x0[1], &x0[2], cMin[*xAxis], yCentre, zCentre);
    DoTransformation(&x1[0], &x1[1], &x1[2], cMax[*xAxis], yCentre, zCentre);
    u0 = (int)((x0[0] - xCentre + mandel_width * ScreenRatio / 2) * xscale);
    v0 = (int)((yCentre * VertBias + mandel_width / 2 - x0[1] * VertBias) * yscale);
    u1 = (int)((x1[0] - xCentre + mandel_width * ScreenRatio / 2) * xscale);
    v1 = (int)((yCentre * VertBias + mandel_width / 2 - x1[1] * VertBias) * yscale);
    Plot.genline(u0, v0, u1, v1, 0x0000ff00);

    DoTransformation(&y0[0], &y0[1], &y0[2], xCentre, cMin[*yAxis], zCentre);
    DoTransformation(&y1[0], &y1[1], &y1[2], xCentre, cMax[*yAxis], zCentre);
    u0 = (int)((y0[0] - xCentre + mandel_width * ScreenRatio / 2) * xscale);
    v0 = (int)((yCentre * VertBias + mandel_width / 2 - y0[1] * VertBias) * yscale);
    u1 = (int)((y1[0] - xCentre + mandel_width * ScreenRatio / 2) * xscale);
    v1 = (int)((yCentre * VertBias + mandel_width / 2 - y1[1] * VertBias) * yscale);
    Plot.genline(u0, v0, u1, v1, 0x00ffffff);

    DoTransformation(&z0[0], &z0[1], &z0[2], xCentre, yCentre, cMin[*zAxis]);
    DoTransformation(&z1[0], &z1[1], &z1[2], xCentre, yCentre, cMax[*zAxis]);
    u0 = (int)((z0[0] - xCentre + mandel_width * ScreenRatio / 2) * xscale);
    v0 = (int)((yCentre * VertBias + mandel_width / 2 - z0[1] * VertBias) * yscale);
    u1 = (int)((z1[0] - xCentre + mandel_width * ScreenRatio / 2) * xscale);
    v1 = (int)((yCentre * VertBias + mandel_width / 2 - z1[1] * VertBias) * yscale);
    Plot.genline(u0, v0, u1, v1, 0x00ff0000);
    }

/**************************************************************************
	Marcus's colouring for Fractal Map
***************************************************************************/

void	COscProcess::FractalMapColour(double x, double y, double z, int u, int v, bool is3d, double ColourFactor, long threshold, double i)
    {
    double	    colour;
    static  double  x1, y1, z1, x2, y2, z2, x3, y3, z3;

    x3 = x2;	y3 = y2;    z3 = z2;
    x2 = x1;	y2 = y1;    z2 = z1;
    x1 = x;	y1 = y;	    z1 = z;

    if (ColourFactor == 0)
	{
	colour = (DWORD)(i / 5.0) % threshold;
	Plot.PlotPoint(u, v, (DWORD)colour % threshold);
	}
    else 
	{
	if (is3d)
	    colour = fabs((z3 - z2) * (y1 - y2) - (z1 - z2) * (y3 - y2));
	else 
	    colour = fabs((x3 - x2) * (y1 - y2));
	Plot.PlotPoint(u, v, (DWORD)(colour * ColourFactor) % threshold);
	}
    }

/**************************************************************************
	Close z value memory
***************************************************************************/

void	COscProcess::CloseZvalues(void)
    {
//    BOOL    RemoveHiddenPixels;		// used in Knots, Surfaces and Curves

//    RemoveHiddenPixels = DatabasePtr->flags & 1;
    if (!*RemoveHiddenPixels)
	return;
    if (zValuesKnots)
	delete[] zValuesKnots;
    zValuesKnots = NULL;
    }

/**************************************************************************
	Are these pixels hidden?
***************************************************************************/

BOOL	COscProcess::CheckForHiddenPixels(double z, int u, int v)
    {
    if (!zValuesKnots)
	return TRUE;
    if (u < 0) u = 0;
    if (v < 0) v = 0;
    if (u >= xdots) u = xdots - 1;
    if (v >= ydots) v = ydots - 1;
    if (z > *(zValuesKnots + v * xdots + u))
	{
	*(zValuesKnots + v * xdots + u) = z;
	return TRUE;
	}
    return FALSE;
    }

/**************************************************************************
	Knot Plotting Hidden Routine
**************************************************************************/

void	COscProcess::PlotHiddenKnot(int centrex, int centrey, DWORD colour, int radius, double z, CPlot Plot)

    {
    int	x, y, d, i;

    i = x = 0;
    y = radius;
    d = 3 - 2 * radius;
    Plot.flags = USEPALETTE;
    while (x <= y)							// draw 8 points per iteration...uses best symmetry
	{
	if (CheckForHiddenPixels(z, centrex + x, centrey + y))
	    Plot.DoPlot(centrex + x, centrey + y, colour);
	if (CheckForHiddenPixels(z, centrex - x, centrey + y))
	    Plot.DoPlot(centrex - x, centrey + y, colour);
	if (CheckForHiddenPixels(z, centrex + x, centrey - y))
	    Plot.DoPlot(centrex + x, centrey - y, colour);
	if (CheckForHiddenPixels(z, centrex - x, centrey - y))
	    Plot.DoPlot(centrex - x, centrey - y, colour);
	if (CheckForHiddenPixels(z, centrex + y, centrey + x))
	    Plot.DoPlot(centrex + y, centrey + x, colour);
	if (CheckForHiddenPixels(z, centrex - y, centrey + x))
	    Plot.DoPlot(centrex - y, centrey + x, colour);
	if (CheckForHiddenPixels(z, centrex + y, centrey - x))
	    Plot.DoPlot(centrex + y, centrey - x, colour);
	if (CheckForHiddenPixels(z, centrex - y, centrey - x))
	    Plot.DoPlot(centrex - y, centrey - x, colour);
	if (d < 0)
	    d = d + 4 * x + 6;					// update error term
	else
	    {
	    d = d + 4 * (x - y) + 10;
	    y--;
	    }
	x++;
	}
    }

/**************************************************************************
	Knot Plotting Routine
***************************************************************************/

void    COscProcess::PlotKnot(WORD u, WORD v, DWORD colour, int size, double z, CPlot Plot)
    {
    int i;

    for (i = 0; i < size; i++)
	{
	if (*RemoveHiddenPixels)
	    PlotHiddenKnot(u, v, colour, i, z, Plot);		// only plot the pixels in the foreground
	else
	    Plot.DisplayCircle(u, v, i, colour);
	}
    }

// Morphing stuff starts here

/**************************************************************************
	Initialise Morphing
***************************************************************************/

int	COscProcess::InitMorphing(int FractalAxes, long FractalIterations, int *NumFrames, WORD InType, ProcessType InOscAnimProc, int InFrames, CDib *InDib/*, BOOL DisplayAxisImagesIn*/)
    {
    int	    i;

    iterations = FractalIterations;
    type = InType;
    OscAnimProc = InOscAnimProc;
    axes = FractalAxes;
    frames = InFrames;
    Dib = InDib;

    if (type == OSCILLATORS)
	axes *= 2;
    for (i = 0; i < axes; i++)
	{
	vMax[i] = -1.0E30;
	vMin[i] = 1.0E30;
	}

    if (PointInfo == NULL)
	{
	PointInfo = new double[axes * (long)iterations];
	if (PointInfo == NULL)
	    return -1;
	}

    OscAnimProc = MORPHING;
    *NumFrames = SetupMorph(axes, (long)iterations);
    return 0;
    }

/**************************************************************************
	Load Morphing data
***************************************************************************/

void	COscProcess::LoadRawMorphData(int /*axes*/fred, long iterations, long CurrentPass, double cTot[], int InPlotWidth, int *InxAxis, int *InyAxis, int *InzAxis)
    {
    int i;

    xAxis = InxAxis;
    yAxis = InyAxis;
    zAxis = InzAxis;
    PlotWidth = InPlotWidth;
    for (i = 0; i < axes; i++)
	if (*xAxis == i || *yAxis == i || *zAxis == i)		// only update axes if they are part of current initialisation
	    *(PointInfo + axes * CurrentPass + i) = cTot[i];
    }

/**************************************************************************
	Morphing Main Routine
***************************************************************************/

int	COscProcess::SetupMorph(int axes, long iterations)
    {
    int	    i, j, r, s;
    long    k;
    int	    count;
    double  temp;
//    char    str[MAXLINE];

    for (s = 0; s < axes; s += 3)
	{
	*xAxis = s;
	*yAxis = *xAxis + 1;
	if (*yAxis >= axes)
	    {
	    *yAxis = 0; *zAxis = 1;
	    }
	else
	    {
	    *zAxis = *xAxis + 2;
	    if (*zAxis >= axes)
		*zAxis = 0;
	    }
//	wsprintf(str, "Setting up axes: %d, %d, %d", *xAxis + 1, *yAxis + 1, *zAxis + 1);
	DisplayStatusBarInfo(INCOMPLETE, "");		// display status bar
	switch (type)			// This loads up the fractal info for all points for each axis
	    {
	    case OSCILLATORS:
		if (DoOscillator() < 0)
		    return -1;
		break;
	    case FRACTALMAPS:
		if (DoFractalMaps() < 0)
		    return -1;
		break;
	    case SPROTTMAPS:
		if (DoSprottMaps() < 0)
		    return -1;
		break;
	    case SURFACES:
		if (DoSurface() < 0)
		    return -1;
		break;
	    case KNOTS:
		if (DoKnots() < 0)
		    return -1;
		break;
	    case CURVES:
		if (DoCurves() < 0)
		    return -1;
		break;
	    }
	for (k = 0; k < iterations; k++)
	    for (r = 0; r < axes; r++)
		{
		if (*xAxis == r || *yAxis == r || *zAxis == r)		// only update axes if they are part of current initialisation
		    {
		    temp = *(PointInfo + axes * k + r);
		    if (temp < vMin[r]) vMin[r] = temp;
		    if (temp > vMax[r]) vMax[r] = temp;
		    }
		}
	}

    xscale = (double) (xdots - 1) / (mandel_width * ScreenRatio);
    yscale = (double) (ydots - 1) / mandel_width;

    if (DisplayAxisImages)
	{
	int NumColumns = (axes - 1);
	ZoomValue = (1.0 / (double)NumColumns) * DatabasePtr->width / mandel_width;
	}
    else
	ZoomValue = 0.6 * DatabasePtr->width / mandel_width;

    for (r = 0; r < axes; r++)
	{
	Range[r] = ZoomValue / (vMax[r] - vMin[r]);
	Offset[r] = (vMax[r] + vMin[r]) / 2;
	}

    count = 0;
    for (i = 0; i < axes; i++)
	{
	for (j = i + 1; j < axes; j++)
	    {
	    In1[count] = i;
	    In2[count] = j;
	    Out1[count] = i;
	    Out2[count] = j + 1;
	    if (j >= axes - 1)
		{
		Out1[count] = i + 1;
		Out2[count] = i + j + 3 - axes;
		if (Out2[count] == axes)
		    Out2[count] = 0;
		}
	    count++;
	    }
	}
    if (axes > 1)		// remember to loop back to the original pair of axes
	{
	In1[count] = Out1[count - 1];
	In2[count] = Out2[count - 1];
	Out1[count] = 0;
	Out2[count] = 1;
	count++;
	}

    if (count < 1)
	return 0;
    FramesPerPair = frames / count;
    if (DisplayAxisImages || FramesPerPair < 1)
	FramesPerPair = 1;	// prevent nasty divide by zero errors if high dimensions
    frames = FramesPerPair * count;
    return frames;
    }

/**************************************************************************
	Morph Images
***************************************************************************/

int	COscProcess::MorphStep(HWND hwnd, char *FileName, char *MoreInfo, int TotalFrames, int ThisStep, double *DelayMultiplier, long threshold)
    {
    int		    AxisIndex;
    int		    AxisIn1, AxisIn2, AxisOut1, AxisOut2;
    int		    HorOffset, VertOffset;
    long	    i;
    double	    x1, x2, y1, y2, Ratio;
    DWORD	    colour;
    char	    s[MAXLINE];
    static  WORD    xOld, yOld;
    WORD	    xNew, yNew;

    if (user_data(hwnd) == -1)				// user pressed a key?
	return -1;
    ClearScreen();
    AxisIndex = ThisStep / FramesPerPair;
    Ratio = (double)(ThisStep % FramesPerPair) / (double)FramesPerPair;
    AxisIn1 = In1[AxisIndex];
    AxisIn2 = In2[AxisIndex];
    AxisOut1 = Out1[AxisIndex];
    AxisOut2 = Out2[AxisIndex];
    if (Ratio == 0.0)
	*DelayMultiplier = MorphDelay;
    else
	*DelayMultiplier = 1.0;

//    For an n Dim osc, we have n rows and 2*n-1 columns or a total of 2*n*n-n images n * (2n - 1)
    int NumColumns = (axes - 1);
    int NumRows = (axes / 2);
    if (DisplayAxisImages)
	{
	HorOffset = (ThisStep % NumColumns) * xdots / NumColumns + xdots / 120;
	VertOffset = (ThisStep / NumColumns) * ydots / NumRows + ydots / 40;
	}

//    Dib->ClearDib(OscBackGround.rgbtRed, OscBackGround.rgbtGreen, OscBackGround.rgbtBlue);	// set background colour
    for (i = 0; i < iterations; i++)
	{
	colour = (DWORD)(i / 5.0) % threshold;
	x1 = (1.0 - Ratio) * (Range[AxisIn1]) * (*(PointInfo + axes * i + AxisIn1) - Offset[AxisIn1]);
	x2 = Ratio * (Range[AxisOut1]) * (*(PointInfo + axes * i + AxisOut1) - Offset[AxisOut1]);
	y1 = (1.0 - Ratio) * (Range[AxisIn2]) * (*(PointInfo + axes * i + AxisIn2) - Offset[AxisIn2]);
	y2 = Ratio * (Range[AxisOut2]) * (*(PointInfo + axes * i + AxisOut2) - Offset[AxisOut2]);
	if (DisplayAxisImages)
	    {
	    xNew = (WORD)((x1 + x2) * ydots) + xdots / (2 * NumColumns);
	    yNew = ydots / (3 * NumRows) - (WORD)((y1 + y2) * ydots);
	    }
	else
	    {
	    xNew = (WORD)((x1 + x2) * ydots) + xdots / 2;
	    yNew = ydots / 2 - (WORD)((y1 + y2) * ydots);
	    }

	if (type == KNOTS || type == CURVES)
	    {
	    *RemoveHiddenPixels = FALSE;
	    PlotKnot(xNew, yNew, colour, PlotWidth, 1.0, Plot);
	    }
	else
	    {
	    if (DisplayLines && type != FRACTALMAPS && type != SPROTTMAPS)
		{
		//		if (ThisStep > 0)
		if (DisplayAxisImages)
		    Plot.genline(xNew + HorOffset, yNew + VertOffset, xOld + HorOffset, yOld + VertOffset, colour);
		else
		    Plot.genline(xNew, yNew, xOld, yOld, colour);
		xOld = xNew;
		yOld = yNew;
		}
	    else
		{
		if (DisplayAxisImages)
		    Plot.PlotPoint(xNew + HorOffset, yNew + VertOffset, colour);
		else
		    Plot.PlotPoint(xNew, yNew, colour);
		}
	    }
	}
    wsprintf(s, "Animating: Frame %d of %d", ThisStep + 1, TotalFrames);
    DisplayStatusBarInfo(INCOMPLETE, s);		// display status bar
    wsprintf(MoreInfo, "Paul's Fractals: Frame %d of %d Axes: %d-%d to %d-%d", ThisStep + 1, TotalFrames, In1[AxisIndex] + 1, In2[AxisIndex] + 1, Out1[AxisIndex] + 1, Out2[AxisIndex] + 1);
    SetWindowText(hwnd, s);			// Show formatted text in the caption bar
    return 0;
    }

/**************************************************************************
	Choose Oscillator based on Axes
***************************************************************************/

int	COscProcess::ChooseOsc(int x, int y)
    {
    int	    NumColumns = (axes - 1);
    int	    NumRows = (axes / 2);
    int	    row, col;
    int	    i, k, r;
    int	    cell;
//    char    s[240];
    double  temp;
    DWORD   colour;
    static  WORD    xOld, yOld;
    WORD    xNew, yNew;
    double  x1, y1;

    if (AxisDisplay == DISPLAYSINGLE)
	{
	*Dib = DisplayDib;
//	DisplayAxisImages = FALSE;
	AxisDisplay = DISPLAYALL;
	DisplayStatusBarInfo(INFORMATION, "Full list of images for each axis pair");		// display status bar
	InvalidateRect(hwnd, NULL, FALSE);
	return 0;
	}

    else if (AxisDisplay == OFF)
	{
	DisplayDib = *Dib;
	AxisDisplay = DISPLAYALL;
	}
    col = x * NumColumns / xdots;
    row = y * NumRows / ydots;
    cell = row * NumColumns + col;
    *xAxis = In1[cell];
    *yAxis = In2[cell];
    *zAxis = *yAxis + 1;
    if (*zAxis >= axes)
	{
	*zAxis = (*xAxis == 0) ? 1 : 0;
	}

//    sprintf(s, "h=%d, v=%d row=%d, col=%d, xAxis=%d, yAxis=%d, zAxis=%d", x, y, row, col, *xAxis, *yAxis, *zAxis);
//    MessageBox(hwnd, s, "ManpWIN", MB_ICONEXCLAMATION | MB_OK);
    for (i = 0; i < axes; i++)
	{
	vMax[i] = -1.0E30;
	vMin[i] = 1.0E30;
	}

    if (PointInfo == NULL)
	{
	PointInfo = new double[axes * (long)iterations];
	if (PointInfo == NULL)
	    return -1;
	}

    if (type == OSCILLATORS)
	if (DoOscillator() < 0)
	    {
	    CloseMorphing();
	    return -1;
	    }
    if (type == FRACTALMAPS)
	if (DoFractalMaps() < 0)
	    {
	    CloseMorphing();
	    return -1;
	    }
    for (k = 0; k < iterations; k++)
	for (r = 0; r < axes; r++)
	    {
	    if (*xAxis == r || *yAxis == r || *zAxis == r)		// only update axes if they are part of current initialisation
		{
		temp = *(PointInfo + axes * k + r);
		if (temp < vMin[r]) vMin[r] = temp;
		if (temp > vMax[r]) vMax[r] = temp;
		}
	    }

    xscale = (double)(xdots - 1) / (mandel_width * ScreenRatio);
    yscale = (double)(ydots - 1) / mandel_width;
    ZoomValue = 0.8 * DatabasePtr->width / mandel_width;
    for (r = 0; r < axes; r++)
	{
	Range[r] = ZoomValue / (vMax[r] - vMin[r]);
	Offset[r] = (vMax[r] + vMin[r]) / 2;
	}

    for (i = 0; i < iterations; i++)
	{
	colour = (DWORD)(i / 5.0) % 255;
	x1 = (Range[*xAxis]) * (*(PointInfo + axes * i + *xAxis) - Offset[*xAxis]);
	y1 = (Range[*yAxis]) * (*(PointInfo + axes * i + *yAxis) - Offset[*yAxis]);
	xNew = (WORD)(x1 * ydots) + xdots / 2;
	yNew = ydots / 2 - (WORD)(y1 * ydots);
	if (DisplayLines)
	    {
	    Plot.genline(xNew, yNew, xOld, yOld, colour);
	    xOld = xNew;
	    yOld = yNew;
	    }
	else
	    Plot.PlotPoint(xNew, yNew, colour);
	}

    BeginPaint(hwnd, &ps);
    OutputAxesLabel(hwnd, ps.hdc, *xAxis, *yAxis);
    EndPaint(hwnd, &ps);
    InvalidateRect(hwnd, NULL, FALSE);
    CloseMorphing();
//    DisplayAxisImages = FALSE;
    AxisDisplay = DISPLAYSINGLE;
    return 0;
    }

/**************************************************************************
	Choose Oscillator based on Axes
***************************************************************************/

void	COscProcess::OutputAxesLabel(HWND hwnd, HDC hdc, int x, int y)
    {
//    RECT    AxisRect;
//    LOGFONT lf;
    char    labels[] = "xyzuvwpqrs";
    char    a[12];
    char    b[12];
    char    text[24];

    /*
    SelectObject(hdc, CreateFontIndirect(&lf));

    AxisRect.left = 0;
    AxisRect.top = ydots - 80;
    AxisRect.right = xdots - 1;
    AxisRect.bottom = AxisRect.top + 40;			// allow 40 pixels height for font

    lf.lfEscapement = 0;
    lf.lfOrientation = 0;
    lf.lfQuality = DEFAULT_QUALITY;
    lf.lfOutPrecision = OUT_DEFAULT_PRECIS;
    lf.lfClipPrecision = CLIP_DEFAULT_PRECIS;
    lf.lfWidth = 0;
    lf.lfItalic = 0;
    lf.lfUnderline = 0;
    lf.lfStrikeOut = 0;
    lf.lfOrientation = 0;
    lf.lfWeight = FW_BOLD;
*/
    if (x < axes / 2)
	{
	*a = *(labels + x);
	*(a + 1) = '\0';
	}
    else
	{
	*a = *(labels + x - axes / 2);
	*(a + 1) = 'n';
	*(a + 2) = '\0';
	}
    if (y < axes / 2)
	{
	*b = *(labels + y);
	*(b + 1) = '\0';
	}
    else
	{
	*b = *(labels + y - axes / 2);
	*(b + 1) = 'n';
	*(b + 2) = '\0';
	}
    sprintf(text, "Displaying axes: %s-%s", a, b);
    DisplayStatusBarInfo(INFORMATION, text);		// display status bar
//    Dib->Text2Dib(hdc, &AxisRect, 0x00ffffff, 0L, &lf, TRANSPARENT, text);
    }

/**************************************************************************
	Close Morphing Memory Objects
***************************************************************************/

void	COscProcess::CloseMorphing(void)
    {
    if (PointInfo)
	delete[] PointInfo;
    PointInfo = NULL;
    }

