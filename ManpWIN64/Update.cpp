#include <windows.h>
#include "manpwin.h"
#include "fractype.h"
#include "manp.h"
#include "CodeVersion.h"

static	UINT	UpdateTimerID;				// update timer ID
	WORD	UpdateDelay = 1000;			// delay in milliseconds

extern	void	DisplayStatusBarInfo(int, char *);
extern	BOOL	RunAnimation;				// are we in the middle of an animation run?
extern	WORD	type;					// M=mand, J=Julia 1,2,4->

#ifdef NEWCODE
extern	bool	PertImageComplete;			// kill timer when perturbation is complete
#endif // NEWCODE


//////////////////////////////////////////////////////////////////////////////////////////////////////
extern	HWND	GlobalHwnd;				// This is the main windows handle
//////////////////////////////////////////////////////////////////////////////////////////////////////

/**************************************************************************
	init screen update
**************************************************************************/

void	UpdateInit(void)

    {
    int	Delay = 10;

    if (UpdateTimerID != 0)
	UpdateTimerID = KillTimer (GlobalHwnd, UPDATETIMER);
    if ((UpdateTimerID = (UINT)SetTimer (GlobalHwnd, UPDATETIMER, Delay, NULL)) == 0)
	{
	MessageBox (GlobalHwnd, "Too many timers open for Animation!", "ManpWIN", MB_ICONEXCLAMATION | MB_OK);
	return;
	}
    }

/**************************************************************************
	Close update
**************************************************************************/

int	UpdateClose(void)

    {
#ifdef NEWCODE
    if (type == PERTURBATION && !PertImageComplete)
	    return 0;
#endif // NEWCODE

    if (UpdateTimerID != 0)
	KillTimer (GlobalHwnd, UPDATETIMER);
    return 0;
    }

/**************************************************************************
	Update 
**************************************************************************/

int	DoUpdate(void)

    {
    if (UpdateTimerID != 0)
	UpdateTimerID = KillTimer (GlobalHwnd, UPDATETIMER);
    if ((UpdateTimerID = (UINT)SetTimer (GlobalHwnd, UPDATETIMER, UpdateDelay, NULL)) == 0)
	{
	MessageBox (GlobalHwnd, "Too many timers open for Animation!", "ManpWIN", MB_ICONEXCLAMATION | MB_OK);
	return -1;
	}
    if (!RunAnimation)
	DisplayStatusBarInfo(INCOMPLETE, "");		// display status bar
    InvalidateRect(GlobalHwnd, NULL, FALSE);
    return 0;
    }

