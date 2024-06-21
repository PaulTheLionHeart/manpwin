/****************************************************
    PERTURBATION.CPP a module to explore Perturbation
    Thanks to Shirom Makkad fractaltodesktop@gmail.com
    Written in Microsoft Visual C++ by Paul de Leeuw.
****************************************************/

#include <process.h>
#include <strsafe.h>
#include "FrameCalculator.h"
#include "fractalp.h"
#include "manpwin.h"
#include "manp.h"
#include "BigDouble.h"
#include "colour.h"
#include "resource.h"
#include "dib.h"
#include "filter.h"
#include "Fract.h"
#include "Fractype.h"
#include "big.h"
#include "Slope.h"
//#include "pixel.h"

#define		MAXTHREADS	40

extern	void	ConvertBignum2String(char *s, mpfr_t num);
extern	int	user_data(HWND);
extern	void	DisplayStatusBarInfo(int, char *);
extern	char	*GetFractalName(void);

void ErrorHandler(LPTSTR lpszFunction);

extern	long	threshold;
extern	double	hor;				// horizontal address
extern	double	vert;				// vertical address
extern	double	mandel_width;			// width of display
extern	BigDouble   BigHor, BigVert, BigWidth;
extern	BYTE	BigNumFlag;			// True if bignum used
extern	double 	*wpixels;			// floating point iterations for each pixels
//extern	char	szStatus[];			// status bar text
extern	double	rqlim;				// bailout level
extern	int	decimals;
extern	double	param[];
extern	double	potparam[];			// potential parameters
//extern	int	method;				// inside and outside filters
extern	int	InsideMethod;			// inside filters
extern	int	OutsideMethod;			// outside filters
extern	int	nFDOption;			// Fractal Dimension option for Tierazon filters
extern	WORD	degree;				// power
extern	int	biomorph;			// biomorph colour
extern	double	dStrands;
extern	BOOL	UseCurrentPalette;		// do we use the ManpWIN palette? If false, generate internal filter palette
extern	int	curpass, totpasses;
extern	BOOL	RunAnimation;			// are we in the middle of an animation generation run?
extern	double	ScreenRatio;			// ratio of width / height for the screen
extern	int	width, height;
extern	int	subtype;
extern	int	precision;
extern	int	xdots, ydots;
extern	BYTE	juliaflag;			// Julia implementation of fractal 
extern	int	PlotType;

// stuff for DwdDiff algorithm
extern	double	bump_transfer_factor;
extern	int	PaletteStart;
extern	double	lightDirectionDegrees;
extern	double	bumpMappingDepth;
extern	double	bumpMappingStrength;

extern	double	LightHeight;			// height of light for slope calculation

extern	int	PaletteShift;			// fractional palette addressing

//////////////////////////////////////////////////////////////////////////////////////////////////////
extern	HWND	GlobalHwnd;			// This is the main windows handle
//////////////////////////////////////////////////////////////////////////////////////////////////////

extern	CTrueCol    TrueCol;			// palette info
extern	CDib	Dib;
extern	CFract	Fractal;			// current fractal stuff
//extern	CPixel	Pix;				// routines for escape fractals

static	CTZfilter	TZfilter[MAXTHREADS];	// Tierazon filters

calculateFrame frameCalculator[MAXTHREADS];
int	PertProgress[MAXTHREADS];
int	*pPertProgress;
bool	ThreadComplete[MAXTHREADS];
char	PertStatus[200];
char	PertErrorMessage[200];
bool	UseMutex = false;			// trade off speed for accuracy
double	IterDiv = 1.0;				// divide ieration by this amount
int	PalOffset = 0;				// Start Palette here
int	RotationAngle = 0;			// rotate image in degrees

static	int	PerturbationPtr = 0, PerturbationNum = 0;
	int	NumberThreads = 0;		// set the default to 0. Multi=threading is for experimentation only
static	HANDLE	hThread[MAXTHREADS];
	int	SlopeType;

int	ThreadCreationDelay = 5;		// not sure if these are required
int	ThreadCompletionDelay = 2;
int	ThreadEndingDelay = 2;
int	ThreadPertDelay = 0;			// if > 0, show pixel update in real time. Otherwise stay with screen update every second (faster)

int	PertColourMethod = 0;			// Kalles colour method

typedef	struct MyData
    {
    double bailout;
    char* StatusBarInfo;
    int powerin;
    int FilterTypeIn;	
    int biomorph;
    int subtype;
    bool IsPositive;				// the next 3 are specific for RedShiftRider
    Complex RSRA;
    bool RSRsign;
    int (*user_data)(HWND hwnd);		// detect user input
    int (*potential)(double, int);		// plot potential
    CTZfilter *TZfilter;			// pass through Tierazon filters
    CTrueCol *TrueCol;				// colouring scheme
    int *pPertProgress;				// pass back the status of each thread
    PMYVARIABLES;
    int i;
    HANDLE  ghMutex;
    } MYDATA, *PMYDATA;

PMYDATA	    pDataArray[MAXTHREADS];
MYDATA	    DataArrayZero;

static	    CSlope	Slope;

/**************************************************************************
	Perturbation engine, pass into thread creation routine
**************************************************************************/

DWORD	WINAPI PertFunction(LPVOID lpParam)
    {
    PMYDATA pDataArray;
    int	    ReturnValue;

    pDataArray = (PMYDATA)lpParam;
    int(*UserData)(HWND) = user_data;

    ReturnValue = frameCalculator[pDataArray->i].calculateOneFrame(rqlim, PertStatus, degree, InsideMethod, OutsideMethod, biomorph, subtype, pDataArray->RSRA, pDataArray->IsPositive, UserData, xdots,
		pDataArray->TZfilter, pDataArray->TrueCol, pDataArray->pPertProgress, &ThreadComplete[pDataArray->i], (NumberThreads > 0), ThreadPertDelay, PertErrorMessage, pDataArray->ghMutex);
    if (ReturnValue < 0)
	{
	if (ReturnValue == -2)			// actual error, else user input
	    ErrorHandler(PertErrorMessage);
	Sleep(50);				// give the threads time to close
	return -1;
	}
    return 0;
    }

/**************************************************************************
	Initialise Perturbation engine
**************************************************************************/

int	InitPerturbation(void)
    {
    BigDouble	BigCentrex, BigCentrey;
    int		i;
    void	pPertFunction(void *);

    DWORD	size = sizeof(CTZfilter);
    static	bool	FirstTime = true;

    if (FirstTime)
	{
	for (i = 0; i < NumberThreads; i++)
	    pDataArray[i] = NULL;								// Ensure address is not reset before being allocated.
	FirstTime = false;
	}

    /*
    // stuff for DwdDiff algorithm
    bump_transfer_factor = param[0];
    PaletteStart = (int)param[1];
    lightDirectionDegrees = param[2];
    bumpMappingDepth = param[3];
    bumpMappingStrength = param[4];
    */

    if (!BigNumFlag)
	{
	BigHor = hor;
	BigVert = vert;
	BigWidth = mandel_width;
	}

//    if (precision > MAXDERPRECISION && param[0] == 2)
//	param[0] = 1;										// beyond MAXDERPRECISION precision, derivative slope crashes, so change to fwd diff

    if (NumberThreads > MAXTHREADS)
	NumberThreads = MAXTHREADS;
    if (InsideMethod > 0 || OutsideMethod > 0 || biomorph >= 0)					// can't have it both ways
	SlopeType = NOSLOPE;
    if (NumberThreads <= 0)									// no multi-threading
	BigCentrex = BigHor + (BigWidth * ((double)Dib.DibWidth / (double)(2 * Dib.DibHeight)));
    BigCentrey = -(BigVert + (BigWidth / 2.0));							// negative because of screen layout
    if (InsideMethod >= TIERAZONFILTERS)
	for (i = 0; i < NumberThreads; i++)
	    TZfilter[i].InitFilter(InsideMethod, threshold, dStrands, nFDOption, UseCurrentPalette);		// initialise the constants used by Tierazon fractals
//    if (BigNumFlag)
//	mandel_width = mpfr_get_d(BigWidth.x, MPFR_RNDN);
    if (NumberThreads <= 0)									// no multi-threading
	{
	if (InsideMethod >= TIERAZONFILTERS)
	    TZfilter[0].InitFilter(InsideMethod, threshold, dStrands, nFDOption, UseCurrentPalette);		// initialise the constants used by Tierazon fractals
	frameCalculator[0].initialiseCalculateFrame(&Dib, &Slope, (int)Dib.DibWidth, (int)Dib.DibHeight, threshold, BigCentrex, BigCentrey, BigWidth / 2, decimals, &TZfilter[0], GlobalHwnd, 0, wpixels, param, potparam, 
			PaletteShift, &PlotType, SlopeType, lightDirectionDegrees, bumpMappingDepth, bumpMappingStrength, PaletteStart, LightHeight, PertColourMethod, PalOffset, IterDiv);
	}
    else
	{
//	i = 1;
	for (i = 0; i < NumberThreads; i++)
	    {
	    BigCentrex = (BigHor + BigWidth * ScreenRatio * (double)i / (double)NumberThreads) + (BigWidth * ((double)Dib.DibWidth / (double)(2 * NumberThreads * Dib.DibHeight)));	// find centre of each slice
	    frameCalculator[i].initialiseCalculateFrame(&Dib, &Slope, (int)Dib.DibWidth / NumberThreads, (int)Dib.DibHeight, threshold, BigCentrex, BigCentrey, BigWidth * ScreenRatio / (2 * NumberThreads), decimals, &TZfilter[i], GlobalHwnd, i, wpixels, param, potparam,
			PaletteShift, &PlotType, SlopeType, lightDirectionDegrees, bumpMappingDepth, bumpMappingStrength, PaletteStart, LightHeight, PertColourMethod, PalOffset, IterDiv);
	    }
	}

    return 0;
    }

/**************************************************************************
	The Perturbation engine
**************************************************************************/

int	DoPerturbation(PMYVARIABLES)
    {
    int		(*UserData)(HWND) = user_data;
    int		i;
//    DWORD	ExitCode;
    DWORD	dwThreadId[MAXTHREADS];
    int		flag, status;
    HANDLE	ghMutex = NULL;									// manage access to shared resources such as Dib and wpixels

    if (UseMutex)										// stuff speed, we want a good image
	ghMutex = CreateMutex(NULL, 0, NULL);
    for (int i = 0; i < NumberThreads; i++)
	{
	if (pDataArray[i] != NULL)
	    {
	    HeapFree(GetProcessHeap(), 0, pDataArray[i]);					// we free memory here because we don't know when all the threads are finished
	    pDataArray[i] = NULL;								// flag it is available for reuse
	    }
	}

    switch (subtype)
	{
	case 1:
	    degree = (int)param[2];
	    break;
	case 11:
	case 53:
	    degree = (int)param[1];
	    break;
	case 13:
	    degree = 3;
	    break;
	case 14:
	    degree = 4;
	    break;
	case 15:
	    degree = 5;
	    break;
	}

    if (NumberThreads <= 0)						// no multi-threading
	{
	if (PerturbationSpecific[subtype].RedShiftRider == 1)
	    {
	    DataArrayZero.RSRA.x = param[1];
	    DataArrayZero.RSRA.y = param[2];
	    degree = (int)param[3];
	    DataArrayZero.IsPositive = (param[4] == 1.0);
	    }

	status = frameCalculator[0].calculateOneFrame(rqlim, PertStatus, degree, InsideMethod, OutsideMethod, biomorph, subtype, DataArrayZero.RSRA, DataArrayZero.IsPositive, UserData, xdots, &TZfilter[0], &TrueCol, &PertProgress[0],
				&ThreadComplete[0], false, ThreadPertDelay, PertErrorMessage, ghMutex);
	if (status < 0)
	    {
	    sprintf(PertErrorMessage, "CreateThread failed in single thread version");
	    return -1;
	    }
	}
    else
	{
	for (i = 0; i < NumberThreads; i++)
	    {
	    pDataArray[i] = (PMYDATA)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(MYDATA));

	    if (pDataArray[i] == NULL)
		{
		// don't start this thread, just move on
		sprintf(PertErrorMessage, "Can't find memory for data array in thread %d", i);
		ErrorHandler(PertErrorMessage);
		break;
		}

	    if (PerturbationSpecific[subtype].RedShiftRider == 1)
		{
		pDataArray[i]->RSRA.x = param[1];
		pDataArray[i]->RSRA.y = param[2];
		degree = (int)param[3];
		pDataArray[i]->IsPositive = (param[4] == 1.0);
		}

	    pDataArray[i]->i = i;
	    pDataArray[i]->pPertProgress = &PertProgress[i];
	    pDataArray[i]->TrueCol = &TrueCol;
	    pDataArray[i]->TZfilter = &TZfilter[i];
	    pDataArray[i]->ghMutex = ghMutex;

	    hThread[i] = CreateThread(
		NULL,						// default security attributes
		0L,						// use default stack size = 0
		(LPTHREAD_START_ROUTINE)PertFunction,		// thread function 
		pDataArray[i],					// argument to thread function 
		0,						// use default creation flags 
		&dwThreadId[i]);				// returns the thread identifier 
	    if (hThread[i] == NULL)
		{
		sprintf(PertErrorMessage, "CreateThread failed in thread %d", i);
		ErrorHandler(PertErrorMessage);
		break;
		}
	    SetThreadPriority(hThread[i], THREAD_PRIORITY_LOWEST);
	    CloseHandle(hThread[i]);
	    Sleep(ThreadCreationDelay);
	    }

	flag = 0;						// make sure all threads are finished

	totpasses = 100;
	while (flag == 0)
	    {
	    if (user_data(GlobalHwnd) == -1)			// user pressed a key?
		{
		Sleep(2500);
		return -1;
		}
	    flag = 1;
	    curpass = 100;
	    for (i = 0; i < NumberThreads; i++)
		{
		if (curpass > PertProgress[i])
		    curpass = PertProgress[i];			// display the slowest thread
		if (ThreadComplete[i] == false)
		    flag = 0;
		}
	    if (!RunAnimation)
		DisplayStatusBarInfo(INCOMPLETE, "");
	    Sleep(ThreadCompletionDelay);			// give the threads time to close
	    }
	Sleep(ThreadEndingDelay);				// make sure all threads are closed before leaving
	}

    /*
    Perturbation parameters have a standard mainly based on forward differencing which is the dominant subtype. 
    param[]:	meaning
    0		Slope type or transfer factor
    1		Start palette
    2		Light Direction
    3		Mapping Depth
    4		Mapping Strength
    5		Degree

    But for some subtypes it gets complicated, so we remap a few here
    */

    Slope.PaletteShift = frameCalculator[0].PaletteShift;
    if (frameCalculator[0].SlopeType == FWDDIFFSLOPE)
	{
//	if (subtype == 0)					// Mandelbrot
//	    param[0] = param[5];				// transfer factor as we already know what the slope type is
//	else if (subtype == 1)					// Power
//	    param[0] = param[5];
	Slope.InitRender(threshold, &TrueCol, &Dib, wpixels, PaletteShift, bump_transfer_factor, PaletteStart, lightDirectionDegrees, bumpMappingDepth, bumpMappingStrength);
	Slope.RenderSlope(xdots, ydots, PertColourMethod, PalOffset, IterDiv);
	}
    if (ghMutex != NULL)
	{
	CloseHandle(ghMutex);
	ghMutex = NULL;
	}
    return 0;
    }

/**************************************************************************
    Error handling - courtesy of Microsnot
**************************************************************************/

void ErrorHandler(LPTSTR lpszFunction)
    {
    // Retrieve the system error message for the last-error code.

    LPVOID lpMsgBuf;
    LPVOID lpDisplayBuf;
    DWORD dw = GetLastError();

    FormatMessage(
	FORMAT_MESSAGE_ALLOCATE_BUFFER |
	FORMAT_MESSAGE_FROM_SYSTEM |
	FORMAT_MESSAGE_IGNORE_INSERTS,
	NULL,
	dw,
	MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
	(LPTSTR)&lpMsgBuf,
	0, NULL);

    // Display the error message.

    lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT, (lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR));
    StringCchPrintf((LPTSTR)lpDisplayBuf, LocalSize(lpDisplayBuf) / sizeof(TCHAR), "%s failed with error %d: %s", lpszFunction, dw, lpMsgBuf);
    MessageBox(GlobalHwnd, (LPCTSTR)lpDisplayBuf, "Error", MB_OK);

    // Free error-handling buffer allocations.

    LocalFree(lpMsgBuf);
    LocalFree(lpDisplayBuf);
    }

/**************************************************************************
    Some simple processing
**************************************************************************/

int	setup_Perturbation(void)
    {
    if (!PerturbationNum)	    // we'd better count how many records we have
	{
	while (PerturbationSpecific[PerturbationNum].name)
	    PerturbationNum++;
	}
    return PerturbationNum;
    }

void	LoadPerturbationParams(void)
    {
    // here is where we can do some specific updates to individual Perturbation fractals
    int	i;

    Fractal.NumParam = PerturbationSpecific[subtype].numparams;
    for (i = 0; i < NUMPERTPARAM; i++)
	{
	param[i] = PerturbationSpecific[subtype].paramvalue[i];
	Fractal.ParamName[i] = PerturbationSpecific[subtype].paramname[i];
	}
    rqlim = PerturbationSpecific[subtype].rqlim;
    SlopeType = PerturbationSpecific[subtype].SlopeType;
    PaletteStart = PerturbationSpecific[subtype].PaletteStart;
    lightDirectionDegrees = PerturbationSpecific[subtype].lightDirectionDegrees;
    bumpMappingDepth = PerturbationSpecific[subtype].bumpMappingDepth;
    bumpMappingStrength = PerturbationSpecific[subtype].bumpMappingStrength;
    }

/**************************************************************************
    Toggle Perturbation lookup table
    This allows one to toggle between selected fractals that also have
    a perturbation method available and back again
**************************************************************************/

int	TogglePerturbation(WORD *type, int *subtype)
    {
    if (*type == MANDELFP || *type == MANDEL)
	{
	*type = PERTURBATION;
	*subtype = 0;
	}
    else if (*type == POWER)
	{
	*type = PERTURBATION;
	*subtype = 1;
	param[2] = degree;
	}
    else if (*type == REDSHIFTRIDER)
	{
	*type = PERTURBATION;
	*subtype = 53;
	param[4] = param[5];		// copy sign
	param[3] = param[2];
	param[2] = param[1];
	param[1] = param[0];
	param[0] = 1.0;			// copy params
	}
    else if (*type == SLOPEDERIVATIVE)
	{
	*type = PERTURBATION;
	switch (*subtype)
	    {
	    case 0:
		*subtype = 0;
		break;
	    case 1:
		*subtype = 1;
		param[2] = 3.0;
		break;
	    case 2:
		*subtype = 1;
		param[2] = param[3];
		break;
	    default:
		return -1;
	    }
	}
    else if (*type == TALIS)
	{
	*type = PERTURBATION;
	rqlim = 12.0;
	switch (degree)
	    {
	    case 2:
		*subtype = 54;
		break;
	    case 3:
		*subtype = 55;
		break;
	    case 4:
		*subtype = 56;
		break;
	    default:
		*subtype = 54;
		break;
	    }
	/*
		param[0] = 1.0;
		param[1] = 0.0;
		param[2] = 45.0;
		param[3] = 50.0;
		param[4] = 50.0;
		param[5] = 0.0;
	*/
	}
    else if (*type == PERTURBATION)
	{
	*type = MANDELDERIVATIVES;		// for most types - exceptions handled separately
	juliaflag = FALSE;
	switch (*subtype)
	    {
	    case 0:
		*type = MANDELFP;
		param[0] = param[1] = 0.0;	// reset params
		break;
	    case 1:
		*type = POWER;
		param[0] = param[2];		// copy degree
		param[1] = param[2] = 0.0;	// reset other params
		break;
	    case 2:
		*subtype = 1;			// Burning Ship
		param[0] = param[1] = 0.0;	// reset params
		break;
	    case 3:
		*subtype = 2;			// cubic Burning Ship
		degree = 3;
		param[0] = degree;
		param[1] = param[2] = 0.0;
		break;
	    case 4:
		*subtype = 21;			// 4th Power Burning Ship
		param[0] = param[1] = 0.0;
		break;
	    case 5:
		*subtype = 46;			// 5th Power Burning Ship
		param[0] = param[1] = 0.0;
		break;
	    case 6:
		degree = 2;
		*subtype = 7;			// Celtic
		param[0] = degree;
		param[1] = param[2] = 0.0;
		break;
	    case 7:
		degree = 3;
		*subtype = 7;			// Cubic Celtic
		param[0] = degree;
		param[1] = param[2] = 0.0;
		break;
	    case 8:
		*subtype = 31;			// Celtic Buffalo 4th
		param[0] = param[1] = 0.0;
		break;
	    case 9:
		*subtype = 50;			// Celtic 5th (Buffalo Partial)
		param[0] = param[1] = 0.0;
		break;
	    case 10:
		*subtype = 6;			// Mandelbar Tricorn
		param[0] = degree = 2;
		param[1] = param[2] = 0.0;
		break;
	    case 11:
		*subtype = 6;			// Mandelbar power
		degree = (BYTE)param[1];
		param[0] = degree;
		param[1] = param[2] = 0.0;
		param[3] = 1.0;
		if (degree == 4 || degree == 5)
		    param[3] = 0.0;
		if (degree > 5)
		    return -1;
		break;
	    case 12:
		*subtype = 4;			// Buffalo
		param[0] = degree = 2;
		param[1] = param[2] = 0.0;
		break;
	    case 13:
		*subtype = 4;			// Cubic Buffalo only
		param[0] = degree = 3;
		param[1] = param[2] = 0.0;
		break;
	    case 14:
		*subtype = 4;			// Quartic Buffalo only
		param[0] = degree = 4;
		param[1] = param[2] = 0.0;
		break;
	    case 15:
		*subtype = 4;			// Qunitic Buffalo only
		param[0] = degree = 5;
		param[1] = param[2] = 0.0;
		break;
	    case 16:
		*subtype = 8;			// Mandelbar Celtic
		param[0] = param[1] = 0.0;
		break;
	    case 17:
		*subtype = 0;			// Perpendicular Mandelbrot
		param[0] = param[1] = 0.0;
		break;
	    case 18:
		*subtype = 3;			// Perpendicular Burning Ship
		param[0] = param[1] = 0.0;
		break;
	    case 19:
		*subtype = 9;			// Perpendicular Celtic
		param[0] = param[1] = 0.0;
		break;
	    case 20:
		*subtype = 5;			// Perpendicular Buffalo
		param[0] = param[1] = 0.0;
		break;
	    case 21:
		*subtype = 16;			//  Cubic Quasi Burning Ship 
		param[0] = param[1] = 0.0;
		break;
	    case 22:
		*subtype = 13;			// Partial Cubic Burning Ship Real 
		param[0] = param[1] = 0.0;
		break;
	    case 23:
		*subtype = 14;			// Partial Cubic Burning Ship Imaginary 
		param[0] = param[1] = 0.0;
		break;
	    case 24:
		*subtype = 10;			// Cubic Flying Squirrel (Buffalo Imaginary)
		param[0] = param[1] = 0.0;
		break;
	    case 25:
		*subtype = 17;			// Cubic Quasi Perpendicular
		param[0] = param[1] = 0.0;
		break;
	    case 26:
		*subtype = 22;			// Burning Ship 4th Partial imag
		param[0] = param[1] = 0.0;
		break;
	    case 27:
		*subtype = 23;			// Burning Ship 4th Partial Real
		param[0] = param[1] = 0.0;
		break;
	    case 28:
		*subtype = 24;			// Burning Ship 4th Partial Real Mandelbar
		param[0] = param[1] = 0.0;
		break;
	    case 29:
		*subtype = 26;			// Celtic Burning Ship 4th Partial imag
		param[0] = param[1] = 0.0;
		break;
	    case 30:
		*subtype = 27;			// Celtic Burning Ship 4th Partial Real
		param[0] = param[1] = 0.0;
		break;
	    case 31:
		*subtype = 28;			// Celtic Burning Ship 4th Partial Real Mandelbar
		param[0] = param[1] = 0.0;
		break;
	    case 32:
		*subtype = 30;			// Buffalo 4th Partial imag
		param[0] = param[1] = 0.0;
		break;
	    case 33:
		*subtype = 32;			// Celtic 4th Mandelbar
		param[0] = param[1] = 0.0;
		break;
	    case 34:
		*subtype = 33;			// 4th False Quasi Perpendicular 
		param[0] = param[1] = 0.0;
		break;
	    case 35:
		*subtype = 34;			// 4th False Quasi Heart 
		param[0] = param[1] = 0.0;
		break;
	    case 36:
		*subtype = 35;			// Celtic 4th False Quasi Perpendicular 
		param[0] = param[1] = 0.0;
		break;
	    case 37:
		*subtype = 36;			// Celtic 4th False Quasi Heart 
		param[0] = param[1] = 0.0;
		break;
	    case 38:
		*subtype = 37;			// Imag Quasi Perpendicular / Heart 4th 
		param[0] = param[1] = 0.0;
		break;
	    case 39:
		*subtype = 38;			// 4th Quasi Perpendicular Real 
		param[0] = param[1] = 0.0;
		break;
	    case 40:
		*subtype = 39;			// 4th Quasi Heart Real 
		param[0] = param[1] = 0.0;
		break;
	    case 41:
		*subtype = 40;			// Celtic Imag Quasi Perpendicular / Heart 4th
		param[0] = param[1] = 0.0;
		break;
	    case 42:
		*subtype = 41;			// Celtic 4th Quasi Perpendicular Real 
		param[0] = param[1] = 0.0;
		break;
	    case 43:
		*subtype = 42;			// Celtic 4th Quasi Heart Real 
		param[0] = param[1] = 0.0;
		break;
	    case 44:
		*subtype = 48;			// Burning Ship 5th Partial
		param[0] = param[1] = 0.0;
		break;
	    case 45:
		*subtype = 49;			// Burning Ship 5th Partial Mandelbar
		param[0] = param[1] = 0.0;
		break;
	    case 46:
		*subtype = 51;			// Celtic 5th Mandelbar
		param[0] = param[1] = 0.0;
		break;
	    case 47:
		*subtype = 52;			// Quazi Burning Ship 5th (BS/Buffalo Hybrid)
		param[0] = param[1] = 0.0;
		break;
	    case 48:
		*subtype = 53;			// 5th Quasi Perpendicular
		param[0] = param[1] = 0.0;
		break;
	    case 49:
		*subtype = 54;			// 5th Quasi Heart
		param[0] = param[1] = 0.0;
		break;
	    case 50:
		*subtype = 55;			// SimonBrot
		param[0] = 4;
		param[1] = param[2] = 0.0;
		break;
	    case 51:
		*subtype = 55;			// Cubic SimonBrot
		param[0] = 6;
		param[1] = param[2] = 0.0;
		break;
	    case 52:
		*subtype = 56;			// SimonBrot2
		param[0] = 4;
		param[1] = param[2] = 0.0;
		break;
	    case 53:

		param[0] = param[1];		// copy params
		param[1] = param[2];
		param[2] = param[3];
		param[5] = param[4];		// copy sign
		param[3] = param[4] = 0.0;

		/*
				param[4] = param[5];		// copy sign
				param[3] = param[2];
				param[2] = param[1];
				param[1] = param[0];
				param[0] = 1.0;			// copy params
		*/


		*type = REDSHIFTRIDER;
		break;
	    case 54:
	    case 55:
	    case 56:
		*type = TALIS;
		rqlim = 400.0;
		param[0] = degree = *subtype - 52;
		param[1] = 1.0;
		param[2] = 0.0;
		param[3] = 0.0;
		break;
	    case 58:
		*subtype = 58;		// HPDZ Buffalo
		param[0] = 0.0;
		param[1] = 0.0;
		param[2] = 0.0;
		param[3] = 0.0;
		break;
	    default:
		return -1;
	    }
	//	for (int i = 0; i < NUMPARAM; i++)
	//	    param[i] = 0;
	}
    else if (*type == MANDELDERIVATIVES)
	{
	*type = PERTURBATION;
	switch (*subtype)
	    {
	    case 0:
		*subtype = 17;		// Perpendicular Mandelbrot
		break;
	    case 1:
		*subtype = 2;		// Burning Ship
		break;
	    case 2:
		if (degree == 3)
		    *subtype = 3;	// Cubic Burning Ship
		else if (degree == 4)
		    *subtype = 4;	// 4th Power Burning Ship
		else if (degree == 5)
		    *subtype = 5;	// 5th Power Burning Ship
		else
		    return -1;
		break;
	    case 3:
		*subtype = 18;		// Perpendicular Burning Ship
		break;
	    case 4:
		if (degree == 2)
		    *subtype = 12;	// Buffalo
		else if (degree == 3)
		    *subtype = 13;	// Cubic Buffalo
		else if (degree == 4)
		    *subtype = 14;	// Quartic Buffalo
		else if (degree == 5)
		    *subtype = 15;	// Quintic Buffalo
		else
		    return -1;
		break;
	    case 5:
		*subtype = 20;		// Perpendicular Buffalo
		break;
	    case 6:
		if (degree == 2)
		    *subtype = 10;	// Mandelbar Tricorn
		else
		    {
		    degree = (BYTE)param[0];
		    param[0] = param[2] = 0.0;
		    param[1] = degree;
		    *subtype = 11;	// Mandelbar Tricorn Higher order
		    }
		break;
	    case 7:
		if (degree == 3)
		    *subtype = 7;	// cubic Celtic
		else if (degree == 2)
		    *subtype = 6;	// Celtic
		break;
	    case 8:
		*subtype = 16;		// Mandelbar Celtic
		break;
	    case 9:
		*subtype = 19;		// Perpendicular Celtic
		break;
	    case 10:
		*subtype = 24;		// Cubic Flying Squirrel (Buffalo Imaginary)
		break;
	    case 13:
		*subtype = 22;		// Partial Cubic Burning Ship Real 
		break;
	    case 14:
		*subtype = 23;		// Partial Cubic Burning Ship Imaginary
		break;
	    case 16:
		*subtype = 21;		// Cubic Quasi Burning Ship 
		break;
	    case 17:
		*subtype = 25;		// Cubic Quasi Perpendicular
		break;
	    case 19:
		*subtype = 1;		// 4th Power Mandelbrot				degree is reset to 0
		param[2] = degree = 4;
		break;
	    case 20:
		*subtype = 11;		// 4th Power Mandelbar
		param[1] = degree = 4;
		break;
	    case 21:
		*subtype = 4;		// 4th Power Burning Ship
		break;
	    case 22:
		*subtype = 26;		// Burning Ship 4th Partial imag
		break;
	    case 23:
		*subtype = 27;		// Burning Ship 4th Partial Real
		break;
	    case 24:
		*subtype = 28;		// Burning Ship 4th Partial Real Mandelbar
		break;
	    case 26:
		*subtype = 29;		// Celtic Burning Ship 4th Partial imag
		break;
	    case 27:
		*subtype = 30;		// Celtic Burning Ship 4th Partial Real
		break;
	    case 28:
		*subtype = 31;		// Celtic Burning Ship 4th Partial Real Mandelbar
		break;
	    case 30:
		*subtype = 32;		// Buffalo 4th Partial imag
		break;
	    case 31:
		*subtype = 8;		// Celtic Buffalo 4th
		break;
	    case 32:
		*subtype = 33;		// Celtic 4th Mandelbar
		break;
	    case 33:
		*subtype = 34;		// 4th False Quasi Perpendicular 
		break;
	    case 34:
		*subtype = 35;		// 4th False Quasi Heart 
		break;
	    case 35:
		*subtype = 36;		// Celtic 4th False Quasi Perpendicular 
		break;
	    case 36:
		*subtype = 37;		// Celtic 4th False Quasi Heart 
		break;
	    case 37:
		*subtype = 38;		// Imag Quasi Perpendicular / Heart 4th 
		break;
	    case 38:
		*subtype = 39;		// 4th Quasi Perpendicular Real 
		break;
	    case 39:
		*subtype = 40;		// 4th Quasi Heart Real 
		break;
	    case 40:
		*subtype = 41;		// Celtic Imag Quasi Perpendicular / Heart 4th
		break;
	    case 41:
		*subtype = 42;		// Celtic 4th Quasi Perpendicular Real 
		break;
	    case 42:
		*subtype = 43;		// Celtic 4th Quasi Heart Real 
		break;
	    case 43:
		param[2] = degree = 5;
		*subtype = 1;		// Mandelbrot 4th 
		break;
	    case 44:
		*subtype = 11;		// 5th Mandelbar (vertical)
		param[1] = degree = 5;
		break;
	    case 46:
		*subtype = 5;		// 5th Power Burning Ship
		break;
	    case 48:
		*subtype = 44;		// Burning Ship 5th Partial
		break;
	    case 49:
		*subtype = 45;		// Burning Ship 5th Partial Mandelbar
		break;
	    case 50:
		*subtype = 9;		// Celtic 5th (Buffalo Partial)
		break;
	    case 51:
		*subtype = 46;		// Celtic 5th Mandelbar
		break;
	    case 52:
		*subtype = 47;		// Quazi Burning Ship 5th (BS/Buffalo Hybrid)
		break;
	    case 53:
		*subtype = 48;		// 5th Quasi Perpendicular
		break;
	    case 54:
		*subtype = 49;		// 5th Quasi Heart
		break;
	    case 55:
		if (param[0] == 4)
		    *subtype = 50;	// SimonBrot
		else
		    *subtype = 51;	// Cubic SimonBrot
		break;
	    case 56:
		*subtype = 52;		// SimonBrot2
		break;
	    case 58:
		*subtype = 58;		// HPDZ Buffalo
		break;
	    default:
		return -1;
	    }
	}
    else
	return -1;
    return 0;
    }

/**************************************************************************
    Show Perturbation Fractal
**************************************************************************/

DLGPROC FAR PASCAL SelectPertDlg(HWND hDlg, unsigned message, WPARAM wParam, LPARAM lParam)

    {
    static	int	i;
    static	int	index = 1;

    switch (message)
	{
	case WM_INITDIALOG:
	    SetDlgItemText(hDlg, ID_LISTTITLE, "Perturbation");
	    for (i = 0; i < PerturbationNum; i++)
		SendDlgItemMessage(hDlg, IDM_LSYSTEM, LB_ADDSTRING, (WPARAM)NULL, (LPARAM)(LPSTR)PerturbationSpecific[i].name);
	    SendDlgItemMessage(hDlg, IDM_LSYSTEM, LB_SETCURSEL, (WPARAM)PerturbationPtr, 0L);
	    return ((DLGPROC)TRUE);

	case WM_COMMAND:
	    switch ((int)LOWORD(wParam))
		{
		case IDOK:
		    index = (int)SendDlgItemMessage(hDlg, IDM_LSYSTEM, LB_GETCURSEL, 0, 0L);
		    if (index == LB_ERR)
			{
			MessageBox(hDlg, "No Choice selected",
			    "Select From a List", MB_OK | MB_ICONEXCLAMATION);
			break;
			}
		    PerturbationPtr = index;
		    subtype = PerturbationPtr;
		    EndDialog(hDlg, TRUE);
		    return ((DLGPROC)TRUE);

		case IDCANCEL:
		    PerturbationPtr = 1;
		    EndDialog(hDlg, FALSE);
		    return ((DLGPROC)FALSE);

		case IDM_LSYSTEM:
		    switch (HIWORD(wParam) & 0x0003)
			{
			case LBN_SELCHANGE:
			    index = (int)SendDlgItemMessage(hDlg, IDM_LSYSTEM, LB_GETCURSEL, 0, 0L);
			    if (index == LB_ERR)
				break;
			    break;

			case LBN_DBLCLK:
			    index = (int)SendDlgItemMessage(hDlg, IDM_LSYSTEM, LB_GETCURSEL, 0, 0L);
			    if (index == LB_ERR)
				{
				MessageBox(hDlg, "No Choice selected",
				    "Select From a List", MB_OK | MB_ICONEXCLAMATION);
				break;
				}
			    subtype = PerturbationPtr = index;
			    EndDialog(hDlg, TRUE);
			    return ((DLGPROC)TRUE);
			}
		    return ((DLGPROC)TRUE);
		}
	}
    return ((DLGPROC)FALSE);
    }

/**************************************************************************
    Setup Multi-threading parameters
**************************************************************************/

DLGPROC FAR PASCAL ThreadingDlg(HWND hDlg, unsigned message, WPARAM wParam, LPARAM lParam)

    {
    BOOL		bTrans;
    SYSTEM_INFO		sysinfo;
    HWND		hCtrl;

    switch (message)
	{
	case WM_INITDIALOG:
	    SetDlgItemInt(hDlg, IDC_NUMBERTHREADS, NumberThreads, TRUE);
	    SetDlgItemInt(hDlg, IDC_THREADCREATIONDELAY, ThreadCreationDelay, TRUE);
	    SetDlgItemInt(hDlg, IDC_THREADCOMPLETIONDELAY, ThreadCompletionDelay, TRUE);
	    SetDlgItemInt(hDlg, IDC_THREADENDINGDELAY, ThreadEndingDelay, TRUE);
	    SetDlgItemInt(hDlg, IDC_THREADPERTDELAY, ThreadPertDelay, TRUE);
	    GetSystemInfo(&sysinfo);
	    SetDlgItemInt(hDlg, IDC_NUMBERCORES, sysinfo.dwNumberOfProcessors, TRUE);
	    hCtrl = GetDlgItem(hDlg, IDC_USE_MUTEX);
	    SendMessage(hCtrl, BM_SETCHECK, UseMutex, 0L);
	    return ((DLGPROC)TRUE);

	case WM_COMMAND:
	    switch ((int)LOWORD(wParam))
		{
		case IDOK:
		    NumberThreads = GetDlgItemInt(hDlg, IDC_NUMBERTHREADS, &bTrans, TRUE);
		    hCtrl = GetDlgItem(hDlg, IDC_USE_MUTEX);
		    UseMutex = (BYTE)SendMessage(hCtrl, BM_GETCHECK, 0, 0L);
		    ThreadCreationDelay = GetDlgItemInt(hDlg, IDC_THREADCREATIONDELAY, &bTrans, TRUE);
		    ThreadCompletionDelay = GetDlgItemInt(hDlg, IDC_THREADCOMPLETIONDELAY, &bTrans, TRUE);
		    ThreadEndingDelay = GetDlgItemInt(hDlg, IDC_THREADENDINGDELAY, &bTrans, TRUE);
		    ThreadPertDelay = GetDlgItemInt(hDlg, IDC_THREADPERTDELAY, &bTrans, TRUE);
		    EndDialog(hDlg, TRUE);
		    return ((DLGPROC)TRUE);

		case IDCANCEL:
		    EndDialog(hDlg, FALSE);
		    return ((DLGPROC)FALSE);
		}
	}
    return ((DLGPROC)FALSE);
    }


/**************************************************************************
    Perturbation Fractal Parameters
**************************************************************************/

DLGPROC FAR PASCAL PertDlg(HWND hDlg, UINT message, UINT wParam, LONG lParam)

    {
    int i, j;
    char		s[10][100];
    char		Bailout[120];
    char		IterDivTxt[120];
    static     short	tempMethod;
    static     UINT	tempParam;
    static     UINT	tempStartPalette;
    static     UINT	tempSlopeAngle;
    static     UINT	tempSlopePower;
    static     UINT	tempSlopeRatio;
    static     double	tempIterDiv;
    static     UINT	tempPalOffset;
    BOOL		bTrans;
    HWND		hCtrl;

    switch (message)
	{
	case WM_INITDIALOG:
	    sprintf(Bailout, "%14.14lf", rqlim);
	    SetDlgItemText(hDlg, IDC_BAILOUT, Bailout);
	    SetDlgItemText(hDlg, ID_FRACNAME, GetFractalName());
	    SetDlgItemInt(hDlg, ID_STARTPALETTE, PaletteStart, TRUE);
	    SetDlgItemInt(hDlg, ID_SLOPEANGLE, (UINT)lightDirectionDegrees, TRUE);
	    SetDlgItemInt(hDlg, ID_SLOPERATIO, (UINT)bumpMappingStrength, TRUE);
	    SetDlgItemInt(hDlg, ID_SLOPEPOWER, (UINT)bumpMappingDepth, TRUE);
	    sprintf(IterDivTxt, "%14.14lf", IterDiv);
	    SetDlgItemText(hDlg, IDC_ITER_DIV, IterDivTxt);
	    SetDlgItemInt(hDlg, IDC_PAL_OFFSET, PalOffset, TRUE);

	    for (i = Fractal.NumFunct, j = 0; i < Fractal.NumFunct + Fractal.NumParam && i < 10; i++, j++)
		{
		sprintf(s[j], "%f", *Fractal.ParamValue[j]);
		SetDlgItemText(hDlg, ID_FRACPARTX1 + i, Fractal.ParamName[j]);
		SetDlgItemText(hDlg, ID_FRACPARAM1 + i, s[j]);
		}
	    for (i = /*Fractal.NumFunct + */Fractal.NumParam; i < 10; i++)
		SetDlgItemText(hDlg, ID_FRACPARTX1 + i, "     N/A");
	    SetDlgItemInt(hDlg, ID_SLOPETYPE, (UINT)SlopeType, TRUE);

	    for(int i = 4; i < 7; i++)
		{
		hCtrl = GetDlgItem(hDlg, IDC_COLOUR0 + i);
		ShowWindow(hCtrl, SW_HIDE);
		}
	    tempMethod = PertColourMethod;
	    tempParam = IDC_COLOUR0 + tempMethod;
	    CheckRadioButton(hDlg, IDC_COLOUR0, IDC_COLOUR6, tempParam);
	    return ((DLGPROC)TRUE);

	case WM_COMMAND:
	    switch ((int)LOWORD(wParam))
		//	    switch (wParam)
		{
		case IDC_COLOUR0:
		case IDC_COLOUR1:
		case IDC_COLOUR2:
		case IDC_COLOUR3:
/* future???
		case IDC_COLOUR4:
		case IDC_COLOUR5:
		case IDC_COLOUR6:
		case IDC_COLOUR7:
*/
		    tempMethod = (int)LOWORD(wParam) - IDC_COLOUR0;
		    CheckRadioButton(hDlg, IDC_COLOUR0, IDC_COLOUR3, (int)LOWORD(wParam));
		    return (DLGPROC)TRUE;

		case ID_STARTPALETTE:
		    tempStartPalette = GetDlgItemInt(hDlg, ID_STARTPALETTE, &bTrans, TRUE);
		    return (DLGPROC)TRUE;

		case ID_SLOPEANGLE:
		    tempSlopeAngle = GetDlgItemInt(hDlg, ID_SLOPEANGLE, &bTrans, TRUE);
		    return (DLGPROC)TRUE;

		case ID_SLOPERATIO:
		    tempSlopeRatio = GetDlgItemInt(hDlg, ID_SLOPERATIO, &bTrans, TRUE);
		    return (DLGPROC)TRUE;

		case ID_SLOPEPOWER:
		    tempSlopePower = GetDlgItemInt(hDlg, ID_SLOPEPOWER, &bTrans, TRUE);
		    return (DLGPROC)TRUE;

		case IDC_ITER_DIV:
		    GetDlgItemText(hDlg, IDC_ITER_DIV, IterDivTxt, 100);
		    tempIterDiv = atof(IterDivTxt);
		    return (DLGPROC)TRUE;

		case IDC_PAL_OFFSET:
		    tempPalOffset = GetDlgItemInt(hDlg, IDC_PAL_OFFSET, &bTrans, TRUE);
		    return (DLGPROC)TRUE;

		case IDOK:
		    GetDlgItemText(hDlg, IDC_BAILOUT, Bailout, 100);
		    rqlim = atof(Bailout);
		    PertColourMethod = tempMethod;
		    PaletteStart = tempStartPalette;
		    lightDirectionDegrees = tempSlopeAngle;
		    bumpMappingDepth = tempSlopePower;
		    bumpMappingStrength = tempSlopeRatio;
		    IterDiv = tempIterDiv;
		    PalOffset = tempPalOffset;

		    SlopeType = GetDlgItemInt(hDlg, ID_SLOPETYPE, &bTrans, TRUE);
		    for (i = Fractal.NumFunct, j = 0; i < Fractal.NumFunct + Fractal.NumParam && i < 10; i++, j++)
			{
			GetDlgItemText(hDlg, ID_FRACPARAM1 + i, s[j], 100);
			*Fractal.ParamValue[j] = atof(s[j]);
			}
		    PaletteStart = GetDlgItemInt(hDlg, ID_STARTPALETTE, &bTrans, TRUE);
		    EndDialog(hDlg, TRUE);
		    return ((DLGPROC)TRUE);

		case IDCANCEL:
		    EndDialog(hDlg, FALSE);
		    return (FALSE);
		}
	}
    return (FALSE);
    }

