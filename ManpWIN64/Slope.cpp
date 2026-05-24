/*
    SLOPE.CPP a module for the per pixel calculations of fractals. 
    Written in Microsoft Visual 'C++' by Paul de Leeuw.
*/

#include	<atomic>
#include	"slope.h"
#include	"manp.h"
#include	"DerivSlopeTemplate.h"

//extern	std::vector<float> wpixels;
extern	std::atomic<bool> gStopRequested;	// force early exit
extern	std::atomic<long> gPixelsDone;

CSlope::CSlope()
    {
    }

CSlope::CSlope(std::vector<float>& wp)
    {
    }

/***********************************************************************
	Convert bignum variables to double double
***********************************************************************/

int	CSlope::ConvertBignumsDD(BigDouble Big_xgap, BigDouble Big_ygap, BigDouble BigHor, BigDouble BigVert, BigDouble BigWidth,
	dd_real *DDxgap, dd_real *DDygap, dd_real *DDhor, dd_real *DDvert, dd_real *DDWidth)
    {
    if (Big_xgap.BigDouble2DD(DDxgap) < 0) return -1;
    if (Big_ygap.BigDouble2DD(DDygap) < 0) return -1;
    if (BigHor.BigDouble2DD(DDhor) < 0) return -1;
    if (BigVert.BigDouble2DD(DDvert) < 0) return -1;
    if (BigWidth.BigDouble2DD(DDWidth) < 0) return -1;
    return 0;
    }

/***********************************************************************
	Convert bignum variables to quad double
***********************************************************************/

int	CSlope::ConvertBignumsQD(BigDouble Big_xgap, BigDouble Big_ygap, BigDouble BigHor, BigDouble BigVert, BigDouble BigWidth,
    qd_real *QDxgap, qd_real *QDygap, qd_real *QDhor, qd_real *QDvert, qd_real *QDWidth)
    {
    if (Big_xgap.BigDouble2QD(QDxgap) < 0) return -1;
    if (Big_ygap.BigDouble2QD(QDygap) < 0) return -1;
    if (BigHor.BigDouble2QD(QDhor) < 0) return -1;
    if (BigVert.BigDouble2QD(QDvert) < 0) return -1;
    if (BigWidth.BigDouble2QD(QDWidth) < 0) return -1;
    return 0;
    }

/**************************************************************************
	Caluclate the colour of the pixel
**************************************************************************/

RGBTRIPLE CSlope::compute_colour(CTrueCol *TrueCol, Complex j, BYTE juliaflag, Complex c, BigComplex cBig, QDComplex cQD, DDComplex cDD, double rqlim, long threshold, BYTE ArithType, Complex v, 
	    bool *Time2Exit, int &iterations, double &reflectionOut, double &smoothIterations)
    {
    double	reflection;
    BYTE	b;
    RGBTRIPLE	colour;
    int		PaletteStart = (int)(fabs(param[2]));

    *Time2Exit = false;

    switch (ArithType)
	{
	case DOUBLEFLOAT:
	    reflection = GiveReflectionT(j, juliaflag, c, iterations, smoothIterations, rqlim, threshold, v, param, subtype, variety, degree, smoothing);
	    break;
	case DOUBLEDOUBLE:
	    reflection = GiveReflectionT(j, juliaflag, cDD, iterations, smoothIterations, rqlim, threshold, v, param, subtype, variety, degree, smoothing);
	    break;
	case QUADDOUBLE:
	    reflection = GiveReflectionT(j, juliaflag, cQD, iterations, smoothIterations, rqlim, threshold, v, param, subtype, variety, degree, smoothing);
	    break;
	case ARBITRARYPREC:
	    reflection = GiveReflectionT(j, juliaflag, cBig, iterations, smoothIterations, rqlim, threshold, v, param, subtype, variety, degree, smoothing);
	    break;
	}

    reflectionOut = reflection;

    if (reflection < 0)
	*Time2Exit = true;
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
	    // colours swap = don't me??? But it works
	    colour.rgbtRed = (BYTE)((reflection)   * (double)(TrueCol->PalettePtr[(BYTE)((iterations) % 256)].rgbtBlue));	// Red
	    colour.rgbtGreen = (BYTE)((reflection) * (double)(TrueCol->PalettePtr[(BYTE)((iterations) % 256)].rgbtGreen));	// Green
	    colour.rgbtBlue = (BYTE)((reflection)  * (double)(TrueCol->PalettePtr[(BYTE)((iterations) % 256)].rgbtRed));	// Blue
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

int CSlope::RunSlopeDerivative(HWND hwndIn, int user_data(HWND hwnd), char* StatusBarInfo, bool *ThreadComplete, int subtypeIn, int NumThreadsIn, int threadIn, Complex j, double mandel_width, double hor, 
					    double vert, BYTE BigNumFlag, BigDouble BigHor, BigDouble BigVert, BigDouble BigWidth, double rqlim, long threshold, double paramIn[], CTrueCol *TrueCol, CDib *Dib, 
					    BYTE juliaflag, int xdots, int ydots, int width, int height, WORD *degreeIn, int precisionIn, double ColourSpeedIn, HANDLE ghMutex, std::vector<std::pair<int, int>> *pixelOrder,
					    std::atomic<int> *workIndex, int totalPixels)
    {
    Complex	c;
    BigComplex	cBig;
    DDComplex	cDD;
    QDComplex	cQD;
    double	xgap, ygap;
    dd_real	DDhor, DDvert, DDxgap, DDygap, DDWidth;
    qd_real	QDhor, QDvert, QDxgap, QDygap, QDWidth;
    RGBTRIPLE	colour = { 0, 0, 0 };
    int		lastChecked = -1;
    Complex	v;// unit 2D vector in this direction
    int		i;
    bool	Time2Exit = false;
    CPlot	Plot;
    int		iterations = 0;
    double	SmoothIterations = 0.0;
    double	ReflectionLocal = 1.0;

    for (i = 0; i < NUMSLOPEDERIVPARAM; i++)
	param[i] = paramIn[i];

    double	LightAngle = param[0];

    precision = precisionIn;
    thread = threadIn;
    NumThreads = NumThreadsIn;
    subtype = subtypeIn;
    hwnd = hwndIn;
    bits_per_pixel = Dib->BitsPerPixel;
    degree = degreeIn;
    ColourSpeed = ColourSpeedIn;
    smoothing = (ColourSpeed != 0.0);

    gStopRequested = false; // NEW: reset flag before starting threads

    Create2DVector(&v, LightAngle);

    long TotalPixels = (long)xdots * ydots;
    *ThreadComplete = false;

    double	temp_x, temp_y;
    double	ScreenRatio = (double) xdots / (double) ydots;

    temp_x = ScreenRatio / (double)(xdots - 1);
    temp_y = 1.0 / (double)(ydots - 1);

    if (BigNumFlag)
	{
	Big_xgap = BigWidth * temp_x;
	Big_ygap = BigWidth * temp_y;
	if (precision <= DDPRECISION)
	    {
	    ConvertBignumsDD(Big_xgap, Big_ygap, BigHor, BigVert, BigWidth, &DDxgap, &DDygap, &DDhor, &DDvert, &DDWidth);
	    ArithType = DOUBLEDOUBLE;
	    }
	else if (precision <= QDPRECISION)
	    {
	    ConvertBignumsQD(Big_xgap, Big_ygap, BigHor, BigVert, BigWidth, &QDxgap, &QDygap, &QDhor, &QDvert, &QDWidth);
	    ArithType = QUADDOUBLE;
	    }
	else
	    ArithType = ARBITRARYPREC;
	}
    else
	{
	ArithType = DOUBLEFLOAT;
	xgap = mandel_width * temp_x;
	ygap = mandel_width * temp_y;
	}
    Plot.InitPlot(threshold, TrueCol, &gManp->wpixels, xdots, height, xdots, height, Dib->BitsPerPixel, Dib, USEPALETTE);
    PlotMode mode = currentMode;
    int	chunk = (currentMode == PlotMode::Tile) ? 1024 : CHUNK_SIZE;

     if (workIndex < 0 || *workIndex >= totalPixels)
	{
	OutputDebugStringA("Deriv Slope: invalid workIndex\n");
	return -1;
	}

    if (pixelOrder->empty())
	{
	OutputDebugStringA("Deriv Slope: pixelOrder empty\n");
	return -1;
	}

    if (subtype == 13) // init Art Matrix Cubic 
	{ 
	switch ((int)param[3]) 
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
	SpecialColour.rgbtRed = (BYTE)param[3];
	SpecialColour.rgbtGreen = (BYTE)param[4];
	SpecialColour.rgbtBlue = (BYTE)param[5];
	SpecialFlag = false; 
	}

    while (true)
	{
	int start = workIndex->fetch_add(chunk, std::memory_order_relaxed);
	if (start >= totalPixels)
	    break;

	for (int k = 0; k < chunk; k++)
	    {
	    int idx = start + k;
	    if (idx >= totalPixels)
		break;

	    int iX, iY;

	    if (mode == PlotMode::Scanline)
		{
		iY = (idx / xdots);
		iX = idx % xdots;
		}
	    else
		{
		iX = (*pixelOrder)[idx].first;
		iY = ydots - 1 - (*pixelOrder)[idx].second;
		}

	    switch (ArithType)
		{
		case DOUBLEFLOAT:
		    c.x = hor + iX * xgap;
		    c.y = vert + mandel_width - iY * ygap;
		    break;
		case DOUBLEDOUBLE:
		    cDD.x = DDhor + DDxgap * iX;
		    cDD.y = DDvert + DDWidth - DDygap * iY;
		    break;
		case QUADDOUBLE:
		    cQD.x = QDhor + QDxgap * iX;
		    cQD.y = QDvert + QDWidth - QDygap * iY;
		    break;
		case ARBITRARYPREC:
		    cBig.x = BigHor + Big_xgap * iX;
		    cBig.y = BigVert + BigWidth - Big_ygap * iY;
		    break;
		}

	    if ((idx & 0x3FF) == 0)
		{
		gManp->UpdateProgress(workIndex, totalPixels, StatusBarInfo, NumThreads);
		}

	    // --- pre-check ONLY for Art Matrix ---
	    if (subtype == 13)
		{
		bool isSpecial = false;

		switch (ArithType)
		    {
		    case DOUBLEFLOAT:
			isSpecial = IsArtMatrixSpecialPixelT(c, j, juliaflag, param, threshold);
			break;

		    case DOUBLEDOUBLE:
			isSpecial = IsArtMatrixSpecialPixelT(cDD, j, juliaflag, param, threshold);
			break;

		    case QUADDOUBLE:
			isSpecial = IsArtMatrixSpecialPixelT(cQD, j, juliaflag, param, threshold);
			break;

		    case ARBITRARYPREC:
			isSpecial = IsArtMatrixSpecialPixelT(cBig, j, juliaflag, param, threshold);
			break;
		    }

		if (isSpecial)
		    {
		    RGBTRIPLE colour;
		    colour.rgbtRed = (int)param[4];
		    colour.rgbtGreen = (int)param[5];
		    colour.rgbtBlue = (int)param[6];
		    Plot.OutRGBpoint(iX, iY, colour);

		    continue;   // THIS is the key
		    }
		}

	    colour = compute_colour(TrueCol, j, juliaflag, c, cBig, cQD, cDD, rqlim, threshold, ArithType, v, &Time2Exit, iterations, ReflectionLocal, SmoothIterations);
	    if (Time2Exit)
		return 0;
	    if (EndSlope)		// program is trying to terminate. Better get out of the thread
		{
		*ThreadComplete = true;
		return -1;
		}
	    if (AbortRequested())
		return -1;
	    if (smoothing)
		{
		// INSIDE set -> show user-defined inside colour
		if (iterations == threshold)
		    {
		    RGBTRIPLE inside;
		    inside.rgbtRed = (BYTE)TrueCol->InsideRed;
		    inside.rgbtGreen = (BYTE)TrueCol->InsideGreen;
		    inside.rgbtBlue = (BYTE)TrueCol->InsideBlue;
		    if (ghMutex != NULL)
			WaitForSingleObject(ghMutex, INFINITE);  // no time-out interval
		    Plot.OutRGBpoint(iX, iY, inside);
		    if (ghMutex != NULL)
			ReleaseMutex(ghMutex);
		    }
		else
		    {
		    // Normal exterior smoothing
		    RGBTRIPLE base = GetSmoothedColour(SmoothIterations, ColourSpeed, *TrueCol, &Plot);

		    double br = ReflectionLocal;

		    RGBTRIPLE final;
		    final.rgbtRed = (BYTE)(base.rgbtRed   * br);
		    final.rgbtGreen = (BYTE)(base.rgbtGreen * br);
		    final.rgbtBlue = (BYTE)(base.rgbtBlue  * br);
		    // plot using thread-local plot
		    if (AbortRequested())
			return -1;

		    // hard bounds check using the dimensions this worker is responsible for
		    if ((unsigned)iX >= (unsigned)Dib->DibWidth || (unsigned)iY >= (unsigned)Dib->DibHeight)
			return -1;

		    if (ghMutex != NULL) 
			WaitForSingleObject(ghMutex, INFINITE);
		    Plot.OutRGBpoint(iX, iY, final);
		    if (ghMutex != NULL)
			ReleaseMutex(ghMutex);
		    }
		}
	    else
		{
		if (ghMutex != NULL)
		    WaitForSingleObject(ghMutex, INFINITE);
		Plot.OutRGBpoint(iX, iY, colour);
		if (ghMutex != NULL)
		    ReleaseMutex(ghMutex);
		}

	    if (user_data(hwnd) < 0)
		return -1;
	    if (AbortRequested())
		return -1;

	    gPixelsDone.fetch_add(1, std::memory_order_relaxed);
	    }
	}
	*ThreadComplete = true;
	return 0;
    }

