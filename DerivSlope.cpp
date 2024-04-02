#include "FrameCalculator.h"

void	calculateFrame::BigProcessDerivativeSlope(ExpComplex ExpDC, ExpComplex ExpTemp, HANDLE ghMutex, long iteration, CTrueCol *TrueCol, int x, int y)
    {
    BigComplex	BigDC = 0.0;
    BigComplex	BigU;
    BigDouble	BigReflection;
    double	reflection;

    if (subtype == 57)					// polynomial
	LightHeight = 2.0;					// param 1 is used for a polynomial coefficient, so use a default
    else
	LightHeight = param[1];				// height 
    LightHeight *= 1.333;					// height * 1.333 : seems to need a bit more height to get similar images to double float
    BigComplex	BigZ;

    BigDC = ExpDC;						// upgrade to BigDouble for accuracy. As we only do it once per pixel, we can afford the overhead.
    BigZ = ExpTemp;

    BigU = BigZ / BigDC;
    BigU = BigU / BigU.CFabs();
    BigReflection = BigU.x * v.x + BigU.y * v.y + LightHeight;
    reflection = BigReflection.BigDoubleToDouble();
    ProcessDerivativeSlopeMain(reflection, ghMutex, iteration, TrueCol, x, y);
    }

void	calculateFrame::ProcessDerivativeSlope(Complex dc, Complex z, HANDLE ghMutex, long iteration, CTrueCol *TrueCol, int x, int y)
    {
    BigComplex	BigDC = 0.0;
    BigComplex	BigU;
    BigDouble	BigReflection;
    Complex	u;
    double	reflection;

    if (subtype == 57)						// polynomial
	LightHeight = 2.0;						// param 1 is used for a polynomial coefficient, so use a default
    else
	LightHeight = param[1];					// height 
    if (precision > MAXDERPRECISION)				// beyond MAXDERPRECISION precision, upgrade Slope calcs to BigDouble for accuracy.
	{
	BigComplex	BigZ;

	LightHeight *= 1.333;					// seems to need a bit more height to get similar images to double float
	BigDC = dc;							// upgrade to BigDouble for accuracy. As we only do it once per pixel, we can afford the overhead.
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
    ProcessDerivativeSlopeMain(reflection, ghMutex, iteration, TrueCol, x, y);
    }

void	calculateFrame::ProcessDerivativeSlopeMain(double reflection, HANDLE ghMutex, long iteration, CTrueCol *TrueCol, int x, int y)
    {
    BYTE	b;
    RGBTRIPLE	colour;
    DWORD	local_width, address;

    reflection = reflection / (1.0 + LightHeight);			// rescale so that t does not get bigger than 1
    if (reflection < 0.0) reflection = 0.0;
    //	    if (reflection > 1.0) reflection = 1.0;
    if (ghMutex != NULL)
	WaitForSingleObject(ghMutex, INFINITE);			// no time-out interval
    if (iteration >= MaxIteration)
	{
	colour.rgbtRed = (BYTE)TrueCol->InsideRed;			// M_waves
	colour.rgbtGreen = (BYTE)TrueCol->InsideGreen;
	colour.rgbtBlue = (BYTE)TrueCol->InsideBlue;
	}
    else // exterior of Mandelbrot set = normal 
	{
	b = (BYTE)(255.0 * reflection);
	if (iteration >= PaletteStart)
	    {
	    if (iteration > 500)
		int qwerty = 1;
	    colour.rgbtRed = (BYTE)((reflection)   * (double)(*(TrueCol->PalettePtr + ((iteration) % TrueCol->ColoursInPALFile) * 3 + 0))); // Red
	    colour.rgbtGreen = (BYTE)((reflection) * (double)(*(TrueCol->PalettePtr + ((iteration) % TrueCol->ColoursInPALFile) * 3 + 1))); // Green
	    colour.rgbtBlue = (BYTE)((reflection)  * (double)(*(TrueCol->PalettePtr + ((iteration) % TrueCol->ColoursInPALFile) * 3 + 2))); // Blue 
	    }
	else
	    {
	    colour.rgbtRed = b;
	    colour.rgbtGreen = b;
	    colour.rgbtBlue = b;
	    }
	}
	    // plot the point
    local_width = WIDTHBYTES((DWORD)Dib->DibWidth * (DWORD)Dib->BitsPerPixel);
    address = ((DWORD)(/*height - 1 - */y) * (DWORD)(local_width + 3 - ((local_width - 1) % 4)) + (DWORD)((thread * width + x) * 3));
    memcpy(Dib->DibPixels + address, &colour, 3);
    if (ghMutex != NULL)
	ReleaseMutex(ghMutex);
    //	return 0;
    }

//////////////////////////////////////////////////////////////////////
// Calculate the derivative slope
//////////////////////////////////////////////////////////////////////

void	calculateFrame::CalculateDerivativeSlope(Complex *dc, Complex z)
    {
    Complex temp;
    int	k;

    if (subtype == 0)				// Mandelbrot
	{
	*dc = z * 2 * *dc + 1.0;
	}
    else if (subtype == 1)			// Power
	{
	temp = 1.0;
	for (k = 0; k < (int)power - 1; k++)
	    temp *= z;
	*dc = temp * *dc * power + 1.0;	// z^k --> k a z^(k - 1)
	}
    else if (subtype == 10)			// Tricorn
						// z -> (z*)^2 + c, which yields the so-called tricorn fractal shown below.
	{
	Complex	conjugate = z;
	conjugate.y = -conjugate.y;
	*dc = conjugate * 2 * *dc + 1.0;
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
	Complex	Tenth, Nineth, Eighth, Seventh, Sixth, Quintic, Quartic, Cubic, Square;
	Square = z;
	Cubic = Square * z;
	Quartic = Cubic * z;
	Quintic = Quartic * z;
	Sixth = Quintic * z;
	Seventh = Sixth * z;
	Eighth = Seventh * z;
	Nineth = Eighth * z;
	Tenth = Nineth * z;
	Tenth *= param[0];
	Nineth *= param[1];
	Eighth *= param[2];
	Seventh *= param[3];
	Sixth *= param[4];
	Quintic *= param[5];
	Quartic *= param[6];
	Cubic *= param[7];
	Square *= param[8];
	*dc = (Square * 2 + Cubic * 3 + Quartic * 4 + Quintic * 5 + Sixth * 6 + Seventh * 7 + Eighth * 8 + Nineth * 9 + Tenth * 10) * *dc + param[9] + 1.0;
	}
    }

//////////////////////////////////////////////////////////////////////
// Calculate the derivative slope for floatexp
//////////////////////////////////////////////////////////////////////

void	calculateFrame::BigCalculateDerivativeSlope(ExpComplex *ExpDC, ExpComplex z)
    {
    ExpComplex temp;
    int	k;

    if (subtype == 0)				// Mandelbrot
	*ExpDC = z * 2 * *ExpDC + 1.0;
    else if (subtype == 1)			// Power
	{
	if (power == 3)
	    *ExpDC = z.CSqr() * 3 * *ExpDC + 1.0;
	else
	    {
	    temp = 1.0;			// temp2 = 1.0;

	    for (k = 0; k < (int)param[2] - 1; k++)
		temp *= z;
	    *ExpDC = temp * *ExpDC * param[2] + 1.0;	// z^k --> k a z^(k - 1)
	    }
	}
    else if (subtype == 57)			// Polynomial
						// z^k --> k a z^(k -1)
	{
	ExpComplex	ExpTenth, ExpNineth, ExpEighth, ExpSeventh, ExpSixth, ExpQuintic, ExpQuartic, ExpCubic, ExpSquare;
	ExpSquare = z;
	ExpCubic = ExpSquare * z;
	ExpQuartic = ExpCubic * z;
	ExpQuintic = ExpQuartic * z;
	ExpSixth = ExpQuintic * z;
	ExpSeventh = ExpSixth * z;
	ExpEighth = ExpSeventh * z;
	ExpNineth = ExpEighth * z;
	ExpTenth = ExpNineth * z;
	ExpTenth = ExpTenth * param[0];
	ExpNineth = ExpNineth * param[1];
	ExpEighth = ExpEighth * param[2];
	ExpSeventh = ExpSeventh * param[3];
	ExpSixth = ExpSixth * param[4];
	ExpQuintic = ExpQuintic * param[5];
	ExpQuartic = ExpQuartic * param[6];
	ExpCubic = ExpCubic * param[7];
	ExpSquare = ExpSquare * param[8];
	*ExpDC = (ExpSquare * 2 + ExpCubic * 3 + ExpQuartic * 4 + ExpQuintic * 5 + ExpSixth * 6 + ExpSeventh * 7 + ExpEighth * 8 + ExpNineth * 9 + ExpTenth * 10) * *ExpDC + param[9] + 1.0;
	}
    }
