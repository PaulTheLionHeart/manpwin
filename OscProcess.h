#include "Manp.h"
#include "Dib.h"
#include "Plot.h"
#include "Matrix.h"

#pragma once

// Macro to determine to round off the given value to the closest byte
#define WIDTHBYTES(i)   ((i+31)/32*4)

//////////////////////////////////////////////////////////////////////
// Class definition
//////////////////////////////////////////////////////////////////////

class COscProcess
    {
    public:
	COscProcess();
	virtual		~COscProcess();
//	ProcessType	OscAnimProc;
	double		cMax[MAXDIM], cMin[MAXDIM], cTot[MAXDIM];
	double		Range[MAXDIM], Centre[MAXDIM];
	double		xscale, yscale, ZoomValue;
	double		mandel_width, ScreenRatio;
	double		xSum, ySum, zSum;
	int		axes;
	int		FindCentre;
	int		xSign, ySign;				// sign of x and y axes - allow reflection of image across axis
	WORD		type;					// fractal type
	double		FrameCounter = 0.0;			// count which animation frame we are doing for evolution animation
	int		subtype;
	double		VertBias = 1.0;				// allow vertical stretching of the image
	double		zBias = 1.0;				// allow stretching of the image in the z direction
	double		iterations;
	int		*xAxis, *yAxis, *zAxis;			// numerical values for axes
	int		CoordSystem;				// transform between Cartesian, Spherical and Cylindrical co-ordinate systems
	struct		OscillatorSpecificStuff	*DatabasePtr;	// point to correct database
	double		*zValuesKnots = NULL;			// array of z values for each pixel
	BOOL		*RemoveHiddenPixels;			// used in Knots, Surfaces and Curves
	BOOL		DisplayLines;
	BOOL		DisplayAxisImages;			// show all possible images for axis pairs
	double		*wpixels;				// floating point iterations for each pixels
	CTrueCol	*TrueCol;				// palette info

	void		InitOscProc(int dimensions, double mandel_width, double ScreenRatio, int InxDots, int InyDots, WORD InType, int InSubType, ProcessType OscAnimProc, int *xAxis, int *yAxis, int *zAxis,	int FindCentre, 
			CDib *Dib, double c1[], BYTE PerspectiveFlag, double iterations, RGBTRIPLE OscBackGround, double *x_rot, double *y_rot, double *z_rot, BOOL *InRemoveHiddenPixels, HWND hwndIn, double *wpixelsIn, CTrueCol *TrueColIn);
	int		COscProcess::DisplayOscillator(double c1[], double cn[], double dt, DWORD colour, double i, int dimensions, BOOL DisplayLines, int xSign, int ySign, double hor, double vert,
				double InVertBias, double InzBias, ProcessType OscAnimProc, int InCoordSystem, long threshold, double ColourFactor, int KnotWidth);
	void		ChangeCoordSystem(double *x1, double *y1, double *z1, double x, double y, double z, int CoordSystem);
	struct		OscillatorSpecificStuff	*LoadDatabasePointer(WORD InType, int InSubType);
	void		FindMinMax(double c1[]);
	void		PlotCircle(int CentrePixels);
	void		PlotAxes(void);
	void		FractalMapColour(double x, double y, double z, int u, int v, bool is3d, double ColourFactor, long threshold, double i);
	void		CloseZvalues(void);
	BOOL		CheckForHiddenPixels(double z, int u, int v);
	void		PlotHiddenKnot(int centrex, int centrey, DWORD colour, int radius, double z, CPlot Plot);
	void		PlotKnot(WORD u, WORD v, DWORD colour, int size, double z, CPlot Plot);
	int		ChooseOsc(int x, int y);

// stuff for morphing
	int		PlotWidth = 5;				// width of plot for curve or knot
	int		frames = 100;

	double		*PointInfo = NULL;			// hold every point for every axis of the fractal
	int		In1[1024], In2[1024], Out1[1024], Out2[1024], FramesPerPair;
	double		vOffset[MAXDIM], hOffset[MAXDIM], Offset[MAXDIM];
	double		vMax[MAXDIM], vMin[MAXDIM];
	ProcessType	OscAnimProc;
	AxisDisplayType	AxisDisplay = OFF;

	double		MorphDelay = 1.0;
	RGBTRIPLE	OscBackGround;
	CDib		*Dib;
	CDib		DisplayDib;
	HWND		hwnd;

	int		InitMorphing(int FractalAxes, long FractalIterations, int *NumFrames, WORD InType, ProcessType InOscAnimProc, int InFrames, CDib *Dib/*, BOOL DisplayAxisImagesIn*/);
	void		LoadRawMorphData(int /*axes*/fred, long iterations, long CurrentPass, double cTot[], int InPlotWidth, int *xAxis, int *yAxis, int *zAxis);
	int		SetupMorph(int axes, long iterations);
	int		MorphStep(HWND hwnd, char *FileName, char *MoreInfo, int TotalFrames, int ThisStep, double *DelayMultiplier, long threshold);
	void		CloseMorphing(void);
	void		OutputAxesLabel(HWND hwnd, HDC	hdc, int x, int y);
//	int		AssignZ(int xAxis, int yAxis);

    private:
	CPlot		Plot;
	CMatrix		Mat;
    };
