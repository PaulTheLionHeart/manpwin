#pragma once
#include <windows.h>
#include "mpfr.h"
#include "Point.h"
#include "filter.h"
#include "Dib.h"
#include "colour.h"
#include "fractalp.h"
#include "BigDouble.h"
#include "BigComplex.h"
#include "Slope.h"
#include "Complex.h"
#include "ManpWIN.h"
#include "Plot.h"
#include "FloatExp.h"
#include "ExpComplex.h"

#define	MAXPOWER    20
#define	MAXFILTER    9
#define	MAXPOLY	     10		// only because we are limited by param[]

#define	NOSLOPE		0
#define	FWDDIFFSLOPE	1
#define	DERIVSLOPE	2

#undef	ALLOW_SIN
//#define	ALLOW_SIN
#undef	ALLOW_ARTMATRIXCUBIC
//#define	ALLOW_ARTMATRIXCUBIC

class calculateFrame 
    {
    public:
	int	initialiseCalculateFrame(CDib *DibIn, CSlope *Slope, int WidthIn, int HeightIn, int threshold, BigDouble xZoomPointin, BigDouble yZoomPointin, BigDouble BigWidthIn, int decimals, CTZfilter *TZfilter, HWND hwndIn, int ThreadIn, double *wpixelsIn, 
		double paramIn[], double potparamIn[], int PaletteShiftIn, int *PlotTypeIn, int SlopeTypeIn, double lightDirectionDegrees, double bumpMappingDepth, double bumpMappingStrength, int PaletteStartIn, double LightHeightIn, int PertColourMethodIn,
		int PalOffset, double IterDiv);
	int	calculateOneFrame(double bailout, char* StatusBarInfo, int powerin, int FilterTypeIn, int biomorph, int subtype, Complex RSRA, bool RSRsign,
									int user_data(HWND hwnd), int xdotsIn, CTZfilter *TZfilter, CTrueCol *TrueCol, int *pPertProgress, bool *ThreadComplete, bool Multi, int delay, char *PertErrorMessage, HANDLE ghMutex);
	int	SlopeType;
	int	PaletteShift = 0;

    private:
	int	calculatePoint(int x, int y, Complex DeltaSub0, double bailout, Point *glitchPoints, int user_data(HWND hwnd), CTZfilter *TZfilter, CTrueCol *TrueCol, HANDLE ghMutex);
	int	BigCalculatePoint(int x, int y, ExpComplex ExpDeltaSub0, double bailout, Point *glitchPoints, int user_data(HWND hwnd), CTZfilter *TZfilter, CTrueCol *TrueCol, HANDLE ghMutex);
	int	ReferenceZoomPoint(BigComplex& centre, int MAX_ITERATIONS, int user_data(HWND hwnd), char* StatusBarInfo, int *pPertProgress, bool Multi);
	void	LoadPascal(long PascalArray[], int n);
	double	DiffAbs(const double c, const double d);
	floatexp	calculateFrame::BigDiffAbs(floatexp c, floatexp d);
#ifdef	ALLOW_SIN
	void	PertFunctions(Complex *XRef, Complex *SinX, Complex *CosX, Complex *DeltaSubN, Complex *DeltaSub0);
#else
	void	PertFunctions(Complex *XRef, Complex *DeltaSubN, Complex *DeltaSub0);
#endif
	void	CalculateDerivativeSlope(Complex *dc, Complex z);
	void	BigCalculateDerivativeSlope(ExpComplex *dc, ExpComplex z);
	void	ProcessDerivativeSlope(Complex dc, Complex z, HANDLE ghMutex, long iteration, CTrueCol *TrueCol, int x, int y);
	void	BigProcessDerivativeSlope(ExpComplex ExpDC, ExpComplex ExpTemp, HANDLE ghMutex, long iteration, CTrueCol *TrueCol, int x, int y);
	void	ProcessDerivativeSlopeMain(double reflection, HANDLE ghMutex, long iteration, CTrueCol *TrueCol, int x, int y);

	void	RefFunctions(BigComplex *centre, BigComplex *Z, BigComplex *ZTimes2);
	void	CloseTheDamnPointers(void);
	void	BigPertFunctions(ExpComplex *XRef, ExpComplex *DeltaSubN, ExpComplex *DeltaSub0);
	int	Bignum2FloatExp(floatexp *a, BigDouble b);
	int	FloatExp2Bignum(BigDouble *a, floatexp b);
	int	BigComplex2ExpComplex(ExpComplex *a, BigComplex b);
	int	ExpComplex2BigComplex(BigComplex *a, ExpComplex b);

	CBigTrig	BigTrig;		// alternative thread-friendly trig functions

	double	*PerturbationToleranceCheck;
	Complex	*XSubN;
	Complex	*sinZp;
	Complex	*cosZp;
	Complex	calculatedDelta;
	double	ZCoordinateMagnitudeSquared;
	int	skippedIterations = 0;
	int	thread;
	int	precision;		// bignum precision
	int	ReferenceNumber;

	floatexp	*ExpPerturbationToleranceCheck;
	ExpComplex	*ExpXSubN;
	floatexp	ExpZCoordinateMagnitudeSquared;

	long	PascalArray[MAXPOWER+1];
	Point	*pointsRemaining;
	Point	*glitchPoints;
	double	*wpixels;		// an array of doubles holding slope modified iteration counts
	Complex	rsrA;			// TheRedshiftRider value of a
	bool	rsrSign;		// TheRedshiftRider sign true if positive
	CSlope	*Slope;
	CDib	*Dib;
	double	param[NUMPERTPARAM];
	double	potparam[3];		// potential parameters
	int	*PlotType;

	Complex	v;			// unit 2D vector in this direction

	int	PaletteStart;
	double	LightAngle;
	double	lightDirectionDegrees;
	double	bumpMappingDepth;
	double	bumpMappingStrength;
	double	LightHeight;		// height of light for slope calculation
	int	PertColourMethod = 0;	// Kalles colour method
	int	PalOffset = 0;		// begin palette here
	double	IterDiv = 1.0;		// divide ieration by this amount

#define	DOUBLE		0		// use noremal double float
#define	DOUBLESHIFT	1		// use double float shifted by Scaling
#define	FLOATEXP	2		// use floatexp

	int	ArithType = DOUBLE;	// do we need to use FloatExp: abs(width) < 10^300
	int	ScalingOffset = 0;
	double	Scaling = 1.0;
	double	ScalingSquared;

	HWND	hwnd;

	int	xdots;
	int	width, height;
	int	MaxIteration;
	int	power, subtype, method, biomorph; 
	long	GlitchPointCount;
	long	RemainingPointCount;
	mpfr_t	xZoomPt, yZoomPt;

	BigDouble	BigWidth, BigZoomRadius;
	BigComplex	BigCalculatedDelta;

	double	ZoomRadius;
	bool	calculateGlitches = true;
	bool	seriesApproximation = false;
	unsigned int numCoefficients = 5;
	double	percentGlitchTolerance = 0.1;				// What percentage of the image is okay to be glitched. 
	int	referencePoints = 0;
	double	SlopeDegree = 2.0;					// used to tell FwdDiff algorithm the degree of the polymomial for slope calculations

#ifdef ALLOW_ARTMATRIXCUBIC
	// stuff for Arty Matric cubic
	Complex	a, b, aa3, t2, t3;
	BigComplex	aBig, a2Big, aa3Big, bBig, l2Big, lm5Big, lp5Big, ozBig, /*sqrBig, */t2Big, t3Big, vBig;
#endif
    };


