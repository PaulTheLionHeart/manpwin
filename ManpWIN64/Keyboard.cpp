/*------------------------------------------------
   KEYBOARD.CPP -- Handle Keyboard Messages
  ------------------------------------------------*/

#include <windows.h>
#include "manpwin.h"
#include "fractype.h"
#include "resource.h"
#include "colour.h"
#include "menu.h"
#include "manp.h"
#include "OtherFunctions.h"

//extern	void	ToggleRandomColour(void);
extern	int	AnimateClose(void);
extern	void	IncreaseAnimSpeed(void);
extern	void	DecreaseAnimSpeed(void);
extern	void	ReverseAnimDirection(void);
extern	void	StepFrame(void);
extern	BOOL	SaveGIFOpenDlg (HWND, LPSTR, LPSTR);
extern	BOOL	SaveMPGOpenDlg (HWND, LPSTR, LPSTR);
extern	void	SaveFile(HWND, LPSTR, LPSTR);
extern	int	SaveIndividualFrames(void);
extern	int	AnimateSuspend(void);
extern	int	AnimateResume(void);
extern	void	DisplayStatusBarInfo(int, char *);

extern	BOOL	RunAnimation;			// are we in the middle of an animation generation run?
extern	BOOL	SuspendAnimation;		// pause animation run?
extern	BOOL	RunMPEG;			// are we in the middle of generating an MPEG file?
extern	BOOL	RunGIF;				// are we in the middle of generating an GIF file?
extern	BOOL	DisplayAnimation;		// allow system to know that we are currently displaying an animation
extern	int	time_to_break;			// time to break out of animation?
extern	int	time_to_quit;			// time to quit?
extern	int	time_to_restart;		// time to restart?
extern	WORD	type;				// M=mand, N=Newton etc
extern	WORD	UpdateDelay;			// delay in milliseconds

extern	ProcessType	OscAnimProc;		// trap certain keyboatd activity when animating
extern	CTrueCol    TrueCol;			// palette info

#undef	TEST_TRIG 


#ifdef TEST_TRIG

#include <thread>
#include <vector>

std::atomic<bool> done[8] = {};

void WorkerCSin(int id)
    {
    CBigTrig trig;

    BigComplex in, out;
    in = 0.0;
    in.x.ChangePrecision(256);
    in.y.ChangePrecision(256);
    out.x.ChangePrecision(256);
    out.y.ChangePrecision(256);

    in.x = 0.5;
    in.y = 0.25;

    for (int i = 0; i < 50000; ++i)
	{
	trig.CSin(&out, in);
	}

    char bx[256], by[256];
    out.x.ToString(bx, sizeof(bx), true);
    out.y.ToString(by, sizeof(by), true);

    for (int i = 0; i < 50000; ++i)
	{
	BigComplex w;
	trig.CSin(&w, in);
	}

    // ADD THIS BLOCK RIGHT HERE
    if (id == 0)
	{
	BigComplex ref = in;
	ref = ref.CSin(); // MPFR version

	char rx[256], ry[256];
	ref.x.ToString(rx, sizeof(rx), true);
	ref.y.ToString(ry, sizeof(ry), true);

	OutputDebugStringA(
	    ("REF: x=" + std::string(rx) + " y=" + std::string(ry) + "\n").c_str());
	}

    // THEN your existing print
    out.x.ToString(bx, sizeof(bx), true);
    out.y.ToString(by, sizeof(by), true);

    OutputDebugStringA(
	("Thread " + std::to_string(id) +
	    " done: x=" + bx + " y=" + by + "\n").c_str());

    OutputDebugStringA(
	("Thread " + std::to_string(id) +
	    " done: x=" + bx + " y=" + by + "\n").c_str());
    OutputDebugStringA(("Thread " + std::to_string(id) + " EXIT\n").c_str());
    done[id] = true;
    }


void TestMultiThreadCSin()
    {
    const int NUM_THREADS = 8;

    std::vector<std::thread> threads;

    for (int i = 0; i < NUM_THREADS; ++i)
	{
	threads.emplace_back(WorkerCSin, i);
	}

    for (auto& t : threads)
	t.join();

    for (int i = 0; i < 8; ++i)
	{
	if (!done[i])
	    {
	    OutputDebugStringA(("Thread " + std::to_string(i) + " MISSING\n").c_str());
	    }
	}

    OutputDebugStringA("Multi-thread CSin test complete\n");
    }

static void DebugBDShort(const char* label, const BigDouble& v)
    {
    char buf[128];
    mpfr_snprintf(buf, sizeof(buf), "%.20Re", v.x);
    std::string msg = std::string(label) + " = " + buf + "\n";
    OutputDebugStringA(msg.c_str());
    }


void TestSingleCSin()
    {
    CBigTrig trig;

    BigComplex in, sout, cout, sref, cref;
    BigDouble pi;
    pi.ChangePrecision(256);
    pi.SetPi();

    in = 0.0;

    in.x.ChangePrecision(256);
    in.y.ChangePrecision(256);
    cout.x.ChangePrecision(256);
    cout.y.ChangePrecision(256);
    cref.x.ChangePrecision(256);
    cref.y.ChangePrecision(256);
    sout.x.ChangePrecision(256);
    sout.y.ChangePrecision(256);
    sref.x.ChangePrecision(256);
    sref.y.ChangePrecision(256);

    in.x = 0.5;
    in.y = 0.25;

    in.x = 1.0;
    in.y = 0.0;

    in.x = pi / 2;
    in.y = 0.0;

    in.x = pi;
    in.y = 0.0;
/*
    in.x = 3 * pi / 2;
    in.y = 0;

    in.x = pi * 2;
    in.y = 0;

    in.x = pi / 2 - 1e-10;
    in.y = 0.0;

    in.x = 0;
    in.y = 1;
*/
    in.x = -pi * 3;
    in.y = 0;

    in.x = 1.0;
    in.y = 250.0;


    DebugBDShort("INPUT x", in.x);
    DebugBDShort("INPUT y", in.y);

//    char ix[256], iy[256];
//    in.x.ToString(ix, sizeof(ix), true);
//    in.y.ToString(iy, sizeof(iy), true);
//    OutputDebugStringA(("INPUT: x=" + std::string(ix) + " y=" + iy + "\n").c_str());

    trig.CSin(&sout, in);
    trig.CCos(&cout, in);

    sref = in;
    sref = sref.CSin();
    cref = in;
    cref = cref.CCos();

    char ox[256], oy[256], rx[256], ry[256];
    sout.x.ToString(ox, sizeof(ox), true);
    sout.y.ToString(oy, sizeof(oy), true);
    sref.x.ToString(rx, sizeof(rx), true);
    sref.y.ToString(ry, sizeof(ry), true);

    OutputDebugStringA(("sin OUT: x=" + std::string(ox) + " y=" + oy + "\n").c_str());
    OutputDebugStringA(("sin REF: x=" + std::string(rx) + " y=" + ry + "\n").c_str());

    cout.x.ToString(ox, sizeof(ox), true);
    cout.y.ToString(oy, sizeof(oy), true);
    cref.x.ToString(rx, sizeof(rx), true);
    cref.y.ToString(ry, sizeof(ry), true);

    OutputDebugStringA(("cos OUT: x=" + std::string(ox) + " y=" + oy + "\n").c_str());
    OutputDebugStringA(("cos REF: x=" + std::string(rx) + " y=" + ry + "\n").c_str());

    }

#include <thread>
#include <vector>

void TestCSinWorker(int id, BigDouble pi)
    {
    CBigTrig trig;

    BigComplex in, sout, cout, sref, cref;

    // --- set precision ---
    in.x.ChangePrecision(256);
    in.y.ChangePrecision(256);
    sout.x.ChangePrecision(256);
    sout.y.ChangePrecision(256);
    cout.x.ChangePrecision(256);
    cout.y.ChangePrecision(256);
    sref.x.ChangePrecision(256);
    sref.y.ChangePrecision(256);
    cref.x.ChangePrecision(256);
    cref.y.ChangePrecision(256);

    // --- vary input per thread (important!) ---
    switch (id % 6)
	{
	case 0: in.x = 0.5;        in.y = 0.25; break;
	case 1: in.x = 1.0;        in.y = 0.0;  break;
	case 2: in.x = pi / 2;     in.y = 0.0;  break;
	case 3: in.x = pi;         in.y = 0.0;  break;
	case 4: in.x = pi * 2;     in.y = 0.0;  break;
	case 5: in.x = -pi * 3;    in.y = 0.0;  break;
	}

    // --- compute ---
    trig.CSin(&sout, in);
    trig.CCos(&cout, in);

    sref = in.CSin();
    cref = in.CCos();

    // --- compare ---
    if (mpfr_cmp(sout.x.x, sref.x.x) != 0 ||
	mpfr_cmp(sout.y.x, sref.y.x) != 0 ||
	mpfr_cmp(cout.x.x, cref.x.x) != 0 ||
	mpfr_cmp(cout.y.x, cref.y.x) != 0)
	{
	OutputDebugStringA(" MISMATCH DETECTED\n");
	}
    }

void TestMultiCSin()
    {
    const int NUM_THREADS = 8;

    BigDouble pi;
    pi.ChangePrecision(256);
    pi.SetPi();

    std::vector<std::thread> threads;

    for (int i = 0; i < NUM_THREADS; i++)
	{
	threads.emplace_back(TestCSinWorker, i, pi);
	}

    for (auto& t : threads)
	t.join();

    OutputDebugStringA(" Multi-thread test complete\n");
    }


void TestExpFractalSingle()
    {
    CBigTrig trig;

    BigComplex z, c;

    // precision
    z.x.ChangePrecision(256);
    z.y.ChangePrecision(256);
    c.x.ChangePrecision(256);
    c.y.ChangePrecision(256);

    // initial values
    z = 0.0;
    c.x = 0.3;
    c.y = 0.5;

    for (int i = 0; i < 50; i++)
	{
	BigComplex tmp;

	trig.CExp(&tmp, z);
	z = tmp + c;

	char zx[128], zy[128];
	z.x.ToString(zx, sizeof(zx), true);
	z.y.ToString(zy, sizeof(zy), true);

	OutputDebugStringA(("Iter " + std::to_string(i) +
	    ": z = " + zx + " , " + zy + "\n").c_str());
	}
    }

void TestExpFractalMulti()
    {
    const int NUM_THREADS = 8;

    std::vector<std::thread> threads;

    OutputDebugStringA("TestExpFractalMulti: launching threads\n");

    for (int t = 0; t < NUM_THREADS; t++)
	{
	threads.emplace_back([t]()
	    {
	    CBigTrig trig;

	    BigComplex z, c;

	    DWORD id = GetCurrentThreadId();

	    char buf[128];
	    sprintf_s(buf, "TestExp worker %d start\n", id);
	    OutputDebugStringA(buf);

	    z.x.ChangePrecision(256);
	    z.y.ChangePrecision(256);
	    c.x.ChangePrecision(256);
	    c.y.ChangePrecision(256);

	    z = 0.0;

	    // vary input per thread
	    c.x = 0.3 + t * 0.01;
	    c.y = 0.5 + t * 0.01;

	    for (int i = 0; i < 50; i++)
		{
		if (gStopRequested.load())
		    break;

		if (i % 10 == 0)
		    {
		    char buf[128];
		    sprintf_s(buf, "worker %lu iteration %d\n", id, i);
		    OutputDebugStringA(buf);
		    }


//		char buf2[128];
//		sprintf_s(buf2, "worker %lu iter %d before CExp\n", id, i);
//		OutputDebugStringA(buf2);

		BigComplex tmp;
		tmp.x.ChangePrecision(256);
		tmp.y.ChangePrecision(256);

		trig.CExp(&tmp, z);

//		sprintf_s(buf2, "worker %lu iter %d after CExp\n", id, i);
//		OutputDebugStringA(buf2);

		z = tmp + c;

//		sprintf_s(buf2, "worker %lu iter %d after add\n", id, i);
//		OutputDebugStringA(buf2);
		}
	    sprintf_s(buf, "TestExp worker %d end\n", id);
	    OutputDebugStringA(buf);
	    });
	}

    OutputDebugStringA("TestExpFractalMulti: waiting for threads\n");

    for (auto& th : threads)
	th.join();

    OutputDebugStringA("TestExpFractalMulti: threads finished waiting\n");
    OutputDebugStringA(" exp fractal multi-thread test complete\n");
    }



#include "DDTrig.h"

void TestDDTrig()
    {
    dd_real s[5], c[5], sh[5], ch[5], x[5], y[5], pi = dd_real::_pi;
    CDDTrig trig;
    dd_real s_ref[5], c_ref[5], sh_ref[5], ch_ref[5];
    char buf[512], buf1[512];

    x[0] = 0.0; y[0] = 0.0;
    x[1] = 1.0; y[1] = 0.0;
    x[2] = pi / 3; y[2] = 0.0;
    x[3] = 0.0, y[3] = 1.0;
    x[4] = 1.0, y[4] = 1.0;

    for (int i = 0; i < 5; i++)
	{
	trig.sincos_dd(&s[i], &c[i], x[i]);       // or whatever order your function claims
	trig.sinhcosh_dd(&sh[i], &ch[i], y[i]);

	s_ref[i] = sin(x[i]);
	c_ref[i] = cos(x[i]);
	sh_ref[i] = sinh(y[i]);
	ch_ref[i] = cosh(y[i]);

	sprintf(buf, "x = %f, y = %f, s = %f, c = %f, sh = %f, ch = %f\n", x[i].x[0], y[i].x[0], s[i].x[0], c[i].x[0], sh[i].x[0], ch[i].x[0]);
	sprintf(buf1, "Reference: s = %f, c = %f, sh = %f, ch = %f\n", s_ref[i].x[0], c_ref[i].x[0], sh_ref[i].x[0], ch_ref[i].x[0]);
	OutputDebugStringA(buf);
	OutputDebugStringA(buf1);
	}
    }
#endif // TEST_TRIG


/**************************************************************************
	Analyse Keyboard activity
**************************************************************************/

void	ProcessKeys(HWND hwnd, WPARAM wParam)

    {
    char	szTitleName[] = "ManpWIN";
    char	szSaveFileName[MAX_PATH];

    // key functions for writing MPEG files
    if (RunMPEG)									// we are currently writing MPEG file
	{
	if (wParam == VK_ESCAPE)
	    RunMPEG = FALSE;
	return;										// all other keys rejected at this stage
	}
    // key functions for writing GIF files
/*
    if (RunGIF)										// we are currently writing GIF file
	{
	if (wParam == VK_ESCAPE)
	    RunGIF = FALSE;
	return;										// all other keys rejected at this stage
	}
*/
    // key functions for animations
    if (DisplayAnimation)								// we are currently displaying an animation
	{
	switch (wParam)									// process any messages for animation
	    {
	    case ' ':									// single step
		if (!SuspendAnimation)
		    AnimateSuspend();
		StepFrame();
		return;
	    case 187: 									// + key cycle in
	    case 107: 									// + key cycle in
		IncreaseAnimSpeed();
		return;
	    case 189: 									// - key cycle out
	    case 109: 									// - key cycle out 
		DecreaseAnimSpeed();
		return;
	    case 'A':  									// write individual PNG files for each frame
		SaveIndividualFrames();                    
		return;
	    case 'G':  									// write animated GIF file
		if (SaveGIFOpenDlg (hwnd, szSaveFileName, szTitleName) >= 0)
		    SaveFile (hwnd, szSaveFileName, szTitleName);                    
		return;
	    case 'M':  									// write animated MPEG file
		if (SaveMPGOpenDlg (hwnd, szSaveFileName, szTitleName) >= 0)
		    SaveFile (hwnd, szSaveFileName, szTitleName);                    
		return;
	    case 'R':  									// reverse direction 
		ReverseAnimDirection();
		return;
	    case 'S':  									// suspend/resume
		SuspendAnimation = !SuspendAnimation;
		if (SuspendAnimation)
		    AnimateSuspend();
		else
		    AnimateResume();
		return;
	    case 'T':  									// new fractal
		time_to_break = TRUE;
		RunAnimation = FALSE;
		AnimateClose();
		SendMessage (hwnd, WM_COMMAND, IDC_FRACTYPE, 0L);
		return;
	    case VK_RETURN:								// Let's get out of here
	    case VK_ESCAPE:  								// stop animation run
//	    case VK_SHIFT:
		time_to_break = TRUE;
		RunAnimation = FALSE;
		AnimateClose();
		return;
	    }
	time_to_break = TRUE;
	AnimateClose();
	}
    switch (wParam) 
	{
	case VK_F1:									// Help    
	    PostMessage(hwnd, WM_COMMAND, IDM_HELP, 0L);
	    break;

	case VK_F5:									// Refresh    
	    DisplayStatusBarInfo(INCOMPLETE, "");
	    InvalidateRect(hwnd, NULL, FALSE);
	    break;

	case VK_UP:									// Scrolling messages    
	    PostMessage (hwnd, WM_VSCROLL, SB_LINEUP,   0L);
	    break;
    	
	case VK_DOWN:  
	    PostMessage (hwnd, WM_VSCROLL, SB_LINEDOWN, 0L);
	    break;
    	
	case VK_PRIOR: 
	    PostMessage (hwnd, WM_VSCROLL, SB_PAGEUP,   0L);
	    break;
    	
	case VK_NEXT:  
	    PostMessage (hwnd, WM_VSCROLL, SB_PAGEDOWN, 0L);
	    break;
    	
	case VK_HOME:  
	    PostMessage (hwnd, WM_HSCROLL, SB_PAGEUP,   0L);
	    break;
    	
	case VK_END:   
	    PostMessage (hwnd, WM_HSCROLL, SB_PAGEDOWN, 0L);
	    break;
    	
	case VK_LEFT:  
	    PostMessage (hwnd, WM_HSCROLL, SB_LINEUP,   0L);
	    break;
    	
	case VK_RIGHT: 
	    PostMessage (hwnd, WM_HSCROLL, SB_LINEDOWN, 0L);
	    break;
    	
	case VK_SPACE:  
	    if (GetKeyState(VK_SHIFT) < 0)
		SendMessage (hwnd, WM_COMMAND, IDM_SET_JULIA_SIZE, 0L);		// Real Time Julia
	    else if (GetKeyState(VK_CONTROL) < 0)
		SendMessage (hwnd, WM_COMMAND, IDM_REALTIMEJULIA, 0L);		// Real Time Julia
	    else
		PostMessage (hwnd, WM_COMMAND, IDM_JULIA, 0L);
	    break;
    	
	case 187: 								// + key cycle in
	case 107: 								// + key cycle in
	    if (GetKeyState(VK_CONTROL) < 0)
		SendMessage (hwnd, WM_COMMAND, IDM_STEP, 1L);
	    else
		SendMessage (hwnd, WM_COMMAND, IDM_CYCLE, 1L);
	    break;
    	
	case 189: 								// - key cycle in
	case 109: 								// - key cycle out 
	    //                case '-': 					// - key cycle out 
	    if (GetKeyState(VK_CONTROL) < 0)
		SendMessage (hwnd, WM_COMMAND, IDM_STEP, 0L);
	    else
		SendMessage (hwnd, WM_COMMAND, IDM_CYCLE, 0L);
	    break;
    	
	case VK_RETURN:								// Let's get out of here
	case VK_ESCAPE: 

	    if (GetKeyState(VK_SHIFT) >= 0)
		{
		if (RunAnimation)
		    time_to_break = TRUE;
		else
		    {
		    if (MessageBox (hwnd, "Do you want to Quit?", "ManpWin", MB_ICONEXCLAMATION | MB_DEFBUTTON2 | MB_YESNO) == IDYES)
			{
//			hWndCopy = hwnd;
			time_to_quit = TRUE;
			SendMessage (hwnd, WM_DESTROY, 0, 0L);
			}
		    }
		}
	    else
		{
//		hWndCopy = hwnd;
		if (RunAnimation)
		    time_to_break = TRUE;
		else
		    {
		    time_to_quit = TRUE;
		    SendMessage (hwnd, WM_DESTROY, 0, 0L);
		    }
		}
	    break;
	case '\t': 
	    SendMessage (hwnd, WM_COMMAND, IDM_FRACTLOC, 0L);
	    break;
    	
	case '1':								// Toggle Random Colour Generation
	    TrueCol.ToggleRandomColour();
	    SendMessage (hwnd, WM_COMMAND, IDM_COLOUR_PAL, 0L);
	    break;
	
	case '3': 
	    SendMessage (hwnd, WM_COMMAND, IDM_3D, 0L);
	    break;
    	
	case 'A':					 
//	    if (GetKeyState(VK_SHIFT) < 0)
//		SendMessage(hwnd, WM_COMMAND, IDM_SHOWOSCALLAXES, 0L);		// Show oscillators for all axes
	    if (GetKeyState(VK_CONTROL) < 0)
		{
		time_to_restart = TRUE;
		SendMessage (hwnd, WM_COMMAND, IDM_SETUPANIMATION, 0L);
		}
	    else 
		SendMessage (hwnd, WM_COMMAND, IDM_SAVEIMAGE, 0L);		// Save as file 
	    break;
    	
	case 'B':								// display location  
	    SendMessage (hwnd, WM_COMMAND, IDM_DISPLAYLOC, 0L);
	    break;
    	
	case 'C':  						
	    if (GetKeyState(VK_SHIFT) < 0)
		SendMessage (hwnd, WM_COMMAND, IDM_TIERAZON_COLOUR, 0L);	// Select Tierazon colour scheme
	    else if (GetKeyState(VK_CONTROL) < 0)
		SendMessage (hwnd, WM_COMMAND, IDM_COPY, 0L);			// Edit Colours 
	    else 
		SendMessage (hwnd, WM_COMMAND, IDM_COLOUR_PAL, 0L);
	    break;
    	
	case 'D':
	    if (GetKeyState(VK_SHIFT) < 0)
		SendMessage(hwnd, WM_COMMAND, IDM_STEREOPAIRSETUP, 0L); 	// setup stereo pairs
	    else
		SendMessage(hwnd, WM_COMMAND, IDM_3DPARAM, 0L);			// Set 3D params 
	    break;

	case 'E': 
	    if (GetKeyState(VK_CONTROL) < 0)
		SendMessage (hwnd, WM_COMMAND, IDM_EDITPAL, 0L);		// edit palette
	    else 
		SendMessage (hwnd, WM_COMMAND, IDM_EDGE, 0L);			// Edge scanning for Zoom 
	    break;
    	
	case 'F':  					
	    if (GetKeyState(VK_SHIFT) < 0)
		SendMessage (hwnd, WM_COMMAND, IDM_FOURIERANIM, 0L);		// Animate Fourier Analysis
	    else if (GetKeyState(VK_CONTROL) < 0)
		SendMessage (hwnd, WM_COMMAND, IDM_FRACTALCOORDS, 0L);		// Enter co-ordinates
	    else 
		SendMessage (hwnd, WM_COMMAND, IDM_FDOPTIONS, 0L);		// Tierazon filter FD-Options
	    break;
    	
	case 'G':					
	    if (GetKeyState(VK_CONTROL) < 0)					// Hailstone Dots toggle
		{
		extern COtherFunctions* g_pOtherFunctions;
		if (g_pOtherFunctions)
		    g_pOtherFunctions->ToggleShowDots();
		}
	    else
		SendMessage (hwnd, WM_COMMAND, IDM_OPEN_PNG, 0L);		// open PNG file  
	    break;
    	
	case 'H':								// Hailstone Axes toggle 
	    if (GetKeyState(VK_CONTROL) < 0)
		{
		extern COtherFunctions* g_pOtherFunctions;
		if (g_pOtherFunctions)
		    g_pOtherFunctions->ToggleShowAxes();
		}
	    break;

	case 'I':
	    if (GetKeyState(VK_SHIFT) < 0)
		SendMessage (hwnd, WM_COMMAND, IDM_INVERSION_ANIM, 0L);		// Inversion Animation
	    else
		SendMessage (hwnd, WM_COMMAND, IDM_OPEN_SCI, 0L);		// open SCI file
	    break;
    	
	case 'J':								// Julia animation  
	    SendMessage(hwnd, WM_COMMAND, IDM_JULIA_ANIM, 0L);
	    break;

	case 'K':								// Save/Load Kalles parameter file  
	    if (GetKeyState(VK_CONTROL) < 0)
		SendMessage(hwnd, WM_COMMAND, IDM_SAVE_KALLES, 0L);
	    else
		SendMessage(hwnd, WM_COMMAND, IDM_OPEN_KALLES, 0L);
	    break;

	case 'L':
	    if (GetKeyState(VK_CONTROL) < 0)					// Hailstone Labels toggle
		{
		extern COtherFunctions* g_pOtherFunctions;
		if (g_pOtherFunctions)
		    g_pOtherFunctions->ToggleShowPointLabels();
		}
	    else if (GetKeyState(VK_SHIFT) < 0)
		SendMessage (hwnd, WM_COMMAND, IDM_OPEN_LST, 0L);		// open PNG animation frame list
	    else 
		SendMessage (hwnd, WM_COMMAND, IDM_OPEN_TRUE_MAP, 0L);		// Open True Colour File  
	    break;
	
	case 'M':			
	    if (GetKeyState(VK_SHIFT) < 0)
		SendMessage (hwnd, WM_COMMAND, IDM_MORPHOSC, 0L);		// Morph Oscillator
	    else if (GetKeyState(VK_CONTROL) < 0)
		SendMessage(hwnd, WM_COMMAND, IDM_SETUPTHREADS, 0L);		// Set up threadiong info
	    break;
    	
	case 'N':						
	    if (GetKeyState(VK_CONTROL) < 0)
		SendMessage (hwnd, WM_COMMAND, IDM_SAVE_PAL_MAP, 0L);		// Save palette MAP file
	    else
		SendMessage (hwnd, WM_COMMAND, IDM_SAVE_TRUE_MAP, 0L);		// Save True Colour File 
	    break;
    	
	case 'O':  
	    if (GetKeyState(VK_CONTROL) < 0)
		SendMessage (hwnd, WM_COMMAND, IDM_OSCILLATOR_ANIMATION, 0L);	// Oscillator Animation
	    else
		SendMessage (hwnd, WM_COMMAND, IDM_OPEN_PAR, 0L);		// open Par file
	    break;
    	
	case 'P':
	    if (GetKeyState(VK_CONTROL) < 0)
		SendMessage (hwnd, WM_COMMAND, IDM_USEDEFAULTPALETTE, 0L);	// Use the default colour palette as per FRACTINT
	    else
		SendMessage (hwnd, WM_COMMAND, IDM_PARAM_ANIM, 0L);		// Parameter animation  
	    break;
    	
	case 'Q':								// Open Map File  
	    SendMessage (hwnd, WM_COMMAND, IDM_OPEN_PAL_MAP, 0L);
	    break;
    	
	case 'R':  
	    if (GetKeyState(VK_SHIFT) < 0)
		SendMessage (hwnd, WM_COMMAND, IDM_RDS_SPOTS, 0L);		// Random Dot Stereogram with guide spots
	    else if (GetKeyState(VK_CONTROL) < 0)
		SendMessage (hwnd, WM_COMMAND, IDM_GET_PIXEL_COLOUR, 0L);	// Get the colour of the pixel where mouse is clicked
	    else
		SendMessage (hwnd, WM_COMMAND, IDM_RDS, 0L); 			// Random Dot Stereogram
	    break;
    	
	case 'S': 
	    if (GetKeyState(VK_SHIFT) < 0)
		{
		if (GetKeyState(VK_CONTROL) < 0)
		    SendMessage(hwnd, WM_COMMAND, IDM_SAVE_PAR_AND_IMAGE, 0L);	// Save both parameter file and image with the same filename
		else
		    SendMessage(hwnd, WM_COMMAND, IDM_STOPANIM, 0L);		// Stop Animation
		}
	    else if (GetKeyState(VK_CONTROL) < 0)
		SendMessage (hwnd, WM_COMMAND, IDM_SETINSIDE, 0L);		// Change Inside colour
	    else 
		SendMessage (hwnd, WM_COMMAND, IDM_SAVEPAR, 0L);		// Save par file
	    break;
    	
	case 'T':								// Fractal Type  
	    time_to_break = (type == FOURIER);					// make sure that the fourier loop ends
	    if (OscAnimProc != STANDARD)
		{
		time_to_break = TRUE;
		RunAnimation = FALSE;
		AnimateClose();
		}
	    if (GetKeyState(VK_SHIFT) < 0)
		SendMessage(hwnd, WM_COMMAND, IDM_TOGGLE_DISPLAY_PALETTE, 0L);
	    else if (GetKeyState(VK_CONTROL) < 0)
		SendMessage(hwnd, WM_COMMAND, IDM_UPDATE_FRACTAL, 0L);
	    else
		SendMessage(hwnd, WM_COMMAND, IDC_FRACTYPE, 0L);
	    break;

	case 'U':								// Toggle Update Timer between 1000 and 10 milliseconds  
	    UpdateDelay = (UpdateDelay == 1000) ? 10 : 1000;
	    break;

#ifdef TEST_TRIG
	case 'V':								// a test for various trig
	    TestDDTrig();
//	    TestMultiThreadCSin();
	    break;
#endif // TEST_TRIG

	case 'W':  								// toggle between normal mandel deriviative and perturbation version 
	    SendMessage(hwnd, WM_COMMAND, IDM_TOGGLEPERT, 0L);
	    break;

	case 'X':  								// Fractal Options 
	    SendMessage(hwnd, WM_COMMAND, IDM_FRACOPTIONS, 0L);
	    break;

	case 'Y':								// Add spiral flag  
	    if (GetKeyState(VK_CONTROL) < 0)
		SendMessage (hwnd, WM_COMMAND, IDM_REDO, 0L);			// Redo
	    else 
		SendMessage (hwnd, WM_COMMAND, IDC_ADDSPIRAL, 0L);
	    break;
    	
	case 'Z':  
	    if (GetKeyState(VK_SHIFT) < 0)
		SendMessage (hwnd, WM_COMMAND, IDM_IMAGE_SIZE, 0L);		// image size
	    else if (GetKeyState(VK_CONTROL) < 0)
		SendMessage (hwnd, WM_COMMAND, IDM_UNDO, 0L);			// Undo
	    else 
		SendMessage (hwnd, WM_COMMAND, IDM_STEREO, 0L);			// Stereo Pairs
	    break;
	}
    }