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
#include "Dib.h"
#include "Anim.h"
#include "manp.h"
//#include "OscProcess.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////
extern	HWND	GlobalHwnd;				// This is the main windows handle
extern	CDib	Dib;
//////////////////////////////////////////////////////////////////////////////////////////////////////

	struct	AnimStruct  ANIM[MAXANIM];
static	UINT	AnimTimerID;				// timer ID
	double	CurrentDelay = 20.0;			// delay in milliseconds
	BOOL	AnimationForward = TRUE;		// order of file frames
	int	TotalFrames;				// total number of frames
	int	CurrentFrame;				// used to increment to the next frame when the timer ticks
	BOOL	DisplayAnimation = FALSE;		// allow system to know that we are currently displaying an animation

extern	char	szStatus[];				// status bar text
extern	char	SaveFileOrig[];				// SaveAs filename base name
extern	BOOL	AutoSaveFlag;
extern	BOOL	WritePNGFrames;				// write frames to PNG files
extern	BOOL	Return2Start;				// do we want to go backwards to the start?
//extern	BOOL	DisplayAxisImages;			// display all axes in a single image
extern	int	time_to_quit;				// time to quit?

extern	int	write_png_file(HWND, char *, char *, char *); 
extern	void	CreateFractalName(BOOL, char *);	// create name string for fractal
extern	ProcessType	OscAnimProc;
//extern	COscProcess	OscProcess;

/**************************************************************************
	Load individual Animation Frames
**************************************************************************/

void	LoadAnimationFrame(char *buf, char *FrameInfo, int Frame, double ParamValue, BOOL IsParamAnim, BOOL IsMorphAnim)
    {
    if (Frame < MAXANIM)
	{
	if (time_to_quit)
	    return;
	ANIM[Frame].animDIB = Dib;
	if ((ANIM[Frame].animFrameData = new char[MAXDATALINE]) == NULL)
	    return;
	memcpy(ANIM[Frame].animFrameData, buf, MAXDATALINE);
	sprintf(ANIM[Frame].FrameFilename, "%s%04d.png", SaveFileOrig, Frame);
//	if (!DisplayAxisImages)
	    strcpy(ANIM[Frame].FrameInfo, FrameInfo);
	ANIM[Frame].ParamValue = ParamValue;
	ANIM[Frame].IsParamAnim = IsParamAnim;
	ANIM[Frame].IsMorphAnim = IsMorphAnim;
	if (OscAnimProc != MORPHING)
	    ANIM[Frame].DelayMultiplier = 1.0;
	TotalFrames = Frame;				// keep a count so we can close the frames if required
	}
    }

/**************************************************************************
	init Animation
**************************************************************************/

void	AnimateInit(int Frames)

    {
    int	Delay = 10;

    TotalFrames = Frames;
    if (AnimTimerID != 0)
	AnimTimerID = KillTimer (GlobalHwnd, ANIMTIMER);
    if ((AnimTimerID = (UINT)SetTimer (GlobalHwnd, ANIMTIMER, Delay, NULL)) == 0)
	{
	MessageBox (GlobalHwnd, "Too many timers open for Animation!", "ManpWIN", MB_ICONEXCLAMATION | MB_OK);
	return;
	}
    CurrentFrame = 0;
    DisplayAnimation = TRUE;
    }

/**************************************************************************
	Close animation frame memory
**************************************************************************/

int	AnimateClose(void)

    {
    int	i;
    if (AnimTimerID != 0)
	KillTimer (GlobalHwnd, ANIMTIMER);
    if (!WritePNGFrames)
	{
	for (i = 0; i < TotalFrames; ++i)	// release all frames
	    {
	    ANIM[i].animDIB.CloseDibPtrs();
	    if (ANIM[i].animFrameData)
		delete ANIM[i].animFrameData;
	    ANIM[i].animFrameData = NULL;
	    }
	}
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
    int	i;
    char	s[2400];
    char	Name[2400];

    if (TotalFrames <= 0)
	return -1;
    if (CurrentFrame >= TotalFrames)
	{
	if (Return2Start)
	    {
	    AnimationForward = !AnimationForward;
	    CurrentFrame = TotalFrames - 1;
	    }
	else
	    CurrentFrame = 0;				// for next time
	}
    if (CurrentFrame < 0)
	{
	if (Return2Start)
	    {
	    AnimationForward = !AnimationForward;
	    CurrentFrame = 0;				// for next time
	    }
	else
	    CurrentFrame = TotalFrames - 1;			// for next time
	}
    i = CurrentFrame;
    CreateFractalName(FALSE, Name);
    sprintf (s, "<%d>of<%d>: Speed %3.1f frames per second, Info%s", i + 1, TotalFrames, 1000.0 / CurrentDelay, Name);
    SetWindowText(GlobalHwnd, s);			// Show formatted text in the caption bar
    if (ANIM[i].IsMorphAnim)				// we display exis info in status bar
	strcpy(szStatus, ANIM[i].FrameInfo);
    else
	{
	if (ANIM[i].IsParamAnim)
	    sprintf(szStatus, "Animation: Frame %d of %d, Param Value = %.12f", i + 1, TotalFrames, ANIM[i].ParamValue);
	else
	    sprintf(szStatus, "Animation: Frame %d of %d", i + 1, TotalFrames);
	}

//    Dib = ANIM[i].animDIB;				// this causes a memory corruption. Not sure why. 
    if (ANIM[i].animDIB.DibPixels)
	memcpy(Dib.DibPixels, ANIM[i].animDIB.DibPixels, Dib.Size());
    InvalidateRect(GlobalHwnd, NULL, FALSE);

    if (AnimTimerID != 0)
	AnimTimerID = KillTimer (GlobalHwnd, ANIMTIMER);
    if ((AnimTimerID = (UINT)SetTimer (GlobalHwnd, ANIMTIMER, (WORD)(CurrentDelay * ANIM[i].DelayMultiplier), NULL)) == 0)
	{
	MessageBox (GlobalHwnd, "Too many timers open for Animation!", "ManpWIN", MB_ICONEXCLAMATION | MB_OK);
	AnimateClose();
	return -1;
	}
    if (AnimationForward)
	CurrentFrame++;
    else
	CurrentFrame--;
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
    int		i;

    for (i = 0; i < TotalFrames; i++)
	{
	sprintf(s, "Writing Frame %d of %d. Filename = %s", i + 1, TotalFrames, ANIM[i].FrameFilename);
	SetWindowText (GlobalHwnd, s);			// Show formatted text in the caption bar
	memcpy(Dib.DibPixels, ANIM[i].animDIB.DibPixels, Dib.Size());
	if (write_png_file(GlobalHwnd, ANIM[i].FrameFilename, "ManpWIN", ANIM[i].animFrameData) < 0)
	    {
	    wsprintf(s, "Error: Could not write file: <%s>", SaveFileName);
	    MessageBox (GlobalHwnd, s, "ManpWIN", MB_ICONEXCLAMATION | MB_OK);
	    MessageBeep (0);
	    return -1;
	    }
	}
    return 0;
    }


