// ANIM.CPP
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include <conio.h>
#include <string.h>
#include <dos.h>
#include <windows.h>
#include <fcntl.h>
#include <tchar.h>
#include <math.h>
#include <atomic>
#include <vector>
#include <thread>
#include <cassert>
#include "Dib.h"
#include "Anim.h"
#include "manp.h"
//#include "OscProcess.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////
extern	HWND	GlobalHwnd;				// This is the main windows handle
extern	CDib	Dib;
//////////////////////////////////////////////////////////////////////////////////////////////////////

std::vector<AnimStruct> ANIM;
static	UINT	AnimTimerID;				// timer ID
static	bool	gAnimTimerRunning = false;
	double	CurrentDelay = 20.0;			// delay in milliseconds
	BOOL	AnimationForward = TRUE;		// order of file frames
	int	gTotalFrames = 0;			// total number of frames
	int	gAnimFirstFrame = 0;
	int	gAnimLastFrame = -1;
	int	gCurrentFrame = 0;			// used to increment to the next frame when the timer ticks
	BOOL	DisplayAnimation = FALSE;		// allow system to know that we are currently displaying an animation

extern	char	szStatus[];				// status bar text
extern	char	SaveFileOrig[];				// SaveAs filename base name
extern	BOOL	AutoSaveFlag;
extern	BOOL	WritePNGFrames;				// write frames to PNG files
extern	BOOL	Return2Start;				// do we want to go backwards to the start?
//extern	BOOL	DisplayAxisImages;			// display all axes in a single image
extern	int	time_to_quit;				// time to quit?
//extern	std::atomic<bool>   gControllerRunning;

extern	int	write_png_file(HWND, char *, char *, char *); 
extern	void	CreateFractalName(BOOL, char *);	// create name string for fractal
extern	ProcessType	OscAnimProc;
//extern	COscProcess	OscProcess;

/**************************************************************************
	Allocate memory for all Animation Frames
**************************************************************************/

void	InitAnimationFrameStructure(int Frames)
    {
    ANIM.clear();
    ANIM.resize(Frames);   // Frames = the script’s total frame count
    gTotalFrames = Frames;
    }

/**************************************************************************
	Rebuild Dib from Animation Frame
**************************************************************************/

bool	BuildDibFromAnimFrame(const AnimStruct& A, CDib& Dib)
    {
    // Create / resize the DIB
    if (!Dib.InitDib(A.width, A.height, A.bitsPerPixel))
	return false;

    // Sanity checks
    if (A.bitsPerPixel != 24)
	return false;

    // Copy pixels row-by-row (respect pitch!)
    for (int y = 0; y < A.height; y++)
	{
	memcpy(Dib.DibPixels.data() + y * Dib.WidthBytes, A.pixels.data() + y * A.pitch, A.width * 3);
	}

    return true;
    }

/**************************************************************************
	Load individual Animation Frames
**************************************************************************/

void	LoadAnimationFrame(char *buf, char *FrameInfo, int Frame, double ParamValue, BOOL IsParamAnim, BOOL IsMorphAnim, double &delay)
    {
    if (Frame < 0 || Frame >= gTotalFrames)
	{
	OutputDebugStringA("LoadAnimationFrame: ANIM array index out of scope\n");
	return;
	}

    AnimStruct& AnimFrame = ANIM[Frame];   // get existing frame

    if (time_to_quit)
	{
	OutputDebugStringA("LoadAnimationFrame: User initiated time to quit\n");
	return;
	}

    AnimFrame.width = Dib.DibWidth;
    AnimFrame.height = Dib.DibHeight;
    AnimFrame.bitsPerPixel = Dib.BitsPerPixel;
    AnimFrame.pitch = Dib.WidthBytes;			// important!
    AnimFrame.pixels.resize((size_t)AnimFrame.pitch * AnimFrame.height);

    for (int y = 0; y < AnimFrame.height; ++y)
	memcpy(&AnimFrame.pixels[y*AnimFrame.pitch], Dib.DibPixels.data() + y * Dib.WidthBytes, (size_t)AnimFrame.width * Dib.BitsPerPixel / 8);

    size_t len = strlen(buf);
    AnimFrame.animFrameData.resize(len + 1);
    memcpy(AnimFrame.animFrameData.data(), buf, len + 1);
    sprintf(AnimFrame.FrameFilename, "%s%04d.png", SaveFileOrig, Frame);
    _snprintf_s(AnimFrame.FrameInfo, MAX_PATH, _TRUNCATE, "%s", FrameInfo);
//    strcpy(AnimFrame.FrameInfo, FrameInfo);
    AnimFrame.ParamValue = ParamValue;
    AnimFrame.IsParamAnim = IsParamAnim;
    AnimFrame.IsMorphAnim = IsMorphAnim;
    AnimFrame.DelayMultiplier = (OscAnimProc == MORPHING) ? delay : 1.0;
    //	ANIM.push_back(AnimFrame);
    gTotalFrames = (int)ANIM.size();				// keep a count so we can close the frames if required
    AnimFrame.valid = true;
    }

/**************************************************************************
	init Animation
**************************************************************************/

void	AnimateInit(int Frames)

    {
    int	Delay = 10;

    gTotalFrames = Frames;
    if (AnimTimerID != 0)
	AnimTimerID = KillTimer(GlobalHwnd, ANIMTIMER);
    if ((AnimTimerID = (UINT)SetTimer(GlobalHwnd, ANIMTIMER, Delay, NULL)) == 0)
	{
	MessageBox(GlobalHwnd, "Too many timers open for Animation!", "ManpWIN", MB_ICONEXCLAMATION | MB_OK);
	return;
	}
    gCurrentFrame = gAnimFirstFrame;
    DisplayAnimation = TRUE;
    }

/**************************************************************************
	Close animation frame memory
**************************************************************************/

int	AnimateClose(void)

    {
    if (AnimTimerID != 0)
	KillTimer (GlobalHwnd, ANIMTIMER);

    ANIM.clear();
    DisplayAnimation = FALSE;
    AutoSaveFlag = FALSE;				// all done
    return 0;
    }

/**************************************************************************
	Suspend animation
**************************************************************************/

int	AnimateSuspend(void)

    {
    if (AnimTimerID != 0)
	KillTimer (GlobalHwnd, ANIMTIMER);
    return 0;
    }

/**************************************************************************
	Resume animation
**************************************************************************/

int	AnimateResume(void)

    {
    if ((AnimTimerID = (UINT)SetTimer (GlobalHwnd, ANIMTIMER, (WORD)CurrentDelay, NULL)) == 0)
	{
	MessageBox (GlobalHwnd, "Too many timers open for Animation!", "ManpWIN", MB_ICONEXCLAMATION | MB_OK);
	AnimateClose();
	return -1;
	}
    return 0;
    }

/**************************************************************************
	Animate 
**************************************************************************/

int	DoAnimation(void)
    {
    char	s[2400];
    char	Name[2400];

    if (ANIM.empty())
	{
	OutputDebugStringA("DoAnimation: ANIM array is empty\n");
	return -1;
	}

    if (!ANIM[gCurrentFrame].valid)
	{
	OutputDebugStringA("DoAnimation: ANIM frame is not initialised\n");
	return -1;
	}

    if (gCurrentFrame < gAnimFirstFrame || gCurrentFrame > gAnimLastFrame)
	{
	OutputDebugStringA("DoAnimation: ANIM frame index is out of range\n");
	return -1;
	}

    // Now it is SAFE
    const AnimStruct& A = ANIM[gCurrentFrame];

    if (gTotalFrames <= 0)
	{
	OutputDebugStringA("DoAnimation: We have no ANIM frames\n");
	return -1;
	}

    assert(gCurrentFrame >= 0 && gCurrentFrame < gTotalFrames);

    CreateFractalName(FALSE, Name);
    sprintf (s, "<%d>of<%d>: Speed %3.1f frames per second, Info%s", gCurrentFrame + 1, gTotalFrames, 1000.0 / CurrentDelay, Name);
    SetWindowText(GlobalHwnd, s);				// Show formatted text in the caption bar
    if (ANIM[gCurrentFrame].IsMorphAnim)				// we display exis info in status bar
//	strcpy(szStatus, ANIM[i].FrameInfo);
	_snprintf_s(szStatus, MAX_PATH, _TRUNCATE, "%s", ANIM[gCurrentFrame].FrameInfo);
    else
	{
	if (ANIM[gCurrentFrame].IsParamAnim)
	    _snprintf_s(szStatus, STATUSSIZE, _TRUNCATE, "Animation: Frame %d of %d, Param Value = %.12f", gCurrentFrame + 1, gTotalFrames, ANIM[gCurrentFrame].ParamValue);
	else
	    _snprintf_s(szStatus, STATUSSIZE, _TRUNCATE, "Animation: Frame %d of %d", gCurrentFrame + 1, gTotalFrames);
	}

    // Deep copy the entire DIB (allocates and copies safely)
//    Dib = ANIM[i].animDIB;
    if (!BuildDibFromAnimFrame(ANIM[gCurrentFrame], Dib))
	{
	OutputDebugStringA("DoAnimation: Unable to build Dib from ANIM frame\n");
	return -1;
	}

    // Now redraw
    InvalidateRect(GlobalHwnd, NULL, FALSE);
    UpdateWindow(GlobalHwnd);

    if (AnimTimerID != 0)
	AnimTimerID = KillTimer (GlobalHwnd, ANIMTIMER);
    if ((AnimTimerID = (UINT)SetTimer (GlobalHwnd, ANIMTIMER, (WORD)(CurrentDelay * ANIM[gCurrentFrame].DelayMultiplier), NULL)) == 0)
	{
	MessageBox (GlobalHwnd, "Too many timers open for Animation!", "ManpWIN", MB_ICONEXCLAMATION | MB_OK);
	AnimateClose();
	return -1;
	}

/*
    if (AnimationForward)
	{
	gCurrentFrame++;
	if (gCurrentFrame > gAnimLastFrame)
	    gCurrentFrame = Return2Start ? gAnimLastFrame : gAnimFirstFrame;
	}
    else
	{
	gCurrentFrame--;
	if (gCurrentFrame < gAnimFirstFrame)
	    gCurrentFrame = Return2Start ? gAnimFirstFrame : gAnimLastFrame;
	}
*/

    do
	{
	if (AnimationForward)
	    {
	    gCurrentFrame++;
	    if (gCurrentFrame > gAnimLastFrame)
		gCurrentFrame = Return2Start ? gAnimLastFrame : gAnimFirstFrame;
	    }
	else
	    {
	    gCurrentFrame--;
	    if (gCurrentFrame < gAnimFirstFrame)
		gCurrentFrame = Return2Start ? gAnimFirstFrame : gAnimLastFrame;
	    }

	// If we've looped all the way around without finding a valid frame, stop
	if (gCurrentFrame == gAnimFirstFrame && !ANIM[gCurrentFrame].valid)
	    {
	    if (AnimTimerID != 0)
		AnimTimerID = KillTimer(GlobalHwnd, ANIMTIMER);
	    OutputDebugStringA("DoAnimation: No valid frames exist\n");
	    return -1;
	    }

	} while (!ANIM[gCurrentFrame].valid);
    return 0;
    }

/**************************************************************************
	Change Anim Speed 
**************************************************************************/

void	IncreaseAnimSpeed(void)
    {
    CurrentDelay = CurrentDelay * 0.8333;
//    if (CurrentDelay < 1)
//	CurrentDelay = 1;
    }

void	DecreaseAnimSpeed(void)
    {
    CurrentDelay = CurrentDelay * 1.2;
    if (CurrentDelay < 1.0)
	CurrentDelay = 1.0;
    }

/**************************************************************************
	Change Anim Direction
**************************************************************************/

void	ReverseAnimDirection(void)
    {
    AnimationForward = !AnimationForward;
    }

/**************************************************************************
	Step to next frame
**************************************************************************/

void	StepFrame(void)
    {
    DoAnimation();
    AnimateSuspend();
    }

/*-----------------------------------------
	Parse script file
  -----------------------------------------*/

int	SaveIndividualFrames(void)
    {
    char	SaveFileName[MAX_PATH];				// individual SaveAs filename
    char	s[MAXLINE];

    for (size_t i = 0; i < ANIM.size(); i++)
	{
	sprintf(s, "Writing Frame %d of %d. Filename = %s", (int)i + 1, gTotalFrames, ANIM[i].FrameFilename);
	SetWindowText (GlobalHwnd, s);			// Show formatted text in the caption bar

	if (!BuildDibFromAnimFrame(ANIM[i], Dib))
	    return -1;
//	memcpy(Dib.DibPixels, ANIM[i].animDIB.DibPixels, Dib.Size());
	if (!ANIM[i].animFrameData.empty())
	    {
	    if (write_png_file(GlobalHwnd, ANIM[i].FrameFilename, "ManpWIN", ANIM[i].animFrameData.data()) < 0)
		{
		wsprintf(s, "Error: Could not write file: <%s>", SaveFileName);
		MessageBox(GlobalHwnd, s, "ManpWIN", MB_ICONEXCLAMATION | MB_OK);
		MessageBeep(0);
		return -1;
		}
	    }
	else
	    OutputDebugStringA("SaveIndividualFrames(): ANIM[i].animFrameData is empty\n");
	}
    return 0;
    }


