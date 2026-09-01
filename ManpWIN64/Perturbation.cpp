/*
    PERTURBATION.CPP - a module to explore Perturbation

    Thanks to Shirom Makkad fractaltodesktop@gmail.com

    Written in Microsoft Visual C++ by Paul de Leeuw.
*/

#include <process.h>
#include <strsafe.h>
#include <cassert>
#include <atomic>
#include <vector>
#include "PertEngine.h"
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
#include "pixel.h"
#include "plotmode.h"
#include "Timer.h"
#include "SafeStrings.h"

#define		MAXTHREADS	40

extern	int	user_data(HWND);
extern	int	ReferenceZoomPoint(BigComplex& centre, int maxIteration, int user_data(HWND hwnd), char* StatusBarInfo, int *pPertProgress, double bailout, int ArithType, int power, BigDouble BigWidth, int &SlopeDegree, int subtype);
extern	void	PertSetupArithType(int &ArithType, int subtype, long MaxIteration, int precision, BYTE BigNumFlag);
extern	void	ShowBignum(BigDouble x, char *Location);
extern	void	SetPertSlopeDegree(void);

extern	int	decimals;

std::atomic<long> gPixelsDone = 0;

static	int	PerturbationPtr = 0, PerturbationNum = 0;

std::atomic<bool> gStopRequested{ false };

/**************************************************************************
    Check validity of reference/BLA parameters
**************************************************************************/

bool	CheckValidRef(BigComplex ReferenceCoordinate, BigDouble BigWidth, int maxIteration, double bailout, StoreReferenceData &RefData, int power, int ArithType)
    {
    int		refSize = (ArithType == DOUBLE || ArithType == DBL_UNSUPPORTED) ? (int)gManp->XSubN.size() : (int)gManp->ExpXSubN.size();
    if (!RefData.valid)
	return false;

    if (RefData.degree != power)
	return false;

    if (RefData.rqlim != bailout)
	return false;

    if (!(RefData.ReferenceCoordinate == ReferenceCoordinate))
	return false;

    if (!(RefData.BigWidth == BigWidth))
	return false;

    if (refSize < (size_t)(maxIteration + 1))
	return false;
    return true;
    }

/**************************************************************************
    Run 3D projection from wpixels after fully created
**************************************************************************/

// Globals used: _3dflag, wpixels, Dib, Pixel[0],
//               x_rot, y_rot, z_rot, sclx, scly, sclz,
//               threshold, hor, vert, GlobalHwnd

void Run3DAfterRender()
    {
    if (!gManp->_3dflag)
	return;
    if (gManp->wpixels.empty())
	return;

    const int xdots = gManp->Dib.DibWidth;
    const int ydots = gManp->Dib.DibHeight;
    const size_t totalPixels = (size_t)xdots * (size_t)ydots;

    if (gManp->wpixels.size() < totalPixels)
	return;     // no complete source buffer: bail safely

    assert(gManp->wpixels.size() >= totalPixels);

    std::vector<float> wpixels3d(totalPixels);

    for (int y = 0; y < ydots; ++y)
	{
	for (int x = 0; x < xdots; ++x)
	    {
	    size_t src = (size_t)(ydots - 1 - y) * xdots + x;
	    size_t dst = (size_t)y * xdots + x;

	    wpixels3d[dst] = gManp->wpixels[src];
	    }
	}

    // 1) Initialise 3D transform exactly as legacy did
    gManp->Pixel[0]->init3d(xdots, ydots,
			    gManp->x_rot, gManp->y_rot, gManp->z_rot,
			    gManp->sclx, gManp->scly, gManp->sclz,
			    gManp->threshold, gManp->hor, gManp->vert);

    // 2) Clear the destination bitmap
    gManp->Dib.ClearDib(0L);

    // 3) Project each pixel - Force projection in strict scanline order
    for (int y = 0; y < ydots; ++y)
	{
	// RESET row linkage
	for (int x = 0; x < xdots; ++x)
	    {
	    size_t idx = (size_t)(ydots - 1 - y) * xdots + x;

	    long colour = (long)wpixels3d[idx];
	    if (colour <= 0)
		continue;

	    long zVal = colour;		// keep simple for step A
	    gManp->Pixel[0]->projection(x, y, zVal);
	    }
	}

    // 5) Show the result
    InvalidateRect(gManp->GlobalHwnd, nullptr, FALSE);
    UpdateWindow(gManp->GlobalHwnd);
    }

/**************************************************************************
	One-call hard stop used by all handlers that invalidate orbit/buffers.
**************************************************************************/

void HardStopNow(HWND hwnd, const char* reason) 
    {
#if defined(_DEBUG)
    OutputDebugStringA(("[HardStop] " + std::string(reason) + "\n").c_str());
#endif
    //    PauseUiTimers(hwnd);
    //    RequestRenderStop();
    //    JoinRenderThreads();
    gStopRequested.store(true, std::memory_order_relaxed);
/*
    if (gManp)
	{
	gManp->time_to_break = TRUE;
	gManp->time_to_restart = TRUE;
	gManp->time_to_reinit = TRUE;
	}
*/
    }

/**************************************************************************
	Perturbation engine, pass into thread creation routine
**************************************************************************/

DWORD	WINAPI PertFunction(LPVOID lpParam)
    {
    pPertThreadData p = (pPertThreadData)lpParam;
    int	    ThreadNum = p->i;
    int(*UserData)(HWND) = user_data;
    int	    ret = 0;

    ret = gManp->PertCalculator[ThreadNum]->calculateOneFrame(gManp->rqlim, gManp->PertStatus, gManp->degree, gManp->InsideMethod, gManp->OutsideMethod, gManp->biomorph, gManp->subtype, p->RSRA, p->IsPositive, UserData, gManp->xdots, //*(p->TZfilter),
	*(p->TrueCol), p->pPertProgress, (gManp->NumberThreads > 0), gManp->ThreadPertDelay, gManp->PertErrorMessage, p->ArithType, p->MaxRefIteration, p->SlopeDegree, p->mode,
	p->pixelOrder, p->workIndex, p->totalPixels, p->ghMutex);
    return ret;
    }

/**************************************************************************
	Initialise Perturbation engine
**************************************************************************/

int InitPerturbation(void)
    {
    BigDouble BigCentreX, BigCentreY;
    int i;

    gPixelsDone = 0;

    PertSetupArithType(gManp->ArithType, gManp->subtype, gManp->threshold, gManp->precision, gManp->BigNumFlag);
    std::fill(gManp->wpixels.begin(), gManp->wpixels.end(), -1.0f);

    if (!gManp->BigNumFlag)
	{
	gManp->BigHor = gManp->hor;
	gManp->BigVert = gManp->vert;
	gManp->BigWidth = gManp->mandel_width;
	}

    // --- Thread count ---
    int threadCount = gManp->NumberThreads;
    if (threadCount < 1) threadCount = 1;
    if (threadCount > MAXTHREADS) threadCount = MAXTHREADS;

    // --- Create Perturbation objects ---
    gManp->PertCalculator.clear();
    gManp->PertCalculator.reserve(threadCount);

    for (i = 0; i < threadCount; i++)
	{
	gManp->PertCalculator.emplace_back(std::make_unique<CPerturbation>());
	}

    // --- Thread arrays (clean + consistent) ---
    gManp->PertProgress.assign(threadCount, 0);

    gManp->hThread.assign(threadCount, nullptr);
    gManp->pDataArray.assign(threadCount, nullptr);

    BigCentreX = gManp->BigHor + (gManp->BigWidth * ((double)gManp->Dib.DibWidth / (double)(2 * gManp->Dib.DibHeight)));
    BigCentreY = -(gManp->BigVert + (gManp->BigWidth / 2.0));

    // initialise degree for those subtypes that need it
    switch (gManp->subtype)
	{
	case 1:
	    gManp->degree = (int)gManp->param[10];
	    break;
	case 11:
	    gManp->degree = (int)gManp->param[9];
	    break;
	case 53:
	    gManp->degree = (int)gManp->param[11];
	    break;
	case 13:
	    gManp->degree = 3;
	    break;
	case 14:
	    gManp->degree = 4;
	    break;
	case 15:
	    gManp->degree = 5;
	    break;
	}

    // --- Per-thread setup ---
    for (i = 0; i < threadCount; i++)
	{
	int baseWidth = gManp->xdots / threadCount;
	int rem = gManp->xdots % threadCount;
	auto& pert = gManp->PertCalculator[i];

	pert->AttachSharedTables(&gManp->XSubN, &gManp->ExpXSubN, &gManp->Bla);

	pert->initialiseCalculateFrame(&gManp->Dib, (int)gManp->Dib.DibHeight, gManp->threshold, BigCentreX, BigCentreY, gManp->BigWidth, decimals, gManp->OutsideMethod, gManp->InsideMethod, gManp->GlobalHwnd, i, 
		gManp->param, gManp->potparam, gManp->PaletteShift, &gManp->PlotType, gManp->SlopeType, gManp->lightDirectionDegrees, gManp->bumpMappingDepth, gManp->bumpMappingStrength, gManp->PaletteStart, 
		gManp->LightHeight, gManp->PertColourMethod, gManp->PalOffset, gManp->IterDiv, gManp->EnableApproximation, gManp->_3dflag, gManp->ColourSpeed, threadCount);
	}

    // --- Reference build ---
    BigComplex ReferenceCoordinate;
    ReferenceCoordinate.x = BigCentreX;
    ReferenceCoordinate.y = BigCentreY;

    gManp->CurrentRenderMode = NOMULTITHREAD;

    if (!CheckValidRef(ReferenceCoordinate, gManp->BigWidth, gManp->threshold, gManp->rqlim, *gManp->RefData, gManp->degree, gManp->ArithType))
	{
	gManp->RefData->valid = false;

	if (ReferenceZoomPoint(ReferenceCoordinate, gManp->threshold, user_data, gManp->PertStatus,
	    gManp->PertProgress.data(), gManp->rqlim, gManp->ArithType, gManp->degree, gManp->BigWidth,
	    gManp->SlopeDegree, gManp->subtype) < 0)
	    {
	    SAFE_SPRINTF(gManp->PertErrorMessage, "User Activity at ReferenceZoomPoint()");
	    gManp->XSubN.clear();
	    gManp->ExpXSubN.clear();
	    if (gManp->EnableApproximation)
		gManp->Bla.clear();
	    return -1;
	    }

	gManp->RefData->ReferenceCoordinate = ReferenceCoordinate;
	gManp->RefData->BigWidth = gManp->BigWidth;
	gManp->RefData->degree = gManp->degree;
	gManp->RefData->rqlim = gManp->rqlim;
	gManp->RefData->valid = true;
	}
    else
	{
	gManp->RefData->valid = true;
	}

    gManp->DumpStartupState("after ReferenceZoomPoint");
    gStopRequested.store(false, std::memory_order_relaxed);
    gManp->CurrentRenderMode = RENDER_PERT;
    SetPertSlopeDegree();
    return 0;
    }

/**************************************************************************
	The Perturbation engine
**************************************************************************/

int	DoPerturbation()
    {
    int		(*UserData)(HWND) = user_data;
    int		i;
    DWORD	dwThreadId[MAXTHREADS];
    int		threadsStarted = 0;
    int		flag;//, status;
    HANDLE	ghMutex = NULL;									// manage access to shared resources such as Dib and wpixels
//    char	buf[256];

    if (gManp->UseMutex)									// stuff speed, we want a good image
	ghMutex = CreateMutex(NULL, 0, NULL);

    std::vector<std::pair<int, int>> pixelOrder;
    std::atomic<int> workIndex(0);
    int totalPixels = 0;

    PlotMode mode = static_cast<PlotMode>(currentMode);

    BuildPixelOrder(pixelOrder, totalPixels, gManp->xdots, gManp->ydots, mode);
    workIndex.store(0);

    for (int i = 0; i < gManp->NumberThreads; i++)
	{
	if (gManp->pDataArray[i] != NULL)
	    {
	    HeapFree(GetProcessHeap(), 0, gManp->pDataArray[i]);				// we free memory here because we don't know when all the threads are finished
	    gManp->pDataArray[i] = NULL;							// flag it is available for reuse
	    }
	}

    SimpleTimer tPertRender;
    tPertRender.start();

    gManp->CurrentRenderMode = RENDER_PERT;

    for (i = 0; i < gManp->NumberThreads; i++)
	{
	gManp->pDataArray[i] = (pPertThreadData)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(PertThreadData));

	if (gManp->pDataArray[i] == NULL)
	    {
	    // don't start this thread, just move on
	    SAFE_SPRINTF(gManp->PertErrorMessage, "Can't find memory for data array in thread %d", i);
	    gManp->ErrorHandler(gManp->PertErrorMessage);
	    break;
	    }

	if (gManp->subtype == 53)	// RedShiftRider
	    {
	    gManp->pDataArray[i]->RSRA.x = gManp->param[9];
	    gManp->pDataArray[i]->RSRA.y = gManp->param[10];
	    gManp->degree = (int)gManp->param[11];
	    gManp->pDataArray[i]->IsPositive = (gManp->param[12] == 1.0);
	    }

	gManp->pDataArray[i]->i = i;
	gManp->pDataArray[i]->pPertProgress = &gManp->PertProgress[i];
	gManp->pDataArray[i]->TrueCol = &gManp->TrueCol;
	gManp->pDataArray[i]->ghMutex = ghMutex;
	gManp->pDataArray[i]->ArithType = gManp->ArithType;
	gManp->pDataArray[i]->MaxRefIteration = gManp->MaxRefIteration;
	gManp->pDataArray[i]->SlopeDegree = gManp->SlopeDegree;
	gManp->pDataArray[i]->pixelOrder = &pixelOrder;
	gManp->pDataArray[i]->workIndex = &workIndex;
	gManp->pDataArray[i]->totalPixels = totalPixels;
	gManp->pDataArray[i]->mode = mode;
#ifdef _DEBUG
	char buf[80];
	sprintf_s(buf, "Pert thread %d started\n", i);
	OutputDebugStringA(buf); 
#endif


	gManp->hThread[i] = CreateThread(
	    NULL,					// default security attributes
	    0L,						// use default stack size = 0
	    (LPTHREAD_START_ROUTINE)PertFunction,	// thread function 
	    gManp->pDataArray[i],				// argument to thread function 
	    0,						// use default creation flags 
	    &dwThreadId[i]);				// returns the thread identifier 
	if (gManp->hThread[i] == NULL)
	    {
	    SAFE_SPRINTF(gManp->PertErrorMessage, "CreateThread failed in thread %d", i);
	    gManp->ErrorHandler(gManp->PertErrorMessage);
	    break;
	    }
	threadsStarted++;
	SetThreadPriority(gManp->hThread[i], THREAD_PRIORITY_LOWEST);
//	CloseHandle(hThread[i]);
	Sleep(gManp->ThreadCreationDelay);
	}

    flag = 0;						// make sure all threads are finished

    gManp->totpasses = 100;
    while (flag == 0)
	{
	if (user_data(gManp->GlobalHwnd) == -1)		// user pressed a key?
	    {
	    gStopRequested.store(true, std::memory_order_relaxed);
#ifdef _DEBUG
	    OutputDebugStringA("[AbortRequested] keypress detected\n");
#endif
	    }
	flag = 1;
	gManp->curpass = 100;
	for (i = 0; i < threadsStarted; i++)
	    {
	    if (gManp->curpass > gManp->PertProgress[i])
		gManp->curpass = gManp->PertProgress[i];	// display the slowest thread

	    if (gManp->hThread[i] != NULL &&
		WaitForSingleObject(gManp->hThread[i], 0) == WAIT_TIMEOUT)
		flag = 0;
	    }
	if (!gManp->RunAnimation)
	    gManp->DisplayStatusBarInfo(INCOMPLETE, "");
	Sleep(gManp->ThreadCompletionDelay);			// polling delay
	}

    //  NOW we can safely close handles
    for (i = 0; i < threadsStarted; i++)
	{
	if (gManp->hThread[i] != NULL)
	    {
	    CloseHandle(gManp->hThread[i]);
	    gManp->hThread[i] = NULL;
//	    char    buf[256];
//	    sprintf_s(buf, "Pert thread %d finished\n", i);
//	    OutputDebugStringA(buf);
	    }
	}

#ifdef _DEBUG
    {
    char    buf[256];
    double pertSeconds = tPertRender.stop_ms();
    auto s = FormatElapsed(pertSeconds);
    SAFE_SPRINTF(buf, "Pert Render: %s\n", s.c_str());
    OutputDebugStringA(buf);
    }
#endif
    gManp->CurrentRenderMode = NOMULTITHREAD;

    /*
    Perturbation parameters have a standard mainly based on forward differencing which is the dominant subtype. 
    gManp->param[]:	meaning
    0		Slope type or transfer factor
    1		Start palette
    2		Light Direction
    3		Mapping Depth
    4		Mapping Strength
    5		Degree

    But for some subtypes it gets complicated, so we remap a few here
    */

    SimpleTimer tCalcSlope;
    tCalcSlope.start();
    auto& pert = gManp->PertCalculator[0];
    CSlope Slope;
    Slope.PaletteShift = gManp->PertCalculator[0]->PaletteShift;
    if (pert->SlopeType == FWDDIFFSLOPE)
	{
	RGBTRIPLE   SpecialColour = {0,0,0};			// future use if we can implement Art Matrix Cubic in perturbation
	Slope.InitRender(gManp->threshold, &gManp->TrueCol, &gManp->Dib, gManp->PaletteShift, gManp->bump_transfer_factor, gManp->PaletteStart, gManp->lightDirectionDegrees, gManp->bumpMappingDepth, gManp->bumpMappingStrength, SpecialColour);
	Slope.RenderSlope(gManp->xdots, gManp->ydots, gManp->PertColourMethod, gManp->PalOffset, gManp->IterDiv, gManp->ColourSpeed);
	}
    /*else */if (gManp->_3dflag)
	{
	Run3DAfterRender();
	}
    if (ghMutex != NULL)
	{
	CloseHandle(ghMutex);
	ghMutex = NULL;
	}

#ifdef _DEBUG
    {
    char    buf[256];
    double slopeSeconds = tCalcSlope.stop_ms();
    auto s = FormatElapsed(slopeSeconds);
    SAFE_SPRINTF(buf, "fwd diff slope (if used): %s\n", s.c_str());
    OutputDebugStringA(buf);
    }
#endif
    return 0;
    }

/**************************************************************************
    Error handling - courtesy of Microsnot
**************************************************************************/

void	CManp::ErrorHandler(LPTSTR lpszFunction)
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

    for (i = 0; i < NUMPERTPARAM - 1; i++)	// NUMPERTPARAM - 1 because param[15] is reserved for start colour
	{
	gManp->param[i] = PerturbationSpecific[gManp->subtype].paramvalue[i];
	}
    gManp->rqlim = PerturbationSpecific[gManp->subtype].rqlim;
    gManp->EnableApproximation = (PerturbationSpecific[gManp->subtype].EnableApproximation == 1) ? true : false;
    gManp->SlopeType = PerturbationSpecific[gManp->subtype].SlopeType;
    gManp->ColourSpeed = gManp->param[0];
    gManp->lightDirectionDegrees = gManp->param[1];
    gManp->bumpMappingDepth = gManp->param[2];
    gManp->bumpMappingStrength = gManp->param[3];
    gManp->bump_transfer_factor = gManp->param[4];
    gManp->PaletteStart = (int)(gManp->param[5]);
    if (gManp->subtype == 57)				// Polynomial has a special parameter layout
	{
	gManp->LightHeight = gManp->param[6];

	// Polynomial does not use the standard Pert colour controls.
	gManp->PertColourMethod = 0;
	gManp->IterDiv = 1.0;
	gManp->PalOffset = 0;
	}
    else
	{
	gManp->PertColourMethod = (int)(gManp->param[6]);
	gManp->IterDiv = gManp->param[7];
	gManp->PalOffset = (int)(gManp->param[8]);
	}
    }

/**************************************************************************
    Initialise the common Perturbation rendering parameters.

    TogglePerturbation() can enter Perturbation from several fractal types
    whose param[] slots have different meanings.  Reset the common Pert
    controls here so values from the previous fractal are not accidentally
    reinterpreted as smoothing, slope or colouring parameters.

    param[9] and above are subtype-specific and must be initialised by the
    individual conversion.
**************************************************************************/

static void InitPertCommonParams()
    {
    gManp->rqlim = 1000.0;
    gManp->param[0] = 0.0;		// Smooth Factor
    gManp->param[1] = 45.0;		// Light Direction
    gManp->param[2] = 50.0;		// Slope Mapping Depth
    gManp->param[3] = 50.0;		// Slope Mapping Strength
    gManp->param[4] = 1.0;		// Slope Transfer Factor
    gManp->param[5] = 0.0;		// Start Palette
    gManp->param[6] = 0.0;		// Pert Colour Method
    gManp->param[7] = 1.0;		// Iteration Divider
    gManp->param[8] = 0.0;		// Palette Offset
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
	InitPertCommonParams();
	*subtype = 0;
	gManp->param[9] = 1.5;			// Light Source Height (Derivative)
	}
    else if (*type == POWER)
	{
	*type = PERTURBATION;
	*subtype = 1;
	InitPertCommonParams();
	gManp->param[9] = 1.5;			// Light Source Height (Derivative)
	gManp->param[10] = gManp->degree;	// Pert Power degree
	}
    else if (*type == REDSHIFTRIDER)
	{
	*type = PERTURBATION;
	*subtype = 53;
	// Preserve RedShiftRider formula parameters in the Pert layout.
	gManp->param[12] = gManp->param[5];	// sign
	gManp->param[11] = gManp->param[2];	// n
	gManp->param[10] = -gManp->param[1];	// a imag
	gManp->param[9] = gManp->param[0];	// a real

	InitPertCommonParams();
	}
    else if (*type == SLOPEDERIVATIVE)
	{
	double LightDirection = gManp->param[0];
	double LightHeight = gManp->param[1];
	double PaletteStart = gManp->param[2];
	double SmoothFactor = gManp->param[3];
	double PolynomialDegree = gManp->param[7];

	*type = PERTURBATION;
	InitPertCommonParams();

	switch (*subtype)
	    {
	    case 0:
		*subtype = 0;
		break;

	    case 1:
		*subtype = 1;
		gManp->param[10] = PolynomialDegree;
		break;

	    case 2:
		*subtype = 1;
		gManp->param[10] = gManp->param[7];
		break;

	    default:
		return -1;
	    }

	// Translate common slope controls into the Perturbation layout.
	gManp->param[0] = SmoothFactor;
	gManp->param[1] = LightDirection;
	gManp->param[5] = PaletteStart;
	gManp->param[9] = LightHeight;
	}
    else if (*type == TALIS)
	{
	*type = PERTURBATION;
	InitPertCommonParams();
	gManp->rqlim = 12.0;		// special case - overrides defaults set in InitPertCommonParams()
	switch (gManp->degree)
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
	}
    else if (*type == POLYNOMIAL)
	{
	double PolyCoeff[MAXPOLY];

	for (int i = 0; i < MAXPOLY; i++)
	    PolyCoeff[i] = gManp->param[i + 2];

	*type = PERTURBATION;
	InitPertCommonParams();
	gManp->param[6] = 1.5;			// Light Source Height (Derivative)
	for (int i = 0; i < MAXPOLY; i++)
	    gManp->param[i + 7] = PolyCoeff[i];
	}

    else if (*type == PERTURBATION)
	{
	*type = MANDELDERIVATIVES;		// for most types - exceptions handled separately
	gManp->rqlim = 4.0;
	gManp->juliaflag = FALSE;
	// Clear Pert rendering parameters before reusing these slots for the destination fractal's native parameters.
	gManp->param[0] = 0.0;
	gManp->param[1] = 0.0;
	gManp->param[2] = 0.0;
	gManp->param[3] = 0.0;
	switch (*subtype)
	    {
	    case 0:
		*type = MANDELFP;
		gManp->param[0] = gManp->param[1] = 0.0;	// reset params
		break;
	    case 1:
		*type = POWER;
		gManp->param[0] = gManp->param[10];		// copy degree
		break;
	    case 2:
		*subtype = 1;					// Burning Ship
		break;
	    case 3:
		*subtype = 2;					// cubic Burning Ship
		gManp->degree = 3;
		gManp->param[0] = gManp->degree;
		break;
	    case 4:
		*subtype = 21;					// 4th Power Burning Ship
		break;
	    case 5:
		*subtype = 46;			// 5th Power Burning Ship
		break;
	    case 6:
		gManp->degree = 2;
		*subtype = 7;			// Celtic
		gManp->param[0] = gManp->degree;
		break;
	    case 7:
		gManp->degree = 3;
		*subtype = 7;			// Cubic Celtic
		gManp->param[0] = gManp->degree;
		break;
	    case 8:
		*subtype = 31;			// Celtic Buffalo 4th
		gManp->param[0] = gManp->param[1] = 0.0;
		break;
	    case 9:
		*subtype = 50;			// Celtic 5th (Buffalo Partial)
		break;
	    case 10:
		*subtype = 6;			// Mandelbar Tricorn
		gManp->param[0] = gManp->degree = 2;
		break;
	    case 11:
		*subtype = 6;			// Mandelbar power
		gManp->degree = (BYTE)gManp->param[9];
		gManp->param[0] = gManp->degree;
		gManp->param[3] = 1.0;
		if (gManp->degree == 4 || gManp->degree == 5)
		    gManp->param[3] = 0.0;
		if (gManp->degree > 5)
		    return -1;
		break;
	    case 12:
		*subtype = 4;			// Buffalo
		gManp->param[0] = gManp->degree = 2;
		break;
	    case 13:
		*subtype = 4;			// Cubic Buffalo only
		gManp->param[0] = gManp->degree = 3;
		break;
	    case 14:
		*subtype = 4;			// Quartic Buffalo only
		gManp->param[0] = gManp->degree = 4;
		gManp->param[1] = gManp->param[2] = 0.0;
		break;
	    case 15:
		*subtype = 4;			// Qunitic Buffalo only
		gManp->param[0] = gManp->degree = 5;
		break;
	    case 16:
		*subtype = 8;			// Mandelbar Celtic
		break;
	    case 17:
		*subtype = 0;			// Perpendicular Mandelbrot
		break;
	    case 18:
		*subtype = 3;			// Perpendicular Burning Ship
		break;
	    case 19:
		*subtype = 9;			// Perpendicular Celtic
		break;
	    case 20:
		*subtype = 5;			// Perpendicular Buffalo
		break;
	    case 21:
		*subtype = 16;			//  Cubic Quasi Burning Ship 
		break;
	    case 22:
		*subtype = 13;			// Partial Cubic Burning Ship Real 
		break;
	    case 23:
		*subtype = 14;			// Partial Cubic Burning Ship Imaginary 
		break;
	    case 24:
		*subtype = 10;			// Cubic Flying Squirrel (Buffalo Imaginary)
		break;
	    case 25:
		*subtype = 17;			// Cubic Quasi Perpendicular
		break;
	    case 26:
		*subtype = 22;			// Burning Ship 4th Partial imag
		break;
	    case 27:
		*subtype = 23;			// Burning Ship 4th Partial Real
		break;
	    case 28:
		*subtype = 24;			// Burning Ship 4th Partial Real Mandelbar
		break;
	    case 29:
		*subtype = 26;			// Celtic Burning Ship 4th Partial imag
		break;
	    case 30:
		*subtype = 27;			// Celtic Burning Ship 4th Partial Real
		break;
	    case 31:
		*subtype = 28;			// Celtic Burning Ship 4th Partial Real Mandelbar
		break;
	    case 32:
		*subtype = 30;			// Buffalo 4th Partial imag
		break;
	    case 33:
		*subtype = 32;			// Celtic 4th Mandelbar
		break;
	    case 34:
		*subtype = 33;			// 4th False Quasi Perpendicular 
		break;
	    case 35:
		*subtype = 34;			// 4th False Quasi Heart 
		break;
	    case 36:
		*subtype = 35;			// Celtic 4th False Quasi Perpendicular 
		break;
	    case 37:
		*subtype = 36;			// Celtic 4th False Quasi Heart 
		break;
	    case 38:
		*subtype = 37;			// Imag Quasi Perpendicular / Heart 4th 
		break;
	    case 39:
		*subtype = 38;			// 4th Quasi Perpendicular Real 
		break;
	    case 40:
		*subtype = 39;			// 4th Quasi Heart Real 
		break;
	    case 41:
		*subtype = 40;			// Celtic Imag Quasi Perpendicular / Heart 4th
		break;
	    case 42:
		*subtype = 41;			// Celtic 4th Quasi Perpendicular Real 
		break;
	    case 43:
		*subtype = 42;			// Celtic 4th Quasi Heart Real 
		break;
	    case 44:
		*subtype = 48;			// Burning Ship 5th Partial
		break;
	    case 45:
		*subtype = 49;			// Burning Ship 5th Partial Mandelbar
		break;
	    case 46:
		*subtype = 51;			// Celtic 5th Mandelbar
		break;
	    case 47:
		*subtype = 52;			// Quazi Burning Ship 5th (BS/Buffalo Hybrid)
		break;
	    case 48:
		*subtype = 53;			// 5th Quasi Perpendicular
		break;
	    case 49:
		*subtype = 54;			// 5th Quasi Heart
		break;
	    case 50:
		*subtype = 55;			// SimonBrot
		gManp->param[0] = 4;
		break;
	    case 51:
		*subtype = 55;			// Cubic SimonBrot
		gManp->param[0] = 6;
		break;
	    case 52:
		*subtype = 56;			// SimonBrot2
		gManp->param[0] = 4;
		break;
	    case 53:
		*type = REDSHIFTRIDER;
		gManp->param[0] = gManp->param[9];		// copy params
		gManp->param[1] = -gManp->param[10];
		gManp->param[2] = gManp->param[11];
		gManp->param[5] = gManp->param[12];		// copy sign
		break;
	    case 54:
	    case 55:
	    case 56:
		*type = TALIS;
		gManp->rqlim = 400.0;
		gManp->param[0] = gManp->degree = *subtype - 52;
		gManp->param[1] = 1.0;
		break;
	    case 57:			// polynomial
		{
		*type = POLYNOMIAL;
		double PolyCoeff[MAXPOLY];

		for (int i = 0; i < MAXPOLY; i++)
		    PolyCoeff[i] = gManp->param[i + 7];

		for (int i = 0; i < MAXPOLY; i++)
		    gManp->param[i + 2] = PolyCoeff[i];
		}
		break;

	    case 58:
		*subtype = 58;		// HPDZ Buffalo
		break;
	    default:
		return -1;
	    }
	//	for (int i = 0; i < NUMPARAM; i++)
	//	    gManp->param[i] = 0;
	}
    else if (*type == MANDELDERIVATIVES)
	{
	double PolynomialDegree = gManp->param[0];

	*type = PERTURBATION;
	InitPertCommonParams();
	switch (*subtype)
	    {
	    case 0:
		*subtype = 17;		// Perpendicular Mandelbrot
		break;
	    case 1:
		*subtype = 2;		// Burning Ship
		break;
	    case 2:
		if (gManp->degree == 3)
		    *subtype = 3;	// Cubic Burning Ship
		else if (gManp->degree == 4)
		    *subtype = 4;	// 4th Power Burning Ship
		else if (gManp->degree == 5)
		    *subtype = 5;	// 5th Power Burning Ship
		else
		    return -1;
		break;
	    case 3:
		*subtype = 18;		// Perpendicular Burning Ship
		break;
	    case 4:
		if (gManp->degree == 2)
		    *subtype = 12;	// Buffalo
		else if (gManp->degree == 3)
		    *subtype = 13;	// Cubic Buffalo
		else if (gManp->degree == 4)
		    *subtype = 14;	// Quartic Buffalo
		else if (gManp->degree == 5)
		    *subtype = 15;	// Quintic Buffalo
		else
		    return -1;
		break;
	    case 5:
		*subtype = 20;		// Perpendicular Buffalo
		break;
	    case 6:
		if (gManp->degree == 2)
		    *subtype = 10;	// Mandelbar Tricorn
		else
		    {
		    gManp->degree = (BYTE)PolynomialDegree;
		    gManp->param[9] = gManp->degree;
		    *subtype = 11;	// Mandelbar Tricorn Higher order
		    }
		break;
	    case 7:
		if (gManp->degree == 3)
		    *subtype = 7;	// cubic Celtic
		else if (gManp->degree == 2)
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
		gManp->param[10] = gManp->degree = 4;
		break;
	    case 20:
		*subtype = 11;		// 4th Power Mandelbar
		gManp->param[9] = gManp->degree = 4;
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
		gManp->param[10] = gManp->degree = 5;
		*subtype = 1;		// Mandelbrot 4th 
		break;
	    case 44:
		*subtype = 11;		// 5th Mandelbar (vertical)
		gManp->param[9] = gManp->degree = 5;
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
		if (PolynomialDegree == 4)
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

INT_PTR CALLBACK SelectPertDlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
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
	    return (TRUE);

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
		    gManp->subtype = PerturbationPtr;
		    EndDialog(hDlg, TRUE);
		    return (TRUE);

		case IDCANCEL:
		    PerturbationPtr = 1;
		    EndDialog(hDlg, FALSE);
		    return (FALSE);

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
			    gManp->subtype = PerturbationPtr = index;
			    EndDialog(hDlg, TRUE);
			    return (TRUE);
			}
		    return (TRUE);
		}
	}
    return (FALSE);
    }

/**************************************************************************
    Setup Multi-threading parameters
**************************************************************************/

INT_PTR CALLBACK ThreadingDlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
    {
    BOOL		bTrans;
    SYSTEM_INFO		sysinfo;
    HWND		hCtrl;

    switch (message)
	{
	case WM_INITDIALOG:
	    SetDlgItemInt(hDlg, IDC_NUMBERTHREADS, gManp->NumberThreads, TRUE);
	    SetDlgItemInt(hDlg, IDC_THREADCREATIONDELAY, gManp->ThreadCreationDelay, TRUE);
	    SetDlgItemInt(hDlg, IDC_THREADCOMPLETIONDELAY, gManp->ThreadCompletionDelay, TRUE);
	    SetDlgItemInt(hDlg, IDC_THREADENDINGDELAY, gManp->ThreadEndingDelay, TRUE);
	    SetDlgItemInt(hDlg, IDC_THREADPERTDELAY, gManp->ThreadPertDelay, TRUE);
	    GetSystemInfo(&sysinfo);
	    SetDlgItemInt(hDlg, IDC_NUMBERCORES, sysinfo.dwNumberOfProcessors, TRUE);
	    hCtrl = GetDlgItem(hDlg, IDC_USE_MUTEX);
	    SendMessage(hCtrl, BM_SETCHECK, gManp->UseMutex, 0L);
	    return (TRUE);

	case WM_COMMAND:
	    switch ((int)LOWORD(wParam))
		{
		case IDOK:
		    gManp->NumberThreads = GetDlgItemInt(hDlg, IDC_NUMBERTHREADS, &bTrans, TRUE);
		    if (gManp->NumberThreads <= 0)
			gManp->NumberThreads = 1;
		    hCtrl = GetDlgItem(hDlg, IDC_USE_MUTEX);
		    gManp->UseMutex = (BYTE)SendMessage(hCtrl, BM_GETCHECK, 0, 0L);
		    gManp->ThreadCreationDelay = GetDlgItemInt(hDlg, IDC_THREADCREATIONDELAY, &bTrans, TRUE);
		    gManp->ThreadCompletionDelay = GetDlgItemInt(hDlg, IDC_THREADCOMPLETIONDELAY, &bTrans, TRUE);
		    gManp->ThreadEndingDelay = GetDlgItemInt(hDlg, IDC_THREADENDINGDELAY, &bTrans, TRUE);
		    gManp->ThreadPertDelay = GetDlgItemInt(hDlg, IDC_THREADPERTDELAY, &bTrans, TRUE);
		    EndDialog(hDlg, TRUE);
		    return (TRUE);

		case IDCANCEL:
		    EndDialog(hDlg, FALSE);
		    return (FALSE);
		}
	}
    return (FALSE);
    }

/**************************************************************************
    Perturbation Fractal Parameters
**************************************************************************/

INT_PTR CALLBACK PertDlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
    {
    int i;
    char		s[NUMPERTPARAM][100];
    char		Bailout[120];
    static  PlotMode	tempMethod;
    static     UINT	tempParam;
    BOOL		bTrans;
    HWND		hCtrl;

    switch (message)
	{
	case WM_INITDIALOG:
	    SAFE_SPRINTF(Bailout, "%4.4lf", gManp->rqlim);
	    SetDlgItemText(hDlg, IDC_BAILOUT, Bailout);
	    SetDlgItemText(hDlg, ID_FRACNAME, gManp->GetFractalName());

	    for (i = 0; i < gManp->Fractal.NumParam && i < NUMPERTPARAM - 1; i++)	// NUMPERTPARAM  - 1 because param[15] is reserved for start colour
		{
		SAFE_SPRINTF(s[i], "%f", *gManp->Fractal.ParamValue[i]);
		SetDlgItemText(hDlg, ID_FRACPARTX1 + i, gManp->Fractal.ParamName[i]);
		SetDlgItemText(hDlg, ID_FRACPARAM1 + i, s[i]);
		}
	    for (i = gManp->Fractal.NumParam; i < NUMPERTPARAM - 1; i++)		// NUMPERTPARAM - 1 because param[15] is reserved for start colour
		SetDlgItemText(hDlg, ID_FRACPARTX1 + i, "     N/A");

	    tempMethod = currentMode;
	    tempParam = IDC_PLOTMODE0 + (int)tempMethod;
	    CheckRadioButton(hDlg, IDC_PLOTMODE0, IDC_PLOTMODE8, tempParam);
	    hCtrl = GetDlgItem(hDlg, IDC_USEBLA);
	    SendMessage(hCtrl, BM_SETCHECK, gManp->EnableApproximation, 0L);
	    SetDlgItemInt(hDlg, ID_SLOPETYPE, (UINT)gManp->SlopeType, TRUE);
	    return (TRUE);

	case WM_COMMAND:
	    switch ((int)LOWORD(wParam))
		{
		case IDC_PLOTMODE0:
		case IDC_PLOTMODE1:
		case IDC_PLOTMODE2:
		case IDC_PLOTMODE3:
		case IDC_PLOTMODE4:
		case IDC_PLOTMODE5:
		case IDC_PLOTMODE6:
		case IDC_PLOTMODE7:
		case IDC_PLOTMODE8:

		    tempMethod = (PlotMode)((int)LOWORD(wParam) - IDC_PLOTMODE0);
		    CheckRadioButton(hDlg, IDC_PLOTMODE0, IDC_PLOTMODE8, (int)LOWORD(wParam));
		    return TRUE;

		case IDOK:
		    GetDlgItemText(hDlg, IDC_BAILOUT, Bailout, 100);
		    gManp->rqlim = atof(Bailout);
		    currentMode = tempMethod;
		    for (i = 0; i < gManp->Fractal.NumParam && i < NUMPERTPARAM - 1; i++)	// NUMPERTPARAM - 1 because param[15] is reserved for start colour
			{
			GetDlgItemText(hDlg, ID_FRACPARAM1 + i, s[i], 100);
			*gManp->Fractal.ParamValue[i] = atof(s[i]);
			}
		    hCtrl = GetDlgItem(hDlg, IDC_USEBLA);
		    // gManp holds the live Pert rendering state.
		    // param[] is the transport/storage form used by the database, PAR files and parameter animation.
		    gManp->EnableApproximation = (BOOL)SendMessage(hCtrl, BM_GETCHECK, 0, 0L);
		    gManp->SlopeType = GetDlgItemInt(hDlg, ID_SLOPETYPE, &bTrans, TRUE);
		    gManp->ColourSpeed = *gManp->Fractal.ParamValue[0];
		    gManp->lightDirectionDegrees = *gManp->Fractal.ParamValue[1];
		    gManp->bumpMappingDepth = *gManp->Fractal.ParamValue[2];
		    gManp->bumpMappingStrength = *gManp->Fractal.ParamValue[3];
		    gManp->bump_transfer_factor = *gManp->Fractal.ParamValue[4];
		    gManp->PaletteStart = (int)(*gManp->Fractal.ParamValue[5]);
		    // Most Pert subtypes use the standard layout:
		    //   [6] PertColourMethod, [7] IterDiv, [8] PalOffset, [9] LightHeight
		    // but Polynomial (subtype 57) uses a special layout:
		    //   [6] LightHeight, [7..14] polynomial coefficients.
		    // So the runtime copy must be subtype-aware.
		    if (gManp->subtype == 57)				// Polynomial has a special parameter layout
			{
			gManp->LightHeight = *gManp->Fractal.ParamValue[6];

			// Polynomial does not use the standard Pert colour controls.
			gManp->PertColourMethod = 0;
			gManp->IterDiv = 1.0;
			gManp->PalOffset = 0;
			}
		    else
			{
			gManp->PertColourMethod = (int)(*gManp->Fractal.ParamValue[6]);
			gManp->IterDiv = *gManp->Fractal.ParamValue[7];
			gManp->PalOffset = (int)(*gManp->Fractal.ParamValue[8]);
			// Only the subtypes that support derivative slope need LightHeight.
			if (gManp->subtype == 0 || gManp->subtype == 1 || gManp->subtype == 59 || gManp->subtype == 60 || gManp->subtype == 61 || gManp->subtype == 62 || gManp->subtype == 63)
			    gManp->LightHeight = *gManp->Fractal.ParamValue[9];
			else
			    gManp->LightHeight = 0.0;
			}
		    EndDialog(hDlg, TRUE);
		    return (TRUE);

		case IDCANCEL:
		    EndDialog(hDlg, FALSE);
		    return (FALSE);
		}
	}
    return (FALSE);
    }


