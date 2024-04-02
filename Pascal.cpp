/*
   PASCAL.CPP a module for Pascal Fractals
   Thanks to Marcus Rezende 

   Written in Microsoft Visual C++ by Paul de Leeuw.
*/

#include	<stdio.h>
#include	"manp.h"
#include	"resource.h"
#include	"Plot.h"

#define		PASCALSIZE	240
#define		TRIANGLESIZE	80
#define		ABS(a)		((a >= 0) ? a : -a)

extern	CPlot	Plot;		// image plotting routines 

extern	long	threshold;
//extern	double	param[];
extern	int	subtype;			// A - E
extern	int	xdots, ydots, width, height, bits_per_pixel;;
static  int	n = 100, moda = 11, hi = 160, lo = 4, CircleSize = 8;

/**************************************************************************
	Pascal Fractal Type Images
***************************************************************************/

int	DoPascal(void)

    {
    int		i, j;
    unsigned	__int64	a[PASCALSIZE];			// use 64 bit unsigned integers for maximum significant figures
    unsigned	__int64	b[PASCALSIZE];
		__int64	c[TRIANGLESIZE][TRIANGLESIZE];

    if (lo > threshold)
	lo = threshold - 1;
    if (hi > threshold)
	hi = threshold - 1;

    if (subtype == 'A')
	{
	if (n > PASCALSIZE - 1)
	    n = PASCALSIZE - 1;
	for (i = 0; i < PASCALSIZE; i++)
	    {
	    a[i] = 0;					// initialise arrays
	    b[i] = 0;
	    }
	}
    else
	{
	if (n > TRIANGLESIZE - 1)
	    n = TRIANGLESIZE - 1;
	for (i = 0; i < TRIANGLESIZE; i++)
	    for (j = 0; j < TRIANGLESIZE; j++)
		c[i][j] = 0;
	}
    switch (subtype)
	{
	case 'A':					// Pascal
	    a[1] = 1;
	    a[2] = 0;
	    for (i = 2; i <= n; i++)
		{
		for (j = 2; j <= n; j++)
		    {
		    b[j] = a[j - 1] + a[j];
		    if (b[j])
			Plot.DisplayFilledCircle(j * CircleSize + xdots / 2 - i * CircleSize / 2, ydots / 2 - CircleSize * n / 2 + i * CircleSize, CircleSize / 2, (b[j] % moda == 0) ? lo : hi);
		    }
		memcpy(&a,&b, sizeof(__int64)*(n+1));	// swap a and b arrays
		}
	    break;
	case 'B':   					// Bell	http://en.wikipedia.org/wiki/Bell_number
	    c[1][1] = 1;
	    c[2][1] = 1;
	    c[2][2] = 2;
	    for (i = 2; i <= n; i++)
		{
		c[i][1] = c[i - 1][i - 1];
//		DisplayFilledCircle(CircleSize + xdots / 2 - i * CircleSize / 2, ydots / 2 - CircleSize * n / 2 + i * CircleSize, CircleSize / 2, (c[i][1] % moda == 0) ? lo : hi);
		for (j = 2; j <= i; j++)
		    {
		    c[i][j] = c[i][j - 1] + c[i - 1][j - 1];
		    Plot.DisplayFilledCircle(xdots / 2 + i * CircleSize / 2 - j * CircleSize, ydots / 2 - CircleSize * n / 2 + i * CircleSize, CircleSize / 2, (c[i][j] % moda == 0) ? lo : hi);
		    }
		}
	    break;
	case 'C':   					// Catalan
	    c[1][1] = 1;
	    c[2][1] = 1;
	    c[2][2] = 2;
	    for (i = 2; i <= n; i++)
		{
		c[i][1] = 1;
//		DisplayFilledCircle(CircleSize + xdots / 2 - i * CircleSize / 2, ydots / 2 - CircleSize * n / 2 + i * CircleSize, CircleSize / 2, (c[i][1] % moda == 0) ? lo : hi);
		for (j = 2; j <= i; j++)
		    {
		    c[i][j] = c[i][j - 1] + c[i - 1][j];
		    Plot.DisplayFilledCircle(xdots / 2 + i * CircleSize / 2 - j * CircleSize, ydots / 2 - CircleSize * n / 2 + i * CircleSize, CircleSize / 2, (c[i][j] % moda == 0) ? lo : hi);
		    }
		}
	    break;
	case 'D':   					// Dudley	http://en.wikipedia.org/wiki/Dudley_triangle
	    Plot.DisplayFilledCircle(xdots / 2 + CircleSize / 2 - CircleSize, ydots / 2 - CircleSize * n / 2 * CircleSize, CircleSize / 2, hi);
	    c[1][1] = 2;
	    c[2][1] = 2;
	    c[2][2] = 2;
	    for (i = 1; i <= n; i++)
		{
//		c[i][1] = c[i - 1][i - 1];
//		DisplayFilledCircle(CircleSize + xdots / 2 - i * CircleSize / 2, ydots / 2 - CircleSize * n / 2 + i * CircleSize, CircleSize / 2, (c[i][1] % moda == 0) ? lo : hi);
		for (j = 1; j <= i; j++)
		    {
		    c[i][j] = j * (j + 1) % (i + 2);
		    Plot.DisplayFilledCircle(xdots / 2 + i * CircleSize / 2 - j * CircleSize, ydots / 2 - CircleSize * n / 2 + i * CircleSize, CircleSize / 2, (c[i][j] % moda == 0) ? lo : hi);
		    }
		}
	    break;
	case 'E':   					// Floyd	
	    Plot.DisplayFilledCircle(xdots / 2 + CircleSize / 2 - CircleSize, ydots / 2 - CircleSize * n / 2 * CircleSize, CircleSize / 2, hi);
	    c[1][1] = 2;
	    c[2][1] = 2;
	    c[2][2] = 2;
	    for (i = 1; i <= n; i++)
		{
		c[i][1] = c[i - 1][i - 1] + 1;
//		DisplayFilledCircle(CircleSize + xdots / 2 - i * CircleSize / 2, ydots / 2 - CircleSize * n / 2 + i * CircleSize, CircleSize / 2, (c[i][1] % moda == 0) ? lo : hi);
		for (j = 1; j <= i; j++)
		    {
		    c[i][j] = c[i][j - 1] + 1;
		    Plot.DisplayFilledCircle(xdots / 2 + i * CircleSize / 2 - j * CircleSize, ydots / 2 - CircleSize * n / 2 + i * CircleSize, CircleSize / 2, (c[i][j] % moda == 0) ? lo : hi);
		    }
		}
	    break;
	case 'F':   					// Hosoya	http://en.wikipedia.org/wiki/Hosoya%27s_triangle
	    c[1][1] = 1;
	    c[1][2] = 0;
	    c[1][3] = 0;
	    c[2][1] = 1;
	    c[2][2] = 1;
	    c[2][3] = 0;
	    for (i = 2; i <= n; i++)
		{
//		c[i][1] = c[i - 1][i - 1];
//		DisplayFilledCircle(CircleSize + xdots / 2 - i * CircleSize / 2, ydots / 2 - CircleSize * n / 2 + i * CircleSize, CircleSize / 2, (c[i][1] % moda == 0) ? lo : hi);
		for (j = 3; j <= i; j++)
		    {
		    c[i][j] = c[i - 1][j] + c[i - 2][j];
    		    if (c[i][j] == 0)
			c[i][j] = c[i - 1][j - 1] + c[i - 2][j - 2];

		    Plot.DisplayFilledCircle(xdots / 2 + i * CircleSize / 2 - j * CircleSize, ydots / 2 - CircleSize * n / 2 + i * CircleSize, CircleSize / 2, (c[i][j] % moda == 0) ? lo : hi);
		    }
		}
	    break;
	case 'G':   					// Leibniz	http://en.wikipedia.org/wiki/Leibniz_triangle
	    double	d[TRIANGLESIZE][TRIANGLESIZE];

	    d[1][1] = 1;
	    d[2][1] = 0.5;
	    d[2][2] = 0.5;

	    c[1][1] = (__int64)(1 / d[1][1]);
	    c[2][1] = (__int64)(1 / d[2][1]);
	    c[2][2] = (__int64)(1 / d[2][2]);
	    for (i = 2; i <= n; i++)
		{
		d[i][1] = 1.0 / i;
		c[i][1] = (__int64)(1 / d[i][1] + 0.5);
		for (j = 2; j <= i; j++)
		    {
		    d[i][j] = fabs(d[i - 1][j - 1] - d[i][j - 1]);
		    c[i][j] = (__int64)(1 / d[i][j] + 0.5);
//		    DisplayFilledCircle(xdots / 2 + i * CircleSize / 2 - j * CircleSize, ydots / 2 - CircleSize * n / 2 + i * CircleSize, CircleSize / 2, (c[i][j] % moda == 0) ? lo : hi);
		    Plot.DisplayFilledCircle(xdots / 2 - i * CircleSize / 2 + j * CircleSize, ydots / 2 - CircleSize * n / 2 + i * CircleSize, CircleSize / 2, (c[i][j] % moda == 0) ? lo : hi);
		    }
		}
	    break;
	case 'H':   					// Lozanic	http://en.wikipedia.org/wiki/Lozani%C4%87%27s_triangle
	    c[0][0] = 1;
	    c[1][1] = 1;
	    for (i = 1; i <= n; i++)
		{
		c[i][0] = 1;
//		DisplayFilledCircle(CircleSize + xdots / 2 - i * CircleSize / 2, ydots / 2 - CircleSize * n / 2 + i * CircleSize, CircleSize / 2, (c[i][1] % moda == 0) ? lo : hi);
		for (j = 1; j <= i; j++)
		    {
		    c[i][j] = c[i - 1][j - 1] + c[i - 1][j];
//IF r / 2 = r \ 2 AND k / 2 > k \ 2 THEN GOTO 8 ELSE GOTO 9
		    if (i % 2 == 0 && j % 2 != 0)
			{
			int	l;
			double	numerator, denominator1, denominator2, pronum, prodem1, prodem2;

			numerator = i / 2 - 1;
			denominator1 = (j - 1) / 2;
			denominator2 = numerator - denominator1;

			pronum = 1;			    // factorial of numerator
			for (l = 1; l <= numerator; l++)
			    pronum *= l;

			prodem1 = 1;			    // factorial of denominator1
			for (l = 1; l <= denominator1; l++)
			    prodem1 *= l;

			prodem2 = 1;			    // factorial of denominator2
			for (l = 1; l <= denominator1; l++)
			    prodem2 *= l;
			c[i][j] -= (__int64)(pronum / prodem1 / prodem2);
			}
//		    else
		    Plot.DisplayFilledCircle(xdots / 2 + i * CircleSize / 2 - j * CircleSize, ydots / 2 - CircleSize * n / 2 + i * CircleSize, CircleSize / 2, (c[i][j] % moda == 0) ? lo : hi);
		    }
		}
	    break;
	case 'I':   					// Stirling (1st)	http://www.jstor.org/discover/10.2307/3026928?uid=3737536&uid=2129&uid=2&uid=70&uid=4&sid=21102292820281
	    c[0][0] = 1;
	    c[1][0] = 0;
	    c[1][1] = 1;
	    c[2][1] = -1;
	    c[2][2] = 1;
	    c[3][1] = 2;
	    c[3][2] = -3;
	    c[3][3] = 1;
	    for (i = 1; i <= n; i++)
		{
//		c[i][1] = c[i - 1][i - 1];
//		DisplayFilledCircle(CircleSize + xdots / 2 - i * CircleSize / 2, ydots / 2 - CircleSize * n / 2 + i * CircleSize, CircleSize / 2, (c[i][1] % moda == 0) ? lo : hi);
		for (j = 1; j <= i; j++)
		    {
		    c[i][j] = -(i - 1) * c[i - 1][j] + c[i - 1][j - 1];
    		    if (i == j)
			c[i][j] = 1;
		    Plot.DisplayFilledCircle(xdots / 2 + i * CircleSize / 2 - j * CircleSize, ydots / 2 - CircleSize * n / 2 + i * CircleSize, CircleSize / 2, (ABS(c[i][j]) % moda == 0) ? lo : hi);
		    }
		}
	    break;
	case 'J':   					// Stirling (2nd)	http://en.wikipedia.org/wiki/Stirling_numbers_of_the_second_kind
	    c[0][0] = 1;
	    c[1][0] = 0;
	    c[1][1] = 1;
	    c[2][1] = 1;
	    c[2][2] = 1;
	    c[3][1] = 2;
	    c[3][2] = 3;
	    c[3][3] = 1;
	    for (i = 1; i <= n; i++)
		{
//		c[i][1] = c[i - 1][i - 1];
//		DisplayFilledCircle(CircleSize + xdots / 2 - i * CircleSize / 2, ydots / 2 - CircleSize * n / 2 + i * CircleSize, CircleSize / 2, (c[i][1] % moda == 0) ? lo : hi);
		for (j = 1; j <= i; j++)
		    {
		    c[i][j] = j * c[i - 1][j] + c[i - 1][j - 1];
    		    if (i == j)
			c[i][j] = 1;
		    Plot.DisplayFilledCircle(xdots / 2 + i * CircleSize / 2 - j * CircleSize, ydots / 2 - CircleSize * n / 2 + i * CircleSize, CircleSize / 2, (ABS(c[i][j]) % moda == 0) ? lo : hi);
		    }
		}
	    break;
	}
    return 0;
    }

DLGPROC FAR PASCAL NumTriangleDlg (HWND hDlg, UINT message, UINT wParam, LONG lParam)
     {
     static     char	temp;
     static     UINT	tempParam;
     static     WORD	temp_special;
//     HWND		hCtrl;
     BOOL		bTrans ;
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
		SetDlgItemInt(hDlg, IDC_PARAM1, (int)n, TRUE);
		SetDlgItemInt(hDlg, IDC_PARAM2, (int)moda, TRUE);
		SetDlgItemInt(hDlg, IDC_PARAM3, (int)lo, TRUE);
		SetDlgItemInt(hDlg, IDC_PARAM4, (int)hi, TRUE);
		SetDlgItemInt(hDlg, IDC_PARAM5, (int)CircleSize, TRUE);
		SetFocus(GetDlgItem(hDlg, tempParam));
	        return FALSE ;

	  case WM_COMMAND:
	        switch ((int) LOWORD(wParam))
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

			CheckRadioButton(hDlg, IDC_A, IDC_J, (int) LOWORD(wParam));
		        return (DLGPROC)TRUE ;

		    case IDOK:
			n = GetDlgItemInt(hDlg, IDC_PARAM1, &bTrans, TRUE);
			moda = GetDlgItemInt(hDlg, IDC_PARAM2, &bTrans, TRUE);
			lo = GetDlgItemInt(hDlg, IDC_PARAM3, &bTrans, TRUE);
			hi = GetDlgItemInt(hDlg, IDC_PARAM4, &bTrans, TRUE);
			CircleSize = GetDlgItemInt(hDlg, IDC_PARAM5, &bTrans, TRUE);
			subtype = temp;
			EndDialog (hDlg, TRUE);
			return (DLGPROC)TRUE;

		    case IDCANCEL:
			EndDialog (hDlg, FALSE);
			return (DLGPROC)FALSE;
		   }
		   break;
	    }
      return (DLGPROC)FALSE ;
      }

