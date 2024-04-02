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

/**************************************************************************
	Analyse Keyboard activity
**************************************************************************/

void	ProcessKeys(HWND hwnd, UINT wParam)

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
    	
	case 'G':								// open PNG file  
	    SendMessage (hwnd, WM_COMMAND, IDM_OPEN_PNG, 0L);
	    break;
    	
	case 'I':  
	    if (GetKeyState(VK_SHIFT) < 0)
		SendMessage (hwnd, WM_COMMAND, IDM_INVERSION_ANIM, 0L);		// Inversion Animation
	    else if (GetKeyState(VK_CONTROL) < 0)
		SendMessage (hwnd, WM_COMMAND, IDM_IKEDAANIMATION, 0L);		// Ikeda Animation
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
	    if (GetKeyState(VK_SHIFT) < 0)
		SendMessage (hwnd, WM_COMMAND, IDM_OPEN_LST, 0L);		// open PNG animation frame list
	    else 
		SendMessage (hwnd, WM_COMMAND, IDM_OPEN_TRUE_MAP, 0L);		// Open True Colour File  
	    break;
	
	case 'M':			
	    if (GetKeyState(VK_SHIFT) < 0)
		SendMessage (hwnd, WM_COMMAND, IDM_MORPHOSC, 0L);		// Morph Oscillator
	    else if (GetKeyState(VK_CONTROL) < 0)
		SendMessage(hwnd, WM_COMMAND, IDM_SETUPTHREADS, 0L);		// Set up threadiong info
	    else
		SendMessage (hwnd, WM_COMMAND, IDM_MALTHUS_RATE_ANIMATION, 0L);	// Malthus animation  
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