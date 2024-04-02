/*
   MALTHUS.CPP a module for 3D-Malthus Fractals
   Thanks to Marcus Rezende 

   Written in Microsoft Visual C++ by Paul de Leeuw.
*/

#include	<stdio.h>
#include	"manp.h"
#include	"Fract.h"
#include	"resource.h"
#include	"fractype.h"
#include	"menu.h"
#include	"anim.h"
#include	"colour.h"
#include	"plot.h"

extern	int	user_data(HWND);
extern	char	*AnimData(void);
extern	void	ConvertRGB2ASCII(BYTE, BYTE, BYTE, char *);
extern	char	*GenerateMPEGFileName (char *, char *);
extern	char	*GenerateAnimFileName (char *, char *);
extern	void	SetUpFilename(char *Filename, char *Folder, char *AnimType);
extern	void	InitTransformation(double tx, double ty, double tz);
extern	void	DoTransformation(double *x1, double *y1, double *z1, double x, double y, double z);

extern	BOOL	WritePNGFrames;			// write frames to PNG files
extern	BOOL	WriteMemFrames;			// write frames to memory
extern	BOOL	WritePNGList;			// write PNG filenames to a *lst file
extern	BOOL	WriteMPEGFrames;		// write frames directly to an MPEG file
extern	double 	*wpixels;			// an array of doubles holding slope modified iteration counts

extern	char	MPGPath[];			// path for MPEG files
extern	char	MPGFile[];			// MPEG file
extern	char	PNGName[];			// base name for PNG file sequence
extern	char	ANIMPNGPath[];			// path for animated PNG files and LST files
extern	char	PNGFile[];			// PNG file
extern	char	ScriptFileName[];		// base name for script file 

extern	long	threshold;
extern	double	mandel_width;			/* width of display */
extern	double	hor;				/* horizontal address */
extern	double	vert;				/* vertical address */
extern	double	ScreenRatio;			// ratio of width / height for the screen
extern	double	param[];
extern	WORD	type;				// fractal type
extern	int	subtype;			// A - E
extern	char	PNGName[];			// base name for PNG file sequence
//extern	BYTE	*PalettePtr;			// points to true colour palette
extern	CTrueCol    TrueCol;			// palette info
extern	BOOL	StartImmediately;		// immediate start of animation generation
//extern	BYTE	ScriptPaletteFlag;		// Display palette 
extern	ProcessType	OscAnimProc;

extern	double	x_rot;				/* angle display plane to x axis */
extern	double	y_rot;				/* angle display plane to y axis */
extern	double	z_rot;				/* angle display plane to z axis */
extern	BYTE	_3dflag;			/* generate image in 3D */
extern	int	xAxis, yAxis, zAxis;		// numerical values for axes for chaotic oscillators

extern	int	xdots, ydots;

extern	CPlot	Plot;		// image plotting routines 
extern	CDib    Dib;				// Device Independent Bitmap

static	double	xscale, yscale;
static	int	frames = 100;
static	double	StartRate = -1.0, EndRate = 2.0;
static	double	cMax[9], cMin[9];

/**************************************************************************
	Common Display Routines
***************************************************************************/

int	DisplayMalthus(double c1[], DWORD colour, int dimensions)
    {
//    double	x, y, z, x1, y1, z1, delta = iterations / GetNumFrames();
    double	x, y, z, x1, y1, z1;
    int		j;
    static  int u, v, uOld, vOld;

    if (OscAnimProc == INITANIM)
	{
	for (j = 0; j < dimensions; j++)
	    {
	    if (c1[j] > cMax[j] && c1[j] < max(hor + 2.0 * mandel_width, vert + 2.0 * mandel_width))	// don't chase points way off screen
		cMax[j] = c1[j];			// find centre of fractal
	    if (c1[j] < cMin[j] && c1[j] > min(hor - mandel_width, vert - mandel_width))		// don't chase points way off screen 
		cMin[j] = c1[j];
	    }
	}

    if (OscAnimProc == RUNANIM)
	{
	x = c1[xAxis];
	y = c1[yAxis];
	z = c1[zAxis];

	DoTransformation(&x1, &y1, &z1, x, y, z);

	u = (int)((x1 - (cMax[xAxis] + cMin[xAxis] - mandel_width * ScreenRatio) / 2) * xscale);
//	v = (int)(((cMax[yAxis] + cMin[yAxis] + mandel_width) / 2 - y1) * yscale);
	v = (int)(((cMax[yAxis] + cMin[yAxis] + mandel_width) / 2 - y1) * yscale);
	}

    if (OscAnimProc == STANDARD || OscAnimProc == EVOLUTION)
	{
	u = (long)((c1[xAxis] - hor) * xscale);
	v = (long)((vert + mandel_width - c1[yAxis]) * yscale);
	}
    if (OscAnimProc != INITANIM)
	{
	Plot.PlotPoint(u, v, colour);
	}
    return 0;
    }

/**************************************************************************
	3D-Malthus Fractal Type Images
***************************************************************************/

int	DoMalthus(void)

    {
//    int	    px, py;

    int	    i, j, k;
    double  pold = 0.1, pnew = 0.1, rate = param[0], RateInc;
    double  y = 0, x = 0, c1[3];
    int	    colour = 1, cor = (param[3] != 0.0) ? threshold / (int) param[3] : threshold;

    xscale = (double) (xdots - 1) / (mandel_width * ScreenRatio);
    yscale = (double) (ydots - 1) / mandel_width;

    if (OscAnimProc == INITANIM)
	{
	c1[0] = x;
	c1[1] = y;
	c1[2] = 0;
	for (j = 0; j < 3; j++)
	    cMax[j] = cMin[j] = c1[j];
	if (!_3dflag)
	    x_rot = y_rot = z_rot = 0.0;
	}

    if (OscAnimProc == RUNANIM)
	InitTransformation((cMax[xAxis] + cMin[xAxis]) / 2, (cMax[yAxis] + cMin[yAxis]) / 2, (cMax[zAxis] + cMin[zAxis]) / 2);	// translate to the centre of the object

    RateInc = (param[2] > 0.0) ? 0.1 / param[2] : 0.01;
    if (subtype == 'B' || subtype == 'I')
	rate /= 100.0;
    if (subtype == 'Q')
	rate /= 10.0;

    if (cor < 1) 
	cor = 1;
    for (j = 0; j < (int) param[1]; j++)
	{
        colour += cor;
        if (colour >= threshold)
	    colour = 1;
	for (k = 0; k < (int) param[2]; k++)
	    {
	    int count = 0;
	    rate += RateInc;
	    pold = 0.01;
	    for (i = 0; i < threshold; i++)
		{
		switch (subtype)
		    {
		    case 'A':					
			pnew = 1 - 1.2 * rate * (pold * (1 - sin(pold)));
			x = pold - 2 * sin(pnew);
			break;
		    case 'B':					
			pnew = pnew - 3 * rate * (pold * (1 - pold));
			x = pold - 2 * pold * pnew;
			break;
		    case 'C':					
			pnew = rate * (pold * (1 - pnew));
			x = pold - 2 * pnew;
			break;
		    case 'D':					
			pnew = rate + (pold * (1 - pold));
			x = pold - 2 * pnew;
			break;
		    case 'E':					
                        pnew = rate * (pold * (1 - pold));
                        x = pold - pnew;
			break;
		    case 'F':					
			pnew = rate * (pold * (1 - pold));
			x = pnew + pold;
			break;
		    case 'G':					
			pnew = rate * (pold * (1 - pnew));
			x = pnew + 2 * pold;
			break;
		    case 'H':					
			pnew = rate * (sqrt(fabs(pold * pnew)) * (1 - 2 * pnew));
			x = pnew - 2 * pold;
			break;
		    case 'I':					
			pnew -= 3 * rate * (pold * (1 - pold));
			x = pold - 6 * pold * pnew;
			break;
		    case 'J':					
			pnew = pold + rate * (sin(pold) * (0.3 - cos(pold)));
			x = pold + sin(pnew);
			break;
		    case 'K':					
//			pnew = rate * (pold * (1 - pnew / (pold + 0.101)));
//			x = pnew - pold;
			pnew = rate * (pold * (1 - pold));
			x = pold * pnew * 10;
			break;
		    case 'L':					
			pnew = pold + rate * (sin(pold) * (0.3 - cos(pold)));
			x = pold - sin(pnew) + tan(pold);
			break;
		    case 'M':					
			pnew = rate * (pold * (1 - pold));
			x = pnew - pold;
			break;
		    case 'N':					
//			pnew = rate * (pold * (1 - pold));
//			x = 3 * pnew - 2.5 * pold;
			pnew = rate * (pold * (1 + cos(pold)));
			x = pnew + 2 * pold;
			break;
		    case 'O':					
//                        pnew = rate * (pold * (1 - pold));
//                        x = (pnew - pold);
			pnew = tan(rate) * (pold * (1 + cos(pold))) * 8;
			x = pnew + 2 * pold;
			break;
		    case 'P':					
			pnew = pold + rate * (sin(pold) * (0.3 - cos(pold)));
			x = pold + sin(pnew) + tan(pold);
			break;
		    case 'Q':					
// Marcus suggested to change this one
//			pnew = rate * (pold * (1 - pnew / (pold + 0.01)));
//			x = pnew - pold;
			pnew = 1 - 3.2 * rate * (pold * (1 - sin(pold)));
			x = pold - 2 *  sin(pold);
			break;
		    case 'R':					
			pnew = rate * (pold * (3 - 6 * pold)) / 3;
			x = pnew - 3 * pold;
			break;
		    case 'S':					
			pnew = -rate * (atan(pold) * (1 - atan(pold)));
			x = sin(pnew) / tan(pold);
			break;
		    case 'T':					
			pnew = rate * (atan(pold) * (1 - sin(pold)));
			x = sin(pnew) / tan(pold);
			break;
		    case 'U':					
			pnew = rate * (pold * (1 - pold));
			x = pold;
			break;
		    case 'V':					
			pnew = rate * (pold * (1 - pnew));
			x = pnew - 2 * pold;
			break;
		    case 'W':					
			pnew = sqrt(fabs(rate * (pold * (1 - pold))));
			x = pold;
			break;
		    case 'X':					
			pnew = rate * (pold * (1 - pnew / (pold - 0.301)));
			x = pnew - 5 * pold;
			break;
		    case 'Y':					
			pnew = rate * (pold * (1 - pnew / (pold + 0.101)));
			x = pnew - pold;
			break;
		    case 'Z':					
			pnew = rate * (pold * (1 - pold));
			x = (pnew / 80 - pold);
			break;
		    }
//		px = (int)((x - hor) * xscale + 0.5);
//		py = (int)((vert + mandel_width - y) * yscale + 0.5);

//		plot (px, py, colour);
		c1[0] = x;
		c1[1] = y;
		c1[2] = sqrt(sqr(sin(x)) + sqr(cos(y)));
		DisplayMalthus(c1, colour, 3);
		if (pold == pnew)
		    {
		    count++;
		    if (count > threshold)
			break;
		    }
		else 
		    count = 0;
		pold = pnew;
		y = x;
		}
	    }
	}
    return 0;
    }

/**************************************************************************
	Malthus script generator
**************************************************************************/

int	GenMalthusScript(HWND hwnd, char *filename) 

    {
    int		i, k;
    char	s[120];
    FILE	*out;
    char	ascii[6];

    if ((out = fopen(filename, "w")) == NULL)
	{
	sprintf(s, "Cannot open output file %s\nDoes Folder exist?", filename);
	MessageBox (hwnd, s, "Animation", MB_ICONEXCLAMATION | MB_OK);
	MessageBeep (0);
	return -1;
	}

    fprintf(out, "-t%ld -s\"%s\" %s", threshold, PNGName, AnimData());	// add quotes to filename to trap spaces in path
    fprintf(out, " -c%24.24f,%24.24f,%24.24f\n", hor, vert, mandel_width);
    fprintf(out, "Malthus Animation: %7.7f %7.7f %d\n", StartRate, EndRate, frames);
    fprintf(out, "Palette=\n");
    for (i = 0, k = 0; i < threshold; i++, k++)
	{
	if (k == 20)							// group into lumps of 20
	    {
	    k = 0;
	    fprintf(out, "\n");
	    }
	ConvertRGB2ASCII(*(TrueCol.PalettePtr + i * 3 + 0), *(TrueCol.PalettePtr + i * 3 + 1), *(TrueCol.PalettePtr + i * 3 + 2), ascii);
	fprintf(out, "%s", ascii);
	}
    fprintf(out, "\n");
    fclose(out);
    return 0;
    }

/**************************************************************************
	Dialogue Box For Setting Up Malthus Animation
**************************************************************************/

DLGPROC FAR PASCAL MalthusAnimDlg (HWND hDlg, UINT message, UINT wParam, LONG lParam)
     {
     char		*fileptr;
     HWND		hCtrl;
     BOOL		TempCheck;
     BOOL		bTrans ;
     static	char	TempFile[MAX_PATH];
     static	char	TempStr[MAX_PATH];

     switch (message)
	  {
	  case WM_INITDIALOG:
		hCtrl = GetDlgItem (hDlg, IDC_SHOWPALETTE);
		SendMessage(hCtrl, BM_SETCHECK, TrueCol.ScriptPaletteFlag, 0L);
		hCtrl = GetDlgItem (hDlg, IDC_STARTNOW);
		SendMessage(hCtrl, BM_SETCHECK, StartImmediately, 0L);
		sprintf(TempStr, "%f", StartRate);
		SetDlgItemText(hDlg, IDC_RATE_START, TempStr);
		sprintf(TempStr, "%f", EndRate);
		SetDlgItemText(hDlg, IDC_RATE_END, TempStr);
		SetUpFilename(ScriptFileName, "sci", "Malthus");
		SetUpFilename(PNGName, "animpng", "Malthus");
		SetDlgItemText(hDlg, IDC_SCRIPT_FILENAME, ScriptFileName);
		SetDlgItemText(hDlg, IDC_SEQUENCE_NAME, PNGName);
		hCtrl = GetDlgItem (hDlg, IDC_WRITEPNGDIRECT);
		SendMessage(hCtrl, BM_SETCHECK, WritePNGFrames, 0L);
		hCtrl = GetDlgItem (hDlg, IDC_WRITEMPEGDIRECT);
		SendMessage(hCtrl, BM_SETCHECK, WriteMPEGFrames, 0L);
		hCtrl = GetDlgItem (hDlg, IDC_WRITEMEMDIRECT);
		SendMessage(hCtrl, BM_SETCHECK, WriteMemFrames, 0L);
		hCtrl = GetDlgItem (hDlg, IDC_WRITEPNGFILELIST);
		SendMessage(hCtrl, BM_SETCHECK, WritePNGList, 0L);
		SetDlgItemInt(hDlg, IDC_FRAMES, frames, TRUE);
	        return (DLGPROC)TRUE ;

	  case WM_COMMAND:
	        switch ((int) LOWORD(wParam))
		    {
		    case IDC_WRITEMPEGDIRECT:
			hCtrl = GetDlgItem (hDlg, IDC_WRITEMPEGDIRECT);
			WriteMPEGFrames = (BYTE)SendMessage(hCtrl, BM_GETCHECK, 0, 0L);
			if (WriteMPEGFrames)					// no point in writing other forms if direct to MPEG
			    {
			    hCtrl = GetDlgItem (hDlg, IDC_WRITEPNGDIRECT);
			    SendMessage(hCtrl, BM_SETCHECK, FALSE, 0L);
			    hCtrl = GetDlgItem (hDlg, IDC_WRITEMEMDIRECT);
			    SendMessage(hCtrl, BM_SETCHECK, FALSE, 0L);
			    hCtrl = GetDlgItem (hDlg, IDC_WRITEPNGFILELIST);
			    SendMessage(hCtrl, BM_SETCHECK, FALSE, 0L);
			    WritePNGFrames = WriteMemFrames = WritePNGList = FALSE;
			    sprintf(MPGFile, "%s", GenerateMPEGFileName (MPGPath, PNGName));
			    SetDlgItemText(hDlg, IDC_SEQUENCE_NAME, MPGFile);
//			    sprintf(MPGFile, "%s", GenerateAnimFileName (MPGPath, PNGName));
//			    SetDlgItemText(hDlg, IDC_SEQUENCE_NAME, MPGFile);
			    }
			return (DLGPROC)TRUE;

		    case IDC_WRITEPNGDIRECT:
		    case IDC_WRITEMEMDIRECT:
		    case IDC_WRITEPNGFILELIST:
			hCtrl = GetDlgItem (hDlg, (int) LOWORD(wParam));
			TempCheck = (BYTE)SendMessage(hCtrl, BM_GETCHECK, 0, 0L);
			if (TempCheck)						// see above
			    {
			    hCtrl = GetDlgItem (hDlg, IDC_WRITEMPEGDIRECT);
			    SendMessage(hCtrl, BM_SETCHECK, FALSE, 0L);
			    WriteMPEGFrames = FALSE;
			    }
			sprintf(PNGFile, "%s", GenerateAnimFileName (ANIMPNGPath, PNGName));
			SetDlgItemText(hDlg, IDC_SEQUENCE_NAME, PNGFile);
			return (DLGPROC)TRUE;

		    case IDOK:
			GetDlgItemText(hDlg, IDC_SEQUENCE_NAME, PNGName, MAX_PATH);
			fileptr = PNGName + strlen(PNGName);
			while (fileptr > PNGName && *fileptr != '.')
			    fileptr--;	
			if (*fileptr == '.')						// remove any extension
			    *fileptr = '\0';
			
			if (WriteMPEGFrames)						// generate MPEG filename
			    {
			    GetDlgItemText(hDlg, IDC_SEQUENCE_NAME, TempFile, MAX_PATH);
			    sprintf(MPGFile, "%s", GenerateMPEGFileName (MPGPath, TempFile));
			    }

			GetDlgItemText(hDlg, IDC_SCRIPT_FILENAME, ScriptFileName, 400);
			fileptr = ScriptFileName + strlen(ScriptFileName);
			while (fileptr > ScriptFileName && *fileptr != '.')
			    fileptr--;							// remove extension
			if (*fileptr == '.')
			    *fileptr = '\0';
			strcat(ScriptFileName, ".sci");
			hCtrl = GetDlgItem (hDlg, IDC_STARTNOW);
			StartImmediately = (BYTE)SendMessage(hCtrl, BM_GETCHECK, 0, 0L);
			hCtrl = GetDlgItem (hDlg, IDC_SHOWPALETTE);
			TrueCol.ScriptPaletteFlag = (BYTE)SendMessage(hCtrl, BM_GETCHECK, 0, 0L);
			GetDlgItemText(hDlg, IDC_RATE_START, TempStr, 100);
			sscanf(TempStr, "%lf", &StartRate);
			GetDlgItemText(hDlg, IDC_RATE_END, TempStr, 100);
			sscanf(TempStr, "%lf", &EndRate);
			hCtrl = GetDlgItem (hDlg, IDC_WRITEPNGDIRECT);
			WritePNGFrames = (BYTE)SendMessage(hCtrl, BM_GETCHECK, 0, 0L);
			hCtrl = GetDlgItem (hDlg, IDC_WRITEMEMDIRECT);
			WriteMemFrames = (BYTE)SendMessage(hCtrl, BM_GETCHECK, 0, 0L);
			hCtrl = GetDlgItem (hDlg, IDC_WRITEMPEGDIRECT);
			WriteMPEGFrames = (BYTE)SendMessage(hCtrl, BM_GETCHECK, 0, 0L);
			hCtrl = GetDlgItem (hDlg, IDC_WRITEPNGFILELIST);
			WritePNGList = (BYTE)SendMessage(hCtrl, BM_GETCHECK, 0, 0L);
			frames = GetDlgItemInt(hDlg, IDC_FRAMES, &bTrans, TRUE);
			if (frames <= 0)
			    frames = 20;
			if (frames > MAXANIM)
			    frames = MAXANIM;
			type = MALTHUS;
			GenMalthusScript(hDlg, ScriptFileName);
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

/**************************************************************************
	Dialog Control for Malthus Fractal Sub Types
**************************************************************************/

DLGPROC FAR PASCAL MalthusDlg (HWND hDlg, UINT message, UINT wParam, LONG lParam)
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
		sprintf(s, "%f", param[0]);
		SetDlgItemText(hDlg, IDC_PARAM1, s);
		sprintf(s, "%f", param[1]);
		SetDlgItemText(hDlg, IDC_PARAM2, s);
		sprintf(s, "%f", param[2]);
		SetDlgItemText(hDlg, IDC_PARAM3, s);
		sprintf(s, "%f", param[3]);
		SetDlgItemText(hDlg, IDC_PARAM4, s);
		SetFocus(GetDlgItem(hDlg, tempParam));
	        return (DLGPROC)FALSE ;

	  case WM_COMMAND:
	        if ((int) LOWORD(wParam) >= IDC_A && (int) LOWORD(wParam) <= IDC_Z)
		    {
		    temp = (int) LOWORD(wParam) - IDC_A + 'A';
		    CheckRadioButton(hDlg, IDC_A, IDC_Z, (int) LOWORD(wParam));
		    return (DLGPROC)TRUE ;
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
			return (DLGPROC)TRUE;

		    case IDCANCEL:
			EndDialog (hDlg, FALSE);
			return (DLGPROC)FALSE;
		   }
		   break;
	    }
      return (DLGPROC)FALSE ;
      }
