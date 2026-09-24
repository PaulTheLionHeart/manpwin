/*
    Clouds.cpp - interface for the Clouds Flarium filter.
    
    Purpose: implement init, cleanup, point accumulation, and map merging

    Written in Microsoft Visual C++ by Paul de Leeuw.
*/

#include "Clouds.h"
#include "Manp.h"

/**************************************************************************
	Initialise Clouds density map
**************************************************************************/

void CClouds::Init(int xdots, int ydots, double hor, double vert,
		   double mandel_width, double AspectRatio)
    {
    Width = xdots;
    Height = ydots;

    Hor = hor;
    Vert = vert;
    MandelWidth = mandel_width;

    const size_t pixelCount = (size_t)Width * (size_t)Height;
    Density.assign(pixelCount, 0);

    XScale = (double)(Width - 1) /
	     (MandelWidth * AspectRatio);

    YScale = (double)(Height - 1) /
	     MandelWidth;
    }

/**************************************************************************
	Release Clouds density map
**************************************************************************/

void CClouds::Close()
    {
    Density.clear();

    Width = 0;
    Height = 0;

    Hor = 0.0;
    Vert = 0.0;
    MandelWidth = 0.0;
    XScale = 0.0;
    YScale = 0.0;
    }

/**************************************************************************
	Add one orbit point to Clouds density map
**************************************************************************/

void CClouds::AddPoint(double x, double y)
    {
    if (Width <= 0 || Height <= 0 || Density.empty())
	return;

    int col = (int)((x - Hor) * XScale);
    int row = (int)(((Vert + MandelWidth) - y) * YScale);

    if (col >= 0 && col < Width &&
	row >= 0 && row < Height)
	{
	Density[(size_t)row * (size_t)Width + (size_t)col]++;
	}
    }

/**************************************************************************
	Add this thread-local Clouds density map to the final density map
**************************************************************************/

void CClouds::AddDensityTo(std::vector<unsigned int>& Destination) const
    {
    if (Destination.size() != Density.size())
	return;

    for (size_t i = 0; i < Density.size(); i++)
	Destination[i] += Density[i];
    }

/**************************************************************************
    Render the accumulated Clouds density map.
    Returns 0 on success, -1 if the user aborts during post-processing.
**************************************************************************/

int	CClouds::RenderCloudDensity(const std::vector<unsigned int>& Density, unsigned int MaxDensity, int RenderMode, int threshold, CTrueCol *TrueCol, std::vector<float> *wpixels, 
	int xdots, int ydots, int BitsPerPixel, CDib *Dib, int(*UserData)(HWND), HWND hwnd)
    {
    CPlot Plot;

    Plot.InitPlot(threshold, TrueCol, wpixels, xdots, ydots, xdots, ydots, BitsPerPixel, Dib, USEPALETTE);

    if (MaxDensity > 0)
	{
	const double LogDenom = log((double)MaxDensity + 1.0);

	for (int y = 0; y < ydots; y++)
	    {
	    // Update status and allow abort every few rows.
	    if ((y & 15) == 0)
		{
		gManp->curpass = y + 1;
		gManp->totpasses = ydots;

		if (!gManp->RunAnimation)
		    gManp->DisplayStatusBarInfo(INCOMPLETE, "Rendering Clouds");

		if (UserData != NULL && UserData(hwnd) < 0)
		    {
		    gStopRequested.store(true, std::memory_order_relaxed);
		    return -1;
		    }

		if (gStopRequested.load(std::memory_order_relaxed))
		    return -1;
		}

	    for (int x = 0; x < xdots; x++)
		{
		const size_t index = (size_t)y * (size_t)xdots + (size_t)x;
		const unsigned int density = Density[index];

		switch (RenderMode)
		    {
		    case CLOUD_RENDER_LINEAR_GREY:
			{
			BYTE grey = (BYTE)(((unsigned long long)density * 255ULL) / (unsigned long long)MaxDensity);

			RGBTRIPLE colour;
			colour.rgbtRed = grey;
			colour.rgbtGreen = grey;
			colour.rgbtBlue = grey;

			Plot.OutRGBpoint((WORD)x, (WORD)y, colour);
			}
			break;

		    case CLOUD_RENDER_LOG_GREY:
			{
			BYTE grey = 0;

			if (density > 0 && LogDenom > 0.0)
			    {
			    double value = log(1.0 + (double)density) / LogDenom;
			    grey = (BYTE)(255.0 * value);
			    }

			RGBTRIPLE colour;
			colour.rgbtRed = grey;
			colour.rgbtGreen = grey;
			colour.rgbtBlue = grey;

			Plot.OutRGBpoint((WORD)x, (WORD)y, colour);
			}
			break;

		    case CLOUD_RENDER_LINEAR_PALETTE:
			{
			if (density == 0)
			    {
			    RGBTRIPLE colour = { 0, 0, 0 };
			    Plot.OutRGBpoint((WORD)x, (WORD)y, colour);
			    }
			else
			    {
			    long colour = (long)(((unsigned long long)density *
				(TrueCol->ColoursInPALFile - 1)) /
				(unsigned long long)MaxDensity);

			    Plot.PlotPoint((WORD)x, (WORD)y, colour);
			    }
			}
			break;

		    case CLOUD_RENDER_LOG_PALETTE:
			{
			if (density == 0)
			    {
			    RGBTRIPLE colour = { 0, 0, 0 };
			    Plot.OutRGBpoint((WORD)x, (WORD)y, colour);
			    }
			else
			    {
			    double value = log(1.0 + (double)density) / LogDenom;

			    long colour = (long)(value *
				(double)(TrueCol->ColoursInPALFile - 1));

			    Plot.PlotPoint((WORD)x, (WORD)y, colour);
			    }
			}
			break;
		    }
		}
	    }
	}
    return 0;
    }

