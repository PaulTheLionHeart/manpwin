/*
    SLOPE.CPP - a module for the per pixel calculations of fractals. 

    Written in Microsoft Visual 'C++' by Paul de Leeuw.
*/

#include <atomic>
#include "slope.h"
#include "manp.h"
#include "DerivSlopeTemplate.h"
#include "FilterTemplate.h"

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
	Calculate the colour of the pixel
**************************************************************************/

RGBTRIPLE CSlope::compute_colour(CTrueCol *TrueCol, CPlot *Plot, Complex j, BYTE juliaflag, Complex c, BigComplex cBig, QDComplex cQD, DDComplex cDD, double rqlim, long threshold, BYTE ArithType, Complex v,
	    bool *Time2Exit, int &iterations, double &smoothIterations)
    {
    double	reflection;
    RGBTRIPLE	colour;
    int		PaletteStart = (int)(fabs(param[2]));
    double	min_orbit = 0.0;
    long	min_index = 0;

    Complex	FinalZ;
    DDComplex   FinalZDD;
    QDComplex   FinalZQD;
    BigComplex  FinalZBig;

    *Time2Exit = false;

    switch (ArithType)
	{
	case DOUBLEFLOAT:
	    reflection = GiveReflectionT(j, juliaflag, c, iterations, smoothIterations, rqlim, threshold, v, param, subtype, variety, degree, smoothing, InsideMethod, min_orbit, min_index, FinalZ);
	    break;
	case DOUBLEDOUBLE:
	    reflection = GiveReflectionT(j, juliaflag, cDD, iterations, smoothIterations, rqlim, threshold, v, param, subtype, variety, degree, smoothing, InsideMethod, min_orbit, min_index, FinalZDD);
	    break;
	case QUADDOUBLE:
	    reflection = GiveReflectionT(j, juliaflag, cQD, iterations, smoothIterations, rqlim, threshold, v, param, subtype, variety, degree, smoothing, InsideMethod, min_orbit, min_index, FinalZQD);
	    break;
	case ARBITRARYPREC:
	    reflection = GiveReflectionT(j, juliaflag, cBig, iterations, smoothIterations, rqlim, threshold, v, param, subtype, variety, degree, smoothing, InsideMethod, min_orbit, min_index, FinalZBig);
	    break;
	}

    if (reflection < 0)
	*Time2Exit = true;

    int index = ApplySlopeFilters(iterations, ArithType, FinalZ, FinalZDD, FinalZQD, FinalZBig, threshold, rqlim, min_orbit, min_index, TrueCol);

    if (iterations >= threshold)
	{
	// Inside point
	if (InsideMethod != NONE)
	    {
	    // Inside filter determines the palette index.
	    // There is no slope reflection for an inside point.
	    colour.rgbtRed = TrueCol->PalettePtr[(BYTE)(index % 256)].rgbtBlue;
	    colour.rgbtGreen = TrueCol->PalettePtr[(BYTE)(index % 256)].rgbtGreen;
	    colour.rgbtBlue = TrueCol->PalettePtr[(BYTE)(index % 256)].rgbtRed;
	    }
	else
	    {
	    colour.rgbtRed = (BYTE)TrueCol->InsideRed;
	    colour.rgbtGreen = (BYTE)TrueCol->InsideGreen;
	    colour.rgbtBlue = (BYTE)TrueCol->InsideBlue;
	    }
	}
    else
	{
	// Outside point.
	// PaletteStart determines whether the pre-palette colour or normal
	// palette colouring supplies the base colour.  Smoothing only affects
	// normal palette colouring after PaletteStart.
	if (index < PaletteStart)
	    {
	    colour.rgbtRed = (BYTE)(reflection * (double)((gManp->PrePaletteColour >> 16) & 0xff));
	    colour.rgbtGreen = (BYTE)(reflection * (double)((gManp->PrePaletteColour >> 8) & 0xff));
	    colour.rgbtBlue = (BYTE)(reflection * (double)(gManp->PrePaletteColour & 0xff));
	    }
	else if (smoothing && (OutsideMethod == NONE || OutsideMethod >= TIERAZONFILTERS) && biomorph < 0)
	    {
	    RGBTRIPLE base = GetSmoothedColour(smoothIterations, ColourSpeed, *TrueCol, Plot);

	    colour.rgbtRed = (BYTE)(base.rgbtRed * reflection);
	    colour.rgbtGreen = (BYTE)(base.rgbtGreen * reflection);
	    colour.rgbtBlue = (BYTE)(base.rgbtBlue * reflection);
	    }
	else
	    {
	    // Filtered or normal iteration colouring supplies the base
	    // colour; Derivative Slope always supplies the lighting.
	    // colours swap = don't me??? But it works
	    colour.rgbtRed = (BYTE)(reflection * (double)TrueCol->PalettePtr[(BYTE)(index % 256)].rgbtBlue);
	    colour.rgbtGreen = (BYTE)(reflection * (double)TrueCol->PalettePtr[(BYTE)(index % 256)].rgbtGreen);
	    colour.rgbtBlue = (BYTE)(reflection * (double)TrueCol->PalettePtr[(BYTE)(index % 256)].rgbtRed);
	    }

/*

	    {
	    // Filtered or normal iteration colouring supplies the base
	    // colour; Derivative Slope always supplies the lighting.
	    if (index >= PaletteStart)
		{
		// colours swap = don't me??? But it works
		colour.rgbtRed = (BYTE)(reflection * (double)TrueCol->PalettePtr[(BYTE)(index % 256)].rgbtBlue);
		colour.rgbtGreen = (BYTE)(reflection * (double)TrueCol->PalettePtr[(BYTE)(index % 256)].rgbtGreen);
		colour.rgbtBlue = (BYTE)(reflection * (double)TrueCol->PalettePtr[(BYTE)(index % 256)].rgbtRed);
		}
	    else
		{
		colour.rgbtRed = (BYTE)(reflection * (double)((gManp->PrePaletteColour >> 16) & 0xff));
		colour.rgbtGreen = (BYTE)(reflection * (double)((gManp->PrePaletteColour >> 8) & 0xff));
		colour.rgbtBlue = (BYTE)(reflection * (double)(gManp->PrePaletteColour & 0xff));
		}
	    }
*/
	}

    return colour;
    }

/**************************************************************************
	Apply ordinary inside and outside filters for Derivative Slope
**************************************************************************/

long CSlope::ApplySlopeFilters(int iterations, BYTE ArithType, Complex &FinalZ, DDComplex &FinalZDD, QDComplex &FinalZQD, BigComplex &FinalZBig, long threshold, double rqlim, double min_orbit, long min_index, CTrueCol *TrueCol)
    {
    int	index;
    Complex filterZ;

    switch (ArithType)
	{
	case DOUBLEFLOAT:
	    filterZ = FinalZ;
	    break;

	case DOUBLEDOUBLE:
	    filterZ.x = to_double(FinalZDD.x);
	    filterZ.y = to_double(FinalZDD.y);
	    break;

	case QUADDOUBLE:
	    filterZ.x = to_double(FinalZQD.x);
	    filterZ.y = to_double(FinalZQD.y);
	    break;

	case ARBITRARYPREC:
	    filterZ = FinalZBig.CBig2Double();
	    break;
	}

    if (OutsideMethod == NONE && InsideMethod == NONE && biomorph < 0)
	{
	index = iterations;
	}
    else
	{
	int hooper = 0;
	int paletteColours = TrueCol->ColoursInPALFile;
	int decomp = 0;
	int special = 0;
	int logval = 0;
	int potentialColours = TrueCol->ColoursInPALFile;
	BYTE *logtable = NULL;

	if (iterations < threshold)
	    {
	    if (OutsideMethod < TIERAZONFILTERS)
		{
		index = DoOutsideFilterT<Complex, double>(iterations, filterZ, OutsideMethod, hooper, threshold, paletteColours, decomp, biomorph, rqlim, special, logval, logtable, potentialColours, TrueCol, potparam);
		}
	    else
		index = iterations;		// Tierazon deliberately not supported in Slope
	    }
	else
	    {
	    index = DoInsideFilterT(iterations, filterZ, InsideMethod, threshold, min_orbit, min_index);
	    }
	}
    return index;
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

int CSlope::RunSlopeDerivative(HWND hwndIn, int user_data(HWND hwnd), char* StatusBarInfo, int subtypeIn, int NumThreadsIn, int threadIn, Complex j, double mandel_width, double hor, 
					    double vert, BYTE BigNumFlag, BigDouble BigHor, BigDouble BigVert, BigDouble BigWidth, double rqlim, long threshold, double paramIn[], CTrueCol *TrueCol, CDib *Dib, 
					    BYTE juliaflag, int xdots, int ydots, int width, int height, WORD *degreeIn, int precisionIn, double ColourSpeedIn, HANDLE ghMutex, std::vector<std::pair<int, int>> *pixelOrder,
					    std::atomic<int> *workIndex, int totalPixels, PlotMode mode)
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
    Complex	v;			// unit 2D vector in this direction
    bool	Time2Exit = false;
    CPlot	Plot;
    int		iterations = 0;
    double	SmoothIterations = 0.0;
    double	ReflectionLocal = 1.0;

    if (InitialiseSlopeDerivativeWorker(hwndIn, subtypeIn, NumThreadsIn, threadIn, BigNumFlag, BigHor, BigVert, BigWidth, mandel_width, paramIn, TrueCol, Dib, xdots, ydots, height, degreeIn, precisionIn, ColourSpeedIn, 
	    workIndex, totalPixels, pixelOrder, v, xgap, ygap, DDhor, DDvert, DDxgap, DDygap, DDWidth, QDhor, QDvert, QDxgap, QDygap, QDWidth, gManp->InsideMethod, gManp->OutsideMethod, gManp->biomorph, gManp->potparam, &Plot) < 0)
	return -1;

    int chunk = (currentMode == PlotMode::Tile) ? 1024 : CHUNK_SIZE;

    if (subtype == 13)				// init Art Matrix Cubic 
	InitialiseArtMatrixSlope();

    while (true)
	{
	int start = workIndex->fetch_add(chunk, std::memory_order_relaxed);
	if (start >= totalPixels)
	    break;

	for (int k = 0; k < chunk; k++)
	    {
	    int iX, iY;
	    int idx = start + k;

	    if (!SetSlopeCoordinate(idx, totalPixels, xdots, ydots, mode, pixelOrder, iX, iY, ArithType, c, cBig, cQD, cDD, hor, vert, mandel_width, xgap, ygap,
		BigHor, BigVert, BigWidth, Big_xgap, Big_ygap, DDhor, DDvert, DDWidth, DDxgap, DDygap, QDhor, QDvert, QDWidth, QDxgap, QDygap))
		break;
		
	    if ((idx & 0x3FF) == 0)
		{
		gManp->UpdateProgress(workIndex, totalPixels, StatusBarInfo, NumThreads);
		}

	    // --- pre-check ONLY for Art Matrix ---
	    if (subtype == 13 && PlotArtMatrixSpecialPixel(iX, iY, ArithType, c, cBig, cQD, cDD, j, juliaflag, threshold, &Plot))
		continue;

	    colour = compute_colour(TrueCol, &Plot, j, juliaflag, c, cBig, cQD, cDD, rqlim, threshold, ArithType, v, &Time2Exit, iterations, SmoothIterations);
	    if (Time2Exit)
		return 0;
	    if (EndSlope)		// program is trying to terminate. Better get out of the thread
		{
		return -1;
		}
	    if (AbortRequested())
		return -1;

	    // hard bounds check using the dimensions this worker is responsible for
	    if ((unsigned)iX >= (unsigned)Dib->DibWidth || (unsigned)iY >= (unsigned)Dib->DibHeight)
		return -1;

	    if (ghMutex != NULL)
		WaitForSingleObject(ghMutex, INFINITE);

	    Plot.OutRGBpoint(iX, iY, colour);

	    if (ghMutex != NULL)
		ReleaseMutex(ghMutex);
	    if (user_data(hwnd) < 0)
		return -1;
	    if (AbortRequested())
		return -1;

	    gPixelsDone.fetch_add(1, std::memory_order_relaxed);
	    }
	}
    return 0;
    }

/**************************************************************************
	Initialise Derivative Slope worker
**************************************************************************/

int CSlope::InitialiseSlopeDerivativeWorker(HWND hwndIn, int subtypeIn, int NumThreadsIn, int threadIn, BYTE BigNumFlag, BigDouble BigHor, BigDouble BigVert, BigDouble BigWidth, double mandel_width, 
	double paramIn[], CTrueCol *TrueCol, CDib *Dib, int xdots, int ydots, int height, WORD *degreeIn, int precisionIn, double ColourSpeedIn, std::atomic<int> *workIndex, int totalPixels,
	std::vector<std::pair<int, int>> *pixelOrder, Complex &v, double &xgap, double &ygap, dd_real &DDhor, dd_real &DDvert, dd_real &DDxgap, dd_real &DDygap, dd_real &DDWidth,
	qd_real &QDhor, qd_real &QDvert, qd_real &QDxgap, qd_real &QDygap, qd_real &QDWidth, int InsideMethodIn, int OutsideMethodIn, int biomorphIn, double *potparamIn, CPlot *Plot)
    {
    for (int i = 0; i < NUMSLOPEPARAM; i++)
	param[i] = paramIn[i];

    double LightAngle = param[0];
    gManp->ColourSpeed = param[3];

    precision = precisionIn;
    thread = threadIn;
    NumThreads = NumThreadsIn;
    subtype = subtypeIn;
    hwnd = hwndIn;
    bits_per_pixel = Dib->BitsPerPixel;
    degree = degreeIn;
    InsideMethod = InsideMethodIn;
    OutsideMethod = OutsideMethodIn;
    biomorph = biomorphIn;
    potparam = potparamIn;

    ColourSpeed = ColourSpeedIn;
    smoothing = (ColourSpeed != 0.0);

    Create2DVector(&v, LightAngle);

    double ScreenRatio = (double)xdots / (double)ydots;
    double temp_x = ScreenRatio / (double)(xdots - 1);
    double temp_y = 1.0 / (double)(ydots - 1);

    if (BigNumFlag)
	{
	Big_xgap = BigWidth * temp_x;
	Big_ygap = BigWidth * temp_y;

	if (precision <= DDPRECISION)
	    {
	    ConvertBignumsDD(Big_xgap, Big_ygap, BigHor, BigVert, BigWidth,
			    &DDxgap, &DDygap, &DDhor, &DDvert, &DDWidth);
	    ArithType = DOUBLEDOUBLE;
	    }
	else if (precision <= QDPRECISION)
	    {
	    ConvertBignumsQD(Big_xgap, Big_ygap, BigHor, BigVert, BigWidth,
			    &QDxgap, &QDygap, &QDhor, &QDvert, &QDWidth);
	    ArithType = QUADDOUBLE;
	    }
	else
	    {
	    ArithType = ARBITRARYPREC;
	    }
	}
    else
	{
	ArithType = DOUBLEFLOAT;
	xgap = mandel_width * temp_x;
	ygap = mandel_width * temp_y;
	}

    Plot->InitPlot(threshold, TrueCol, &gManp->wpixels,
		   xdots, height, xdots, height,
		   Dib->BitsPerPixel, Dib, USEPALETTE);

    int wi = workIndex->load(std::memory_order_relaxed);
    if (wi < 0 || wi >= totalPixels)
	{
	OutputDebugStringA("Deriv Slope: invalid workIndex\n");
	return -1;
	}

    if (pixelOrder->empty())
	{
	OutputDebugStringA("Deriv Slope: pixelOrder empty\n");
	return -1;
	}

    return 0;
    }

/**************************************************************************
    Calculate pixel and fractal coordinates for Derivative Slope
**************************************************************************/

bool	CSlope::SetSlopeCoordinate(int idx, int totalPixels, int xdots, int ydots, PlotMode mode, std::vector<std::pair<int, int>> *pixelOrder, int &iX, int &iY, BYTE ArithType, Complex &c, BigComplex &cBig, QDComplex &cQD, DDComplex &cDD,
	double hor, double vert, double mandel_width, double xgap, double ygap, BigDouble BigHor, BigDouble BigVert, BigDouble BigWidth, BigDouble Big_xgap, BigDouble Big_ygap, dd_real DDhor, dd_real DDvert, dd_real DDWidth,
	dd_real DDxgap, dd_real DDygap, qd_real QDhor, qd_real QDvert, qd_real QDWidth, qd_real QDxgap, qd_real QDygap)
    {
    if (idx >= totalPixels)
	return false;

    if (mode == PlotMode::Scanline)
	{
	iY = idx / xdots;
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
    return true;
    }

/**************************************************************************
    Initialise Art Matrix Cubic for Derivative Slope
**************************************************************************/

void CSlope::InitialiseArtMatrixSlope()
    {
    switch ((int)param[7])
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

    SpecialColour.rgbtRed = (BYTE)param[8];
    SpecialColour.rgbtGreen = (BYTE)param[9];
    SpecialColour.rgbtBlue = (BYTE)param[10];

    SpecialFlag = false;
    }

/**************************************************************************
    Check and plot Art Matrix special pixel for Derivative Slope
**************************************************************************/

bool CSlope::PlotArtMatrixSpecialPixel(int iX, int iY, BYTE ArithType, Complex c, BigComplex cBig, QDComplex cQD, DDComplex cDD, Complex j, BYTE juliaflag, long threshold, CPlot *Plot)
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

    if (!isSpecial)
	return false;

    RGBTRIPLE colour;
    colour.rgbtRed = (int)param[8];
    colour.rgbtGreen = (int)param[9];
    colour.rgbtBlue = (int)param[10];

    Plot->OutRGBpoint(iX, iY, colour);

    return true;
    }

