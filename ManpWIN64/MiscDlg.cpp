/*
   MISCDLG.CPP a module for Fractals that are not bitmapped
   Written in Microsoft Visual C++ by Paul de Leeuw.
*/

#include <windowsx.h>
#include "OtherFunctions.h"
#include "resource.h"
#include "SafeStrings.h"

extern	char	LyapSequence[];		// hold the AB sequence for Lyapunov fractals
extern	CFract	Fractal;
extern	double	rqlim;
extern	WORD	type;			// M=mand, N=Newton etc
extern	int	subtype;

extern	char	*InitCond[];		// initial conditions for direct formula parsing 
extern	char	*DirectFormula[];	// formulae for direct formula parsing 
extern	int	FindInitCond(char *Str);
extern	int	FindFormula(char *Str);

extern	PAINTSTRUCT 	ps;
extern	COtherFunctions	OthFn;

/**************************************************************************
	Dialog Control for Plasma Type
**************************************************************************/

INT_PTR CALLBACK PlasmaDlg (HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
     {
     static	HANDLE	hCursor;
     char		s[100];
//     BOOL		bTrans ;
     static	UINT	tempPoints; 
     static     char	temp[20];
//     static     char	temp;
     static     UINT	tempParam;

     switch (message)
	  {
	  case WM_INITDIALOG:
	      SAFE_SPRINTF(s, "%2.2f", param[0]);
		SetDlgItemText(hDlg, IDC_PLASMA_GRAIN, s);
	        return TRUE ;

	  case WM_COMMAND:
	        switch ((int) LOWORD(wParam))
//	        switch (wParam)
		    {
		    case IDC_PLASMA_GRAIN:
			GetDlgItemText(hDlg, IDC_PLASMA_GRAIN, temp, 20);
		        return TRUE ;

		    case IDOK:
			sscanf(temp, "%lf", &param[0]);
			if (param[0] < 0.1 || param[0] > 50.0)
			    param[0] = 2.0;
			EndDialog (hDlg, TRUE);
			return TRUE;

		    case IDCANCEL:
			EndDialog (hDlg, FALSE);
			return FALSE;
		   }
	  break;
	  }
      return FALSE ;
      }

/**************************************************************************
	Dialog Control for FrothyBasin Type
**************************************************************************/

INT_PTR CALLBACK FrothDlg (HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
     {
//     HWND		hCtrl ;
     static	HANDLE	hCursor;
//     short		nCtrlID, filterSum, i, divisor; 
     BOOL		bTrans ;
     static	UINT	tempShading; 
     static     UINT	tempAttractors;
     static     UINT	tempParam;

     switch (message)
	  {
	  case WM_INITDIALOG:
		SetDlgItemInt(hDlg, IDC_3_OR_6, (int)param[0], TRUE);
		SetDlgItemInt(hDlg, IDC_SHADING, (int)param[1], TRUE);
	        return TRUE ;

	  case WM_COMMAND:
	        switch ((int) LOWORD(wParam))
//	        switch (wParam)
		    {
		    case IDC_3_OR_6:
			tempAttractors = GetDlgItemInt(hDlg, IDC_3_OR_6, &bTrans, TRUE);
		        return TRUE ;

		    case IDC_SHADING:
			tempShading = GetDlgItemInt(hDlg, IDC_SHADING, &bTrans, TRUE);
		        return TRUE ;

		    case IDOK:
			param[0] = (double)tempAttractors;
			if (param[0] != 6.0)		// only 3 or 6 allowed
			    param[0] = 3.0;
			param[1] = (double)tempShading;
			EndDialog (hDlg, TRUE);
			return TRUE;

		    case IDCANCEL:
			EndDialog (hDlg, FALSE);
			return FALSE;
		   }
	  break;
	  }
      return FALSE ;
      }

/**************************************************************************
	Dialog Control for Cellular Automata Type
**************************************************************************/

INT_PTR CALLBACK CellularDlg (HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
     {
//     HWND		hCtrl ;
     static	HANDLE	hCursor;
//     short		nCtrlID, filterSum, i, divisor; 
     char		s[100];
     BOOL		bTrans ;
     static	UINT	tempInit; 
     static     char	temp[20];
     static     UINT	tempType;
     static     UINT	tempStart;
     static     UINT	tempParam;

     switch (message)
	  {
	  case WM_INITDIALOG:
		SAFE_SPRINTF(s, "%2.0f", param[1]);
		SetDlgItemText(hDlg, IDC_CELL_RULE, s);
		SetDlgItemInt(hDlg, IDC_INIT_STRING, (int)param[0], TRUE);
		SetDlgItemInt(hDlg, IDC_CELL_TYPE, (int)param[2], TRUE);
		SetDlgItemInt(hDlg, IDC_START_ROW, (int)param[3], TRUE);
	        return TRUE ;

	  case WM_COMMAND:
	        switch ((int) LOWORD(wParam))
//	        switch (wParam)
		    {
		    case IDC_CELL_RULE:
			GetDlgItemText(hDlg, IDC_CELL_RULE, temp, 20);
		        return TRUE ;

		    case IDC_INIT_STRING:
			tempInit = GetDlgItemInt(hDlg, IDC_INIT_STRING, &bTrans, TRUE);
		        return TRUE ;

		    case IDC_START_ROW:
			tempStart = GetDlgItemInt(hDlg, IDC_START_ROW, &bTrans, TRUE);
		        return TRUE ;

		    case IDC_CELL_TYPE:
			tempType = GetDlgItemInt(hDlg, IDC_CELL_TYPE, &bTrans, TRUE);
		        return TRUE ;

		    case IDOK:
			sscanf(temp, "%lf", &param[1]);
			param[0] = (double)tempInit;
			param[2] = (double)tempType;
			param[3] = (double)tempStart;
			EndDialog (hDlg, TRUE);
			return TRUE;

		    case IDCANCEL:
			EndDialog (hDlg, FALSE);
			return FALSE;
		   }
	  break;
	  }
      return FALSE ;
      }

/**************************************************************************
	Dialog Control for Walk Type
**************************************************************************/

INT_PTR CALLBACK WalkDlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
    {
    //     static	HANDLE	hCursor;
    char		s[100];
    //     BOOL		bTrans ;
    static	UINT	tempPoints;
    static     char	temp[20];
    //     static     char	temp;
    static     UINT	tempParam;

    switch (message)
	{
	case WM_INITDIALOG:
	    SAFE_SPRINTF(s, "%3.2f", param[0]);
	    SetDlgItemText(hDlg, IDC_WALK_STEPSIZE, s);
	    return TRUE;

	case WM_COMMAND:
	    switch ((int)LOWORD(wParam))
		//	        switch (wParam)
		{
		case IDC_WALK_STEPSIZE:
		    GetDlgItemText(hDlg, IDC_WALK_STEPSIZE, temp, 20);
		    return TRUE;

		case IDOK:
		    sscanf(temp, "%lf", &param[0]);
		    if (param[0] != 999.0)
			if (param[0] < 0.1 || param[0] > 100.0)
			    param[0] = 1.0;
		    EndDialog(hDlg, TRUE);
		    return TRUE;

		case IDCANCEL:
		    EndDialog(hDlg, FALSE);
		    return FALSE;
		}
	    break;
	}
    return FALSE;
    }

/**************************************************************************
	Dialog Box for loading functions
**************************************************************************/

INT_PTR CALLBACK LyapDlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)

    {
    int i, j;
    static	int	index1 = -1, index2 = -1;
    static	int	numtrig;
    char		s[10][100];
    char		Bailout[120];
    CTrigFn		TrigFn;

    switch (message) 
	{
        case WM_INITDIALOG:
	    SAFE_SPRINTF(Bailout, "%14.14lf", rqlim);
	    SetDlgItemText(hDlg, IDC_BAILOUT, Bailout);
//            SetDlgItemText(hDlg, ID_FRACNAME, 	fractalspecific[type].name);
            for (j = 0; j < Fractal.NumParam; j++) 
		{
		SAFE_SPRINTF(s[j], "%f", *Fractal.ParamValue[j]);
		SetDlgItemText(hDlg, ID_FRACPARTX1 + j, Fractal.ParamName[j]);
		SetDlgItemText(hDlg, ID_FRACPARAM1 + j, s[j]);
		}
            for (i = Fractal.NumFunct + Fractal.NumParam; i < 10; i++) 
		SetDlgItemText(hDlg, ID_FRACPARTX1 + i, "     N/A");
	    SetDlgItemText(hDlg, IDC_LYAPSEQ, LyapSequence);	// we cheat and use Fractal.Fn1 to store the Lyapunov sequence
	    return ( TRUE);

        case WM_COMMAND:
	    switch ((int) LOWORD(wParam))
//	    switch (wParam)
		{
                case IDOK:
		    GetDlgItemText(hDlg, IDC_BAILOUT, Bailout, 100);
		    rqlim = atof(Bailout);
		    for (j = 0; j < Fractal.NumParam; j++) 
			{
			GetDlgItemText(hDlg, ID_FRACPARAM1 + j, s[j], 100);
			*Fractal.ParamValue[j] = atof(s[j]);
			}
		    GetDlgItemText(hDlg, IDC_LYAPSEQ, LyapSequence, 100);	// we cheat and use Fractal.Fn1 to store the Lyapunov sequence
		    EndDialog(hDlg, TRUE);
                    return ( TRUE);
                  
                case IDCANCEL:
                    EndDialog(hDlg, FALSE);
                    return (FALSE);
                    
//		    return ( TRUE);
	    return ( TRUE);
	    }
	}
    return (FALSE);
    }

/**************************************************************************
	Dialog Box for screen formulae
**************************************************************************/


char	FormulaString[MAXFORMULASTRINGLENGTH] = "\0";		// used to hold the full formula
extern	char	*str_find_ci(char *, char *);

void	AnalyseFormulaString(char *Startup, char *OrigStartup, char *Formula, char *Bailout, char *FormulaString)

    {
    char    *p, *s, *t, *u;
    size_t  length = strlen(FormulaString);

    char    *TempStr = new char [length + 1];
    
    strcpy(TempStr, FormulaString);			// protect original string
    p = TempStr;

    u = str_find_ci(TempStr, ";");			// is it a comment?
    if (u)
	*u = '\0';					// remove comment
    s = str_find_ci(TempStr, ":");			// possible initial condition
    if (s)						// initial condition is given in the string and it's not part of a comment
	{
	*(s - 1) = '\0';				// replace the ':' with a NULL
	strcpy(Startup, p);
	p = s;
	}
    else 
	strcpy(Startup, OrigStartup);

    if ((t = str_find_ci(p, "|")) != 0)			// bailout is given in the string
	{ 
	*(t - 2) = '\0';				// t - 2 so we keep the '|'
	strcpy(Formula, p);
	p = t - 1;
	strcpy(Bailout, p);
	}
    else
	{
	strcpy(Formula, p);
	strcpy(Bailout, "|z| < 4.0");
	}
    
    if (TempStr) { delete[] TempStr; TempStr = NULL; }
    }

void	CreateFormulaString(char *Startup, char *Formula, char *Bailout, char *FormulaString)

    {
    char    *p, *q;
    char    temp[MAXFORMULASTRINGLENGTH];

    SAFE_SPRINTF(temp, "%s:%s,%s", Startup, Formula, Bailout);
    p = temp;
    q = FormulaString;

    while (*p)
	{
	if (*p == '\n')
	    {
	    *q = ',';
	    p++; q++;
	    }
	else if(*p == '\r')
	    p++;			// remove linefeeds
	else
	    { 
	    *q = *p; 
	    p++; q++;
	    }
	}
    *q = '\0';
    }

INT_PTR CALLBACK ScrnFormDlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)

    {
    int			i;
    static	int	index = -1;
    static	int	index1 = -1;
    static	char	Formula[1200] = "z = z*z + c + p1";
    static	char	Bailout[120] = "|z| < 4.0";
    static	char	Startup[1200] = "z=c=pixel";
    static	int	OldIndex1;				// used to check if formula has been edited. If it doesn't change, then it is either a repeat or a change in the current formula

    switch (message) 
	{
        case WM_INITDIALOG:
	    for (i = 0; InitCond[i]; i++)
		SendDlgItemMessage(hDlg, IDC_INITIAL, LB_ADDSTRING, (WPARAM)NULL, (LPARAM) (LPSTR) InitCond[i]);
	    if (index == -1 || *Startup != '\0')		// index not loaded, but have a default
		index = FindInitCond(Startup);
	    for (i = 0; DirectFormula[i]; i++)
		SendDlgItemMessage(hDlg, IDC_FORMULA, LB_ADDSTRING, (WPARAM)NULL, (LPARAM) (LPSTR) DirectFormula[i]);
	    if (index1 == -1 || *Formula != '\0')		// index not loaded, but have a default
		if (OldIndex1 != index1)			// it won't be found if the formula has been edited
		    index1 = FindFormula(Formula);
	    if (*FormulaString == '\0')				// allow the user to choose one of the formulae from the list
		OldIndex1 = index1;
	    else						// if we already hgave a formula loaded, then we present this to the user
		{
		AnalyseFormulaString(Startup, "", Formula, Bailout, FormulaString);
		OldIndex1 = index1 = -1;
		}
	    SetDlgItemText(hDlg, IDC_BAILOUT, Bailout);
	    SetDlgItemText(hDlg, IDC_FORMULAVALUE, Formula);
	    SetDlgItemText(hDlg, IDC_INITIALVALUE, Startup);
	    return ( TRUE);

        case WM_COMMAND:
	    switch ((int) LOWORD(wParam))
//	    switch (wParam)
		{
                case IDOK:
		    GetDlgItemText(hDlg, IDC_FORMULAVALUE, Formula, 1200);
		    GetDlgItemText(hDlg, IDC_BAILOUT, Bailout, 100);
		    GetDlgItemText(hDlg, IDC_INITIALVALUE, Startup, 1200);
		    CreateFormulaString(Startup, Formula, Bailout, FormulaString);
		    EndDialog(hDlg, TRUE);
                    return ( TRUE);
                  
                case IDCANCEL:
                    EndDialog(hDlg, FALSE);
                    return (FALSE);

                case IDC_FORMULA:
		    index1 = (int)SendDlgItemMessage(hDlg, IDC_FORMULA, LB_GETCURSEL, 0, 0L);
		    if (index1 == LB_ERR) 
			{
			index1 = 0;
			break;
			}
		    index = (int)SendDlgItemMessage(hDlg, IDC_INITIAL, LB_GETCURSEL, 0, 0L);
		    if (index < 0) index = 1;
		    if (*DirectFormula[index1] != '\0')
			if (OldIndex1 != index1)			// don't overwrite an edited formula
			    AnalyseFormulaString(Startup, InitCond[index], Formula, Bailout, DirectFormula[index1]);
		    SetDlgItemText(hDlg, IDC_BAILOUT, Bailout);
		    SetDlgItemText(hDlg, IDC_FORMULAVALUE, Formula);
		    SetDlgItemText(hDlg, IDC_INITIALVALUE, Startup);
		    return ( TRUE);
                case IDC_INITIAL:
		    index = (int)SendDlgItemMessage(hDlg, IDC_INITIAL, LB_GETCURSEL, 0, 0L);
		    if (index == LB_ERR) 
			{
			index1 = 0;
			break;
			}
		    SetDlgItemText(hDlg, IDC_INITIALVALUE, InitCond[index]);
		    strcpy(Startup, InitCond[index]);
//		    return ( TRUE);
	    return ( TRUE);
	    }
	}
    return (FALSE);
    }

/**************************************************************************
    Dialog Control for Triangles
**************************************************************************/

INT_PTR CALLBACK NumTriangleDlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
    {
    static     char	temp;
    static     UINT	tempParam;
    static     WORD	temp_special;
    BOOL		bTrans;
    //     char		s[24];

    switch (message)
	{
	case WM_INITDIALOG:
	    temp = subtype;
	    switch (subtype)
		{
		case 'A':
		    tempParam = IDC_A;
		    break;
		case 'B':
		    tempParam = IDC_B;
		    break;
		case 'C':
		    tempParam = IDC_C;
		    break;
		case 'D':
		    tempParam = IDC_D;
		    break;
		case 'E':
		    tempParam = IDC_E;
		    break;
		case 'F':
		    tempParam = IDC_F;
		    break;
		case 'G':
		    tempParam = IDC_G;
		    break;
		case 'H':
		    tempParam = IDC_H;
		    break;
		case 'I':
		    tempParam = IDC_I;
		    break;
		case 'J':
		    tempParam = IDC_J;
		    break;
		default:				// uninitialised
		    tempParam = IDC_A;
		    temp = 'A';
		    break;
		}

	    CheckRadioButton(hDlg, IDC_A, IDC_J, tempParam);
	    SetDlgItemInt(hDlg, IDC_PARAM1, 100, TRUE);
	    SetDlgItemInt(hDlg, IDC_PARAM2, 11, TRUE);
	    SetDlgItemInt(hDlg, IDC_PARAM3, 4, TRUE);
	    SetDlgItemInt(hDlg, IDC_PARAM4, 160, TRUE);
	    SetDlgItemInt(hDlg, IDC_PARAM5, 8, TRUE);
	    SetFocus(GetDlgItem(hDlg, tempParam));
	    return FALSE;

	case WM_COMMAND:
	    switch ((int)LOWORD(wParam))
		{
		case IDC_A:
		case IDC_B:
		case IDC_C:
		case IDC_D:
		case IDC_E:
		case IDC_F:
		case IDC_G:
		case IDC_H:
		case IDC_I:
		case IDC_J:
		    switch ((int)LOWORD(wParam))
			{
			case IDC_A:
			    temp = 'A';
			    break;
			case IDC_B:
			    temp = 'B';
			    break;
			case IDC_C:
			    temp = 'C';
			    break;
			case IDC_D:
			    temp = 'D';
			    break;
			case IDC_E:
			    temp = 'E';
			    break;
			case IDC_F:
			    temp = 'F';
			    break;
			case IDC_G:
			    temp = 'G';
			    break;
			case IDC_H:
			    temp = 'H';
			    break;
			case IDC_I:
			    temp = 'I';
			    break;
			case IDC_J:
			    temp = 'J';
			    break;
			}

		    CheckRadioButton(hDlg, IDC_A, IDC_J, (int)LOWORD(wParam));
		    return TRUE;

		case IDOK:
		    OthFn.GetNumberIterations(GetDlgItemInt(hDlg, IDC_PARAM1, &bTrans, TRUE));
		    OthFn.Getmoda(GetDlgItemInt(hDlg, IDC_PARAM2, &bTrans, TRUE));
		    OthFn.Getlo(GetDlgItemInt(hDlg, IDC_PARAM3, &bTrans, TRUE));
		    OthFn.Gethi(GetDlgItemInt(hDlg, IDC_PARAM4, &bTrans, TRUE));
		    OthFn.GetCircleSize(GetDlgItemInt(hDlg, IDC_PARAM5, &bTrans, TRUE));
		    subtype = temp;
		    EndDialog(hDlg, TRUE);
		    return TRUE;

		case IDCANCEL:
		    EndDialog(hDlg, FALSE);
		    return FALSE;
		}
	    break;
	}
    return FALSE;
    }

/*---------------------------------------------------------------------
	Circle Fractal Type dialogue box
  -------------------------------------------------------------------*/

INT_PTR CALLBACK CircleDlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
    {
    static     char	temp;
    static     UINT	tempParam, tempParam1;
    static     WORD	temp_special;
    HWND		hCtrl;
    BOOL		bTrans;
    int		FilledCircle;
    //     char		s[24];

    switch (message)
	{
	case WM_INITDIALOG:
	    temp = subtype;
	    switch (subtype)
		{
		case 'A':
		    tempParam = IDC_A;
		    break;
		case 'B':
		    tempParam = IDC_B;
		    break;
		case 'C':
		    tempParam = IDC_C;
		    break;
		case 'D':
		    tempParam = IDC_D;
		    break;
		case 'E':
		    tempParam = IDC_E;
		    break;
		case 'F':
		    tempParam = IDC_F;
		    break;
		case 'G':
		    tempParam = IDC_G;
		    break;
		case 'H':
		    tempParam = IDC_H;
		    break;
		case 'I':
		    tempParam = IDC_I;
		    break;
		case 'J':
		    tempParam = IDC_J;
		    break;
		case 'K':
		    tempParam = IDC_K;
		    break;
		case 'L':
		    tempParam = IDC_L;
		    break;
		case 'M':
		    tempParam = IDC_M;
		    break;
		    /*
					case 'N':
					    tempParam = IDC_N;
					    break;
					case 'O':
					    tempParam = IDC_O;
					    break;
					case 'P':
					    tempParam = IDC_P;
					    break;
					case 'Q':
					    tempParam = IDC_Q;
					    break;
					case 'R':
					    tempParam = IDC_R;
					    break;
					case 'S':
					    tempParam = IDC_S;
					    break;
					case 'T':
					    tempParam = IDC_T;
					    break;
		    */
		default:				// uninitialised
		    tempParam = IDC_A;
		    temp = 'A';
		    break;
		}
	    CheckRadioButton(hDlg, IDC_A, IDC_M, tempParam);
	    SetDlgItemInt(hDlg, IDC_PARAM1, (int)10, TRUE);
	    hCtrl = GetDlgItem(hDlg, IDC_DEFAULTPALETTE);
	    SendMessage(hCtrl, BM_SETCHECK, TRUE, 0L);
	    /*
			    switch (OthFn.FilledCircle)
				{
				case 1:
				    tempParam1 = IDC_FILLED;
				    break;
				case 2:
				    tempParam1 = IDC_UNFILLED;
				    break;
				case 3:
				    tempParam1 = IDC_3D;
				    break;
				}
	    */
	    OthFn.GetFilledCircle(3);
	    tempParam1 = IDC_3D;
	    CheckRadioButton(hDlg, IDC_FILLED, IDC_3D, tempParam1);
	    //		SetFocus(GetDlgItem(hDlg, tempParam));
	    return TRUE;

	case WM_COMMAND:
	    switch ((int)LOWORD(wParam))
		{
		case IDC_A:
		case IDC_B:
		case IDC_C:
		case IDC_D:
		case IDC_E:
		case IDC_F:
		case IDC_G:
		case IDC_H:
		case IDC_I:
		case IDC_J:
		case IDC_K:
		case IDC_L:
		case IDC_M:
		case IDC_N:
		case IDC_O:
		case IDC_P:
		case IDC_Q:
		case IDC_R:
		case IDC_S:
		case IDC_T:
		    switch ((int)LOWORD(wParam))
			{
			case IDC_A:
			    temp = 'A';
			    break;
			case IDC_B:
			    temp = 'B';
			    break;
			case IDC_C:
			    temp = 'C';
			    break;
			case IDC_D:
			    temp = 'D';
			    break;
			case IDC_E:
			    temp = 'E';
			    break;
			case IDC_F:
			    temp = 'F';
			    break;
			case IDC_G:
			    temp = 'G';
			    break;
			case IDC_H:
			    temp = 'H';
			    break;
			case IDC_I:
			    temp = 'I';
			    break;
			case IDC_J:
			    temp = 'J';
			    break;
			case IDC_K:
			    temp = 'K';
			    break;
			case IDC_L:
			    temp = 'L';
			    break;
			case IDC_M:
			    temp = 'M';
			    break;
			    /*
							case IDC_N:
							    temp = 'N';
							    break;
							case IDC_O:
							    temp = 'O';
							    break;
							case IDC_P:
							    temp = 'P';
							    break;
							case IDC_Q:
							    temp = 'Q';
							    break;
							case IDC_R:
							    temp = 'R';
							    break;
							case IDC_S:
							    temp = 'S';
							    break;
							case IDC_T:
							    temp = 'T';
							    break;
			    */
			}

		    CheckRadioButton(hDlg, IDC_A, IDC_M, (int)LOWORD(wParam));
		    return TRUE;

		case IDC_FILLED:
		case IDC_UNFILLED:
		case IDC_3D:
		    switch ((int)LOWORD(wParam))
			{
			case IDC_FILLED:
			    FilledCircle = 1;
			    break;
			case IDC_UNFILLED:
			    FilledCircle = 2;
			    break;
			case IDC_3D:
			    FilledCircle = 3;
			    break;
			}
		    CheckRadioButton(hDlg, IDC_FILLED, IDC_3D, (int)LOWORD(wParam));
		    OthFn.GetFilledCircle(FilledCircle);
		    //			InvalidateRect(hDlg, NULL, FALSE);
		    return TRUE;

		case IDOK:
		    OthFn.GetPasses(GetDlgItemInt(hDlg, IDC_PARAM1, &bTrans, TRUE));
		    subtype = temp;
		    switch (subtype)
			{
			case 'K':
			    fractalspecific[type].hor = -0.1;
			    fractalspecific[type].vert = -0.2;
			    fractalspecific[type].width = 0.65;
			    break;
			case 'L':
			    fractalspecific[type].hor = -1.4;
			    fractalspecific[type].vert = -0.96;
			    fractalspecific[type].width = 2.2;
			    break;
			case 'M':
			    fractalspecific[type].hor = -3.0;
			    fractalspecific[type].vert = -1.5;
			    fractalspecific[type].width = 3.0;
			    break;
			default:
			    fractalspecific[type].hor = -0.25;
			    fractalspecific[type].vert = -0.2;
			    fractalspecific[type].width = 2.5;
			    break;
			}
		    hCtrl = GetDlgItem(hDlg, IDC_DEFAULTPALETTE);
		    OthFn.GetUseDefaultPalette((BYTE)SendMessage(hCtrl, BM_GETCHECK, 0, 0L));
		    EndDialog(hDlg, TRUE);
		    return TRUE;

		case IDCANCEL:
		    EndDialog(hDlg, FALSE);
		    return FALSE;
		}
	    break;
	}
    return FALSE;
    }

/**************************************************************************
    Dialog Control for Triangles
**************************************************************************/

INT_PTR CALLBACK TriangleDlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
    {
    static     char	temp;
    static     UINT	tempParam;
    static     WORD	temp_special;
    HWND		hCtrl;
    BOOL		bTrans;
    char		s[24];

    switch (message)
	{
	case WM_INITDIALOG:
	    temp = subtype;
	    switch (subtype)
		{
		case 'A':
		    tempParam = IDC_A;
		    break;
		case 'B':
		    tempParam = IDC_B;
		    break;
		case 'C':
		    tempParam = IDC_C;
		    break;
		case 'D':
		    tempParam = IDC_D;
		    break;
		case 'E':
		    tempParam = IDC_E;
		    break;
		case 'F':
		    tempParam = IDC_F;
		    break;
		case 'G':
		    tempParam = IDC_G;
		    break;
		case 'H':
		    tempParam = IDC_H;
		    break;
		case 'I':
		    tempParam = IDC_I;
		    break;
		case 'J':
		    tempParam = IDC_J;
		    break;
		case 'K':
		    tempParam = IDC_K;
		    break;
		case 'L':
		    tempParam = IDC_L;
		    break;
		case 'M':
		    tempParam = IDC_M;
		    break;
		case 'N':
		    tempParam = IDC_N;
		    break;
		case 'O':
		    tempParam = IDC_O;
		    break;
		case 'P':
		    tempParam = IDC_P;
		    break;
		case 'Q':
		    tempParam = IDC_Q;
		    break;
		case 'R':
		    tempParam = IDC_R;
		    break;
		case 'S':
		    tempParam = IDC_S;
		    break;
		case 'T':
		    tempParam = IDC_T;
		    break;
		case 'U':
		    tempParam = IDC_U;
		    break;
		case 'V':
		    tempParam = IDC_V;
		    break;
		default:				// uninitialised
		    tempParam = IDC_A;
		    temp = 'A';
		    break;
		}

	    CheckRadioButton(hDlg, IDC_A, IDC_V, tempParam);
	    SetDlgItemInt(hDlg, IDC_PARAM1, 8, TRUE);
	    SetDlgItemInt(hDlg, IDC_PARAM2, 8, TRUE);
	    SetDlgItemInt(hDlg, IDC_PARAM3, 9, TRUE);
	    SetDlgItemInt(hDlg, IDC_PARAM4, 4, TRUE);
	    SetDlgItemInt(hDlg, IDC_PARAM5, 6, TRUE);
	    SetDlgItemInt(hDlg, IDC_PARAM6, 3, TRUE);

	    SAFE_SPRINTF(s, "%3.2f", 1.0);
	    SetDlgItemText(hDlg, IDC_PARAM7, s);
	    hCtrl = GetDlgItem(hDlg, IDC_EXPANDPALETTE);
	    SendMessage(hCtrl, BM_SETCHECK, TRUE, 0L);
	    hCtrl = GetDlgItem(hDlg, IDC_STAR);
	    SendMessage(hCtrl, BM_SETCHECK, TRUE, 0L);
	    hCtrl = GetDlgItem(hDlg, IDC_FILLPOLYGON);
	    SendMessage(hCtrl, BM_SETCHECK, TRUE, 0L);
	    SetFocus(GetDlgItem(hDlg, tempParam));
	    return FALSE;

	case WM_COMMAND:
	    switch ((int)LOWORD(wParam))
		{
		case IDC_A:
		case IDC_B:
		case IDC_C:
		case IDC_D:
		case IDC_E:
		case IDC_F:
		case IDC_G:
		case IDC_H:
		case IDC_I:
		case IDC_J:
		case IDC_K:
		case IDC_L:
		case IDC_M:
		case IDC_N:
		case IDC_O:
		case IDC_P:
		case IDC_Q:
		case IDC_R:
		case IDC_S:
		case IDC_T:
		case IDC_U:
		case IDC_V:
		    switch ((int)LOWORD(wParam))
			{
			case IDC_A:
			    temp = 'A';
			    break;
			case IDC_B:
			    temp = 'B';
			    break;
			case IDC_C:
			    temp = 'C';
			    break;
			case IDC_D:
			    temp = 'D';
			    break;
			case IDC_E:
			    temp = 'E';
			    break;
			case IDC_F:
			    temp = 'F';
			    break;
			case IDC_G:
			    temp = 'G';
			    break;
			case IDC_H:
			    temp = 'H';
			    break;
			case IDC_I:
			    temp = 'I';
			    break;
			case IDC_J:
			    temp = 'J';
			    break;
			case IDC_K:
			    temp = 'K';
			    break;
			case IDC_L:
			    temp = 'L';
			    break;
			case IDC_M:
			    temp = 'M';
			    break;
			case IDC_N:
			    temp = 'N';
			    break;
			case IDC_O:
			    temp = 'O';
			    break;
			case IDC_P:
			    temp = 'P';
			    break;
			case IDC_Q:
			    temp = 'Q';
			    break;
			case IDC_R:
			    temp = 'R';
			    break;
			case IDC_S:
			    temp = 'S';
			    break;
			case IDC_T:
			    temp = 'T';
			    break;
			case IDC_U:
			    temp = 'U';
			    break;
			case IDC_V:
			    temp = 'V';
			    break;
			}

		    CheckRadioButton(hDlg, IDC_A, IDC_V, (int)LOWORD(wParam));
		    return TRUE;

		case IDOK:
		    OthFn.GetPasses(GetDlgItemInt(hDlg, IDC_PARAM1, &bTrans, TRUE));
		    OthFn.GetRemainder(GetDlgItemInt(hDlg, IDC_PARAM2, &bTrans, TRUE));
		    OthFn.GetDivisor(GetDlgItemInt(hDlg, IDC_PARAM3, &bTrans, TRUE));
		    OthFn.GetjAngle(GetDlgItemInt(hDlg, IDC_PARAM4, &bTrans, TRUE));
		    OthFn.GetwAngle(GetDlgItemInt(hDlg, IDC_PARAM5, &bTrans, TRUE));
		    OthFn.GetSides(GetDlgItemInt(hDlg, IDC_PARAM6, &bTrans, TRUE));
		    double	Exponent;
		    GetDlgItemText(hDlg, IDC_PARAM7, s, 20);
		    sscanf(s, "%lf", &Exponent);
		    OthFn.GetExponent(Exponent);
		    subtype = temp;
		    hCtrl = GetDlgItem(hDlg, IDC_EXPANDPALETTE);
		    OthFn.GetExpandPalette((BYTE)SendMessage(hCtrl, BM_GETCHECK, 0, 0L));
		    hCtrl = GetDlgItem(hDlg, IDC_STAR);
		    OthFn.GetPlotStars((BYTE)SendMessage(hCtrl, BM_GETCHECK, 0, 0L));
		    hCtrl = GetDlgItem(hDlg, IDC_FILLPOLYGON);
		    OthFn.GetFillPolygon((BYTE)SendMessage(hCtrl, BM_GETCHECK, 0, 0L));
		    EndDialog(hDlg, TRUE);
		    return TRUE;

		case IDCANCEL:
		    EndDialog(hDlg, FALSE);
		    return FALSE;
		}
	    break;
	}
    return FALSE;
    }

/**************************************************************************
    Dialog Control for Geometry
**************************************************************************/

#define MAXPOINTS 16
INT_PTR CALLBACK GeometryDlg (HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
     {
     static     char	temp;
     static     UINT	tempParam;
     static     WORD	temp_special;
     BOOL		bTrans ;
     int		sides;
     long		count;

     switch (message)
	  {
	  case WM_INITDIALOG:
	        temp = subtype;
/*
	        switch (subtype)
		    {
		    case 'A':
			tempParam = IDC_A;
			break;
		    case 'B':
			tempParam = IDC_B;
			break;
		    case 'C':
			tempParam = IDC_C;
			break;
		    case 'D':
			tempParam = IDC_D;
			break;
		    default:				// uninitialised
			tempParam = IDC_A;
			temp = 'A';
			break;
		    }
*/
		sides = 7;
		count = 250L; 
		tempParam = IDC_A;
		temp = 'A';

		CheckRadioButton(hDlg, IDC_A, IDC_F, tempParam);
		SetDlgItemInt(hDlg, IDC_PARAM1, 7, TRUE);
		SetDlgItemInt(hDlg, IDC_PARAM2, 250L, TRUE);
		SetFocus(GetDlgItem(hDlg, tempParam));
	        return FALSE ;

	  case WM_COMMAND:
	        switch ((int) LOWORD(wParam))
		    {
		    case IDC_A:
		    case IDC_B:
		    case IDC_C:
		    case IDC_D:
		        switch ((int) LOWORD(wParam))
			    {
			    case IDC_A:
				temp = 'A';
				break;
			    case IDC_B:
				temp = 'B';
				break;
			    case IDC_C:
				temp = 'C';
				break;
			    case IDC_D:
				temp = 'D';
				break;
			    }

			CheckRadioButton(hDlg, IDC_A, IDC_F, (int) LOWORD(wParam));
		        return TRUE ;

		    case IDOK:
			OthFn.GetCount(GetDlgItemInt(hDlg, IDC_PARAM2, &bTrans, TRUE));
			sides = GetDlgItemInt(hDlg, IDC_PARAM1, &bTrans, TRUE);
			if (sides > MAXPOINTS)
			    sides = MAXPOINTS;
			if (sides < 2)
			    sides = 2;
			OthFn.GetSides(sides);
			subtype = temp;
			OthFn.GetSubtype(subtype);
			EndDialog (hDlg, TRUE);
			return TRUE;

		    case IDCANCEL:
			EndDialog (hDlg, FALSE);
			return FALSE;
		   }
		   break;
	    }
      return FALSE ;
      }

/**************************************************************************
	Dialog Control for Malthus Fractal Sub Types
**************************************************************************/

INT_PTR CALLBACK MalthusDlg (HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
     {
     static     char	temp;
     static     UINT	tempParam;
     static     WORD	temp_special;
		char	s[100];

     switch (message)
	  {
	  case WM_INITDIALOG:
	        temp = subtype;
		if (subtype == 0)
		    {
		    tempParam = IDC_A;
		    temp = 'A';
		    }
		else
		    tempParam = IDC_A + subtype - 'A';	    // ' ' (space) is uninitilised subtype
		CheckRadioButton(hDlg, IDC_A, IDC_Z, tempParam);
		SAFE_SPRINTF(s, "%f", param[0]);
		SetDlgItemText(hDlg, IDC_PARAM1, s);
		SAFE_SPRINTF(s, "%f", param[1]);
		SetDlgItemText(hDlg, IDC_PARAM2, s);
		SAFE_SPRINTF(s, "%f", param[2]);
		SetDlgItemText(hDlg, IDC_PARAM3, s);
		SAFE_SPRINTF(s, "%f", param[3]);
		SetDlgItemText(hDlg, IDC_PARAM4, s);
		SetFocus(GetDlgItem(hDlg, tempParam));
	        return FALSE ;

	  case WM_COMMAND:
	        if ((int) LOWORD(wParam) >= IDC_A && (int) LOWORD(wParam) <= IDC_Z)
		    {
		    temp = (int) LOWORD(wParam) - IDC_A + 'A';
		    CheckRadioButton(hDlg, IDC_A, IDC_Z, (int) LOWORD(wParam));
		    return TRUE ;
		    }
	        switch ((int) LOWORD(wParam))
		    {
		    case IDOK:
			GetDlgItemText(hDlg, IDC_PARAM1, s, 100);
			param[0] = atof(s);
			GetDlgItemText(hDlg, IDC_PARAM2, s, 100);
			param[1] = atof(s);
			GetDlgItemText(hDlg, IDC_PARAM3, s, 100);
			param[2] = atof(s);
			GetDlgItemText(hDlg, IDC_PARAM4, s, 100);
			param[3] = atof(s);
			subtype = temp;
			EndDialog (hDlg, TRUE);
			return TRUE;

		    case IDCANCEL:
			EndDialog (hDlg, FALSE);
			return FALSE;
		   }
		   break;
	    }
      return FALSE ;
      }
