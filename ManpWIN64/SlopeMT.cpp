/*
    SLOPEMT.CPP a module to explore Slope fractals

    Written in Microsoft Visual C++ by Paul de Leeuw.
*/

#include <process.h>
#include <strsafe.h>
#include <atomic>
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
#include "SafeStrings.h"
#include "..\QDLib\FractalAbort.h"

#define		MAXTHREADS	40

/**************************************************************************
	Additional globals for slope fractal
**************************************************************************/

extern	int	user_data(HWND);
extern	std::atomic<long> gPixelsDone;

void	ApplySlopeDerivParams();
void	ApplySlopeFwdDiffParams();

/**************************************************************************
	Slope engine, pass into thread creation routine
**************************************************************************/

DWORD WINAPI SlopeFunction(LPVOID lpParam)
    {
    pSlopeThreadData p = (pSlopeThreadData)lpParam;
    int ThreadNum = p->i;
    int(*UserData)(HWND) = user_data;
    int ret = 0;

    // Slope rendering parameters are stored in param[] so that they are
    // preserved in PAR files.  Mirror them into the working gManp values
    // used by the slope renderers and status/reporting code.
    if (gManp->type == SLOPEDERIVATIVE)
	ApplySlopeDerivParams();
    else
	ApplySlopeFwdDiffParams();

     try
	{
	if (gManp->type == SLOPEDERIVATIVE)
	    ret = gManp->Slope[ThreadNum]->RunSlopeDerivative(gManp->GlobalHwnd, UserData, gManp->SlopeStatus, gManp->subtype, p->NumThreads, ThreadNum, gManp->j, gManp->mandel_width,
		gManp->hor, gManp->vert, gManp->BigNumFlag, gManp->BigHor, gManp->BigVert, gManp->BigWidth, gManp->rqlim, gManp->threshold, gManp->param, &gManp->TrueCol, &gManp->Dib, gManp->juliaflag, gManp->xdots, gManp->ydots, 
		gManp->width, gManp->height, &gManp->degree, gManp->precision, gManp->ColourSpeed, p->ghMutex, p->pixelOrder, p->workIndex, p->totalPixels, p->mode);
	else
	    ret = gManp->Slope[ThreadNum]->RunSlopeFwdDiff(gManp->GlobalHwnd, UserData, gManp->SlopeStatus, gManp->subtype, p->NumThreads, ThreadNum, gManp->j, gManp->mandel_width,
		gManp->hor, gManp->vert, gManp->BigNumFlag, gManp->BigHor, gManp->BigVert, gManp->BigWidth, gManp->rqlim, gManp->threshold, gManp->param, &gManp->TrueCol, &gManp->Dib, &gManp->wpixels, gManp->juliaflag, gManp->xdots,
		gManp->ydots, gManp->width, &gManp->degree, gManp->precision, gManp->ColourSpeed, p->pixelOrder, p->workIndex, p->totalPixels, p->mode, gManp->InsideMethod, gManp->OutsideMethod, gManp->biomorph, gManp->potparam);
	}
    catch (const FractalAbort&)
	{
	gStopRequested.store(true);
	OutputDebugStringA("Slope thread aborted cleanly\n");
	ret = -1;
	}
    catch (...)
	{
	gStopRequested.store(true);
	OutputDebugStringA("Unknown exception in Slope thread\n");
	ret = -1;
	}

    return ret;
    }

/**************************************************************************
	Start Derivative Slope worker threads
**************************************************************************/

int StartDerivSlopeThreads(int threadCount, HANDLE ghMutex, std::vector<std::pair<int, int>> *pixelOrder, std::atomic<int> *workIndex, int totalPixels, PlotMode mode, DWORD dwThreadId[])
    {
    int threadsStarted = 0;

    for (int i = 0; i < threadCount; i++)
	{
	gManp->pSlopeDataArray[i] =
	    (pSlopeThreadData)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(SlopeThreadData));

	if (gManp->pSlopeDataArray[i] == NULL)
	    {
	    SAFE_SPRINTF(gManp->SlopeErrorMessage, "Can't find memory for data array in thread %d", i);
	    gManp->ErrorHandler(gManp->SlopeErrorMessage);
	    gStopRequested.store(true, std::memory_order_relaxed);
	    break;
	    }

	gManp->pSlopeDataArray[i]->NumThreads = threadCount;
	gManp->pSlopeDataArray[i]->i = i;
	gManp->pSlopeDataArray[i]->TrueCol = &gManp->TrueCol;
	gManp->pSlopeDataArray[i]->ghMutex = ghMutex;
	gManp->pSlopeDataArray[i]->pixelOrder = pixelOrder;
	gManp->pSlopeDataArray[i]->workIndex = workIndex;
	gManp->pSlopeDataArray[i]->totalPixels = totalPixels;
	gManp->pSlopeDataArray[i]->mode = mode;
	gManp->pSlopeDataArray[i]->InsideMethod = gManp->InsideMethod;
	gManp->pSlopeDataArray[i]->OutsideMethod = gManp->OutsideMethod;
	gManp->pSlopeDataArray[i]->biomorph = gManp->biomorph;
	gManp->pSlopeDataArray[i]->potparam = gManp->potparam;

	gManp->hSlopeThread[i] = CreateThread(NULL, 0L, (LPTHREAD_START_ROUTINE)SlopeFunction, gManp->pSlopeDataArray[i], 0, &dwThreadId[i]);

	if (gManp->hSlopeThread[i] == NULL)
	    {
	    SAFE_SPRINTF(gManp->SlopeErrorMessage, "CreateThread failed in thread %d", i);
	    gManp->ErrorHandler(gManp->SlopeErrorMessage);
	    gStopRequested.store(true, std::memory_order_relaxed);
	    break;
	    }

	SetThreadPriority(gManp->hSlopeThread[i], THREAD_PRIORITY_LOWEST);
	threadsStarted++;
	Sleep(gManp->ThreadCreationDelay);
	}

    return threadsStarted;
    }
    
/**************************************************************************
	Get number of Slope worker threads
**************************************************************************/

int GetSlopeThreadCount()
    {
    int threadCount = (gManp->NumberThreads <= 0 ? 1 : gManp->NumberThreads);

    if (threadCount > MAXTHREADS)
	threadCount = MAXTHREADS;

    return threadCount;
    }

/**************************************************************************
	Initialise Slope engine
**************************************************************************/

int	InitSlopeFractal()
    {
    int		i;

    int threadCount = GetSlopeThreadCount();

    // --- Create Slope objects (like CPixel) ---
    gManp->Slope.clear();
    gManp->Slope.reserve(threadCount);

    for (i = 0; i < threadCount; i++)
	{
	gManp->Slope.emplace_back(std::make_unique<CSlope>());
	}

    // --- Thread tracking arrays ---
    gManp->SlopeProgress.assign(threadCount, 0);
    gManp->hSlopeThread.assign(threadCount, nullptr);
    gManp->pSlopeDataArray.assign(threadCount, nullptr);

    for (i = 0; i < threadCount; i++)
	gManp->Slope[i]->EndSlope = false;

    gStopRequested.store(false, std::memory_order_relaxed);	// reset before starting threads
    gManp->CurrentRenderMode = RENDER_SLOPE;
    return 0;
    }

/**************************************************************************
	Wait for successfully started Slope worker threads
**************************************************************************/

void WaitForStartedSlopeThreads(int threadsStarted)
    {
    for (int i = 0; i < threadsStarted; i++)
	{
	if (gManp->hSlopeThread[i] != NULL)
	    WaitForSingleObject(gManp->hSlopeThread[i], INFINITE);
	}
    }

/**************************************************************************
	Clean up Slope resources after thread startup failure
**************************************************************************/

void CleanupSlopeStartupFailure(int threadCount, HANDLE &ghMutex)
    {
    for (int i = 0; i < threadCount; i++)
	{
	if (gManp->hSlopeThread[i] != NULL)
	    {
	    CloseHandle(gManp->hSlopeThread[i]);
	    gManp->hSlopeThread[i] = NULL;
	    }

	if (gManp->pSlopeDataArray[i] != NULL)
	    {
	    HeapFree(GetProcessHeap(), 0, gManp->pSlopeDataArray[i]);
	    gManp->pSlopeDataArray[i] = NULL;
	    }
	}

    if (ghMutex != NULL)
	{
	CloseHandle(ghMutex);
	ghMutex = NULL;
	}

    gManp->CurrentRenderMode = NOMULTITHREAD;
    }
    
/**************************************************************************
	Wait for Slope worker threads to finish
**************************************************************************/

void WaitForSlopeThreads(int threadsStarted)
    {
    int flag = 0;

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

	for (int i = 0; i < threadsStarted; i++)
	    {
	    if (gManp->curpass > gManp->SlopeProgress[i])
		gManp->curpass = gManp->SlopeProgress[i];

	    if (gManp->hSlopeThread[i] != NULL &&
		WaitForSingleObject(gManp->hSlopeThread[i], 0) == WAIT_TIMEOUT)
		flag = 0;
	    }

	if (!gManp->RunAnimation)
	    gManp->DisplayStatusBarInfo(INCOMPLETE, "");

	Sleep(gManp->ThreadCompletionDelay);
	}
    }

/**************************************************************************
	Clean up Slope worker thread resources
**************************************************************************/

void CleanupSlopeThreads(int threadCount)
    {
    for (int i = 0; i < threadCount; i++)
	{
	if (gManp->hSlopeThread[i] != NULL)
	    {
	    CloseHandle(gManp->hSlopeThread[i]);
	    gManp->hSlopeThread[i] = NULL;
	    }

	if (gManp->pSlopeDataArray[i] != NULL)
	    {
	    HeapFree(GetProcessHeap(), 0, gManp->pSlopeDataArray[i]);
	    gManp->pSlopeDataArray[i] = NULL;
	    }
	}
    }

/**************************************************************************
	Render completed Forward Difference Slope image
**************************************************************************/

void RenderForwardDifferenceSlope()
    {
    ApplySlopeFwdDiffParams();
    gManp->Slope[0]->InitRender(gManp->threshold, &gManp->TrueCol, &gManp->Dib, gManp->PaletteShift, gManp->bump_transfer_factor, gManp->PaletteStart, 
		gManp->lightDirectionDegrees, gManp->bumpMappingDepth, gManp->bumpMappingStrength, gManp->Slope[0]->SpecialColour);
    // Non-Kalles Forward Difference path.
    // Keep PertColourMethod = 0, PalOffset = 0 and IterDiv = 1.0; Pert/BLA supplies the real Kalles colour settings.
    gManp->Slope[0]->RenderSlope(gManp->xdots, gManp->ydots,
	0,		// PertColourMethod
	0,		// PalOffset
	1.0,		// IterDiv
	gManp->ColourSpeed);
    }

/**************************************************************************
    The Slope engine
**************************************************************************/

int	DoSlopeFractal()
    {
    DWORD	dwThreadId[MAXTHREADS];
    HANDLE	ghMutex = NULL;
    std::vector<std::pair<int, int>> pixelOrder;
    std::atomic<int> workIndex(0);
    int		totalPixels = gManp->xdots * gManp->ydots;

    int threadCount = GetSlopeThreadCount();

    if (gManp->UseMutex)							// stuff speed, we want a good image
	ghMutex = CreateMutex(NULL, 0, NULL);
 
    gPixelsDone = 0;								// initialise progress counter
    gManp->CurrentRenderMode = RENDER_SLOPE;
    
    PlotMode mode = static_cast<PlotMode>(currentMode);

    BuildPixelOrder(pixelOrder, totalPixels, gManp->xdots, gManp->ydots, mode);
    workIndex.store(0);
    int threadsStarted = StartDerivSlopeThreads(threadCount, ghMutex, &pixelOrder, &workIndex, totalPixels, mode, dwThreadId);

    if (threadsStarted < threadCount)
	{
	// Startup failed.  Keep pixelOrder and workIndex alive until
	// every worker that did start has terminated.
	WaitForStartedSlopeThreads(threadsStarted);
	CleanupSlopeStartupFailure(threadCount, ghMutex);
	return -1;
	}

    WaitForSlopeThreads(threadsStarted);
    CleanupSlopeThreads(threadsStarted);

    gManp->CurrentRenderMode = NOMULTITHREAD;

    if (gManp->type == SLOPEFORWARDDIFF)
	RenderForwardDifferenceSlope();

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

    if (gManp->NumberThreads == 0)
	return 0;

    for (i = 0; i < gManp->NumberThreads; i++)
	if (i >= 0 && i < (int)gManp->Slope.size())
	    gManp->Slope[i]->SlopeIsExiting();
    return 0;
    }

/**************************************************************************
    Some simple processing
**************************************************************************/

int setup_SlopeDeriv(void)
    {
    if (!gManp->Fractal.SlopeDerivNum)
	{
	while (SlopeDerivSpecific[gManp->Fractal.SlopeDerivNum].name)
	    (gManp->Fractal.SlopeDerivNum)++;
	}
    return 0;
    }

int setup_SlopeFwdDiff(void)
    {
    if (!gManp->Fractal.SlopeFwdDiffNum)
	{
	while (SlopeFwdDiffSpecific[gManp->Fractal.SlopeFwdDiffNum].name)
	    (gManp->Fractal.SlopeFwdDiffNum)++;
	}
    return 0;
    }

void ApplySlopeDerivParams()
    {
    gManp->PaletteStart = (int)gManp->param[2];
    gManp->ColourSpeed = gManp->param[3];
    gManp->PrePaletteColour = (DWORD)gManp->param[15];
    }

void ApplySlopeFwdDiffParams()
    {
    gManp->bump_transfer_factor = gManp->param[0];
    gManp->PaletteStart = (int)gManp->param[1];
    gManp->lightDirectionDegrees = gManp->param[2];
    gManp->bumpMappingDepth = gManp->param[3];
    gManp->bumpMappingStrength = gManp->param[4];
    gManp->ColourSpeed = gManp->param[5];
    gManp->PrePaletteColour = (DWORD)gManp->param[15];
    }

void LoadSlopeDerivParams()
    {
    int i;

    for (i = 0; i < SlopeDerivSpecific[gManp->subtype].numparams; i++)
	gManp->param[i] = SlopeDerivSpecific[gManp->subtype].paramvalue[i];

    gManp->rqlim = SlopeDerivSpecific[gManp->subtype].rqlim;

    ApplySlopeDerivParams();
    }

void LoadSlopeFwdDiffParams()
    {
    int i;

    for (i = 0; i < NUMSLOPEPARAM - 1; i++)
	gManp->param[i] = SlopeFwdDiffSpecific[gManp->subtype].paramvalue[i];

    gManp->rqlim = SlopeFwdDiffSpecific[gManp->subtype].rqlim;

    ApplySlopeFwdDiffParams();
    }

/**************************************************************************
    Show Slope Fractals
**************************************************************************/

INT_PTR CALLBACK SelectSlopeDerivDlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)

    {
    static	int	i;
    static	int	index = 1;

    switch (message)
	{
	case WM_INITDIALOG:
	    SetDlgItemText(hDlg, ID_LISTTITLE, "Slope Using Derivative");
	    for (i = 0; i < gManp->Fractal.SlopeDerivNum; i++)
		SendDlgItemMessage(hDlg, IDM_LSYSTEM, LB_ADDSTRING, (WPARAM)NULL, (LPARAM)(LPSTR)SlopeDerivSpecific[i].name);
	    SendDlgItemMessage(hDlg, IDM_LSYSTEM, LB_SETCURSEL, (WPARAM)gManp->Fractal.SlopeDerivPtr, 0L);
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
		    gManp->Fractal.SlopeDerivPtr = index;
		    gManp->subtype = gManp->Fractal.SlopeDerivPtr;
		    EndDialog(hDlg, TRUE);
		    return (TRUE);

		case IDCANCEL:
		    gManp->Fractal.SlopeDerivPtr = 0;
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
			    gManp->subtype = gManp->Fractal.SlopeDerivPtr = index;
			    EndDialog(hDlg, TRUE);
			    return (TRUE);
			}
		    return (TRUE);
		}
	}
    return (FALSE);
    }

INT_PTR CALLBACK SelectSlopeFwdDiffDlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)

    {
    static	int	i;
    static	int	index = 1;

    switch (message)
	{
	case WM_INITDIALOG:
	    SetDlgItemText(hDlg, ID_LISTTITLE, "Slope Using Forward Differences");
	    for (i = 0; i < gManp->Fractal.SlopeFwdDiffNum; i++)
		SendDlgItemMessage(hDlg, IDM_LSYSTEM, LB_ADDSTRING, (WPARAM)NULL, (LPARAM)(LPSTR)SlopeFwdDiffSpecific[i].name);
	    SendDlgItemMessage(hDlg, IDM_LSYSTEM, LB_SETCURSEL, (WPARAM)gManp->Fractal.SlopeFwdDiffPtr, 0L);
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
		    gManp->Fractal.SlopeFwdDiffPtr = index;
		    gManp->subtype = gManp->Fractal.SlopeFwdDiffPtr;
		    EndDialog(hDlg, TRUE);
		    return (TRUE);

		case IDCANCEL:
		    gManp->Fractal.SlopeFwdDiffPtr = 0;
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
			    gManp->subtype = gManp->Fractal.SlopeFwdDiffPtr = index;
			    EndDialog(hDlg, TRUE);
			    return (TRUE);
			}
		    return (TRUE);
		}
	}
    return (FALSE);
    }

/**************************************************************************
    Slope Fractal Parameters
**************************************************************************/

INT_PTR CALLBACK SlopeDlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
    {
    int i;
    char		s[NUMSLOPEPARAM][100];
    char		Bailout[120];
    static  PlotMode	tempMethod;
    static     UINT	tempParam;
    
    switch (message)
	{
	case WM_INITDIALOG:
	    SAFE_SPRINTF(Bailout, "%4.4lf", gManp->rqlim);
	    SetDlgItemText(hDlg, IDC_BAILOUT, Bailout);
	    SetDlgItemText(hDlg, ID_FRACNAME, gManp->GetFractalName());

	    for (i = 0; i < gManp->Fractal.NumParam && i < NUMSLOPEPARAM - 1; i++)	// NUMSLOPEPARAM  - 1 because param[15] is reserved for start colour
		{
		SAFE_SPRINTF(s[i], "%f", *gManp->Fractal.ParamValue[i]);
		SetDlgItemText(hDlg, ID_FRACPARTX1 + i, gManp->Fractal.ParamName[i]);
		SetDlgItemText(hDlg, ID_FRACPARAM1 + i, s[i]);
		}
	    for (i = gManp->Fractal.NumParam; i < NUMSLOPEPARAM - 1; i++)		// NUMSLOPEPARAM  - 1 because param[15] is reserved for start colour
		SetDlgItemText(hDlg, ID_FRACPARTX1 + i, "     N/A");

	    tempMethod = currentMode;
	    tempParam = IDC_PLOTMODE0 + (int)tempMethod;
	    CheckRadioButton(hDlg, IDC_PLOTMODE0, IDC_PLOTMODE8, tempParam);
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
		    for (i = 0; i < gManp->Fractal.NumParam && i < NUMSLOPEPARAM - 1; i++)	// NUMSLOPEPARAM  - 1 because param[15] is reserved for start colour
			{
			GetDlgItemText(hDlg, ID_FRACPARAM1 + i, s[i], 100);
			*gManp->Fractal.ParamValue[i] = atof(s[i]);
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

