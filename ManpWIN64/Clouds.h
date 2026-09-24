/*
    Clouds.h - interface for the Clouds Flarium filter.

    Written in Microsoft Visual C++ by Paul de Leeuw.
*/

#pragma once

#include <vector>
#include "Plot.h"
#include "colour.h"
#include "Dib.h"

enum CloudRenderModeType
    {
    CLOUD_RENDER_LINEAR_GREY = 0,
    CLOUD_RENDER_LOG_GREY,
    CLOUD_RENDER_LINEAR_PALETTE,
    CLOUD_RENDER_LOG_PALETTE
    };

class CClouds
    {
    public:
	void	Init(int xdots, int ydots, double hor, double vert, double mandel_width, double AspectRatio);
	void	Close();
	void	AddPoint(double x, double y);
	void	AddDensityTo(std::vector<unsigned int>& Destination) const;
	int	RenderCloudDensity(const std::vector<unsigned int>& Density, unsigned int MaxDensity, int RenderMode, int threshold, CTrueCol *TrueCol, std::vector<float> *wpixels, 
		int xdots, int ydots, int BitsPerPixel, CDib *Dib, int(*UserData)(HWND), HWND hwnd);
	
	const std::vector<unsigned int>& GetDensity() const
	    {
	    return Density;
	    }

    private:
	std::vector<unsigned int> Density;	// orbit-hit count for each screen pixel

	int	Width = 0;
	int	Height = 0;

	double	Hor = 0.0;
	double	Vert = 0.0;
	double	MandelWidth = 0.0;
	double	XScale = 0.0;
	double	YScale = 0.0;
    };

