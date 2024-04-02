/*
    FWDDIFF.CPP a module for determining the slope using forward differencing calculations of fractals. 
    
    Written in Microsoft Visual 'C++' by Paul de Leeuw.

    This program is written in "standard" API and not in MFC for portability
     https://github.com/hrkalona/Fractal-Zoomer/blob/master/src/fractalzoomer/core/ThreadDraw.java#L3640
     https://github.com/hrkalona/Fractal-Zoomer/blob/master/src/fractalzoomer/main/app_settings/BumpMapSettings.java
*/

#include	"slope.h"

/**************************************************************************
    Calculate functions
**************************************************************************/

void	CSlope::DoSlopeFwdDiffFn(Complex *z, Complex *q)
    {
    Complex	sqr, temp, poly;
    Complex	t = { 1.0, 0.0 };
    double	real_imag;
    int		k;
    int		degree1, degree2;

//    PaletteShift = (int)param[4];

    *degree = (int)param[5];
    switch (subtype)
	{
	case 0:					// Mandelbrot
	    SlopeDegree = 2;
	    sqr.x = z->x * z->x;
	    sqr.y = z->y * z->y;
	    real_imag = z->x * z->y;
	    z->x = q->x + sqr.x - sqr.y;
	    z->y = q->y + real_imag + real_imag;
	    break;
	case 1:					// Burning Ship
	    SlopeDegree = 2;
	    sqr.x = z->x * z->x;
	    sqr.y = z->y * z->y;
	    real_imag = fabs(z->x * z->y);
	    z->x = sqr.x - sqr.y + q->x;
	    z->y = real_imag + real_imag - q->y;
	    break;

	case 2:					// Cubic
	    SlopeDegree = 3;
	    *z = z->CCube() + *q;
	    break;
	case 3:					// Power
	    SlopeDegree = *degree;
	    temp = 1.0;
	    for (k = 0; k < *degree - 1; k++)
		temp *= *z;
	    *z = temp * *z + *q;
	    //		Z = Z.CPolynomial(degree) + Q;
	    break;
	case 4:					// Sin
	    SlopeDegree = 2;
	    if (param[5] == 0)
		*z = z->CSin() * *q;
	    else
		*z = z->CSin() + *q;
	    break;
	case 5:					// Sin + 1/c
	    SlopeDegree = 2;
	    *z = z->CSin() + t / *q;
	    break;
	case 6:					// exp
	    SlopeDegree = 2;
	    *z = z->CExp() + *q;
	    break;
	case 7:					// Power + 1/c
	    SlopeDegree = *degree;
	    temp = 1.0;
	    for (k = 0; k < *degree - 1; k++)
		temp *= *z;
	    *z = temp * *z + t / *q;
	    break;
	case 8:
	    degree1 = (int)param[6];
	    degree2 = (int)param[7];
	    SlopeDegree = degree2;
	    *z = z->CPolynomial(*degree) + z->CPolynomial(degree1) + z->CPolynomial(degree2) + *q;
	    break;
	case 9:
	    SlopeDegree = 3;
	    *z = z->CCube() + (*q - 1.0) * *z + *q;
	    break;
	case 10:
	    SlopeDegree = 3;
	    *z = z->CCube() / 4.0 + *z + *q;
	    break;
	case 11:					// Sin(z^n)
	    SlopeDegree = 2;
	    temp = z->CPolynomial(*degree);
	    *z = temp.CSin() + *q;
	    break;
	case 12:					// Sinh
	    SlopeDegree = 2;
	    *z = z->CSinh() + *q;
	    break;
	case 13:					// Sinh(z^n)
	    SlopeDegree = *degree;
	    temp = z->CPolynomial(*degree);
	    *z = temp.CSinh() + *q;
	    break;
	case 14:					//  z1 = conj(z)); z = z1*z1+c; [Tricorn or Mandelbar]
	    SlopeDegree = 2;
	    sqr.x = z->x * z->x;
	    sqr.y = z->y * z->y;
	    real_imag = z->x * z->y;
	    z->x = sqr.x - sqr.y + q->x;
	    z->y = q->y - real_imag - real_imag;
	    break;
	case 15:					// Art Matrix Cubic
	    SlopeDegree = 3;
	    if (variety == 'K')				// CKIN
		*z = z->CCube() + b;			// Z = Z*Z*Z + B
	    else
		{
		temp = z->CCube() + b;			// Z = Z*Z*Z + B
		*z = temp - aa3 * *z;			// Z = Z*Z*Z - AA3*Z + B
		}
	    break;
	case 16:					// Basic Newton
	    temp = *z;
	    poly = z->CPolynomial(*degree - 1);
	    *z = *z - (poly * *z - *q - 1) / (poly * *degree);
	    t2 = *z - temp;
	    break;
	}
    }

void	CSlope::DoBigSlopeFwdDiffFn(BigComplex *z, BigComplex *q, BigComplex *bBig, BigComplex *aa3Big)
    {
    BigComplex	sqr, temp, temp1, poly;
    BigComplex	unity = { 1.0, 0.0 };
    BigDouble	real_imag, t;
    int		k;
    int		degree1, degree2;

//    PaletteStart = (int)param[4];

    *degree = (int)param[5];
    switch (subtype)
	{
	case 0:					// Mandelbrot
	    SlopeDegree = 2;
	    sqr.x = z->x * z->x;
	    sqr.y = z->y * z->y;
	    real_imag = z->x * z->y;
	    z->x = q->x + sqr.x - sqr.y;
	    z->y = q->y + real_imag + real_imag;
	    break;
	case 1:					// Burning Ship
	    SlopeDegree = 2;
	    sqr.x = z->x * z->x;
	    sqr.y = z->y * z->y;
	    t = z->x * z->y;
	    real_imag = t.BigAbs();
	    z->x = sqr.x - sqr.y + q->x;
	    z->y = real_imag + real_imag - q->y;
	    break;

	case 2:					// Cubic
	    SlopeDegree = 3;
	    *z = z->CCube() + *q;
	    break;
	case 3:					// Power
	    SlopeDegree = *degree;
	    temp = 1.0;
	    for (k = 0; k < *degree - 1; k++)
		temp *= *z;
	    *z = temp * *z + *q;
	    //		Z = Z.CPolynomial(degree) + Q;
	    break;
	case 4:					// Sin
	    SlopeDegree = 2;
	    if (NumThreads == 0)
		{
		if (param[5] == 0)
		    *z = z->CSin() * *q;
		else
		    *z = z->CSin() + *q;
		}
	    else
		{
		BigTrig.CSin(&temp, *z);
		if (param[5] == 0)
		    *z = temp * *q;
		else
		    *z = temp + *q;
		}
	    break;
	case 5:					// Sin + 1/c
	    SlopeDegree = 2;
	    if (NumThreads == 0)
		*z = z->CSin() + unity / *q;
	    else
		{
		BigTrig.CSin(&temp, *z);
		*z = temp + unity / *q;
		}
	    break;
	case 6:					// exp
	    SlopeDegree = 2;
	    if (NumThreads == 0)
		*z = z->CExp() + *q;
	    else
		{
		BigTrig.CExp(&temp, *z);
		*z = temp + *q;
		}
	    break;
	case 7:					// Power + 1/c
	    SlopeDegree = *degree;
	    temp = 1.0;
	    for (k = 0; k < *degree - 1; k++)
		temp *= *z;
	    *z = temp * *z + unity / *q;
	    break;
	case 8:
	    degree1 = (int)param[6];
	    degree2 = (int)param[7];
	    SlopeDegree = degree2;
	    *z = z->CPolynomial(*degree) + z->CPolynomial(degree1) + z->CPolynomial(degree2) + *q;
	    break;
	case 9:
	    SlopeDegree = 3;
	    *z = z->CCube() + (*q - 1.0) * *z + *q;
	    break;
	case 10:
	    SlopeDegree = 3;
	    *z = z->CCube() / 4.0 + *z + *q;
	    break;
	case 11:					// Sin(z^n)
	    SlopeDegree = 2;
	    temp = z->CPolynomial(*degree);
	    if (NumThreads == 0)
		*z = temp.CSin() + *q;
	    else
		{
		BigTrig.CSin(&temp1, temp);
		*z = temp1 + *q;
		}
	    break;
	case 12:					// Sinh
	    SlopeDegree = 2;
	    if (NumThreads == 0)
		*z = z->CSinh() + *q;
	    else
		{
		BigTrig.CSinh(&temp, *z);
		*z = temp + *q;
		}
	    break;
	case 13:					// Sinh(z^n)
	    SlopeDegree = *degree;
	    temp = z->CPolynomial(*degree);
	    if (NumThreads == 0)
		{
		*z = temp.CSinh() + *q;
		}
	    else
		{
		BigTrig.CSinh(&temp1, temp);
		*z = temp1 + *q;
		}
	    break;
	case 14:					//  z1 = conj(z)); z = z1*z1+c; [Tricorn or Mandelbar]
	    SlopeDegree = 2;
	    sqr.x = z->x * z->x;
	    sqr.y = z->y * z->y;
	    real_imag = z->x * z->y;
	    z->x = sqr.x - sqr.y + q->x;
	    z->y = q->y - real_imag - real_imag;
	    break;
	case 15:					// Art Matrix Cubic
	    SlopeDegree = 3;
	    if (variety == 'K')				// CKIN
		*z = z->CCube() + *bBig;		// Z = Z*Z*Z + B
	    else
		{
		temp = z->CCube() + *bBig;		// Z = Z*Z*Z + B
		*z = temp - *aa3Big * *z;		// Z = Z*Z*Z - AA3*Z + B
		}
	    break;
	case 16:					// Basic Newton
	    temp = *z;
	    poly = z->CPolynomial(*degree - 1);
	    *z = *z - (poly * *z - *q - 1) / (poly * *degree);
	    zNewton = *z - temp;
	    break;
	}
    }

/**************************************************************************
    Get the gradients in the x and y directions
**************************************************************************/

double	CSlope::getGradientX(double *wpixels, int index, int width)
    {
    int x = index % width;
    double it = *(wpixels + index);

    if (x == 0) {
	return (*(wpixels + index + 1) - it) * 2;
	}
    else if (x == width - 1) {
	return (it - *(wpixels + index - 1)) * 2;
	}
    else {
	double diffL = it - *(wpixels + index - 1);
	double diffR = it - *(wpixels + index + 1);
	return diffL * diffR >= 0 ? 0 : diffL - diffR;
	}
    }

double	CSlope::getGradientY(double *wpixels, int index, int width, int height)
    {
    int y = index / width;
    double it = *(wpixels + index);

    if (y == 0) {
	return (it - *(wpixels + index + width)) * 2;
	}
    else if (y == height - 1) {
	return (*(wpixels + index - width) - it) * 2;
	}
    else {
	double diffU = it - *(wpixels + index - width);
	double diffD = it - *(wpixels + index + width);
	return diffD * diffU >= 0 ? 0 : diffD - diffU;
	}
    }

/**************************************************************************
   Brightness Scaling
**************************************************************************/

int	CSlope::changeBrightnessOfColorScaling(int rgb, double delta, double bump_transfer_factor)
    {
    int	    new_color = 0;

    //    double mul = getBumpCoef(delta);
    double  mul = (1.5 / (fabs(delta * bump_transfer_factor) + 1.5));

    if (delta > 0) {
	rgb ^= 0xFFFFFF;
	int r = rgb & 0xFF0000;
	int g = rgb & 0x00FF00;
	int b = rgb & 0x0000FF;
	int ret = (int)(r * mul + 0.5) & 0xFF0000 | (int)(g * mul + 0.5) & 0x00FF00 | (int)(b * mul + 0.5);
	new_color = 0xff000000 | (ret ^ 0xFFFFFF);
	}
    else {
	int r = rgb & 0xFF0000;
	int g = rgb & 0x00FF00;
	int b = rgb & 0x0000FF;
	new_color = 0xff000000 | (int)(r * mul + 0.5) & 0xFF0000 | (int)(g * mul + 0.5) & 0x00FF00 | (int)(b * mul + 0.5);
	}

    return new_color;
    }

/**************************************************************************
   Initialise per pixel
**************************************************************************/

void	CSlope::InitFwd(BYTE BigNumFlag, BYTE juliaflag, BigComplex cBig, BigComplex *zBig, BigComplex *qBig, BigComplex *aBig, BigComplex *bBig, BigComplex *vBig,
    BigComplex *a2Big, BigComplex *aa3Big, Complex j, Complex c, Complex *z, Complex *q)
    {
    BigComplex	t2Big, t3Big;

    if (subtype == 15)					// init Art Matrix Newton
	{
//	Complex	temp;

	switch ((int)param[6])
	    {
	    case 0:
		variety = 'B';
		break;
	    case 1:
		variety = 'C';
		break;
	    case 2:
		variety = 'F';
		break;
	    case 3:
		variety = 'K';
		break;
	    default:
		variety = 'B';
		break;
	    }
	if ((int)param[5] < 0)
	    SpecialColour = 0;
	else
	    SpecialColour = (int)param[5];
	SpecialFlag = false;
	}
    if (BigNumFlag)
	{
	if (juliaflag)
	    {
	    *qBig = j;
	    *zBig = cBig;
	    }
	else
	    {
	    *qBig = cBig;
	    if (subtype == 4)				// Sine
		*zBig = param[6];
	    else if (subtype == 16)			// good old Newton
		*zBig = cBig;
	    else if (subtype == 15)			// init Art Matrix Newton
		{
		BigComplex	temp;

		if (variety == 'B')			// CBIN 
		    {
		    t3Big = *qBig * 3;			// T3 = 3*T
		    t2Big = qBig->CSqr();		// T2 = T*T
		    *aBig = (t2Big + 1) / t3Big;		// A  = (T2 + 1)/T3
							// B  = 2*A*A*A + (T2 - 2)/T3    
		    temp = aBig->CCube();		// A*A*A
		    temp = temp.CDouble();		// 2*A*A*A
		    *bBig = (t2Big - 2) / t3Big + temp;	// B  = 2*A*A*A + (T2 - 2)/T3
		    }
		else if (variety == 'C' || variety == 'F')	// CCIN or CFIN
		    {
		    *aBig = *qBig;			// A = T
							// find B = T + 2*T*T*T
		    temp = qBig->CCube();		// B = T*T*T
		    if (variety == 'C')
			*bBig = temp + temp + *qBig;	// B = B * 2 + T
		    else
			{
			*bBig = (temp - *qBig) * 2;	// B = B * 2 - 2 * T
			*a2Big = *aBig + *aBig;
			}
		    }
		else if (variety == 'K')		// CKIN 
		    {
		    *aBig = 0;
		    *vBig = 0;
		    *bBig = *qBig;			// B = T
		    }
		*aa3Big = aBig->CSqr() * 3;		// AA3 = A*A*3
		*zBig = -(*aBig);				// Z = -A
		}
	    else
		*zBig = 0.0;
	    }
	}
    else
	{
	if (juliaflag)
	    {
	    *q = j;
	    *z = c;
	    }
	else
	    {
	    *q = c;
	    if (subtype == 4)				// Sine
		*z = param[6];
	    else if (subtype == 16)			// good old Newton
		*z = c;
	    else if (subtype == 15)				// init Art Matrix Cubic
		{
		Complex	temp;

		if (variety == 'B')			// CBIN 
		    {
		    t3 = *q * 3;				// T3 = 3*T
		    t2 = q->CSqr();			// T2 = T*T
		    a = (t2 + 1) / t3;			// A  = (T2 + 1)/T3
							// B  = 2*A*A*A + (T2 - 2)/T3    
		    temp = a.CCube() * 2;		// 2*A*A*A
		    b = (t2 - 2) / t3 + temp;		// B  = 2*A*A*A + (T2 - 2)/T3
		    }
		else if (variety == 'C' || variety == 'F')	// CCIN or CFIN
		    {
		    a = *q;				// A = T
							// find B = T + 2*T*T*T
		    temp = q->CCube();			// B = T*T*T
		    if (variety == 'C')
			b = temp + temp + *q;		// B = B * 2 + T
		    else
			{
			b = (temp - *q) * 2;		// B = B * 2 - 2 * T
			a2 = a + a;
			}
		    }
		else if (variety == 'K')		// CKIN 
		    {
		    a = 0;
		    v = 0;
		    b = *q;				// B = T
		    }
		aa3 = a.CSqr() * 3;			// AA3 = A*A*3
		*z = -a;				// Z = -A
		}
	    else
		*z = 0.0;
	    }
	}
    }


/**************************************************************************
	Slope Fractal
**************************************************************************/

int	CSlope::RunSlopeFwdDiff(HWND hwndIn, int user_data(HWND hwnd), char* StatusBarInfo, bool *ThreadComplete, int subtypeIn, int NumThreadsIn, int threadIn, Complex j, double mandel_width, double hor, double vert, double xgap, double ygap,
    BYTE BigNumFlag, BigDouble Big_xgap, BigDouble Big_ygap, BigDouble BigHor, BigDouble BigVert, BigDouble BigWidth, double rqlim, long threshold, double paramIn[], CTrueCol *TrueCol, CDib *Dib, double *wpixels, BYTE juliaflag, int xdots, int ydots, int width, WORD *degreeIn)
    {
    Complex	z = 0.0;			// initial value for iteration Z0
    Complex	c, q;
    BigComplex	cBig, zBig, qBig;
    BigDouble	BigTemp, BigTemp1;
    BigComplex	t2Big, t3Big, aBig, bBig, vBig, a2Big, aa3Big;
    BigComplex	OldZBig, OlderZBig;

    double	log_zn, nu;
    int		lastChecked = -1;

    int		x, y, i;
    DWORD	index;
    long	ColourPtr;
    double	iterations;
    Complex	OldZ, OlderZ;

    thread = threadIn;
    NumThreads = NumThreadsIn;
    subtype = subtypeIn;
    hwnd = hwndIn;
    degree = degreeIn;

    for (i = 0; i < NUMSLOPEDERIVPARAM; i++)
	param[i] = paramIn[i];

    if (NumThreads == 0)
	StripWidth = xdots;
    else
	StripWidth = xdots / NumThreads;
    StripStart = StripWidth * thread;
    *ThreadComplete = false;

    Plot.InitPlot(threshold, TrueCol, wpixels, xdots, ydots, xdots, ydots, Dib->BitsPerPixel, Dib, USEPALETTE);

    for (y = 0; y < ydots; y++)
	{
	if (BigNumFlag)
	    cBig.y = BigVert + BigWidth - Big_ygap * y;
	else
	    c.y = vert + mandel_width - y * ygap;
	double progress = (double)y / ydots;
	if (int(progress * 100) != lastChecked)
	    {
	    lastChecked = int(progress * 10);
	    sprintf(StatusBarInfo, "Progess (%d%%), %d Threads", int(progress * 100), NumThreads);
	    }
	for (x = StripStart; x < StripStart + StripWidth; x++)
	    {
	    if (user_data(hwnd) < 0)	// trap user input
		return -1;

	    if (BigNumFlag)
		cBig.x = BigHor + Big_xgap * x;
	    else
		c.x = hor + x * xgap;
	    InitFwd(BigNumFlag, juliaflag, cBig, &zBig, &qBig, &aBig, &bBig, &vBig, &a2Big, &aa3Big, j, c, &z, &q);
	    iterations = 0.0;
	    for (;;)
		{
		if (subtype == 16)				// good old Newton
		    {
		    if (BigNumFlag)
			{
			OlderZBig = OldZBig;
			OldZBig = zBig;
			}
		    else
			{
			OlderZ = OldZ;
			OldZ = z;
			}
		    }
		iterations++;
		if (iterations >= threshold)
		    break;
		if (BigNumFlag)
		    {
		    DoBigSlopeFwdDiffFn(&zBig, &qBig, &bBig, &aa3Big);
		    if (subtype == 15)				// init Art Matrix Cubic
			{
			if (zBig.CSumSqr() > 100.0)
			    break;
			else
			    {
			    if (variety == 'F')
				{
				if (qBig.CSumSqr() < 0.111111)
				    {
				    SpecialFlag = true;			// for decomp and biomorph
				    break;
				    }
				vBig = zBig + a2Big;
				}
			    else if (variety == 'K')
				vBig = zBig - vBig;
			    else
				vBig = zBig - aBig;
			    if (vBig.CSumSqr() <= 0.000001)
				{
				SpecialFlag = true;			// for decomp and biomorph
				break;
				}
			    }
			}
		    else if (subtype == 16)				// good old Newton
			{
			double	d;
			d = zNewton.CSumSqr();
			if (d < MINSIZE)
			    break;
			}
		    else if (zBig.CSumSqr() >= rqlim)
			break;
		    }
		else
		    {
		    DoSlopeFwdDiffFn(&z, &q);
		    if (subtype == 15)				// init Art Matrix Newton
			{
			if (z.CSumSqr() > 100.0)
			    break;
			else
			    {
			    if (variety == 'F')
				{
				if (q.CSumSqr() < 0.111111)
				    {
				    SpecialFlag = true;			// for decomp and biomorph
				    break;
				    }
				v = z + a2;
				}
			    else if (variety == 'K')
				v = z - v;
			    else
				v = z - a;
			    if (v.CSumSqr() <= 0.000001)
				{
				SpecialFlag = true;			// for decomp and biomorph
				break;
				}
			    }
			}
		    else if (subtype == 16)				// good old Newton
			{
			double	d;
			d = t2.CSumSqr();
			if (d < MINSIZE)
			    break;
			}
		    else if ((z.x * z.x + z.y * z.y) >= rqlim)
			break;
		    }
		}
	    if (iterations < threshold)
		{
		if (BigNumFlag)
		    {
		    double	t, ta, tb;
		    BigComplex	aBig, bBig;
		    if (subtype == 16)				// good old Newton
			{
			aBig = OldZBig - OlderZBig;
			bBig = zBig - OldZBig;
			BigTemp = aBig.x * aBig.x + aBig.y * aBig.y;
			ta = mpfr_get_d(BigTemp.x, MPFR_RNDN);
			log_zn = log(MINSIZE) - log(ta);						// convergence method 1 (page 25 Fractal-Zoomer Algorithms.docx)
			BigTemp = bBig.x * bBig.x + bBig.y * bBig.y;
			tb = mpfr_get_d(BigTemp.x, MPFR_RNDN);
			t = log(tb) - log(ta);
			nu = log_zn / t;
			iterations = iterations + nu;
			}
		    else
			{
			// sqrt of inner term removed using log simplification rules.
			BigTemp = zBig.x * zBig.x + zBig.y * zBig.y;
//			BigTemp1 = BigTemp.BigLog();
//			log_zn = mpfr_get_d(BigTemp1.x, MPFR_RNDN) / SlopeDegree;
			t = mpfr_get_d(BigTemp.x, MPFR_RNDN);
			log_zn = log(t) / SlopeDegree;
			nu = log(log_zn / log(SlopeDegree)) / log(SlopeDegree);
			iterations = iterations + 1 - nu;
			// Rearranging the potential function.
			// Dividing log_zn by log(2) instead of log(N = 1<<8)
			// because we want the entire palette to range from the
			// center to radius 2, NOT our bailout radius.
			}
		    }
		else
		    {
		    if (subtype == 16)				// good old Newton
			{
			double	t;
			a = OldZ - OlderZ;
			b = z - OldZ;
			log_zn = log(MINSIZE) - log(a.x * a.x + a.y * a.y);				// convergence method 1 (page 25 Fractal-Zoomer Algorithms.docx)
			t = log(b.x * b.x + b.y * b.y) - log(a.x * a.x + a.y * a.y);
			nu = log_zn / t;
			iterations = iterations + nu;
			}
		    else
			{
			// sqrt of inner term removed using log simplification rules.
			log_zn = log(z.x * z.x + z.y * z.y) / SlopeDegree;
			nu = log(log_zn / log(SlopeDegree)) / log(SlopeDegree);
			iterations = iterations + 1 - nu;
			}
		    }
		if (subtype == 15)				// Art Matrix Cubic
		    {
		    if (SpecialFlag)
			iterations = (double)SpecialColour;
		    }
		}
	    index = ((DWORD)y * (DWORD)width) + (DWORD)x;
	    if (x >= 0 && x < xdots && y >= 0 && y < ydots)
		*(wpixels + index) = iterations;
	    if ((long)iterations >= threshold)
		ColourPtr = threshold;
	    else
		{
		if (abs(PaletteShift) <= 1)
		    ColourPtr = (long)iterations;
		else
		    ColourPtr = ((long)(iterations * PaletteShift)) % 256;
		}
	    Plot.PlotPoint(x, y, ColourPtr);
	    }
	}
    *ThreadComplete = true;
    return 0;
    }

/**************************************************************************
	Init Render 
**************************************************************************/

void	CSlope::InitRender(long thresholdIn, CTrueCol *TrueColIn, CDib *DibIn, double *wpixelsIn, int PaletteShiftIn, double bump_transfer_factorIn, int PaletteStartIn, double lightDirectionDegreesIn, 
		double bumpMappingDepthIn, double bumpMappingStrengthIn)

    {
    threshold = thresholdIn; TrueCol = TrueColIn;  Dib = DibIn; wpixels = wpixelsIn; PaletteShift = PaletteShiftIn; bump_transfer_factor = bump_transfer_factorIn;
    PaletteStart = PaletteStartIn; lightDirectionDegrees = lightDirectionDegreesIn; bumpMappingDepth = bumpMappingDepthIn, bumpMappingStrength = bumpMappingStrengthIn;
    }

/**************************************************************************
	Render Slope Fractal
**************************************************************************/

int	CSlope::RenderSlope(int xdots, int ydots, int PertColourMethod, int PalOffset, double IterDiv)

    {
    double	dotp, gradAbs, gradCorr, cosAngle, sizeCorr, smoothGrad, lightAngleRadians, lightx, lighty;
    
    double	iterations;
    int		lastChecked = -1;
    DWORD	index;
    int		x, y;
    double	gradx, grady;
    unsigned char r, g, b;
    RGBTRIPLE	colour;
    int		modified;

    lastChecked = -1;
    sizeCorr = 0.0;
    lightx = 0.0;
    lighty = 0.0;

    Plot.InitPlot(threshold, TrueCol, wpixels, xdots, ydots, xdots, ydots, Dib->BitsPerPixel, Dib, USEPALETTE);
    gradCorr = pow(2, (bumpMappingStrength - DEFAULT_BUMP_MAPPING_STRENGTH) * 0.05);
    sizeCorr = ydots / pow(2, (MAX_BUMP_MAPPING_DEPTH - bumpMappingDepth) * 0.16);
    lightAngleRadians = lightDirectionDegrees * PI / 180.0;
    lightx = cos(lightAngleRadians) * gradCorr;
    lighty = sin(lightAngleRadians) * gradCorr;

    for (y = 0; y < ydots; y++)
	{
	for (x = 0; x < xdots; x++)
	    {
	    index = ((DWORD)y * (DWORD)Dib->DibWidth) + (DWORD)x;
	    if (wpixels)
		iterations = *(wpixels + index);
	    else
		return 0;			// oops, we don't actually have forward differencing

	    if (PertColourMethod != 0 && iterations < threshold)		// Kalles colour method
		{
		double  iter = iterations;
		if (PertColourMethod == 1)
		    iter = sqrt(iter);
		else if (PertColourMethod == 2)
		    iter = pow(iter, (double)1 / (double)3);
		else if (PertColourMethod == 3)
		    iter = log(iter);
		iterations = iter;
		}
	    if (IterDiv != 1.0 && IterDiv != 0.0 && iterations < threshold)
		iterations /= IterDiv;
	    if (PalOffset && iterations < threshold)
		iterations = ((int)iterations + PalOffset) % TrueCol->ColoursInPALFile;

	    if (iterations >= threshold)
		{				//  interior of Mandelbrot set = inside_color = blue 
		colour.rgbtRed = (BYTE)TrueCol->InsideRed;		// M_waves
		colour.rgbtGreen = (BYTE)TrueCol->InsideGreen;
		colour.rgbtBlue = (BYTE)TrueCol->InsideBlue;
		}
	    else
		{
		// modified = rgbs[index];
		if (iterations < PaletteStart)
		    modified = 0x00FFFFFF;
		else
		    {
		    if (abs(PaletteShift) > 1)
			modified = 0xFF000000 | ((DWORD)*(TrueCol->PalettePtr + (((long)(iterations * abs(PaletteShift))) % TrueCol->ColoursInPALFile) * 3 + 0) << 16)
					      | ((DWORD)*(TrueCol->PalettePtr + (((long)(iterations * abs(PaletteShift))) % TrueCol->ColoursInPALFile) * 3 + 1) << 8)
					      | *(TrueCol->PalettePtr + (((long)(iterations * abs(PaletteShift))) % TrueCol->ColoursInPALFile) * 3 + 2);
		    else
			modified = 0xFF000000 | ((DWORD)*(TrueCol->PalettePtr + (((long)iterations) % TrueCol->ColoursInPALFile) * 3 + 0) << 16)
					      | ((DWORD)*(TrueCol->PalettePtr + (((long)iterations) % TrueCol->ColoursInPALFile) * 3 + 1) << 8)
					      | *(TrueCol->PalettePtr + (((long)iterations) % TrueCol->ColoursInPALFile) * 3 + 2);
		    }
		gradx = getGradientX(wpixels, index, xdots);
		grady = getGradientY(wpixels, index, xdots, ydots);
		dotp = gradx * lightx + grady * lighty;
//		int	original_color = modified;		// not sure what this is for
		if (dotp != 0)
		    {
		    gradAbs = sqrt(gradx * gradx + grady * grady);
		    cosAngle = dotp / gradAbs;
		    smoothGrad = -2.3562 / (gradAbs * sizeCorr + 1.5) + 1.57;
		    //smoothGrad = Math.atan(gradAbs * sizeCorr);
		    modified = changeBrightnessOfColorScaling(modified, cosAngle * smoothGrad, bump_transfer_factor);
		    }
		//	    else if (dotp != 0 || (dotp == 0 && !isInt(image_iterations[index]))) 
		//		{
		//		gradAbs = sqrt(gradx * gradx + grady * grady);
		//		cosAngle = dotp / gradAbs;
		//		smoothGrad = -2.3562 / (gradAbs * sizeCorr + 1.5) + 1.57;
		//		//smoothGrad = Math.atan(gradAbs * sizeCorr);
		//		modified = changeBrightnessOfColorBlending(modified, cosAngle * smoothGrad);
		//		}
		//	    modified = postProcessingSmoothing(modified, image_iterations, original_color, y, x, image_size, bms.bm_noise_reducing_factor);
			    // compute  pixel color (24 bit = 3 bytes)
		//	    rgbs[index] = modified;
		r = (modified >> 16) & 0xFF;
		g = (modified >> 8) & 0xFF;
		b = modified & 0xFF;
		colour.rgbtRed = r;
		colour.rgbtGreen = g;
		colour.rgbtBlue = b;
		}
	    Plot.OutRGBpoint(x, y, colour);
	    }
	}
    return 0;
    }

