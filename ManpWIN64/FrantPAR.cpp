/*
    FRANTPAR.CPP imports legacy Fractint PAR files.

    Fractint PAR files predate ManpWIN's own parameter format and use
    different assumptions for fractal names, colour maps, coordinates,
    formula references, bailout tests, and inline palette encoding.

    This module should be treated as a compatibility translator:

	Fractint PAR text
	    -> Fractint-specific parsed fields
	    -> ManpWIN fractal state

    The long-term cleanup goal is to keep Fractint-specific quirks isolated
    here while applying the converted result through normal ManpWIN metadata,
    palette, coordinate, and formula systems.

    Written in Microsoft Visual 'C++' by Paul de Leeuw.
*/

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include "manpwin.h"
#include "manp.h"
#include "colour.h"
#include "fractype.h"
#include "fractalp.h"
#include "fract.h"
#include "complex.h"
#include "BigDouble.h"
#include "big.h"
#include "pixel.h"
#include "SafeStrings.h"
#include "..\parser\TrigFn.h"

#define	INSIDE	TRUE
#define	OUTSIDE	FALSE

extern	char	MAPFile[];		// colour map file
extern	short int ismand;		// parser version of the inverse of juliaflag
extern	double	HenonA, HenonXStart, HenonYStart, HenonStep;
extern	char	lsys_type[];
Complex	RotationCentre;			// centre of rotation
extern	double	z_rot;			// angle display plane to z axis 

extern	struct	FractintFilterStuff	FractintFilter[];	// default values for each 

extern	char	lptr[][100];
char	par_type[80];			// for display
extern	char	lsys_Label[];		// for display in type selection
extern	int	lsys_ptr;
extern	int	lsys_num;
static	char	loaded = 0;
static	int	endloop;		// ensure a clean exit
static	double	param1, param2, param3, param4, param5, param6;

//int	par(HWND, char *);
int	ParLoad(HWND, char *);

extern	char	*str_find_ci(char *, char *);
extern	short	FilePalette(HWND, char *, char *);
extern	int	ifsload(HWND, char *);
extern	int	load_lsystems(HWND, char *);
extern	int	fpFormulaSetup(char *);
extern	int	FindFunct(char *);
extern	int	ProcessFormulaString(char *);
extern	void	cvtcentermag(double *, double *, LDBL *, double *, double *, double *);

extern	char	FRMPath[];		// path for formula files
extern	char	IFSPath[];		// path for formula files

struct FractintFilterStuff		// database of Fractint Outside filters
    {
    char    *name;			// name of the fractal 
    char    method;			// only allow '1', '2', blinds or spiral 
    };

extern	Complex	j;

/**************************************************************************
	Remove trailing spaces and newlines from filenames
**************************************************************************/

char	*trailing(char *instr)
    {
    char *s;

    s = instr + strlen(instr) - 1;
    while ((*s == '\n') || (*s == ' '))
	*s-- = '\0';
    return(instr);
    }

/**************************************************************************
	Check if file and formula exists - 
	This saves warning messages if it doesn't 
	because there may still be a formula in the par file
**************************************************************************/

int	CheckFileAndFormulaExist(char *filename, char *lsys_type)
    {
    FILE	*fp;
    char	InLine[200];
    char	*word;
    int		linenum = 0;

    if ((fp = fopen(filename, "rt")) == NULL)
	return -1;
    else
	{
	do
	    {
	    if (fgets(InLine, 160, fp) == 0)
		return -1;

	    if ((word = strchr(InLine, ';')))	// strip comment
		*word = 0;
	    ++linenum;
	    } while (str_find_ci(InLine, lsys_type) == 0);
	    fclose(fp);
	}
    return 0;
    }

/**************************************************************************
	Analyse the formula file and type
**************************************************************************/

int	AnalyseFormula(char *FormulaData)
    {
    char    *tok, *p, *q;
    char    filename[MAX_PATH];
    char    temp[MAX_PATH];

    p = FormulaData;
    q = temp;
    while (*p != ' ')
	{
	*q = *p;
	p++;
	q++;
	}
    *q = '\0';
    SAFE_SPRINTF(filename, "%s\\%s", FRMPath, temp);
    if (tok = str_find_ci(FormulaData, "formulaname="))
	{
	p = tok;
	q = lsys_type;
	while (*p != ' ')
	    {
	    *q = *p;
	    p++;
	    q++;
	    }
	*q = '\0';
	}

    // check if file and formula exists - saves warning messages if it doesn't because there may still be a formula in the par file
    if (CheckFileAndFormulaExist(filename, lsys_type) < 0)
	return -1;
    if (fpFormulaSetup(filename) <= 0)
	return -1;
    return 0;
    }

/**************************************************************************
	Analyse the IFS file and type
**************************************************************************/

int	AnalyseIFS(HWND hwnd, char *ifsdata)
    {
    char    *tok, *p, *q;
    char    filename[MAX_PATH];
    char    temp[MAX_PATH];

    p = ifsdata;
    q = temp;
    while (*p != ' ')
	{
	*q = *p;
	p++;
	q++;
	}
    *q = '\0';
    SAFE_SPRINTF(filename, "%s\\%s", IFSPath, temp);
    if (tok = str_find_ci(ifsdata, "ifs="))
	{
	p = tok;
	q = lsys_type;
	while (*p != ' ')
	    {
	    *q = *p;
	    p++;
	    q++;
	    }
	*q = '\0';
	}

    if (ifsload(hwnd, filename) < 0)
	return -1;
    return 0;
    }
/**************************************************************************
	Analyse the LSystem file and type
**************************************************************************/

int	AnalyseLSystem(HWND hwnd, char *lsysdata)
    {
    char    *tok, *p, *q;
    char    filename[MAX_PATH];
    char    LsysName[MAX_PATH];
    int	    k;
    BOOL    flag = FALSE;

    p = lsysdata;
    q = filename;
    while (*p != ' ')
	{
	*q = *p;
	p++;
	q++;
	}
    *q = '\0';
    if (tok = str_find_ci(lsysdata, "lname="))
	{
	p = tok;
	q = LsysName;
	while (*p != ' ')
	    {
	    *q = *p;
	    p++;
	    q++;
	    }
	*q = '\0';
	}

    if (load_lsystems(hwnd, filename) < 0)	// load all Lsystems in file
	return -1;
    for (k = 0; k < lsys_num; k++)
	{
	if (strcmp(LsysName, lptr[k]) == 0)	// find the one in the PAR file
	    {
	    flag = TRUE;
	    break;
	    }
	}
    if (!flag)					// no item found
	return -1;
    else
	lsys_ptr = k;
    strcpy(lsys_type, LsysName);
    gManp->param[0] = param1;
    gManp->degree = (int)gManp->param[0];
    return 0;
    }


/**************************************************************************
	Analyse the function type
**************************************************************************/

int	CManp::AnalyseFunction(char *Trigdata)
    {
    static  char	fn1[16], fn2[16]/*, s[1024]*/;
    int		numfn;
    int		FnPtr;
    CTrigFn	TrigFn;
    std::vector<char> s(strlen(Trigdata) + 1);

#ifdef DEBUG
    _snprintf_s(s, 1024, _TRUNCATE, "Length %d", strlen(Trigdata));
    MessageBox (hwnd, Trigdata, s, MB_ICONEXCLAMATION | MB_OK);
#endif   

    strcpy_s(s.data(), s.size(), Trigdata);			// don't splatter main string
    for (char* t = s.data(); *t; ++t)
	{
	if (*t == ' ')
	    {
	    *t = '\0';
	    break;
	    }

	if (*t == '/')
	    *t = ' ';
	}
	
    numfn = sscanf_s(s.data(), "%s %s", fn1, (unsigned)_countof(fn1), fn2, (unsigned)_countof(fn2));
    if (numfn == 1 || numfn == 2)
	if ((FnPtr = FindFunct(fn1)) >= 0)
	    {
	    Fractal.Fn1 = TrigFn.FunctList[FnPtr];
	    Fractal.Fn1Index = FnPtr;
	    }

    if (numfn == 2)
	if ((FnPtr = FindFunct(fn2)) >= 0)
	    {
	    Fractal.Fn2 = TrigFn.FunctList[FnPtr];
	    Fractal.Fn2Index = FnPtr;
//	    dtrig1 = *FnctList[FnPtr].ptr;	// load function pointer 
	    }
    if (numfn == 1 || numfn == 2)
	Fractal.NumFunct = numfn;
    return 0;
    }

/**************************************************************************
	Find the fractal type
**************************************************************************/

int	CManp::FindType(HWND hwnd, char *FractType, char *FractName, bool *IsFrm, double TempRqlim)
    {
    char	*tok;
    int		k;
    CTrigFn	TrigFn;
    char	TempLyapSequence[120];		// hold the AB sequence for Lyapunov fractals

    std::vector<char> tmpstr(strlen(FractType) + 1);

    strcpy_s(tmpstr.data(), tmpstr.size(), FractType);			// don't splatter main string

    juliaflag = FALSE;
    sscanf(FractType, "%s", FractName);
    if (tok = str_find_ci(tmpstr.data(), "ismand="))
	{
	juliaflag = (*tok == 'y') ? false : true;
	ismand = !juliaflag;
	}
    if (!_strnicmp(FractType, "lsystem", 6))
	{
	type = LSYSTEM;
	if (tok = str_find_ci(FractType, "lfile="))
	    return (AnalyseLSystem(hwnd, tok));
	return 0;
	}
    if (!_strnicmp(FractType, "ManDerivatives", 14))
	{
	type = MANDELDERIVATIVES;
	subtype = (int)param1;
	degree = (WORD)param2;
	return 0;
	}
    if (!_strnicmp(FractType, "Perturbation", 12))
	{
	type = PERTURBATION;
	subtype = (int)param1;
	degree = (WORD)param2;
	return 0;
	}
    if (!_strnicmp(FractType, "ArtMatrix", 9))
	{
	subtype = (int)param2;
	special = (int)param3;
	gManp->param[0] = subtype;
	switch ((int)param1)
	    {
	    case 0:
		type = CUBIC;
		gManp->param[1] = special;
		break;
	    case 1:
		type = SPECIALNEWT;
		gManp->param[0] = special;
		break;
	    case 2:
		type = MATEIN;
		break;
	    case 3:
		type = RATIONALMAP;
		gManp->param[1] = special;
		break;
	    default:
		type = CUBIC;
		gManp->param[1] = special;
		break;
	    }
	return 0;
	}
    if (!_strnicmp(FractType, "Tierazon", 8))
	{
	type = TIERAZON;
	subtype = (int)param1;
	degree = (WORD)param2;
	return 0;
	}
    if (!_strnicmp(FractType, "lyapunov", 8))
	{
	long	i = (long)param1;		// used to decode lyapunov sequence from gManp->param[0]
	int	t, r;
	int	lyaRxy[34];
	int	lyaLength = 1;

	type = LYAPUNOV;
	param1 = param2;			// ManpWIN uses these parameters differently than FractInt
	param2 = param3;
	lyaRxy[0] = 1;
	for (t = 31; t >= 0; t--)
	    if (i & (1 << t)) break;
	for (; t >= 0; t--)
	    lyaRxy[lyaLength++] = (i & (1 << t)) != 0;
	lyaRxy[lyaLength++] = 0;
	for (r = 0; r < lyaLength; r++)
	    TempLyapSequence[r] = lyaRxy[r] ? 'A' : 'B';
	TempLyapSequence[r] = '\0';

	//	return 0;
	}
    if (!_strnicmp(FractType, "ifs", 3))
	{
	type = IFS;
	if (tok = str_find_ci(FractType, "ifsfile="))
	    return (AnalyseIFS(hwnd, tok));
	return 0;
	}
    if (!_strnicmp(FractType, "formula", 7))
	{
	int	result;

	type = FORMULA;
	gManp->param[0] = param1;				// this stuff is still experimental
	gManp->param[1] = param2;
	gManp->param[2] = param3;
	gManp->param[3] = param4;
	gManp->param[4] = param5;
	gManp->param[5] = param6;
	if (tok = str_find_ci(FractType, "formulafile="))
	    {
	    result = AnalyseFormula(tok);
	    if (result == 0)
		{
		return 0;
		}
	    else
		*IsFrm = true;		// let's see if we can find a formula in the par file
	    }
	else	
	    *IsFrm = true;
	}
    for (k = 0; fractalspecific[k].name != NULL; k++)
	{
	if (strcmp(FractName,(*fractalspecific[k].name == '*') ? fractalspecific[k].name + 1 : fractalspecific[k].name) == 0)
	    break;
	}
    if (fractalspecific[k].name == NULL)
	{
	return -1;
	}
    else
	{
	InitFract(k);
	if (k == LYAPUNOV)
	    strcpy(LyapSequence, TempLyapSequence);	// prevent it being splattered by the default in InitFract()
	if (TempRqlim > 0.0)
	    rqlim = TempRqlim;				// prevent it being splattered by the default in InitFract()
	type = k;
	gManp->param[0] = param1;				// this stuff is still experimental
	gManp->param[1] = param2;
	gManp->param[2] = param3;
	gManp->param[3] = param4;
	gManp->param[4] = param5;
	gManp->param[5] = param6;
	switch (type)
	    {
	    // start with Julia fractals
	    case LLAMBDAFNFN:
	    case FPLAMBDAFNFN:
	    case LJULFNFN:
	    case FPJULFNFN:
	    case BARNSLEYJ1FP:
	    case BARNSLEYJ2FP:
	    case BARNSLEYJ3FP:
	    case FPPOPCORNJUL:
	    case LPOPCORNJUL:
	    case MANOWARJ:
	    case MANOWARJFP:
	    case LJULIAZPOWER:
		juliaflag = TRUE;
		j.x = gManp->param[0];
		j.y = gManp->param[1];
		break;
	    case COMPLEXNEWTON:
		gManp->param[4] = 0.0;				// subtype = 'N' or normal
		break;
	    case COMPLEXBASIN:
		gManp->param[4] = 1.0;				// subtype = 'B' or basin
		break;
	    case NEWTON:
		gManp->param[1] = 0.0;				// subtype = 'N' or normal
		break;
	    case NEWTBASIN:
		gManp->param[1] = (gManp->param[1] == 0.0) ? 2.0 : 1.0;	// don't blame me, I didn't invent Fractint
		break;

	    }

	if (tok = str_find_ci(tmpstr.data(), "function="))
	    {
	    if (gManp->AnalyseFunction(tok) < 0)
		{
		return -1;
		}
	    }
	else
	    {
	    int	FnPtr;

	    if (fractalspecific[type].numfn > 0)	// oh dear! no functions specified, better use default.
		{
		if ((FnPtr = FindFunct(fractalspecific[type].fn1)) >= 0)
		    {
		    Fractal.Fn1 = TrigFn.FunctList[FnPtr];
		    Fractal.NumFunct = fractalspecific[type].numfn;
		    }
		}
	    if (fractalspecific[type].numfn > 1)	// oh dear! a second function exists, better use default.
		{
		if ((FnPtr = FindFunct(fractalspecific[type].fn2)) >= 0)
		    {
		    Fractal.Fn2 = TrigFn.FunctList[FnPtr];
		    }
		}
	    }

	if (fractalspecific[k].juliaflag == JULIAFP)
	    {
	    juliaflag = TRUE;
	    j.x = gManp->param[0];
	    j.y = gManp->param[1];
	    }
	else
	    juliaflag = FALSE;
	}
    return 0;
    }

/**************************************************************************
	Load Palette Map
**************************************************************************/

int	CManp::ProcessBailoutTest(char *value)
    {
    int	test;
    if (strncmp(value, "mod", 3) == 0)
	test = BAIL_MOD;
    else if (strncmp(value, "real", 4) == 0)
	test = BAIL_REAL;
    else if (strncmp(value, "imag", 4) == 0)
	test = BAIL_IMAG;
    else if (strncmp(value, "or", 2) == 0)
	test = BAIL_AND;
    else if (strncmp(value, "and", 3) == 0)
	test = BAIL_AND;
    else if (strncmp(value, "manh", 4) == 0)
	test = MANH;
    else if (strncmp(value, "manr", 4) == 0)
	test = MANR;
    else
	test = BAIL_MOD;
    return test;
    }

/**************************************************************************
	Load Palette Map
**************************************************************************/

bool	CManp::DecodeFractintColourChar(char c, BYTE& out)
    {
    int k;

    if (c >= '0' && c <= '9')
	k = c - '0';
    else if (c >= 'A' && c <= 'Z')
	k = c - ('A' - 10);
    else if (c >= '_' && c <= 'z')
	k = c - ('_' - 36);
    else
	return false;

    out = (BYTE)k;   // still 0..63 here
    return true;
    }

int	CManp::ParseColours(char *value)
    {
    int		i, j;
    int		smooth, PalSize;
    char	*p, *q;

//    SetPalettePointer(PalettePtr);
    if (*value == '@') 
	{
	p = value;
	q = MAPFile;
	while (*p++ )			// extract filename
	    {
	    if (*p == '}' || *p == ' ' || *p == '\n')
		break;
	    *q++ = *p;
	    }
	*q = '\0';
	if (str_find_ci(MAPFile, "map") == 0)		// some fractint par files assume the extension 
	    strcat_s(MAPFile, MAX_PATH, ".map");
	FilePalette(GlobalHwnd, MAPFile, "Fractint Par: Get Colour Map");
	TrueCol.FillPalette(REPEAT, TrueCol.PalettePtr, threshold);
	TrueCol.FinalisePalette(256, threshold);

#ifdef DEBUG
    for (i = 0; i < 8; i++)
	{
	_snprintf_s(s, 150, _TRUNCATE, "Col[%d]: %d %d %d", i, *(PalettePtr + 3 * i),
				    *(PalettePtr + 3 * i + 1), *(PalettePtr + 3 * i + 2));
	MessageBox (hwnd, s, "File colours", MB_ICONEXCLAMATION | MB_OK);
	}
#endif   

	return 0;
	}
    else 
	{
	_snprintf_s(MAPFile, _MAX_PATH, _TRUNCATE, "Colour info from Fractint Par File");
	i = smooth = 0;
	while (*value) 
	    {
	    if (i >= 256) 
//		goto badcolor;
		break;
	    if (*value == '<') 
		{
		if (i == 0 || smooth
		  || (smooth = atoi(value+1)) < 2
		  || (value = strchr(value,'>')) == NULL)
		{}
//		goto badcolor;
		i += smooth;
		++value;
		}
	    else 
		{
		for (j = 0; j < 3; ++j)
		    {
		    BYTE k6;

		    if (!DecodeFractintColourChar(*value++, k6))
			goto badcolor;

		    // store raw 6-bit value for now
		    // Fractint inline PAR colours are being decoded into RGBTRIPLE storage.
		    // RGBTRIPLE memory order is Blue, Green, Red.
		    if (j == 0) TrueCol.PalettePtr[i].rgbtBlue = k6;
		    if (j == 1) TrueCol.PalettePtr[i].rgbtGreen = k6;
		    if (j == 2) TrueCol.PalettePtr[i].rgbtRed = k6;

		    if (smooth)
			{
			int start = i - (smooth + 1);
			int spread = smooth + 1;
			int cnum = 0;

			BYTE startVal;

			if (j == 0) startVal = TrueCol.PalettePtr[start].rgbtBlue;
			if (j == 1) startVal = TrueCol.PalettePtr[start].rgbtGreen;
			if (j == 2) startVal = TrueCol.PalettePtr[start].rgbtRed;

			if (k6 == startVal)
			    {
			    while (++cnum < spread)
				{
				if (j == 0) TrueCol.PalettePtr[start + cnum].rgbtBlue = k6;
				if (j == 1) TrueCol.PalettePtr[start + cnum].rgbtGreen = k6;
				if (j == 2) TrueCol.PalettePtr[start + cnum].rgbtRed = k6;
				}
			    }
			else
			    {
			    while (++cnum < spread)
				{
				BYTE interp = (BYTE)(
				    (cnum * k6 +
				    (i - (start + cnum)) * startVal +
					spread / 2) / spread);

				if (j == 0) TrueCol.PalettePtr[start + cnum].rgbtBlue = interp;
				if (j == 1) TrueCol.PalettePtr[start + cnum].rgbtGreen = interp;
				if (j == 2) TrueCol.PalettePtr[start + cnum].rgbtRed = interp;
				}
			    }
			}
		    }

		smooth = 0;
		++i;
		}
	    PalSize = i;
	    }
	}
    badcolor:
    for (i = 0; i < PalSize; i++)
	{
	TrueCol.PalettePtr[i].rgbtBlue <<= 2;
	TrueCol.PalettePtr[i].rgbtGreen <<= 2;
	TrueCol.PalettePtr[i].rgbtRed <<= 2;
	if (i == TrueCol.inside_colour)
	    {
	    TrueCol.InsideRed = TrueCol.PalettePtr[i].rgbtBlue;		// values for r, g, b channels for inside colour
	    TrueCol.InsideGreen = TrueCol.PalettePtr[i].rgbtGreen;
	    TrueCol.InsideBlue = TrueCol.PalettePtr[i].rgbtRed;
	    }
	}

#ifdef DEBUG
    for (i = 0; i < 16; i++)
	{
	_snprintf_s(s, 150, _TRUNCATE, "Col[%d]: %d %d %d", i, *(PalettePtr + 3 * i),
				    *(PalettePtr + 3 * i + 1), *(PalettePtr + 3 * i + 2));
	MessageBox (hwnd, s, "Par colours", MB_ICONEXCLAMATION | MB_OK);
	}
#endif   
    TrueCol.FinalisePalette(PalSize, threshold);
    return(0);
    }

/**************************************************************************
	Process params
**************************************************************************/

int	CManp::ProcessParams(char *s)
    {
    char	*t;

    t = s;
    while(*s && *s != ' ')
	{
	if (!isdigit(*s) && *s != '.' && *s != '+' && *s != '-' && *s != 'e')
	    *s = ' ';
	s++;
	}
    sscanf(t, "%lf %lf %lf %lf %lf %lf", &param1, &param2, &param3, &param4, &param5, &param6);
    return 0;
    }

/**************************************************************************
	Process Corners
**************************************************************************/

int	CManp::ProcessCorners(char *s, BOOL CentreFlag)
    {
    char	*t;

    std::vector<char> s1(SIZEOF_BF_VARS);
    std::vector<char> s2(SIZEOF_BF_VARS);
    std::vector<char> s3(SIZEOF_BF_VARS);
    std::vector<char> s4(SIZEOF_BF_VARS);
    std::vector<char> s5(SIZEOF_BF_VARS);
    std::vector<char> s6(SIZEOF_BF_VARS);

    std::vector<char> CoordText(SIZEOF_BF_VARS * 3);		// make sure we can hold 3 bignum co-ordinates

    double	Magnification = 1.0;
    double	Xmagfactor = 1.0;
    double	Rotation = 0.0;
    double	Skew = 0.0;
    double	floatval[6]{ 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };	// pre-parsed floating parms
    Complex	centre{ 0.0, 0.0 };
    CMatrix	Mat;						// transformation and rotation matrix

    int		count, NumCorners = 0;
    BigDouble   BigMag, temp, OneOverMag;

    strncpy_s(CoordText.data(), CoordText.size(), s, _TRUNCATE);
    char* p = CoordText.data();
    while (*p && *p != ' ')
	{
	if (!isdigit((unsigned char)*p) &&
	    *p != '.' &&
	    *p != '+' &&
	    *p != '-' &&
	    *p != 'e' &&
	    *p != 'E')
	    *p = ' ';

	p++;
	}

    t = CoordText.data();

    if (CentreFlag)
	{
	NumCorners = sscanf_s(t, "%lf %lf %lf %lf %lf %lf", &hor, &vert, &Magnification, &Xmagfactor, &Rotation, &Skew);

	if (NumCorners != 3 && NumCorners != 6)
	    {
	    OutputDebugStringA("Can't convert fractal location in Fractint par file\n");
	    return -1;
	    }
	RotationCentre.x = hor;
	RotationCentre.y = vert;
	hor -= (AspectRatio / Magnification);
	vert -= (1.0 / Magnification);
	mandel_width = 2.0 / Magnification;
	}
    else
	{
	NumCorners = sscanf_s(t, "%lf %lf %lf %lf %lf %lf", &floatval[0], &floatval[1], &floatval[2], &floatval[3], &floatval[4], &floatval[5]);
	if (NumCorners == 4)										// no rotation or skew
	    {
	    hor = floatval[0];
	    vert = floatval[2];
	    mandel_width = vert - floatval[3];
	    Rotation = 0.0;
	    Skew = 0.0;
	    }
	else if (NumCorners == 6)									// rotation and skew 
	    {
	    LDBL    mag;
	    xxmin = floatval[0];
	    xxmax = floatval[1];
	    yymin = floatval[2];
	    yymax = floatval[3];
	    xx3rd = floatval[4];
	    yy3rd = floatval[5];
	    cvtcentermag(&centre.x, &centre.y, &mag, &Xmagfactor, &Rotation, &Skew);
	    Magnification = mag;
	    hor = centre.x - (AspectRatio / Magnification);
	    vert = centre.y - (1.0 / Magnification);
	    mandel_width = 2.0 / Magnification;
	    RotationCentre.x = hor + (AspectRatio / Magnification);
	    RotationCentre.y = vert + (1.0 / Magnification);
	    }
	else
	    {
	    OutputDebugStringA("Can't convert fractal corner coordinates in Fractint par file\n");
	    return -1;
	    }
	}

    RotationAngle = (int)Rotation;
    if (Rotation == 0.0 || Rotation == 90.0 || Rotation == 180.0 || Rotation == 270.0)		// save calcs in rotating, just remap
	{
	RotationCentre = 0.0;
	}
    else
	{
	Mat.InitTransformation(RotationCentre.x, RotationCentre.y, 0.0, 0.0, 0.0, Rotation);
	}

    if (gManp->mandel_width < 0.0)
	gManp->mandel_width = -gManp->mandel_width;

    count = sscanf_s(t, "%s %s %s %s %s %s",
	s1.data(), (unsigned)s1.size(),
	s2.data(), (unsigned)s2.size(),
	s3.data(), (unsigned)s3.size(),
	s4.data(), (unsigned)s4.size(),
	s5.data(), (unsigned)s5.size(),
	s6.data(), (unsigned)s6.size());

    if (count < 4)
	{
	OutputDebugStringA("Invalid Fractint coordinate format\n");
	return -1;
	}

    precision = gManp->getprecbf_mag();
    if (precision < 0)							// exceeded allowable precision
	{
	OutputDebugStringA("Exceeded allowable precision in Fractint par file\n");
	return -1;
	}
    if (precision > DBL_DIG - 3 && NumCorners != 6)			// bignum support not available yet
	{
	decimals = precision + PRECISION_FACTOR;

	if (gManp->ChangeBigPrecision(decimals) < 0)			// increase precision of Big numbers	
	    {
	    OutputDebugStringA("Exceeded allowable precision in Fractint par file\n");
	    return -1;							// too many decimals for library
	    }
	BigNumFlag = TRUE;
	BigBailout = rqlim;
	ConvertString2Bignum(BigHor.x, s1.data());
	ConvertString2Bignum(BigVert.x, CentreFlag ? s2.data() : s3.data());
	if (CentreFlag)
	    {
	    ConvertString2Bignum(BigMag.x, s3.data());
	    if (mpfr_zero_p(BigMag.x))					// no naughty division
		{
		OutputDebugStringA("Naughty division in Fractint par file\n");
		return -1;
		}
	    OneOverMag = BigMag.BigInvert();
	    BigHor = BigHor - OneOverMag * AspectRatio;
	    BigVert = BigVert - OneOverMag;
	    BigWidth = OneOverMag + OneOverMag;
	    }
	else
	    {
	    ConvertString2Bignum(temp.x, s4.data());		// mandel_width = vert - a2;
	    BigWidth = BigVert - temp;
	    if (mpfr_sgn(BigWidth.x) == 0)	// no naughty division
		{
		OutputDebugStringA("Naughty division in Fractint par file\n");
		return -1;
		}
	    }
	}
    else
	{
	BigNumFlag = FALSE;
	if (mandel_width < DBL_MIN)
	    mandel_width = 1.0;
	}
    return 0;
    }

/**************************************************************************
	Process Outside Filters
**************************************************************************/

int	CManp::Processfilters(char *s, BOOL IsInside)
    {
    char	FilterName[24];
    int		k, method;
    BOOL	flag = FALSE;

    sscanf(s, "%s", FilterName);
    for (k = 0; FractintFilter[k].name != NULL; k++)
	{
	if (str_find_ci(FilterName, FractintFilter[k].name))	// find the filter type
	    {
	    flag = TRUE;
	    break;
	    }
	}
    method = (flag) ? FractintFilter[k].method : NONE;
    if (IsInside)
	InsideMethod = method;
    else
	OutsideMethod = method;
    if (IsInside && !flag)
	TrueCol.inside_colour = atoi(FilterName);
    return 0;
    }

/**************************************************************************
	Process Potential parameters
**************************************************************************/

int	CManp::ProcessPotential(char *s)
    {
    char	*t;

    t = s;
    while (*s && *s != ' ')
	{
	//  if (!isdigit(*s) && *s != '.' && *s != '+' && *s != '-')
	if (!isdigit(*s) && *s != '.' && *s != '+' && *s != '-' && *s != 'e')
	    *s = ' ';
	s++;
	}

    sscanf(t, "%lf %lf %lf", &gManp->potparam[0], &gManp->potparam[1], &gManp->potparam[2]);

    return 0;
    }

/**************************************************************************
	    Read the par file
**************************************************************************/

#define BUFFERSIZE  5000

char	*leading(char *instr) // strips leading spaces 
    {
    char *s = instr;
    while (*s && isspace(*s))
	s++;
    return s;
    }

char	*StripStuff(char *instr) // strips backslash and newlines 
    {
    char *s = instr;
    while (*s)
	{
	if (*s == '\\'/* || *s == '\n' || *s == '\r'*/)
	    {
	    *s = '\0';
	    return instr;
	    }
	if (*s == '\n')
	    {
	    *s = ' ';
	    return instr;
	    }
	s++;
	}
    return instr;
    }

static	int	ReadParFile(HWND hwnd, char *filename)
    {
    int		i;
    char	*tok, *q;
    int		err = 0;
    int		linenum = 0, check = 0;
    char	*word;
    char	InLine[200];
    char	InLine1[200];
    FILE	*fp;
    char	s[200];
    bool	IsFrm = false;		// do we have a formula?
    char	tmp[164];
    double	TempRqlim = -1.0;
    std::vector<char> temp(BUFFERSIZE);

    gManp->param[0] = param1 = 0.0;
    gManp->param[1] = param2 = 0.0;
    gManp->param[2] = param3 = 0.0;
    gManp->param[3] = param4 = 0.0;
    gManp->param[4] = param5 = 0.0;
    gManp->param[5] = param6 = 0.0;
    gManp->invert = FALSE;
    if ((fp = fopen(filename, "r")) == NULL)
	{
	_snprintf_s(s, 200, _TRUNCATE, "Can't Open par File: <%s>", filename);
	MessageBox (hwnd, s, "MANPWIN", MB_ICONEXCLAMATION | MB_OK);
	return -1;
	}

    do
	{
	char	*a, *b;
	if (fgets(InLine, 160, fp) == 0)
	    {
	    _snprintf_s(s, 200, _TRUNCATE, "Can't Get FRACTINT Par Data: <%s>", filename);
	    MessageBox (hwnd, s, "MANPWIN", MB_ICONEXCLAMATION | MB_OK);
	    fclose(fp);
	    return -1;
	    }

	a = InLine;
	b = tmp;
	while (*a && *a != ' ' && *a != '\t')	// remove trailing white scharacters for final comparison
	    {
	    *b = *a;
	    a++;b++;
	    }
	*b = '\0';

	if ((word = strchr(InLine, ';')))	// strip comment
	    *word = 0;
	++linenum;
	}
	while (_stricmp(tmp, lptr[lsys_ptr]) != 0);

	std::vector<char> buffer(BUFFERSIZE);
	buffer[0] = '\0';

    while(fgets(InLine1, 160, fp))		// Max line length 160 chars
	{
	linenum++;
   
	if ((word = strchr(InLine1,';')))	// strip comment
	    *word = 0;
	strcpy(InLine, StripStuff(InLine1));
	strcat_s(buffer.data(), buffer.size(), leading(InLine));
	if (str_find_ci(InLine, "}"))
	     break;
	if ((i = (int)strlen(buffer.data())) > BUFFERSIZE - 160)
	    {
	    _snprintf_s(s, 200, _TRUNCATE, "Par Data full, line: <%d>", linenum);
	    MessageBox (hwnd, s, "ManpWIN", MB_ICONEXCLAMATION | MB_OK);
	    break;
	    }
	}    

    if (tok = str_find_ci(buffer.data(), "params="))		// must be first
	gManp->ProcessParams(tok);
    if (tok = str_find_ci(buffer.data(), "maxiter="))
	{
	gManp->threshold = atol(tok);
	if (gManp->threshold < 0L)
	    gManp->threshold = MAXTHRESHOLD;
	}
    if (tok = str_find_ci(buffer.data(), "inside="))
	gManp->Processfilters(tok, INSIDE);
    if (tok = str_find_ci(buffer.data(), "bailout="))
	TempRqlim = (double)atoi(tok);
    if (tok = str_find_ci(buffer.data(), "decomp="))
	gManp->decomp = atoi(tok);
    if (tok = str_find_ci(buffer.data(), "potential="))
	{
	gManp->ProcessPotential(tok);
	gManp->OutsideMethod = POTENTIAL;
	}
    if (tok = str_find_ci(buffer.data(), "corners="))
	gManp->ProcessCorners(tok, FALSE);
    if (tok = str_find_ci(buffer.data(), "center-mag="))
	gManp->ProcessCorners(tok, TRUE);
    if (tok = str_find_ci(buffer.data(), "biomorph="))
	gManp->biomorph = atoi(tok);
    if (tok = str_find_ci(buffer.data(), "function="))
	gManp->AnalyseFunction(tok);
    if (tok = str_find_ci(buffer.data(), "colors="))
	gManp->ParseColours(tok);
    if (tok = str_find_ci(buffer.data(), "invert="))
	gManp->ProcessInvert(tok);
    if (tok = str_find_ci(buffer.data(), "outside="))
	gManp->Processfilters(tok, OUTSIDE);
    if (tok = str_find_ci(buffer.data(), "bailoutest="))
	gManp->BailoutTestType = gManp->ProcessBailoutTest(tok);
    if (tok = str_find_ci(buffer.data(), "type="))		// must be the only one which is after strlwr
	{
	if (gManp->FindType(hwnd, tok, buffer.data(), &IsFrm, TempRqlim) < 0)
	    {
	    switch (gManp->type)
		{
		case LSYSTEM:
		    _snprintf_s(s, 200, _TRUNCATE, "Error in L-system file");
		    break;
		case IFS:
		    _snprintf_s(s, 200, _TRUNCATE, "Error in IFS file");
		    break;
		case FORMULA:
		    _snprintf_s(s, 200, _TRUNCATE, "Error in Formula file");
		    break;
		default:
		    _snprintf_s(s, 200, _TRUNCATE, "Fractal type <%s> not supported by ManpWin", temp.data());
		    break;
		}
	    MessageBox (hwnd, s, "Reading Fractint Par File", MB_ICONEXCLAMATION | MB_OK);
	    fclose(fp);
	    return -1;
	    }
	else
	    gManp->RebuildFractalMetadata(gManp->type, gManp->subtype);		// load all the metadata for parameters
	}
    if (IsFrm)
	{
	char *p;
	do
	    {
	    if (fgets(InLine, 160, fp) == 0)
		{
		_snprintf_s(s, 200, _TRUNCATE, "Can't Find formula in the FRACTINT Par Data: <%s>", filename);
		MessageBox(hwnd, s, "MANPWIN", MB_ICONEXCLAMATION | MB_OK);
		fclose(fp);
		return -1;
		}

#ifdef DEBUG
	    _snprintf_s(s, 200, _TRUNCATE, "Line %d, <%s>", linenum, InLine);
	    MessageBox(hwnd, s, "Finding Par", MB_ICONEXCLAMATION | MB_OK);
#endif   

	    ++linenum;
	    } while (str_find_ci(InLine, "frm:") == 0);

	temp[0] = '\0';

	// now let's interpret any formula if present
	while (fgets(InLine, 160, fp))		// Max line length 160 chars
	    {

#ifdef DEBUG
	    _snprintf_s(s, 200, _TRUNCATE, "Line %d, <%s>, <%s>", linenum, InLine, lptr[lsys_ptr]);
	    MessageBox(hwnd, s, "Reading Par File", MB_ICONEXCLAMATION | MB_OK);
#endif   


	    linenum++;

	    if ((word = strchr(InLine, ';')))	// strip comment
		*word = 0;
	    strcat_s(temp.data(), BUFFERSIZE, InLine);
	    if (str_find_ci(InLine, "}"))
		break;
	    if ((i = (int)strlen(temp.data())) > BUFFERSIZE - 200)
		{
		_snprintf_s(s, 200, _TRUNCATE, "Par Frm Data full, line: <%d>", linenum);
		MessageBox(hwnd, s, "ManpWIN", MB_ICONEXCLAMATION | MB_OK);
		fclose(fp);
		break;
		}
	    }
	p = temp.data();
	q = gManp->FormulaString;

	bool	WasBackslash = false;
	while (*p)
	    {
	    if (*p == '\n')				// replace returns with ',' as long as it isn't after a backslash or we already have a comma
		{
		if (!WasBackslash)
		    {
		    if (*(p - 1) != ',')
			{
			*q = ',';
			q++;
			}
		    }
		else
		    WasBackslash = false;
		p++;
		}
	    else if (*p == '}' || *p == ';')		// nemove '}' and comments
		{
		*q = '\0';
		break;
		}
	    else if (*p == ':')				// nemove ',' after ':'
		{
		*q = *p;
		p++; q++;
		p++;
		}
	    else if (*p == '\\')			// remove '\'
		{
		WasBackslash = true;
		p++;
		}
	    else if (*p == '\r')
		p++;					// remove linefeeds
	    else if (*p == ' ' || *p == '\t')
		p++;					// remove spaces
	    else
		{
		*q = *p;
		p++; q++;
		}
	    }
	*q = '\0';

	ProcessFormulaString(gManp->FormulaString);
	}
    if (gManp->type == FRACTPAR)				// if we haven't found a fractal, then we still have fractal type FRACTPAR
	{
	gManp->type = MANDELFP;
	_snprintf_s(s, 200, _TRUNCATE, "No fractal type was found <%s>. Assume Mandelbrot", temp.data());
	MessageBox (hwnd, s, "Reading Fractint Par File", MB_ICONEXCLAMATION | MB_OK);
	}
    fclose(fp);
    return 0;
    }

/**************************************************************************
	Main subroutine
**************************************************************************/

int	FractintPar(HWND hwnd, char *filename)

    {
    if ((!loaded) && ParLoad(hwnd, filename)) 
	return (-1);
    return 0;
    }

/**************************************************************************
	Load par file Name
**************************************************************************/

int	ParLoad(HWND hwnd, char *filename)

    {
    if (ReadParFile(hwnd, filename) < 0)
	{							/* error occurred */
	loaded = 0;
	return -1;
	}
    return 0;
    }

/**************************************************************************
	Compare function for L-System name sort
**************************************************************************/

int	compare_par(char *p1, char *p2)

    {
    return (_strnicmp(p1, p2, 20));
    }

/**************************************************************************
	Load L_System names from file
**************************************************************************/

int	load_par(HWND hwnd, char *filename)

    {
    FILE	*fp;
    char	s[250];
    int	i, j;

    strcpy(par_type, "Not Loaded Yet");
    if ((fp = fopen(filename, "r")) == NULL)
	{
	_snprintf_s(s, 250, _TRUNCATE, "Can't Open Par File: <%s>", filename);
	MessageBox (hwnd, s, "ManpWIN", MB_ICONEXCLAMATION | MB_OK);
	return -1;
	}

    lsys_num = 0;
    while (fgets(s, 160, fp) != NULL)
	{
	if (str_find_ci(s, "{") != 0 && str_find_ci(s, "comment") == 0)
	    {
	    for (i = 0; s[i] == ' '; ++i);
	    for (j = 0; s[j]; ++j)
		if (s[j] == '{' || s[j] == '\n' || s[j] == ' ')
		    s[j] = '\0';
	    s[57] = '\0';
	    strcpy(lptr[lsys_num], s + i);
	    if (lsys_num < 64)
		++lsys_num;
	    else
		break;
	    }
	}

//    qsort((void *)lptr, (size_t)lsys_num, sizeof(lptr[0]), (int (*)(void *, void *)) compare_lsys);
    qsort((void *)lptr, (size_t)lsys_num, sizeof(lptr[0]), (int (__cdecl *)(const void *, const void *)) compare_par);
    fclose(fp);
    if (lsys_ptr >= lsys_num)
	lsys_ptr = 0;
    strcpy(lsys_Label, "Fractint Par");
    return 0;
    }

