/*
    DerivSlope.cpp - Functions to implement derivative slope in Perturbation

    Written in Microsoft Visual 'C++' by Paul de Leeuw.
*/

#include "PertEngine.h"
#include "Manp.h"

void	CPerturbation::BigProcessDerivativeSlope(ExpComplex ExpDC, ExpComplex ExpTemp, HANDLE ghMutex, long MaxIteration, long iteration, CTrueCol &TrueCol, int x, int y)
    {
    BigComplex	BigDC = 0.0;
    BigComplex	BigU;
    BigDouble	BigReflection;
    double	reflection;

    if (subtype == 57)
	LightHeight = param[6];					// height 
    else
	LightHeight = param[9];					// height 
    LightHeight *= 1.333;					// height * 1.333 : seems to need a bit more height to get similar images to double float
    BigComplex	BigZ;

    BigDC = ExpDC;						// upgrade to BigDouble for accuracy. As we only do it once per pixel, we can afford the overhead.
    BigZ = ExpTemp;

    BigU = BigZ / BigDC;
    BigU = BigU / BigU.CFabs();
    BigReflection = BigU.x * v.x + BigU.y * v.y + LightHeight;
    reflection = BigReflection.BigDoubleToDouble();
    ProcessDerivativeSlopeMain(BigZ.CBig2Double(), reflection, ghMutex, MaxIteration, iteration, TrueCol, x, y);
    }

void	CPerturbation::ProcessDerivativeSlope(Complex dc, Complex z, HANDLE ghMutex, long MaxIteration, long iteration, CTrueCol &TrueCol, int x, int y)
    {
    BigComplex	BigDC = 0.0;
    BigComplex	BigU;
    BigDouble	BigReflection;
    Complex	u;
    double	reflection;

    if (subtype == 57)
	LightHeight = param[6];					// height 
    else
	LightHeight = param[9];					// height 
    if (precision > MAXDERPRECISION)				// beyond MAXDERPRECISION precision, upgrade Slope calcs to BigDouble for accuracy.
	{
	BigComplex	BigZ;

	LightHeight *= 1.333;					// seems to need a bit more height to get similar images to double float
	BigDC = dc;						// upgrade to BigDouble for accuracy. As we only do it once per pixel, we can afford the overhead.
	BigZ = z;
	BigU = BigZ / BigDC;
	BigU = BigU / BigU.CFabs();
	BigReflection = BigU.x * v.x + BigU.y * v.y + LightHeight;
	reflection = BigReflection.BigDoubleToDouble();
	}
    else
	{
	u = z / dc;
	u = u / u.CFabs();
	reflection = u.x * v.x + u.y * v.y + LightHeight;
	}
    ProcessDerivativeSlopeMain(z, reflection, ghMutex, MaxIteration, iteration, TrueCol, x, y);
    }

void	CPerturbation::ProcessDerivativeSlopeMain(Complex z, double reflection, HANDLE ghMutex, long MaxIteration, long iteration, CTrueCol &TrueCol, int x, int y)
    {
    RGBTRIPLE	colour;

    CPlot plot;								// binds to global wpixels by default
    plot.InitPlot(MaxIteration, &TrueCol, &gManp->wpixels, Dib->DibWidth, Dib->DibHeight, Dib->DibWidth, Dib->DibHeight, Dib->BitsPerPixel, Dib, USEPALETTE | USEWPIXELS);

    reflection = reflection / (1.0 + LightHeight);			// rescale so that t does not get bigger than 1
    if (reflection < 0.0) reflection = 0.0;
    if (ghMutex != NULL)
	WaitForSingleObject(ghMutex, INFINITE);				// no time-out interval
    if (iteration >= MaxIteration)
	{
	colour.rgbtRed = (BYTE)TrueCol.InsideRed;			// M_waves
	colour.rgbtGreen = (BYTE)TrueCol.InsideGreen;
	colour.rgbtBlue = (BYTE)TrueCol.InsideBlue;
	}
    else // exterior of Mandelbrot set = normal 
	{
	if (iteration >= PaletteStart)
	    {
	    if (smoothing)
		{
		double mag2 = z.x*z.x + z.y*z.y;			// done up top
		double log_zn = log(mag2) * 0.5;			// log(|z|)
		double nu = log(log_zn / log(SlopeDegree)) / log(SlopeDegree);
		double FloatIteration = iteration + 1 - nu + PalOffset;
		colour = GetSmoothedColour(FloatIteration, ColourSpeed, TrueCol, &plot);
		}
	    else
		plot.GetRGB((iteration + PalOffset) % TrueCol.ColoursInPALFile, &colour);
	    colour.rgbtRed = (BYTE)(colour.rgbtRed * reflection);
	    colour.rgbtGreen = (BYTE)(colour.rgbtGreen * reflection);
	    colour.rgbtBlue = (BYTE)(colour.rgbtBlue * reflection);
	    }
	else
	    {
	    BYTE preRed = (BYTE)((gManp->PrePaletteColour >> 16) & 0xFF);
	    BYTE preGreen = (BYTE)((gManp->PrePaletteColour >> 8) & 0xFF);
	    BYTE preBlue = (BYTE)(gManp->PrePaletteColour & 0xFF);

	    colour.rgbtRed = (BYTE)(preRed   * reflection);
	    colour.rgbtGreen = (BYTE)(preGreen * reflection);
	    colour.rgbtBlue = (BYTE)(preBlue  * reflection);
	    }
	}
	    // plot the point
    plot.OutRGBpoint((WORD)x/* + xStart*/, (WORD)height - 1 - y, colour);

    if (ghMutex != NULL)
	ReleaseMutex(ghMutex);
    }

//////////////////////////////////////////////////////////////////////
// Calculate the derivative slope
//////////////////////////////////////////////////////////////////////

void	CPerturbation::CalculateDerivativeSlope(Complex &dc, Complex z)
    {
    Complex temp;
    int	k;

    if (subtype == 0)				// Mandelbrot
	{
	dc = z * 2 * dc + 1.0;
	}
    else if (subtype == 1)			// Power
	{
	temp = 1.0;
	for (k = 0; k < (int)power - 1; k++)
	    temp *= z;
	dc = temp * dc * power + 1.0;	// z^k --> k a z^(k - 1)
	}
    else if (subtype == 10)			// Tricorn
						// z -> (z*)^2 + c, which yields the so-called tricorn fractal shown below.
	{
	// Tricorn derivative slope requires a 2×2 real Jacobian; the ordinary complex-derivative slope path is not applicable.
	Complex	conjugate = z;
	conjugate.y = -conjugate.y;
	dc = conjugate * 2 * dc + 1.0;
	//		dc = temp1 * 2 * dc + 1.0;
	//		dc.y = dc.y;
	//		conjugate.y = -conjugate.y;
	//		dc = conjugate * dc * 2 + 1.0;
	//		dc.x = (conjugate.x - conjugate.y) * 2 * dc.x + 1.0;
	//		dc.y = -2 * (conjugate.x + conjugate.y) * dc.y;
	}
    else if (subtype == 57)			// Polynomial
						// z^k --> k a z^(k - 1)
	{
	Complex	Eighth, Seventh, Sixth, Quintic;
	Complex	Quartic, Cubic, Square;

	Square = z;
	Cubic = Square * z;
	Quartic = Cubic * z;
	Quintic = Quartic * z;
	Sixth = Quintic * z;
	Seventh = Sixth * z;
	Eighth = Seventh * z;

	Eighth *= param[7];
	Seventh *= param[8];
	Sixth *= param[9];
	Quintic *= param[10];
	Quartic *= param[11];
	Cubic *= param[12];
	Square *= param[13];

	dc = (Square * 2 + Cubic * 3 + Quartic * 4 + Quintic * 5 + Sixth * 6 + Seventh * 7 + Eighth * 8) * dc + param[14] + 1.0;
	}
    else if (subtype == 59)			// Exp
	{
	dc = z.CExp() * dc + 1.0;		// d/dz exp(z) = exp(z)
	}
    else if (subtype == 60)			// Sinh
	{
	dc = z.CCosh() * dc + 1.0;		// d/dz sinh(z) = cosh(z)
	dc.y = -dc.y;				// Rotate/negate derivative
	}
    else if (subtype == 61)			// Sin
	{
	dc = z.CCos() * dc + 1.0;		// d/dz sin(z) = cos(z)
	}
    else if (subtype == 62)			// Cos
	{
	dc = (z.CSin() * -1.0) * dc + 1.0;	// d/dz cos(z) = -sin(z)
	}
    else if (subtype == 63)			// Fractional Half Power -> z^(n + 0.5)
	{
	int n = (int)param[10];

	if (n < 1)
	    n = 1;
	if (n > 6)
	    n = 6;

	//--------------------------------------------
	// z^n
	//--------------------------------------------

	Complex ZPow(1.0, 0.0);

	for (k = 0; k < n; k++)
	    ZPow *= z;

	//--------------------------------------------
	// sqrt(z)
	//--------------------------------------------

	Complex sqrtZ = z.CSqrt();

	//--------------------------------------------
	// Transport-style field
	//
	// Instead of strict derivative:
	//
	// (n + 0.5) z^(n-0.5)
	//
	// use the function itself:
	//
	// z^n * sqrt(z)
	//
	// This avoids reciprocal sqrt instability
	// and follows orbit flow more naturally.
	//--------------------------------------------

	Complex transport;
	transport = ZPow;
	dc = transport * dc + 1.0;
	}
    }

//////////////////////////////////////////////////////////////////////
// Calculate the derivative slope for floatexp
//////////////////////////////////////////////////////////////////////

void	CPerturbation::BigCalculateDerivativeSlope(ExpComplex &ExpDC, ExpComplex z)
    {
    ExpComplex temp;
    int	k;

    if (subtype == 0)				// Mandelbrot
	ExpDC = z * 2 * ExpDC + 1.0;
    else if (subtype == 1)			// Power
	{
	if (power == 3)
	    ExpDC = z.CSqr() * 3 * ExpDC + 1.0;
	else
	    {
	    temp = 1.0;			// temp2 = 1.0;

	    for (k = 0; k < (int)param[10] - 1; k++)
		temp *= z;
	    ExpDC = temp * ExpDC * param[10] + 1.0;	// z^k --> k a z^(k - 1)
	    }
	}
    else if (subtype == 57)			// Polynomial
						// z^k --> k a z^(k - 1)
	{
	ExpComplex ExpEighth, ExpSeventh, ExpSixth, ExpQuintic;
	ExpComplex ExpQuartic, ExpCubic, ExpSquare;

	ExpSquare = z;
	ExpCubic = ExpSquare * z;
	ExpQuartic = ExpCubic * z;
	ExpQuintic = ExpQuartic * z;
	ExpSixth = ExpQuintic * z;
	ExpSeventh = ExpSixth * z;
	ExpEighth = ExpSeventh * z;

	ExpEighth = ExpEighth * param[7];
	ExpSeventh = ExpSeventh * param[8];
	ExpSixth = ExpSixth * param[9];
	ExpQuintic = ExpQuintic * param[10];
	ExpQuartic = ExpQuartic * param[11];
	ExpCubic = ExpCubic * param[12];
	ExpSquare = ExpSquare * param[13];

	ExpDC = (ExpSquare * 2 + ExpCubic * 3 + ExpQuartic * 4 + ExpQuintic * 5 + ExpSixth * 6 + ExpSeventh * 7 + ExpEighth * 8 + param[14]) * ExpDC + 1.0;
	}

    else if (subtype == 10)                 // Tricorn
	{
	ExpComplex conjugate = z;
	conjugate.y = -conjugate.y;
	ExpDC = conjugate * 2 * ExpDC + 1.0;
	}

    // the following need to wait unti we build floatexp trig functions library
    /*
    else if (subtype == 59)                 // Exp
	{
	ExpDC = z.CExp() * ExpDC + 1.0;     // d/dz exp(z) = exp(z)
	}

    else if (subtype == 60)                 // Sinh
	{
	ExpDC = z.CCosh() * ExpDC + 1.0;    // d/dz sinh(z) = cosh(z)
	ExpDC.y = -ExpDC.y;
	}

    else if (subtype == 61)                 // Sin
	{
	ExpDC = z.CCos() * ExpDC + 1.0;     // d/dz sin(z) = cos(z)
	}

    else if (subtype == 62)                 // Cos
	{
	ExpDC = (z.CSin() * -1.0) * ExpDC + 1.0;   // d/dz cos(z) = -sin(z)
	}

    else if (subtype == 63)                 // Fractional Half Power
	{
	int n = (int)param[2];

	if (n < 1)
	    n = 1;
	if (n > 6)
	    n = 6;

	ExpComplex ZPow(1.0, 0.0);

	for (k = 0; k < n; k++)
	    ZPow *= z;

	// Transport-style field, matching the double implementation.
	ExpComplex transport = ZPow;

	ExpDC = transport * ExpDC + 1.0;
	}
    */
    }
