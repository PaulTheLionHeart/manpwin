/*
    PERTFILTERS.CPP - a module to explore Perturbation filter functions

    Thanks to Claude Heiland-Allen https://fractalforums.org/programming/11/perturbation-code-for-cubic-and-higher-order-polynomials/2783,
    SuperHeal https://fractalforums.org/index.php?topic=5524.new#new
    and Shirom Makkad https://github.com/ShiromMakkad/MandelbrotPerturbation
    for their inspiration and methods of calculating perturbation fractals

    Written in Microsoft Visual C++ by Paul de Leeuw.
*/

#include <atomic>
#include "PertEngine.h"
#include "Potential.h"
#include "Manp.h"
#include "FilterTemplate.h"


extern	std::atomic<bool> gStopRequested;	// force early exit

/**************************************************************************
    Smooth the transitions between adjascent pallete colours
**************************************************************************/

RGBTRIPLE CPerturbation::GetSmoothedColour(double fIter, double color_speed, CTrueCol &TrueCol, CPlot *Plot)
    {
    double  color_bias = 0.0;
    // ---------------------------------------
    // Move through palette smoothly
    // Apply palette speed (default 1.0)
    // ---------------------------------------
    double v = fIter * (color_speed)+color_bias;
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

    out.rgbtRed	    = BYTE(c0.rgbtRed   * (1.0 - t) + c1.rgbtRed   * t);
    out.rgbtGreen   = BYTE(c0.rgbtGreen * (1.0 - t) + c1.rgbtGreen * t);
    out.rgbtBlue    = BYTE(c0.rgbtBlue  * (1.0 - t) + c1.rgbtBlue  * t);
    return out;
    }

/**************************************************************************
    Double colour processing
**************************************************************************/

int	CPerturbation::ColourProcessing(Complex z, long iteration, int x, int y, CTrueCol &TrueCol, /*CTZfilter &TZfilter, */double bailout)
    {
    int		index;
    double	FloatIteration = 0.0;
    double	log_zn, nu;
    float	value = 0.0f;
    int		SlopeIndex;
    CPlot	Plot;
    long	RawIteration = iteration;

    Plot.InitPlot(MaxIteration, &TrueCol, &gManp->wpixels, xdots, height, xdots, height, Dib->BitsPerPixel, Dib, USEPALETTE);

    if (PertColourMethod != 0 && iteration < MaxIteration)		// Kalles colour method
	{
	double  iter = (double)iteration;
	if (PertColourMethod == 1)
	    iter = sqrt(iter);
	else if (PertColourMethod == 2)
	    iter = pow(iter, (double)1 / (double)3);
	else if (PertColourMethod == 3)
	    iter = log(iter);
	iteration = (int)iter;
	}
    if (IterDiv != 1.0 && iteration < MaxIteration)
	iteration = (int)((double)iteration / IterDiv);
    if (PalOffset && iteration < MaxIteration)
	iteration = (iteration + PalOffset) % TrueCol.ColoursInPALFile;
    if (gManp->wpixels.size() >= (size_t)Dib->DibWidth * Dib->DibHeight)	// wpixels is available for smoothing, 3D or Forward Difference data
	{
	if (_3dflag)
	    {
	    // 3D: use raw integer iteration count
	    value = static_cast<float>(iteration);
	    }
	else if (SlopeType == FWDDIFFSLOPE || smoothing)
	    {
	    FloatIteration = 0.0;
	    if (iteration < MaxIteration)
		{
		log_zn = log(z.x * z.x + z.y * z.y) / (double)SlopeDegree;
		nu = log(log_zn / log((double)SlopeDegree)) / log((double)SlopeDegree);
		FloatIteration = iteration + 2 - nu;
		}
	    else
		FloatIteration = MaxIteration;
	    if (FloatIteration > MaxIteration)
		FloatIteration = MaxIteration;
	    if (FloatIteration < 0)
		FloatIteration = 0;
	    value = static_cast<float>(FloatIteration);
	    }
	SlopeIndex = (((DWORD)Dib->DibHeight - 1 - y) * (DWORD)Dib->DibWidth) + (DWORD)x;
	// Forward Difference is stored after filter processing because the
	// filtered value may replace the normal fractional iteration value.
	if (SlopeType != FWDDIFFSLOPE && x < Dib->DibWidth && y < Dib->DibHeight && !gStopRequested)
	    gManp->wpixels[SlopeIndex] = value;
	}

    if (OutsideMethod == NONE && InsideMethod == NONE && biomorph < 0)
	{
	if (iteration == MaxIteration)
	    index = MaxIteration;
	else
	    index = iteration;
	}
    else
	{
	// set up some values for filters not used in perturbation
	int hooper = 0;							// EPSCROSS not being supplied by Pert here
	int paletteColours = TrueCol.ColoursInPALFile;
	int decomp = 0;
	int special = 0;
	int logval = 0;
	int potentialColours = TrueCol.ColoursInPALFile;
	BYTE *logtable = NULL;		

	if (OutsideMethod >= TIERAZONFILTERS)
	    {
	    TZfilter.EndTierazonFilter(z, (long *)&iteration, &TrueCol);
	    index = iteration;
	    }
	else
	    {
	    index = DoOutsideFilterT<Complex, double>(iteration, z, OutsideMethod, hooper, MaxIteration, paletteColours, decomp, biomorph,
		bailout, special, logval, logtable, potentialColours, &TrueCol, potparam);
	    }

	if (iteration == MaxIteration)
	    index = DoInsideFilterT(iteration, z, InsideMethod, MaxIteration, min_orbit, min_index);
	}

    if (index > MaxIteration)
	index = MaxIteration;
    if (index < 0)
	index = 0;
    if (AbortRequested())
	return -1;

    if (SlopeType == FWDDIFFSLOPE && gManp->wpixels.size() >= (size_t)Dib->DibWidth * Dib->DibHeight)
	{
	SlopeIndex = (((DWORD)Dib->DibHeight - 1 - y) * (DWORD)Dib->DibWidth) + (DWORD)x;

	if (x >= 0 && x < Dib->DibWidth && y >= 0 && y < Dib->DibHeight && !gStopRequested)
	    {
	    if (iteration == MaxIteration)
		{
		// Inside filter result is a colour index.  Preserve it and
		// prevent Forward Difference lighting on this pixel.
		gManp->wpixels[SlopeIndex] = (float)index;
		gManp->PixelFlags[SlopeIndex] |= PIXEL_INSIDE;
		}
	    else if (OutsideMethod != NONE || biomorph >= 0)
		{
		// The outside filter result becomes the Forward Difference
		// value, allowing the filter to modify the slope surface.
		gManp->wpixels[SlopeIndex] = (float)index;
		}
	    else
		{
		// Ordinary Forward Difference uses fractional iteration height.
		gManp->wpixels[SlopeIndex] = value;
		}
	    }
	}

    if (SlopeType != DERIVSLOPE || (iteration == MaxIteration && InsideMethod > 0) || EnableApproximation)
	{
	if (*PlotType == FILTERPLOT)
	    Plot.FilterPoint(x, height - 1 - y, index, &(TZfilter.FilterRGB));
	else if (index == MaxIteration)
	    {
	    RGBTRIPLE	inside;
	    inside.rgbtBlue = (BYTE)TrueCol.InsideBlue;
	    inside.rgbtGreen = (BYTE)TrueCol.InsideGreen;
	    inside.rgbtRed = (BYTE)TrueCol.InsideRed;
	    Plot.OutRGBpoint(x, height - 1 - y, inside);
	    }
	else
	    {
	    if (RawIteration < PaletteStart && OutsideMethod == NONE && InsideMethod == NONE && biomorph < 0)
		{
		RGBTRIPLE colour;

		colour.rgbtRed = (BYTE)((gManp->PrePaletteColour >> 16) & 0xff);
		colour.rgbtGreen = (BYTE)((gManp->PrePaletteColour >> 8) & 0xff);
		colour.rgbtBlue = (BYTE)(gManp->PrePaletteColour & 0xff);

		Plot.OutRGBpoint(x, height - 1 - y, colour);
		}
	    else if (smoothing && InsideMethod == NONE)
		{
		RGBTRIPLE col = GetSmoothedColour(FloatIteration, ColourSpeed, TrueCol, &Plot);
		Plot.OutRGBpoint(x, height - 1 - y, col);
		}
	    else
		Plot.PlotPoint(x, height - 1 - y, index);
	    }
	}
    return 0;
    }

/**************************************************************************
    Floatexp colour processing
**************************************************************************/

int	CPerturbation::ColourProcessingExp(ExpComplex ExpW, long iteration, int x, int y, CTrueCol &TrueCol, /*CTZfilter &TZfilter, */double bailout)
    {
    int		index;
    double	magnitude = 0.0;
    double	FloatIteration = 0.0;
    double	rqlim2 = sqrt(bailout);
    double	log_zn, nu;
    int		SlopeIndex;
    CPlot	Plot;
    Complex	z;
    long	RawIteration = iteration;

    z.x = ExpW.x.todouble();
    z.y = ExpW.y.todouble();

    Plot.InitPlot(MaxIteration, &TrueCol, &gManp->wpixels, xdots, height, xdots, height, Dib->BitsPerPixel, Dib, USEPALETTE);
    size_t requiredPixels = (size_t)Dib->DibWidth * (size_t)Dib->DibHeight;
    if (gManp->wpixels.size() >= requiredPixels)			// wpixels is available for smoothing, 3D or Forward Difference data
	{
	if (iteration < MaxIteration)
	    {
	    double	SumSqW;
	    floatexp	ExpSumSqW = ExpW.x * ExpW.x + ExpW.y * ExpW.y;
	    SumSqW = ExpSumSqW.todouble();
	    log_zn = log(SumSqW) / SlopeDegree;
	    nu = log(log_zn / log(SlopeDegree)) / log(SlopeDegree);
	    FloatIteration = iteration + 2 - nu;
	    }
	else
	    FloatIteration = MaxIteration;
	SlopeIndex = (((DWORD)Dib->DibHeight - 1 - y) * (DWORD)xdots) + (DWORD)x;
	if (AbortRequested())
	    return -1;
	}

    if (OutsideMethod == NONE && InsideMethod == NONE && biomorph < 0)
	{
	if (iteration == MaxIteration)
	    index = MaxIteration;
	else
	    index = iteration;
	}
    else
	{
	// set up some values for filters not used in perturbation
	int hooper = 0;							// EPSCROSS not being supplied by Pert here
	int paletteColours = TrueCol.ColoursInPALFile;
	int decomp = 0;
	int special = 0;
	int logval = 0;
	int potentialColours = TrueCol.ColoursInPALFile;
	BYTE *logtable = NULL;

	if (OutsideMethod >= TIERAZONFILTERS)
	    {
	    TZfilter.EndTierazonFilter(z, (long *)&iteration, &TrueCol);
	    index = iteration;
	    }
	else
	    {
	    index = DoOutsideFilterT<Complex, double>(iteration, z, OutsideMethod, hooper, MaxIteration, paletteColours, decomp, biomorph,
		bailout, special, logval, logtable, potentialColours, &TrueCol, potparam);
	    }

	if (iteration == MaxIteration)
	    index = DoInsideFilterT(iteration, z, InsideMethod, MaxIteration, min_orbit, min_index);
	}

    if (index > MaxIteration)
	index = MaxIteration;
    if (index < 0)
	index = 0;

    if (SlopeType == FWDDIFFSLOPE && gManp->wpixels.size() >= requiredPixels)
	{
	SlopeIndex = (((DWORD)Dib->DibHeight - 1 - y) * (DWORD)xdots) + (DWORD)x;

	if (x >= 0 && x < Dib->DibWidth && y >= 0 && y < Dib->DibHeight && !gStopRequested)
	    {
	    if (iteration == MaxIteration)
		{
		// Preserve the inside filter colour index and prevent
		// Forward Difference lighting on this pixel.
		gManp->wpixels[SlopeIndex] = (float)index;
		gManp->PixelFlags[SlopeIndex] |= PIXEL_INSIDE;
		}
	    else if (OutsideMethod != NONE)
		{
		// Use the outside filter result as the Forward Difference value.
		gManp->wpixels[SlopeIndex] = (float)index;
		}
	    else
		{
		// Ordinary Forward Difference uses fractional iteration height.
		gManp->wpixels[SlopeIndex] = (float)FloatIteration;
		}
	    }
	}

    if (SlopeType != DERIVSLOPE || (iteration == MaxIteration && InsideMethod > 0) || EnableApproximation)
	{
	if (AbortRequested())
	    return -1;
	if (*PlotType == FILTERPLOT)
	    Plot.FilterPoint(x/* + xStart*/, height - 1 - y, index, &(TZfilter.FilterRGB));
	else if (index == MaxIteration)
	    {
	    RGBTRIPLE	inside;
	    inside.rgbtBlue = (BYTE)TrueCol.InsideBlue;
	    inside.rgbtGreen = (BYTE)TrueCol.InsideGreen;
	    inside.rgbtRed = (BYTE)TrueCol.InsideRed;
	    Plot.OutRGBpoint(x/* + xStart*/, height - 1 - y, inside);
	    }
	else
	    {
	    if (RawIteration < PaletteStart &&
		OutsideMethod == NONE &&
		InsideMethod == NONE &&
		biomorph < 0)
		{
		RGBTRIPLE colour;

		colour.rgbtRed = (BYTE)((gManp->PrePaletteColour >> 16) & 0xff);
		colour.rgbtGreen = (BYTE)((gManp->PrePaletteColour >> 8) & 0xff);
		colour.rgbtBlue = (BYTE)(gManp->PrePaletteColour & 0xff);

		Plot.OutRGBpoint(x, height - 1 - y, colour);
		}
	    else if (smoothing && InsideMethod == NONE)
		{
		RGBTRIPLE col = GetSmoothedColour(FloatIteration, ColourSpeed, TrueCol, &Plot);
		Plot.OutRGBpoint(x, height - 1 - y, col);
		}
	    else
		Plot.PlotPoint(x, height - 1 - y, index);
	    }
	}
    return 0;
    }
