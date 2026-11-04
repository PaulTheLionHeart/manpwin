/*
    Rings.cpp - Selection and parameter management for Rings fractals.

    Written in Microsoft Visual C++ by Paul de Leeuw.

    The Rings subtype is the zero-based index into RingSpecific[].
*/

#include <math.h>
#include "manp.h"
#include "resource.h"
#include "Fract.h"
#include "fractalp.h"

static int RingsPtr = 0;
static int RingsNum = 0;

/**************************************************************************
    setup_Rings()

    Count the available Rings formulas in RingSpecific[].
    The final database entry must have a NULL name.
**************************************************************************/

int setup_Rings(void)
    {
    if (!RingsNum)
        {
        while (RingSpecific[RingsNum].name)
            RingsNum++;
        }
    return 0;
    }

/**************************************************************************
    LoadRingsParams()

    Load the default parameters, bailout, maximum iterations
    and starting view for the selected Rings formula.
**************************************************************************/

void LoadRingsParams(void)
    {
    const int subtype = gManp->subtype;

    for (int i = 0; i < NUMPARAM; i++)
	gManp->param[i] = RingSpecific[subtype].paramvalue[i];

    gManp->rqlim = RingSpecific[subtype].rqlim;
    gManp->threshold = RingSpecific[subtype].maxIteration;

    gManp->hor = RingSpecific[subtype].xmin;
    gManp->vert = RingSpecific[subtype].ymin;
    gManp->mandel_width = RingSpecific[subtype].width;
    }

/**************************************************************************
    SelectRingsDlg()

    Display the available Rings formulas and update the subtype
    from the selected database entry.
**************************************************************************/

INT_PTR CALLBACK SelectRingsDlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
    {
    static int index;

    switch (message)
        {
        case WM_INITDIALOG:
            {
            SetDlgItemText(hDlg, ID_LISTTITLE, "Rings Fractals");
            for (int i = 0; i < RingsNum; i++)
                {
                SendDlgItemMessage(hDlg, IDM_LSYSTEM, LB_ADDSTRING, 0, (LPARAM)(LPSTR)RingSpecific[i].name);
                }
            SendDlgItemMessage(hDlg, IDM_LSYSTEM, LB_SETCURSEL, (WPARAM)RingsPtr, 0L);
            return TRUE;
            }

        case WM_COMMAND:
            {
            switch (LOWORD(wParam))
                {
                case IDOK:
                    {
                    index = (int)SendDlgItemMessage(hDlg, IDM_LSYSTEM, LB_GETCURSEL, 0, 0L);

                    if (index == LB_ERR)
                        {
                        MessageBox(hDlg, "No Choice selected", "Select From a List", MB_OK | MB_ICONEXCLAMATION);
                        return TRUE;
                        }

                    RingsPtr = index;
                    gManp->subtype = RingsPtr;

                    EndDialog(hDlg, TRUE);
                    return TRUE;
                    }

                case IDCANCEL:
                    {
                    EndDialog(hDlg, FALSE);
                    return TRUE;
                    }

                case IDM_LSYSTEM:
                    {
                    if (HIWORD(wParam) == LBN_DBLCLK)
                        {
                        index = (int)SendDlgItemMessage(hDlg, IDM_LSYSTEM, LB_GETCURSEL, 0, 0L);

                        if (index == LB_ERR)
                            {
                            MessageBox(hDlg, "No Choice selected", "Select From a List", MB_OK | MB_ICONEXCLAMATION);
                            return TRUE;
                            }

                        RingsPtr = index;
                        gManp->subtype = RingsPtr;
                        EndDialog(hDlg, TRUE);
                        return TRUE;
                        }

                    return TRUE;
                    }
                }
            break;
            }
        }

    return FALSE;
    }