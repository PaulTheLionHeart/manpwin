/*
    Slope.h - interface for the CSlope class.

    Written in Microsoft Visual C++ by Paul de Leeuw.
*/

#include <Windows.h>
#include "colour.h"
#include "Dib.h"
#include "complex.h"
#include "BigComplex.h"
#include "DDComplex.h"
#include "QDComplex.h"
#include "Fractalp.h"
#include "filter.h"
#include "plot.h"
#include "BigTrig.h"
#include "Arithmetic.h"
#include "PlotMode.h"

#define	    MAX_BUMP_MAPPING_DEPTH		100
#define	    DEFAULT_BUMP_MAPPING_STRENGTH	50
#define	    MAXDERPRECISION			150				// after this derivative slope calcs runs out of precision 
#define	    PI					3.141592653589793238462643383279

#define	    NOSLOPE				0
#define	    FWDDIFFSLOPE			1
#define	    DERIVSLOPE				2

#pragma once

// There are so many arguments to pass into the template, so let's make a struct to hold it all
struct FwdDiffContext
    {
    // --- environment ---
    HWND hwnd;
    int NumThreads;
    char* StatusBarInfo;

    // --- fractal config ---
    int subtype;
    char variety;
    double* param;
    RGBTRIPLE SpecialColour;
    int PaletteShift;
    WORD *degree;
    // --- rendering ---
    CDib* Dib;
    std::vector<float>* wpixels;
    CPlot* Plot;

    // --- geometry ---
    int xdots;
    int ydots;
    int width;

    // --- threading ---
    std::vector<std::pair<int, int>>* pixelOrder;
    std::atomic<int>* workIndex;
    int totalPixels;
    };

//extern	inline void UpdateProgress(std::atomic<int>* workIndex, int totalPixels, char* StatusBarInfo, int NumThreads);

typedef	struct MySlopeData
    {
    CTrueCol	*TrueCol;			// colouring scheme
    int		NumThreads;
    int		i;
    // stuff for new plotting modes
    std::vector<std::pair<int, int>>* pixelOrder;
    std::atomic<int>* workIndex;
    int		totalPixels;
    HANDLE	ghMutex;
    } SlopeThreadData, *pSlopeThreadData;

class CSlope
    {
    public:
	CSlope();                     // default binds to global wpixels
	CSlope(std::vector<float>&);  // explicit

	int	RunSlopeDerivative(HWND GlobalHwnd, int user_data(HWND hwnd), char* StatusBarInfo, int subtype, int NumThreads, int thread, Complex j, double mandel_width, double hor, double vert, 
								BYTE BigNumFlag, BigDouble BigHor, BigDouble BigVert, BigDouble BigWidth, double rqlim, long threshold, double paramIn[], CTrueCol *TrueCol, CDib *Dib, 
								BYTE juliaflag, int xdots, int ydots, int width, int height, WORD *degreeIn, int precision, double ColourSpeedIn, HANDLE ghMutex, std::vector<std::pair<int, int>> *pixelOrder,
								std::atomic<int> *workIndex, int totalPixels);
	int	RunSlopeFwdDiff(HWND hwndIn, int user_data(HWND hwnd), char* StatusBarInfo, int subtypeIn, int NumThreadsIn, int threadIn, Complex j, double mandel_width, double hor, double vert, 
								BYTE BigNumFlag, BigDouble BigHor, BigDouble BigVert, BigDouble BigWidth, double rqlim, long threshold, double paramIn[], CTrueCol *TrueCol, CDib *Dib, 
								std::vector <float> *wpixels, BYTE juliaflag, int xdots, int ydots, int width, WORD *degreeIn, int precision, double ColourSpeedIn, std::vector<std::pair<int, int>> *pixelOrder,
								std::atomic<int> *workIndex, int totalPixels);

	RGBTRIPLE GetSmoothedColour(double fIter, double color_speed, CTrueCol &TrueCol, CPlot *Plot);

	void	InitRender(long threshold, CTrueCol *TrueCol, CDib *Dib, /*std::vector <float> *wpixels, */int PaletteShift, double bump_transfer_factor, int PaletteStart, double lightDirectionDegrees, 
		double bumpMappingDepth, double bumpMappingStrength, RGBTRIPLE SpecialColourIn);
	int	RenderSlope(int xdots, int ydots, int PertColourMethod, int PalOffset, double IterDiv, double ColourSpeedIn);

	void	SlopeIsExiting(void);
	bool	EndSlope;
	double	param[NUMSLOPEDERIVPARAM];
	int	PaletteStart;
	int	PaletteShift = 0;
	// Art Matrix Cubic
	RGBTRIPLE	SpecialColour;

    private:

	int	ConvertBignumsDD(BigDouble Big_xgap, BigDouble Big_ygap, BigDouble BigHor, BigDouble BigVert, BigDouble BigWidth, dd_real *DDxgap, dd_real *DDygap, dd_real *DDhor, dd_real *DDvert, dd_real *DDWidth);
	int	ConvertBignumsQD(BigDouble Big_xgap, BigDouble Big_ygap, BigDouble BigHor, BigDouble BigVert, BigDouble BigWidth, qd_real *QDxgap, qd_real *QDygap, qd_real *QDhor, qd_real *QDvert, qd_real *QDWidth);
	RGBTRIPLE compute_colour(CTrueCol *TrueCol, Complex j, BYTE juliaflag, Complex c, BigComplex cBig, QDComplex cQD, DDComplex cDD, double rqlim, long threshold, BYTE BigNumFlag, Complex v, bool *Time2Exit, 
		int &iterations, double &reflectionOut, double &smoothIterations);
	void	Create2DVector(Complex *v, double LightAngle);
	double	getGradientX(std::vector <float> *wpixels, int index, int width);
	double	getGradientY(std::vector <float> *wpixels, int index, int width, int height);
	int	changeBrightnessOfColorScaling(int rgb, double delta, double bump_transfer_factor);

	HWND	hwnd;
	int	NumThreads;
	int	thread;
	int	bits_per_pixel;
	int	precision;
	WORD	*degree;
	int	ArithType = DOUBLEFLOAT;

	// Fwd Diff Render Stuff
	long	threshold;
	CTrueCol *TrueCol;
	CDib	*Dib;
	CBigTrig	BigTrig;		// alternative thread-friendly trig functions
	double	bump_transfer_factor, lightDirectionDegrees, bumpMappingDepth, bumpMappingStrength, LightHeight;
	double	xgap, ygap;
	BigDouble   Big_xgap, Big_ygap;

	// screen ( integer) coordinate 
	int iX, iY;
	const int iXmax = 1000;
	const int iYmax = 1001; // for main antenna
	// world ( double) coordinate = parameter plane
	double Cx, Cy;
	const double CxMin = -2.2;
	const double CxMax = 0.8;
	const double CyMin = -1.5;
	const double CyMax = 1.5;
	/* */
	double PixelWidth   =(CxMax-CxMin)/iXmax;
	double PixelHeight  =(CyMax-CyMin)/iYmax;
	// color component ( R or G or B) is coded from 0 to 255 
	// it is 24 bit color RGB file 
	const int MaxColorComponentValue = 255;
//	static RGBTRIPLE color[3];	// 24-bit rgb color
	int	StripStart;
	int	StripMult;		// e.g. 4 or 8
	int	subtype;
	int	SlopeDegree = 2;	// used to tell FwdDiff algorithm the degree of the polymomial for slope calculations
	CPlot	Plot;
	int	smoothing = 0;
	double	ColourSpeed = 0.0;

	// Art Matrix Cubic
	Complex	t2, t3, a, b, v, a2, aa3;
	BigComplex	zNewtonBig;
	DDComplex	zNewtonDD;
	QDComplex	zNewtonQD;
	char	variety;
	bool	SpecialFlag;
    };

  