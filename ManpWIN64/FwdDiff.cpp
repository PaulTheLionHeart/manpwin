/*
    FWDDIFF.CPP - a module for determining the slope using forward differencing calculations of fractals. 
    
    Written in Microsoft Visual 'C++' by Paul de Leeuw.

    This program is written in "standard" API and not in MFC for portability
     https://github.com/hrkalona/Fractal-Zoomer/blob/master/src/fractalzoomer/core/ThreadDraw.java#L3640
     https://github.com/hrkalona/Fractal-Zoomer/blob/master/src/fractalzoomer/main/app_settings/BumpMapSettings.java
*/

#include <atomic>
#include <cassert>
#include "slope.h"
#include "FwdDiffSlopeTemplate.h"

extern	std::atomic<bool> gStopRequested;	// force early exit
extern	std::atomic<long> gPixelsDone;

/**************************************************************************
    Smooth the transitions between adjascent pallete colours
**************************************************************************/

RGBTRIPLE CSlope::GetSmoothedColour(double fIter, double ColourSpeed, CTrueCol &TrueCol, CPlot *Plot)
    {
    double  color_bias = 0.0;
    // ---------------------------------------
    // Move through palette smoothly
    // Apply palette speed (default 1.0)
    // ---------------------------------------
    double v = fIter * (ColourSpeed)+color_bias;
//    RGBTRIPLE* rgbPal = reinterpret_cast<RGBTRIPLE*>(TrueCol.PalettePtr);
    int paletteSize = TrueCol.ColoursInPALFile;

    // Wrap t into [0, paletteSize)
    double pos = fmod(v, TrueCol.ColoursInPALFile);
    if (pos < 0) pos += TrueCol.ColoursInPALFile;

    int i0 = (int)pos;
    int i1 = (i0 + 1) % TrueCol.ColoursInPALFile;

    // Fraction between the two colours
    double t = v - floor(v);

    double frac = pos - i0;
    RGBTRIPLE c0;
    RGBTRIPLE c1;
    Plot->GetRGB(i0, &c0);
    Plot->GetRGB(i1, &c1);

    RGBTRIPLE out;

    out.rgbtRed = BYTE(c0.rgbtRed   * (1.0 - t) + c1.rgbtRed   * t);
    out.rgbtGreen = BYTE(c0.rgbtGreen * (1.0 - t) + c1.rgbtGreen * t);
    out.rgbtBlue = BYTE(c0.rgbtBlue  * (1.0 - t) + c1.rgbtBlue  * t);
    return out;
    }

/**************************************************************************
    Get the gradients in the x and y directions
**************************************************************************/

double	CSlope::getGradientX(std::vector <float> *wpixels, int index, int width)
    {
    int x = index % width;
    if (index > wpixels->size())
	return 0.0;

    double it = (*wpixels)[index];

    if (x == 0) {
	return ((*wpixels)[index + 1] - it) * 2;
	}
    else if (x == width - 1) {
	return (it - (*wpixels)[index - 1]) * 2;
	}
    else {
	double diffL = it - (*wpixels)[index - 1];
	double diffR = it - (*wpixels)[index + 1];
	return diffL * diffR >= 0 ? 0 : diffL - diffR;
	}
    }

double	CSlope::getGradientY(std::vector <float> *wpixels, int index, int width, int height)
    {
    int y = index / width;
    if (index > wpixels->size())
	return 0.0;

    double it = (*wpixels)[index];

    if (y == 0) {
	return (it - (*wpixels)[index + width]) * 2;
	}
    else if (y == height - 1) {
	return ((*wpixels)[index - width] - it) * 2;
	}
    else {
	double diffU = it - (*wpixels)[index - width];
	double diffD = it - (*wpixels)[index + width];
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
    Slope Fractal
**************************************************************************/

int	CSlope::RunSlopeFwdDiff(HWND hwndIn, int user_data(HWND hwnd), char* StatusBarInfo, int subtypeIn, int NumThreadsIn, int threadIn, Complex j, double mandel_width, double hor, double vert, 
	BYTE BigNumFlag, BigDouble BigHor, BigDouble BigVert, BigDouble BigWidth, double rqlim, long threshold, double paramIn[], CTrueCol *TrueCol, CDib *Dib, std::vector<float> *wpixels, BYTE juliaflag, int xdots,
	int ydots, int width, WORD *degreeIn, int precision, double ColourSpeedIn, std::vector<std::pair<int, int>> *pixelOrder, std::atomic<int> *workIndex, int totalPixels)
    {
    int	i;
    FwdDiffContext ctx;

    thread = threadIn;
    NumThreads = NumThreadsIn;
    subtype = subtypeIn;
    hwnd = hwndIn;
    degree = degreeIn;
    ColourSpeed = ColourSpeedIn;
    smoothing = (ColourSpeed != 0.0);

    gStopRequested = false; // NEW: reset flag before starting threads

    for (i = 0; i < NUMSLOPEDERIVPARAM; i++)
	param[i] = paramIn[i];

    Plot.InitPlot(threshold, TrueCol, &gManp->wpixels, xdots, ydots, xdots, ydots, Dib->BitsPerPixel, Dib, USEPALETTE);
    if (subtype == 15)					// init Art Matrix Newton
	{
	switch ((int)param[5])
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

	SpecialColour.rgbtRed = (BYTE)param[6];
	SpecialColour.rgbtGreen = (BYTE)param[7];
	SpecialColour.rgbtBlue = (BYTE)param[8];
	}

    // load ctx structure to pass into template functions
    ctx.hwnd = hwnd;
    ctx.NumThreads = NumThreads;
    ctx.StatusBarInfo = StatusBarInfo;

    ctx.subtype = subtype;
    ctx.variety = variety;
    ctx.param = param;
    ctx.SpecialColour = SpecialColour;
    ctx.PaletteShift = PaletteShift;
    ctx.degree = degree;

    ctx.Dib = Dib;
    ctx.wpixels = wpixels;
    ctx.Plot = &Plot;

    ctx.xdots = xdots;
    ctx.ydots = ydots;
    ctx.width = width;

    ctx.pixelOrder = pixelOrder;
    ctx.workIndex = workIndex;

    if (BigNumFlag)
	{
	double	ScreenRatio = (double)xdots / (double)ydots;
	BigDouble temp_x = ScreenRatio / (double)(xdots - 1);
	BigDouble temp_y = 1.0 / (double)(ydots - 1);
	Big_xgap = BigWidth * temp_x;
	Big_ygap = BigWidth * temp_y;

	if (precision <= DDPRECISION)
	    {
	    dd_real	DDhor, DDvert, DDxgap, DDygap, DDWidth;

	    ArithType = DOUBLEDOUBLE;
	    ConvertBignumsDD(Big_xgap, Big_ygap, BigHor, BigVert, BigWidth, &DDxgap, &DDygap, &DDhor, &DDvert, &DDWidth);
	    return RunSlopeFwdDiffT<DDComplex, dd_real>(user_data, j, DDWidth, DDhor, DDvert, DDxgap, DDygap, rqlim, threshold, juliaflag, ctx);
	    }
	else if (precision <= QDPRECISION)
	    {
	    qd_real	QDhor, QDvert, QDxgap, QDygap, QDWidth;

	    ArithType = QUADDOUBLE;
	    ConvertBignumsQD(Big_xgap, Big_ygap, BigHor, BigVert, BigWidth, &QDxgap, &QDygap, &QDhor, &QDvert, &QDWidth);
	    return RunSlopeFwdDiffT<QDComplex, qd_real>(user_data, j, QDWidth, QDhor, QDvert, QDxgap, QDygap, rqlim, threshold, juliaflag, ctx);
	    }
	else
	    {
	    ArithType = ARBITRARYPREC;
	    return RunSlopeFwdDiffT<BigComplex, BigDouble>(user_data, j, BigWidth, BigHor, BigVert, Big_xgap, Big_ygap, rqlim, threshold, juliaflag, ctx);
	    }
	}
    else
	{
	ArithType = DOUBLEFLOAT;
	return RunSlopeFwdDiffT<Complex, double>(user_data, j, mandel_width, hor, vert, xgap, ygap, rqlim, threshold, juliaflag, ctx);
	}

    return 0;
    }

/**************************************************************************
	Init Render 
**************************************************************************/

void	CSlope::InitRender(long thresholdIn, CTrueCol *TrueColIn, CDib *DibIn, /*std::vector <float> &wpixelsIn, */int PaletteShiftIn, double bump_transfer_factorIn, int PaletteStartIn, double lightDirectionDegreesIn,
		double bumpMappingDepthIn, double bumpMappingStrengthIn, RGBTRIPLE SpecialColourIn)
    {
    gStopRequested = false; // NEW: reset flag before starting threads
    threshold = thresholdIn; TrueCol = TrueColIn;  Dib = DibIn; /*wpixels = wpixelsIn; */PaletteShift = PaletteShiftIn; bump_transfer_factor = bump_transfer_factorIn;
    PaletteStart = PaletteStartIn; lightDirectionDegrees = lightDirectionDegreesIn; bumpMappingDepth = bumpMappingDepthIn, bumpMappingStrength = bumpMappingStrengthIn;
    SpecialColour = SpecialColourIn;
    }

/**************************************************************************
	Render Slope Fractal
**************************************************************************/

int	CSlope::RenderSlope(int xdots, int ydots, int PertColourMethod, int PalOffset, double IterDiv, double ColourSpeedIn)
    {
    double	dotp, gradAbs, gradCorr, cosAngle, sizeCorr, smoothGrad, lightAngleRadians, lightx, lighty;
    
    double	iterations;
    int		lastChecked = -1;
//    DWORD	index;
    int		x, y;
    double	gradx, grady;
    unsigned char r, g, b;
    RGBTRIPLE	colour;
    int		modified;

    ColourSpeed = ColourSpeedIn; 
    smoothing = (ColourSpeed != 0.0);
    lastChecked = -1;
    sizeCorr = 0.0;
    lightx = 0.0;
    lighty = 0.0;
    gStopRequested = false; // NEW: reset flag before starting threads

    Plot.InitPlot(threshold, TrueCol, &gManp->wpixels, xdots, ydots, xdots, ydots, Dib->BitsPerPixel, Dib, USEPALETTE);
    gradCorr = pow(2, (bumpMappingStrength - DEFAULT_BUMP_MAPPING_STRENGTH) * 0.05);
    sizeCorr = ydots / pow(2, (MAX_BUMP_MAPPING_DEPTH - bumpMappingDepth) * 0.16);
    lightAngleRadians = lightDirectionDegrees * PI / 180.0;
    lightx = cos(lightAngleRadians) * gradCorr;
    lighty = sin(lightAngleRadians) * gradCorr;

    for (y = 0; y < ydots; y++)
	{
	for (x = 0; x < xdots; x++)
	    {
	    if (AbortRequested())
		return -1;
	    const DWORD index = (DWORD)y * (DWORD)xdots + (DWORD)x;
	    const size_t needed = (size_t)xdots * (size_t)ydots;
	    if (gManp->wpixels.size() < needed)
		return 0; // or error: no valid forward differencing buffer
	    iterations = gManp->wpixels[index];
	    if (iterations == INSIDEPIXEL)
		continue;							// do nothing in case we splatter inside filter pixels
	    if (iterations == SPECIALPIXEL)
		{
		Plot.OutRGBpoint(iX, iY, SpecialColour);
		continue;							// set pixel to special colour
		}

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
		else if (smoothing)		// use colour smoothing
		    {
		    // Use smoothed color from existing BLA smoothing code
		    RGBTRIPLE base = GetSmoothedColour(iterations, ColourSpeed, *TrueCol, &Plot);

		    // Convert to packed DWORD for later bump-mapping brightness ops
		    modified = 0xFF000000 | (base.rgbtRed << 16) | (base.rgbtGreen << 8) | base.rgbtBlue;
		    }
		else
		    {
		    if (abs(PaletteShift) > 1)
			modified = 0xFF000000 | ((DWORD)TrueCol->PalettePtr[(((long)(iterations * abs(PaletteShift))) % TrueCol->ColoursInPALFile)].rgbtBlue << 16)
					      | ((DWORD)TrueCol->PalettePtr[(((long)(iterations * abs(PaletteShift))) % TrueCol->ColoursInPALFile)].rgbtGreen << 8)
					      | TrueCol->PalettePtr[(((long)(iterations * abs(PaletteShift))) % TrueCol->ColoursInPALFile)].rgbtRed;
		    else
			modified = 0xFF000000 | ((DWORD)TrueCol->PalettePtr[(((long)iterations) % TrueCol->ColoursInPALFile)].rgbtBlue << 16)
					      | ((DWORD)TrueCol->PalettePtr[(((long)iterations) % TrueCol->ColoursInPALFile)].rgbtGreen << 8)
					      | TrueCol->PalettePtr[(((long)iterations) % TrueCol->ColoursInPALFile)].rgbtRed;
		    }
		gradx = getGradientX(&gManp->wpixels, index, xdots);
		grady = getGradientY(&gManp->wpixels, index, xdots, ydots);
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
	    if (AbortRequested())
		return -1;
	    Plot.OutRGBpoint(x, y, colour);
	    }
	}
    return 0;
    }

