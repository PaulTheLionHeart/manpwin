#include <windows.h>
#include "manpwin.h"
#include "fractype.h"
#include "manp.h"
#include "CodeVersion.h"

static	UINT	UpdateTimerID;				// update timer ID

#ifdef NEWCODE
extern	bool	PertImageComplete;			// kill timer when perturbation is complete
#endif // NEWCODE


//////////////////////////////////////////////////////////////////////////////////////////////////////
//extern	HWND	GlobalHwnd;				// This is the main windows handle
//////////////////////////////////////////////////////////////////////////////////////////////////////

/**************************************************************************
	init screen update
**************************************************************************/

void	CManp::UpdateInit(void)
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

int	CManp::UpdateClose(void)
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

int	CManp::DoUpdate(void)
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

