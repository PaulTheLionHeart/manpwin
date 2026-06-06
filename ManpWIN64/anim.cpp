/*
    ANIM.CPP - animation functions.

    Written in Microsoft Visual C++ by Paul de Leeuw.
*/

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
#include "manp.h"
#include "SafeStrings.h"

extern	char	SaveFileOrig[];				// SaveAs filename base name
extern	BOOL	WritePNGFrames;				// write frames to PNG files
extern	int	write_png_file(HWND, char *, char *, char *); 

/**************************************************************************
	Allocate memory for all Animation Frames
**************************************************************************/

void	CManp::InitAnimationFrameStructure(int Frames)
    {
    ANIM.clear();
    ANIM.resize(Frames);   // Frames = the script’s total frame count
    TotalFrames = Frames;
    AnimFirstFrame = 0;
    AnimLastFrame = Frames - 1;

    CurrentFrame = AnimFirstFrame;

    AnimationForward = TRUE;

    DisplayAnimation = FALSE;
    RunAnimation = FALSE;   // if appropriate
    }

/**************************************************************************
	Rebuild Dib from Animation Frame
**************************************************************************/

bool	BuildDibFromAnimFrame(const AnimStruct& A, CDib& Dib)
    {
    // Create / resize the DIB
    gManp->WaitForAllThreadsToFinish();   // MUST be first
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


	{
	BYTE *p = Dib.DibPixels.data();

	int firstDifferent = 0;

	BYTE b0 = p[0];
	BYTE g0 = p[1];
	BYTE r0 = p[2];

	for (int k = 1; k < A.width * A.height; k++)
	    {
	    BYTE b = p[k * 3 + 0];
	    BYTE g = p[k * 3 + 1];
	    BYTE r = p[k * 3 + 2];

	    if (b != b0 || g != g0 || r != r0)
		{
		firstDifferent = k;
		break;
		}
	    }

	char s[240];

	sprintf(
	    s,
	    "[BUILD DIB] frame first=%d,%d,%d firstDifferent=%d\n",
	    b0, g0, r0, firstDifferent);

	OutputDebugStringA(s);
	}



    return true;
    }

/**************************************************************************
	Load individual Animation Frames
**************************************************************************/

void	CManp::LoadAnimationFrame(char *buf, char *FrameInfo, int Frame, double ParamValue, BOOL IsParamAnim, BOOL IsMorphAnim, double &delay)
    {
    if (Frame < 0 || Frame >= TotalFrames)
	{
	OutputDebugStringA("LoadAnimationFrame: ANIM array index out of scope\n");
	return;
	}

    AnimStruct& AnimFrame = ANIM[Frame];   // get existing frame

    if (gManp->time_to_quit)
	{
	#ifdef _DEBUG
	OutputDebugStringA("LoadAnimationFrame: User initiated time to quit\n");
	#endif
	return;
	}

    AnimFrame.width = gManp->Dib.DibWidth;
    AnimFrame.height = gManp->Dib.DibHeight;
    AnimFrame.bitsPerPixel = gManp->Dib.BitsPerPixel;
    AnimFrame.pitch = gManp->Dib.WidthBytes;			// important!
    AnimFrame.pixels.resize((size_t)AnimFrame.pitch * AnimFrame.height);

    for (int y = 0; y < AnimFrame.height; ++y)
	memcpy(&AnimFrame.pixels[y*AnimFrame.pitch], gManp->Dib.DibPixels.data() + y * gManp->Dib.WidthBytes, (size_t)AnimFrame.width * gManp->Dib.BitsPerPixel / 8);

    size_t len = strlen(buf);
    AnimFrame.animFrameData.resize(len + 1);
    memcpy(AnimFrame.animFrameData.data(), buf, len + 1);
    SAFE_SPRINTF(AnimFrame.FrameFilename, "%s%04d.png", SaveFileOrig, Frame);
    _snprintf_s(AnimFrame.FrameInfo, MAX_PATH, _TRUNCATE, "%s", FrameInfo);
//    strcpy(AnimFrame.FrameInfo, FrameInfo);
    AnimFrame.ParamValue = ParamValue;
    AnimFrame.IsParamAnim = IsParamAnim;
    AnimFrame.IsMorphAnim = IsMorphAnim;
    AnimFrame.DelayMultiplier = (gManp->OscAnimProc == MORPHING) ? delay : 1.0;
    //	ANIM.push_back(AnimFrame);
    TotalFrames = (int)ANIM.size();				// keep a count so we can close the frames if required
    AnimFrame.valid = true;
    }

/**************************************************************************
	init Animation
**************************************************************************/

void	CManp::AnimateInit(int Frames)
    {
    int	Delay = 10;

    TotalFrames = Frames;
    if (AnimTimerID != 0)
	AnimTimerID = KillTimer(GlobalHwnd, ANIMTIMER);
    if ((AnimTimerID = (UINT)SetTimer(GlobalHwnd, ANIMTIMER, Delay, NULL)) == 0)
	{
	MessageBox(GlobalHwnd, "Too many timers open for Animation!", "ManpWIN", MB_ICONEXCLAMATION | MB_OK);
	return;
	}
    CurrentFrame = AnimFirstFrame;
    gManp->DisplayAnimation = TRUE;
    }

/**************************************************************************
	Close animation frame memory
**************************************************************************/

int	CManp::AnimateClose(void)
    {
    if (AnimTimerID != 0)
	KillTimer (GlobalHwnd, ANIMTIMER);

    ANIM.clear();
    gManp->DisplayAnimation = FALSE;
    gManp->AutoSaveFlag = FALSE;				// all done
    return 0;
    }

/**************************************************************************
	Suspend animation
**************************************************************************/

int	CManp::AnimateSuspend(void)
    {
    if (AnimTimerID != 0)
	KillTimer (GlobalHwnd, ANIMTIMER);
    return 0;
    }

/**************************************************************************
	Resume animation
**************************************************************************/

int	CManp::AnimateResume(void)
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

int	CManp::DoAnimation(void)
    {
    char	s[2400];
    char	Name[6400];

    if (ANIM.empty())
	{
	OutputDebugStringA("DoAnimation: ANIM array is empty\n");
	return -1;
	}

    if (!ANIM[CurrentFrame].valid)
	{
	OutputDebugStringA("DoAnimation: ANIM frame is not initialised\n");
	return -1;
	}

    if (CurrentFrame < AnimFirstFrame || CurrentFrame > AnimLastFrame)
	{
	OutputDebugStringA("DoAnimation: ANIM frame index is out of range\n");
	return -1;
	}

    // Now it is SAFE
    const AnimStruct& A = ANIM[CurrentFrame];

    if (TotalFrames <= 0)
	{
	OutputDebugStringA("DoAnimation: We have no ANIM frames\n");
	return -1;
	}

    assert(CurrentFrame >= 0 && CurrentFrame < TotalFrames);

    gManp->CreateFractalName(FALSE, Name);
    _snprintf_s(s, 2400, _TRUNCATE, "<%d>of<%d>: Speed %3.1f frames per second, Info%s", CurrentFrame + 1, TotalFrames, 1000.0 / CurrentDelay, Name);
    SetWindowText(GlobalHwnd, s);				// Show formatted text in the caption bar
    if (ANIM[CurrentFrame].IsMorphAnim)				// we display exis info in status bar
//	strcpy(szStatus, ANIM[i].FrameInfo);
	_snprintf_s(gManp->szStatus, MAX_PATH, _TRUNCATE, "%s", ANIM[CurrentFrame].FrameInfo);
    else
	{
	if (ANIM[CurrentFrame].IsParamAnim)
	    _snprintf_s(gManp->szStatus, STATUSSIZE, _TRUNCATE, "Animation: Frame %d of %d, Param Value = %.12f", CurrentFrame + 1, TotalFrames, ANIM[CurrentFrame].ParamValue);
	else
	    _snprintf_s(gManp->szStatus, STATUSSIZE, _TRUNCATE, "Animation: Frame %d of %d", CurrentFrame + 1, TotalFrames);
	}

    // Deep copy the entire DIB (allocates and copies safely)
//    Dib = ANIM[i].animDIB;
    if (!BuildDibFromAnimFrame(ANIM[CurrentFrame], gManp->Dib))
	{
	OutputDebugStringA("DoAnimation: Unable to build Dib from ANIM frame\n");
	return -1;
	}

    // Now redraw
    InvalidateRect(GlobalHwnd, NULL, FALSE);
    UpdateWindow(GlobalHwnd);

    if (AnimTimerID != 0)
	AnimTimerID = KillTimer (GlobalHwnd, ANIMTIMER);
    if ((AnimTimerID = (UINT)SetTimer (GlobalHwnd, ANIMTIMER, (WORD)(CurrentDelay * ANIM[CurrentFrame].DelayMultiplier), NULL)) == 0)
	{
	MessageBox (GlobalHwnd, "Too many timers open for Animation!", "ManpWIN", MB_ICONEXCLAMATION | MB_OK);
	AnimateClose();
	return -1;
	}

    do
	{
	if (AnimationForward)
	    {
	    CurrentFrame++;
	    if (CurrentFrame > AnimLastFrame)
		CurrentFrame = Return2Start ? AnimLastFrame : AnimFirstFrame;
	    }
	else
	    {
	    CurrentFrame--;
	    if (CurrentFrame < AnimFirstFrame)
		CurrentFrame = Return2Start ? AnimFirstFrame : AnimLastFrame;
	    }

	// If we've looped all the way around without finding a valid frame, stop
	if (CurrentFrame == AnimFirstFrame && !ANIM[CurrentFrame].valid)
	    {
	    if (AnimTimerID != 0)
		AnimTimerID = KillTimer(GlobalHwnd, ANIMTIMER);
	    OutputDebugStringA("DoAnimation: No valid frames exist\n");
	    return -1;
	    }

	} while (!ANIM[CurrentFrame].valid);
    return 0;
    }

/**************************************************************************
	Change Anim Speed 
**************************************************************************/

void	CManp::IncreaseAnimSpeed(void)
    {
    CurrentDelay = CurrentDelay * 0.8333;
//    if (CurrentDelay < 1)
//	CurrentDelay = 1;
    }

void	CManp::DecreaseAnimSpeed(void)
    {
    CurrentDelay = CurrentDelay * 1.2;
    if (CurrentDelay < 1.0)
	CurrentDelay = 1.0;
    }

/**************************************************************************
	Change Anim Direction
**************************************************************************/

void	CManp::ReverseAnimDirection(void)
    {
    AnimationForward = !AnimationForward;
    }

/**************************************************************************
	Step to next frame
**************************************************************************/

void	CManp::StepFrame(void)
    {
    DoAnimation();
    AnimateSuspend();
    }

/*-----------------------------------------
	Parse script file
  -----------------------------------------*/

int	CManp::SaveIndividualFrames(void)
    {
    char	SaveFileName[MAX_PATH];				// individual SaveAs filename
    char	s[MAXLINE];

    for (size_t i = 0; i < ANIM.size(); i++)
	{
	SAFE_SPRINTF(s, "Writing Frame %d of %d. Filename = %s", (int)i + 1, TotalFrames, ANIM[i].FrameFilename);
	SetWindowText (GlobalHwnd, s);			// Show formatted text in the caption bar

	if (!BuildDibFromAnimFrame(ANIM[i], gManp->Dib))
	    return -1;
//	memcpy(Dib.DibPixels, ANIM[i].animDIB.DibPixels, Dib.Size());
	if (!ANIM[i].animFrameData.empty())
	    {
	    if (write_png_file(GlobalHwnd, ANIM[i].FrameFilename, "ManpWIN", ANIM[i].animFrameData.data()) < 0)
		{
		_snprintf_s(s, MAXLINE, _TRUNCATE, "Error: Could not write file: <%s>", SaveFileName);
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


