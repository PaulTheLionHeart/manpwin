/*
    FRANTPAR.CPP a module to import PAR files from FRACTINT.EXE.
    
    Written in Microsoft Visual 'C++' by Paul de Leeuw.

    This program is written in "standard" C. Hardware dependant code
    (console drivers & serial I/O) is in separate machine libraries.
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
#include ".\parser\TrigFn.h"

#define	INSIDE	TRUE
#define	OUTSIDE	FALSE

//////////////////////////////////////////////////////////////////////////////////////////////////////
extern	HWND	GlobalHwnd;				// This is the main windows handle
//////////////////////////////////////////////////////////////////////////////////////////////////////

extern	double	hor;			// horizontal address
extern	double	vert;			// vertical address
extern	double	mandel_width;		// width of display
extern	double	ScreenRatio;		// ratio of width / height for the screen
//extern	int	inside_colour;		// normal 'lake' colour
extern	long	threshold;		// maximum iterations
//extern	short	InsideRed;		// values for r, g, b channels for inside colour
//extern	short	InsideGreen;			
//extern	short	InsideBlue;			
extern	WORD	type;			// M=mand, N=Newton etc
extern	CTrueCol    TrueCol;		// palette info
//extern	BYTE	*PalettePtr;		// points to true colour palette
//extern	CTrueCol    TrueCol;		// palette info
extern	char	MAPFile[];		// colour map file

extern	double	f_radius,f_xcenter,f_ycenter;    // inversion radius, center 
extern	BYTE	juliaflag;		// Julia implementation of fractal
extern	short int ismand;		// parser version of the inverse of juliaflag

extern	int	decomp;			// number of decomposition colours
extern	WORD	degree;			// special colour, phase etc
extern	WORD	special;		// special colour, phase etc
extern	int	subtype;		// B=basin, S=stripe, N=normal
extern	double	HenonA, HenonXStart, HenonYStart, HenonStep;
extern	char	lsys_type[];
extern	double	rqlim;			// bailout level
extern	int	BailoutTestType;	// type of bailout test
Complex	RotationCentre;		// centre of rotation
extern	int	RotationAngle;
extern	double	z_rot;			// angle display plane to z axis 

extern	struct	FractintFilterStuff	FractintFilter[];	// default values for each 

// Big num declarations **********************************************************
extern	BYTE	BigNumFlag;		// True if bignum used
extern	int	decimals, precision;
//extern	bf_t	BigTemp, BigTemp1;
extern	BigDouble	BigBailout, BigHor, BigVert, BigWidth;
// Big num declarations **********************************************************

extern	char	lptr[][100];
char	par_type[80];			// for display
extern	char	lsys_Label[];		// for display in type selection
extern	int	lsys_ptr;
//int	Previouslsys_ptr = 0;		// remember last time
extern	int	lsys_num;
//int	level = 2;
//DWORD	colour = 15;

extern	BOOL	invert;			// invert fractal

static	char	loaded = 0;
static	int	endloop;		// ensure a clean exit
static	double	param1, param2, param3, param4, param5, param6;
extern	double	param[];		// note that param[0] = param1
extern	double	potparam[];
extern	int	InsideMethod;		// inside filters
extern	int	OutsideMethod;		// outside filters
extern	int	biomorph;		// biomorph colour
//extern	void	FinalisePalette(int);
extern	char	FormulaString[];	// used to hold the full formula
extern	char	LyapSequence[];		// hold the AB sequence for Lyapunov fractals

int	par(HWND, char *);
int	ParLoad(HWND, char *);

//extern	char	*str_find(char *, char *);
extern	char	*str_find_ci(char *, char *);
extern	void	InitFract(int);
extern	int	getprecbf_mag(void);
extern	short	FilePalette(HWND, char *, char *);
//extern	void	SetPalettePointer(BYTE *);
//extern	void	cvtcorners(double, double, LDBL, double, double, double);
extern	int	ifsload(HWND, char *);
extern	int	load_lsystems(HWND, char *);
extern	int	fpFormulaSetup(char *);
extern	int	FindFunct(char *);
//extern	void	FillPalette(int, BYTE *);	// if threshold larger than palette, fill or stretch
extern	void	ConvertString2Bignum(mpfr_t, char *);
extern	void	ConvertBignum2String(char *s, mpfr_t num);
extern	int	ChangeBigPrecision(int decimals);
extern	int	ProcessFormulaString(char *);
extern	void	cvtcentermag(double *, double *, LDBL *, double *, double *, double *);
//extern	char	*GetFractalName(void);
extern	CMatrix	Mat;			// transformation and roatation matrix

//static	int	ReadParFile(HWND, char *);
extern	char	FRMPath[];		// path for formula files
extern	char	IFSPath[];		// path for formula files

struct FractintFilterStuff		// database of Fractint Outside filters
    {
    char    *name;			// name of the fractal 
    char    method;			// only allow '1', '2', blinds or spiral 
    };

//extern	CPixel	Pixel;		// routines for escape fractals
extern	CFract	Fractal;
extern	Complex	j;

/**************************************************************************
	Remove trailing spaces and newlines from filenames
**************************************************************************/

char	*trailing(char *instr)

    {
    register char *s;

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
    sprintf(filename, "%s\\%s", FRMPath, temp);
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
    sprintf(filename, "%s\\%s", IFSPath, temp);
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
    param[0] = param1;
    degree = (int)param[0];
    return 0;
    }


/**************************************************************************
	Analyse the function type
**************************************************************************/

int	AnalyseFunction(char *Trigdata)
    {
    char	*s, *t;
    static  char	fn1[16], fn2[16]/*, s[1024]*/;
    int		numfn;
    int		FnPtr;
    CTrigFn	TrigFn;

    if ((s = new char[strlen(Trigdata) + 1]) == NULL)
	return -1;

#ifdef DEBUG
wsprintf(s, "Length %d", strlen(Trigdata));
MessageBox (hwnd, Trigdata, s, MB_ICONEXCLAMATION | MB_OK);
#endif   

    strcpy(s, Trigdata);			// don't splatter main string
    t = s;
    while(*t && *t != ' ')
	{
	if (*t == '/')
	    *t = ' ';
	t++;
	}
    *t = '\0';
    numfn = sscanf(s, "%s %s", fn1, fn2);
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
    delete [] s;
    return 0;
    }

/**************************************************************************
	Find the fractal type
**************************************************************************/

int	FindType(HWND hwnd, char *FractType, char *FractName, bool *IsFrm, double TempRqlim)

    {
    char	*tok, *tmpstr;
    int		k;
    CTrigFn	TrigFn;
    char	TempLyapSequence[120];		// hold the AB sequence for Lyapunov fractals

    if ((tmpstr = new char[strlen(FractType) + 1]) == NULL)
	return -1;

    strcpy(tmpstr, FractType);			// don't splatter main string

    juliaflag = FALSE;
    sscanf(FractType, "%s", FractName);
    if (tok = str_find_ci(tmpstr, "ismand="))
	{
	juliaflag = (*tok == 'y') ? false : true;
	ismand = !juliaflag;
	}
    if (!_strnicmp(FractType, "lsystem", 6))
	{
	type = LSYSTEM;
	if (tok = str_find_ci(FractType, "lfile="))
	    return (AnalyseLSystem(hwnd, tok));
	delete[] tmpstr;
	return 0;
	}
    if (!_strnicmp(FractType, "ManDerivatives", 14))
	{
	type = MANDELDERIVATIVES;
	subtype = (int)param1;
	degree = (WORD)param2;
	delete[] tmpstr;
	return 0;
	}
    if (!_strnicmp(FractType, "Perturbation", 12))
	{
	type = PERTURBATION;
	subtype = (int)param1;
	degree = (WORD)param2;
	delete[] tmpstr;
	return 0;
	}
    if (!_strnicmp(FractType, "ArtMatrix", 9))
	{
	subtype = (int)param2;
	special = (int)param3;
	param[0] = subtype;
	switch ((int)param1)
	    {
	    case 0:
		type = CUBIC;
		param[1] = special;
		break;
	    case 1:
		type = SPECIALNEWT;
		param[0] = special;
		break;
	    case 2:
		type = MATEIN;
		break;
	    case 3:
		type = RATIONALMAP;
		param[1] = special;
		break;
	    default:
		type = CUBIC;
		param[1] = special;
		break;
	    }
	delete[] tmpstr;
	return 0;
	}
    if (!_strnicmp(FractType, "Tierazon", 8))
	{
	type = TIERAZON;
	subtype = (int)param1;
	degree = (WORD)param2;
	delete[] tmpstr;
	return 0;
	}
    if (!_strnicmp(FractType, "lyapunov", 8))
	{
	long	i = (long)param1;		// used to decode lyapunov sequence from param[0]
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
	delete[] tmpstr;
	if (tok = str_find_ci(FractType, "ifsfile="))
	    return (AnalyseIFS(hwnd, tok));
	return 0;
	}
    if (!_strnicmp(FractType, "formula", 7))
	{
	int	result;

	type = FORMULA;
	param[0] = param1;				// this stuff is still experimental
	param[1] = param2;
	param[2] = param3;
	param[3] = param4;
	param[4] = param5;
	param[5] = param6;
	if (tok = str_find_ci(FractType, "formulafile="))
	    {
	    result = AnalyseFormula(tok);
	    if (result == 0)
		{
		delete[] tmpstr;
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
	delete[] tmpstr;
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
	param[0] = param1;				// this stuff is still experimental
	param[1] = param2;
	param[2] = param3;
	param[3] = param4;
	param[4] = param5;
	param[5] = param6;
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
		j.x = param[0];
		j.y = param[1];
		break;
	    case COMPLEXNEWTON:
		param[4] = 0.0;				// subtype = 'N' or normal
		break;
	    case COMPLEXBASIN:
		param[4] = 1.0;				// subtype = 'B' or basin
		break;
	    case NEWTON:
		param[1] = 0.0;				// subtype = 'N' or normal
		break;
	    case NEWTBASIN:
		param[1] = (param[1] == 0.0) ? 2.0 : 1.0;	// don't blame me, I didn't invent Fractint
		break;

	    }

	if (tok = str_find_ci(tmpstr, "function="))
	    {
	    if (AnalyseFunction(tok) < 0)
		{
		delete[] tmpstr;
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
	    j.x = param[0];
	    j.y = param[1];
	    }
	else
	    juliaflag = FALSE;
	}

    delete[] tmpstr;
    return 0;
    }

/**************************************************************************
	Load Palette Map
**************************************************************************/

static int  ProcessBailoutTest(char *value)

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

static int ParseColours(char *value)

    {
    int		i, j, k;
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
	    strcat(MAPFile, ".map");
	FilePalette(GlobalHwnd, MAPFile, "Fractint Par: Get Colour Map");
	TrueCol.FillPalette(REPEAT, TrueCol.PalettePtr, threshold);
	TrueCol.FinalisePalette(256, threshold);

#ifdef DEBUG
    for (i = 0; i < 8; i++)
	{
	wsprintf(s, "Col[%d]: %d %d %d", i, *(PalettePtr + 3 * i), 
				    *(PalettePtr + 3 * i + 1), *(PalettePtr + 3 * i + 2));
	MessageBox (hwnd, s, "File colours", MB_ICONEXCLAMATION | MB_OK);
	}
#endif   



/*
	if (merge_pathnames(MAP_name,&value[1], 3) < 0)
	     init_msg(0,"",&value[1],3);
	if ((int)strlen(value) > FILE_MAX_PATH || ValidateLuts(MAP_name) != 0)
	     goto badcolor;
	if (display3d) 
	    {
	    mapset = 1;
	    }
	else 
	    {
	    if (merge_pathnames(colorfile,&value[1],3)<0)
		init_msg(0,"",&value[1],3);
	    colorstate = 2;
	    }
*/
	return 0;
	}
    else 
	{
	wsprintf (MAPFile, "Colour info from Fractint Par File");
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
		    if ((k = *(value++)) < '0')  
//		    *(VGA_PALETTE + 3 * i + j) = 0;
			goto badcolor;
		    else if (k <= '9')
			k -= '0';
		    else if (k < 'A')            
//		    *(VGA_PALETTE + 3 * i + j) = 0;
			goto badcolor;
		    else if (k <= 'Z')       
			k -= ('A'-10);
		    else if (k < '_' || k > 'z') 
//		    *(VGA_PALETTE + 3 * i + j) = 0;
			goto badcolor;
		    else                     
			k -= ('_'-36);
		    *(TrueCol.PalettePtr + 3 * i + j) = (BYTE)k;
		    if (smooth) 
			{
			int start, spread, cnum;
                      
			start = i - (spread = smooth + 1);
			cnum = 0;
			if ((k - (int)*(TrueCol.PalettePtr + 3 * start + j)) == 0) 
			    {
			    while (++cnum < spread)
				*(TrueCol.PalettePtr + 3 * (start+cnum) + j) = (BYTE)k;
			    }
			else 
			    {
			    while (++cnum < spread)
				*(TrueCol.PalettePtr + 3 * (start+cnum) + j) 
				    = (BYTE)((cnum * (short)(*(TrueCol.PalettePtr + 3 * i + j))
				    + (i-(start+cnum)) * (short)(*(TrueCol.PalettePtr + 3 * start + j))
						+ spread / 2) / (BYTE) spread);
			    }
			}
		    }
		smooth = 0;
		++i;
		}
	    PalSize = i;
	    }
//    if (smooth) 
//	goto badcolor;
//    while (i < 256)  
//	{ // zap unset entries
//	dacbox[i][0] = dacbox[i][1] = dacbox[i][2] = 40;
//	*(PalettePtr + 3 * i + 0) = 40;
//	*(PalettePtr + 3 * i + 1) = 40;
//	*(PalettePtr + 3 * i + 2) = 40;
//	++i;
//	}
	}
    badcolor:
    for (i = 0; i < PalSize; i++)
	{
	for (j = 0; j < 3; ++j)
	    *(TrueCol.PalettePtr + 3 * i + j) <<= 2;
	if (i == TrueCol.inside_colour)
	    {
	    TrueCol.InsideRed = *(TrueCol.PalettePtr + 3 * i + 0);		// values for r, g, b channels for inside colour
	    TrueCol.InsideGreen = *(TrueCol.PalettePtr + 3 * i + 1);			
	    TrueCol.InsideBlue = *(TrueCol.PalettePtr + 3 * i + 2);
	    }
	}

#ifdef DEBUG
    for (i = 0; i < 16; i++)
	{
	wsprintf(s, "Col[%d]: %d %d %d", i, *(PalettePtr + 3 * i), 
				    *(PalettePtr + 3 * i + 1), *(PalettePtr + 3 * i + 2));
	MessageBox (hwnd, s, "Par colours", MB_ICONEXCLAMATION | MB_OK);
	}
#endif   
    TrueCol.FinalisePalette(PalSize, threshold);
    return(0);
    }

/**************************************************************************
	Process Corners
**************************************************************************/

int	ProcessParams(char *s)

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

int	ProcessCorners(char *s, BOOL CentreFlag)

    {
    char	*t;
    char	*s1;
    char	*s2;
    char	*s3;
    char	*s4;
    char	*s5;
    char	*s6;
    double	Magnification;
    double	Xmagfactor;
    double	Rotation;
    double	Skew;
    double	floatval[6];					// pre-parsed floating parms
    Complex	centre;

    int		count, NumCorners;
    BigDouble   BigMag, temp, OneOverMag;

    t = s;
    while(*s && *s != ' ')
	{
	if (!isdigit(*s) && *s != '.' && *s != '+' && *s != '-' && *s != 'e')
	    *s = ' ';
	s++;
	}

    if (CentreFlag)
	{
	sscanf(t, "%lf %lf %lf %lf %lf %lf", &hor, &vert, &Magnification, &Xmagfactor, &Rotation, &Skew);
	RotationCentre.x = hor;
	RotationCentre.y = vert;
	hor -= (ScreenRatio / Magnification);
	vert -= (1.0 / Magnification);
	mandel_width = 2.0 / Magnification;
	}
    else
	{
	NumCorners = sscanf(t, "%lf %lf %lf %lf %lf %lf", &floatval[0], &floatval[1], &floatval[2], &floatval[3], &floatval[4], &floatval[5]);
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
	    hor = centre.x - (ScreenRatio / Magnification);
	    vert = centre.y - (1.0 / Magnification);
	    mandel_width = 2.0 / Magnification;
	    RotationCentre.x = hor + (ScreenRatio / Magnification);
	    RotationCentre.y = vert + (1.0 / Magnification);
	    }

//	count = sscanf(t, "%lf %lf %lf %lf %lf %lf", &hor, &a1, &vert, &a2, &Rotation, &Skew);
	}

    RotationAngle = (int)Rotation;
    if (Rotation == 0.0 || Rotation == 90.0 || Rotation == 180.0 || Rotation == 270.0)		// save calcs in rotating, just remap
	RotationCentre = 0.0;
    else
	{
	//	z_rot = Rotation;
	//	RotationAngle = (int)Rotation;
	Mat.InitTransformation(RotationCentre.x, RotationCentre.y, 0.0, 0.0, 0.0, Rotation);
	}

    if (mandel_width < 0.0)
	mandel_width = - mandel_width;

    s1 = new char[SIZEOF_BF_VARS];
    s2 = new char[SIZEOF_BF_VARS];
    s3 = new char[SIZEOF_BF_VARS];
    s4 = new char[SIZEOF_BF_VARS];
    s5 = new char[SIZEOF_BF_VARS];
    s6 = new char[SIZEOF_BF_VARS];
    count = sscanf(t, "%s %s %s %s %s %s", s1, s2, s3, s4, s5, s6);
/*
    if (mandel_width < DBL_MIN)			// we can do a BigNum calculation here to allow deeper zooming
	{
//	if (init_big_dec((int)strlen(s3) + PRECISION_FACTOR) < 0)	// need enough temp precision to load BigWidth
//	    return -1;
	ConvertString2Bignum(BigWidth.x, s3);
	BigNumFlag = TRUE;
	}
*/
    precision = getprecbf_mag();
    if (precision < 0)			// exceeded allowable precision
	{
	if (s1) delete[] s1;
	if (s2) delete[] s2;
	if (s3) delete[] s3;
	if (s4) delete[] s4;
	if (s5) delete[] s5;
	if (s6) delete[] s6;
	return -1;							// too many decimals for library
	}
    if (precision > DBL_DIG - 3 && NumCorners != 6)			// bignum support not available yout
	{
	decimals = precision + PRECISION_FACTOR;
//	if (!BigNumFlag)
//	    if (init_big_dec(decimals) < 0)
//		return -1;
//	mpfr_set_default_prec(decimals);
	if (ChangeBigPrecision(decimals) < 0)				// increase precision of Big numbers	
	    {
	    if (s1) delete[] s1;
	    if (s2) delete[] s2;
	    if (s3) delete[] s3;
	    if (s4) delete[] s4;
	    if (s5) delete[] s5;
	    if (s6) delete[] s6;
	    return -1;							// too many decimals for library
	    }
	BigNumFlag = TRUE;
	BigBailout = rqlim;
	ConvertString2Bignum(BigHor.x, s1);
	ConvertString2Bignum(BigVert.x, (CentreFlag) ? s2 : s3);
	if (CentreFlag)
	    {
	    ConvertString2Bignum(BigMag.x, s3);
	    if (mpfr_sgn(BigMag.x) == 0)	// no naughty division
		{
		if (s1) delete[] s1;
		if (s2) delete[] s2;
		if (s3) delete[] s3;
		if (s4) delete[] s4;
		if (s5) delete[] s5;
		if (s6) delete[] s6;
		return -1;
		}
	    OneOverMag = BigMag.BigInvert();
	    // for debugging
	    ConvertBignum2String(s4, BigHor.x);
	    ConvertBignum2String(s5, BigVert.x);
	    ConvertBignum2String(s6, BigMag.x);

	    BigHor = BigHor - OneOverMag * ScreenRatio;
	    BigVert = BigVert - OneOverMag;
	    BigWidth = OneOverMag + OneOverMag;
	    // for debugging
	    ConvertBignum2String(s4, BigHor.x);
	    ConvertBignum2String(s5, BigVert.x);
	    ConvertBignum2String(s6, BigWidth.x);
	    }
	else
	    {
	    ConvertString2Bignum(temp.x, s4);			// mandel_width = vert - a2;
	    BigWidth = BigVert - temp;
	    if (mpfr_sgn(BigWidth.x) == 0)	// no naughty division
		{
		if (s1) delete[] s1;
		if (s2) delete[] s2;
		if (s3) delete[] s3;
		if (s4) delete[] s4;
		if (s5) delete[] s5;
		if (s6) delete[] s6;
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

    if (s1) delete[] s1;
    if (s2) delete[] s2;
    if (s3) delete[] s3;
    if (s4) delete[] s4;
    if (s5) delete[] s5;
    if (s6) delete[] s6;
    return 0;
    }

/**************************************************************************
	Process Invert paramemters
**************************************************************************/

int	ProcessInvert(char *s)

    {
    char	*t;
    int		count;

    t = s;
    while (*s && *s != ' ')
	{
	if (!isdigit(*s) && *s != '.' && *s != '+' && *s != '-' && *s != 'e')
	    *s = ' ';
	s++;
	}

    count = sscanf(t, "%lf %lf %lf", &f_radius, &f_xcenter, &f_ycenter);    // inversion radius, center 
    if (f_radius)
	invert = TRUE;
    return 0;
    }

/**************************************************************************
	Process Outside Filters
**************************************************************************/

int	Processfilters(char *s, BOOL IsInside)
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

int	ProcessPotential(char *s)

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

    sscanf(t, "%lf %lf %lf", &potparam[0], &potparam[1], &potparam[2]);

    return 0;
    }

/**************************************************************************
	    Read the par file
**************************************************************************/

#define BUFFERSIZE  5000

char	*leading(char *instr) // strips leading spaces 
    {
    register char *s = instr;
    while (*s && isspace(*s))
	s++;
    return s;
    }

char	*StripStuff(char *instr) // strips backslash and newlines 
    {
    register char *s = instr;
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
    char	*temp = NULL, *buffer = NULL, *word;
    char	InLine[200];
    char	InLine1[200];
    FILE	*fp;
    char	s[200];
    bool	IsFrm = false;		// do we have a formula?
    char	tmp[164];
    double	TempRqlim = -1.0;

    param[0] = param1 = 0.0;
    param[1] = param2 = 0.0;
    param[2] = param3 = 0.0;
    param[3] = param4 = 0.0;
    param[4] = param5 = 0.0;
    param[5] = param6 = 0.0;
    invert = FALSE;
    if ((fp = fopen(filename, "r")) == NULL)
	{
	wsprintf(s, "Can't Open par File: <%s>", filename);
	MessageBox (hwnd, s, "MANPWIN", MB_ICONEXCLAMATION | MB_OK);
	return -1;
	}

    do
	{
	char	*a, *b;
	if (fgets(InLine, 160, fp) == 0)
	    {
	    wsprintf(s, "Can't Get FRACTINT Par Data: <%s>", filename);
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

    buffer = new char[BUFFERSIZE];
    *buffer = '\0';

    while(fgets(InLine1, 160, fp))		// Max line length 160 chars
	{
	linenum++;
   
	if ((word = strchr(InLine1,';')))	// strip comment
	    *word = 0;
	strcpy(InLine, StripStuff(InLine1));
	strcat(buffer, leading(InLine));
	if (str_find_ci(InLine, "}"))
	     break;
	if ((i = (int)strlen(buffer)) > BUFFERSIZE - 160)
	    {
	    wsprintf(s, "Par Data full, line: <%d>", linenum);
	    MessageBox (hwnd, s, "ManpWIN", MB_ICONEXCLAMATION | MB_OK);
	    break;
	    }
	}    

    if (tok = str_find_ci(buffer, "params="))		// must be first
	ProcessParams(tok);
    if (tok = str_find_ci(buffer, "maxiter="))
	{
	threshold = atol(tok);
	if (threshold < 0L)
	    threshold = MAXTHRESHOLD;
	}
    if (tok = str_find_ci(buffer, "inside="))
	Processfilters(tok, INSIDE);
    if (tok = str_find_ci(buffer, "bailout="))
	TempRqlim = (double)atoi(tok);
    if (tok = str_find_ci(buffer, "decomp="))
	decomp = atoi(tok);
    if (tok = str_find_ci(buffer, "potential="))
	{
	ProcessPotential(tok);
	OutsideMethod = POTENTIAL;
	}
    if (tok = str_find_ci(buffer, "corners="))
	ProcessCorners(tok, FALSE);
    if (tok = str_find_ci(buffer, "center-mag="))
	ProcessCorners(tok, TRUE);
    if (tok = str_find_ci(buffer, "biomorph="))
	biomorph = atoi(tok);
    if (tok = str_find_ci(buffer, "function="))
	AnalyseFunction(tok);
    if (tok = str_find_ci(buffer, "colors="))
	ParseColours(tok);
    if (tok = str_find_ci(buffer, "invert="))
	ProcessInvert(tok);
    if (tok = str_find_ci(buffer, "outside="))
	Processfilters(tok, OUTSIDE);
    if (tok = str_find_ci(buffer, "bailoutest="))
	BailoutTestType = ProcessBailoutTest(tok);
    if (tok = str_find_ci(buffer, "type="))		// must be the only one which is after strlwr
	{
	if (FindType(hwnd, tok, buffer, &IsFrm, TempRqlim) < 0)
	    {
	    switch (type)
		{
		case LSYSTEM:
		    wsprintf(s, "Error in L-system file");
		    break;
		case IFS:
		    wsprintf(s, "Error in IFS file");
		    break;
		case FORMULA:
		    wsprintf(s, "Error in Formula file");
		    break;
		default:
		    wsprintf(s, "Fractal type <%s> not supported by ManpWin", temp);
		    break;
		}
	    MessageBox (hwnd, s, "Reading Fractint Par File", MB_ICONEXCLAMATION | MB_OK);
	    fclose(fp);
	    if (buffer) { delete[] buffer; buffer = NULL; }
		return -1;
	    }
	}
    if (buffer) { delete[] buffer; buffer = NULL; }
    if (IsFrm)
	{
	char *p;
	do
	    {
	    if (fgets(InLine, 160, fp) == 0)
		{
		wsprintf(s, "Can't Find formula in the FRACTINT Par Data: <%s>", filename);
		MessageBox(hwnd, s, "MANPWIN", MB_ICONEXCLAMATION | MB_OK);
		fclose(fp);
		return -1;
		}

#ifdef DEBUG
	    wsprintf(s, "Line %d, <%s>", linenum, InLine);
	    MessageBox(hwnd, s, "Finding Par", MB_ICONEXCLAMATION | MB_OK);
#endif   

	    ++linenum;
	    } while (str_find_ci(InLine, "frm:") == 0);

	temp = new char[BUFFERSIZE];
	*temp = '\0';

	// now let's interpret any formula if present
	while (fgets(InLine, 160, fp))		// Max line length 160 chars
	    {

#ifdef DEBUG
	    wsprintf(s, "Line %d, <%s>, <%s>", linenum, InLine, lptr[lsys_ptr]);
	    MessageBox(hwnd, s, "Reading Par File", MB_ICONEXCLAMATION | MB_OK);
#endif   


	    linenum++;

	    if ((word = strchr(InLine, ';')))	// strip comment
		*word = 0;
	    //	    strcat(temp, InLine + 2);
	    strcat(temp, InLine);
	    if (str_find_ci(InLine, "}"))
		break;
	    if ((i = (int)strlen(temp)) > BUFFERSIZE - 200)
		{
		wsprintf(s, "Par Frm Data full, line: <%d>", linenum);
		MessageBox(hwnd, s, "ManpWIN", MB_ICONEXCLAMATION | MB_OK);
		fclose(fp);
		break;
		}
	    }
	p = temp;
	q = FormulaString;

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

	ProcessFormulaString(FormulaString);
	if (temp) { delete[] temp; temp = NULL; }
	}
    if (type == FRACTPAR)				// if we haven't found a fractal, then we still have fractal type FRACTPAR
	{
	type = MANDELFP;
	wsprintf(s, "No fractal type was found <%s>. Assume Mandelbrot", temp);
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
	Load L-System Name
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
	wsprintf(s, "Can't Open Par File: <%s>", filename);
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

