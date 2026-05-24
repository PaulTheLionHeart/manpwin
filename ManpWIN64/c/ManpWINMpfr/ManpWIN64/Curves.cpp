/*
   CURVES.CPP a module to explore 3D curves

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
//#include	"anim.h"
#include	"OscProcess.h"

//extern	HWND	gManp->GlobalHwnd;			// This is the main windows handle

extern	int	user_data(HWND);
extern	void	PlotExtras(void);
extern	double	sech(double x);
extern	int	OscillatorNum;
extern	double	dt;				// delta time
extern	double	VertBias;			// allow vertical stretching of the image
extern	double	zBias;				// allow stretching of the image in the z direction
//extern	int	xAxis, yAxis, zAxis;		// numerical values for axes for chaotic oscillators
extern	BOOL	RemoveHiddenPixels;

extern	int	OscPtrArray[];			// array of pointers to specific oscillators or fractal maps

//extern	std::vector <float> wpixels;		// an array of doubles holding slope modified iteration counts

//extern	RGBTRIPLE	OscBackGround;

void	InitCurve(double c1[], int dimensions);
int	DisplayCurve(double c1[], DWORD colour, int CurveWidth, double i, int dimensions);
void	CloseZvaluesCurves(void);

/**************************************************************************
	Abdank-Abakanowicz Quadratrix
	https://www.mathcurve.com/courbes2d.gb/abdank/abdank.shtml
***************************************************************************/

int	DoAbdankAbakanowiczQuadratrix(void)
    {
    int		i, j;
    double	c1[6], t, gap, Start, End, CurveWidth, ColourFactor, a, frequency, amplitude;

    c1[0] = c1[3] = gManp->param[10];	// x
    c1[1] = c1[4] = gManp->param[11];	// y
    c1[2] = c1[5] = gManp->param[12];	// z

    Start = gManp->param[0];
    End = gManp->param[1];
    a = gManp->param[2];
    CurveWidth = gManp->param[3];
    ColourFactor = gManp->param[4];
    amplitude = gManp->param[5];
    frequency = gManp->param[6];
    gManp->totpasses = 10;

    gap = (End - Start) / gManp->iterations;
    t = Start;
    InitCurve(c1, 3);							// pass in number of dimensions
    for (i = 0; i < gManp->iterations; i++)
	{
	gManp->curpass = (int)(i * gManp->totpasses);
	c1[0] = a * sin(t);
	c1[1] = a * a / 2 * (t + sin(t) * cos(t));
	c1[2] = amplitude*sin(frequency*t) + amplitude*sin(frequency*t);
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;

	if (user_data(gManp->GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	if (DisplayCurve(c1, ((DWORD)(i / ColourFactor)% gManp->threshold), (int)CurveWidth, i, 6) < 0)
	    break;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesCurves();
    return 0;
    }

/**************************************************************************
	Ampersand Curve
	http://mathworld.wolfram.com/AmpersandCurve.html
***************************************************************************/

int	DoAmpersandCurve(void)

    {
    int		i, j;
    double	c1[6], t, gap, Start, End, CurveWidth, ColourFactor, a, b, c, r, frequency, amplitude;

    c1[0] = c1[3] = gManp->param[10];	// x
    c1[1] = c1[4] = gManp->param[11];	// y
    c1[2] = c1[5] = gManp->param[12];	// z

    Start = gManp->param[0];
    End = gManp->param[1];
    CurveWidth = gManp->param[2];
    ColourFactor = gManp->param[3];
    amplitude = gManp->param[4];
    frequency = gManp->param[5];
    gManp->totpasses = 10;

    gap = (End - Start) / gManp->iterations;
    t = Start;
    InitCurve(c1, 3);							// pass in number of dimensions
    for (i = 0; i < gManp->iterations; i++)
	{
	gManp->curpass = (int)(i * gManp->totpasses);
	a = 2 * cos(2 * t) + cos(4 * t) + 9;
	b = -37 * cos(t) - 5 * cos(3 * t);
	c = 22 * cos(2 * t) + 16;

	r = (-b + sqrt(fabs(b * b - 4 * a * c))) / 2 / a;

	c1[0] = r * cos(t);
	c1[1] = r * sin(t);
	c1[2] = amplitude*sin(frequency*t) + amplitude*sin(frequency*t);
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;

	if (user_data(gManp->GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	if (DisplayCurve(c1, ((DWORD)(i / ColourFactor)% gManp->threshold), (int)CurveWidth, i, 6) < 0)
	    break;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesCurves();
    return 0;
    }

/**************************************************************************
	Anguinea
	https://www.mathcurve.com/courbes2d.gb/anguinee/anguinee.shtml
***************************************************************************/

int	DoAnguinea(void)

    {
    int		i, j;
    double	c1[6], t, gap, Start, End, CurveWidth, ColourFactor, a, d, frequency, amplitude;

    c1[0] = c1[3] = gManp->param[10];	// x
    c1[1] = c1[4] = gManp->param[11];	// y
    c1[2] = c1[5] = gManp->param[12];	// z

    Start = gManp->param[0];
    End = gManp->param[1];
    a = gManp->param[2];
    d = gManp->param[3];
    CurveWidth = gManp->param[4];
    ColourFactor = gManp->param[5];
    amplitude = gManp->param[6];
    frequency = gManp->param[7];
    gManp->totpasses = 10;

    gap = (End - Start) / gManp->iterations;
    t = Start;
    InitCurve(c1, 3);							// pass in number of dimensions
    for (i = 0; i < gManp->iterations; i++)
	{
	gManp->curpass = (int)(i * gManp->totpasses);
	c1[0] = d * tan(t / 2);
	c1[1] = a / 2 * sin(t);
	c1[2] = amplitude*sin(frequency*t) + amplitude*sin(frequency*t);
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;

	if (user_data(gManp->GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	if (DisplayCurve(c1, ((DWORD)(i / ColourFactor)% gManp->threshold), (int)CurveWidth, i, 6) < 0)
	    break;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesCurves();
    return 0;
    }

/**************************************************************************
	Archimedean Spiral
	http://www.3d-meier.de/
***************************************************************************/

int	DoArchimedeanSpiralCurve(void)

    {
    int		i, j;
    double	c1[6], t, gap, Start, End, CurveWidth, ColourFactor, frequency, amplitude;

    c1[0] = c1[3] = gManp->param[10];	// x
    c1[1] = c1[4] = gManp->param[11];	// y
    c1[2] = c1[5] = gManp->param[12];	// z

    Start = gManp->param[0];
    End = gManp->param[1];
    CurveWidth = gManp->param[2];
    ColourFactor = gManp->param[3];
    amplitude = gManp->param[4];
    frequency = gManp->param[5];
    gManp->totpasses = 10;

    gap = (End - Start) / gManp->iterations;
    t = Start;
    InitCurve(c1, 3);							// pass in number of dimensions
    for (i = 0; i < gManp->iterations; i++)
	{
	gManp->curpass = (int)(i * gManp->totpasses);
	c1[0] = t * cos(t);
	c1[1] = t * sin(t);
	c1[2] = amplitude*sin(frequency*t) + amplitude*sin(frequency*t);
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;

	if (user_data(gManp->GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	if (DisplayCurve(c1, ((DWORD)(i / ColourFactor)% gManp->threshold), (int)CurveWidth, i, 6) < 0)
	    break;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesCurves();
    return 0;
    }

/**************************************************************************
	Astroid
	http://www.3d-meier.de/
***************************************************************************/

int	DoAstroid(void)

    {
    int		i, j;
    double	c1[6], t, gap, Start, End, CurveWidth, ColourFactor, a, frequency, amplitude;

    c1[0] = c1[3] = gManp->param[10];	// x
    c1[1] = c1[4] = gManp->param[11];	// y
    c1[2] = c1[5] = gManp->param[12];	// z

    Start = gManp->param[0];
    End = gManp->param[1];
    a = gManp->param[2];
    CurveWidth = gManp->param[3];
    ColourFactor = gManp->param[4];
    amplitude = gManp->param[5];
    frequency = gManp->param[6];
    gManp->totpasses = 10;

    gap = (End - Start) / gManp->iterations;
    t = Start;
    InitCurve(c1, 3);							// pass in number of dimensions
    for (i = 0; i < gManp->iterations; i++)
	{
	gManp->curpass = (int)(i * gManp->totpasses);
	c1[0] = a * pow(cos(t), 3);
	c1[1] = a * pow(sin(t), 3);
	c1[2] = amplitude*sin(frequency*t) + amplitude*sin(frequency*t);
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;

	if (user_data(gManp->GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	if (DisplayCurve(c1, ((DWORD)(i / ColourFactor)% gManp->threshold), (int)CurveWidth, i, 6) < 0)
	    break;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesCurves();
    return 0;
    }

/**************************************************************************
	Bean Curve
	http://www.2dcurves.com/quartic/quarticbn.html
***************************************************************************/

int	DoBeanCurve(void)

    {
    int		i, j;
    double	c1[6], t, gap, Start, End, CurveWidth, ColourFactor, s, c, r, frequency, amplitude;

    c1[0] = c1[3] = gManp->param[10];	// x
    c1[1] = c1[4] = gManp->param[11];	// y
    c1[2] = c1[5] = gManp->param[12];	// z

    Start = gManp->param[0];
    End = gManp->param[1];
    CurveWidth = gManp->param[2];
    ColourFactor = gManp->param[3];
    amplitude = gManp->param[4];
    frequency = gManp->param[5];
    gManp->totpasses = 10;

    gap = (End - Start) / gManp->iterations;
    t = Start;
    InitCurve(c1, 3);							// pass in number of dimensions
    for (i = 0; i < gManp->iterations; i++)
	{
	gManp->curpass = (int)(i * gManp->totpasses);
	s = sin(t);
	c = cos(t);
	r = s * s * s + c * c * c;

	c1[0] = r * cos(t);
	c1[1] = r * sin(t);
	c1[2] = amplitude*sin(frequency*t) + amplitude*sin(frequency*t);
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;

	if (user_data(gManp->GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	if (DisplayCurve(c1, ((DWORD)(i / ColourFactor)% gManp->threshold), (int)CurveWidth, i, 6) < 0)
	    break;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesCurves();
    return 0;
    }

/**************************************************************************
	Besace
	https://www.mathcurve.com/courbes2d.gb/besace/besace.shtml
***************************************************************************/

int	DoBesace(void)

    {
    int		i, j;
    double	c1[6], t, gap, Start, End, CurveWidth, ColourFactor, a, b, frequency, amplitude;

    c1[0] = c1[3] = gManp->param[10];	// x
    c1[1] = c1[4] = gManp->param[11];	// y
    c1[2] = c1[5] = gManp->param[12];	// z

    Start = gManp->param[0];
    End = gManp->param[1];
    a = gManp->param[2];
    b = gManp->param[3];
    CurveWidth = gManp->param[4];
    ColourFactor = gManp->param[5];
    amplitude = gManp->param[6];
    frequency = gManp->param[7];
    gManp->totpasses = 10;

    gap = (End - Start) / gManp->iterations;
    t = Start;
    InitCurve(c1, 3);							// pass in number of dimensions
    for (i = 0; i < gManp->iterations; i++)
	{
	gManp->curpass = (int)(i * gManp->totpasses);
	c1[0] = a * cos(t) - b * sin(t);
	c1[1] = -sin(t) * c1[0];
	c1[2] = amplitude*sin(frequency*t) + amplitude*sin(frequency*t);
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;

	if (user_data(gManp->GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	if (DisplayCurve(c1, ((DWORD)(i / ColourFactor)% gManp->threshold), (int)CurveWidth, i, 6) < 0)
	    break;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesCurves();
    return 0;
    }

/**************************************************************************
	Bicorn Curve
	http://www.3d-meier.de/
***************************************************************************/

int	DoBicornCurve(void)

    {
    int		i, j;
    double	c1[6], t, gap, Start, End, CurveWidth, ColourFactor, frequency, amplitude;

    c1[0] = c1[3] = gManp->param[10];	// x
    c1[1] = c1[4] = gManp->param[11];	// y
    c1[2] = c1[5] = gManp->param[12];	// z

    Start = gManp->param[0];
    End = gManp->param[1];
    CurveWidth = gManp->param[2];
    ColourFactor = gManp->param[3];
    amplitude = gManp->param[4];
    frequency = gManp->param[5];
    gManp->totpasses = 10;

    gap = (End - Start) / gManp->iterations;
    t = Start;
    InitCurve(c1, 3);							// pass in number of dimensions
    for (i = 0; i < gManp->iterations; i++)
	{
	gManp->curpass = (int)(i * gManp->totpasses);
	c1[0] = sin(t);
	c1[1] = COSSQR(t) * (2 + cos(t))/(3 + SINSQR(t));
	c1[2] = amplitude*sin(frequency*t) + amplitude*sin(frequency*t);
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;

	if (user_data(gManp->GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	if (DisplayCurve(c1, ((DWORD)(i / ColourFactor)% gManp->threshold), (int)CurveWidth, i, 6) < 0)
	    break;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesCurves();
    return 0;
    }

/**************************************************************************
	Bifoliate Curve
	http://mathworld.wolfram.com/Bifoliate.html
***************************************************************************/

int	DoBifoliate(void)

    {
    int		i, j;
    double	c1[6], t, gap, Start, End, a, r, CurveWidth, ColourFactor, frequency, amplitude;

    c1[0] = c1[3] = gManp->param[10];	// x
    c1[1] = c1[4] = gManp->param[11];	// y
    c1[2] = c1[5] = gManp->param[12];	// z

    Start = gManp->param[0];
    End = gManp->param[1];
    a = gManp->param[2];
    CurveWidth = gManp->param[3];
    ColourFactor = gManp->param[4];
    amplitude = gManp->param[5];
    frequency = gManp->param[6];
    gManp->totpasses = 10;

    gap = (End - Start) / gManp->iterations;
    t = Start;
    InitCurve(c1, 3);							// pass in number of dimensions
    for (i = 0; i < gManp->iterations; i++)
	{
	gManp->curpass = (int)(i * gManp->totpasses);
	r = a * 8 * cos(t) * sin(t) * sin(t) / (3 + cos(4 * t));
	c1[0] = r * cos(t);
	c1[1] = r * sin(t);
	c1[2] = amplitude*sin(frequency*t) + amplitude*sin(frequency*t);
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;

	if (user_data(gManp->GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	if (DisplayCurve(c1, ((DWORD)(i / ColourFactor)% gManp->threshold), (int)CurveWidth, i, 6) < 0)
	    break;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesCurves();
    return 0;
    }

/**************************************************************************
	Biquartic-Carlos Sacre
	https://www.mathcurve.com/courbes2d.gb/biquartic/biquartic.shtml
***************************************************************************/

int	DoBiquarticCarlosSacre(void)

    {
    int		i, j;
    double	c1[6], t, gap, Start, End, n, CurveWidth, ColourFactor, frequency, amplitude;

    c1[0] = c1[3] = gManp->param[10];	// x
    c1[1] = c1[4] = gManp->param[11];	// y
    c1[2] = c1[5] = gManp->param[12];	// z

    Start = gManp->param[0];
    End = gManp->param[1];
    n = gManp->param[2];
    CurveWidth = gManp->param[3];
    ColourFactor = gManp->param[4];
    amplitude = gManp->param[5];
    frequency = gManp->param[6];
    gManp->totpasses = 10;

    gap = (End - Start) / gManp->iterations;
    t = Start;
    InitCurve(c1, 3);							// pass in number of dimensions
    for (i = 0; i < gManp->iterations; i++)
	{
	gManp->curpass = (int)(i * gManp->totpasses);
	c1[0] = sin(3 * t) * cos(t);
	c1[1] = pow(sin(3 * t) * sin(t), n);
	c1[2] = amplitude*sin(frequency*t) + amplitude*sin(frequency*t);
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;

	if (user_data(gManp->GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	if (DisplayCurve(c1, ((DWORD)(i / ColourFactor)% gManp->threshold), (int)CurveWidth, i, 6) < 0)
	    break;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesCurves();
    return 0;
    }

/**************************************************************************
	Booth's Curve
	https://www.mathcurve.com/courbes2d.gb/booth/booth.shtml
***************************************************************************/

int	DoBoothsCurve(void)

    {
    int		i, j;
    double	c1[6], t, gap, Start, End, a, b, e, r, CurveWidth, ColourFactor, frequency, amplitude;

    c1[0] = c1[3] = gManp->param[10];	// x
    c1[1] = c1[4] = gManp->param[11];	// y
    c1[2] = c1[5] = gManp->param[12];	// z

    Start = gManp->param[0];
    End = gManp->param[1];
    a = gManp->param[2];
    b = gManp->param[3];
    e = gManp->param[4];
    CurveWidth = gManp->param[5];
    ColourFactor = gManp->param[6];
    amplitude = gManp->param[7];
    frequency = gManp->param[8];
    gManp->totpasses = 10;

    gap = (End - Start) / gManp->iterations;
    t = Start;
    InitCurve(c1, 3);							// pass in number of dimensions
    for (i = 0; i < gManp->iterations; i++)
	{
	gManp->curpass = (int)(i * gManp->totpasses);
	r = sqrt(a * a * cos(t) * cos(t) + fabs(e * b * b * sin(t) * sin(t)));
	c1[0] = r * cos(t);
	c1[1] = r * sin(t);
	c1[2] = amplitude*sin(frequency*t) + amplitude*sin(frequency*t);
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;

	if (user_data(gManp->GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	if (DisplayCurve(c1, ((DWORD)(i / ColourFactor)% gManp->threshold), (int)CurveWidth, i, 6) < 0)
	    break;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesCurves();
    return 0;
    }

/**************************************************************************
	Brachistochrone Curve
	https://www.mathcurve.com/courbes2d.gb/brachistochrone/brachistochrone.shtml
***************************************************************************/

int	DoBrachistochroneCurve(void)

    {
    int		i, j;
    double	c1[6], t, gap, Start, End, a, CurveWidth, ColourFactor, frequency, amplitude;

    c1[0] = c1[3] = gManp->param[10];	// x
    c1[1] = c1[4] = gManp->param[11];	// y
    c1[2] = c1[5] = gManp->param[12];	// z

    Start = gManp->param[0];
    End = gManp->param[1];
    a = gManp->param[2];
    CurveWidth = gManp->param[3];
    ColourFactor = gManp->param[4];
    amplitude = gManp->param[5];
    frequency = gManp->param[6];
    gManp->totpasses = 10;

    gap = (End - Start) / gManp->iterations;
    t = Start;
    InitCurve(c1, 3);							// pass in number of dimensions
    for (i = 0; i < gManp->iterations; i++)
	{
	gManp->curpass = (int)(i * gManp->totpasses);
	c1[0] = a * (t - sin(t));
	c1[1] = a * (cos(t) - 1);
	c1[2] = amplitude*sin(frequency*t) + amplitude*sin(frequency*t);
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;

	if (user_data(gManp->GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	if (DisplayCurve(c1, ((DWORD)(i / ColourFactor)% gManp->threshold), (int)CurveWidth, i, 6) < 0)
	    break;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesCurves();
    return 0;
    }

/**************************************************************************
	Butterfly Curve
	https://en.wikipedia.org/wiki/Butterfly_curve_(transcendental)
***************************************************************************/

int	DoButterflyCurve(void)

    {
    int		i, j;
    double	c1[6], t, gap, Start, End, r, CurveWidth, ColourFactor, frequency, amplitude;

    c1[0] = c1[3] = gManp->param[10];	// x
    c1[1] = c1[4] = gManp->param[11];	// y
    c1[2] = c1[5] = gManp->param[12];	// z

    Start = gManp->param[0];
    End = gManp->param[1];
    CurveWidth = gManp->param[2];
    ColourFactor = gManp->param[3];
    amplitude = gManp->param[4];
    frequency = gManp->param[5];
    gManp->totpasses = 10;

    gap = (End - Start) / gManp->iterations;
    t = Start;
    InitCurve(c1, 3);							// pass in number of dimensions
    for (i = 0; i < gManp->iterations; i++)
	{
	gManp->curpass = (int)(i * gManp->totpasses);
	r = exp(cos(t)) - 2 * cos(4 * t) + pow(sin(t / 24), 5);
	c1[0] = r * sin(t);
	c1[1] = r * cos(t);
	c1[2] = amplitude*sin(frequency*t) + amplitude*sin(frequency*t);
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;

	if (user_data(gManp->GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	if (DisplayCurve(c1, ((DWORD)(i / ColourFactor)% gManp->threshold), (int)CurveWidth, i, 6) < 0)
	    break;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesCurves();
    return 0;
    }

/**************************************************************************
	Cassinian Oval
	http://www.3d-meier.de/
***************************************************************************/

int	DoCassinianOval(void)

    {
    int		i, j;
    double	c1[6], t, gap, Start, End, CurveWidth, ColourFactor, a, b, M, frequency, amplitude;

    c1[0] = c1[3] = gManp->param[10];	// x
    c1[1] = c1[4] = gManp->param[11];	// y
    c1[2] = c1[5] = gManp->param[12];	// z

    Start = gManp->param[0];
    End = gManp->param[1];
    a = gManp->param[2];
    b = gManp->param[3];
    CurveWidth = gManp->param[4];
    ColourFactor = gManp->param[5];
    amplitude = gManp->param[6];
    frequency = gManp->param[7];
    gManp->totpasses = 10;

    gap = (End - Start) / gManp->iterations;
    t = Start;
    InitCurve(c1, 3);							// pass in number of dimensions
    for (i = 0; i < gManp->iterations; i++)
	{
	gManp->curpass = (int)(i * gManp->totpasses);
	M = 2 * a*a * cos(2*t) + 2 * sqrt((-a*a*a*a + b*b*b*b) + a*a*a*a * COSSQR(2*t));
	c1[0] = sqrt(M/2) * cos(t);
	c1[1] = sqrt(M/2) * sin(t);
	c1[2] = amplitude*sin(frequency*t) + amplitude*sin(frequency*t);
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;

	if (user_data(gManp->GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	if (DisplayCurve(c1, ((DWORD)(i / ColourFactor)% gManp->threshold), (int)CurveWidth, i, 6) < 0)
	    break;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesCurves();
    return 0;
    }

/**************************************************************************
	Catenary Curve
	http://astrowww.phys.uvic.ca/~tatum/classmechs/class18.pdf
***************************************************************************/

int	DoCatenary(void)

    {
    int		i, j;
    double	c1[6], t, gap, Start, End, CurveWidth, ColourFactor, a, frequency, amplitude;

    c1[0] = c1[3] = gManp->param[10];	// x
    c1[1] = c1[4] = gManp->param[11];	// y
    c1[2] = c1[5] = gManp->param[12];	// z

    Start = gManp->param[0];
    End = gManp->param[1];
    a = gManp->param[2];
    CurveWidth = gManp->param[3];
    ColourFactor = gManp->param[4];
    amplitude = gManp->param[5];
    frequency = gManp->param[6];
    gManp->totpasses = 10;

    gap = (End - Start) / gManp->iterations;
    t = Start;
    InitCurve(c1, 3);							// pass in number of dimensions
    for (i = 0; i < gManp->iterations; i++)
	{
	gManp->curpass = (int)(i * gManp->totpasses);
	c1[0] = t;
	c1[1] = a * cosh(t / a);
	c1[2] = amplitude*sin(frequency*t) + amplitude*sin(frequency*t);
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;

	if (user_data(gManp->GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	if (DisplayCurve(c1, ((DWORD)(i / ColourFactor)% gManp->threshold), (int)CurveWidth, i, 6) < 0)
	    break;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesCurves();
    return 0;
    }

/**************************************************************************
	Cayley's Sextic
	http://mathworld.wolfram.com/CayleysSextic.html
***************************************************************************/

int	DoCayleysSextic(void)

    {
    int		i, j;
    double	c1[6], t, gap, Start, End, CurveWidth, ColourFactor, a, r, frequency, amplitude;

    c1[0] = c1[3] = gManp->param[10];	// x
    c1[1] = c1[4] = gManp->param[11];	// y
    c1[2] = c1[5] = gManp->param[12];	// z

    Start = gManp->param[0];
    End = gManp->param[1];
    a = gManp->param[2];
    CurveWidth = gManp->param[3];
    ColourFactor = gManp->param[4];
    amplitude = gManp->param[5];
    frequency = gManp->param[6];
    gManp->totpasses = 10;

    gap = (End - Start) / gManp->iterations;
    t = Start;
    InitCurve(c1, 3);							// pass in number of dimensions
    for (i = 0; i < gManp->iterations; i++)
	{
	gManp->curpass = (int)(i * gManp->totpasses);
	r = 4 * a * pow(cos(t / 3), 3);
	c1[0] = r * cos(t);
	c1[1] = r * sin(t);
	c1[2] = amplitude*sin(frequency*t) + amplitude*sin(frequency*t);
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;

	if (user_data(gManp->GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	if (DisplayCurve(c1, ((DWORD)(i / ColourFactor)% gManp->threshold), (int)CurveWidth, i, 6) < 0)
	    break;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesCurves();
    return 0;
    }

/**************************************************************************
	Chrysanthemum Curve
	Written by Paul Bourke Attributed to Temple Fay
	http://paulbourke.net/geometry/chrysanthemum/
***************************************************************************/

int	DoChrysanthemumCurve(void)

    {
    int		i, j;
    double	c1[6], CurveWidth, ColourFactor, u, r, p4, p8;

    c1[0] = c1[3] = gManp->param[10];	// x
    c1[1] = c1[4] = gManp->param[11];	// y
    c1[2] = c1[5] = gManp->param[12];	// z

    CurveWidth = gManp->param[0];
    ColourFactor = gManp->param[1];
    gManp->totpasses = 10;

    InitCurve(c1, 3);							// pass in number of dimensions
    for (i = 0; i < gManp->iterations; i++)
	{
	gManp->curpass = (int)(i * gManp->totpasses);
	u = i * 21.0 * PI / gManp->iterations;
	p4 = sin(17 * u / 3);
	p8 = sin(2 * cos(3 * u) - 28 * u);
	r = 5*(1 + sin(11*u/5)) - 4*p4*p4*p4*p4 * p8*p8*p8*p8*p8*p8*p8*p8;
	c1[0] = r * cos(u);
	c1[1] = r * sin(u);
	c1[2] = (r/20+0.2)*sin(r*2*PI/7);
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;

	if (user_data(gManp->GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	if (DisplayCurve(c1, ((DWORD)(i / ColourFactor)% gManp->threshold), (int)CurveWidth, i, 6) < 0)
	    break;
	}
    PlotExtras();
    CloseZvaluesCurves();
    return 0;
    }

/**************************************************************************
	Cissoid
	http://www.3d-meier.de/
***************************************************************************/

int	DoCissoid(void)

    {
    int		i, j;
    double	c1[6], t, gap, Start, End, CurveWidth, ColourFactor, a, frequency, amplitude;

    c1[0] = c1[3] = gManp->param[10];	// x
    c1[1] = c1[4] = gManp->param[11];	// y
    c1[2] = c1[5] = gManp->param[12];	// z

    Start = gManp->param[0];
    End = gManp->param[1];
    a = gManp->param[2];
    CurveWidth = gManp->param[3];
    ColourFactor = gManp->param[4];
    amplitude = gManp->param[5];
    frequency = gManp->param[6];
    gManp->totpasses = 10;

    gap = (End - Start) / gManp->iterations;
    t = Start;
    InitCurve(c1, 3);							// pass in number of dimensions
    for (i = 0; i < gManp->iterations; i++)
	{
	gManp->curpass = (int)(i * gManp->totpasses);
	c1[0] = a * t*t*t / (t*t + 1);
	c1[1] = a * t*t / (t*t + 1);
	c1[2] = amplitude*sin(frequency*t) + amplitude*sin(frequency*t);
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;

	if (user_data(gManp->GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	if (DisplayCurve(c1, ((DWORD)(i / ColourFactor)% gManp->threshold), (int)CurveWidth, i, 6) < 0)
	    break;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesCurves();
    return 0;
    }

/**************************************************************************
	Clelie Curve
	https://en.wikipedia.org/wiki/Clelie
***************************************************************************/

int	DoClelieCurve(void)

    {
    int		i, j;
    double	c1[6], t, gap, Start, End, CurveWidth, ColourFactor, a, m;

    c1[0] = c1[3] = gManp->param[10];	// x
    c1[1] = c1[4] = gManp->param[11];	// y
    c1[2] = c1[5] = gManp->param[12];	// z

    Start = gManp->param[0];
    End = gManp->param[1];
    a = gManp->param[2];
    m = gManp->param[3];
    CurveWidth = gManp->param[4];
    ColourFactor = gManp->param[5];
    gManp->totpasses = 10;

    gap = (End - Start) / gManp->iterations;
    t = Start;
    InitCurve(c1, 3);							// pass in number of dimensions
    for (i = 0; i < gManp->iterations; i++)
	{
	gManp->curpass = (int)(i * gManp->totpasses);
	c1[0] = a * sin(m * t) * cos(t);
	c1[1] = a * sin(m * t) * sin(t);
	c1[2] = a * cos(m * t);
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;

	if (user_data(gManp->GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	if (DisplayCurve(c1, ((DWORD)(i / ColourFactor)% gManp->threshold), (int)CurveWidth, i, 6) < 0)
	    break;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesCurves();
    return 0;
    }

/**************************************************************************
	Cochleoid
	https://www.mathcurve.com/courbes2d.gb/cochleoid/cochleoid.shtml
***************************************************************************/

int	DoCochleoid(void)

    {
    int		i, j;
    double	c1[6], t, gap, Start, End, CurveWidth, ColourFactor, a, r, frequency, amplitude;

    c1[0] = c1[3] = gManp->param[10];	// x
    c1[1] = c1[4] = gManp->param[11];	// y
    c1[2] = c1[5] = gManp->param[12];	// z

    Start = gManp->param[0];
    End = gManp->param[1];
    a = gManp->param[2];
    CurveWidth = gManp->param[3];
    ColourFactor = gManp->param[4];
    amplitude = gManp->param[5];
    frequency = gManp->param[6];
    gManp->totpasses = 10;

    gap = (End - Start) / gManp->iterations;
    t = Start;
    InitCurve(c1, 3);							// pass in number of dimensions
    for (i = 0; i < gManp->iterations; i++)
	{
	gManp->curpass = (int)(i * gManp->totpasses);
	r = a * sin(t) / t;
	c1[0] = r * cos(t);
	c1[1] = r * sin(t);
	c1[2] = amplitude*sin(frequency*t) + amplitude*sin(frequency*t);
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;

	if (user_data(gManp->GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	if (DisplayCurve(c1, ((DWORD)(i / ColourFactor)% gManp->threshold), (int)CurveWidth, i, 6) < 0)
	    break;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesCurves();
    return 0;
    }

/**************************************************************************
	Conchoid of de Sluze
	http://mathworld.wolfram.com/ConchoidofdeSluze.html
***************************************************************************/

int	DoConchoidDeSluze(void)

    {
    int		i, j;
    double	c1[6], t, gap, Start, End, CurveWidth, ColourFactor, a, k1, k2, k3, k4, k5, k6, frequency, amplitude;

    c1[0] = c1[3] = 0.0;	// x
    c1[1] = c1[4] = 0.0;	// y
    c1[2] = c1[5] = 0.0;	// z

    Start = gManp->param[0];
    End = gManp->param[1];
    a = gManp->param[2];
    k1 = gManp->param[3];
    k2 = gManp->param[4];
    k3 = gManp->param[5];
    k4 = gManp->param[6];
    k5 = gManp->param[7];
    k6 = gManp->param[8];
    CurveWidth = gManp->param[9];
    ColourFactor = gManp->param[10];
    amplitude = gManp->param[11];
    frequency = gManp->param[12];
    gManp->totpasses = 10;

    gap = (End - Start) / gManp->iterations;
    t = Start;
    InitCurve(c1, 3);							// pass in number of dimensions
    for (i = 0; i < gManp->iterations; i++)
	{
	gManp->curpass = (int)(i * gManp->totpasses);
	c1[0] = (1 / cos(t*k1) + a * cos(t*k2)) * cos(t*k3);
	c1[1] = (1 / cos(t*k4) + a * cos(t*k5)) * sin(t*k6);
	c1[2] = amplitude*sin(frequency*t) + amplitude*sin(frequency*t);
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;

	if (user_data(gManp->GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	if (DisplayCurve(c1, ((DWORD)(i / ColourFactor)% gManp->threshold), (int)CurveWidth, i, 6) < 0)
	    break;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesCurves();
    return 0;
    }

/**************************************************************************
	Convict's Curve
	https://www.mathcurve.com/courbes2d.gb/syntractrice/syntractrice.shtml
***************************************************************************/

int	DoConvictsCurve(void)

    {
    int		i, j;
    double	c1[6], t, gap, Start, End, CurveWidth, ColourFactor, a, k, frequency, amplitude;

    c1[0] = c1[3] = gManp->param[10];	// x
    c1[1] = c1[4] = gManp->param[11];	// y
    c1[2] = c1[5] = gManp->param[12];	// z

    Start = gManp->param[0];
    End = gManp->param[1];
    a = gManp->param[2];
    k = gManp->param[3];
    CurveWidth = gManp->param[4];
    ColourFactor = gManp->param[5];
    amplitude = gManp->param[6];
    frequency = gManp->param[7];
    gManp->totpasses = 10;

    gap = (End - Start) / gManp->iterations;
    t = Start;
    InitCurve(c1, 3);							// pass in number of dimensions
    for (i = 0; i < gManp->iterations; i++)
	{
	gManp->curpass = (int)(i * gManp->totpasses);
	c1[0] = a * (t - k * tanh(t));
	c1[1] = a * k / cosh(t);
	c1[2] = amplitude*sin(frequency*t) + amplitude*sin(frequency*t);
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;

	if (user_data(gManp->GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	if (DisplayCurve(c1, ((DWORD)(i / ColourFactor)% gManp->threshold), (int)CurveWidth, i, 6) < 0)
	    break;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesCurves();
    return 0;
    }

/**************************************************************************
	Cornoid
	http://mathworld.wolfram.com/Cornoid.html
***************************************************************************/

/*
int	DoCornoid(void)

    {
    int		i, j;
    double	c1[6], t, gap, Start, End, CurveWidth, ColourFactor, a, frequency, amplitude;

    c1[0] = c1[3] = gManp->param[10];	// x
    c1[1] = c1[4] = gManp->param[11];	// y
    c1[2] = c1[5] = gManp->param[12];	// z

    Start = gManp->param[0];
    End = gManp->param[1];
    a = gManp->param[2];
    CurveWidth = gManp->param[3];
    ColourFactor = gManp->param[4];
    amplitude = gManp->param[5];
    frequency = gManp->param[6];
    gManp->totpasses = 10;

    gap = (End - Start) / gManp->iterations;
    t = Start;
    InitCurve(c1, 3);							// pass in number of dimensions
    for (i = 0; i < gManp->iterations; i++)
	{
	gManp->curpass = (int)(i * gManp->totpasses);
	c1[0] = a * cos(t) * (1 - 2 * SINSQR(t));
	c1[1] = a * sin(t) * (1 + 2 * COSSQR(t));
	c1[2] = amplitude*sin(frequency*t) + amplitude*sin(frequency*t);
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;

	if (user_data(gManp->GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	if (DisplayCurve(c1, ((DWORD)(i / ColourFactor)% gManp->threshold), (int)CurveWidth, i, 6) < 0)
	    break;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesCurves();
    return 0;
    }
*/

/**************************************************************************
	Cornoid Family
	https://www.mathcurve.com/courbes2d.gb/cornoid/cornoid.shtml
***************************************************************************/

int	DoCornoidFamily(void)

    {
    int		i, j;
    double	c1[6], t, gap, Start, End, CurveWidth, ColourFactor, a, k, frequency, amplitude;

    c1[0] = c1[3] = gManp->param[10];	// x
    c1[1] = c1[4] = gManp->param[11];	// y
    c1[2] = c1[5] = gManp->param[12];	// z

    Start = gManp->param[0];
    End = gManp->param[1];
    a = gManp->param[2];
    k = gManp->param[3];
    CurveWidth = gManp->param[4];
    ColourFactor = gManp->param[5];
    amplitude = gManp->param[6];
    frequency = gManp->param[7];
    gManp->totpasses = 10;

    gap = (End - Start) / gManp->iterations;
    t = Start;
    InitCurve(c1, 3);							// pass in number of dimensions
    for (i = 0; i < gManp->iterations; i++)
	{
	gManp->curpass = (int)(i * gManp->totpasses);
	c1[0] = a * cos(t) * cos(2*t);
	c1[1] = a * sin(t) * (2 + cos(2*t*k));
	c1[2] = amplitude*sin(frequency*t) + amplitude*sin(frequency*t);
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;

	if (user_data(gManp->GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	if (DisplayCurve(c1, ((DWORD)(i / ColourFactor)% gManp->threshold), (int)CurveWidth, i, 6) < 0)
	    break;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesCurves();
    return 0;
    }

/**************************************************************************
	Cruciform
	http://mathworld.wolfram.com/Cruciform.html
***************************************************************************/

int	DoCruciform(void)
    {
    int		i, j;
    double	c1[6], t, gap, Start, End, CurveWidth, ColourFactor, a, b, PlaneLimit, frequency, amplitude;

    c1[0] = c1[3] = gManp->param[10];	// x
    c1[1] = c1[4] = gManp->param[11];	// y
    c1[2] = c1[5] = gManp->param[12];	// z

    Start = gManp->param[0];
    End = gManp->param[1];
    a = gManp->param[2];
    b = 0.5 * a;
    PlaneLimit = gManp->param[3];
    CurveWidth = gManp->param[4];
    ColourFactor = gManp->param[5];
    amplitude = gManp->param[6];
    frequency = gManp->param[7];
    gManp->totpasses = 10;

    gap = (End - Start) / gManp->iterations;
    t = Start;
    InitCurve(c1, 3);							// pass in number of dimensions
    for (i = 0; i < gManp->iterations; i++)
	{
	gManp->curpass = (int)(i * gManp->totpasses);
	c1[0] = a / cos(t);
	c1[1] = a / sin(t);
	c1[2] = amplitude*sin(frequency*t) + amplitude*sin(frequency*t);
	if (c1[0] < -PlaneLimit * gManp->AspectRatio) c1[0] = -PlaneLimit * gManp->AspectRatio;
	if (c1[0] > PlaneLimit * gManp->AspectRatio) c1[0] = PlaneLimit * gManp->AspectRatio;
	if (c1[1] < -PlaneLimit) c1[1] = -PlaneLimit; 
	if (c1[1] > PlaneLimit) c1[1] = PlaneLimit; 
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;

	if (user_data(gManp->GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	if (DisplayCurve(c1, ((DWORD)(i / ColourFactor)% gManp->threshold), (int)CurveWidth, i, 6) < 0)
	    break;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesCurves();
    return 0;
    }

/**************************************************************************
	Cycloid of Ceva
	http://mathworld.wolfram.com/CycloidofCeva.html
***************************************************************************/

int	DoCycloidOfCeva(void)

    {
    int		i, j;
    double	c1[6], t, gap, Start, End, CurveWidth, ColourFactor, r, frequency, amplitude;

    c1[0] = c1[3] = gManp->param[10];	// x
    c1[1] = c1[4] = gManp->param[11];	// y
    c1[2] = c1[5] = gManp->param[12];	// z

    Start = gManp->param[0];
    End = gManp->param[1];
    CurveWidth = gManp->param[2];
    ColourFactor = gManp->param[3];
    amplitude = gManp->param[4];
    frequency = gManp->param[5];
    gManp->totpasses = 10;

    gap = (End - Start) / gManp->iterations;
    t = Start;
    InitCurve(c1, 3);							// pass in number of dimensions
    for (i = 0; i < gManp->iterations; i++)
	{
	gManp->curpass = (int)(i * gManp->totpasses);
	r = 1 + 2 * cos(2 * t);
	c1[0] = r * cos(t);
	c1[1] = r * sin(t);
	c1[2] = amplitude*sin(frequency*t) + amplitude*sin(frequency*t);
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;

	if (user_data(gManp->GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	if (DisplayCurve(c1, ((DWORD)(i / ColourFactor)% gManp->threshold), (int)CurveWidth, i, 6) < 0)
	    break;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesCurves();
    return 0;
    }

/**************************************************************************
	Cycloids
	http://paulbourke.net/geometry/cycloid/
***************************************************************************/

int	DoCycloids(void)

    {
    int		i, j;
    double	c1[6], t, gap, Start, End, CurveWidth, ColourFactor, r, frequency, amplitude;

    c1[0] = c1[3] = gManp->param[10];	// x
    c1[1] = c1[4] = gManp->param[11];	// y
    c1[2] = c1[5] = gManp->param[12];	// z

    Start = gManp->param[0];
    End = gManp->param[1];
    r = gManp->param[2];
    CurveWidth = gManp->param[3];
    ColourFactor = gManp->param[4];
    amplitude = gManp->param[5];
    frequency = gManp->param[6];
    gManp->totpasses = 10;

    gap = (End - Start) / gManp->iterations;
    t = Start;
    InitCurve(c1, 3);							// pass in number of dimensions
    for (i = 0; i < gManp->iterations; i++)
	{
	gManp->curpass = (int)(i * gManp->totpasses);
	c1[0] = r * (t - sin(t));
	c1[1] = r * (1.0 - cos(t));
	c1[2] = amplitude*sin(frequency*t) + amplitude*sin(frequency*t);
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;

	if (user_data(gManp->GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	if (DisplayCurve(c1, ((DWORD)(i / ColourFactor)% gManp->threshold), (int)CurveWidth, i, 6) < 0)
	    break;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesCurves();
    return 0;
    }

/**************************************************************************
	Devil's Curve
	http://mathworld.wolfram.com/DevilsCurve.html
***************************************************************************/

int	DoDevilsCurve(void)

    {
    int		i, j;
    double	c1[6], t, gap, Start, End, CurveWidth, ColourFactor, a, b, r, frequency, amplitude;

    c1[0] = c1[3] = gManp->param[10];	// x
    c1[1] = c1[4] = gManp->param[11];	// y
    c1[2] = c1[5] = gManp->param[12];	// z

    Start = gManp->param[0];
    End = gManp->param[1];
    a = gManp->param[2];
    b = gManp->param[3];
    CurveWidth = gManp->param[4];
    ColourFactor = gManp->param[5];
    amplitude = gManp->param[6];
    frequency = gManp->param[7];
    gManp->totpasses = 10;

    gap = (End - Start) / gManp->iterations;
    t = Start;
    InitCurve(c1, 3);							// pass in number of dimensions
    for (i = 0; i < gManp->iterations; i++)
	{
	gManp->curpass = (int)(i * gManp->totpasses);
	r = sqrt((a*a*SINSQR(t) - b*b*COSSQR(t)) / (SINSQR(t) - COSSQR(t)));
	c1[0] = r * cos(t);
	c1[1] = r * sin(t);
	c1[2] = amplitude*sin(frequency*t) + amplitude*sin(frequency*t);
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;

	if (user_data(gManp->GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	if (DisplayCurve(c1, ((DWORD)(i / ColourFactor)% gManp->threshold), (int)CurveWidth, i, 6) < 0)
	    break;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesCurves();
    return 0;
    }

/**************************************************************************
	Double-Heart Curve-Kerner
	https://www.mathcurve.com/courbes2d.gb/doublecoeur/doublecoeur.shtml
***************************************************************************/

int	DoDoubleHeartCurveKerner(void)

    {
    int		i, j;
    double	c1[6], t, gap, Start, End, CurveWidth, ColourFactor, frequency, amplitude;

    c1[0] = c1[3] = gManp->param[10];	// x
    c1[1] = c1[4] = gManp->param[11];	// y
    c1[2] = c1[5] = gManp->param[12];	// z

    Start = gManp->param[0];
    End = gManp->param[1];
    CurveWidth = gManp->param[2];
    ColourFactor = gManp->param[3];
    amplitude = gManp->param[4];
    frequency = gManp->param[5];
    gManp->totpasses = 10;

    gap = (End - Start) / gManp->iterations;
    t = Start;
    InitCurve(c1, 3);							// pass in number of dimensions
    for (i = 0; i < gManp->iterations; i++)
	{
	gManp->curpass = (int)(i * gManp->totpasses);
	c1[0] = sin(t) * log(sin(t) * sin(t));
	c1[1] = -cos(4 * t);
	c1[2] = amplitude*sin(frequency*t) + amplitude*sin(frequency*t);
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;

	if (user_data(gManp->GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	if (DisplayCurve(c1, ((DWORD)(i / ColourFactor)% gManp->threshold), (int)CurveWidth, i, 6) < 0)
	    break;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesCurves();
    return 0;
    }

/**************************************************************************
	Dumbbell Curve
	http://mathworld.wolfram.com/DumbbellCurve.html
***************************************************************************/

int	DoDumbbellCurve(void)

    {
    int		i, j;
    double	c1[6], t, gap, Start, End, CurveWidth, ColourFactor, a, frequency, amplitude;

    c1[0] = c1[3] = gManp->param[10];	// x
    c1[1] = c1[4] = gManp->param[11];	// y
    c1[2] = c1[5] = gManp->param[12];	// z

    Start = gManp->param[0];
    End = gManp->param[1];
    a = gManp->param[2];
    CurveWidth = gManp->param[3];
    ColourFactor = gManp->param[4];
    amplitude = gManp->param[5];
    frequency = gManp->param[6];
    gManp->totpasses = 10;

    gap = (End - Start) / gManp->iterations;
    t = Start;
    InitCurve(c1, 3);							// pass in number of dimensions
    for (i = 0; i < gManp->iterations; i++)
	{
	gManp->curpass = (int)(i * gManp->totpasses);
	c1[0] = a * t;
	c1[1] = a * t * t * sqrt(1 - t * t);
	c1[2] = amplitude*sin(frequency*t) + amplitude*sin(frequency*t);
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;
	if (DisplayCurve(c1, ((DWORD)(i / ColourFactor)% gManp->threshold), (int)CurveWidth, i, 6) < 0)
	    break;
	c1[1] = -c1[1];
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;
	if (DisplayCurve(c1, ((DWORD)(i / ColourFactor)% gManp->threshold), (int)CurveWidth, i, 6) < 0)
	    break;

	if (user_data(gManp->GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesCurves();
    return 0;
    }

/**************************************************************************
	Eight Curve
	http://mathworld.wolfram.com/EightCurve.html
***************************************************************************/

int	DoEightCurve(void)

    {
    int		i, j;
    double	c1[6], t, gap, Start, End, CurveWidth, ColourFactor, a, frequency, amplitude;

    c1[0] = c1[3] = gManp->param[10];	// x
    c1[1] = c1[4] = gManp->param[11];	// y
    c1[2] = c1[5] = gManp->param[12];	// z

    Start = gManp->param[0];
    End = gManp->param[1];
    a = gManp->param[2];
    CurveWidth = gManp->param[3];
    ColourFactor = gManp->param[4];
    amplitude = gManp->param[5];
    frequency = gManp->param[6];
    gManp->totpasses = 10;

    gap = (End - Start) / gManp->iterations;
    t = Start;
    InitCurve(c1, 3);							// pass in number of dimensions
    for (i = 0; i < gManp->iterations; i++)
	{
	gManp->curpass = (int)(i * gManp->totpasses);
	c1[0] = a * sin(t);
	c1[1] = a * sin(t) * cos(t);
	c1[2] = amplitude*sin(frequency*t) + amplitude*sin(frequency*t);
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;

	if (user_data(gManp->GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	if (DisplayCurve(c1, ((DWORD)(i / ColourFactor)% gManp->threshold), (int)CurveWidth, i, 6) < 0)
	    break;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesCurves();
    return 0;
    }

/**************************************************************************
	Ellipse
	http://www-groups.dcs.st-andrews.ac.uk/%7Ehistory/Curves/Ellipse.html
***************************************************************************/

int	DoEllipse(void)

    {
    int		i, j;
    double	c1[6], t, gap, Start, End, CurveWidth, ColourFactor, a, b, frequency, amplitude;

    c1[0] = c1[3] = gManp->param[10];	// x
    c1[1] = c1[4] = gManp->param[11];	// y
    c1[2] = c1[5] = gManp->param[12];	// z

    Start = gManp->param[0];
    End = gManp->param[1];
    a = gManp->param[2];
    b = gManp->param[3];
    CurveWidth = gManp->param[4];
    ColourFactor = gManp->param[5];
    amplitude = gManp->param[6];
    frequency = gManp->param[7];
    gManp->totpasses = 10;

    gap = (End - Start) / gManp->iterations;
    t = Start;
    InitCurve(c1, 3);							// pass in number of dimensions
    for (i = 0; i < gManp->iterations; i++)
	{
	gManp->curpass = (int)(i * gManp->totpasses);
	c1[0] = a * cos(t);
	c1[1] = b * sin(t);
	c1[2] = amplitude*sin(frequency*t) + amplitude*sin(frequency*t);
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;

	if (user_data(gManp->GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	if (DisplayCurve(c1, ((DWORD)(i / ColourFactor)% gManp->threshold), (int)CurveWidth, i, 6) < 0)
	    break;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesCurves();
    return 0;
    }

/**************************************************************************
	Epicycloids
	Paul the LionHeart 2018
***************************************************************************/

int	DoEpicycloids(void)

    {
    int		i, j;
    double	c1[6], t, gap, Start, End, CurveWidth, ColourFactor, index, frequency, amplitude;

    c1[0] = c1[3] = gManp->param[10];	// x
    c1[1] = c1[4] = gManp->param[11];	// y
    c1[2] = c1[5] = gManp->param[12];	// z

    Start = gManp->param[0];
    End = gManp->param[1];
    index = gManp->param[2];
    CurveWidth = gManp->param[3];
    ColourFactor = gManp->param[4];
    amplitude = gManp->param[5];
    frequency = gManp->param[6];
    gManp->totpasses = 10;

    gap = (End - Start) / gManp->iterations;
    t = Start;
    InitCurve(c1, 3);							// pass in number of dimensions
    for (i = 0; i < gManp->iterations; i++)
	{
	gManp->curpass = (int)(i * gManp->totpasses);
	c1[0] = index * cos(t) - cos(index * t);
	c1[1] = index * sin(t) - sin(index * t);
	c1[2] = amplitude*sin(frequency*t) + amplitude*sin(frequency*t);
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;

	if (user_data(gManp->GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	if (DisplayCurve(c1, ((DWORD)(i / ColourFactor)% gManp->threshold), (int)CurveWidth, i, 6) < 0)
	    break;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesCurves();
    return 0;
    }

/**************************************************************************
	Epitrochoid
	http://www-groups.dcs.st-and.ac.uk/~history/Curves/Epitrochoid.html
***************************************************************************/

int	DoEpitrochoid(void)

    {
    int		i, j;
    double	c1[6], t, gap, Start, End, CurveWidth, ColourFactor, a, b, c, k, frequency, amplitude;

    c1[0] = c1[3] = gManp->param[10];	// x
    c1[1] = c1[4] = gManp->param[11];	// y
    c1[2] = c1[5] = gManp->param[12];	// z

    Start = gManp->param[0];
    End = gManp->param[1];
    a = gManp->param[2];
    b = gManp->param[3];
    c = gManp->param[4];
    k = gManp->param[5];
    CurveWidth = gManp->param[6];
    ColourFactor = gManp->param[7];
    amplitude = gManp->param[8];
    frequency = gManp->param[9];
    gManp->totpasses = 10;

    gap = (End - Start) / gManp->iterations;
    t = Start;
    InitCurve(c1, 3);							// pass in number of dimensions
    for (i = 0; i < gManp->iterations; i++)
	{
	gManp->curpass = (int)(i * gManp->totpasses);
	c1[0] = (a + b) * cos(k*t) - c * cos((k * a / b + 1) * t);
	c1[1] = (a + b) * sin(k*t) - c * sin((k * a / b + 1) * t);
	c1[2] = amplitude*sin(frequency*t) + amplitude*sin(frequency*t);
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;

	if (user_data(gManp->GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	if (DisplayCurve(c1, ((DWORD)(i / ColourFactor)% gManp->threshold), (int)CurveWidth, i, 6) < 0)
	    break;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesCurves();
    return 0;
    }

/**************************************************************************
	Equangular Spiral
	http://www-groups.dcs.st-andrews.ac.uk/~history/Curves/Equiangular.html
***************************************************************************/

int	DoEquangularSpiral(void)

    {
    int		i, j;
    double	c1[6], t, gap, Start, End, CurveWidth, ColourFactor, angle, alpha, r, frequency, amplitude;

    c1[0] = c1[3] = gManp->param[10];	// x
    c1[1] = c1[4] = gManp->param[11];	// y
    c1[2] = c1[5] = gManp->param[12];	// z

    Start = gManp->param[0];
    End = gManp->param[1];
    angle = gManp->param[2];
    CurveWidth = gManp->param[3];
    ColourFactor = gManp->param[4];
    amplitude = gManp->param[5];
    frequency = gManp->param[6];
    gManp->totpasses = 10;

    alpha = angle / 180.0 * PI;

    gap = (End - Start) / gManp->iterations;
    t = Start;
    InitCurve(c1, 3);							// pass in number of dimensions
    for (i = 0; i < gManp->iterations; i++)
	{
	gManp->curpass = (int)(i * gManp->totpasses);
	r = exp(t / tan(alpha));
	c1[0] = r * cos(t);
	c1[1] = r * sin(t);
	c1[2] = amplitude*sin(frequency*t) + amplitude*sin(frequency*t);
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;

	if (user_data(gManp->GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	if (DisplayCurve(c1, ((DWORD)(i / ColourFactor)% gManp->threshold), (int)CurveWidth, i, 6) < 0)
	    break;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesCurves();
    return 0;
    }

/**************************************************************************
	Evolute of Ellipse
	Marcus 2018
***************************************************************************/

int	DoEvoluteEllipse(void)

    {
    int		i, j;
    double	c1[6], t, gap, Start, End, CurveWidth, ColourFactor, a, b, frequency, amplitude;

    c1[0] = c1[3] = gManp->param[10];	// x
    c1[1] = c1[4] = gManp->param[11];	// y
    c1[2] = c1[5] = gManp->param[12];	// z

    Start = gManp->param[0];
    End = gManp->param[1];
    a = gManp->param[2];
    b = gManp->param[3];
    CurveWidth = gManp->param[4];
    ColourFactor = gManp->param[5];
    amplitude = gManp->param[6];
    frequency = gManp->param[7];
    gManp->totpasses = 10;

    gap = (End - Start) / gManp->iterations;
    t = Start;
    InitCurve(c1, 3);							// pass in number of dimensions
    for (i = 0; i < gManp->iterations; i++)
	{
	gManp->curpass = (int)(i * gManp->totpasses);
	c1[0] = a * cos(t) * cos(t) * cos(t);
	c1[1] = b * sin(t) * sin(t) * sin(t);
	c1[2] = amplitude*sin(frequency*t) + amplitude*sin(frequency*t);
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;

	if (user_data(gManp->GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	if (DisplayCurve(c1, ((DWORD)(i / ColourFactor)% gManp->threshold), (int)CurveWidth, i, 6) < 0)
	    break;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesCurves();
    return 0;
    }

/**************************************************************************
	Fermat Spiral
	http://www.3d-meier.de/
***************************************************************************/

int	DoFermatSpiralCurve(void)

    {
    int		i, j;
    double	c1[6], t, gap, Start, End, CurveWidth, ColourFactor, frequency, amplitude;

    c1[0] = c1[3] = gManp->param[10];	// x
    c1[1] = c1[4] = gManp->param[11];	// y
    c1[2] = c1[5] = gManp->param[12];	// z

    Start = gManp->param[0];
    End = gManp->param[1];
    CurveWidth = gManp->param[2];
    ColourFactor = gManp->param[3];
    amplitude = gManp->param[4];
    frequency = gManp->param[5];
    gManp->totpasses = 10;

    gap = (End - Start) / gManp->iterations;
    t = Start;
    InitCurve(c1, 3);							// pass in number of dimensions
    for (i = 0; i < gManp->iterations; i++)
	{
	gManp->curpass = (int)(i * gManp->totpasses);
	if (t > 0)
	    c1[0] = sqrt(fabs(t)) * cos(t);
	else
	    c1[0] = -sqrt(fabs(t)) * cos(t);
	c1[1] = sqrt(fabs(t)) * sin(t);
	c1[2] = amplitude*sin(frequency*t) + amplitude*sin(frequency*t);
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;

	if (user_data(gManp->GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	if (DisplayCurve(c1, ((DWORD)(i / ColourFactor)% gManp->threshold), (int)CurveWidth, i, 6) < 0)
	    break;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesCurves();
    return 0;
    }

/**************************************************************************
	Fish Curve
	http://mathworld.wolfram.com/FishCurve.html
***************************************************************************/

int	DoFishCurve(void)

    {
    int		i, j;
    double	c1[6], t, gap, Start, End, CurveWidth, ColourFactor, a, frequency, amplitude;

    c1[0] = c1[3] = gManp->param[10];	// x
    c1[1] = c1[4] = gManp->param[11];	// y
    c1[2] = c1[5] = gManp->param[12];	// z

    Start = gManp->param[0];
    End = gManp->param[1];
    a = gManp->param[2];
    CurveWidth = gManp->param[3];
    ColourFactor = gManp->param[4];
    amplitude = gManp->param[5];
    frequency = gManp->param[6];
    gManp->totpasses = 10;

    gap = (End - Start) / gManp->iterations;
    t = Start;
    InitCurve(c1, 3);							// pass in number of dimensions
    for (i = 0; i < gManp->iterations; i++)
	{
	gManp->curpass = (int)(i * gManp->totpasses);
	c1[0] = a * cos(t) - a * sin(t) * sin(t) / sqrt(2.0);
	c1[1] = a * sin(t) * cos(t);
	c1[2] = amplitude*sin(frequency*t) + amplitude*sin(frequency*t);
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;

	if (user_data(gManp->GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	if (DisplayCurve(c1, ((DWORD)(i / ColourFactor)% gManp->threshold), (int)CurveWidth, i, 6) < 0)
	    break;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesCurves();
    return 0;
    }

/**************************************************************************
	Folium of Descartes
	http://www-groups.dcs.st-andrews.ac.uk/%7Ehistory/Curves/Foliumd.html
***************************************************************************/

int	DoFoliumDescartes(void)

    {
    int		i, j;
    double	c1[6], t, gap, Start, End, CurveWidth, ColourFactor, PlaneLimit, frequency, amplitude;

    c1[0] = c1[3] = gManp->param[10];	// x
    c1[1] = c1[4] = gManp->param[11];	// y
    c1[2] = c1[5] = gManp->param[12];	// z

    Start = gManp->param[0];
    End = gManp->param[1];
    PlaneLimit = gManp->param[2];
    CurveWidth = gManp->param[3];
    ColourFactor = gManp->param[4];
    amplitude = gManp->param[5];
    frequency = gManp->param[6];
    gManp->totpasses = 10;

    gap = (End - Start) / gManp->iterations;
    t = Start;
    InitCurve(c1, 3);							// pass in number of dimensions
    for (i = 0; i < gManp->iterations; i++)
	{
	gManp->curpass = (int)(i * gManp->totpasses);
	c1[0] = 3*t / (1 + t*t*t);
	c1[1] = 3*t*t / (1 + t*t*t);
	c1[2] = amplitude*sin(frequency*t) + amplitude*sin(frequency*t);
	if (c1[0] < -PlaneLimit * gManp->AspectRatio) c1[0] = -PlaneLimit * gManp->AspectRatio;
	if (c1[0] > PlaneLimit * gManp->AspectRatio) c1[0] = PlaneLimit * gManp->AspectRatio;
	if (c1[1] < -PlaneLimit) c1[1] = -PlaneLimit; 
	if (c1[1] > PlaneLimit) c1[1] = PlaneLimit; 
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;

	if (user_data(gManp->GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	if (DisplayCurve(c1, ((DWORD)(i / ColourFactor)% gManp->threshold), (int)CurveWidth, i, 6) < 0)
	    break;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesCurves();
    return 0;
    }

/**************************************************************************
	Freeth's Nephroid
	http://mathworld.wolfram.com/DumbbellCurve.html
***************************************************************************/

int	DoFreethsNephroid(void)

    {
    int		i, j;
    double	c1[6], t, gap, Start, End, CurveWidth, ColourFactor, a, r, frequency, amplitude;

    c1[0] = c1[3] = gManp->param[10];	// x
    c1[1] = c1[4] = gManp->param[11];	// y
    c1[2] = c1[5] = gManp->param[12];	// z

    Start = gManp->param[0];
    End = gManp->param[1];
    a = gManp->param[2];
    CurveWidth = gManp->param[3];
    ColourFactor = gManp->param[4];
    amplitude = gManp->param[5];
    frequency = gManp->param[6];
    gManp->totpasses = 10;

    gap = (End - Start) / gManp->iterations;
    t = Start;
    InitCurve(c1, 3);							// pass in number of dimensions
    for (i = 0; i < gManp->iterations; i++)
	{
	gManp->curpass = (int)(i * gManp->totpasses);
	r = a * (1 + 2 * sin(t / 2));
	c1[0] = r * cos(t);
	c1[1] = r * sin(t);
	c1[2] = amplitude*sin(frequency*t) + amplitude*sin(frequency*t);
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;
	if (DisplayCurve(c1, ((DWORD)(i / ColourFactor)% gManp->threshold), (int)CurveWidth, i, 6) < 0)
	    break;
	if (user_data(gManp->GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesCurves();
    return 0;
    }

/**************************************************************************
	Freeth's Nephroid Irrational
	http://mathworld.wolfram.com/DumbbellCurve.html
***************************************************************************/

int	DoFreethsNephroidIrrational(void)

    {
    int		i, j;
    double	c1[6], t, gap, Start, End, CurveWidth, ColourFactor, a, r, k, frequency, amplitude;

    c1[0] = c1[3] = gManp->param[10];	// x
    c1[1] = c1[4] = gManp->param[11];	// y
    c1[2] = c1[5] = gManp->param[12];	// z

    Start = gManp->param[0];
    End = gManp->param[1];
    a = gManp->param[2];
    k = gManp->param[3];
    CurveWidth = gManp->param[4];
    ColourFactor = gManp->param[5];
    amplitude = gManp->param[6];
    frequency = gManp->param[7];
    gManp->totpasses = 10;

    gap = (End - Start) / gManp->iterations;
    t = Start;
    InitCurve(c1, 3);							// pass in number of dimensions
    for (i = 0; i < gManp->iterations; i++)
	{
	gManp->curpass = (int)(i * gManp->totpasses);
	r = a * (1 + 2 * sin(t * k / 2));
	c1[0] = r * cos(t);
	c1[1] = r * sin(t);
	c1[2] = amplitude*sin(frequency*t) + amplitude*sin(frequency*t);
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;
	if (DisplayCurve(c1, ((DWORD)(i / ColourFactor)% gManp->threshold), (int)CurveWidth, i, 6) < 0)
	    break;
	if (user_data(gManp->GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesCurves();
    return 0;
    }

/**************************************************************************
	Gauss Curve
	http://www.3d-meier.de/
***************************************************************************/

int	DoGaussCurve(void)

    {
    int		i, j;
    double	c1[6], t, gap, Start, End, CurveWidth, ColourFactor, a, b, frequency, amplitude;

    c1[0] = c1[3] = gManp->param[10];	// x
    c1[1] = c1[4] = gManp->param[11];	// y
    c1[2] = c1[5] = gManp->param[12];	// z

    Start = gManp->param[0];
    End = gManp->param[1];
    a = gManp->param[2];
    b = gManp->param[3];
    CurveWidth = gManp->param[4];
    ColourFactor = gManp->param[5];
    amplitude = gManp->param[6];
    frequency = gManp->param[7];
    gManp->totpasses = 10;

    gap = (End - Start) / gManp->iterations;
    t = Start;
    InitCurve(c1, 3);							// pass in number of dimensions
    for (i = 0; i < gManp->iterations; i++)
	{
	gManp->curpass = (int)(i * gManp->totpasses);
	c1[0] = t;
	c1[1] = b * exp( -(a*a*t*t));
	c1[2] = amplitude*sin(frequency*t) + amplitude*sin(frequency*t);
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;

	if (user_data(gManp->GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	if (DisplayCurve(c1, ((DWORD)(i / ColourFactor)% gManp->threshold), (int)CurveWidth, i, 6) < 0)
	    break;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesCurves();
    return 0;
    }

/**************************************************************************
	Gears
	http://www.3d-meier.de/
***************************************************************************/

int	DoGears(void)

    {
    int		i, j;
    double	c1[6], t, gap, Start, End, CurveWidth, ColourFactor, a, b, n, r, frequency, amplitude;

    c1[0] = c1[3] = gManp->param[10];	// x
    c1[1] = c1[4] = gManp->param[11];	// y
    c1[2] = c1[5] = gManp->param[12];	// z

    Start = gManp->param[0];
    End = gManp->param[1];
    a = gManp->param[2];
    b = gManp->param[3];
    n = gManp->param[4];
    CurveWidth = gManp->param[5];
    ColourFactor = gManp->param[6];
    amplitude = gManp->param[7];
    frequency = gManp->param[8];
    gManp->totpasses = 10;

    gap = (End - Start) / gManp->iterations;
    t = Start;
    InitCurve(c1, 3);							// pass in number of dimensions
    for (i = 0; i < gManp->iterations; i++)
	{
	gManp->curpass = (int)(i * gManp->totpasses);
	r = a + tanh(b * sin(n*t))/b;
	c1[0] = r * cos(t);
	c1[1] = r * sin(t);
	c1[2] = amplitude*sin(frequency*t) + amplitude*sin(frequency*t);
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;

	if (user_data(gManp->GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	if (DisplayCurve(c1, ((DWORD)(i / ColourFactor)% gManp->threshold), (int)CurveWidth, i, 6) < 0)
	    break;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesCurves();
    return 0;
    }

/**************************************************************************
	Hippopede
	http://mathworld.wolfram.com/Hippopede.html
***************************************************************************/

int	DoHippopede(void)

    {
    int		i, j;
    double	c1[6], t, gap, Start, End, CurveWidth, ColourFactor, a, b, r, frequency, amplitude;

    c1[0] = c1[3] = gManp->param[10];	// x
    c1[1] = c1[4] = gManp->param[11];	// y
    c1[2] = c1[5] = gManp->param[12];	// z

    Start = gManp->param[0];
    End = gManp->param[1];
    a = gManp->param[2];
    b = gManp->param[3];
    CurveWidth = gManp->param[4];
    ColourFactor = gManp->param[5];
    amplitude = gManp->param[6];
    frequency = gManp->param[7];
    gManp->totpasses = 10;

    gap = (End - Start) / gManp->iterations;
    t = Start;
    InitCurve(c1, 3);							// pass in number of dimensions
    for (i = 0; i < gManp->iterations; i++)
	{
	gManp->curpass = (int)(i * gManp->totpasses);
	r = sqrt(4 * b * (a - b * sin(t) * sin(t)));
	c1[0] = r * cos(t);
	c1[1] = r * sin(t);
	c1[2] = amplitude*sin(frequency*t) + amplitude*sin(frequency*t);
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;

	if (user_data(gManp->GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	if (DisplayCurve(c1, ((DWORD)(i / ColourFactor)% gManp->threshold), (int)CurveWidth, i, 6) < 0)
	    break;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesCurves();
    return 0;
    }

/**************************************************************************
	Hyperbola
	http://www-groups.dcs.st-andrews.ac.uk/%7Ehistory/Curves/Hyperbola.html
***************************************************************************/

int	DoHyperbola(void)

    {
    int		i, j;
    double	c1[6], t, gap, Start, End, CurveWidth, ColourFactor, a, b, PlaneLimit, frequency, amplitude;

    c1[0] = c1[3] = gManp->param[10];	// x
    c1[1] = c1[4] = gManp->param[11];	// y
    c1[2] = c1[5] = gManp->param[12];	// z

    Start = gManp->param[0];
    End = gManp->param[1];
    a = gManp->param[2];
    b = gManp->param[3];
    PlaneLimit = gManp->param[4];
    CurveWidth = gManp->param[5];
    ColourFactor = gManp->param[6];
    amplitude = gManp->param[7];
    frequency = gManp->param[8];
    gManp->totpasses = 10;

    gap = (End - Start) / gManp->iterations;
    t = Start;
    InitCurve(c1, 3);							// pass in number of dimensions
    for (i = 0; i < gManp->iterations; i++)
	{
	gManp->curpass = (int)(i * gManp->totpasses);
	c1[0] = a * SEC(t);
	c1[1] = b * tan(t);
	c1[2] = amplitude*sin(frequency*t) + amplitude*sin(frequency*t);
	if (c1[0] < -PlaneLimit * gManp->AspectRatio) c1[0] = -PlaneLimit * gManp->AspectRatio;
	if (c1[0] > PlaneLimit * gManp->AspectRatio) c1[0] = PlaneLimit * gManp->AspectRatio;
	if (c1[1] < -PlaneLimit) c1[1] = -PlaneLimit; 
	if (c1[1] > PlaneLimit) c1[1] = PlaneLimit; 
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;

	if (user_data(gManp->GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	if (DisplayCurve(c1, ((DWORD)(i / ColourFactor)% gManp->threshold), (int)CurveWidth, i, 6) < 0)
	    break;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesCurves();
    return 0;
    }

/**************************************************************************
	Hyperbolic Spiral
	http://www.3d-meier.de/
***************************************************************************/

int	DoHyperbolicSpiralCurve(void)

    {
    int		i, j;
    double	c1[6], t, gap, Start, End, CurveWidth, ColourFactor, frequency, amplitude;

    c1[0] = c1[3] = gManp->param[10];	// x
    c1[1] = c1[4] = gManp->param[11];	// y
    c1[2] = c1[5] = gManp->param[12];	// z

    Start = gManp->param[0];
    End = gManp->param[1];
    CurveWidth = gManp->param[2];
    ColourFactor = gManp->param[3];
    amplitude = gManp->param[4];
    frequency = gManp->param[5];
    gManp->totpasses = 10;

    gap = (End - Start) / gManp->iterations;
    t = Start;
    InitCurve(c1, 3);							// pass in number of dimensions
    for (i = 0; i < gManp->iterations; i++)
	{
	gManp->curpass = (int)(i * gManp->totpasses);
	c1[0] = cos(t)/t;
	c1[1] = sin(t)/t;
	c1[2] = amplitude*sin(frequency*t) + amplitude*sin(frequency*t);
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;

	if (user_data(gManp->GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	if (DisplayCurve(c1, ((DWORD)(i / ColourFactor)% gManp->threshold), (int)CurveWidth, i, 6) < 0)
	    break;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesCurves();
    return 0;
    }

/**************************************************************************
	Hypocycloids
	http://www.3d-meier.de/
***************************************************************************/

int	DoHypocycloids(void)

    {
    int		i, j;
    double	c1[6], t, gap, Start, End, R, r, h, CurveWidth, ColourFactor, frequency, amplitude;

    c1[0] = c1[3] = gManp->param[10];	// x
    c1[1] = c1[4] = gManp->param[11];	// y
    c1[2] = c1[5] = gManp->param[12];	// z

    Start = gManp->param[0];
    End = gManp->param[1];
    R = gManp->param[2];
    r = gManp->param[3];
    h = gManp->param[4];
    CurveWidth = gManp->param[5];
    ColourFactor = gManp->param[6];
    amplitude = gManp->param[7];
    frequency = gManp->param[8];
    gManp->totpasses = 10;

//    End = 2.0 * PI * (double)lcm((int)R, (int)r);
    gap = (End - Start) / gManp->iterations;
    t = Start;
    InitCurve(c1, 3);							// pass in number of dimensions
    for (i = 0; i < gManp->iterations; i++)
	{
	gManp->curpass = (int)(i * gManp->totpasses);
	c1[0] = (R - r) * cos(t) + h * cos(((R - r)/r) * t);
	c1[1] = (R - r) * sin(t) - h * sin(((R - r)/r) * t);
	c1[2] = amplitude*sin(frequency*t) + amplitude*sin(frequency*t);
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;

	if (user_data(gManp->GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	if (DisplayCurve(c1, ((DWORD)(i / ColourFactor)% gManp->threshold), (int)CurveWidth, i, 6) < 0)
	    break;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesCurves();
    return 0;
    }

/**************************************************************************
	Hypotrochoids
	http://www-groups.dcs.st-andrews.ac.uk/%7Ehistory/Curves/Hypotrochoid.html
***************************************************************************/

int	DoHypotrochoid(void)

    {
    int		i, j;
    double	c1[6], t, gap, Start, End, R, r, d, CurveWidth, ColourFactor, frequency, amplitude;

    c1[0] = c1[3] = gManp->param[10];	// x
    c1[1] = c1[4] = gManp->param[11];	// y
    c1[2] = c1[5] = gManp->param[12];	// z

    Start = gManp->param[0];
    End = gManp->param[1];
    R = gManp->param[2];
    r = gManp->param[3];
    d = gManp->param[4];
    CurveWidth = gManp->param[5];
    ColourFactor = gManp->param[6];
    amplitude = gManp->param[7];
    frequency = gManp->param[8];
    gManp->totpasses = 10;

//    End = 2.0 * PI * (double)lcm((int)R, (int)r);
    gap = (End - Start) / gManp->iterations;
    t = Start;
    InitCurve(c1, 3);							// pass in number of dimensions
    for (i = 0; i < gManp->iterations; i++)
	{
	gManp->curpass = (int)(i * gManp->totpasses);
	c1[0] = (R - r) * cos(t) - d * cos((R - r) * t / r);
	c1[1] = (R - r) * sin(t) - d * sin((R - r) * t / r);
	c1[2] = amplitude*sin(frequency*t) + amplitude*sin(frequency*t);
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;

	if (user_data(gManp->GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	if (DisplayCurve(c1, ((DWORD)(i / ColourFactor)% gManp->threshold), (int)CurveWidth, i, 6) < 0)
	    break;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesCurves();
    return 0;
    }

/**************************************************************************
	Illumination Curve - Octic
	http://www.2dcurves.com/octic/octici.html
***************************************************************************/

int	DoIlluminationCurve(void)

    {
    int		i, j;
    double	c1[6], t, gap, Start, End, a, b, CurveWidth, ColourFactor, frequency, amplitude;

    c1[0] = c1[3] = gManp->param[10];	// x
    c1[1] = c1[4] = gManp->param[11];	// y
    c1[2] = c1[5] = gManp->param[12];	// z

    Start = gManp->param[0];
    End = gManp->param[1];
    b = gManp->param[2];
    a = gManp->param[3];
    CurveWidth = gManp->param[4];
    ColourFactor = gManp->param[5];
    amplitude = gManp->param[6];
    frequency = gManp->param[7];
    gManp->totpasses = 10;

    gap = (End - Start) / gManp->iterations;
    t = Start;
    InitCurve(c1, 3);							// pass in number of dimensions
    for (i = 0; i < gManp->iterations; i++)
	{
	gManp->curpass = (int)(i * gManp->totpasses);
	c1[0] = t;
	c1[1] = sqrt(1 / pow(t * t + 1, 3));
	c1[2] = amplitude*sin(frequency*t) + amplitude*sin(frequency*t);
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;

	if (user_data(gManp->GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	if (DisplayCurve(c1, ((DWORD)(i / ColourFactor)% gManp->threshold), (int)CurveWidth, i, 6) < 0)
	    break;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesCurves();
    return 0;
    }

/**************************************************************************
	Involute of a Circle
	http://www-groups.dcs.st-andrews.ac.uk/%7Ehistory/Curves/Involute.html
***************************************************************************/

int	DoInvoluteCircle(void)

    {
    int		i, j;
    double	c1[6], t, gap, Start, End, CurveWidth, ColourFactor, a, frequency, amplitude;

    c1[0] = c1[3] = gManp->param[10];	// x
    c1[1] = c1[4] = gManp->param[11];	// y
    c1[2] = c1[5] = gManp->param[12];	// z

    Start = gManp->param[0];
    End = gManp->param[1];
    a = gManp->param[2];
    CurveWidth = gManp->param[3];
    ColourFactor = gManp->param[4];
    amplitude = gManp->param[5];
    frequency = gManp->param[6];
    gManp->totpasses = 10;

    gap = (End - Start) / gManp->iterations;
    t = Start;
    InitCurve(c1, 3);							// pass in number of dimensions
    for (i = 0; i < gManp->iterations; i++)
	{
	gManp->curpass = (int)(i * gManp->totpasses);
	c1[0] = a * (cos(t) + t * sin(t));
	c1[1] = a * (sin(t) - t * cos(t));
	c1[2] = amplitude*sin(frequency*t) + amplitude*sin(frequency*t);
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;

	if (user_data(gManp->GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	if (DisplayCurve(c1, ((DWORD)(i / ColourFactor)% gManp->threshold), (int)CurveWidth, i, 6) < 0)
	    break;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesCurves();
    return 0;
    }

/**************************************************************************
	Kampyle of Eudoxus
	http://mathworld.wolfram.com/KampyleofEudoxus.html
***************************************************************************/

int	DoKampyleOfEudoxus(void)

    {
    int		i, j;
    double	c1[6], t, gap, Start, End, CurveWidth, ColourFactor, a, b, r, frequency, amplitude;

    c1[0] = c1[3] = gManp->param[10];	// x
    c1[1] = c1[4] = gManp->param[11];	// y
    c1[2] = c1[5] = gManp->param[12];	// z

    Start = gManp->param[0];
    End = gManp->param[1];
    a = gManp->param[2];
    b = gManp->param[3];
    CurveWidth = gManp->param[4];
    ColourFactor = gManp->param[5];
    amplitude = gManp->param[6];
    frequency = gManp->param[7];
    gManp->totpasses = 10;

    gap = (End - Start) / gManp->iterations;
    t = Start;
    InitCurve(c1, 3);							// pass in number of dimensions
    for (i = 0; i < gManp->iterations; i++)
	{
	gManp->curpass = (int)(i * gManp->totpasses);
	r = a / COSSQR(t);
	c1[0] = r * cos(t);
	c1[1] = r * sin(t);
	c1[2] = amplitude*sin(frequency*t) + amplitude*sin(frequency*t);
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;

	if (user_data(gManp->GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	if (DisplayCurve(c1, ((DWORD)(i / ColourFactor)% gManp->threshold), (int)CurveWidth, i, 6) < 0)
	    break;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesCurves();
    return 0;
    }

/**************************************************************************
	L'Hospital Quintic
	https://www.mathcurve.com/courbes2d.gb/quintic/quintic.shtml
***************************************************************************/

int	DoLHospitalQuintic(void)

    {
    int		i, j;
    double	c1[6], t, gap, Start, End, CurveWidth, ColourFactor, a, r, frequency, amplitude;

    c1[0] = c1[3] = gManp->param[10];	// x
    c1[1] = c1[4] = gManp->param[11];	// y
    c1[2] = c1[5] = gManp->param[12];	// z

    Start = gManp->param[0];
    End = gManp->param[1];
    a = gManp->param[2];
    CurveWidth = gManp->param[3];
    ColourFactor = gManp->param[4];
    amplitude = gManp->param[5];
    frequency = gManp->param[6];
    gManp->totpasses = 10;

    gap = (End - Start) / gManp->iterations;
    t = Start;
    InitCurve(c1, 3);							// pass in number of dimensions
    for (i = 0; i < gManp->iterations; i++)
	{
	gManp->curpass = (int)(i * gManp->totpasses);
	r = a * sin(t) / (1 + cos(t) * cos(2 * t));
	c1[0] = r * cos(t);
	c1[1] = r * sin(t);
	c1[2] = amplitude*sin(frequency*t) + amplitude*sin(frequency*t);
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;

	if (user_data(gManp->GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	if (DisplayCurve(c1, ((DWORD)(i / ColourFactor)% gManp->threshold), (int)CurveWidth, i, 6) < 0)
	    break;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesCurves();
    return 0;
    }

/**************************************************************************
	Lemniskate Curve
	http://www.3d-meier.de/
***************************************************************************/

int	DoLemniskate(void)

    {
    int		i, j;
    double	c1[6], t, gap, Start, End, CurveWidth, ColourFactor, a, frequency, amplitude;

    c1[0] = c1[3] = gManp->param[10];	// x
    c1[1] = c1[4] = gManp->param[11];	// y
    c1[2] = c1[5] = gManp->param[12];	// z

    Start = gManp->param[0];
    End = gManp->param[1];
    a = gManp->param[2];
    CurveWidth = gManp->param[3];
    ColourFactor = gManp->param[4];
    amplitude = gManp->param[5];
    frequency = gManp->param[6];
    gManp->totpasses = 10;

    gap = (End - Start) / gManp->iterations;
    t = Start;
    InitCurve(c1, 3);							// pass in number of dimensions
    for (i = 0; i < gManp->iterations; i++)
	{
	gManp->curpass = (int)(i * gManp->totpasses);
	c1[0] = a * cos(t)/(1 + sin(t) * sin(t));
	c1[1] = a * sin(t) * cos(t)/(1 + sin(t) * sin(t));
	c1[2] = amplitude*sin(frequency*t) + amplitude*sin(frequency*t);
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;

	if (user_data(gManp->GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	if (DisplayCurve(c1, ((DWORD)(i / ColourFactor)% gManp->threshold), (int)CurveWidth, i, 6) < 0)
	    break;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesCurves();
    return 0;
    }

/**************************************************************************
	Lilliput-Brobdingnag - from Cochleoid Formula
	https://www.mathcurve.com/courbes2d.gb/cochleoid/cochleoid.shtml
***************************************************************************/

int	DoLilliputBrobdingnag(void)

    {
    int		i, j;
    double	c1[6], t, gap, Start, End, CurveWidth, ColourFactor, a, r, frequency, amplitude;

    c1[0] = c1[3] = gManp->param[10];	// x
    c1[1] = c1[4] = gManp->param[11];	// y
    c1[2] = c1[5] = gManp->param[12];	// z

    Start = gManp->param[0];
    End = gManp->param[1];
    a = gManp->param[2];
    CurveWidth = gManp->param[3];
    ColourFactor = gManp->param[4];
    amplitude = gManp->param[5];
    frequency = gManp->param[6];
    gManp->totpasses = 10;

    gap = (End - Start) / gManp->iterations;
    t = Start;
    InitCurve(c1, 3);							// pass in number of dimensions
    for (i = 0; i < gManp->iterations; i++)
	{
	gManp->curpass = (int)(i * gManp->totpasses);
	r = a * sin(t * t) / t;
	c1[0] = r * cos(t);
	c1[1] = r * sin(t);
	c1[2] = amplitude*sin(frequency*t) + amplitude*sin(frequency*t);
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;

	if (user_data(gManp->GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	if (DisplayCurve(c1, ((DWORD)(i / ColourFactor)% gManp->threshold), (int)CurveWidth, i, 6) < 0)
	    break;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesCurves();
    return 0;
    }

/**************************************************************************
	Limacon of Pascal
	http://www-groups.dcs.st-andrews.ac.uk/~history/Curves/Limacon.html
***************************************************************************/

int	DoLimaconPascal(void)

    {
    int		i, j;
    double	c1[6], t, gap, Start, End, CurveWidth, ColourFactor, a, b, r, frequency, amplitude;

    c1[0] = c1[3] = gManp->param[10];	// x
    c1[1] = c1[4] = gManp->param[11];	// y
    c1[2] = c1[5] = gManp->param[12];	// z

    Start = gManp->param[0];
    End = gManp->param[1];
    a = gManp->param[2];
    b = 1.3 * a;
    CurveWidth = gManp->param[3];
    ColourFactor = gManp->param[4];
    amplitude = gManp->param[5];
    frequency = gManp->param[6];
    gManp->totpasses = 10;

    gap = (End - Start) / gManp->iterations;
    t = Start;
    InitCurve(c1, 3);							// pass in number of dimensions
    for (i = 0; i < gManp->iterations; i++)
	{
	gManp->curpass = (int)(i * gManp->totpasses);
	r = b + 2 * a * cos(t);
	c1[0] = r * cos(t);
	c1[1] = r * sin(t);
	c1[2] = amplitude*sin(frequency*t) + amplitude*sin(frequency*t);
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;

	if (user_data(gManp->GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	if (DisplayCurve(c1, ((DWORD)(i / ColourFactor)% gManp->threshold), (int)CurveWidth, i, 6) < 0)
	    break;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesCurves();
    return 0;
    }

/**************************************************************************
	Lissajous Curves
	http://www-groups.dcs.st-andrews.ac.uk/%7Ehistory/Curves/Lissajous.html
***************************************************************************/

int	DoLissajousCurves(void)

    {
    int		i, j;
    double	c1[6], t, gap, Start, End, CurveWidth, ColourFactor, a, b, c, n, frequency, amplitude;

    c1[0] = c1[3] = gManp->param[10];	// x
    c1[1] = c1[4] = gManp->param[11];	// y
    c1[2] = c1[5] = gManp->param[12];	// z

    Start = gManp->param[0];
    End = gManp->param[1];
    a = gManp->param[2];
    b = gManp->param[3];
    c = gManp->param[4];
    n = gManp->param[5];
    CurveWidth = gManp->param[6];
    ColourFactor = gManp->param[7];
    amplitude = gManp->param[8];
    frequency = gManp->param[9];
    gManp->totpasses = 10;

    gap = (End - Start) / gManp->iterations;
    t = Start;
    InitCurve(c1, 3);							// pass in number of dimensions
    for (i = 0; i < gManp->iterations; i++)
	{
	gManp->curpass = (int)(i * gManp->totpasses);
	c1[0] = a * sin(n*t + c);
	c1[1] = b * sin(t);
	c1[2] = amplitude*sin(frequency*t) + amplitude*sin(frequency*t);
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;

	if (user_data(gManp->GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	if (DisplayCurve(c1, ((DWORD)(i / ColourFactor)% gManp->threshold), (int)CurveWidth, i, 6) < 0)
	    break;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesCurves();
    return 0;
    }

/**************************************************************************
	Lituus
	http://www-groups.dcs.st-andrews.ac.uk/~history/Curves/Lituus.html
***************************************************************************/

int	DoLituus(void)

    {
    int		i, j;
    double	c1[6], t, gap, Start, End, CurveWidth, ColourFactor, r, frequency, amplitude;

    c1[0] = c1[3] = gManp->param[10];	// x
    c1[1] = c1[4] = gManp->param[11];	// y
    c1[2] = c1[5] = gManp->param[12];	// z

    Start = gManp->param[0];
    End = gManp->param[1];
    CurveWidth = gManp->param[2];
    ColourFactor = gManp->param[3];
    amplitude = gManp->param[4];
    frequency = gManp->param[5];
    gManp->totpasses = 10;

    gap = (End - Start) / gManp->iterations;
    t = Start;
    InitCurve(c1, 3);							// pass in number of dimensions
    for (i = 0; i < gManp->iterations; i++)
	{
	gManp->curpass = (int)(i * gManp->totpasses);
	r = (t == 0.0) ? 10000.0 : (1 / sqrt(fabs(t)));
	c1[0] = r * cos(t);
	c1[1] = r * sin(t);
	c1[2] = amplitude*sin(frequency*t) + amplitude*sin(frequency*t);
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;

	if (user_data(gManp->GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	if (DisplayCurve(c1, ((DWORD)(i / ColourFactor)% gManp->threshold), (int)CurveWidth, i, 6) < 0)
	    break;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesCurves();
    return 0;
    }

/**************************************************************************
	Logarithmic Spiral
	http://www.3d-meier.de/
***************************************************************************/

int	DoLogarithmicSpiralCurve(void)

    {
    int		i, j;
    double	c1[6], t, gap, Start, End, CurveWidth, ColourFactor, a, frequency, amplitude;

    c1[0] = c1[3] = gManp->param[10];	// x
    c1[1] = c1[4] = gManp->param[11];	// y
    c1[2] = c1[5] = gManp->param[12];	// z

    Start = gManp->param[0];
    End = gManp->param[1];
    a = gManp->param[2];
    CurveWidth = gManp->param[3];
    ColourFactor = gManp->param[4];
    amplitude = gManp->param[5];
    frequency = gManp->param[6];
    gManp->totpasses = 10;

    gap = (End - Start) / gManp->iterations;
    t = Start;
    InitCurve(c1, 3);							// pass in number of dimensions
    for (i = 0; i < gManp->iterations; i++)
	{
	gManp->curpass = (int)(i * gManp->totpasses);
	c1[0] = exp(a*t) * cos(t);
	c1[1] = exp(a*t) * sin(t);
	c1[2] = amplitude*sin(frequency*t) + amplitude*sin(frequency*t);
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;

	if (user_data(gManp->GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	if (DisplayCurve(c1, ((DWORD)(i / ColourFactor)% gManp->threshold), (int)CurveWidth, i, 6) < 0)
	    break;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesCurves();
    return 0;
    }

/**************************************************************************
	Maltese Cross Curve
	http://mathworld.wolfram.com/MalteseCrossCurve.html
***************************************************************************/

int	DoMalteseCross(void)

    {
    int		i, j;
    double	c1[6], t, gap, Start, End, CurveWidth, PlaneLimit, ColourFactor, a, r, frequency, amplitude;

    c1[0] = c1[3] = gManp->param[10];	// x
    c1[1] = c1[4] = gManp->param[11];	// y
    c1[2] = c1[5] = gManp->param[12];	// z

    Start = gManp->param[0];
    End = gManp->param[1];
    a = gManp->param[2];
    PlaneLimit = gManp->param[3];
    CurveWidth = gManp->param[4];
    ColourFactor = gManp->param[5];
    amplitude = gManp->param[6];
    frequency = gManp->param[7];
    gManp->totpasses = 10;

    gap = (End - Start) / gManp->iterations;
    t = Start;
    InitCurve(c1, 3);							// pass in number of dimensions
    for (i = 0; i < gManp->iterations; i++)
	{
	gManp->curpass = (int)(i * gManp->totpasses);
	r = 2 / sqrt(fabs(sin(a * t)));    
	c1[0] = r * cos(t);
	c1[1] = r * sin(t);
	c1[2] = amplitude*sin(frequency*t) + amplitude*sin(frequency*t);
	if (c1[0] < -PlaneLimit * gManp->AspectRatio) c1[0] = -PlaneLimit * gManp->AspectRatio;
	if (c1[0] > PlaneLimit * gManp->AspectRatio) c1[0] = PlaneLimit * gManp->AspectRatio;
	if (c1[1] < -PlaneLimit) c1[1] = -PlaneLimit; 
	if (c1[1] > PlaneLimit) c1[1] = PlaneLimit; 
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;

	if (user_data(gManp->GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	if (DisplayCurve(c1, ((DWORD)(i / ColourFactor)% gManp->threshold), (int)CurveWidth, i, 6) < 0)
	    break;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesCurves();
    return 0;
    }

/**************************************************************************
	Marcus Butterfly 1
	Marcus Rezende 04/09/2019
***************************************************************************/

int	DoMarcusButterfly1(void)

    {
    int		i, j;
    double	c1[6], t, gap, Start, End, CurveWidth, ColourFactor, a, r, frequency, amplitude;

    c1[0] = c1[3] = gManp->param[10];	// x
    c1[1] = c1[4] = gManp->param[11];	// y
    c1[2] = c1[5] = gManp->param[12];	// z

    Start = gManp->param[0];
    End = gManp->param[1];
    a = gManp->param[2];
    CurveWidth = gManp->param[3];
    ColourFactor = gManp->param[4];
    amplitude = gManp->param[5];
    frequency = gManp->param[6];
    gManp->totpasses = 10;

    gap = (End - Start) / gManp->iterations;
    t = Start;
    InitCurve(c1, 3);							// pass in number of dimensions
    for (i = 0; i < gManp->iterations; i++)
	{
	gManp->curpass = (int)(i * gManp->totpasses);
	r = a * sin(t) * cos(t);
	c1[0] = r * sin(t);
	c1[1] = r * r / 2 * (t + sin(t) * cos(t));
	c1[2] = amplitude * sin(frequency*t) + amplitude * sin(frequency*t);
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;

	if (user_data(gManp->GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	if (DisplayCurve(c1, ((DWORD)(i / ColourFactor)% gManp->threshold), (int)CurveWidth, i, 6) < 0)
	    break;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesCurves();
    return 0;
    }

/**************************************************************************
	Marcus Butterfly 2
	Marcus Rezende 04/09/2019
***************************************************************************/

int	DoMarcusButterfly2(void)

    {
    int		i, j;
    double	c1[6], t, gap, Start, End, CurveWidth, ColourFactor, a, r, frequency, amplitude;

    c1[0] = c1[3] = gManp->param[10];	// x
    c1[1] = c1[4] = gManp->param[11];	// y
    c1[2] = c1[5] = gManp->param[12];	// z

    Start = gManp->param[0];
    End = gManp->param[1];
    a = gManp->param[2];
    CurveWidth = gManp->param[3];
    ColourFactor = gManp->param[4];
    amplitude = gManp->param[5];
    frequency = gManp->param[6];
    gManp->totpasses = 10;

    gap = (End - Start) / gManp->iterations;
    t = Start;
    InitCurve(c1, 3);							// pass in number of dimensions
    for (i = 0; i < gManp->iterations; i++)
	{
	gManp->curpass = (int)(i * gManp->totpasses);
	r = a * sin(t) - cos(t);
	c1[0] = r * sin(t);
	c1[1] = r * r / 2 * (t + sin(t) * cos(t));
	c1[2] = amplitude * sin(frequency*t) + amplitude * sin(frequency*t);
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;

	if (user_data(gManp->GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	if (DisplayCurve(c1, ((DWORD)(i / ColourFactor)% gManp->threshold), (int)CurveWidth, i, 6) < 0)
	    break;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesCurves();
    return 0;
    }

/**************************************************************************
	Marcus Butterfly 3
	Marcus Rezende 04/09/2019
***************************************************************************/

int	DoMarcusButterfly3(void)

    {
    int		i, j;
    double	c1[6], t, gap, Start, End, CurveWidth, ColourFactor, a, r, frequency, amplitude;

    c1[0] = c1[3] = gManp->param[10];	// x
    c1[1] = c1[4] = gManp->param[11];	// y
    c1[2] = c1[5] = gManp->param[12];	// z

    Start = gManp->param[0];
    End = gManp->param[1];
    a = gManp->param[2];
    CurveWidth = gManp->param[3];
    ColourFactor = gManp->param[4];
    amplitude = gManp->param[5];
    frequency = gManp->param[6];
    gManp->totpasses = 10;

    gap = (End - Start) / gManp->iterations;
    t = Start;
    InitCurve(c1, 3);							// pass in number of dimensions
    for (i = 0; i < gManp->iterations; i++)
	{
	gManp->curpass = (int)(i * gManp->totpasses);
	r = a * -cos(t);
	c1[0] = r * sin(t);
	c1[1] = r * r / 2 * (t + sin(t) * cos(t));
	c1[2] = amplitude * sin(frequency*t) + amplitude * sin(frequency*t);
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;

	if (user_data(gManp->GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	if (DisplayCurve(c1, ((DWORD)(i / ColourFactor)% gManp->threshold), (int)CurveWidth, i, 6) < 0)
	    break;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesCurves();
    return 0;
    }

/**************************************************************************
	Marcus Butterfly 4 (Generic)
	Marcus Rezende 04/09/2019
***************************************************************************/

int	DoMarcusButterfly4(void)

    {
    int		i, j;
    double	c1[6], t, gap, Start, End, CurveWidth, ColourFactor, a, m, n, r, frequency, amplitude;

    c1[0] = c1[3] = gManp->param[10];	// x
    c1[1] = c1[4] = gManp->param[11];	// y
    c1[2] = c1[5] = gManp->param[12];	// z

    Start = gManp->param[0];
    End = gManp->param[1];
    a = gManp->param[2];
    m = gManp->param[3];
    n = gManp->param[4];
    CurveWidth = gManp->param[5];
    ColourFactor = gManp->param[6];
    amplitude = gManp->param[7];
    frequency = gManp->param[8];
    gManp->totpasses = 10;

    gap = (End - Start) / gManp->iterations;
    t = Start;
    InitCurve(c1, 3);							// pass in number of dimensions
    for (i = 0; i < gManp->iterations; i++)
	{
	gManp->curpass = (int)(i * gManp->totpasses);
	r = a * sin(m * t) - cos(n * t);
	c1[0] = r * sin(t);
	c1[1] = r * r / 2 * (t + sin(t) * cos(t));
	c1[2] = amplitude * sin(frequency*t) + amplitude * sin(frequency*t);
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;

	if (user_data(gManp->GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	if (DisplayCurve(c1, ((DWORD)(i / ColourFactor)% gManp->threshold), (int)CurveWidth, i, 6) < 0)
	    break;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesCurves();
    return 0;
    }

/**************************************************************************
	Marcus Curve
	Marcus Rezende September 2018
	https://www.mathcurve.com/courbes2d.gb/larme/larme.shtml ???
***************************************************************************/

int	DoMarcusCurve(void)

    {
    int		i, j;
    double	c1[6], t, gap, Start, End, CurveWidth, ColourFactor, a, m, n, PlaneLimit, frequency, amplitude;

    c1[0] = c1[3] = gManp->param[10];	// x
    c1[1] = c1[4] = gManp->param[11];	// y
    c1[2] = c1[5] = gManp->param[12];	// z

    Start = gManp->param[0];
    End = gManp->param[1];
    a = gManp->param[2];
    m = gManp->param[3];
    n = gManp->param[4];
    PlaneLimit = gManp->param[5];
    CurveWidth = gManp->param[6];
    ColourFactor = gManp->param[7];
    amplitude = gManp->param[8];
    frequency = gManp->param[9];
    gManp->totpasses = 10;

    gap = (End - Start) / gManp->iterations;
    t = Start;
    InitCurve(c1, 3);							// pass in number of dimensions
    for (i = 0; i < gManp->iterations; i++)
	{
	gManp->curpass = (int)(i * gManp->totpasses);
	c1[0] = a * cos(t) / (1 + cosh(t / sqrt(n)) * cosh(t / sqrt(n)));
	c1[1] = a * sin(t) / (1 - sinh(t / sqrt(m)) * sinh(t / sqrt(m)));
	c1[2] = amplitude * sin(frequency*t) + amplitude * sin(frequency*t);
	if (c1[0] < -PlaneLimit * gManp->AspectRatio) c1[0] = -PlaneLimit * gManp->AspectRatio;
	if (c1[0] > PlaneLimit * gManp->AspectRatio) c1[0] = PlaneLimit * gManp->AspectRatio;
	if (c1[1] < -PlaneLimit) c1[1] = -PlaneLimit;
	if (c1[1] > PlaneLimit) c1[1] = PlaneLimit;
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;

	if (user_data(gManp->GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	if (DisplayCurve(c1, ((DWORD)(i / ColourFactor)% gManp->threshold), (int)CurveWidth, i, 6) < 0)
	    break;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesCurves();
    return 0;
    }

/**************************************************************************
	Mordell Curve
	Marcus 2018
***************************************************************************/

int	DoMordellCurve(void)

    {
    int		i, j;
    double	c1[6], t, gap, Start, End, CurveWidth, ColourFactor, a, limit, frequency, amplitude;

    c1[0] = c1[3] = gManp->param[10];	// x
    c1[1] = c1[4] = gManp->param[11];	// y
    c1[2] = c1[5] = gManp->param[12];	// z

    Start = gManp->param[0];
    End = gManp->param[1];
    a = gManp->param[2];
    CurveWidth = gManp->param[3];
    ColourFactor = gManp->param[4];
    amplitude = gManp->param[5];
    frequency = gManp->param[6];
    gManp->totpasses = 10;

    limit = pow(a, 1.0/3.0);
    gap = (End - Start) / gManp->iterations;
    t = Start;
    InitCurve(c1, 3);							// pass in number of dimensions
    for (i = 0; i < gManp->iterations; i++)
	{
	gManp->curpass = (int)(i * gManp->totpasses);
	if (t > -limit)
	    {
	    c1[0] = t;
	    c1[1] = sqrt(t*t*t + a);
	    c1[2] = amplitude*sin(frequency*t) + amplitude*sin(frequency*t);
	    for (j = 0; j < 3; j++)
		c1[j + 3] += c1[j] * dt;
	    if (DisplayCurve(c1, ((DWORD)(i / ColourFactor)% gManp->threshold), (int)CurveWidth, i, 6) < 0)
		break;
	    c1[0] = t;
	    c1[1] = -sqrt(t*t*t + a);
	    c1[2] = amplitude*sin(frequency*t) + amplitude*sin(frequency*t);
	    for (j = 0; j < 3; j++)
		c1[j + 3] += c1[j] * dt;
	    if (DisplayCurve(c1, ((DWORD)(i / ColourFactor)% gManp->threshold), (int)CurveWidth, i, 6) < 0)
		break;
	    }
	if (user_data(gManp->GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesCurves();
    return 0;
    }

/**************************************************************************
	Nodal Curve
	https://www.mathcurve.com/courbes2d.gb/noeud/noeud.shtml
***************************************************************************/

int	DoNodalCurve(void)

    {
    int		i, j;
    double	c1[6], t, gap, Start, End, CurveWidth, ColourFactor, a, b, r, frequency, amplitude;

    c1[0] = c1[3] = gManp->param[10];	// x
    c1[1] = c1[4] = gManp->param[11];	// y
    c1[2] = c1[5] = gManp->param[12];	// z

    Start = gManp->param[0];
    End = gManp->param[1];
    a = gManp->param[2];
    b = gManp->param[3];
    Start *= b;
    End *= b;
    CurveWidth = gManp->param[4];
    ColourFactor = gManp->param[5];
    amplitude = gManp->param[6];
    frequency = gManp->param[7];
    gManp->totpasses = 10;

    gap = (End - Start) / gManp->iterations;
    t = Start;
    InitCurve(c1, 3);							// pass in number of dimensions
    for (i = 0; i < gManp->iterations; i++)
	{
	gManp->curpass = (int)(i * gManp->totpasses);
	r = a / tan(b * t);
	c1[0] = r * cos(t);
	c1[1] = r * sin(t);
	c1[2] = amplitude*sin(frequency*t) + amplitude*sin(frequency*t);
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;

	if (user_data(gManp->GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	if (DisplayCurve(c1, ((DWORD)(i / ColourFactor)% gManp->threshold), (int)CurveWidth, i, 6) < 0)
	    break;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesCurves();
    return 0;
    }

/**************************************************************************
	Ophiuride
	http://mathworld.wolfram.com/Ophiuride.html
***************************************************************************/

int	DoOphiuride(void)

    {
    int		i, j;
    double	c1[6], t, gap, Start, End, CurveWidth, ColourFactor, a, b, r, frequency, amplitude;

    c1[0] = c1[3] = gManp->param[10];	// x
    c1[1] = c1[4] = gManp->param[11];	// y
    c1[2] = c1[5] = gManp->param[12];	// z

    Start = gManp->param[0];
    End = gManp->param[1];
    a = gManp->param[2];
    b = gManp->param[3];
    CurveWidth = gManp->param[4];
    ColourFactor = gManp->param[5];
    amplitude = gManp->param[6];
    frequency = gManp->param[7];
    gManp->totpasses = 10;

    gap = (End - Start) / gManp->iterations;
    t = Start;
    InitCurve(c1, 3);							// pass in number of dimensions
    for (i = 0; i < gManp->iterations; i++)
	{
	gManp->curpass = (int)(i * gManp->totpasses);
	r = (b * sin(t) - a * cos(t)) * tan(t);
	c1[0] = r * cos(t);
	c1[1] = r * sin(t);
	c1[2] = amplitude*sin(frequency*t) + amplitude*sin(frequency*t);
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;
	if (DisplayCurve(c1, ((DWORD)(i / ColourFactor)% gManp->threshold), (int)CurveWidth, i, 6) < 0)
	    break;
	if (user_data(gManp->GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesCurves();
    return 0;
    }

/**************************************************************************
	Parametric Curves
	https://en.wikipedia.org/wiki/Parametric_equation#/media/File:Param34_1.jpg
***************************************************************************/

int	DoParametricCurves(int version)

    {
    int		i, j;
    double	c1[6], t, gap, Start, End, CurveWidth, ColourFactor, a, b, c, d, frequency, amplitude;

    c1[0] = c1[3] = gManp->param[10];	// x
    c1[1] = c1[4] = gManp->param[11];	// y
    c1[2] = c1[5] = gManp->param[12];	// z

    Start = gManp->param[0];
    End = gManp->param[1];
    a = gManp->param[2];
    b = gManp->param[3];
    c = gManp->param[4];
    d = gManp->param[5];
    CurveWidth = gManp->param[6];
    ColourFactor = gManp->param[7];
    amplitude = gManp->param[8];
    frequency = gManp->param[9];
    gManp->totpasses = 10;

    gap = (End - Start) / gManp->iterations;
    t = Start;
    InitCurve(c1, 3);							// pass in number of dimensions
    for (i = 0; i < gManp->iterations; i++)
	{
	gManp->curpass = (int)(i * gManp->totpasses);
	c1[0] = cos(a * t) - pow((cos(b * t)), 3);
	switch (version)
	    {
	    case 0:
		c1[1] = sin(c * t) - pow(cos(d * t), 4);
		break;
	    case 1:
		c1[1] = sin(c * t) - pow(sin(d * t), 3);
		break;
	    case 2:
		c1[1] = sin(c * t) - pow(sin(d * t), 4);
		break;
	    }
	c1[2] = amplitude*sin(frequency*t) + amplitude*sin(frequency*t);
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;
	if (DisplayCurve(c1, ((DWORD)(i / ColourFactor)% gManp->threshold), (int)CurveWidth, i, 6) < 0)
	    break;
	if (user_data(gManp->GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesCurves();
    return 0;
    }

int	DoParametricCurvesI()
    {
    return DoParametricCurves(0);
    }
int	DoParametricCurvesII()
    {
    return DoParametricCurves(1);
    }
int	DoParametricCurvesIII()
    {
    return DoParametricCurves(2);
    }

/**************************************************************************
	Parametric Curves
	https://en.wikipedia.org/wiki/Parametric_equation#/media/File:Param34_1.jpg
***************************************************************************/

int	DoParametricCurvesQ2T(int variety)

    {
    int		i, j;
    double	c1[6], t, gap, Start, End, CurveWidth, ColourFactor, a, b, c, d, e, f, g, frequency, amplitude;

    c1[0] = c1[3] = 0.0;	// x
    c1[1] = c1[4] = 0.0;	// y
    c1[2] = c1[5] = 0.0;	// z

    Start = gManp->param[0];
    End = gManp->param[1];
    a = gManp->param[2];
    b = gManp->param[3];
    c = gManp->param[4];
    d = gManp->param[5];
    e = gManp->param[6];
    f = gManp->param[7];		// i in Marcus code
    g = gManp->param[8];		// j in Marcus code
    CurveWidth = gManp->param[9];
    ColourFactor = gManp->param[10];
    amplitude = gManp->param[11];
    frequency = gManp->param[12];
    gManp->totpasses = 10;

    gap = (End - Start) / gManp->iterations;
    t = Start;
    InitCurve(c1, 3);							// pass in number of dimensions
    for (i = 0; i < gManp->iterations; i++)
	{
	gManp->curpass = (int)(i * gManp->totpasses);
	switch(variety)
	    {
	    case 0:
		c1[0] = f * cos(a * t) - cos(b * t) * sin(c * t);
		c1[1] = g * sin(d * t) - sin(e * t);
		break;
	    case 1:
		c1[0] = f * cos(a * t) - cos(b * t) * sin(c * t);
		c1[1] = g * sin(d * t) - sin(e * t) * cos(b * t);
		break;
	    case 2:
		c1[0] = f * cos(a * t) - cos(b * t);
		c1[1] = g * sin(d * t) - sin(e * t) * cos(b * t);
		break;
	    }
	c1[2] = amplitude*sin(frequency*t) + amplitude*sin(frequency*t);
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;
	if (DisplayCurve(c1, ((DWORD)(i / ColourFactor)% gManp->threshold), (int)CurveWidth, i, 6) < 0)
	    break;
	if (user_data(gManp->GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesCurves();
    return 0;
    }

int	DoParametricCurvesQT()
    {
    return DoParametricCurvesQ2T(0);
    }
int	DoParametricCurvesQT1()
    {
    return DoParametricCurvesQ2T(1);
    }
int	DoParametricCurvesQT2()
    {
    return DoParametricCurvesQ2T(2);
    }

/**************************************************************************
	Generalised Parametric Curves
	https://en.wikipedia.org/wiki/Parametric_equation#/media/File:Param34_1.jpg
***************************************************************************/

int	DoParametricCurvesGeneral(void)

    {
    int		n, m;
    double	c1[6], t, gap, Start, End, CurveWidth, ColourFactor, a, b, c, d, f, g, j, k, frequency, amplitude;

    c1[0] = c1[3] = 0.0;	// x
    c1[1] = c1[4] = 0.0;	// y
    c1[2] = c1[5] = 0.0;	// z

    Start = gManp->param[0];
    End = gManp->param[1];
    a = gManp->param[2];
    b = gManp->param[3];
    c = gManp->param[4];
    d = gManp->param[5];
    f = gManp->param[6];		// i in Marcus code
    g = gManp->param[7];		// j in Marcus code
    j = gManp->param[8];
    k = gManp->param[9];

    CurveWidth = gManp->param[10];
    ColourFactor = gManp->param[11];
    amplitude = gManp->param[12];
    frequency = gManp->param[13];
    gManp->totpasses = 10;

    gap = (End - Start) / gManp->iterations;
    t = Start;
    InitCurve(c1, 3);							// pass in number of dimensions
    for (n = 0; n < gManp->iterations; n++)
	{
	gManp->curpass = (int)(n * gManp->totpasses);
	c1[0] = f * cos(a * t) - pow(cos(b * t), j);
	c1[1] = g * sin(c * t) - pow(sin(d * t), k);
	c1[2] = amplitude*sin(frequency*t) + amplitude*sin(frequency*t);
	for (m = 0; m < 3; m++)
	    c1[m + 3] += c1[m] * dt;
	if (DisplayCurve(c1, ((DWORD)(n / ColourFactor)% gManp->threshold), (int)CurveWidth, n, 6) < 0)
	    break;
	if (user_data(gManp->GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesCurves();
    return 0;
    }

/**************************************************************************
	Pear-Shaped Curve
	http://mathworld.wolfram.com/Pear-ShapedCurve.html
***************************************************************************/

int	DoPearShapedCurve(void)

    {
    int		i, j;
    double	c1[6], t, gap, Start, End, CurveWidth, ColourFactor, a, b, frequency, amplitude;
    c1[0] = c1[3] = gManp->param[10];	// x
    c1[1] = c1[4] = gManp->param[11];	// y
    c1[2] = c1[5] = gManp->param[12];	// z

    Start = gManp->param[0];
    End = gManp->param[1];
    a = gManp->param[2];
    b = gManp->param[3];
    CurveWidth = gManp->param[4];
    ColourFactor = gManp->param[5];
    amplitude = gManp->param[6];
    frequency = gManp->param[7];
    gManp->totpasses = 10;

    gap = (End - Start) / gManp->iterations;
    t = Start;
    InitCurve(c1, 3);							// pass in number of dimensions
    for (i = 0; i < gManp->iterations; i++)
	{
	gManp->curpass = (int)(i * gManp->totpasses);
	c1[0] = t;
	c1[1] = sqrt(t * t * t * (a - t) / (b * b));
	c1[2] = amplitude*sin(frequency*t) + amplitude*sin(frequency*t);
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;
	if (DisplayCurve(c1, ((DWORD)(i / ColourFactor)% gManp->threshold), (int)CurveWidth, i, 6) < 0)
	    break;
	c1[1] = -c1[1];
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;
	if (DisplayCurve(c1, ((DWORD)(i / ColourFactor)% gManp->threshold), (int)CurveWidth, i, 6) < 0)
	    break;
	if (user_data(gManp->GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesCurves();
    return 0;
    }

/**************************************************************************
	Pedal of the Parabola
	http://www.beck-shop.de/fachbuch/leseprobe/9780521646413_Excerpt_001.pdf
***************************************************************************/

int	DoPedaloftheParabola(void)

    {
    int		i, j;
    double	c1[6], t, gap, Start, End, CurveWidth, ColourFactor, a, alpha, frequency, amplitude;

    c1[0] = c1[3] = gManp->param[10];	// x
    c1[1] = c1[4] = gManp->param[11];	// y
    c1[2] = c1[5] = gManp->param[12];	// z

    Start = gManp->param[0];
    End = gManp->param[1];
    alpha = gManp->param[2];
    a = gManp->param[3];
    CurveWidth = gManp->param[4];
    ColourFactor = gManp->param[5];
    amplitude = gManp->param[6];
    frequency = gManp->param[7];
    gManp->totpasses = 10;

    gap = (End - Start) / gManp->iterations;
    t = Start;
    InitCurve(c1, 3);							// pass in number of dimensions
    for (i = 0; i < gManp->iterations; i++)
	{
	gManp->curpass = (int)(i * gManp->totpasses);
	c1[0] = (alpha - a) * t * t / (1 + t * t);
	c1[1] = t * (alpha + a * t * t) / (1 + t * t);
	c1[2] = amplitude*sin(frequency*t) + amplitude*sin(frequency*t);
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;
	if (DisplayCurve(c1, ((DWORD)(i / ColourFactor)% gManp->threshold), (int)CurveWidth, i, 6) < 0)
	    break;
	if (user_data(gManp->GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesCurves();
    return 0;
    }

/**************************************************************************
	Piriform Curve
	http://www.3d-meier.de/
***************************************************************************/

int	DoPiriform(void)

    {
    int		i, j;
    double	c1[6], t, gap, Start, End, CurveWidth, ColourFactor, frequency, amplitude;

    c1[0] = c1[3] = gManp->param[10];	// x
    c1[1] = c1[4] = gManp->param[11];	// y
    c1[2] = c1[5] = gManp->param[12];	// z

    Start = gManp->param[0];
    End = gManp->param[1];
    CurveWidth = gManp->param[2];
    ColourFactor = gManp->param[3];
    amplitude = gManp->param[4];
    frequency = gManp->param[5];
    gManp->totpasses = 10;

    gap = (End - Start) / gManp->iterations;
    t = Start;
    InitCurve(c1, 3);							// pass in number of dimensions
    for (i = 0; i < gManp->iterations; i++)
	{
	gManp->curpass = (int)(i * gManp->totpasses);
	c1[0] = 1 + sin(t);
	c1[1] = cos(t) * (1 + sin(t));
	c1[2] = amplitude*sin(frequency*t) + amplitude*sin(frequency*t);
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;

	if (user_data(gManp->GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	if (DisplayCurve(c1, ((DWORD)(i / ColourFactor)% gManp->threshold), (int)CurveWidth, i, 6) < 0)
	    break;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesCurves();
    return 0;
    }

/**************************************************************************
	Plateau Curves
	http://mathworld.wolfram.com/PlateauCurves.html
***************************************************************************/

int	DoPlateauCurves(void)

    {
    int		i, j;
    double	c1[6], t, gap, Start, End, CurveWidth, ColourFactor, a, m, n, frequency, amplitude;

    c1[0] = c1[3] = gManp->param[10];	// x
    c1[1] = c1[4] = gManp->param[11];	// y
    c1[2] = c1[5] = gManp->param[12];	// z

    Start = gManp->param[0];
    End = gManp->param[1];
    a = gManp->param[2];
    m = gManp->param[3];
    n = gManp->param[4];
    CurveWidth = gManp->param[5];
    ColourFactor = gManp->param[6];
    amplitude = gManp->param[7];
    frequency = gManp->param[8];
    gManp->totpasses = 10;

    gap = (End - Start) / gManp->iterations;
    t = Start;
    InitCurve(c1, 3);							// pass in number of dimensions
    for (i = 0; i < gManp->iterations; i++)
	{
	gManp->curpass = (int)(i * gManp->totpasses);
	c1[0] = a * sin((m + n) * t) / sin((m - n) * t);
	c1[1] = 2 * a * sin(m*t) * sin(n*t) / sin((m - n) * t);
	c1[2] = amplitude*sin(frequency*t) + amplitude*sin(frequency*t);
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;

	if (user_data(gManp->GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	if (DisplayCurve(c1, ((DWORD)(i / ColourFactor)% gManp->threshold), (int)CurveWidth, i, 6) < 0)
	    break;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesCurves();
    return 0;
    }

/**************************************************************************
	Poinsot's Spirals
	https://en.wikipedia.org/wiki/Poinsot%27s_spirals
***************************************************************************/

int	DoPoinsotsSpirals(void)

    {
    int		i, j;
    double	c1[6], t, gap, Start, End, CurveWidth, ColourFactor, frequency, r, amplitude;

    c1[0] = c1[3] = gManp->param[10];	// x
    c1[1] = c1[4] = gManp->param[11];	// y
    c1[2] = c1[5] = gManp->param[12];	// z

    Start = gManp->param[0];
    End = gManp->param[1];
    CurveWidth = gManp->param[2];
    ColourFactor = gManp->param[3];
    amplitude = gManp->param[4];
    frequency = gManp->param[5];
    gManp->totpasses = 10;

    gap = (End - Start) / gManp->iterations;
    t = Start;
    InitCurve(c1, 3);							// pass in number of dimensions
    for (i = 0; i < gManp->iterations; i++)
	{
	gManp->curpass = (int)(i * gManp->totpasses);
	r = sech(t/3);
	c1[0] = r * cos(t);
	c1[1] = r * sin(t);
	c1[2] = amplitude*sin(frequency*t) + amplitude*sin(frequency*t);
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;

	if (user_data(gManp->GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	if (DisplayCurve(c1, ((DWORD)(i / ColourFactor)% gManp->threshold), (int)CurveWidth, i, 6) < 0)
	    break;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesCurves();
    return 0;
    }

/**************************************************************************
	Polygasteroid
	https://www.mathcurve.com/courbes2d.gb/polygasteroid/polygasteroid.shtml
***************************************************************************/

int	DoPolygasteroid(void)

    {
    int		i, j;
    double	c1[6], t, gap, Start, End, CurveWidth, ColourFactor, a, r, n, e, frequency, amplitude;

    c1[0] = c1[3] = gManp->param[10];	// x
    c1[1] = c1[4] = gManp->param[11];	// y
    c1[2] = c1[5] = gManp->param[12];	// z

    Start = gManp->param[0];
    End = gManp->param[1];
    a = gManp->param[2];
    n = gManp->param[3];
    CurveWidth = gManp->param[4];
    ColourFactor = gManp->param[5];
    amplitude = gManp->param[6];
    frequency = gManp->param[7];
    gManp->totpasses = 10;
    e = exp(1.0);

    gap = (End - Start) / gManp->iterations;
    t = Start;
    InitCurve(c1, 3);							// pass in number of dimensions
    for (i = 0; i < gManp->iterations; i++)
	{
	gManp->curpass = (int)(i * gManp->totpasses);
	r = a / (1 + e * cos(n * t));
	c1[0] = r * cos(t);
	c1[1] = r * sin(t);
	c1[2] = amplitude*sin(frequency*t) + amplitude*sin(frequency*t);
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;

	if (user_data(gManp->GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	if (DisplayCurve(c1, ((DWORD)(i / ColourFactor)% gManp->threshold), (int)CurveWidth, i, 6) < 0)
	    break;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesCurves();
    return 0;
    }

/**************************************************************************
	Power Curves
	Paul the LionHeart 2018
***************************************************************************/

int	DoPowerCurves(void)

    {
    int		i, j;
    double	c1[6], t, gap, Start, End, CurveWidth, ColourFactor, n, index, frequency, amplitude;

    c1[0] = c1[3] = gManp->param[10];	// x
    c1[1] = c1[4] = gManp->param[11];	// y
    c1[2] = c1[5] = gManp->param[12];	// z

    Start = gManp->param[0];
    End = gManp->param[1];
    n = gManp->param[2];
    CurveWidth = gManp->param[3];
    ColourFactor = gManp->param[4];
    amplitude = gManp->param[5];
    frequency = gManp->param[6];
    gManp->totpasses = 10;

    gap = (End - Start) / gManp->iterations;
    t = Start;
    InitCurve(c1, 3);							// pass in number of dimensions
    for (i = 0; i < gManp->iterations; i++)
	{
	gManp->curpass = (int)(i * gManp->totpasses);
	index = (t < 0.0) ? (int)n : n;					// no sqrt() of negative numbers
	c1[0] = t;
	c1[1] = pow(t, index);
	c1[2] = amplitude*sin(frequency*t) + amplitude*sin(frequency*t);
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;

	if (user_data(gManp->GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	if (DisplayCurve(c1, ((DWORD)(i / ColourFactor)% gManp->threshold), (int)CurveWidth, i, 6) < 0)
	    break;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesCurves();
    return 0;
    }

/**************************************************************************
	Quadratrix of Hippias
	https://en.wikipedia.org/wiki/Quadratrix_of_Hippias
***************************************************************************/

int	DoQuadratrixHippias(void)

    {
    int		i, j;
    double	c1[6], t, gap, Start, End, CurveWidth, ColourFactor, a, PlaneLimit, frequency, amplitude;

    c1[0] = c1[3] = gManp->param[10];	// x
    c1[1] = c1[4] = gManp->param[11];	// y
    c1[2] = c1[5] = gManp->param[12];	// z

    Start = gManp->param[0];
    End = gManp->param[1];
    a = gManp->param[2];
    PlaneLimit = gManp->param[3];
    CurveWidth = gManp->param[4];
    ColourFactor = gManp->param[5];
    amplitude = gManp->param[6];
    frequency = gManp->param[7];
    gManp->totpasses = 10;

    gap = (End - Start) / gManp->iterations;
    t = Start;
    InitCurve(c1, 3);							// pass in number of dimensions
    for (i = 0; i < gManp->iterations; i++)
	{
	gManp->curpass = (int)(i * gManp->totpasses);
	c1[0] = a * t * COT(t);
	c1[1] = a * t;
	c1[2] = amplitude*sin(frequency*t) + amplitude*sin(frequency*t);
	if (c1[0] < -PlaneLimit * gManp->AspectRatio) c1[0] = -PlaneLimit * gManp->AspectRatio;
	if (c1[0] > PlaneLimit * gManp->AspectRatio) c1[0] = PlaneLimit * gManp->AspectRatio;
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;

	if (user_data(gManp->GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	if (DisplayCurve(c1, ((DWORD)(i / ColourFactor)% gManp->threshold), (int)CurveWidth, i, 6) < 0)
	    break;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesCurves();
    return 0;
    }

/**************************************************************************
	Ranunculoid
	http://www.3d-meier.de/
***************************************************************************/

int	DoRanunculoid(void)

    {
    int		i, j;
    double	c1[6], t, gap, Start, End, CurveWidth, ColourFactor, a, frequency, amplitude;

    c1[0] = c1[3] = gManp->param[10];	// x
    c1[1] = c1[4] = gManp->param[11];	// y
    c1[2] = c1[5] = gManp->param[12];	// z

    Start = gManp->param[0];
    End = gManp->param[1];
    a = gManp->param[2];
    CurveWidth = gManp->param[3];
    ColourFactor = gManp->param[4];
    amplitude = gManp->param[5];
    frequency = gManp->param[6];
    gManp->totpasses = 10;

    gap = (End - Start) / gManp->iterations;
    t = Start;
    InitCurve(c1, 3);							// pass in number of dimensions
    for (i = 0; i < gManp->iterations; i++)
	{
	gManp->curpass = (int)(i * gManp->totpasses);
	c1[0] = a * (6 * cos(t) - cos(6 * t));
	c1[1] = a * (6 * sin(t) - sin(6 * t));
	c1[2] = amplitude*sin(frequency*t) + amplitude*sin(frequency*t);
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;

	if (user_data(gManp->GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	if (DisplayCurve(c1, ((DWORD)(i / ColourFactor)% gManp->threshold), (int)CurveWidth, i, 6) < 0)
	    break;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesCurves();
    return 0;
    }

/**************************************************************************
	Rhodonea Curves
	http://www-groups.dcs.st-and.ac.uk/~history/Curves/Rhodonea.html
***************************************************************************/

int	DoRhodoneaCurves(void)

    {
    int		i, j;
    double	c1[6], t, gap, Start, End, CurveWidth, ColourFactor, a, k, r, frequency, amplitude;

    c1[0] = c1[3] = gManp->param[10];	// x
    c1[1] = c1[4] = gManp->param[11];	// y
    c1[2] = c1[5] = gManp->param[12];	// z

    Start = gManp->param[0];
    End = gManp->param[1];
    a = gManp->param[2];
    k = gManp->param[3];
    CurveWidth = gManp->param[4];
    ColourFactor = gManp->param[5];
    amplitude = gManp->param[6];
    frequency = gManp->param[7];
    gManp->totpasses = 10;

    gap = (End - Start) / gManp->iterations;
    t = Start;
    InitCurve(c1, 3);							// pass in number of dimensions
    for (i = 0; i < gManp->iterations; i++)
	{
	gManp->curpass = (int)(i * gManp->totpasses);
	r = a * sin(t * k);
	c1[0] = r * cos(t);
	c1[1] = r * sin(t);
	c1[2] = amplitude*sin(frequency*t) + amplitude*sin(frequency*t);
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;

	if (user_data(gManp->GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	if (DisplayCurve(c1, ((DWORD)(i / ColourFactor)% gManp->threshold), (int)CurveWidth, i, 6) < 0)
	    break;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesCurves();
    return 0;
    }

/**************************************************************************
	Rose Curve  
	https://academo.org/demos/rhodonea-curves/
***************************************************************************/

int	DoRoseCurve(void)

    {
    int		i, j;
    double	c1[6], t, gap, Start, End, m, n, k, CurveWidth, ColourFactor, frequency, amplitude;

    c1[0] = c1[3] = gManp->param[10];	// x
    c1[1] = c1[4] = gManp->param[11];	// y
    c1[2] = c1[5] = gManp->param[12];	// z

    Start = gManp->param[0];
    End = gManp->param[1];
    m = gManp->param[2];
    n = gManp->param[3];
    CurveWidth = gManp->param[4];
    ColourFactor = gManp->param[5];
    amplitude = gManp->param[6];
    frequency = gManp->param[7];
    gManp->totpasses = 10;

    k = m / n;

    gap = (End - Start) / gManp->iterations;
    t = Start;
    InitCurve(c1, 3);							// pass in number of dimensions
    for (i = 0; i < gManp->iterations; i++)
	{
	gManp->curpass = (int)(i * gManp->totpasses);
	c1[0] = cos(k*t) * cos(t);
	c1[1] = cos(k*t) * sin(t);
	c1[2] = amplitude*sin(frequency*t) + amplitude*sin(frequency*t);
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;

	if (user_data(gManp->GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	if (DisplayCurve(c1, ((DWORD)(i / ColourFactor)% gManp->threshold), (int)CurveWidth, i, 6) < 0)
	    break;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesCurves();
    return 0;
    }

/**************************************************************************
	Scarabaeus Curve
	http://mathworld.wolfram.com/DumbbellCurve.html
***************************************************************************/

int	DoScarabaeus(void)

    {
    int		i, j;
    double	c1[6], t, gap, Start, End, CurveWidth, ColourFactor, a, b, r, frequency, amplitude;

    c1[0] = c1[3] = gManp->param[10];	// x
    c1[1] = c1[4] = gManp->param[11];	// y
    c1[2] = c1[5] = gManp->param[12];	// z

    Start = gManp->param[0];
    End = gManp->param[1];
    a = gManp->param[2];
    b = gManp->param[3];
    CurveWidth = gManp->param[4];
    ColourFactor = gManp->param[5];
    amplitude = gManp->param[6];
    frequency = gManp->param[7];
    gManp->totpasses = 10;

    gap = (End - Start) / gManp->iterations;
    t = Start;
    InitCurve(c1, 3);							// pass in number of dimensions
    for (i = 0; i < gManp->iterations; i++)
	{
	gManp->curpass = (int)(i * gManp->totpasses);
	r = b * cos(2 * t) - a * cos(t);
	c1[0] = r * cos(t);
	c1[1] = r * sin(t);
	c1[2] = amplitude*sin(frequency*t) + amplitude*sin(frequency*t);
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;
	if (DisplayCurve(c1, ((DWORD)(i / ColourFactor)% gManp->threshold), (int)CurveWidth, i, 6) < 0)
	    break;
	if (user_data(gManp->GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesCurves();
    return 0;
    }

/**************************************************************************
	Semi-Cubic Parabola  
	http://mathworld.wolfram.com/SemicubicalParabola.html
***************************************************************************/

int	DoSemiCubicParabola(void)

    {
    int		i, j;
    double	c1[6], t, gap, Start, End, a, CurveWidth, ColourFactor, frequency, amplitude;

    c1[0] = c1[3] = gManp->param[10];	// x
    c1[1] = c1[4] = gManp->param[11];	// y
    c1[2] = c1[5] = gManp->param[12];	// z

    Start = gManp->param[0];
    End = gManp->param[1];
    a = gManp->param[2];
    CurveWidth = gManp->param[3];
    ColourFactor = gManp->param[4];
    amplitude = gManp->param[5];
    frequency = gManp->param[6];
    gManp->totpasses = 10;

    gap = (End - Start) / gManp->iterations;
    t = Start;
    InitCurve(c1, 3);							// pass in number of dimensions
    for (i = 0; i < gManp->iterations; i++)
	{
	gManp->curpass = (int)(i * gManp->totpasses);
	c1[0] = t*t;
	c1[1] = a*t*t*t;
	c1[2] = amplitude*sin(frequency*t) + amplitude*sin(frequency*t);
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;

	if (user_data(gManp->GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	if (DisplayCurve(c1, ((DWORD)(i / ColourFactor)% gManp->threshold), (int)CurveWidth, i, 6) < 0)
	    break;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesCurves();
    return 0;
    }

/**************************************************************************
	Serpentine  
	http://mathworld.wolfram.com/SerpentineCurve.html
***************************************************************************/

int	DoSerpentine(void)

    {
    int		i, j;
    double	c1[6], t, gap, Start, End, a, b, CurveWidth, ColourFactor, frequency, amplitude;

    c1[0] = c1[3] = gManp->param[10];	// x
    c1[1] = c1[4] = gManp->param[11];	// y
    c1[2] = c1[5] = gManp->param[12];	// z

    Start = gManp->param[0];
    End = gManp->param[1];
    a = gManp->param[2];
    CurveWidth = gManp->param[3];
    ColourFactor = gManp->param[4];
    amplitude = gManp->param[5];
    frequency = gManp->param[6];
    gManp->totpasses = 10;
    b = 2 * a;

    gap = (End - Start) / gManp->iterations;
    t = Start;
    InitCurve(c1, 3);							// pass in number of dimensions
    for (i = 0; i < gManp->iterations; i++)
	{
	gManp->curpass = (int)(i * gManp->totpasses);
	c1[0] = t;
	c1[1] = a * b * t / (t * t + a * a);
	c1[2] = amplitude*sin(frequency*t) + amplitude*sin(frequency*t);
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;

	if (user_data(gManp->GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	if (DisplayCurve(c1, ((DWORD)(i / ColourFactor)% gManp->threshold), (int)CurveWidth, i, 6) < 0)
	    break;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesCurves();
    return 0;
    }

/**************************************************************************
	Squircle  
	https://en.wikipedia.org/wiki/Squircle
***************************************************************************/

int	DoSquircle(void)

    {
    int		i, j;
    double	c1[6], t, gap, Start, End, a, r, CurveWidth, ColourFactor, frequency, amplitude;

    c1[0] = c1[3] = gManp->param[10];	// x
    c1[1] = c1[4] = gManp->param[11];	// y
    c1[2] = c1[5] = gManp->param[12];	// z

    Start = gManp->param[0];
    End = gManp->param[1];
    r = gManp->param[2];
    a = gManp->param[3];
    CurveWidth = gManp->param[4];
    ColourFactor = gManp->param[5];
    amplitude = gManp->param[6];
    frequency = gManp->param[7];
    gManp->totpasses = 10;

    gap = (End - Start) / gManp->iterations;
    t = Start;
    InitCurve(c1, 3);							// pass in number of dimensions
    for (i = 0; i < gManp->iterations; i++)
	{
	gManp->curpass = (int)(i * gManp->totpasses);
	c1[0] = sqrt(fabs(cos(t))) * r * fabs(cos(t)) / cos(t);
	c1[1] = sqrt(fabs(sin(t))) * r * fabs(sin(t)) / sin(t);
	c1[2] = amplitude*sin(frequency*t) + amplitude*sin(frequency*t);
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;

	if (user_data(gManp->GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	if (DisplayCurve(c1, ((DWORD)(i / ColourFactor)% gManp->threshold), (int)CurveWidth, i, 6) < 0)
	    break;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesCurves();
    return 0;
    }

/**************************************************************************
	Strophoid
	http://www.3d-meier.de/
***************************************************************************/

int	DoStrophoid(void)

    {
    int		i, j;
    double	c1[6], t, gap, Start, End, CurveWidth, ColourFactor, a, frequency, amplitude;

    c1[0] = c1[3] = gManp->param[10];	// x
    c1[1] = c1[4] = gManp->param[11];	// y
    c1[2] = c1[5] = gManp->param[12];	// z

    Start = gManp->param[0];
    End = gManp->param[1];
    a = gManp->param[2];
    CurveWidth = gManp->param[3];
    ColourFactor = gManp->param[4];
    amplitude = gManp->param[5];
    frequency = gManp->param[6];
    gManp->totpasses = 10;

    gap = (End - Start) / gManp->iterations;
    t = Start;
    InitCurve(c1, 3);							// pass in number of dimensions
    for (i = 0; i < gManp->iterations; i++)
	{
	gManp->curpass = (int)(i * gManp->totpasses);
	c1[0] = a * t * (t*t - 1)/(t*t + 1);
	c1[1] = a * (t*t - 1)/(t*t + 1);
	c1[2] = amplitude*sin(frequency*t) + amplitude*sin(frequency*t);
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;

	if (user_data(gManp->GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	if (DisplayCurve(c1, ((DWORD)(i / ColourFactor)% gManp->threshold), (int)CurveWidth, i, 6) < 0)
	    break;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesCurves();
    return 0;
    }

/**************************************************************************
	Swastika Curve
	http://mathworld.wolfram.com/SwastikaCurve.html
***************************************************************************/

int	DoSwastikaCurve(void)

    {
    int		i, j;
    double	c1[6], t, gap, Start, End, CurveWidth, ColourFactor, r, frequency, amplitude;

    c1[0] = c1[3] = gManp->param[10];	// x
    c1[1] = c1[4] = gManp->param[11];	// y
    c1[2] = c1[5] = gManp->param[12];	// z

    Start = gManp->param[0];
    End = gManp->param[1];
    CurveWidth = gManp->param[2];
    ColourFactor = gManp->param[3];
    amplitude = gManp->param[4];
    frequency = gManp->param[5];
    gManp->totpasses = 10;

    gap = (End - Start) / gManp->iterations;
    t = Start;
    InitCurve(c1, 3);							// pass in number of dimensions
    for (i = 0; i < gManp->iterations; i++)
	{
	gManp->curpass = (int)(i * gManp->totpasses);
	r = sqrt(sin(t) * cos(t) / (pow(sin(t), 4) - pow(cos(t), 4)));
	c1[0] = r * cos(t);
	c1[1] = r * sin(t);
	c1[2] = amplitude*sin(frequency*t) + amplitude*sin(frequency*t);
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;

	if (user_data(gManp->GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	if (DisplayCurve(c1, ((DWORD)(i / ColourFactor)% gManp->threshold), (int)CurveWidth, i, 6) < 0)
	    break;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesCurves();
    return 0;
    }

/**************************************************************************
	Talbot's Curve
	http://www-groups.dcs.st-andrews.ac.uk/%7Ehistory/Curves/Talbots.html
***************************************************************************/

int	DoTalbotsCurve(void)

    {
    int		i, j;
    double	c1[6], t, gap, Start, End, CurveWidth, ColourFactor, a, b, f, frequency, amplitude;

    c1[0] = c1[3] = gManp->param[10];	// x
    c1[1] = c1[4] = gManp->param[11];	// y
    c1[2] = c1[5] = gManp->param[12];	// z

    Start = gManp->param[0];
    End = gManp->param[1];
    a = gManp->param[2];
    b = gManp->param[3];
    f = gManp->param[4];
    CurveWidth = gManp->param[5];
    ColourFactor = gManp->param[6];
    amplitude = gManp->param[7];
    frequency = gManp->param[8];
    gManp->totpasses = 10;

    gap = (End - Start) / gManp->iterations;
    t = Start;
    InitCurve(c1, 3);							// pass in number of dimensions
    for (i = 0; i < gManp->iterations; i++)
	{
	gManp->curpass = (int)(i * gManp->totpasses);
	c1[0] = (a*a + f*f * SINSQR(t)) * cos(t)/a;
	c1[1] = (a*a - 2*f*f + f*f * SINSQR(t)) * sin(t)/b;
	c1[2] = amplitude*sin(frequency*t) + amplitude*sin(frequency*t);
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;

	if (user_data(gManp->GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	if (DisplayCurve(c1, ((DWORD)(i / ColourFactor)% gManp->threshold), (int)CurveWidth, i, 6) < 0)
	    break;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesCurves();
    return 0;
    }

/**************************************************************************
	Tanh Spiral
	http://www.3d-meier.de/
***************************************************************************/

int	DoTanhSpiralCurve(void)

    {
    int		i, j;
    double	c1[6], t, gap, Start, End, CurveWidth, ColourFactor, a, frequency, amplitude;

    c1[0] = c1[3] = gManp->param[10];	// x
    c1[1] = c1[4] = gManp->param[11];	// y
    c1[2] = c1[5] = gManp->param[12];	// z

    Start = gManp->param[0];
    End = gManp->param[1];
    a = gManp->param[2];
    CurveWidth = gManp->param[3];
    ColourFactor = gManp->param[4];
    amplitude = gManp->param[5];
    frequency = gManp->param[6];
    gManp->totpasses = 10;

    gap = (End - Start) / gManp->iterations;
    t = Start;
    InitCurve(c1, 3);							// pass in number of dimensions
    for (i = 0; i < gManp->iterations; i++)
	{
	gManp->curpass = (int)(i * gManp->totpasses);
	c1[0] = sinh(2*t)/(cos(2*a*t) + cosh(2*t));
	c1[1] = sin(2*a*t)/(cos(2*a*t) + cosh(2*t));
	c1[2] = amplitude*sin(frequency*t) + amplitude*sin(frequency*t);
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;

	if (user_data(gManp->GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	if (DisplayCurve(c1, ((DWORD)(i / ColourFactor)% gManp->threshold), (int)CurveWidth, i, 6) < 0)
	    break;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesCurves();
    return 0;
    }

/**************************************************************************
	Tear Drop Curve
	https://www.mathcurve.com/courbes2d.gb/larme/larme.shtml
***************************************************************************/

int	DoTearDropCurve(void)

    {
    int		i, j;
    double	c1[6], t, gap, Start, End, CurveWidth, ColourFactor, a, n, frequency, amplitude;

    c1[0] = c1[3] = gManp->param[10];	// x
    c1[1] = c1[4] = gManp->param[11];	// y
    c1[2] = c1[5] = gManp->param[12];	// z

    Start = gManp->param[0];
    End = gManp->param[1];
    a = gManp->param[2];
    n = gManp->param[3];
    CurveWidth = gManp->param[4];
    ColourFactor = gManp->param[5];
    amplitude = gManp->param[6];
    frequency = gManp->param[7];
    gManp->totpasses = 10;

    gap = (End - Start) / gManp->iterations;
    t = Start;
    InitCurve(c1, 3);							// pass in number of dimensions
    for (i = 0; i < gManp->iterations; i++)
	{
	gManp->curpass = (int)(i * gManp->totpasses);
	c1[0] = a * cos(t);
	c1[1] = a * sin(t) * pow(sin(t / 2), n);
	c1[2] = amplitude*sin(frequency*t) + amplitude*sin(frequency*t);
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;

	if (user_data(gManp->GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	if (DisplayCurve(c1, ((DWORD)(i / ColourFactor)% gManp->threshold), (int)CurveWidth, i, 6) < 0)
	    break;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesCurves();
    return 0;
    }

/**************************************************************************
	Torpedo Curve
	https://www.mathcurve.com/courbes2d.gb/torpille/torpille.shtml
***************************************************************************/

int	DoTorpedoCurve(void)

    {
    int		i, j;
    double	c1[6], t, gap, Start, End, CurveWidth, ColourFactor, a, r, frequency, amplitude;

    c1[0] = c1[3] = gManp->param[10];	// x
    c1[1] = c1[4] = gManp->param[11];	// y
    c1[2] = c1[5] = gManp->param[12];	// z

    Start = gManp->param[0];
    End = gManp->param[1];
    a = gManp->param[2];
    CurveWidth = gManp->param[3];
    ColourFactor = gManp->param[4];
    amplitude = gManp->param[5];
    frequency = gManp->param[6];
    gManp->totpasses = 10;

    gap = (End - Start) / gManp->iterations;
    t = Start;
    InitCurve(c1, 3);							// pass in number of dimensions
    for (i = 0; i < gManp->iterations; i++)
	{
	gManp->curpass = (int)(i * gManp->totpasses);
	r = a * sin(4 * t) / 4 / sin(t);
	c1[0] = r * cos(t);
	c1[1] = r * sin(t);
	c1[2] = amplitude*sin(frequency*t) + amplitude*sin(frequency*t);
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;

	if (user_data(gManp->GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	if (DisplayCurve(c1, ((DWORD)(i / ColourFactor)% gManp->threshold), (int)CurveWidth, i, 6) < 0)
	    break;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesCurves();
    return 0;
    }

/**************************************************************************
	Tractrix
	http://www-groups.dcs.st-andrews.ac.uk/%7Ehistory/Curves/Tractrix.html
***************************************************************************/

int	DoTractrix(void)

    {
    int		i, j;
    double	c1[6], t, gap, Start, End, CurveWidth, ColourFactor, frequency, amplitude;

    c1[0] = c1[3] = gManp->param[10];	// x
    c1[1] = c1[4] = gManp->param[11];	// y
    c1[2] = c1[5] = gManp->param[12];	// z

    Start = gManp->param[0];
    End = gManp->param[1];
    CurveWidth = gManp->param[2];
    ColourFactor = gManp->param[3];
    amplitude = gManp->param[4];
    frequency = gManp->param[5];
    gManp->totpasses = 10;

    gap = (End - Start) / gManp->iterations;
    t = Start;
    InitCurve(c1, 3);							// pass in number of dimensions
    for (i = 0; i < gManp->iterations; i++)
	{
	gManp->curpass = (int)(i * gManp->totpasses);
	c1[0] = 3 / cosh(t);
	c1[1] = t - tanh(t);
	c1[2] = amplitude*sin(frequency*t) + amplitude*sin(frequency*t);
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;

	if (user_data(gManp->GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	if (DisplayCurve(c1, ((DWORD)(i / ColourFactor)% gManp->threshold), (int)CurveWidth, i, 6) < 0)
	    break;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesCurves();
    return 0;
    }

/**************************************************************************
	Tricuspoid
	http://www.3d-meier.de/
***************************************************************************/

int	DoTricuspoid(void)

    {
    int		i, j;
    double	c1[6], t, gap, Start, End, CurveWidth, ColourFactor, a, frequency, amplitude;

    c1[0] = c1[3] = gManp->param[10];	// x
    c1[1] = c1[4] = gManp->param[11];	// y
    c1[2] = c1[5] = gManp->param[12];	// z

    Start = gManp->param[0];
    End = gManp->param[1];
    a = gManp->param[2];
    CurveWidth = gManp->param[3];
    ColourFactor = gManp->param[4];
    amplitude = gManp->param[5];
    frequency = gManp->param[6];
    gManp->totpasses = 10;

    gap = (End - Start) / gManp->iterations;
    t = Start;
    InitCurve(c1, 3);							// pass in number of dimensions
    for (i = 0; i < gManp->iterations; i++)
	{
	gManp->curpass = (int)(i * gManp->totpasses);
	c1[0] = a * (2 * cos(t) + cos(2*t));
	c1[1] = a * (2 * sin(t) - sin(2*t));
	c1[2] = amplitude*sin(frequency*t) + amplitude*sin(frequency*t);
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;

	if (user_data(gManp->GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	if (DisplayCurve(c1, ((DWORD)(i / ColourFactor)% gManp->threshold), (int)CurveWidth, i, 6) < 0)
	    break;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesCurves();
    return 0;
    }

/**************************************************************************
	Trifolium
	https://www.mathcurve.com/courbes2d.gb/ornementales/ornementales.shtml
***************************************************************************/

int	DoTrifolium(void)

    {
    int		i, j;
    double	c1[6], t, gap, Start, End, CurveWidth, ColourFactor, a, b, r, frequency, amplitude;

    c1[0] = c1[3] = gManp->param[10];	// x
    c1[1] = c1[4] = gManp->param[11];	// y
    c1[2] = c1[5] = gManp->param[12];	// z

    Start = gManp->param[0];
    End = gManp->param[1];
    a = gManp->param[2];
    b = gManp->param[3];
    CurveWidth = gManp->param[4];
    ColourFactor = gManp->param[5];
    amplitude = gManp->param[6];
    frequency = gManp->param[7];
    gManp->totpasses = 10;

    gap = (End - Start) / gManp->iterations;
    t = Start;
    InitCurve(c1, 3);							// pass in number of dimensions
    for (i = 0; i < gManp->iterations; i++)
	{
	gManp->curpass = (int)(i * gManp->totpasses);
	r = 1 + cos(b * t) + sin(b * t) * sin(b * t);
	c1[0] = r * cos(t);
	c1[1] = r * sin(t);
	c1[2] = amplitude*sin(frequency*t) + amplitude*sin(frequency*t);
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;

	if (user_data(gManp->GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	if (DisplayCurve(c1, ((DWORD)(i / ColourFactor)% gManp->threshold), (int)CurveWidth, i, 6) < 0)
	    break;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesCurves();
    return 0;
    }

/**************************************************************************
	Trigonometric Curve-II - based on Rhodonea
	http://www-groups.dcs.st-and.ac.uk/~history/Curves/Rhodonea.html
***************************************************************************/

int	DoTrigonometricCurveII(void)

    {
    int		i, j;
    double	c1[6], t, gap, Start, End, CurveWidth, ColourFactor, a, k, r, frequency, amplitude;

    c1[0] = c1[3] = gManp->param[10];	// x
    c1[1] = c1[4] = gManp->param[11];	// y
    c1[2] = c1[5] = gManp->param[12];	// z

    Start = gManp->param[0];
    End = gManp->param[1];
    a = gManp->param[2];
    k = gManp->param[3];
    CurveWidth = gManp->param[4];
    ColourFactor = gManp->param[5];
    amplitude = gManp->param[6];
    frequency = gManp->param[7];
    gManp->totpasses = 10;

    gap = (End - Start) / gManp->iterations;
    t = Start;
    InitCurve(c1, 3);							// pass in number of dimensions
    for (i = 0; i < gManp->iterations; i++)
	{
	gManp->curpass = (int)(i * gManp->totpasses);
	r = a * sin(t * k);
	c1[0] = r * cos(t) + sin(t);
	c1[1] = r * sin(t) * cos(t);
	c1[2] = amplitude*sin(frequency*t) + amplitude*sin(frequency*t);
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;

	if (user_data(gManp->GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	if (DisplayCurve(c1, ((DWORD)(i / ColourFactor)% gManp->threshold), (int)CurveWidth, i, 6) < 0)
	    break;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesCurves();
    return 0;
    }

/**************************************************************************
	Trigonometric Curve-III - based on Rhodonea
	http://www-groups.dcs.st-and.ac.uk/~history/Curves/Rhodonea.html
***************************************************************************/

int	DoTrigonometricCurveIII(void)

    {
    int		i, j;
    double	c1[6], t, gap, Start, End, CurveWidth, ColourFactor, a, k, r, frequency, amplitude;

    c1[0] = c1[3] = gManp->param[10];	// x
    c1[1] = c1[4] = gManp->param[11];	// y
    c1[2] = c1[5] = gManp->param[12];	// z

    Start = gManp->param[0];
    End = gManp->param[1];
    a = gManp->param[2];
    k = gManp->param[3];
    CurveWidth = gManp->param[4];
    ColourFactor = gManp->param[5];
    amplitude = gManp->param[6];
    frequency = gManp->param[7];
    gManp->totpasses = 10;

    gap = (End - Start) / gManp->iterations;
    t = Start;
    InitCurve(c1, 3);							// pass in number of dimensions
    for (i = 0; i < gManp->iterations; i++)
	{
	gManp->curpass = (int)(i * gManp->totpasses);
	r = a * sin(t * k);
	c1[0] = r * cos(t) - sin(t);
	c1[1] = r * sin(t) - cos(t);
	c1[2] = amplitude*sin(frequency*t) + amplitude*sin(frequency*t);
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;

	if (user_data(gManp->GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	if (DisplayCurve(c1, ((DWORD)(i / ColourFactor)% gManp->threshold), (int)CurveWidth, i, 6) < 0)
	    break;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesCurves();
    return 0;
    }

/**************************************************************************
	Trigonometric Curve-IV - based on Rhodonea
	http://www-groups.dcs.st-and.ac.uk/~history/Curves/Rhodonea.html
***************************************************************************/

int	DoTrigonometricCurveIV(void)

    {
    int		i, j;
    double	c1[6], t, gap, Start, End, CurveWidth, ColourFactor, a, k, r, frequency, amplitude;

    c1[0] = c1[3] = gManp->param[10];	// x
    c1[1] = c1[4] = gManp->param[11];	// y
    c1[2] = c1[5] = gManp->param[12];	// z

    Start = gManp->param[0];
    End = gManp->param[1];
    a = gManp->param[2];
    k = gManp->param[3];
    CurveWidth = gManp->param[4];
    ColourFactor = gManp->param[5];
    amplitude = gManp->param[6];
    frequency = gManp->param[7];
    gManp->totpasses = 10;

    gap = (End - Start) / gManp->iterations;
    t = Start;
    InitCurve(c1, 3);							// pass in number of dimensions
    for (i = 0; i < gManp->iterations; i++)
	{
	gManp->curpass = (int)(i * gManp->totpasses);
	r = a * sin(t * k);
	c1[0] = r * cos(t) - sin(t);
	c1[1] = r * sin(t) + cos(t);
	c1[2] = amplitude*sin(frequency*t) + amplitude*sin(frequency*t);
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;

	if (user_data(gManp->GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	if (DisplayCurve(c1, ((DWORD)(i / ColourFactor)% gManp->threshold), (int)CurveWidth, i, 6) < 0)
	    break;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesCurves();
    return 0;
    }

/**************************************************************************
	Trigonometric Curve-V - based on Rhodonea
	http://www-groups.dcs.st-and.ac.uk/~history/Curves/Rhodonea.html
***************************************************************************/

int	DoTrigonometricCurveV(void)

    {
    int		i, j;
    double	c1[6], t, gap, Start, End, CurveWidth, ColourFactor, a, k, r, frequency, amplitude;

    c1[0] = c1[3] = gManp->param[10];	// x
    c1[1] = c1[4] = gManp->param[11];	// y
    c1[2] = c1[5] = gManp->param[12];	// z

    Start = gManp->param[0];
    End = gManp->param[1];
    a = gManp->param[2];
    k = gManp->param[3];
    CurveWidth = gManp->param[4];
    ColourFactor = gManp->param[5];
    amplitude = gManp->param[6];
    frequency = gManp->param[7];
    gManp->totpasses = 10;

    gap = (End - Start) / gManp->iterations;
    t = Start;
    InitCurve(c1, 3);							// pass in number of dimensions
    for (i = 0; i < gManp->iterations; i++)
	{
	gManp->curpass = (int)(i * gManp->totpasses);
	r = a * sin(t * k);
	c1[0] = r * cos(t) * sin(t);
	c1[1] = r * sin(t) / cos(t);
	c1[2] = amplitude*sin(frequency*t) + amplitude*sin(frequency*t);
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;

	if (user_data(gManp->GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	if (DisplayCurve(c1, ((DWORD)(i / ColourFactor)% gManp->threshold), (int)CurveWidth, i, 6) < 0)
	    break;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesCurves();
    return 0;
    }

/**************************************************************************
	Trigonometric Curve-VI - based on Rhodonea
	http://www-groups.dcs.st-and.ac.uk/~history/Curves/Rhodonea.html
***************************************************************************/

int	DoTrigonometricCurveVI(void)

    {
    int		i, j;
    double	c1[6], t, gap, Start, End, CurveWidth, ColourFactor, a, k, r, frequency, amplitude;

    c1[0] = c1[3] = gManp->param[10];	// x
    c1[1] = c1[4] = gManp->param[11];	// y
    c1[2] = c1[5] = gManp->param[12];	// z

    Start = gManp->param[0];
    End = gManp->param[1];
    a = gManp->param[2];
    k = gManp->param[3];
    CurveWidth = gManp->param[4];
    ColourFactor = gManp->param[5];
    amplitude = gManp->param[6];
    frequency = gManp->param[7];
    gManp->totpasses = 10;

    gap = (End - Start) / gManp->iterations;
    t = Start;
    InitCurve(c1, 3);							// pass in number of dimensions
    for (i = 0; i < gManp->iterations; i++)
	{
	gManp->curpass = (int)(i * gManp->totpasses);
	r = a * sin(t * k);
	c1[0] = r * cos(t) / sin(t);
	c1[1] = r * sin(t) - cos(t);
	c1[2] = amplitude*sin(frequency*t) + amplitude*sin(frequency*t);
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;

	if (user_data(gManp->GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	if (DisplayCurve(c1, ((DWORD)(i / ColourFactor)% gManp->threshold), (int)CurveWidth, i, 6) < 0)
	    break;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesCurves();
    return 0;
    }

/**************************************************************************
	Trigonometric Curve-VII - based on Rhodonea
	http://www-groups.dcs.st-and.ac.uk/~history/Curves/Rhodonea.html
***************************************************************************/

int	DoTrigonometricCurveVII(void)

    {
    int		i, j;
    double	c1[6], t, gap, Start, End, CurveWidth, ColourFactor, a, k, r, frequency, amplitude;

    c1[0] = c1[3] = gManp->param[10];	// x
    c1[1] = c1[4] = gManp->param[11];	// y
    c1[2] = c1[5] = gManp->param[12];	// z

    Start = gManp->param[0];
    End = gManp->param[1];
    a = gManp->param[2];
    k = gManp->param[3];
    CurveWidth = gManp->param[4];
    ColourFactor = gManp->param[5];
    amplitude = gManp->param[6];
    frequency = gManp->param[7];
    gManp->totpasses = 10;

    gap = (End - Start) / gManp->iterations;
    t = Start;
    InitCurve(c1, 3);							// pass in number of dimensions
    for (i = 0; i < gManp->iterations; i++)
	{
	gManp->curpass = (int)(i * gManp->totpasses);
	r = a * sin(t * k);
	c1[0] = r * cos(t) / sin(t);
	c1[1] = r * sin(t) / cos(t);
	c1[2] = amplitude*sin(frequency*t) + amplitude*sin(frequency*t);
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;

	if (user_data(gManp->GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	if (DisplayCurve(c1, ((DWORD)(i / ColourFactor)% gManp->threshold), (int)CurveWidth, i, 6) < 0)
	    break;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesCurves();
    return 0;
    }

/**************************************************************************
	Trigonometric Curve-VIII - based on Rhodonea
	http://www-groups.dcs.st-and.ac.uk/~history/Curves/Rhodonea.html
***************************************************************************/

int	DoTrigonometricCurveVIII(void)

    {
    int		i, j;
    double	c1[6], t, gap, Start, End, CurveWidth, ColourFactor, a, k, r, frequency, amplitude;

    c1[0] = c1[3] = gManp->param[10];	// x
    c1[1] = c1[4] = gManp->param[11];	// y
    c1[2] = c1[5] = gManp->param[12];	// z

    Start = gManp->param[0];
    End = gManp->param[1];
    a = gManp->param[2];
    k = gManp->param[3];
    CurveWidth = gManp->param[4];
    ColourFactor = gManp->param[5];
    amplitude = gManp->param[6];
    frequency = gManp->param[7];
    gManp->totpasses = 10;

    gap = (End - Start) / gManp->iterations;
    t = Start;
    InitCurve(c1, 3);							// pass in number of dimensions
    for (i = 0; i < gManp->iterations; i++)
	{
	gManp->curpass = (int)(i * gManp->totpasses);
	r = a * sin(t * k);
	c1[0] = r * cos(t) - sin(t);
	c1[1] = r * sin(t) - cos(t);
	c1[2] = amplitude*sin(frequency*t) + amplitude*sin(frequency*t);
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;

	if (user_data(gManp->GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	if (DisplayCurve(c1, ((DWORD)(i / ColourFactor)% gManp->threshold), (int)CurveWidth, i, 6) < 0)
	    break;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesCurves();
    return 0;
    }

/**************************************************************************
	Trigonometric Curve-IX - based on Rhodonea
	http://www-groups.dcs.st-and.ac.uk/~history/Curves/Rhodonea.html
***************************************************************************/

int	DoTrigonometricCurveIX(void)

    {
    int		i, j;
    double	c1[6], t, gap, Start, End, CurveWidth, ColourFactor, a, k, r, frequency, amplitude;

    c1[0] = c1[3] = gManp->param[10];	// x
    c1[1] = c1[4] = gManp->param[11];	// y
    c1[2] = c1[5] = gManp->param[12];	// z

    Start = gManp->param[0];
    End = gManp->param[1];
    a = gManp->param[2];
    k = gManp->param[3];
    CurveWidth = gManp->param[4];
    ColourFactor = gManp->param[5];
    amplitude = gManp->param[6];
    frequency = gManp->param[7];
    gManp->totpasses = 10;

    gap = (End - Start) / gManp->iterations;
    t = Start;
    InitCurve(c1, 3);							// pass in number of dimensions
    for (i = 0; i < gManp->iterations; i++)
	{
	gManp->curpass = (int)(i * gManp->totpasses);
	r = a * sin(t * k);
	c1[0] = r * cos(t) - sin(t);
	c1[1] = r * cos(t) + cos(t);
	c1[2] = amplitude*sin(frequency*t) + amplitude*sin(frequency*t);
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;

	if (user_data(gManp->GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	if (DisplayCurve(c1, ((DWORD)(i / ColourFactor)% gManp->threshold), (int)CurveWidth, i, 6) < 0)
	    break;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesCurves();
    return 0;
    }

/**************************************************************************
	Trigonometric Curve-X - based on Rhodonea
	http://www-groups.dcs.st-and.ac.uk/~history/Curves/Rhodonea.html
***************************************************************************/

int	DoTrigonometricCurveX(void)

    {
    int		i, j;
    double	c1[6], t, gap, Start, End, CurveWidth, ColourFactor, a, k, r, frequency, amplitude;

    c1[0] = c1[3] = gManp->param[10];	// x
    c1[1] = c1[4] = gManp->param[11];	// y
    c1[2] = c1[5] = gManp->param[12];	// z

    Start = gManp->param[0];
    End = gManp->param[1];
    a = gManp->param[2];
    k = gManp->param[3];
    CurveWidth = gManp->param[4];
    ColourFactor = gManp->param[5];
    amplitude = gManp->param[6];
    frequency = gManp->param[7];
    gManp->totpasses = 10;

    gap = (End - Start) / gManp->iterations;
    t = Start;
    InitCurve(c1, 3);							// pass in number of dimensions
    for (i = 0; i < gManp->iterations; i++)
	{
	gManp->curpass = (int)(i * gManp->totpasses);
	r = a * sin(t * k);
	c1[0] = r * cos(t) / sin(t);
	c1[1] = r * cos(t) + cos(t);
	c1[2] = amplitude*sin(frequency*t) + amplitude*sin(frequency*t);
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;

	if (user_data(gManp->GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	if (DisplayCurve(c1, ((DWORD)(i / ColourFactor)% gManp->threshold), (int)CurveWidth, i, 6) < 0)
	    break;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesCurves();
    return 0;
    }

/**************************************************************************
	Trisectrix of Maclaurin
	http://www-groups.dcs.st-andrews.ac.uk/~history/Curves/Trisectrix.html
***************************************************************************/

int	DoTrisectrix(void)

    {
    int		i, j;
    double	c1[6], t, gap, Start, End, CurveWidth, ColourFactor, a, r, frequency, amplitude;

    c1[0] = c1[3] = gManp->param[10];	// x
    c1[1] = c1[4] = gManp->param[11];	// y
    c1[2] = c1[5] = gManp->param[12];	// z

    Start = gManp->param[0];
    End = gManp->param[1];
    a = gManp->param[2];
    CurveWidth = gManp->param[3];
    ColourFactor = gManp->param[4];
    amplitude = gManp->param[5];
    frequency = gManp->param[6];
    gManp->totpasses = 10;

    gap = (End - Start) / gManp->iterations;
    t = Start;
    InitCurve(c1, 3);							// pass in number of dimensions
    for (i = 0; i < gManp->iterations; i++)
	{
	gManp->curpass = (int)(i * gManp->totpasses);
	r = a / cos(t/3);    
	c1[0] = r * cos(t);
	c1[1] = r * sin(t);
	c1[2] = amplitude*sin(frequency*t) + amplitude*sin(frequency*t);
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;

	if (user_data(gManp->GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	if (DisplayCurve(c1, ((DWORD)(i / ColourFactor)% gManp->threshold), (int)CurveWidth, i, 6) < 0)
	    break;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesCurves();
    return 0;
    }

/**************************************************************************
	Tschirnhausen Cubic
	http://mathworld.wolfram.com/TschirnhausenCubic.html
***************************************************************************/

int	DoTschirnhausenCubic(void)

    {
    int		i, j;
    double	c1[6], t, gap, Start, End, CurveWidth, ColourFactor, a, frequency, amplitude;

    c1[0] = c1[3] = gManp->param[10];	// x
    c1[1] = c1[4] = gManp->param[11];	// y
    c1[2] = c1[5] = gManp->param[12];	// z

    Start = gManp->param[0];
    End = gManp->param[1];
    a = gManp->param[2];
    CurveWidth = gManp->param[3];
    ColourFactor = gManp->param[4];
    amplitude = gManp->param[5];
    frequency = gManp->param[6];
    gManp->totpasses = 10;

    gap = (End - Start) / gManp->iterations;
    t = Start;
    InitCurve(c1, 3);							// pass in number of dimensions
    for (i = 0; i < gManp->iterations; i++)
	{
	gManp->curpass = (int)(i * gManp->totpasses);
	c1[0] = a * (1 - 3 * t * t);
	c1[1] = a * t * (3 - t * t);
	c1[2] = amplitude*sin(frequency*t) + amplitude*sin(frequency*t);
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;

	if (user_data(gManp->GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	if (DisplayCurve(c1, ((DWORD)(i / ColourFactor)% gManp->threshold), (int)CurveWidth, i, 6) < 0)
	    break;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesCurves();
    return 0;
    }

/**************************************************************************
	Viviani Curve
	http://www.wolframalpha.com/input/?i=tangent+vector+Viviani%27s+curve
***************************************************************************/

int	DoVivianiCurve(void)

    {
    int		i, j;
    double	c1[6], t, gap, Start, End, CurveWidth, ColourFactor, a, f, frequency, amplitude;

    c1[0] = c1[3] = gManp->param[10];	// x
    c1[1] = c1[4] = gManp->param[11];	// y
    c1[2] = c1[5] = gManp->param[12];	// z

    Start = gManp->param[0];
    End = gManp->param[1];
    a = gManp->param[2];
    CurveWidth = gManp->param[3];
    ColourFactor = gManp->param[4];
    amplitude = gManp->param[5];
    frequency = gManp->param[6];
    gManp->totpasses = 10;

    gap = (End - Start) / gManp->iterations;
    t = Start;
    InitCurve(c1, 3);							// pass in number of dimensions
    for (i = 0; i < gManp->iterations; i++)
	{
	gManp->curpass = (int)(i * gManp->totpasses);
	f = sqrt(3 + cos(t));
	c1[0] = -a * sin(t) / f;
	c1[1] = a * cos(t) / f;
	c1[2] = a * cos(t / 2) / f;
	for (j = 0; j < 3; j++)
	    c1[j + 3] += c1[j] * dt;

	if (user_data(gManp->GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	if (DisplayCurve(c1, ((DWORD)(i / ColourFactor)% gManp->threshold), (int)CurveWidth, i, 6) < 0)
	    break;
	t += gap;
	}
    PlotExtras();
    CloseZvaluesCurves();
    return 0;
    }

/**************************************************************************
	Close z value memory
***************************************************************************/

void	CloseZvaluesCurves(void)
    {
    return gManp->OscProcess.CloseZvalues();
    }

/**************************************************************************
	Common init Routines
***************************************************************************/

void	InitCurve(double c1[], int dimensions)
    {
    gManp->OscProcess.InitOscProc(dimensions, gManp->mandel_width, gManp->AspectRatio, gManp->xdots, gManp->ydots, gManp->type, gManp->subtype, gManp->OscAnimProc, &gManp->xAxis, &gManp->yAxis, &gManp->zAxis, BOUNDARY, &gManp->Dib, c1,
		gManp->PerspectiveFlag, gManp->iterations, gManp->OscBackGround, &gManp->x_rot, &gManp->y_rot, &gManp->z_rot, &RemoveHiddenPixels, gManp->GlobalHwnd, &gManp->TrueCol);
    }

/**************************************************************************
	Common Display Routines
***************************************************************************/

int	DisplayCurve(double c1[], DWORD colour, int CurveWidth, double i, int dimensions)
    {
    return gManp->OscProcess.DisplayOscillator(c1, c1, 0.0, colour, i, dimensions, FALSE, 1, 1, gManp->hor, gManp->vert, VertBias, zBias, gManp->OscAnimProc, gManp->CoordSystem, gManp->threshold, 0.0, CurveWidth);
    }

/**************************************************************************
	Compare function for Fractal Map name sort
**************************************************************************/

int	CompareCurve(int	*p1, int *p2)

    {
    char    *a1, *a2;

    a1 = CurveSpecific[*p1].name;
    a2 = CurveSpecific[*p2].name;
    if (*a1 == '\"')			    // ignore " character
	a1 = a1++;
    if (*a2 == '\"')
	a2 = a2++;
    return (_stricmp(a1, a2));
    }

/**************************************************************************
	General Curve Routines
***************************************************************************/

int	setup_Curve(void)
    {
    int	    i;

    OscillatorNum = 0;
			// we'd better count how many records we have
    while (CurveSpecific[OscillatorNum].name)
	OscillatorNum++;
    for (i = 0; i < OscillatorNum; i++) 
	OscPtrArray[i] = i;
    qsort((void *)OscPtrArray, (size_t)OscillatorNum, sizeof(OscPtrArray[0]), (int (__cdecl *)(const void *, const void *)) CompareCurve);
#ifdef	PRINTOSC
    PrintOsc(OscillatorNum);		// used for listing oscillator names in d:\temp\OscDump.txt
#endif
    return 0;
    }

int	DoCurves(void) 
    {
    return (CurveSpecific[gManp->subtype].calctype());
    }


