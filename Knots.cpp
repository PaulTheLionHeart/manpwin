/*
   KNOTS.CPP a module to explore 3D knots

   Written in Microsoft Visual C++ by Paul de Leeuw.
*/

#include	<stdio.h>
#include	<time.h>
#include	"manp.h"
#include	"Fract.h"
#include	"resource.h"
#include	"fractype.h"
#include	"fractalp.h"
#include	"menu.h"
#include	"anim.h"
#include	"OscProcess.h"

extern	HWND	GlobalHwnd;			// This is the main windows handle

extern	int	user_data(HWND);
extern	void	PlotExtras(void);
extern	void	PrintOsc(int);			// used for listing oscillator names in d:\temp\OscDump.txt

extern	double	x_rot;				// angle display plane to x axis
extern	double	y_rot;				// angle display plane to y axis
extern	double	z_rot;				// angle display plane to z axis
extern	long	threshold;
extern	BYTE	PerspectiveFlag;		// display using perspective
extern	double	ScreenRatio;			// ratio of width / height for the screen
extern	int	CoordSystem;

extern	double	mandel_width;			// width of display 
extern	double	hor;				// horizontal address 
extern	double	vert;				// vertical address 
extern	WORD	type;				// fractal type
extern	int	subtype;
extern	int	curpass, totpasses;
extern	double	param[];
extern	int	OscPtrArray[];			// array of pointers to specific oscillators or fractal maps
extern	int	OscillatorNum;
extern	double	iterations;
extern	double	dt;				// delta time
extern	double	VertBias;			// allow vertical stretching of the image
extern	double	zBias;				// allow stretching of the image in the z direction
extern	int	xAxis, yAxis, zAxis;		// numerical values for axes for chaotic oscillators
extern	BOOL	RemoveHiddenPixels;
extern	int	xdots, ydots;

extern	double 	*wpixels;			// an array of doubles holding slope modified iteration counts

extern	CTrueCol	    TrueCol;		// palette info
extern	RGBTRIPLE	OscBackGround;
extern	ProcessType	OscAnimProc;
extern	COscProcess	OscProcess;
extern	CDib	Dib;				// Device Independent Bitmap

void	InitKnot(double c1[], int dimensions);
int	DisplayKnot(double c1[], DWORD colour, int KnotWidth, double i, int dimensions);
void	CloseZvaluesKnots(void);

/**************************************************************************
	An Introductions to Harmonic Knots - Generic knots - page 359
	Corresponding author: akt@carthague.edu
***************************************************************************/

int	DoAnIntroductions2HarmonicKnots(void)
    {
    int		i, j;
    double	c1[6], t, gap, Start, End, KnotWidth, ColourFactor, p, q;

    c1[0] = c1[3] = param[10];	// x
    c1[1] = c1[4] = param[11];	// y
    c1[2] = c1[5] = param[12];	// z

    Start = param[0];
    End = param[1];
    p = param[2];
    q = 4 * p + 1;
    KnotWidth = param[3];
    ColourFactor = param[4];
    totpasses = 10;

    gap = (End - Start) / iterations;
    t = Start;
    InitKnot(c1, 3);							// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	curpass = (int)(i * totpasses);
	c1[0] = cos(p * i) + 0.25 * cos(p * i) * cos(q * i);
	c1[1] = sin(p * i) + 0.25 * sin(p * i) * cos(q * i);
	c1[2] = 0.25 * sin(q * i);
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	if (DisplayKnot(c1, ((DWORD)(i / ColourFactor) % threshold), (int)KnotWidth, i, 6) < 0)
	    break;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesKnots();
    return 0;
    }

/**************************************************************************
	Borromean Rings
	Meier 2001
	http://www.3d-meier.de/ (Representation of knots 6)
***************************************************************************/

int	DoBorromeanRings(void)

    {
    int		i, j;
    double	c1[6], t, gap, Start, End, r, KnotWidth, ColourFactor;

    c1[0] = c1[3] = param[10];	// x
    c1[1] = c1[4] = param[11];	// y
    c1[2] = c1[5] = param[12];	// z

    Start = param[0];
    End = param[1];
    KnotWidth = param[2];
    ColourFactor = param[3];
    totpasses = 10;

    r = sqrt(3.0) / 3.0;
    gap = (End - Start) / iterations;
    t = Start;
    InitKnot(c1, 6);							// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	curpass = (int)(i * totpasses);
	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	c1[0] = cos(t);							// ring 1
	c1[1] = sin(t) + r;
	c1[2] = cos(3*t) / 3;
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;
	if (DisplayKnot(c1, ((DWORD)(i / ColourFactor) % threshold), (int)KnotWidth, i, 6) < 0)
	    break;
	c1[0] = cos(t) + 0.5;						// ring 2
	c1[1] = sin(t) - r/2;
	c1[2] = cos(3*t) / 3;
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;
	if (DisplayKnot(c1, ((DWORD)(i * 2 / ColourFactor) % threshold), (int)KnotWidth, i, 6) < 0)
	    break;
	c1[0] = cos(t) - 0.5;						// ring 3
	c1[1] = sin(t) - r/2;
	c1[2] = cos(3*t) / 3;
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;
	if (DisplayKnot(c1, ((DWORD)(i * 3 / ColourFactor) % threshold), (int)KnotWidth, i, 6) < 0)
	    break;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesKnots();
    return 0;
    }

/**************************************************************************
	Cinquefoil Knot
	Graphics by Paul Bourke November 2004 
	http://paulbourke.net/geometry/knots/
***************************************************************************/

int	DoCinquefoilKnot(void)

    {
    int		i, j;
    double	c1[6], t, gap, Start, End, KnotWidth, ColourFactor, k;

    c1[0] = c1[3] = param[10];	// x
    c1[1] = c1[4] = param[11];	// y
    c1[2] = c1[5] = param[12];	// z

    Start = param[0];
    End = param[1];
    k = param[2];
    KnotWidth = param[3];
    ColourFactor = param[4];
    totpasses = 10;

    gap = (End - Start) / iterations;
    t = Start;
    InitKnot(c1, 3);							// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	curpass = (int)(i * totpasses);
	c1[0] = cos(t) * (2 - cos(2*t/(2*k + 1)));
	c1[1] = sin(t) * (2 - cos(2*t/(2*k + 1)));
	c1[2] = -sin(2*t/(2*k + 1));
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	if (DisplayKnot(c1, ((DWORD)(i / ColourFactor) % threshold), (int)KnotWidth, i, 6) < 0)
	    break;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesKnots();
    return 0;
    }

/**************************************************************************
	Chrysanthemum Curve
	Written by Paul Bourke Attributed to Temple Fay
	http://paulbourke.net/geometry/chrysanthemum/
***************************************************************************/

/*
int	DoChrysanthemumCurve(void)

    {
    int		i, j;
    double	c1[6], KnotWidth, ColourFactor, u, r, p4, p8;

    c1[0] = c1[3] = param[10];	// x
    c1[1] = c1[4] = param[11];	// y
    c1[2] = c1[5] = param[12];	// z

    KnotWidth = param[0];
    ColourFactor = param[1];
    totpasses = 10;

    InitKnot(c1, 3);							// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	curpass = (int)(i * totpasses);
	u = i * 21.0 * PI / iterations;
	p4 = sin(17 * u / 3);
	p8 = sin(2 * cos(3 * u) - 28 * u);
	r = 5*(1 + sin(11*u/5)) - 4*p4*p4*p4*p4 * p8*p8*p8*p8*p8*p8*p8*p8;
	c1[0] = r * cos(u);
	c1[1] = r * sin(u);
	c1[2] = (r/20+0.2)*sin(r*2*PI/7);
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	if (DisplayKnot(c1, ((DWORD)(i / ColourFactor) % threshold), (int)KnotWidth, i, 6) < 0)
	    break;
	}
    PlotExtras();
    CloseZvaluesKnots();
    return 0;
    }
*/

/**************************************************************************
	Decorative Knot Patterns - Compound Pattern 3
	Prof. Y. F. Chen 2011
	History of Knot Theory / Definition of Knot Theory & Knots
***************************************************************************/

int	DoDecorativeKnotPatternsCompoundPattern3(void)

    {
    int		i, j;
    double	c1[6], t, gap, Start, End, KnotWidth, ColourFactor;

    c1[0] = c1[3] = param[10];	// x
    c1[1] = c1[4] = param[11];	// y
    c1[2] = c1[5] = param[12];	// z

    Start = param[0];
    End = param[1];
    KnotWidth = param[2];
    ColourFactor = param[3];
    totpasses = 10;

    gap = (End - Start) / iterations;
    t = Start;
    InitKnot(c1, 3);							// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	curpass = (int)(i * totpasses);
	c1[0] = cos(t) + 0.2 * cos(3 * t) + 0.98 * cos(-5 * t);
	c1[1] = 1.875 * sin(t) + 0.665 * sin(3 * t) + 0.45 * sin(-5 * t);
	c1[2] = 0.35 * sin(8 * t) + 0.15 * sin(2 * t);
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	if (DisplayKnot(c1, ((DWORD)(i / ColourFactor) % threshold), (int)KnotWidth, i, 6) < 0)
	    break;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesKnots();
    return 0;
    }

/**************************************************************************
	Decorative Knot Patterns - Four Main Loops
	Prof. Y. F. Chen 2011
	History of Knot Theory / Definition of Knot Theory & Knots
***************************************************************************/

int	DoDecorativeKnotPatternsFourMainLoops(void)

    {
    int		i, j;
    double	c1[6], t, gap, Start, End, KnotWidth, ColourFactor;

    c1[0] = c1[3] = param[10];	// x
    c1[1] = c1[4] = param[11];	// y
    c1[2] = c1[5] = param[12];	// z

    Start = param[0];
    End = param[1];
    KnotWidth = param[2];
    ColourFactor = param[3];
    totpasses = 10;

    gap = (End - Start) / iterations;
    t = Start;
    InitKnot(c1, 3);							// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	curpass = (int)(i * totpasses);
	c1[0] = 0.6 * cos(t) + 0.25 * cos(-3 * t) - 0.26 * cos(-9 * t);
	c1[1] = 0.6 * sin(t) + 0.25 * sin(-3 * t) - 0.26 * sin(-9 * t);
	c1[2] = 0.12 * sin(16 * t) + 0.06 * sin(4 * t);
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	if (DisplayKnot(c1, ((DWORD)(i / ColourFactor) % threshold), (int)KnotWidth, i, 6) < 0)
	    break;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesKnots();
    return 0;
    }

/**************************************************************************
	Decorative Knot Patterns - Inner Loop Pattern 3
	Prof. Y. F. Chen 2011
	History of Knot Theory / Definition of Knot Theory & Knots
***************************************************************************/

int	DoDecorativeKnotPatternsInnerLoopPattern3(void)

    {
    int		i, j;
    double	c1[6], t, gap, Start, End, KnotWidth, ColourFactor;

    c1[0] = c1[3] = param[10];	// x
    c1[1] = c1[4] = param[11];	// y
    c1[2] = c1[5] = param[12];	// z

    Start = param[0];
    End = param[1];
    KnotWidth = param[2];
    ColourFactor = param[3];
    totpasses = 10;

    gap = (End - Start) / iterations;
    t = Start;
    InitKnot(c1, 3);							// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	curpass = (int)(i * totpasses);
	c1[0] = cos(4*t) + 0.875 * cos(7 * t);
	c1[1] = sin(4*t) + 0.875 * sin(7 * t);
	c1[2] = 0.35 * sin(12 * t) - 0.15 * sin(3 * t);
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	if (DisplayKnot(c1, ((DWORD)(i / ColourFactor) % threshold), (int)KnotWidth, i, 6) < 0)
	    break;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesKnots();
    return 0;
    }

/**************************************************************************
	Decorative Knot Patterns - Inner Loop Pattern 5
	Prof. Y. F. Chen 2011
	History of Knot Theory / Definition of Knot Theory & Knots
***************************************************************************/

int	DoDecorativeKnotPatternsInnerLoopPattern5(void)

    {
    int		i, j;
    double	c1[6], t, gap, Start, End, KnotWidth, ColourFactor;

    c1[0] = c1[3] = param[10];	// x
    c1[1] = c1[4] = param[11];	// y
    c1[2] = c1[5] = param[12];	// z

    Start = param[0];
    End = param[1];
    KnotWidth = param[2];
    ColourFactor = param[3];
    totpasses = 10;

    gap = (End - Start) / iterations;
    t = Start;
    InitKnot(c1, 3);							// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	curpass = (int)(i * totpasses);
	c1[0] = cos(2*t) + 0.67 * cos(7 * t);
	c1[1] = sin(2*t) + 0.67 * sin(7 * t);
	c1[2] = 0.2 * sin(10 * t) - 0.1 * sin(5 * t);
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	if (DisplayKnot(c1, ((DWORD)(i / ColourFactor) % threshold), (int)KnotWidth, i, 6) < 0)
	    break;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesKnots();
    return 0;
    }

/**************************************************************************
	Decorative Knot Patterns - Inner Loop Pattern 6
	Prof. Y. F. Chen 2011
	History of Knot Theory / Definition of Knot Theory & Knots
***************************************************************************/

int	DoDecorativeKnotPatternsInnerLoopPattern6(void)

    {
    int		i, j;
    double	c1[6], t, gap, Start, End, KnotWidth, ColourFactor;

    c1[0] = c1[3] = param[10];	// x
    c1[1] = c1[4] = param[11];	// y
    c1[2] = c1[5] = param[12];	// z

    Start = param[0];
    End = param[1];
    KnotWidth = param[2];
    ColourFactor = param[3];
    totpasses = 10;

    gap = (End - Start) / iterations;
    t = Start;
    InitKnot(c1, 3);							// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	curpass = (int)(i * totpasses);
	c1[0] = 1.5 * cos(t) + 0.35 * cos(3 * t) - 0.9 * cos(5 * t);
	c1[1] = 1.5 * sin(t) + 0.35 * sin(3 * t) - 0.9 * sin(5 * t);
	c1[2] = 0.35 * sin(8 * t) - 0.2 * sin(2 * t);
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	if (DisplayKnot(c1, ((DWORD)(i / ColourFactor) % threshold), (int)KnotWidth, i, 6) < 0)
	    break;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesKnots();
    return 0;
    }

/**************************************************************************
	Decorative Knot Patterns - Three Main Loops
	Prof. Y. F. Chen 2011
	History of Knot Theory / Definition of Knot Theory & Knots
***************************************************************************/

int	DoDecorativeKnotPatternsThreeMainLoops(void)

    {
    int		i, j;
    double	c1[6], t, gap, Start, End, KnotWidth, ColourFactor;

    c1[0] = c1[3] = param[10];	// x
    c1[1] = c1[4] = param[11];	// y
    c1[2] = c1[5] = param[12];	// z

    Start = param[0];
    End = param[1];
    KnotWidth = param[2];
    ColourFactor = param[3];
    totpasses = 10;

    gap = (End - Start) / iterations;
    t = Start;
    InitKnot(c1, 3);							// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	curpass = (int)(i * totpasses);
	c1[0] = 0.59 * cos(t) + 0.3 * cos(-2 * t) - 0.45 * cos(-5 * t);
	c1[1] = 0.59 * sin(t) + 0.3 * sin(-2 * t) - 0.45 * sin(-5 * t);
	c1[2] = 0.1 * sin(9 * t) + 0.25 * sin(6 * t);
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	if (DisplayKnot(c1, ((DWORD)(i / ColourFactor) % threshold), (int)KnotWidth, i, 6) < 0)
	    break;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesKnots();
    return 0;
    }

/**************************************************************************
	Eight Knot
	Meier 2001
	http://www.3d-meier.de/ (Representation of knots 2:5)
***************************************************************************/

int	DoEightKnot(void)

    {
    int		i, j;
    double	c1[6], t, gap, Start, End, KnotWidth, ColourFactor;

    c1[0] = c1[3] = param[10];	// x
    c1[1] = c1[4] = param[11];	// y
    c1[2] = c1[5] = param[12];	// z

    Start = param[0];
    End = param[1];
    KnotWidth = param[2];
    ColourFactor = param[3];
    totpasses = 10;

    gap = (End - Start) / iterations;
    t = Start;
    InitKnot(c1, 3);							// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	curpass = (int)(i * totpasses);
	c1[0] = 10*cos(t) + 10*cos(3*t);
	c1[1] = 6*sin(t) + 10*sin(3*t);
	c1[2] = 4*sin(3*t) - 10*sin(6*t);
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	if (DisplayKnot(c1, ((DWORD)(i / ColourFactor) % threshold), (int)KnotWidth, i, 6) < 0)
	    break;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesKnots();
    return 0;
    }

/**************************************************************************
	Fibonacci Fourier Knots
	Louis H. Kauffman 3 Dec 1997
	https://arxiv.org/pdf/q-alg/9711013.pdf
***************************************************************************/

int	DoFibonacciFourierKnots(void)

    {
    int		i, j;
    double	c1[6], t, gap, Start, End, KnotWidth, ColourFactor, n, fn, fn1, fn2, sqrt5;

    c1[0] = c1[3] = param[10];	// x
    c1[1] = c1[4] = param[11];	// y
    c1[2] = c1[5] = param[12];	// z

    Start = param[0];
    End = param[1];
    n = param[2];
    KnotWidth = param[3];
    ColourFactor = param[4];
    totpasses = 10;

    gap = (End - Start) / iterations;
    t = Start;
    sqrt5 = sqrt(5.0);
    fn = (pow((1 + sqrt5), n) - pow((1 - sqrt5), n)) / (pow(2, n)) / sqrt5;
    fn1 = (pow((1 + sqrt5), (n + 1)) - pow((1 - sqrt5), (n + 1))) / pow(2, (n + 1)) / sqrt5;
    fn2 = (pow((1 + sqrt5), (n + 2)) - pow((1 - sqrt5), (n + 2))) / pow(2, (n + 2)) / sqrt5;
    InitKnot(c1, 3);							// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	curpass = (int)(i * totpasses);

	c1[0] = cos(fn * t);
	c1[1] = sin(fn1 * t + 0.5);
	c1[2] = 0.5 * cos(fn2 * t + 0.5) + 0.5 * sin(fn1 * t + 0.5);
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	if (DisplayKnot(c1, ((DWORD)(i / ColourFactor) % threshold), (int)KnotWidth, i, 6) < 0)
	    break;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesKnots();
    return 0;
    }

/**************************************************************************
	Fourier Knot
	Louis H. Kauffman 3 Dec 1997
	https://arxiv.org/pdf/q-alg/9711013.pdf
***************************************************************************/

int	DoFourierKnot(int variety)

    {
    int		i, j;
    double	c1[6], t, gap, Start, End, KnotWidth, ColourFactor;

    c1[0] = c1[3] = param[10];	// x
    c1[1] = c1[4] = param[11];	// y
    c1[2] = c1[5] = param[12];	// z

    Start = param[0];
    End = param[1];
    KnotWidth = param[2];
    ColourFactor = param[3];
    totpasses = 10;

    gap = (End - Start) / iterations;
    t = Start;
    InitKnot(c1, 3);							// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	curpass = (int)(i * totpasses);

	if (variety == 0)
	    c1[0] = cos(2 * t);
	else
	    c1[0] = sin(2 * t);
	c1[1] = cos(3 * t + 0.5);
	c1[2] = 0.5 * cos(5 * t + 0.5) + 0.5 * sin(3 * t + 0.5);
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	if (DisplayKnot(c1, ((DWORD)(i / ColourFactor) % threshold), (int)KnotWidth, i, 6) < 0)
	    break;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesKnots();
    return 0;
    }

int	DoFourierKnotI()
    {
    return (DoFourierKnot(0));
    }

int	DoFourierKnotIII()
    {
    return (DoFourierKnot(1));
    }


/**************************************************************************
	Granny Knot
	Meier 2001
	http://www.3d-meier.de/ (Representation of knots 2.3)
***************************************************************************/

int	DoGrannyKnot(void)

    {
    int		i, j;
    double	c1[6], t, gap, Start, End, KnotWidth, ColourFactor;

    c1[0] = c1[3] = param[10];	// x
    c1[1] = c1[4] = param[11];	// y
    c1[2] = c1[5] = param[12];	// z

    Start = param[0];
    End = param[1];
    KnotWidth = param[2];
    ColourFactor = param[3];
    totpasses = 10;

    gap = (End - Start) / iterations;
    t = Start;
    InitKnot(c1, 6);							// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	curpass = (int)(i * totpasses);
	c1[0] = -22*cos(t) - 128*sin(t) - 44*cos(3*t) - 78*sin(3*t);
	c1[1] = -10*cos(2*t) - 27*sin(2*t) + 38*cos(4*t) + 46*sin(4*t);
	c1[2] =  70*cos(3*t) - 40*sin(3*t);
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	if (DisplayKnot(c1, ((DWORD)(i / ColourFactor) % threshold), (int)KnotWidth, i, 6) < 0)
	    break;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesKnots();
    return 0;
    }

/**************************************************************************
	Knot 3-1
	Meier 2001
	http://www.3d-meier.de/ (Representation of knots 2.6)
***************************************************************************/

int	DoKnot3_1(void)

    {
    int		i, j;
    double	c1[6], t, gap, Start, End, KnotWidth, ColourFactor;

    c1[0] = c1[3] = param[10];	// x
    c1[1] = c1[4] = param[11];	// y
    c1[2] = c1[5] = param[12];	// z

    Start = param[0];
    End = param[1];
    KnotWidth = param[2];
    ColourFactor = param[3];
    totpasses = 10;

    gap = (End - Start) / iterations;
    t = Start;
    InitKnot(c1, 6);							// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	curpass = (int)(i * totpasses);
	c1[0] = 41*cos(t) - 18*sin(t) - 83*cos(2*t) - 83*sin(2*t) - 11*cos(3*t) + 27*sin(3*t);
	c1[1] = 36*cos(t) + 27*sin(t) - 113*cos(2*t) + 30*sin(2*t) + 11*cos(3*t) - 27*sin(3*t);
	c1[2] = 45*sin(t) - 30*cos(2*t) + 113*sin(2*t) - 11*cos(3*t) + 27*sin(3*t);
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	if (DisplayKnot(c1, ((DWORD)(i / ColourFactor) % threshold), (int)KnotWidth, i, 6) < 0)
	    break;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesKnots();
    return 0;
    }

/**************************************************************************
	Knot 4-1
	Meier 2001
	http://www.3d-meier.de/ (Representation of knots 2.7)
***************************************************************************/

int	DoKnot4_1(void)

    {
    int		i, j;
    double	c1[6], t, gap, Start, End, KnotWidth, ColourFactor;

    c1[0] = c1[3] = param[10];	// x
    c1[1] = c1[4] = param[11];	// y
    c1[2] = c1[5] = param[12];	// z

    Start = param[0];
    End = param[1];
    KnotWidth = param[2];
    ColourFactor = param[3];
    totpasses = 10;

    gap = (End - Start) / iterations;
    t = Start;
    InitKnot(c1, 6);							// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	curpass = (int)(i * totpasses);
	c1[0] = 32*cos(t) - 51*sin(t) - 104*cos(2*t) - 34*sin(2*t) + 104*cos(3*t) - 91*sin(3*t);
	c1[1] = 94*cos(t) + 41*sin(t) + 113*cos(2*t) - 68*cos(3*t) - 124*sin(3*t);
	c1[2] = 16*cos(t) + 73*sin(t) - 211*cos(2*t) - 39*sin(2*t) - 99*cos(3*t) - 21*sin(3*t);
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	if (DisplayKnot(c1, ((DWORD)(i / ColourFactor) % threshold), (int)KnotWidth, i, 6) < 0)
	    break;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesKnots();
    return 0;
    }

/**************************************************************************
	Knot 5-1
	Meier 2001
	http://www.3d-meier.de/ (Representation of knots 2.8)
***************************************************************************/

int	DoKnot5_1(void)

    {
    int		i, j;
    double	c1[6], t, gap, Start, End, KnotWidth, ColourFactor;

    c1[0] = c1[3] = param[10];	// x
    c1[1] = c1[4] = param[11];	// y
    c1[2] = c1[5] = param[12];	// z

    Start = param[0];
    End = param[1];
    KnotWidth = param[2];
    ColourFactor = param[3];
    totpasses = 10;

    gap = (End - Start) / iterations;
    t = Start;
    InitKnot(c1, 6);							// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	curpass = (int)(i * totpasses);
	c1[0] = 88*cos(t) + 115*sin(t) - 475*cos(2*t) - 127*sin(2*t) - 87*cos(3*t) + 36*sin(3*t) + 11*cos(4*t) - 19*sin(4*t);
	c1[1] = 89*cos(t) - 32*sin(t) - 172*cos(2*t) + 294*sin(2*t) + 76*cos(3*t) + 102*sin(3*t) - 61*cos(4*t) + 113*sin(4*t);
	c1[2] = 44*cos(t) - 69*sin(t) + 34*cos(2*t) + 223*sin(2*t) + 16*cos(3*t)+ 120*sin(3*t) + 42*cos(4*t) - 125*sin(4*t);
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	if (DisplayKnot(c1, ((DWORD)(i / ColourFactor) % threshold), (int)KnotWidth, i, 6) < 0)
	    break;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesKnots();
    return 0;
    }

/**************************************************************************
	Knot 5-2
	Meier 2001
	http://www.3d-meier.de/ (Representation of knots 2.9)
***************************************************************************/

int	DoKnot5_2(void)

    {
    int		i, j;
    double	c1[6], t, gap, Start, End, KnotWidth, ColourFactor;

    c1[0] = c1[3] = param[10];	// x
    c1[1] = c1[4] = param[11];	// y
    c1[2] = c1[5] = param[12];	// z

    Start = param[0];
    End = param[1];
    KnotWidth = param[2];
    ColourFactor = param[3];
    totpasses = 10;

    gap = (End - Start) / iterations;
    t = Start;
    InitKnot(c1, 6);							// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	curpass = (int)(i * totpasses);
	c1[0] = -33*cos(t) + 43*sin(t) + 214*sin(2*t) - 101*cos(3*t) - 47*sin(3*t) + 11*sin(4*t);
	c1[1] = -57*cos(t) + 99*sin(t) - 54*cos(2*t) - 159*sin(2*t) - 117*cos(3*t) - 5*sin(3*t) - 31*cos(4*t) - 45*sin(4*t);
	c1[2] = 34*cos(t) - 21*sin(t) - 100*cos(2*t) - 93*sin(2*t) - 27*cos(3*t) - 16*sin(3*t) + 52*cos(4*t) + 84*sin(4*t);
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	if (DisplayKnot(c1, ((DWORD)(i / ColourFactor) % threshold), (int)KnotWidth, i, 6) < 0)
	    break;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesKnots();
    return 0;
    }

/**************************************************************************
	Knot 6-1
	Meier 2001
	http://www.3d-meier.de/ (Representation of knots 2.10)
***************************************************************************/

int	DoKnot6_1(void)

    {
    int		i, j;
    double	c1[6], t, gap, Start, End, e, KnotWidth, ColourFactor;

    c1[0] = c1[3] = param[10];	// x
    c1[1] = c1[4] = param[11];	// y
    c1[2] = c1[5] = param[12];	// z

    Start = param[0];
    End = param[1];
    KnotWidth = param[2];
    ColourFactor = param[3];
    totpasses = 10;

    e = atan(1.0);

    gap = (End - Start) / iterations;
    t = Start;
    InitKnot(c1, 6);							// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	curpass = (int)(i * totpasses);
	c1[0] = 12*cos(t*e) + 20*sin(t) - 163*cos(2*t) + 76*sin(2*t) - 87*cos(3*t) - 15*sin(3*t) - 21*cos(4*t) + 14*sin(4*t) + 24*cos(5*t) - 50*sin(5*t);
	c1[1] = 29*cos(t*e) + 78*sin(t) - 180*cos(2*t) + 58*sin(2*t) + 88*cos(3*t) + 72*sin(3*t) - 14*sin(4*t);
	c1[2] = -30*cos(t*e) - 78*sin(t) - 111*cos(2*t) + 37*sin(2*t) - 67*cos(3*t) - 51*sin(3*t) + 31*cos(4*t) + 8*sin(4*t) -11*cos(5*t) + 65*sin(5*t);
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	if (DisplayKnot(c1, ((DWORD)(i / ColourFactor) % threshold), (int)KnotWidth, i, 6) < 0)
	    break;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesKnots();
    return 0;
    }

/**************************************************************************
	Knot 6-2
	Meier 2001
	http://www.3d-meier.de/ (Representation of knots 2.11)
***************************************************************************/

int	DoKnot6_2(void)

    {
    int		i, j;
    double	c1[6], t, gap, Start, End, KnotWidth, ColourFactor;

    c1[0] = c1[3] = param[10];	// x
    c1[1] = c1[4] = param[11];	// y
    c1[2] = c1[5] = param[12];	// z

    Start = param[0];
    End = param[1];
    KnotWidth = param[2];
    ColourFactor = param[3];
    totpasses = 10;

    gap = (End - Start) / iterations;
    t = Start;
    InitKnot(c1, 6);							// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	curpass = (int)(i * totpasses);
	c1[0] = -6*cos(t) - 21*sin(t) - 195*cos(2*t) + 92*sin(2*t) - 64*cos(3*t) - 23*sin(3*t) - 6*cos(4*t) + 13*sin(4*t) +24*cos(5*t) + 15*sin(5*t) + 41*sin(6*t);
	c1[1] = -21*cos(t) - 24*sin(t) - 207*cos(2*t) - 72*sin(2*t) + 112*cos(3*t) - 7*sin(3*t) - 13*cos(4*t) - 40*sin(4*t) -27*cos(5*t) - 3*sin(5*t) - 17*cos(6*t);
	c1[2] = -18*cos(t) - 13*sin(t) + 113*cos(2*t) - 107*sin(2*t) + 86*cos(3*t) - 9*sin(3*t) - 26*cos(4*t) - 7*sin(4*t) +24*cos(5*t) + 33*sin(5*t) + 21*cos(6*t) + 31*sin(6*t);
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	if (DisplayKnot(c1, ((DWORD)(i / ColourFactor) % threshold), (int)KnotWidth, i, 6) < 0)
	    break;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesKnots();
    return 0;
    }

/**************************************************************************
	Knot 6-3
	Meier 2001
	http://www.3d-meier.de/ (Representation of knots 2.12)
***************************************************************************/

int	DoKnot6_3(void)

    {
    int		i, j;
    double	c1[6], t, gap, Start, End, KnotWidth, ColourFactor;

    c1[0] = c1[3] = param[10];	// x
    c1[1] = c1[4] = param[11];	// y
    c1[2] = c1[5] = param[12];	// z

    Start = param[0];
    End = param[1];
    KnotWidth = param[2];
    ColourFactor = param[3];
    totpasses = 10;

    gap = (End - Start) / iterations;
    t = Start;
    InitKnot(c1, 6);							// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	curpass = (int)(i * totpasses);
	c1[0] = -40*cos(t) + 32*sin(t) + 69*cos(2*t) - 12*sin(2*t) + 120*cos(3*t) - 52*sin(3*t) - 56*cos(4*t) + 46*sin(4*t) -17*sin(5*t) + 14*cos(6*t) + 19*sin(6*t);
	c1[1] = 90*cos(t) + 89*sin(t) - 142*cos(2*t) + 147*sin(2*t) + 74*cos(3*t) + 85*sin(3*t) - 56*sin(4*t) + 23*cos(5*t) +16*cos(6*t) + 7*sin(6*t);
	c1[2] = 52*cos(t) + 64*sin(t) + 53*cos(2*t) + 35*sin(2*t) + 77*cos(3*t) - 87*sin(3*t) + 101*cos(4*t) - 19*sin(4*t) -5*cos(5*t) + 2*sin(5*t) + 3*cos(6*t) + 9*sin(6*t);
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	if (DisplayKnot(c1, ((DWORD)(i / ColourFactor) % threshold), (int)KnotWidth, i, 6) < 0)
	    break;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesKnots();
    return 0;
    }

/**************************************************************************
	Knot 7-1
	Meier 2001
	http://www.3d-meier.de/ (Representation of knots 2.13)
***************************************************************************/

int	DoKnot7_1(void)

    {
    int		i, j;
    double	c1[6], t, gap, Start, End, KnotWidth, ColourFactor;

    c1[0] = c1[3] = param[10];	// x
    c1[1] = c1[4] = param[11];	// y
    c1[2] = c1[5] = param[12];	// z

    Start = param[0];
    End = param[1];
    KnotWidth = param[2];
    ColourFactor = param[3];
    totpasses = 10;

    gap = (End - Start) / iterations;
    t = Start;
    InitKnot(c1, 6);							// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	curpass = (int)(i * totpasses);
	c1[0] = 54*cos(t) - 4*sin(t) - 239*cos(2*t) - 44*sin(2*t) - 45*cos(3*t) - 17*sin(3*t) - 33*cos(4*t) - 27*sin(4*t) +19*cos(5*t) + 19*sin(5*t) - 8*cos(6*t) + 15*sin(6*t);
	c1[1] = - 16*cos(t) - 43*sin(t) + 11*cos(2*t) + 170*sin(2*t) - 22*cos(3*t) + 41*sin(3*t) + 21*cos(4*t) + 2*cos(5*t) -12*sin(5*t) - 43*cos(6*t) + 26*sin(6*t);
	c1[2] = - 55*sin(t) - 78*cos(2*t) + 189*sin(2*t) - 38*cos(3*t) + 41*sin(3*t) + 10*cos(4*t) - 16*sin(4*t) - 38*cos(5*t) +18*sin(5*t) + 22*cos(6*t) - 15*sin(6*t);
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	if (DisplayKnot(c1, ((DWORD)(i / ColourFactor) % threshold), (int)KnotWidth, i, 6) < 0)
	    break;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesKnots();
    return 0;
    }

/**************************************************************************
	Knot 7-2
	Meier 2001
	http://www.3d-meier.de/ (Representation of knots 2.14)
***************************************************************************/

int	DoKnot7_2(void)

    {
    int		i, j;
    double	c1[6], t, gap, Start, End, KnotWidth, ColourFactor;

    c1[0] = c1[3] = param[10];	// x
    c1[1] = c1[4] = param[11];	// y
    c1[2] = c1[5] = param[12];	// z

    Start = param[0];
    End = param[1];
    KnotWidth = param[2];
    ColourFactor = param[3];
    totpasses = 10;

    gap = (End - Start) / iterations;
    t = Start;
    InitKnot(c1, 6);							// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	curpass = (int)(i * totpasses);
	c1[0] = 10*cos(t) + 115*sin(t) - 184*cos(2*t) + 10*sin(2*t) + 101*sin(3*t) + 23*cos(4*t) + 55*sin(4*t) 
							-38*cos(5*t) -6*sin(5*t) - 14*cos(6*t) + 8*sin(6*t) + 16*cos(6*t) + 14*sin(6*t);
	c1[1] = 42*cos(t) - 104*sin(t) - 252*cos(2*t) + 47*sin(2*t) - 21*cos(3*t) - 65*sin(3*t) - 23*cos(4*t) 
							- 23*sin(4*t) +36*cos(5*t) - 10*sin(5*t) - 13*cos(6*t) + 2*sin(6*t) - 18*cos(7*t) - 9*sin(7*t);
	c1[2] = 30*sin(t) + 20*cos(2*t) + 19*sin(2*t) + 6*cos(3*t) -31*sin(3*t) - 4*cos(4*t) - 24*sin(4*t) 
							- 44*cos(5*t) - 50*sin(5*t)+ 31*cos(6*t) +39*sin(6*t) - 16*cos(7*t) + 23*sin(7*t);
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	if (DisplayKnot(c1, ((DWORD)(i / ColourFactor) % threshold), (int)KnotWidth, i, 6) < 0)
	    break;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesKnots();
    return 0;
    }

/**************************************************************************
	Knot 7-3
	Meier 2001
	http://www.3d-meier.de/ (Representation of knots 2.15)
***************************************************************************/

int	DoKnot7_3(void)

    {
    int		i, j;
    double	c1[6], t, gap, Start, End, KnotWidth, ColourFactor;

    c1[0] = c1[3] = param[10];	// x
    c1[1] = c1[4] = param[11];	// y
    c1[2] = c1[5] = param[12];	// z

    Start = param[0];
    End = param[1];
    KnotWidth = param[2];
    ColourFactor = param[3];
    totpasses = 10;

    gap = (End - Start) / iterations;
    t = Start;
    InitKnot(c1, 6);							// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	curpass = (int)(i * totpasses);
	c1[0] = -14*cos(t) - 19*sin(t) - 53*cos(2*t) - 120*sin(2*t) + 71*cos(3*t) - 50*sin(3*t) - 47*cos(4*t) - 86*sin(4*t) +9*cos(5*t) - 5*sin(5*t) + 12*cos(6*t) + 22*sin(6*t);
	c1[1] = -11*cos(t) + 13*sin(t) - 294*cos(2*t) + 112*sin(2*t) + 29*cos(3*t) - 22*sin(3*t) + 4*cos(4*t) + 11*sin(4*t) -6*cos(5*t) - 14*cos(6*t) + 18*sin(6*t);
	c1[2] = 28*cos(t) + 9*sin(t) - 39*cos(2*t) - 60*sin(2*t) - 60*cos(3*t) + 21*sin(3*t) + 50*cos(4*t) - 11*sin(4*t) +44*cos(5*t) - 63*sin(5*t) + 14*cos(6*t) + 9*sin(6*t);
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	if (DisplayKnot(c1, ((DWORD)(i / ColourFactor) % threshold), (int)KnotWidth, i, 6) < 0)
	    break;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesKnots();
    return 0;
    }

/**************************************************************************
	Knot 7-4
	Meier 2001
	http://www.3d-meier.de/ (Representation of knots 2.16)
***************************************************************************/

int	DoKnot7_4(void)

    {
    int		i, j;
    double	c1[6], t, gap, Start, End, KnotWidth, ColourFactor;

    c1[0] = c1[3] = param[10];	// x
    c1[1] = c1[4] = param[11];	// y
    c1[2] = c1[5] = param[12];	// z

    Start = param[0];
    End = param[1];
    KnotWidth = param[2];
    ColourFactor = param[3];
    totpasses = 10;

    gap = (End - Start) / iterations;
    t = Start;
    InitKnot(c1, 6);							// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	curpass = (int)(i * totpasses);
	c1[0] = 32*cos(t) + 10*sin(t) - 12*cos(2*t) + 10*sin(2*t) + 48*cos(3*t) + 75*sin(3*t) - 32*cos(4*t) + 12*sin(4*t) +3*cos(5*t) - 8*sin(5*t) - 2*cos(6*t) - 4*sin(6*t);
	c1[1] = -16*cos(t) - 10*sin(t) - 90*cos(2*t) + 129*sin(2*t) - 6*cos(3*t) - 8*sin(3*t) - 19*cos(4*t) - 6*sin(4*t) - 8*cos(5*t)+ 6*sin(5*t) + 4*cos(6*t) + 13*sin(6*t);
	c1[2] = -12*cos(t) + sin(t) - 55*cos(2*t) + 32*sin(2*t) - 10*cos(3*t) - 13*sin(3*t) + 2*cos(4*t) + 26*sin(4*t) +17*cos(5*t) + 9*sin(5*t) + 15*cos(6*t) - 21*sin(6*t);
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	if (DisplayKnot(c1, ((DWORD)(i / ColourFactor) % threshold), (int)KnotWidth, i, 6) < 0)
	    break;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesKnots();
    return 0;
    }

/**************************************************************************
	Knot 7-5
	Meier 2001
	http://www.3d-meier.de/ (Representation of knots 2.17)
***************************************************************************/

int	DoKnot7_5(void)

    {
    int		i, j;
    double	c1[6], t, gap, Start, End, KnotWidth, ColourFactor;

    c1[0] = c1[3] = param[10];	// x
    c1[1] = c1[4] = param[11];	// y
    c1[2] = c1[5] = param[12];	// z

    Start = param[0];
    End = param[1];
    KnotWidth = param[2];
    ColourFactor = param[3];
    totpasses = 10;

    gap = (End - Start) / iterations;
    t = Start;
    InitKnot(c1, 6);							// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	curpass = (int)(i * totpasses);
	c1[0] = 5*cos(t) + 5*sin(t) - 163*cos(2*t) - 80*sin(2*t) - 28*cos(3*t) + 15*sin(3*t) - 20*cos(4*t) + 13*sin(4*t) 
											-11*cos(5*t) - 12*sin(5*t) + 4*cos(6*t) - 19*sin(6*t) + 12*cos(7*t) + 2*sin(7*t);
	c1[1] = -31*cos(t) + 6*sin(t) - 113*cos(2*t) + 81*sin(2*t) + 87*cos(3*t) + 4*sin(3*t) + 5*cos(4*t) + 18*sin(4*t) 
											-4*cos(5*t) + 11*sin(5*t) - 17*cos(6*t) + 5*sin(6*t) - 6*cos(7*t) - 13*sin(7*t);
	c1[2] = -11*cos(t) - 3*sin(t) + 21*cos(2*t) + 31*sin(2*t) + 8*cos(3*t) + 2*sin(3*t) + 2*cos(4*t) + 13*sin(4*t) 
											+27*cos(5*t) + 7*sin(5*t) - 8*cos(6*t) - 29*sin(6*t) + 11*cos(7*t) - 8*sin(7*t);
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	if (DisplayKnot(c1, ((DWORD)(i / ColourFactor) % threshold), (int)KnotWidth, i, 6) < 0)
	    break;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesKnots();
    return 0;
    }

/**************************************************************************
	Knot 7-6
	Meier 2001
	http://www.3d-meier.de/ (Representation of knots 2.18)
***************************************************************************/

int	DoKnot7_6(void)

    {
    int		i, j;
    double	c1[6], t, gap, Start, End, KnotWidth, ColourFactor;

    c1[0] = c1[3] = param[10];	// x
    c1[1] = c1[4] = param[11];	// y
    c1[2] = c1[5] = param[12];	// z

    Start = param[0];
    End = param[1];
    KnotWidth = param[2];
    ColourFactor = param[3];
    totpasses = 10;

    gap = (End - Start) / iterations;
    t = Start;
    InitKnot(c1, 6);							// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	curpass = (int)(i * totpasses);
	c1[0] = -17*cos(t) + 80*sin(t) - 70*cos(2*t) - 190*sin(2*t) - 26*cos(3*t) - 145*sin(3*t) - 62*cos(4*t) - 31*sin(4*t) +26*cos(5*t) 
												+ 13*sin(5*t) - 2*cos(6*t) + 8*sin(6*t) - 10*cos(7*t) + 9*sin(7*t);
	c1[1] = 102*cos(t) - 151*sin(t) - 36*cos(2*t) - 25*sin(2*t) + 200*cos(3*t) - 18*sin(3*t) -30*cos(4*t) + 28*sin(4*t) -7*cos(5*t) 
												+ 2*sin(5*t) + 13*cos(6*t) - 11*sin(6*t) - 5*cos(7*t) - sin(7*t);
	c1[2] = 58*cos(t) - 17*sin(t) - 5*cos(2*t) - 27*sin(2*t) + 66*cos(3*t) - 38*sin(3*t) + 28*cos(4*t) - 80*sin(4*t) - 35*cos(5*t)
												+ 28*sin(5*t) - 26*cos(6*t) - 34*sin(6*t) + 32*cos(7*t) + sin(7*t);
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	if (DisplayKnot(c1, ((DWORD)(i / ColourFactor) % threshold), (int)KnotWidth, i, 6) < 0)
	    break;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesKnots();
    return 0;
    }

/**************************************************************************
	Knot 7-7
	Meier 2001
	http://www.3d-meier.de/ (Representation of knots 2.19)
***************************************************************************/

int	DoKnot7_7(void)

    {
    int		i, j;
    double	c1[6], t, gap, Start, End, KnotWidth, ColourFactor;

    c1[0] = c1[3] = param[10];	// x
    c1[1] = c1[4] = param[11];	// y
    c1[2] = c1[5] = param[12];	// z

    Start = param[0];
    End = param[1];
    KnotWidth = param[2];
    ColourFactor = param[3];
    totpasses = 10;

    gap = (End - Start) / iterations;
    t = Start;
    InitKnot(c1, 6);							// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	curpass = (int)(i * totpasses);
	c1[0] = -5*cos(t) + 8*cos(2*t) + 83*sin(2*t) + 87*cos(3*t) + 100*sin(3*t) - 5*cos(4*t) + 22*sin(4*t) - 10*cos(5*t) 
									    +10*sin(5*t) - 2*cos(6*t) - 10*sin(6*t) + 5*cos(7*t) + 6*sin(7*t);
	c1[1] = 17*cos(t) + 21*sin(t) - 174*cos(2*t) + 13*sin(2*t) - 15*cos(3*t) + 3*sin(3*t) - 9*cos(4*t) + 46*sin(4*t) 
    									    +16*cos(5*t) - 25*sin(5*t) - 21*cos(6*t) + 7*sin(6*t) - 9*cos(7*t) - 3*sin(7*t);
	c1[2] = -28*cos(t) + 9*sin(t) + 110*cos(2*t) + 4*sin(2*t) + 11*cos(3*t) - 6*sin(3*t) - 46*cos(4*t) - 17*sin(4*t) 
									    +32*cos(5*t) - 9*sin(5*t) - 12*cos(6*t) - 9*sin(6*t) - 9*cos(7*t) + 18*sin(7*t);
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	if (DisplayKnot(c1, ((DWORD)(i / ColourFactor) % threshold), (int)KnotWidth, i, 6) < 0)
	    break;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesKnots();
    return 0;
    }

/**************************************************************************
	Knot Universes in Bianchi Type I Cosmology I
	Ratbay Myrzakulov 15 July 2012
	https://arxiv.org/abs/1204.1093
***************************************************************************/

int	DoKnotUniversesI(void)

    {
    int		i, j;
    double	c1[8], t, gap, Start, End, KnotWidth, ColourFactor;

    c1[0] = c1[4] = param[10];	// x
    c1[1] = c1[5] = param[11];	// y
    c1[2] = c1[6] = param[12];	// z
    c1[3] = c1[7] = param[12];	// z

    Start = param[0];
    End = param[1];
    KnotWidth = param[2];
    ColourFactor = param[3];
    totpasses = 10;

    gap = (End - Start) / iterations;
    t = Start;
    InitKnot(c1, 8);							// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	curpass = (int)(i * totpasses);
	c1[4] = sin(3 * t) * (2 + cos(3 * t)) * sin(2 * t);
	c1[5] = sin(3 * t) * (2 + cos(3 * t)) * cos(2 * t);
	c1[6] = sqr(2 + cos(3 * t)) * cos(2 * t) * sin(2 * t);
	c1[7] = sqr(2 + cos(3 * t)) * cos(2 * t) * sin(2 * t) * sin(3 * t);
	for (j = 0; j < 4; j++)
	    c1[j] += c1[j + 4] * dt;

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	if (DisplayKnot(c1, ((DWORD)(i / ColourFactor) % threshold), (int)KnotWidth, i, 8) < 0)
	    break;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesKnots();
    return 0;
    }

/**************************************************************************
	Knot Universes in Bianchi Type I Cosmology - III
	Ratbay Myrzakulov 15 July 2012
	https://arxiv.org/abs/1204.1093
***************************************************************************/

int	DoKnotUniversesICosmologyIII(void)

    {
    int		i, j;
    double	c1[8], t, gap, Start, End, KnotWidth, ColourFactor;

    c1[0] = c1[4] = param[10];	// x
    c1[1] = c1[5] = param[11];	// y
    c1[2] = c1[6] = param[12];	// z
    c1[3] = c1[7] = param[13];	// z

    Start = param[0];
    End = param[1];
    KnotWidth = param[2];
    ColourFactor = param[3];
    totpasses = 10;

    gap = (End - Start) / iterations;
    t = Start;
    InitKnot(c1, 8);							// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	curpass = (int)(i * totpasses);
	c1[4] = (-12*(SINSQR(3*t) + 36 * cos(3*t) + 18*(COSSQR(3*t)))) - 49*sin(2*t) * (26 / 49 + cos(3 * t)) * sin(3 * t);
	c1[5] = -18 * sin(2*t) * (COSSQR(3*t)) + (-49*sin(3*t) * cos(2*t) - 36*sin(2*t)) * cos(3*t) - 26*sin(3*t) * cos(2*t) + 12*(SINSQR(3*t)) * sin(2*t);
	c1[6] = -30 * sin(3*t) * (2 + COSSQR(3*t)) - 38*(sin(2*t) * cos(3*t) * cos(3*t) - 27.0/38.0*sin(3*t) * sin(3*t) + 58.0/19.0 * cos(3*t) + 40.0/19.0) * cos(2*t);
	c1[7] = 6 * COSSQR(2*t);
	for (j = 0; j < 4; j++)
	    c1[j] += c1[j + 4] * dt;

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	if (DisplayKnot(c1, ((DWORD)(i / ColourFactor) % threshold), (int)KnotWidth, i, 8) < 0)
	    break;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesKnots();
    return 0;
    }

/**************************************************************************
	Knot Universes in Bianchi Type I Cosmology - Tempestuous
	Ratbay Myrzakulov 15 July 2012
	https://arxiv.org/abs/1204.1093
***************************************************************************/

int	DoKnotUniversesITempestuous(void)

    {
    int		i, j;
    double	c1[8], t, gap, Start, End, KnotWidth, ColourFactor, s21, s22, s23, s31, s32, s33, c21, c22, c23, c31, c32, c33;

    c1[0] = c1[4] = param[10];	// x
    c1[1] = c1[5] = param[11];	// y
    c1[2] = c1[6] = param[12];	// z
    c1[3] = c1[7] = param[13];	// z

    Start = param[0];
    End = param[1];
    KnotWidth = param[2];
    ColourFactor = param[3];
    totpasses = 10;

    gap = (End - Start) / iterations;
    t = Start;
    InitKnot(c1, 8);							// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	curpass = (int)(i * totpasses);
	s21 = sin(2 * t); 
	s22 = s21 * s21; 
	s23 = s21 * s22;
	s31 = sin(3 * t); 
	s32 = s31 * s31; 
	s33 = s31 * s32;
	c21 = cos(2 * t); 
	c22 = c21 * c21; 
	c23 = c21 * c22;
	c31 = cos(3 * t); 
	c32 = c31 * c31; 
	c33 = c31 * c32;
	c1[4] = (-12 * s32 + 36 * c31 + 18 * c32) * c21 - 49 * s21 * (26 / 49 + c31) * s31;
	c1[5] = -18 * s21 * c32 + (-49 * s31 * c21 - 36 * s21) * c31 - 26 * s31 * c21 + 12 * s32 * s21;
	c1[6] = -30 * s31 * (2 + c31) * c22 - 38 * s21 * (c32 - 27 / 38 * s32 + 58 / 19 * c31 + 40 / 19) * c21 + 30 * s31 * s22 * (2 + c31);
	c1[7] = (6 * c22 - 6 * s22) * c33 + (24 * c22 - 22 * s31 * s21 * c21 - 24 * s22) * c32 
					+ ((6 * s32 - 24) * s22) * c31 - (12 * (c21 - 3 / 4 * s31 * s21)) * (s31 * c21 + 4 / 3 * s21) * s31;
	for (j = 0; j < 4; j++)
	    c1[j] += c1[j + 4] * dt;

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	if (DisplayKnot(c1, ((DWORD)(i / ColourFactor) % threshold), (int)KnotWidth, i, 8) < 0)
	    break;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesKnots();
    return 0;
    }

/**************************************************************************
	Lissajous 
	Meier 2001
	http://www.3d-meier.de/ (Representation of knots 5.1 - 5)
***************************************************************************/

int	DoLissajous(void)

    {
    int		i, j;
    double	c1[6], t, gap, Start, End, a1, a2, a3, k1, k2, k3, l1, l2, l3, KnotWidth, ColourFactor;

    l1 = param[10]; 		// use variable parameters for additional constants
    l2 = param[11]; 
    l3 = param[12]; 

    c1[0] = c1[3] = 0.1;	// x
    c1[1] = c1[4] = 0.2;	// y
    c1[2] = c1[5] = 0.3;	// z


    Start = param[0];
    End = param[1];
    a1 = param[2];
    a2 = param[3];
    a3 = param[4];
    k1 = param[5];
    k2 = param[6];
    k3 = param[7];
    KnotWidth = param[8];
    ColourFactor = param[9];
    totpasses = 10;

    gap = (End - Start) / iterations;
    t = Start;
    InitKnot(c1, 6);							// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	curpass = (int)(i * totpasses);
	c1[0] = a1 * cos(k1*t + l1);
	c1[1] = a2 * cos(k2*t + l2);
	c1[2] = a3 * cos(k3*t + l3);
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	if (DisplayKnot(c1, ((DWORD)(i / ColourFactor) % threshold), (int)KnotWidth, i, 6) < 0)
	    break;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesKnots();
    return 0;
    }

/**************************************************************************
	Marcus Torus Knot
	Marcus Rezende August 2018
***************************************************************************/

int	DoMarcusTorusKnot(void)

    {
    double	t, c1[6], R, r, p, q, gap, Start, End, ColourFactor, KnotWidth;
    int		i, j;

    c1[0] = c1[3] = param[10];	// x
    c1[1] = c1[4] = param[11];	// y
    c1[2] = c1[5] = param[12];	// z

    Start = param[0];
    End = param[1];
    R = param[2];
    r = param[3];
    p = param[4];
    q = param[5];
    KnotWidth = (int)param[6];
    ColourFactor = param[7];
    totpasses = 10;

    InitKnot(c1, 6);							// pass in number of dimensions
    gap = (End - Start) / iterations;
    t = Start;
    for (i = 0; i < iterations; i++)
	{
	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	c1[3] = (R + r * cos(p * t)) * cos(q * t);
	c1[4] = (R + r * cos(p * t)) * sin(q * t);
	c1[5] = R * cos(p * t);
	for (j = 0; j < 3; j++)
	    c1[j] += c1[j + 3] * dt;

	if (DisplayKnot(c1, ((DWORD)(i / ColourFactor) % threshold), (int)KnotWidth, i, 6) < 0)
//	if (DisplayOscillator(c1, cn, dt, ((DWORD)(t * ColourFactor) % threshold), t, 3, KnotWidth) < 0)
	    break;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesKnots();
    return 0;
    }

/**************************************************************************
	Paul Bourke's Knot 4
	Written by Paul Bourke October 1992 

	There are a whole family of curves including knots which are formed by the equations:
	    x = r * cos(phi) * cos(theta)
	    y = r * cos(phi) * sin(theta)
	    z = r * sin(phi)
	which are the equations for converting from polar to Cartesian coordinates except 
	that we make r, theta, phi a function of a parameter beta which ranges from 0 to pi.
	http://paulbourke.net/geometry/knots/
***************************************************************************/

int	DoPaulBourkesKnot4(void)

    {
    int		i, j;
    double	c1[6], t, gap, Start, End, KnotWidth, ColourFactor, r, theta, phi;

    c1[0] = c1[3] = param[10];	// x
    c1[1] = c1[4] = param[11];	// y
    c1[2] = c1[5] = param[12];	// z

    Start = param[0];
    End = param[1];
    KnotWidth = param[2];
    ColourFactor = param[3];
    totpasses = 10;

    gap = (End - Start) / iterations;
    t = Start;
    InitKnot(c1, 6);							// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	curpass = (int)(i * totpasses);
	r = 0.8 + 1.6 * sin(6 * t);
	theta = 2 * t;
	phi = 0.6 * PI * sin(12 * t);

	c1[0] = r * cos(phi) * cos(theta);
	c1[1] = r * cos(phi) * sin(theta);
	c1[2] = r * sin(phi);
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	if (DisplayKnot(c1, ((DWORD)(i / ColourFactor) % threshold), (int)KnotWidth, i, 6) < 0)
	    break;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesKnots();
    return 0;
    }

/**************************************************************************
	Polynomial Knots - 5 Crossings
	Ashley N. Brown August 5, 2004
***************************************************************************/

int	DoPolynomialKnotsCrossings5(void)

    {
    int		i, j;
    double	c1[6], t, gap, Start, End, KnotWidth, ColourFactor;

    c1[0] = c1[3] = param[10];	// x
    c1[1] = c1[4] = param[11];	// y
    c1[2] = c1[5] = param[12];	// z

    Start = param[0];
    End = param[1];
    KnotWidth = param[2];
    ColourFactor = param[3];
    totpasses = 10;

    gap = (End - Start) / iterations;
    t = Start;
    InitKnot(c1, 6);							// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	curpass = (int)(i * totpasses);
	c1[0] = t*t*t*t*t - 36*t*t*t + 260*t;
	c1[1] = t*t*t*t - 24*t*t;
	c1[2] = t*t*t*t*t*t*t - 31*t*t*t*t*t + 168*t*t*t + 560*t;
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	if (DisplayKnot(c1, ((DWORD)(i / ColourFactor) % threshold), (int)KnotWidth, i, 6) < 0)
	    break;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesKnots();
    return 0;
    }

/**************************************************************************
	Polynomial Knots - 6 Crossings
	Ashley N. Brown August 5, 2004
***************************************************************************/

int	DoPolynomialKnotsCrossings6(void)

    {
    int		i, j;
    double	c1[6], t, gap, Start, End, t2, KnotWidth, ColourFactor;

    c1[0] = c1[3] = param[10];	// x
    c1[1] = c1[4] = param[11];	// y
    c1[2] = c1[5] = param[12];	// z

    Start = param[0];
    End = param[1];
    KnotWidth = param[2];
    ColourFactor = param[3];
    totpasses = 10;

    gap = (End - Start) / iterations;
    t = Start;
    InitKnot(c1, 6);							// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	curpass = (int)(i * totpasses);
	t2 = t * t;
	c1[0] = t * (t2 - 4) * (t2 - 11);
	c1[1] = t*t*t*t - 12 * t2;
	c1[2] = t * (t2 - 1) * (t2 - 9) * (t2 - 49.0 / 16.0) * (t2 - 169.0 / 16.0) * (t2 - 100.0 / 9.0);
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	if (DisplayKnot(c1, ((DWORD)(i / ColourFactor) % threshold), (int)KnotWidth, i, 6) < 0)
	    break;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesKnots();
    return 0;
    }

/**************************************************************************
	Polynomial Knots - 7 Crossings
	Ashley N. Brown August 5, 2004
***************************************************************************/

int	DoPolynomialKnotsCrossings7(void)

    {
    int		i, j;
    double	c1[6], t, gap, Start, End, KnotWidth, t2, ColourFactor;

    c1[0] = c1[3] = param[10];	// x
    c1[1] = c1[4] = param[11];	// y
    c1[2] = c1[5] = param[12];	// z

    Start = param[0];
    End = param[1];
    KnotWidth = param[2];
    ColourFactor = param[3];
    totpasses = 10;

    gap = (End - Start) / iterations;
    t = Start;
    InitKnot(c1, 6);							// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	curpass = (int)(i * totpasses);
	t2 = t * t;
	c1[0] = t * (t2 - 6) * (t2 - 12);
	c1[1] = t2 * (t2 - 7) * (t2 - 9);
	c1[2] = t * (t2 - 1) * (t2 - 9) * (t2 - 1.0 / 16.0) * (t2 - 81.0 / 16.0) * (t2 - 25.0 / 4.0) * (t2 - 256.0 / 25.0);
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	if (DisplayKnot(c1, ((DWORD)(i / ColourFactor) % threshold), (int)KnotWidth, i, 6) < 0)
	    break;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesKnots();
    return 0;
    }

/**************************************************************************
	Polynomial Knots - 8 Crossings
	Ashley N. Brown August 5, 2004
***************************************************************************/

int	DoPolynomialKnotsCrossings8(void)

    {
    int		i, j;
    double	c1[6], t, t2, gap, Start, End, KnotWidth, ColourFactor;

    c1[0] = c1[3] = param[10];	// x
    c1[1] = c1[4] = param[11];	// y
    c1[2] = c1[5] = param[12];	// z

    Start = param[0];
    End = param[1];
    KnotWidth = param[2];
    ColourFactor = param[3];
    totpasses = 10;

    gap = (End - Start) / iterations;
    t = Start;
    InitKnot(c1, 6);							// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	curpass = (int)(i * totpasses);
	t2 = t * t;
	c1[0] = t*t*t*t*t * (t2 - 8) * (t2 - 10);
	c1[1] = t2*t2 - 11 * t2;
	c1[2] = t * (t2 - 1) * (t2 - 4) * (t2 - 9) * (t2 - 25.0 / 16.0) * (t2 - 961.0 / 100.0) * (t2 - 3969.0 / 400.0) * (t2 - 361.0 / 36.0);
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	if (DisplayKnot(c1, ((DWORD)(i / ColourFactor) % threshold), (int)KnotWidth, i, 6) < 0)
	    break;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesKnots();
    return 0;
    }

/**************************************************************************
	Polynomial Knots - 10 Crossings
	Ashley N. Brown August 5, 2004
***************************************************************************/

int	DoPolynomialKnotsCrossings10(void)

    {
    int		i, j;
    double	c1[6], t, t2, gap, Start, End, KnotWidth, ColourFactor;

    c1[0] = c1[3] = param[10];	// x
    c1[1] = c1[4] = param[11];	// y
    c1[2] = c1[5] = param[12];	// z

    Start = param[0];
    End = param[1];
    KnotWidth = param[2];
    ColourFactor = param[3];
    totpasses = 10;

    gap = (End - Start) / iterations;
    t = Start;
    InitKnot(c1, 6);							// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	curpass = (int)(i * totpasses);
	t2 = t * t;
	c1[0] = t * (t2 - 4) * (t2 - 9);
	c1[1] = t2 * (t2 - 7) * (t2 - 8);
	c1[2] = t2 * (t2 - 1) * (t2 - 1.0 / 16.0) * (t2 - 9.0 / 4.0) * (t2 - 441.0 / 100.0) * (t2 - 81.0 / 16.0) * (t2 - 25.0 / 4.0) * (t2 - 841.0 / 100.0) 
														* (t2 - 3721.0 / 400.0) * (t2 - 961.0 / 100.0);
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	if (DisplayKnot(c1, ((DWORD)(i / ColourFactor) % threshold), (int)KnotWidth, i, 6) < 0)
	    break;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesKnots();
    return 0;
    }

/**************************************************************************
	Shastri´s Trefoil
	Meier 2001
	http://www.3d-meier.de/ (Representation of knots 3.1)
***************************************************************************/

int	DoShastrisTrefoil(void)

    {
    int		i, j;
    double	c1[6], t, gap, Start, End, KnotWidth, ColourFactor;

    c1[0] = c1[3] = param[10];	// x
    c1[1] = c1[4] = param[11];	// y
    c1[2] = c1[5] = param[12];	// z

    Start = param[0];
    End = param[1];
    KnotWidth = param[2];
    ColourFactor = param[3];
    totpasses = 10;

    gap = (End - Start) / iterations;
    t = Start;
    InitKnot(c1, 6);							// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	curpass = (int)(i * totpasses);
	c1[0] = t*t*t - 3*t;
	c1[1] = t*t*t*t - 4*t*t;
	c1[2] = t*t*t*t*t - 10*t;
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	if (DisplayKnot(c1, ((DWORD)(i / ColourFactor) % threshold), (int)KnotWidth, i, 6) < 0)
	    break;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesKnots();
    return 0;
    }

/**************************************************************************
	Square Knot
	Meier 2001
	http://www.3d-meier.de/ (Representation of knots 2.4)
***************************************************************************/

int	DoSquareKnot(void)

    {
    int		i, j;
    double	c1[6], t, gap, Start, End, KnotWidth, ColourFactor;

    c1[0] = c1[3] = param[10];	// x
    c1[1] = c1[4] = param[11];	// y
    c1[2] = c1[5] = param[12];	// z

    Start = param[0];
    End = param[1];
    KnotWidth = param[2];
    ColourFactor = param[3];
    totpasses = 10;

    gap = (End - Start) / iterations;
    t = Start;
    InitKnot(c1, 6);							// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	curpass = (int)(i * totpasses);
	c1[0] = -22*cos(t) - 128*sin(t) - 44*cos(3*t) - 78*sin(3*t);
	c1[1] = 11*cos(t) - 43*cos(3*t) + 34*cos(5*t) - 39*sin(5*t);
	c1[2] = 70*cos(3*t) - 40*sin(3*t) + 8*cos(5*t) - 9*sin(5*t);
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	if (DisplayKnot(c1, ((DWORD)(i / ColourFactor) % threshold), (int)KnotWidth, i, 6) < 0)
	    break;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesKnots();
    return 0;
    }

/**************************************************************************
	Torus Knot
	Meier 2001
	http://www.3d-meier.de/ (Representation of knots 4.1-13)
***************************************************************************/

int	DoTorusKnot(void)

    {
    int		i, j;
    double	c1[6], t, gap, Start, End, R, r, p, q, KnotWidth, ColourFactor;

    c1[0] = c1[3] = param[10];	// x
    c1[1] = c1[4] = param[11];	// y
    c1[2] = c1[5] = param[12];	// z

    Start = param[0];
    End = param[1];
    R = param[2];
    r = param[3];
    p = param[4];
    q = param[5];
    KnotWidth = param[6];
    ColourFactor = param[7];
    totpasses = 10;

    gap = (End - Start) / iterations;
    t = Start;
    InitKnot(c1, 6);							// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	curpass = (int)(i * totpasses);
	c1[0] = (R + r * cos(p * t)) * cos(q * t);
	c1[1] = (R + r * cos(p * t)) * sin(q * t);
	c1[2] = r * sin(p * t);
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	if (DisplayKnot(c1, ((DWORD)(i / ColourFactor) % threshold), (int)KnotWidth, i, 6) < 0)
	    break;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesKnots();
    return 0;
    }

/**************************************************************************
	Trefoil Knot A
	Meier 2001
	http://www.3d-meier.de/ (Representation of knots 2.1)
***************************************************************************/

int	DoTrefoilKnotA(void)

    {
    int		i, j;
    double	c1[6], t, gap, Start, End, KnotWidth, ColourFactor;

    c1[0] = c1[3] = param[10];	// x
    c1[1] = c1[4] = param[11];	// y
    c1[2] = c1[5] = param[12];	// z

    Start = param[0];
    End = param[1];
    KnotWidth = param[2];
    ColourFactor = param[3];
    totpasses = 10;

    gap = (End - Start) / iterations;
    t = Start;
    InitKnot(c1, 6);							// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	curpass = (int)(i * totpasses);
	c1[0] = (2 + cos(1.5*t))*cos(t);
	c1[1] = (2 + cos(1.5*t))*sin(t);
	c1[2] =  sin(1.5*t);
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	if (DisplayKnot(c1, ((DWORD)(i / ColourFactor) % threshold), (int)KnotWidth, i, 6) < 0)
	    break;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesKnots();
    return 0;
    }

/**************************************************************************
	Trefoil Knot B
	Meier 2001
	http://www.3d-meier.de/ (Representation of knots 2.2)
***************************************************************************/

int	DoTrefoilKnotB(void)

    {
    int		i, j;
    double	c1[6], t, gap, Start, End, KnotWidth, ColourFactor;

    c1[0] = c1[3] = param[10];	// x
    c1[1] = c1[4] = param[11];	// y
    c1[2] = c1[5] = param[12];	// z

    Start = param[0];
    End = param[1];
    KnotWidth = param[2];
    ColourFactor = param[3];
    totpasses = 10;

    gap = (End - Start) / iterations;
    t = Start;
    InitKnot(c1, 6);							// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	curpass = (int)(i * totpasses);
	c1[0] = -10*cos(t) - 2*cos(5*t) + 15*sin(2*t);
	c1[1] = -15*cos(2*t) + 10*sin(t) - 2*sin(5*t);
	c1[2] =  10*cos(3*t);
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	if (DisplayKnot(c1, ((DWORD)(i / ColourFactor) % threshold), (int)KnotWidth, i, 6) < 0)
	    break;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesKnots();
    return 0;
    }

/**************************************************************************
	Close z value memory
***************************************************************************/

void	CloseZvaluesKnots(void)
    {
    return OscProcess.CloseZvalues();
    }

/**************************************************************************
	Common init Routines
***************************************************************************/


void	InitKnot(double c1[], int dimensions)
    {
    OscProcess.InitOscProc(dimensions, mandel_width, ScreenRatio, xdots, ydots, type, subtype, OscAnimProc, &xAxis, &yAxis, &zAxis, BOUNDARY, &Dib, c1, PerspectiveFlag, iterations, OscBackGround, &x_rot, &y_rot, &z_rot, &RemoveHiddenPixels, GlobalHwnd, wpixels, &TrueCol);
    }

/**************************************************************************
	Common Display Routines
***************************************************************************/

int	DisplayKnot(double c1[], DWORD colour, int KnotWidth, double i, int dimensions)
    {
    return OscProcess.DisplayOscillator(c1, c1, 0.0, colour, i, dimensions, FALSE, 1, 1, hor, vert, VertBias, zBias, OscAnimProc, CoordSystem, threshold, 0.0, KnotWidth);
    }

/**************************************************************************
	Compare function for Fractal Map name sort
**************************************************************************/

int	CompareKnot(int	*p1, int *p2)

    {
    char    *a1, *a2;

    a1 = KnotSpecific[*p1].name;
    a2 = KnotSpecific[*p2].name;
    if (*a1 == '\"')			    // ignore " character
	a1 = a1++;
    if (*a2 == '\"')
	a2 = a2++;
    return (_stricmp(a1, a2));
    }

/**************************************************************************
	General Knot Routines
***************************************************************************/

int	setup_Knot(void)
    {
    int	    i;

    OscillatorNum = 0;
			// we'd better count how many records we have
    while (KnotSpecific[OscillatorNum].name)
	OscillatorNum++;
    for (i = 0; i < OscillatorNum; i++) 
	OscPtrArray[i] = i;
    qsort((void *)OscPtrArray, (size_t)OscillatorNum, sizeof(OscPtrArray[0]), (int (__cdecl *)(const void *, const void *)) CompareKnot);
#ifdef	PRINTOSC
    PrintOsc(OscillatorNum);		// used for listing oscillator names in d:\temp\OscDump.txt
#endif
    return 0;
    }

#ifdef	MULTITHREAD
int	DoKnots(PMYVARIABLES var)
#else
int	DoKnots(void) 
#endif
    {
    return (KnotSpecific[subtype].calctype());
    }


