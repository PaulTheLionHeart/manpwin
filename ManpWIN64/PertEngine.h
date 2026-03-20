#pragma once
#include <windows.h>
#include "mpfr.h"
//#include "Point.h"
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
#include "Approximation.h"
#include "PlotMode.h"

#define	MAXPOWER		20
#define	MAXFILTER		9
#define	MAXPOLY			10	// only because we are limited by param[]
#define MAXLINE			250	// maximum length of a line

#define	NOSLOPE			0
#define	FWDDIFFSLOPE		1
#define	DERIVSLOPE		2

#define	DOUBLE			0	// use noremal double float
#define	FLOATEXP		1	// use floatexp
#define	DBL_UNSUPPORTED		2	// use double for calculation, but don't process BLA table at this stage as this fractal is unsupported by BLA
#define	EXP_UNSUPPORTED		3	// use floatexp for calculation, but don't process BLA table at this stage as this fractal is unsupported by BLA

#define max(a,b)		(((a) > (b)) ? (a) : (b))
#define min(a,b)		(((a) < (b)) ? (a) : (b))

struct StoreReferenceData
    {
    bool	valid = false;
    BigDouble	BigWidth = 0.0;
    BigComplex	ReferenceCoordinate = 0.0;
    double	rqlim = 0;
    WORD	degree;
    };

typedef	struct MyData
    {
    double bailout;
    char* StatusBarInfo;
    int powerin;
    int FilterTypeIn;
    int biomorph;
    int subtype;
    int SlopeDegree;
    bool IsPositive;			// the next 3 are specific for RedShiftRider
    Complex RSRA;
    bool RSRsign;
    int(*user_data)(HWND hwnd);		// detect user input
    int(*potential)(double, int);	// plot potential
    CTZfilter *TZfilter;		// pass through Tierazon filters
    CTrueCol *TrueCol;			// colouring scheme
    int *pPertProgress;			// pass back the status of each thread
    int	ArithType;
    int	MaxRefIteration;
    int	i;				// thread number
    HANDLE  ghMutex;
    } MYDATA, *PMYDATA;

class CPerturbation
    {
    public:
	CPerturbation();			// default binds to global wpixels
	CPerturbation(std::vector<float>&);	// explicit
	int	initialiseCalculateFrame(CDib *DibIn, CSlope *Slope, int xStartIn, int xEndIn, int HeightIn, int threshold, BigDouble xZoomPointin, BigDouble yZoomPointin, BigDouble BigWidthIn, int decimals, CTZfilter *TZfilter, HWND hwndIn,
		int ThreadIn, std::vector<float> &wpixelsIn, double paramIn[], double potparamIn[], int PaletteShiftIn, int *PlotTypeIn, int SlopeTypeIn, double lightDirectionDegrees, double bumpMappingDepth, double bumpMappingStrength,
		int PaletteStartIn, double LightHeightIn, int PertColourMethodIn, int PalOffset, double IterDiv, bool EnableApproximationIn, BYTE _3dflagIn, double ColourSpeedIn, int NumberThreadsIn);
	int	calculateOneFrame(double bailout, char* StatusBarInfo, int powerin, int InsideMethodIn, int OutsideMethodIn, int biomorphin, int subtypein, Complex rsrAin, bool rsrSignIn, int user_data(HWND hwnd), int xdotsIn, 
		CTZfilter &TZfilter, CTrueCol &TrueCol, int *pPertProgress, BYTE &ThreadComplete, bool Multi, int delay, char *PertErrorMessage, int ArithTypeIn, int MaxRefIteration, int SlopeDegreeIn, HANDLE ghMutex);
	void	AttachSharedTables(const std::vector<Complex>* xs, const std::vector<ExpComplex>* exs, const BLAS* bla/*, int maxRefIter*/);

	int	BigComplex2ExpComplex(ExpComplex *a, BigComplex b);
//	void	RefFunctions(BigComplex *centre, BigComplex *Z, int &SlopeDegree, int subtype);

	int	SlopeType;
	int	PaletteShift = 0;

    private:
	void	LoadPascal(std::vector<long>   &PascalArray, int n);
	double	DiffAbs(const double c, const double d);
	floatexp BigDiffAbs(floatexp c, floatexp d);
	void	PertFunctions(Complex *XRef, Complex *DeltaSubN, Complex *DeltaSub0);
	void	CalculateDerivativeSlope(Complex &dc, Complex z);
	void	BigCalculateDerivativeSlope(ExpComplex &dc, ExpComplex z);
	void	ProcessDerivativeSlope(Complex dc, Complex z, HANDLE ghMutex, long MaxIteration, long iteration, CTrueCol &TrueCol, int x, int y);
	void	BigProcessDerivativeSlope(ExpComplex ExpDC, ExpComplex ExpTemp, HANDLE ghMutex, long MaxIteration, long iteration, CTrueCol &TrueCol, int x, int y);
	void	ProcessDerivativeSlopeMain(Complex z, double reflection, HANDLE ghMutex, long MaxIteration, long iteration, CTrueCol &TrueCol, int x, int y);
	int	ColourProcessing(Complex z, long iteration, int x, int y, CTrueCol &TrueCol, CTZfilter &TZfilter, double bailout);
	int	ColourProcessingExp(ExpComplex z, long iteration, int x, int y, CTrueCol &TrueCol, CTZfilter &TZfilter, double bailout);

	void	BigPertFunctions(ExpComplex *XRef, ExpComplex *DeltaSubN, ExpComplex *DeltaSub0);
	int	Bignum2FloatExp(floatexp *a, BigDouble b);
	int	FloatExp2Bignum(BigDouble *a, floatexp b);
	int	ExpComplex2BigComplex(BigComplex *a, ExpComplex b);

	int	iterateFractalWithPerturbationBLA(const std::vector<Complex> *XSubN, int MaxIteration, double bailout, Complex& DeltaSub0, const BLAS *Bla, CTZfilter &TZfilter, Complex &z, Complex &dc, int user_data(HWND hwnd));
	int	iterateFractalWithPerturbationBLAExp(const std::vector<ExpComplex> *XSubN, int MaxIteration, double bailout, ExpComplex& DeltaSub0, const BLAS *Bla, CTZfilter &TZfilter, ExpComplex &z, ExpComplex &dc, int user_data(HWND hwnd));
	RGBTRIPLE GetSmoothedColour(double fIter, double color_speed, CTrueCol &TrueCol, CPlot *Plot);

	CBigTrig	BigTrig;		// alternative thread-friendly trig functions
	const	BLAS    *Bla;			// Bilinear Approximation
	bool	EnableApproximation;
	int	MaxRefIteration;
	void	debugPrint(const std::string& msg);

	double	ZCoordinateMagnitudeSquared;
	floatexp	ExpZCoordinateMagnitudeSquared;
	int	thread;
	int	NumberThreads;
	int	precision;			// bignum precision

	double	BOFmagnitude;
	double	min_orbit;			// orbit value closest to origin
	long	min_index;			// iteration of min_orbit

//	std::vector<floatexp>	ExpPerturbationToleranceCheck;
	const std::vector<ExpComplex>	*ExpXSubN;
//	std::vector<double>	PerturbationToleranceCheck;
	const std::vector<Complex>	*XSubN;

	std::vector<long>   PascalArray {0};
	std::vector<float> &wpixels;	// an array of doubles holding slope modified iteration counts
	Complex	rsrA;			// TheRedshiftRider value of a
	bool	rsrSign;		// TheRedshiftRider sign true if positive
	CSlope	*Slope;
	CDib	*Dib;
	double	param[NUMPERTPARAM];
	double	potparam[3];		// potential parameters
	int	*PlotType;
	BYTE	_3dflag;
	int	smoothing = 0;
	double	ColourSpeed = 1.0;

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

	int	ArithType = DOUBLE;	// do we need to use FloatExp: abs(width) < 10^300
	HWND	hwnd;

	int	xdots;
	int	width, height;
	int	MaxIteration;
	int	power = 3;
	int	subtype = 0; 
	int	biomorph = -1; 
	int	InsideMethod;			// the number of the inside filter
	int	OutsideMethod;			// the number of the outside filter

	BigDouble	BigWidth, BigCentreX, BigCentreY;
	int	SlopeDegree = 2;					// used to tell FwdDiff algorithm the degree of the polymomial for slope calculations
    };


