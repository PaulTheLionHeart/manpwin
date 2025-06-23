/*
    FWDDIFF.CPP a module for determining the slope using forward differencing calculations of fractals. 
    
    Written in Microsoft Visual 'C++' by Paul de Leeuw.

    This program is written in "standard" API and not in MFC for portability
     https://github.com/hrkalona/Fractal-Zoomer/blob/master/src/fractalzoomer/core/ThreadDraw.java#L3640
     https://github.com/hrkalona/Fractal-Zoomer/blob/master/src/fractalzoomer/main/app_settings/BumpMapSettings.java
*/

#include	"slope.h"

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

/**************************************************************************
   Initialise double float
**************************************************************************/

void	CSlope::InitFwdDouble(BYTE juliaflag, Complex j, Complex c, Complex *z, Complex *q)
    {
    if (juliaflag)
	{
	*q = j;
	*z = c;
	}
    else
	{
	*q = c;
	if (subtype == 4)			// Sine
	    *z = param[6];
	else if (subtype == 16)			// good old Newton
	    *z = c;
	else if (subtype == 15)			// init Art Matrix Cubic
	    {
	    Complex	temp;

	    if (variety == 'B')			// CBIN 
		{
		t3 = *q * 3;			// T3 = 3*T
		t2 = q->CSqr();			// T2 = T*T
		a = (t2 + 1) / t3;		// A  = (T2 + 1)/T3
						// B  = 2*A*A*A + (T2 - 2)/T3    
		temp = a.CCube() * 2;		// 2*A*A*A
		b = (t2 - 2) / t3 + temp;	// B  = 2*A*A*A + (T2 - 2)/T3
		}
	    else if (variety == 'C' || variety == 'F')	// CCIN or CFIN
		{
		a = *q;				// A = T
						// find B = T + 2*T*T*T
		temp = q->CCube();		// B = T*T*T
		if (variety == 'C')
		    b = temp + temp + *q;	// B = B * 2 + T
		else
		    {
		    b = (temp - *q) * 2;	// B = B * 2 - 2 * T
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

/**************************************************************************
   Initialise double double
**************************************************************************/

void	CSlope::InitFwdDD(BYTE juliaflag,
		DDComplex cDD, DDComplex *zDD, DDComplex *qDD, DDComplex *aDD, DDComplex *bDD, DDComplex *vDD, DDComplex *a2DD, DDComplex *aa3DD,
		Complex j, Complex c, Complex *z, Complex *q)
    {
    DDComplex	t2DD, t3DD;
    if (juliaflag)
	{
	*qDD = j;
	*zDD = cDD;
	}
    else
	{
	*qDD = cDD;
	if (subtype == 4)			// Sine
	    *zDD = param[6];
	else if (subtype == 16)			// good old Newton
	    *zDD = cDD;
	else if (subtype == 15)			// init Art Matrix Newton
	    {
	    DDComplex	temp;

	    if (variety == 'B')			// CBIN 
		{
		t3DD = *qDD * 3;		// T3 = 3*T
		t2DD = qDD->CSqr();		// T2 = T*T
		*aDD = (t2DD + 1) / t3DD;	// A  = (T2 + 1)/T3
						// B  = 2*A*A*A + (T2 - 2)/T3    
		temp = aDD->CCube();		// A*A*A
		temp *= 2;			// 2*A*A*A
		*bDD = (t2DD - 2) / t3DD + temp;// B  = 2*A*A*A + (T2 - 2)/T3
		}
	    else if (variety == 'C' || variety == 'F')	// CCIN or CFIN
		{
		*aDD = *qDD;			// A = T
						// find B = T + 2*T*T*T
		temp = qDD->CCube();		// B = T*T*T
		if (variety == 'C')
		    *bDD = temp + temp + *qDD;	// B = B * 2 + T
		else
		    {
		    *bDD = (temp - *qDD) * 2;	// B = B * 2 - 2 * T
		    *a2DD = *aDD + *aDD;
		    }
		}
	    else if (variety == 'K')		// CKIN 
		{
		*aDD = 0;
		*vDD = 0;
		*bDD = *qDD;			// B = T
		}
	    *aa3DD = aDD->CSqr() * 3;		// AA3 = A*A*3
	    *zDD = -(*aDD);			// Z = -A
	    }
	else
	    *zDD = 0.0;
	}
    }

/**************************************************************************
   Initialise quad double 
**************************************************************************/

void	CSlope::InitFwdQD(BYTE juliaflag,
		QDComplex cQD, QDComplex *zQD, QDComplex *qQD, QDComplex *aQD, QDComplex *bQD, QDComplex *vQD, QDComplex *a2QD, QDComplex *aa3QD, 
		Complex j, Complex c, Complex *z, Complex *q)
    
    {
    QDComplex	t2QD, t3QD;
    if (juliaflag)
	{
	*qQD = j;
	*zQD = cQD;
	}
    else
	{
	*qQD = cQD;
	if (subtype == 4)			// Sine
	    *zQD = param[6];
	else if (subtype == 16)			// good old Newton
	    *zQD = cQD;
	else if (subtype == 15)			// init Art Matrix Newton
	    {
	    QDComplex	temp;

	    if (variety == 'B')			// CBIN 
		{
		t3QD = *qQD * 3;		// T3 = 3*T
		t2QD = qQD->CSqr();		// T2 = T*T
		*aQD = (t2QD + 1) / t3QD;	// A  = (T2 + 1)/T3
						// B  = 2*A*A*A + (T2 - 2)/T3    
		temp = aQD->CCube();		// A*A*A
		temp = temp * 2;		// 2*A*A*A
		*bQD = (t2QD - 2) / t3QD + temp;// B  = 2*A*A*A + (T2 - 2)/T3
		}
	    else if (variety == 'C' || variety == 'F')	// CCIN or CFIN
		{
		*aQD = *qQD;			// A = T
						// find B = T + 2*T*T*T
		temp = qQD->CCube();		// B = T*T*T
		if (variety == 'C')
		    *bQD = temp + temp + *qQD;	// B = B * 2 + T
		else
		    {
		    *bQD = (temp - *qQD) * 2;	// B = B * 2 - 2 * T
		    *a2QD = *aQD + *aQD;
		    }
		}
	    else if (variety == 'K')		// CKIN 
		{
		*aQD = 0;
		*vQD = 0;
		*bQD = *qQD;			// B = T
		}
	    *aa3QD = aQD->CSqr() * 3;		// AA3 = A*A*3
	    *zQD = -(*aQD);			// Z = -A
	    }
	else
	    *zQD = 0.0;
	}
    }

/**************************************************************************
   Initialise arbitrary precision
**************************************************************************/

void	CSlope::InitFwdBig(BYTE juliaflag,
		BigComplex cBig, BigComplex *zBig, BigComplex *qBig, BigComplex *aBig, BigComplex *bBig, BigComplex *vBig, BigComplex *a2Big, BigComplex *aa3Big, 
		Complex j, Complex c, Complex *z, Complex *q)
    {
    BigComplex	t2Big, t3Big;
    if (juliaflag)
	{
	*qBig = j;
	*zBig = cBig;
	}
    else
	{
	*qBig = cBig;
	if (subtype == 4)			// Sine
	    *zBig = param[6];
	else if (subtype == 16)			// good old Newton
	    *zBig = cBig;
	else if (subtype == 15)			// init Art Matrix Newton
	    {
	    BigComplex	temp;

	    if (variety == 'B')			// CBIN 
		{
		t3Big = *qBig * 3;		// T3 = 3*T
		t2Big = qBig->CSqr();		// T2 = T*T
		*aBig = (t2Big + 1) / t3Big;	// A  = (T2 + 1)/T3
						// B  = 2*A*A*A + (T2 - 2)/T3    
		temp = aBig->CCube();		// A*A*A
		temp = temp.CDouble();		// 2*A*A*A
		*bBig = (t2Big - 2) / t3Big + temp;// B  = 2*A*A*A + (T2 - 2)/T3
		}
	    else if (variety == 'C' || variety == 'F')	// CCIN or CFIN
		{
		*aBig = *qBig;			// A = T
						// find B = T + 2*T*T*T
		temp = qBig->CCube();		// B = T*T*T
		if (variety == 'C')
		    *bBig = temp + temp + *qBig;// B = B * 2 + T
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
	    *zBig = -(*aBig);			// Z = -A
	    }
	else
	    *zBig = 0.0;
	}
    }

/**************************************************************************
	Slope Fractal arbitrary precision
**************************************************************************/

int	CSlope::RunSlopeFwdDiffBig(int user_data(HWND hwnd), char* StatusBarInfo, bool *ThreadComplete, Complex j, double mandel_width, double hor, double vert, double xgap, double ygap,
								BigDouble Big_xgap, BigDouble Big_ygap, BigDouble BigHor, BigDouble BigVert, BigDouble BigWidth,
								double rqlim, long threshold, CDib *Dib, double *wpixels, BYTE juliaflag, int xdots, int ydots, int width)
    {
    Complex	z = 0.0;			// initial value for iteration Z0
    Complex	c, q;
    BigComplex	cBig, zBig, qBig;
    BigDouble	BigTemp, BigTemp1;
    BigComplex	t2Big, t3Big, aBig, bBig, vBig, a2Big, aa3Big;
    BigComplex	OldZBig, OlderZBig;
    double	log_zn, nu;
    int		lastChecked = -1;

    int		x, y;
    DWORD	index;
    long	ColourPtr;
    double	iterations;

    *ThreadComplete = false;
    for (y = 0; y < ydots; y++)
	{
	cBig.y = BigVert + BigWidth - Big_ygap * y;
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
	    cBig.x = BigHor + Big_xgap * x;
	    SpecialFlag = false;
	    InitFwdBig(juliaflag, cBig, &zBig, &qBig, &aBig, &bBig, &vBig, &a2Big, &aa3Big, j, c, &z, &q);

	    iterations = 0.0;
	    for (;;)
		{
		if (subtype == 16)				// good old Newton
		    {
		    OlderZBig = OldZBig;
		    OldZBig = zBig;
		    }
		iterations++;
		if (iterations >= threshold)
		    break;

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
		    d = zNewtonBig.CSumSqr();
		    if (d < MINSIZE)
			break;
		    }
		else if (zBig.CSumSqr() >= rqlim)
		    break;
		}
	    if (iterations < threshold)
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
	Slope Fractal quad double
**************************************************************************/

int	CSlope::RunSlopeFwdDiffQD(int user_data(HWND hwnd), char* StatusBarInfo, bool *ThreadComplete, Complex j, double mandel_width, double hor, double vert, double xgap, double ygap,
					BigDouble Big_xgap, BigDouble Big_ygap, BigDouble BigHor, BigDouble BigVert, BigDouble BigWidth,
					double rqlim, long threshold, CDib *Dib, double *wpixels, BYTE juliaflag, int xdots, int ydots, int width)
    {
    Complex	z = 0.0;			// initial value for iteration Z0
    Complex	c, q;

    QDComplex	cQD, zQD, qQD;
    QDComplex	QDTemp, QDTemp1;
    QDComplex	t2QD, t3QD, aQD, bQD, vQD, a2QD, aa3QD;
    QDComplex	OldZQD, OlderZQD;

    qd_real	QDhor, QDvert, QDxgap, QDygap, QDWidth;

    double	log_zn, nu;
    int		lastChecked = -1;

    int		x, y;
    DWORD	index;
    long	ColourPtr;
    double	iterations;

    *ThreadComplete = false;
    ConvertBignumsQD(Big_xgap, Big_ygap, BigHor, BigVert, BigWidth, &QDxgap, &QDygap, &QDhor, &QDvert, &QDWidth);
    for (y = 0; y < ydots; y++)
	{
	cQD.y = QDvert + QDWidth - QDygap * y;
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
	    cQD.x = QDhor + QDxgap * x;
	    SpecialFlag = false;
	    InitFwdQD(juliaflag, cQD, &zQD, &qQD, &aQD, &bQD, &vQD, &a2QD, &aa3QD, j, c, &z, &q);

	    iterations = 0.0;
	    for (;;)
		{
		if (subtype == 16)				// good old Newton
		    {
		    OlderZQD = OldZQD;
		    OldZQD = zQD;
		    }
		iterations++;
		if (iterations >= threshold)
		    break;

		DoQDSlopeFwdDiffFn(&zQD, &qQD, &bQD, &aa3QD);
		if (subtype == 15)				// init Art Matrix Cubic
		    {
		    if (zQD.CSumSqr() > 100.0)
			break;
		    else
			{
			if (variety == 'F')
			    {
			    if (qQD.CSumSqr() < 0.111111)
				{
				SpecialFlag = true;			// for decomp and biomorph
				break;
				}
			    vQD = zQD + a2QD;
			    }
			else if (variety == 'K')
			    vQD = zQD - vQD;
			else
			    vQD = zQD - aQD;
			if (vQD.CSumSqr() <= 0.000001)
			    {
			    SpecialFlag = true;			// for decomp and biomorph
			    break;
			    }
			}
		    }
		else if (subtype == 16)				// good old Newton
		    {
		    double	d;
		    d = zNewtonQD.CSumSqr();
		    if (d < MINSIZE)
			break;
		    }
		else if (zQD.CSumSqr() >= rqlim)
		    break;
		}
	    if (iterations < threshold)
		{
		double	t, ta, tb;
		QDComplex	aQD, bQD;
		qd_real	temp;
		if (subtype == 16)				// good old Newton
		    {
		    aQD = OldZQD - OlderZQD;
		    bQD = zQD - OldZQD;
		    temp = aQD.x * aQD.x + aQD.y * aQD.y;
		    ta = to_double(temp);
		    log_zn = log(MINSIZE) - log(ta);						// convergence method 1 (page 25 Fractal-Zoomer Algorithms.docx)
		    temp = bQD.x * bQD.x + bQD.y * bQD.y;
		    tb = to_double(temp);
		    t = log(tb) - log(ta);
		    nu = log_zn / t;
		    iterations = iterations + nu;
		    }
		else
		    {
		    // sqrt of inner term removed using log simplification rules.
		    temp = zQD.x * zQD.x + zQD.y * zQD.y;
		    t = to_double(temp);
		    log_zn = log(t) / SlopeDegree;
		    nu = log(log_zn / log(SlopeDegree)) / log(SlopeDegree);
		    iterations = iterations + 1 - nu;
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
	Slope Fractal double double
**************************************************************************/

int	CSlope::RunSlopeFwdDiffDD(int user_data(HWND hwnd), char* StatusBarInfo, bool *ThreadComplete, Complex j, double mandel_width, double hor, double vert, double xgap, double ygap,
					BigDouble Big_xgap, BigDouble Big_ygap, BigDouble BigHor, BigDouble BigVert, BigDouble BigWidth,
					double rqlim, long threshold, CDib *Dib, double *wpixels, BYTE juliaflag, int xdots, int ydots, int width)
    {
    Complex	z = 0.0;			// initial value for iteration Z0
    Complex	c, q;

    DDComplex	cDD, zDD, qDD;
    DDComplex	DDTemp, DDTemp1;
    DDComplex	t2DD, t3DD, aDD, bDD, vDD, a2DD, aa3DD;
    DDComplex	OldZDD, OlderZDD;

    dd_real	DDhor, DDvert, DDxgap, DDygap, DDWidth;

    double	log_zn, nu;
    int		lastChecked = -1;

    int		x, y;
    DWORD	index;
    long	ColourPtr;
    double	iterations;

    *ThreadComplete = false;
    ConvertBignumsDD(Big_xgap, Big_ygap, BigHor, BigVert, BigWidth, &DDxgap, &DDygap, &DDhor, &DDvert, &DDWidth);

    for (y = 0; y < ydots; y++)
	{
	cDD.y = DDvert + DDWidth - DDygap * y;
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
	    cDD.x = DDhor + DDxgap * x;
	    SpecialFlag = false;
	    InitFwdDD(juliaflag, cDD, &zDD, &qDD, &aDD, &bDD, &vDD, &a2DD, &aa3DD, j, c, &z, &q);
	    iterations = 0.0;
	    for (;;)
		{
		if (subtype == 16)				// good old Newton
		    {
		    OlderZDD = OldZDD;
		    OldZDD = zDD;
		    }
		iterations++;
		if (iterations >= threshold)
		    break;

		DoDDSlopeFwdDiffFn(&zDD, &qDD, &bDD, &aa3DD);
		if (subtype == 15)				// init Art Matrix Cubic
		    {
		    if (zDD.CSumSqr() > 100.0)
			break;
		    else
			{
			if (variety == 'F')
			    {
			    if (qDD.CSumSqr() < 0.111111)
				{
				SpecialFlag = true;			// for decomp and biomorph
				break;
				}
			    vDD = zDD + a2DD;
			    }
			else if (variety == 'K')
			    vDD = zDD - vDD;
			else
			    vDD = zDD - aDD;
			if (vDD.CSumSqr() <= 0.000001)
			    {
			    SpecialFlag = true;			// for decomp and biomorph
			    break;
			    }
			}
		    }
		else if (subtype == 16)				// good old Newton
		    {
		    double	d;
		    d = zNewtonDD.CSumSqr();
		    if (d < MINSIZE)
			break;
		    }
		else if (zDD.CSumSqr() >= rqlim)
		    break;
		}
	    if (iterations < threshold)
		{
		double	t, ta, tb;
		DDComplex	aDD, bDD;
		dd_real	temp;
		if (subtype == 16)				// good old Newton
		    {
		    aDD = OldZDD - OlderZDD;
		    bDD = zDD - OldZDD;
		    temp = aDD.x * aDD.x + aDD.y * aDD.y;
		    ta = to_double(temp);
		    log_zn = log(MINSIZE) - log(ta);						// convergence method 1 (page 25 Fractal-Zoomer Algorithms.docx)
		    temp = bDD.x * bDD.x + bDD.y * bDD.y;
		    tb = to_double(temp);
		    t = log(tb) - log(ta);
		    nu = log_zn / t;
		    iterations = iterations + nu;
		    }
		else
		    {
		    // sqrt of inner term removed using log simplification rules.
		    temp = zDD.x * zDD.x + zDD.y * zDD.y;
		    t = to_double(temp);
		    log_zn = log(t) / SlopeDegree;
		    nu = log(log_zn / log(SlopeDegree)) / log(SlopeDegree);
		    iterations = iterations + 1 - nu;
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
	Slope Fractal Double Float 
**************************************************************************/

int	CSlope::RunSlopeFwdDiffDouble(int user_data(HWND hwnd), char* StatusBarInfo, bool *ThreadComplete, Complex j, double mandel_width, double hor, double vert, double xgap, double ygap,
	double rqlim, long threshold, CDib *Dib, double *wpixels, BYTE juliaflag, int xdots, int ydots, int width)
    {
    Complex	z = 0.0;			// initial value for iteration Z0
    Complex	c, q;
    double	log_zn, nu;
    int		lastChecked = -1;
    int		x, y;
    DWORD	index;
    long	ColourPtr;
    double	iterations;
    Complex	OldZ, OlderZ;

    *ThreadComplete = false;
    for (y = 0; y < ydots; y++)
	{
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

	    c.x = hor + x * xgap;
	    SpecialFlag = false;
	    InitFwdDouble(juliaflag, j, c, &z, &q);
	    iterations = 0.0;
	    for (;;)
		{
		if (subtype == 16)				// good old Newton
		    {
		    OlderZ = OldZ;
		    OldZ = z;
		    }
		iterations++;
		if (iterations >= threshold)
		    break;

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
	    if (iterations < threshold)
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
//			break;

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
    Slope Fractal
**************************************************************************/

int	CSlope::RunSlopeFwdDiff(HWND hwndIn, int user_data(HWND hwnd), char* StatusBarInfo, bool *ThreadComplete, int subtypeIn, int NumThreadsIn, int threadIn, Complex j, double mandel_width, double hor, double vert, double xgap, double ygap,
	BYTE BigNumFlag, BigDouble Big_xgap, BigDouble Big_ygap, BigDouble BigHor, BigDouble BigVert, BigDouble BigWidth, double rqlim, long threshold, double paramIn[], CTrueCol *TrueCol, CDib *Dib, double *wpixels, BYTE juliaflag, int xdots, 
	int ydots, int width, WORD *degreeIn, int precision)
	{
	int	i;
	
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
	    if (precision <= 30)
		{
		ArithType = DOUBLEDOUBLE;
		return RunSlopeFwdDiffDD(user_data, StatusBarInfo, ThreadComplete, j, mandel_width, hor, vert, xgap, ygap, Big_xgap, Big_ygap, BigHor, BigVert, BigWidth, rqlim, threshold, Dib, wpixels, juliaflag, xdots, ydots, width);
		}
	    else if (precision <= 60)
		{
		ArithType = QUADDOUBLE;
		return RunSlopeFwdDiffQD(user_data, StatusBarInfo, ThreadComplete, j, mandel_width, hor, vert, xgap, ygap, Big_xgap, Big_ygap, BigHor, BigVert, BigWidth, rqlim, threshold, Dib, wpixels, juliaflag, xdots, ydots, width);
		}
	    else
		{
		ArithType = ARBITRARYPREC;
		return RunSlopeFwdDiffBig(user_data, StatusBarInfo, ThreadComplete, j, mandel_width, hor, vert, xgap, ygap, Big_xgap, Big_ygap, BigHor, BigVert, BigWidth, rqlim, threshold, Dib, wpixels, juliaflag, xdots, ydots, width);
		}
	    }
	else
	    {
	    ArithType = DOUBLEFLOAT;
	    return RunSlopeFwdDiffDouble(user_data, StatusBarInfo, ThreadComplete, j, mandel_width, hor, vert, xgap, ygap, rqlim, threshold, Dib, wpixels, juliaflag, xdots, ydots, width);
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

