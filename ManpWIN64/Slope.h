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
    std::vector<BYTE>* PixelFlags;     // shared per-pixel classification
    CPlot* Plot;

    // --- filters ---
    int InsideMethod;
    int OutsideMethod;
    int biomorph;
    double *potparam;
    CTrueCol *TrueCol;

    // --- geometry ---
    int xdots;
    int ydots;
    int width;
    PlotMode mode;

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
    PlotMode	mode;
    int		InsideMethod;
    int		OutsideMethod;
    int		biomorph;
    double	*potparam;
    HANDLE	ghMutex;
    } SlopeThreadData, *pSlopeThreadData;

class CSlope
    {
    public:
	CSlope();                     // default binds to global wpixels
	CSlope(std::vector<float>&);  // bind to supplied wpixels buffer

	int	RunSlopeDerivative(HWND GlobalHwnd, int user_data(HWND hwnd), char* StatusBarInfo, int subtype, int NumThreads, int thread, Complex j, double mandel_width, double hor, double vert, 
								BYTE BigNumFlag, BigDouble BigHor, BigDouble BigVert, BigDouble BigWidth, double rqlim, long threshold, double paramIn[], CTrueCol *TrueCol, CDib *Dib, 
								BYTE juliaflag, int xdots, int ydots, int width, int height, WORD *degreeIn, int precision, double ColourSpeedIn, HANDLE ghMutex, std::vector<std::pair<int, int>> *pixelOrder,
								std::atomic<int> *workIndex, int totalPixels, PlotMode mode);
	int	RunSlopeFwdDiff(HWND hwndIn, int user_data(HWND hwnd), char* StatusBarInfo, int subtypeIn, int NumThreadsIn, int threadIn, Complex j, double mandel_width, double hor, double vert, 
								BYTE BigNumFlag, BigDouble BigHor, BigDouble BigVert, BigDouble BigWidth, double rqlim, long threshold, double paramIn[], CTrueCol *TrueCol, CDib *Dib, 
								std::vector <float> *wpixels, BYTE juliaflag, int xdots, int ydots, int width, WORD *degreeIn, int precision, double ColourSpeedIn, std::vector<std::pair<int, int>> *pixelOrder,
								std::atomic<int> *workIndex, int totalPixels, PlotMode mode, int InsideMethodIn, int OutsideMethodIn, int biomorphIn, double *potparamIn);

	RGBTRIPLE GetSmoothedColour(double fIter, double color_speed, CTrueCol &TrueCol, CPlot *Plot);

	void	InitRender(long threshold, CTrueCol *TrueCol, CDib *Dib, int PaletteShift, double bump_transfer_factor, int PaletteStart, double lightDirectionDegrees, 
		double bumpMappingDepth, double bumpMappingStrength, RGBTRIPLE SpecialColourIn);
	int	RenderSlope(int xdots, int ydots, int PertColourMethod, int PalOffset, double IterDiv, double ColourSpeedIn);

	void	SlopeIsExiting(void);
	bool	EndSlope;
	double	param[NUMSLOPEPARAM];
	int	PaletteStart;
	int	PaletteShift = 0;		// palette movement between animation frames
	// Art Matrix Cubic
	RGBTRIPLE	SpecialColour;

    private:

	int	ConvertBignumsDD(BigDouble Big_xgap, BigDouble Big_ygap, BigDouble BigHor, BigDouble BigVert, BigDouble BigWidth, dd_real *DDxgap, dd_real *DDygap, dd_real *DDhor, dd_real *DDvert, dd_real *DDWidth);
	int	ConvertBignumsQD(BigDouble Big_xgap, BigDouble Big_ygap, BigDouble BigHor, BigDouble BigVert, BigDouble BigWidth, qd_real *QDxgap, qd_real *QDygap, qd_real *QDhor, qd_real *QDvert, qd_real *QDWidth);
	RGBTRIPLE compute_colour(CTrueCol *TrueCol, CPlot *Plot, Complex j, BYTE juliaflag, Complex c, BigComplex cBig, QDComplex cQD, DDComplex cDD, double rqlim, long threshold, BYTE BigNumFlag, Complex v, bool *Time2Exit,
		int &iterations, double &smoothIterations);
	void	Create2DVector(Complex *v, double LightAngle);
	double	getGradientX(std::vector <float> *wpixels, int index, int width);
	double	getGradientY(std::vector <float> *wpixels, int index, int width, int height);
	int	changeBrightnessOfColorScaling(int rgb, double delta, double bump_transfer_factor);
	bool	SetSlopeCoordinate(int idx, int totalPixels, int xdots, int ydots, PlotMode mode, std::vector<std::pair<int, int>> *pixelOrder, int &iX, int &iY, BYTE ArithType, Complex &c, BigComplex &cBig, 
		QDComplex &cQD, DDComplex &cDD, double hor, double vert, double mandel_width, double xgap, double ygap, BigDouble BigHor, BigDouble BigVert, BigDouble BigWidth, BigDouble Big_xgap, BigDouble Big_ygap,
		dd_real DDhor, dd_real DDvert, dd_real DDWidth, dd_real DDxgap, dd_real DDygap, qd_real QDhor, qd_real QDvert, qd_real QDWidth, qd_real QDxgap, qd_real QDygap);
	void	InitialiseArtMatrixSlope();
	bool	PlotArtMatrixSpecialPixel(int iX, int iY, BYTE ArithType, Complex c, BigComplex cBig, QDComplex cQD, DDComplex cDD, Complex j, BYTE juliaflag, long threshold, CPlot *Plot);
	int	InitialiseSlopeDerivativeWorker(HWND hwndIn, int subtypeIn, int NumThreadsIn, int threadIn, BYTE BigNumFlag, BigDouble BigHor, BigDouble BigVert, BigDouble BigWidth, double mandel_width, 
		double paramIn[], CTrueCol *TrueCol, CDib *Dib, int xdots, int ydots, int height, WORD *degreeIn, int precisionIn, double ColourSpeedIn, std::atomic<int> *workIndex, int totalPixels, 
		std::vector<std::pair<int, int>> *pixelOrder, Complex &v, double &xgap, double &ygap, dd_real &DDhor, dd_real &DDvert, dd_real &DDxgap, dd_real &DDygap, dd_real &DDWidth, qd_real &QDhor, qd_real 
		&QDvert, qd_real &QDxgap, qd_real &QDygap, qd_real &QDWidth, int InsideMethodIn, int OutsideMethodIn, int biomorphIn, double *potparamIn, CPlot *Plot);
	long	ApplySlopeFilters(int iterations, BYTE ArithType, Complex &FinalZ, DDComplex &FinalZDD, QDComplex &FinalZQD, BigComplex &FinalZBig, long threshold, double rqlim, double min_orbit, long min_index, CTrueCol *TrueCol);

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
	const int iXmax = 1000;
	const int iYmax = 1001; // for main antenna
	// world ( double) coordinate = parameter plane
	double Cx, Cy;
	const double CxMin = -2.2;
	const double CxMax = 0.8;
	const double CyMin = -1.5;
	const double CyMax = 1.5;
	
	double PixelWidth   =(CxMax-CxMin)/iXmax;
	double PixelHeight  =(CyMax-CyMin)/iYmax;
	// color component ( R or G or B) is coded from 0 to 255 
	// it is 24 bit color RGB file 
	const int MaxColorComponentValue = 255;
	int	StripStart;
	int	StripMult;		// e.g. 4 or 8
	int	subtype;
	int	SlopeDegree = 2;	// used to tell FwdDiff algorithm the degree of the polymomial for slope calculations
	CPlot	Plot;
	int	smoothing = 0;
	double	ColourSpeed = 0.0;
	int	InsideMethod;
	int	OutsideMethod;
	int	biomorph;
	double	*potparam;

	// Art Matrix Cubic
	Complex	t2, t3, a, b, v, a2, aa3;
	BigComplex	zNewtonBig;
	DDComplex	zNewtonDD;
	QDComplex	zNewtonQD;
	char	variety;
	bool	SpecialFlag;
    };

  