/*
    SLOPE.CPP a module for the per pixel calculations of fractals. 
    Written in Microsoft Visual 'C++' by Paul de Leeuw.
*/

#include	"slope.h"

/***********************************************************************
	The dot product of two vectors a = [a1, a2, ..., an] and b = [b1, b2, ..., bn] is defined as:[1]
	d = a1b1 + a2b2
***********************************************************************/

double CSlope::cdot(Complex a, Complex b)
    {
    return a.x * b.x + a.y * b.y;
    }

/***********************************************************************
	Reflection: fractal calculations Bignum version
***********************************************************************/

double CSlope::GiveBigReflection(Complex j, BYTE juliaflag, BigComplex cBig, int *iterations, double rqlim, long threshold, Complex v)
    {
    int		i = 0; // iteration 
    int		k;
    BigDouble	realimagBig;
    BigComplex	sqrBig, vBig, qBig, tempBig, tempBig1;

    BigComplex	dCBig = 0.0;		// derivative with respect to c 
    BigDouble	BigReflection = 0.0;
    double	reflection = FP_ZERO;	// inside 
    BigComplex	zBig = 0.0;
    BigDouble	h2 = param[1];		// height factor of the incoming light
    BigComplex	u;
    int		degree1, degree2;
    BigComplex	unity = { 1.0, 0.0 };

    vBig = v;

    *degree = (int)param[3];
    if (juliaflag)
	{
	qBig = j;
	zBig = cBig;
	}
    else
	{
	if (subtype == 3)				// Sine
	    zBig = param[4];
	qBig = cBig;
	}
    for (i = 0; i < threshold; i++)
	{
	switch (subtype)
	    {
	    case 0:					// Mandelbrot
		dCBig = dCBig * (zBig + zBig) + 1.0;
		sqrBig.x = zBig.x * zBig.x;
		sqrBig.y = zBig.y * zBig.y;
		realimagBig = zBig.x * zBig.y;
		zBig.x = qBig.x + sqrBig.x - sqrBig.y;
		zBig.y = qBig.y + realimagBig + realimagBig;
		break;
	    case 1:					// Cubic
		dCBig = dCBig * zBig.CSqr() * 3.0 + 1.0;
		zBig = zBig.CCube() + qBig;
		break;
	    case 2:					// Power
		tempBig = 1.0;
		for (k = 0; k < *degree - 1; k++)
		    tempBig *= zBig;
		dCBig = tempBig * dCBig * (double)*degree + 1.0;
		zBig = tempBig * zBig + qBig;
		break;
	    case 3:					// Sin
		if (NumThreads == 0)
		    {
		    dCBig = dCBig * zBig.CCos() + 1.0;
		    if (param[3] == 0)
			zBig = zBig.CSin() * qBig;
		    else
			zBig = zBig.CSin() + qBig;
		    }
		else
		    {
		    BigTrig.CCos(&tempBig, zBig);
		    dCBig = dCBig * tempBig + 1.0;
		    BigTrig.CSin(&tempBig, zBig);
		    if (param[3] == 0)
			zBig = tempBig * qBig;
		    else
			zBig = tempBig + qBig;
		    }
		break;
	    case 4:					// Sin + 1/c
		if (NumThreads == 0)
		    {
		    dCBig = dCBig * zBig.CCos() + 1.0;
		    zBig = zBig.CSin() + unity / qBig;
		    }
		else
		    {
		    BigTrig.CCos(&tempBig, zBig);
		    dCBig = dCBig * tempBig + 1.0;
		    BigTrig.CSin(&tempBig, zBig);
		    zBig = tempBig + unity / qBig;
		    }
		break;
	    case 5:					// exp
		if (NumThreads == 0)
		    {
		    dCBig = dCBig * zBig.CExp() + 1.0;
		    zBig = zBig.CExp() + qBig;
		    }
		else
		    {
		    BigTrig.CExp(&tempBig, zBig);
		    dCBig = dCBig * tempBig + 1.0;
		    zBig = tempBig + qBig;
		    }
		break;
	    case 6:					// Power + 1/c
		tempBig = 1.0;
		for (k = 0; k < *degree - 1; k++)
		    tempBig *= zBig;
		dCBig = tempBig * dCBig * (double)*degree + 1.0;
		zBig = tempBig * zBig + unity / qBig;
		break;
	    case 7:
		degree1 = (int)param[4];
		degree2 = (int)param[5];
		dCBig = dCBig * zBig.CPolynomial(*degree - 1) * param[3] + zBig.CPolynomial(degree1 - 1) * param[4] + zBig.CPolynomial(degree2 - 1) * param[5] + 1.0;
		zBig = zBig.CPolynomial(*degree) + zBig.CPolynomial(degree1) + zBig.CPolynomial(degree2) + qBig;
		break;
	    case 8:
		dCBig = dCBig * zBig.CSqr() * 3.0 + 1.0;
		zBig = zBig.CCube() + (qBig - 1.0) * zBig + qBig;
		break;
	    case 9:
		dCBig = dCBig * zBig.CSqr() * 0.75 + 2.0;
		zBig = zBig.CCube() / 4.0 + zBig + qBig;
		break;
	    case 10:					// Sin(z^n)
		if (NumThreads == 0)
		    {
		    tempBig = zBig.CPolynomial(*degree);
		    dCBig = dCBig * tempBig.CCos() * zBig.CPolynomial(*degree - 1) * param[3] + 1.0;
		    zBig = tempBig.CSin() + qBig;
		    }
		else
		    {
		    tempBig = zBig.CPolynomial(*degree);
		    BigTrig.CCos(&tempBig1, tempBig);
		    dCBig = dCBig * tempBig1 * zBig.CPolynomial(*degree - 1) * param[3] + 1.0;
		    BigTrig.CSin(&tempBig1, tempBig);
		    zBig = tempBig1 + qBig;
		    }
		break;
	    case 11:					// Sinh
		if (NumThreads == 0)
		    {
		    dCBig = dCBig * zBig.CSinh() + 1.0;
		    zBig = zBig.CSinh() + qBig;
		    }
		else
		    {
		    BigTrig.CSinh(&tempBig, zBig);
		    dCBig = dCBig * tempBig + 1.0;
		    zBig = tempBig + qBig;
		    }
		break;
	    case 12:					// Sinh(z^n)
		if (NumThreads == 0)
		    {
		    tempBig = zBig.CPolynomial(*degree);
		    dCBig = dCBig * tempBig.CSinh() * zBig.CPolynomial(*degree - 1) * param[3] + 1.0;
		    zBig = tempBig.CSinh() + qBig;
		    }
		else
		    {
		    tempBig = zBig.CPolynomial(*degree);
		    BigTrig.CSinh(&tempBig1, tempBig);
		    dCBig = dCBig * tempBig1 * zBig.CPolynomial(*degree - 1) * param[3] + 1.0;
		    zBig = tempBig1 + qBig;
		    }
		break;
	    }

	if (zBig.CSumSqr() > rqlim) 
	    {
	    u = zBig / dCBig;
	    u = u / u.CFabs();
	    BigReflection = u.x * vBig.x + u.y * vBig.y + h2;
	    BigReflection = BigReflection / (h2 + 1.0); // rescale so that t does not get bigger than 1
	    reflection = (double)mpfr_get_d(BigReflection.x, MPFR_RNDN);
	    if (reflection < 0.0) reflection = 0.0;
	    *iterations = i;
	    break;
	    }
	}
    return reflection;
    }

/***********************************************************************
	Reflection: fractal calculations double float version
***********************************************************************/

double CSlope::GiveReflection(Complex j, BYTE juliaflag, Complex C, int *iterations, double rqlim, long threshold, Complex v)
    {
    int		i = 0;					// iteration 
    int		k;
    double	real_imag;
    Complex	sqr, temp, Q;

    int		degree1, degree2;

    Complex	Z = 0.0;				// initial value for iteration Z0
//    Complex	z = 1.0;				// initial value for iteration Z0 used in Newton fractals
    Complex	dC = 0.0;				// derivative with respect to c 
    double	reflection = FP_ZERO; // inside 
    double	h2 = param[1];				// height factor of the incoming light
    Complex	u;
    Complex	t = { 1.0, 0.0 };

    *degree = (int)param[3];
    if (juliaflag)
	{
	Q = j;
	Z = C;
	}
    else
	{
	if (subtype == 3)				// Sine
	    Z = param[4];
	Q = C;
	}
    for (i = 0; i < threshold; i++)
	{
	switch (subtype)
	    {
	    case 0:					// Mandelbrot
		dC = dC * (Z + Z) + 1.0;
		sqr.x = Z.x * Z.x;
		sqr.y = Z.y * Z.y;
		real_imag = Z.x * Z.y;
		Z.x = Q.x + sqr.x - sqr.y;
		Z.y = Q.y + real_imag + real_imag;
		break;
	    case 1:					// Cubic
		dC = 3.0 * dC * Z.CSqr() + 1.0;
		Z = Z.CCube() + Q;
		break;
	    case 2:					// Power
		temp = 1.0;
		for (k = 0; k < *degree - 1; k++)
		    temp *= Z;
		dC = temp * dC * (double)*degree + 1.0;
		Z = temp * Z + Q;
//		dC = dC * Z.CPolynomial((degree - 1)) * (double)degree + 1.0;
//		Z = Z.CPolynomial(degree) + Q;
		break;
	    case 3:					// Sine
		dC = dC * Z.CCos() + 1.0;
		if (param[3] == 0)
		    Z = Z.CSin() * Q;
		else
		    Z = Z.CSin() + Q;
		break;
	    case 4:					// Sine + 1/c
		dC = dC * Z.CCos() + 1.0;
		Z = Z.CSin() + t / Q;
		break;
	    case 5:					// exp
		dC = dC * Z.CExp() + 1.0;
		Z = Z.CExp() + Q;
		break;
	    case 6:					// Power + 1/c
		temp = 1.0;
		for (k = 0; k < *degree - 1; k++)
		    temp *= Z;
		dC = temp * dC * (double)*degree + 1.0;
		Z = temp * Z + t/Q;
		break;
	    case 7:
		degree1 = (int)param[4];
		degree2 = (int)param[5];
		dC = dC * Z.CPolynomial(*degree - 1) * param[3] + Z.CPolynomial(degree1 - 1) * param[4] + Z.CPolynomial(degree2 - 1) * param[5] + 1.0;
		Z = Z.CPolynomial(*degree) + Z.CPolynomial(degree1) + Z.CPolynomial(degree2) + Q;
		break;
	    case 8:
		dC = 3.0 * dC * Z.CSqr() + 1.0;
		Z = Z.CCube() + (Q - 1.0) * Z + Q;
		break;
	    case 9:
		dC = dC * Z.CSqr() * 0.75 + 2.0;
		Z = Z.CCube() / 4.0 + Z + Q;
		break;
	    case 10:					// Sin(z^n)
		temp = Z.CPolynomial(*degree);
		dC = dC * temp.CCos() * Z.CPolynomial(*degree - 1) * param[3] + 1.0;
		Z = temp.CSin() + Q;
		break;
	    case 11:					// Sinh
		dC = dC * Z.CSinh() + 1.0;
		Z = Z.CSinh() + Q;
		break;
	    case 12:					// Sinh(z^n)
		temp = Z.CPolynomial(*degree);
		dC = dC * temp.CSinh() * Z.CPolynomial(*degree - 1) * param[3] + 1.0;
		Z = temp.CSinh() + Q;
		break;
//	    case 13:					//  z1 = conj(z)); z = z1*z1+c; [Tricorn or Mandelbar]


/*
405       <reference t="R">
406         Xrn = Xr2 - Xi2 + Cr;
407         Xin = Ci - Xr * Xi * 2;
408       </reference>
409       <perturbation t="R">
410         xrn = 2 * Xr * xr + xr2 - xi2 - 2 * xi * Xi + cr;
411         xin = ci - 2 * (Xr * xi + xr * Xi + xr * xi);
412       </perturbation>
413	    < derivative t = 'M' >
414         dxan = ((-2 * (Xxi * dya)) + ((2 * (Xxr * dxa)) + daa));
415         dxbn = ((-2 * (Xxi * dyb)) + ((2 * (Xxr * dxb)) + dab));
416         dyan = ((-2 * (Xxi * dxa)) + ((-2 * (Xxr * dya)) + dba));
417         dybn = ((-2 * (Xxi * dxb)) + ((-2 * (Xxr * dyb)) + dbb));
418	    < / derivative >


		Complex temp1;
		temp = Z;
		temp.y = -temp.y;			// get conjugate
//		dC = dC * (Z + Z) + 1.0;
		dC = dC * (temp + temp)  * param[9] + 1.0;
		sqr.x = temp.x * temp.x;
		sqr.y = temp.y * temp.y;
		real_imag = temp.x * temp.y;
		Z.x = Q.x + sqr.x - sqr.y;
		Z.y = Q.y + real_imag + real_imag;
//		temp1 = Z + Z;
//		temp1.y = -temp1.y;
//		dC = dC * temp1 * param[9] + 1.0;
//		dC = dC * (temp + temp) + 1.0;
//		dC = dC * (Z + Z) + 1.0;
//		dC.y = -dC.y;
//		temp = Z;
//		temp.y = -temp.y;			// get conjugate
*/
/*
		sqr.x = Z.x * Z.x;
		sqr.y = Z.y * Z.y;
		real_imag = Z.x * Z.y;
		dC.x = ((Z.x + Z.x) * dC.x) - ((Z.y + Z.y) * dC.y) + 1.0;
		dC.y = ((Z.x + Z.x) * dC.y) - ((Z.y + Z.y) * dC.y);
		Z.x = sqr.x - sqr.y + Q.x;
		Z.y = Q.y - real_imag - real_imag;
		break;
*/
/*
	    case 13:					//  z=z-(z*z1-z)/((3*z1)-z)+c; [3rd Order Nova Variation]
		// Quotient Rule: f/g =	(f'g - g'f) / g*g
		// remember 
		Complex	z1, z2, f, g, df, dg;
		
		z1 = z;
		z2 = z.CSqr();
		f = z*z2 - 1;
		g = z2 * 3;
		df = 3 * z2;
		dg = 6 * z;
		// Applying quotient rule:
		temp = (df * g - dg * f) / g * g;
//		dC = dC * (1.0 - temp) + 2.0;
		dC = dC * Z.CSinh() + 1.0;
		z = z - ((z*z2 - 1) / (z2 * 3)) + Q;
		Z = z - z1;
		break;
*/
	    }
	if (Z.CSumSqr() > rqlim)
	    {
	    u = Z / dC;
	    u = u / u.CFabs();
	    reflection = cdot(u, v) + h2;
	    reflection = reflection / (1.0 + h2); // rescale so that t does not get bigger than 1
	    if (reflection < 0.0) reflection = 0.0;
	    *iterations = i;
	    break;
	    }
	}
    return reflection;
    }

/**************************************************************************
	Caluclate the colour of the pixel
**************************************************************************/

RGBTRIPLE CSlope::compute_colour(CTrueCol *TrueCol, Complex j, BYTE juliaflag, Complex c, BigComplex cBig, double rqlim, long threshold, BYTE BigNumFlag, Complex v, bool *Time2Exit)
    {

    double	reflection;
   BYTE		b;
    static	RGBTRIPLE colour;
    int		PaletteStart = (int)(fabs(param[2]));

    *Time2Exit = false;
    if (BigNumFlag)
	{
	reflection = GiveBigReflection(j, juliaflag, cBig, &iterations, rqlim, threshold, v);
	if (reflection < 0)
	    *Time2Exit = true;
	}
    else
	reflection = GiveReflection(j, juliaflag, c, &iterations, rqlim, threshold, v);

    if (reflection == FP_ZERO) 
	{
	colour.rgbtRed = (BYTE)TrueCol->InsideRed;		// M_waves
	colour.rgbtGreen = (BYTE)TrueCol->InsideGreen;
	colour.rgbtBlue = (BYTE)TrueCol->InsideBlue;
	}
    else // exterior of Mandelbrot set = normal 
	{
	b = (BYTE)(255.0 * reflection);

	if (iterations >= PaletteStart)
	    {
	    colour.rgbtRed = (BYTE)((reflection)   * (double)(*(TrueCol->PalettePtr + (BYTE)((iterations) % 256) * 3 + 0))); /* Red*/
	    colour.rgbtGreen = (BYTE)((reflection) * (double)(*(TrueCol->PalettePtr + (BYTE)((iterations) % 256) * 3 + 1))); /* Green */
	    colour.rgbtBlue = (BYTE)((reflection)  * (double)(*(TrueCol->PalettePtr + (BYTE)((iterations) % 256) * 3 + 2))); /* Blue */
	    }
	else
	    {
	    colour.rgbtRed = b;
	    colour.rgbtGreen = b;
	    colour.rgbtBlue = b;
	    }
	}
    return colour;
    }

/**************************************************************************
    End Threads
**************************************************************************/

void	CSlope::SlopeIsExiting(void)
    {
    EndSlope = true;		// advise thread to ternminate
    }

/**************************************************************************
	Create 2D vector
	Complex v = cexp(2.0 * angle * PI * I); // = exp(1j*angle*2*pi/360)  // unit 2D vector in this direction
	incoming light 3D vector = (v.re,v.im,h2)
**************************************************************************/

void	    CSlope::Create2DVector(Complex *v, double LightAngle)
    {
    Complex	w;	    // unit 2D vector
    w = { 0.0, 2.0 * LightAngle * PI / 360.0};
    *v = w.CExp();
    }

/**************************************************************************
	Slope Fractal
**************************************************************************/

int CSlope::RunSlopeDerivative(HWND hwndIn, int user_data(HWND hwnd), char* StatusBarInfo, bool *ThreadComplete, int subtypeIn, int NumThreadsIn, int threadIn, Complex j, double mandel_width, double hor, double vert, double xgap, double ygap,
					    BYTE BigNumFlag, BigDouble Big_xgap, BigDouble Big_ygap, BigDouble BigHor, BigDouble BigVert, BigDouble BigWidth, double rqlim, long threshold, double paramIn[], CTrueCol *TrueCol, CDib *Dib, 
					    int bits_per_pixelIn, BYTE juliaflag, int xdots, int ydots, int width, int height, WORD *degreeIn, HANDLE ghMutex)
//    BYTE BigNumFlag, BigDouble Big_xgap, BigDouble Big_ygap, BigDouble BigHor, BigDouble BigVert, BigDouble BigWidth, double rqlim, long threshold, int degree, bool UsePalette, double LightAngle, double LightHeight, CTrueCol *TrueCol, CDib *Dib)
    {
    Complex	c;
    BigComplex	cBig;
    RGBTRIPLE	colour;
    int		lastChecked = -1;
    Complex	v;// unit 2D vector in this direction
    int		i;
    double	LightAngle = param[0];
    bool	Time2Exit = false;

    for (i = 0; i < NUMSLOPEDERIVPARAM; i++)
	param[i] = paramIn[i];

    thread = threadIn;
    NumThreads = NumThreadsIn;
    subtype = subtypeIn;
    hwnd = hwndIn;
    bits_per_pixel = bits_per_pixelIn;
    degree = degreeIn;

    Create2DVector(&v, LightAngle);
    if (NumThreads == 0)
	StripWidth = xdots;
    else
	StripWidth = xdots / NumThreads + 1;
    StripStart = StripWidth * thread;

    *ThreadComplete = false;

    Plot.InitPlot(threshold, TrueCol, wpixels, xdots, height, xdots, height, Dib->BitsPerPixel, Dib, USEPALETTE);
    for (iY = 0; iY < ydots; iY++)
	{
	double progress = (double)iY / ydots;
	if (BigNumFlag)
	    cBig.y = BigVert + BigWidth - Big_ygap * iY;
	else
	    c.y = vert + mandel_width - iY * ygap;
	if (int(progress * 100) != lastChecked)
	    {
	    lastChecked = int(progress * 100);
	    sprintf(StatusBarInfo, "Progess (%d%%), %d Threads", int(progress * 100), NumThreads);
	    }
	for (iX = StripStart; iX < StripStart + StripWidth && iX < xdots; iX++)
	    {
	    if (user_data(hwnd) < 0)
		return -1;
	    if (BigNumFlag)
		cBig.x = BigHor + Big_xgap * iX;
	    else
		c.x = hor + iX * xgap;

	    colour = compute_colour(TrueCol, j, juliaflag, c, cBig, rqlim, threshold, BigNumFlag, v, &Time2Exit);
	    if (Time2Exit)
		return 0;
//	    plot(iX, iY, iterations);
	    if (EndSlope)		// program is trying to terminate. Better get out of the thread
		{ 
		*ThreadComplete = true;
		return -1;
		}
	    if (ghMutex != NULL)
		WaitForSingleObject(ghMutex, INFINITE);  // no time-out interval
	    Plot.OutRGBpoint(iX, iY, colour);
	    if (ghMutex != NULL)
		ReleaseMutex(ghMutex);
	    }
	}
    *ThreadComplete = true;
    return 0;
    }

