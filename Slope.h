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

#define	    MAX_BUMP_MAPPING_DEPTH		100
#define	    DEFAULT_BUMP_MAPPING_STRENGTH	50
#define	    MAXDERPRECISION			150				// after this derivative slope calcs runs out of precision 
#define	    WIDTHBYTES(i)			((i+31)/32*4)
#define	    PI					3.141592653589793238462643383279

#define	    NOSLOPE				0
#define	    FWDDIFFSLOPE			1
#define	    DERIVSLOPE				2

#pragma once
class CSlope
    {
    public:
	int	RunSlopeDerivative(HWND GlobalHwnd, int user_data(HWND hwnd), char* StatusBarInfo, bool *ThreadComplete, int subtype, int NumThreads, int thread, Complex j, double mandel_width, double hor, double vert, double xgap, double ygap,
								BYTE BigNumFlag, BigDouble Big_xgap, BigDouble Big_ygap, BigDouble BigHor, BigDouble BigVert, BigDouble BigWidth, double rqlim, long threshold, double paramIn[], CTrueCol *TrueCol, CDib *Dib, 
								int bits_per_pixel, BYTE juliaflag, int xdots, int ydots, int width, int height, WORD *degreeIn, int precision, HANDLE ghMutex);
	int	RunSlopeFwdDiff(HWND hwndIn, int user_data(HWND hwnd), char* StatusBarInfo, bool *ThreadComplete, int subtypeIn, int NumThreadsIn, int threadIn, Complex j, double mandel_width, double hor, double vert, double xgap, double ygap,
								BYTE BigNumFlag, BigDouble Big_xgap, BigDouble Big_ygap, BigDouble BigHor, BigDouble BigVert, BigDouble BigWidth, double rqlim, long threshold, double paramIn[], CTrueCol *TrueCol, CDib *Dib, double *wpixels, BYTE juliaflag, 
								int xdots, int ydots, int width, WORD *degreeIn);
	void	InitRender(long threshold, CTrueCol *TrueCol, CDib *Dib, double *wpixels, int PaletteShift, double bump_transfer_factor, int PaletteStart, double lightDirectionDegrees, double bumpMappingDepth, double bumpMappingStrength);
	int	RenderSlope(int xdots, int ydots, int PertColourMethod, int PalOffset, double IterDiv);
	void	DoSlopeFwdDiffFn(Complex *z, Complex *q);
	void	DoBigSlopeFwdDiffFn(BigComplex *z, BigComplex *q, BigComplex *bBig, BigComplex *aa3Big);
	void	SlopeIsExiting(void);
	bool	EndSlope;
	double	param[NUMSLOPEDERIVPARAM];
	int	PaletteStart;
	int	PaletteShift = 0;

    private:
	double	cdot(Complex a, Complex b);
	double	GiveReflection(Complex j, BYTE juliaflag, Complex Z, int *iterations, double rqlim, long threshold, Complex v);
	double	GiveBigReflection(Complex j, BYTE juliaflag, BigComplex C, QDComplex cQD, DDComplex cDD, int *iterations, double rqlim, long threshold, Complex v);
	double	GiveDDReflection(Complex j, BYTE juliaflag, DDComplex cDD, int *iterations, double rqlim, long threshold, Complex v);
	double	GiveQDReflection(Complex j, BYTE juliaflag, QDComplex cQD, int *iterations, double rqlim, long threshold, Complex v);
	int	BigDouble2DD(dd_real *out, BigDouble *in);
	int	BigDouble2QD(qd_real *out, BigDouble *in);
	int	ConvertBignumsDD(BigDouble Big_xgap, BigDouble Big_ygap, BigDouble BigHor, BigDouble BigVert, BigDouble BigWidth, dd_real *DDxgap, dd_real *DDygap, dd_real *DDhor, dd_real *DDvert, dd_real *DDWidth);
	int	ConvertBignumsQD(BigDouble Big_xgap, BigDouble Big_ygap, BigDouble BigHor, BigDouble BigVert, BigDouble BigWidth, qd_real *QDxgap, qd_real *QDygap, qd_real *QDhor, qd_real *QDvert, qd_real *QDWidth);
	RGBTRIPLE compute_colour(CTrueCol *TrueCol, Complex j, BYTE juliaflag, Complex c, BigComplex cBig, QDComplex cQD, DDComplex cDD, double rqlim, long threshold, BYTE BigNumFlag, Complex v, bool *Time2Exit);
	void	Create2DVector(Complex *v, double LightAngle);
	double	getGradientX(double *wpixels, int index, int width);
	double	getGradientY(double *wpixels, int index, int width, int height);
	int	changeBrightnessOfColorScaling(int rgb, double delta, double bump_transfer_factor);
	void	InitFwd(BYTE BigNumFlag, BYTE juliaflag, BigComplex cBig, BigComplex *zBig, BigComplex *qBig, BigComplex *aBig, BigComplex *bBig, BigComplex *vBig, BigComplex *a2Big, BigComplex *aa3Big, Complex j, Complex c, Complex *z, Complex *q);

	HWND	hwnd;
	int	NumThreads;
	int	thread;
	int	bits_per_pixel;
	int	precision;
	WORD	*degree;

	// Fwd Diff Render Stuff
	long	threshold;
	CTrueCol *TrueCol;
	CDib	*Dib;
	CBigTrig	BigTrig;		// alternative thread-friendly trig functions
	double	*wpixels;
	double	bump_transfer_factor, lightDirectionDegrees, bumpMappingDepth, bumpMappingStrength, LightHeight;

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
	static RGBTRIPLE color[3]; // 24-bit rgb color
	int	iterations;
	int	StripWidth;
	int	StripStart;
	int	subtype;
	double	SlopeDegree = 2.0;					// used to tell FwdDiff algorithm the degree of the polymomial for slope calculations
	CPlot	Plot;

	// Art Matrix Cubic
	WORD	SpecialColour;
	Complex	t2, t3, a, b, v, a2, aa3;
//	BigComplex	t2Big, t3Big, aBig, bBig, vBig, a2Big, aa3Big;
	BigComplex	zNewton;
	char	variety;
	bool	SpecialFlag;
    };

  