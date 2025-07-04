/****************************************************
    SLOPEMT.CPP a module to explore Slope fractals
    Written in Microsoft Visual C++ by Paul de Leeuw.
****************************************************/

#include <process.h>
#include <strsafe.h>
#include "slope.h"
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

#define		MAXTHREADS	40

extern	int	user_data(HWND);
extern	void	DisplayStatusBarInfo(int, char *);
extern	void	OutputStatusBar(HWND);
extern	void	RefreshScreen(void);

extern	void	ErrorHandler(LPTSTR lpszFunction);

/**************************************************************************
	Additional globals for slope fractal
**************************************************************************/

extern	double	mandel_width;		// final width
extern	double	hor;			/* horizontal address */
extern	double	vert;			/* vertical address */

extern	double	xgap;			// gap between pixels 
extern	double	ygap;			// gap between pixels 
extern	BYTE	BigNumFlag;		// True if bignum used
extern	BOOL	RunAnimation;			// are we in the middle of an animation generation run?
extern	bool	UseMutex;			// trade off speed for accuracy

extern	BigDouble   Big_xgap, Big_ygap;
extern	BigDouble   BigHor, BigVert, BigWidth;

extern  CDib	    Dib;
extern	CTrueCol    TrueCol;		// palette info
extern	HWND	    GlobalHwnd;		// This is the main windows handle
extern	double	rqlim;
extern	long	threshold;
extern	double	param[];
extern	WORD	degree;			// power
extern	int	curpass, totpasses;
extern	WORD	type;
extern	int	subtype;
extern	int	bits_per_pixel, width, height;
extern	BOOL	ShowOrbits;		// show julia orbits for each image
//extern	WORD	OrbitColour;		// Indexed colour for the orbit displays in Julia sets
extern	int	xdots, ydots;
extern	int	precision;
extern	CFract	Fractal;		// current fractal stuff

extern	    Complex j;
extern	    BYTE	juliaflag;

CSlope	Slope[MAXTHREADS];
//CSlope	Slope;

int	SlopeProgress[MAXTHREADS];
bool	SlopeThreadComplete[MAXTHREADS];
char	SlopeStatus[200];
char	SlopeErrorMessage[200];

// stuff for DwdDiff algorithm
double	bump_transfer_factor = 1.0;
int	PaletteStart = 0;
double	lightDirectionDegrees = 45.0;
double	bumpMappingDepth = 50.0;
double	bumpMappingStrength = 75.0;
double	LightHeight = 2.0;				// height of light for slope calculation


//static	int	PerturbationPtr = 0, PerturbationNum = 0;
extern	int	NumberThreads;			// set the default to 0. Multi=threading is for experimentation only
static	HANDLE	hThread[MAXTHREADS];

extern	int	ThreadCreationDelay;		// not sure if these are required
extern	int	ThreadCompletionDelay;
extern	int	ThreadEndingDelay;
extern	int	ThreadPertDelay;		// if > 0, show pixel update in real time. Otherwise stay with screen update every second (faster)

extern	double 	*wpixels;			// an array of doubles holding slope modified iteration counts
	int	PaletteShift = 0;		// fractional palette addressing

typedef	struct MySlopeData
    {
    CTrueCol *TrueCol;				// colouring scheme
    int		NumThreads;
    int		i;
    HANDLE	ghMutex;
    PMYVARIABLES;
    } MYSLOPEDATA, *PMYSLOPEDATA;

PMYSLOPEDATA	    pSlopeDataArray[MAXTHREADS];
MYSLOPEDATA	    SlopeDataArrayZero;

/**************************************************************************
	Slope engine, pass into thread creation routine
**************************************************************************/

DWORD	WINAPI SlopeFunction(LPVOID lpParam)
    {
    PMYSLOPEDATA pSlopeDataArray;
    int	    ReturnValue;

    pSlopeDataArray = (PMYSLOPEDATA)lpParam;
    int(*UserData)(HWND) = user_data;

    if (type == SLOPEDERIVATIVE)
	ReturnValue = Slope[pSlopeDataArray->i].RunSlopeDerivative(GlobalHwnd, UserData, SlopeStatus, &SlopeThreadComplete[pSlopeDataArray->i], subtype, pSlopeDataArray->NumThreads, pSlopeDataArray->i, j, mandel_width, 
		hor, vert, xgap, ygap, BigNumFlag, Big_xgap, Big_ygap, BigHor, BigVert, BigWidth, rqlim, threshold, param, &TrueCol, &Dib, bits_per_pixel, juliaflag, xdots, ydots, width, height, &degree, precision, 
		pSlopeDataArray->ghMutex);
    else
	ReturnValue = Slope[pSlopeDataArray->i].RunSlopeFwdDiff(GlobalHwnd, UserData, SlopeStatus, &SlopeThreadComplete[pSlopeDataArray->i], subtype, pSlopeDataArray->NumThreads, pSlopeDataArray->i, j, mandel_width, 
		hor, vert, xgap, ygap, BigNumFlag, Big_xgap, Big_ygap, BigHor, BigVert, BigWidth, rqlim, threshold, param, &TrueCol, &Dib, wpixels, juliaflag, xdots, ydots, width, &degree, precision);
    if (ReturnValue < 0)
	{
	if (ReturnValue == -2)			// actual error, else user input
	    ErrorHandler(SlopeErrorMessage);
	Sleep(50);				// give the threads time to close
	return -1;
	}
    return 0;
    }

/**************************************************************************
	Initialise Slope engine
**************************************************************************/

int	InitSlopeFractal(PMYVARIABLES)
    {
    int		i;
    static	bool	FirstTime = true;

    if (NumberThreads > MAXTHREADS)
	NumberThreads = MAXTHREADS;

    if (FirstTime)
	{
	for (i = 0; i < NumberThreads; i++)
	    pSlopeDataArray[i] = NULL;		// Ensure address is not reset before being allocated.
	FirstTime = false;
	}

    for (i = 0; i < NumberThreads; i++)
	Slope[i].EndSlope = false;		// We're not exiting yet
    return 0;
    }

/**************************************************************************
	The Perturbation engine
**************************************************************************/

int	DoSlopeFractal()
    {
    int		(*UserData)(HWND) = user_data;
    int		i;
//    DWORD	ExitCode;
    DWORD	dwThreadId[MAXTHREADS];
    int		flag, status;
    HANDLE	ghMutex = NULL;

    if (UseMutex)										// stuff speed, we want a good image
	ghMutex = CreateMutex(NULL, 0, NULL);
    for (int i = 0; i < NumberThreads; i++)
	{
	if (pSlopeDataArray[i] != NULL)
	    {
	    HeapFree(GetProcessHeap(), 0, pSlopeDataArray[i]);					// we free memory here because we don't know when all the threads are finished
	    pSlopeDataArray[i] = NULL;								// flag it is available for reuse
	    }
	}

    if (NumberThreads <= 0)									// no multi-threading
	{
	if (type == SLOPEDERIVATIVE)
	    status = Slope[0].RunSlopeDerivative(GlobalHwnd, UserData, SlopeStatus, &SlopeThreadComplete[0], subtype, 0, 0, j, mandel_width, hor, vert, xgap, ygap, BigNumFlag, Big_xgap, Big_ygap, BigHor, BigVert, BigWidth, rqlim, 
			threshold, param, &TrueCol, &Dib, bits_per_pixel, juliaflag, xdots, ydots, width, height, &degree, precision, ghMutex);
	else 
	    status = Slope[0].RunSlopeFwdDiff(GlobalHwnd, UserData, SlopeStatus, &SlopeThreadComplete[0], subtype, 0, 0, j, mandel_width, hor, vert, xgap, ygap, BigNumFlag, Big_xgap, Big_ygap, BigHor, BigVert, BigWidth, 
			rqlim, threshold, param, &TrueCol, &Dib, wpixels, juliaflag, xdots, ydots, width, &degree, precision);
	if (status < 0)
	    {
	    return -1;
	    }
	}
    else
	{
	for (i = 0; i < NumberThreads; i++)
	    {
	    pSlopeDataArray[i] = (PMYSLOPEDATA)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(MYSLOPEDATA));

	    if (pSlopeDataArray[i] == NULL)
		{
		// don't start this thread, just move on
		sprintf(SlopeErrorMessage, "Can't find memory for data array in thread %d", i);
		ErrorHandler(SlopeErrorMessage);
		break;
		}


	    pSlopeDataArray[i]->NumThreads = NumberThreads;
	    pSlopeDataArray[i]->i = i;
	    pSlopeDataArray[i]->TrueCol = &TrueCol;
	    pSlopeDataArray[i]->ghMutex = ghMutex;

	    hThread[i] = CreateThread(
		NULL,						// default security attributes
		0L,						// use default stack size = 0
		(LPTHREAD_START_ROUTINE)SlopeFunction,		// thread function 
		pSlopeDataArray[i],					// argument to thread function 
		0,						// use default creation flags 
		&dwThreadId[i]);				// returns the thread identifier 
	    if (hThread[i] == NULL)
		{
		sprintf(SlopeErrorMessage, "CreateThread failed in thread %d", i);
		ErrorHandler(SlopeErrorMessage);
		break;
		}
	    SetThreadPriority(hThread[i], THREAD_PRIORITY_LOWEST);
	    Sleep(ThreadCreationDelay);
	    CloseHandle(hThread[i]);
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
		if (curpass > SlopeProgress[i])
		    curpass = SlopeProgress[i];			// display the slowest thread
		if (SlopeThreadComplete[i] == false)
		    flag = 0;
		}
	    if (!RunAnimation)
		DisplayStatusBarInfo(INCOMPLETE, "");
	    Sleep(ThreadCompletionDelay);			// give the threads time to close
	    }
	Sleep(ThreadEndingDelay);				// make sure all threads are closed before leaving
	}
    if (type == SLOPEFORWARDDIFF)				// render the slope image from the stored forward differences
	{
	// stuff for DwdDiff algorithm
	bump_transfer_factor = param[0];
	PaletteStart = (int)param[1];
	lightDirectionDegrees = param[2];
	bumpMappingDepth = param[3];
	bumpMappingStrength = param[4];
//	PaletteShift = (int)param[5];
	Slope[0].InitRender(threshold, &TrueCol, &Dib, wpixels, PaletteShift, bump_transfer_factor, PaletteStart, lightDirectionDegrees, bumpMappingDepth, bumpMappingStrength);
	Slope[0].RenderSlope(xdots, ydots, 0/*PertColourMethod*/, 0/*PalOffset*/, 1.0/*IterDiv*/);	// fix later
	}
    if (ghMutex != NULL)
	{
	CloseHandle(ghMutex);
	ghMutex = NULL;
	}
    return 0;
    }

/**************************************************************************
    End Threads
**************************************************************************/

int	EndSlope(void)
    {
    int	    i;

    if (NumberThreads == 0)
	return 0;

    for (i = 0; i < NumberThreads; i++)
	Slope[0].SlopeIsExiting();
    return 0;
    }

/**************************************************************************
    Some simple processing
**************************************************************************/

int	setup_SlopeDeriv(void)
    {
    if (!Fractal.SlopeDerivNum)	    // we'd better count how many records we have
	{
	while (SlopeDerivSpecific[Fractal.SlopeDerivNum].name)
	    (Fractal.SlopeDerivNum)++;
	}
    return 0;
    }

int	setup_SlopeFwdDiff(void)
    {
    if (!Fractal.SlopeFwdDiffNum)	    // we'd better count how many records we have
	{
	while (SlopeFwdDiffSpecific[Fractal.SlopeFwdDiffNum].name)
	    (Fractal.SlopeFwdDiffNum)++;
	}
    return 0;
    }

void	LoadSlopeDerivParams()
    {
    // here is where we can do some specific updates to individual fractals
    int	i;

    Fractal.NumParam = SlopeDerivSpecific[subtype].numparams;
    Fractal.ParamName[0] = "Light Source Angle (degrees)";
    Fractal.ParamName[1] = "Light Source Height";
    Fractal.ParamName[2] = "Start Palette 0 - Max Iterations";

    switch (subtype)
	{
	case 3:
	    Fractal.ParamName[3] = "0=multiply, else=plus";
	    Fractal.ParamName[4] = "Start value for z";
	    break;
	case 2:
	case 6:
	case 10:
	case 12:
	    Fractal.ParamName[3] = "Polynomial Degree (>= 0)";
	    break;
	case 7:
	    Fractal.ParamName[3] = "First Polynomial Degree (>= 0)";
	    Fractal.ParamName[4] = "Second Polynomial Degree (>= 0)";
	    Fractal.ParamName[5] = "Third Polynomial Degree (>= 0)";
	    break;
	}

    for (i = 0; i < SlopeDerivSpecific[subtype].numparams; i++)
	param[i] = SlopeDerivSpecific[subtype].paramvalue[i];
    rqlim = SlopeDerivSpecific[subtype].rqlim;
    }

void	LoadSlopeFwdDiffParams()
    {
    // here is where we can do some specific updates to individual fractals
    int	i;

    Fractal.NumParam = SlopeFwdDiffSpecific[subtype].numparams;
    Fractal.ParamName[0] = "Transfer Factor";
    Fractal.ParamName[1] = "Start Palette 0 - Max Iterations";
    Fractal.ParamName[2] = "Light Direction (Degrees)";
    Fractal.ParamName[3] = "Mapping Depth";
    Fractal.ParamName[4] = "Mapping Strength";
    Fractal.ParamName[5] = "Palette Shift";
    //    PaletteStart = 0;

    switch (subtype)
	{
	case 4:
	    Fractal.ParamName[5] = "0=multiply, else=plus";
	    Fractal.ParamName[6] = "Start value for z";
	    break;
	case 3:
	case 7:
	case 11:
	case 13:
	    Fractal.ParamName[5] = "Polynomial Degree (>= 0)";
	    break;
	case 8:
	    Fractal.ParamName[5] = "First Polynomial Degree (>= 0)";
	    Fractal.ParamName[6] = "Second Polynomial Degree (>= 0)";
	    Fractal.ParamName[7] = "Third Polynomial Degree (>= 0)";
	    break;
	case 15:
	    Fractal.ParamName[5] = "Special Colour";
	    Fractal.ParamName[6] = "Subtype: 0=CBIN, 1=CCIN, 2=CFIN, 3=CKIN";
	    break;
	case 16:
	    Fractal.ParamName[5] = "Polynomial Degree (>= 2)";
	    break;
	}

    for (i = 0; i < NUMSLOPEDERIVPARAM; i++)
	param[i] = SlopeFwdDiffSpecific[subtype].paramvalue[i];
    rqlim = SlopeFwdDiffSpecific[subtype].rqlim;
    }

/**************************************************************************
    Show Slope Fractals
**************************************************************************/

DLGPROC FAR PASCAL SelectSlopeDerivDlg(HWND hDlg, unsigned message, WPARAM wParam, LPARAM lParam)

    {
    static	int	i;
    static	int	index = 1;

    switch (message)
	{
	case WM_INITDIALOG:
	    SetDlgItemText(hDlg, ID_LISTTITLE, "Slope Using Derivative");
	    for (i = 0; i < Fractal.SlopeDerivNum; i++)
		SendDlgItemMessage(hDlg, IDM_LSYSTEM, LB_ADDSTRING, (WPARAM)NULL, (LPARAM)(LPSTR)SlopeDerivSpecific[i].name);
	    SendDlgItemMessage(hDlg, IDM_LSYSTEM, LB_SETCURSEL, (WPARAM)Fractal.SlopeDerivPtr, 0L);
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
		    Fractal.SlopeDerivPtr = index;
		    subtype = Fractal.SlopeDerivPtr;
		    EndDialog(hDlg, TRUE);
		    return ((DLGPROC)TRUE);

		case IDCANCEL:
		    Fractal.SlopeDerivPtr = 0;
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
			    subtype = Fractal.SlopeDerivPtr = index;
			    EndDialog(hDlg, TRUE);
			    return ((DLGPROC)TRUE);
			}
		    return ((DLGPROC)TRUE);
		}
	}
    return ((DLGPROC)FALSE);
    }

DLGPROC FAR PASCAL SelectSlopeFwdDiffDlg(HWND hDlg, unsigned message, WPARAM wParam, LPARAM lParam)

    {
    static	int	i;
    static	int	index = 1;

    switch (message)
	{
	case WM_INITDIALOG:
	    SetDlgItemText(hDlg, ID_LISTTITLE, "Slope Using Forward Differences");
	    for (i = 0; i < Fractal.SlopeFwdDiffNum; i++)
		SendDlgItemMessage(hDlg, IDM_LSYSTEM, LB_ADDSTRING, (WPARAM)NULL, (LPARAM)(LPSTR)SlopeFwdDiffSpecific[i].name);
	    SendDlgItemMessage(hDlg, IDM_LSYSTEM, LB_SETCURSEL, (WPARAM)Fractal.SlopeFwdDiffPtr, 0L);
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
		    Fractal.SlopeFwdDiffPtr = index;
		    subtype = Fractal.SlopeFwdDiffPtr;
		    EndDialog(hDlg, TRUE);
		    return ((DLGPROC)TRUE);

		case IDCANCEL:
		    Fractal.SlopeFwdDiffPtr = 0;
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
			    subtype = Fractal.SlopeFwdDiffPtr = index;
			    EndDialog(hDlg, TRUE);
			    return ((DLGPROC)TRUE);
			}
		    return ((DLGPROC)TRUE);
		}
	}
    return ((DLGPROC)FALSE);
    }


