/*
    MANPPAR.CPP - a module for handling ManpWIN par files.

    Written in Microsoft Visual 'C++' by Paul de Leeuw.
*/

#include	"manp.h"
#include	"fractype.h"
#include	"fractalp.h"
#include	"fract.h"
#include	"complex.h"
#include	"big.h"
#include	<conio.h>
#include	<string.h>
#include	<stdio.h>
#include	"colour.h"
#include	"filter.h"
#include	"BigDouble.h"
#include	"OscProcess.h"
#include	"Plot.h"
#include	"Pixel.h"
#include	"Matrix.h"
#include	"BigMatrix.h"
#include	"SafeStrings.h"
#include	"..\parser\TrigFn.h"

extern	char	LSYSFile[];
extern	char	lptr[][100];
extern	char	lsys_type[];
extern	char	IFSFile[];		// IFS file
extern	char	MAPFile[];		// colour map file
extern	char	AxesText[];		// used to convert axes and display them

extern	int	ifs_type;
extern	int	eye_dots;		// eye dots for AutoStereo
extern	int	stereo_sign;
extern	double	HenonA, HenonXStart, HenonYStart, HenonStep;
extern	int	HenonPoints;
extern	int	lsys_ptr;

extern	WORD	steps, NumHarmonics;	// for Fourier Analysis

extern	BOOL	NonStandardImage;	// has user changed image size?

extern	int	StartColourCycling;	// we can start the colour cycling from any point..good for cycling animations
extern	int	OscillatorPtr;		// points to current subtype
extern	int	OscPtrArray[];		// array of pointers to specific oscillators or fractal maps
extern	double	VertBias;		// allow vertical stretching of the image
extern	double	zBias;			// allow stretching of the image in the z direction
extern	double	dt;			// delta time
extern	BOOL	DisplayLines;		// used in Chaotic Oscillators
extern	BOOL	FractalMapColouring;	// used in Fractal Maps
extern	BOOL	RemoveHiddenPixels;	// used in Surfaces
extern	int	NumParams;		// number of parameters in this oscillator or fractal map
extern	int	NumVariables;		// number of variables in this oscillator or fractal map
extern	int	xSign, ySign;		// sign of x and y axes - allow reflection of image across axis
extern	int	FindCentre;		// statistical average or contained volume
extern	BOOL	BlockAnimation;		// used for 2D or less to preventanimating non-existent dimensions

extern	void	Axes2Text(char *text, int x, int y, int z);
extern 	void	set_palette(void), draw3D(HWND), stereo_init(void);

extern	char	*str_find_ci(char *, char *);
extern	char	*trailing(char *instr);
extern	int	load_lsystems(HWND, char *);
extern	void	InitTrueColourPalette(BYTE);
extern	void	ClosePtrs(void);
extern	int	mainview(HWND, BOOL);
extern	void	SaveTriplets(FILE *);
extern	char	*WriteSliders(void);
extern	int	ReadSliders(char *);
extern	void	DisplayPalette(HWND, BOOL);
extern	int	ProcessFormulaString(char *);
extern	int	ifsload(HWND, char *);
extern	int	get_IFS_names(HWND, char *);		// get the IFS fractal names
extern	char	*strtok1(char *, const char *);		// see maninit.c 
extern	int	setup_Oscillator(void);			// sort the oscillator database alphabetically
extern	int	setup_FractalMaps();			// sort the fractal maps database alphabetically
extern	int	setup_SprottMaps(void);			// count how many Sprott map fractals there are
extern	int	setup_Surface();			// sort the surface database alphabetically
extern	int	setup_Knot();				// sort the knot database alphabetically
extern	int	setup_Curve();				// sort the curve database alphabetically
extern	int	setup_Perturbation(void);		// count how many Perturbation fractals there are

int	GetParamData(HWND, LPSTR, LPSTR, LPSTR, BOOL);
void	BasicFractData(StringBuilder& sb, BOOL);

/**************************************************************************
	Reduce binary 24 bits to 4 ASCII characters
**************************************************************************/

void	CManp::ConvertRGB2ASCII(RGBTRIPLE rgb, char *buf)
    {
    BYTE    s[4], a;
    int	    i, k;

    s[0] = rgb.rgbtRed & 0x3f;
    s[1] = (rgb.rgbtRed & 0xc0) >> 6;
    a = (rgb.rgbtGreen & 0x0f) << 2;
    s[1] |= a;
    s[2] = (rgb.rgbtGreen & 0xf0) >> 4;
    a = (rgb.rgbtBlue & 0x03) << 4;
    s[2] |= a;
    s[3] = rgb.rgbtBlue >> 2;

    for (i = 0; i < 4; i++)
	{
	if ((k = s[i]) < 10)
	    k += '0';
	else if (k < 36)
	    k += ('A' - 10);
	else
	    k += ('_' - 36);
	buf[i] = (char)k;
	}
    buf[4] = '\0';
    }

/**************************************************************************
	Convert 4 ASCII characters to RGB
**************************************************************************/

void	CManp::ConvertASCII2RGB(RGBTRIPLE &rgb, char *buf)
    {
    BYTE    s[4], a;
    int	    i, k;
    BOOL    error = FALSE;

    for (i = 0; i < 4; i++)
	{
	if ((k = *(buf + i)) < '0')
	    error = TRUE;
	else if (k <= '9')
	    k -= '0';
	else if (k < 'A')
	    error = TRUE;
	else if (k <= 'Z')
	    k -= ('A' - 10);
	else if (k < '_' || k > 'z')
	    error = TRUE;
	else
	    k -= ('_' - 36);
	s[i] = (error) ? 0 : (char)k;
	}

    rgb.rgbtBlue = s[3] << 2;
    a = (s[2] & 0xc0) >> 4;
    rgb.rgbtRed |= a;
    rgb.rgbtGreen = (s[2] & 0x0f) << 4;
    a = s[1] >> 2;
    rgb.rgbtGreen |= a;
    rgb.rgbtRed = (s[1] << 6) | s[0];
    }

/**************************************************************************
	Write batch file with current parameters
**************************************************************************/

void	output_batch(double h, double v, double w, HWND hwnd, LPSTR filename)
    {
    FILE	*fb;				// batch file   
    char	s[400];
    char	info[MAXDATALINE];
    char	ascii[6];
    long	i, k;

    info[0] = '\0';
    StringBuilder sb(info, sizeof(info));

    if ((fb = fopen(filename, "w")) == NULL)
	{
	_snprintf_s(s, 400, _TRUNCATE, "\nCan't open file: %s for save", filename);
	MessageBox(hwnd, s, "View", MB_ICONEXCLAMATION | MB_OK);
	MessageBeep(0);
	return;
	}
    else
	{
	if (gManp->BigNumFlag)
	    {
	    std::vector<char>	s1(SIZEOF_BF_VARS);
	    std::vector<char>	s2(SIZEOF_BF_VARS);
	    std::vector<char>	s3(SIZEOF_BF_VARS);
	    gManp->BigHor.ToString(s1.data(), SIZEOF_BF_VARS, false);
	    gManp->BigVert.ToString(s2.data(), SIZEOF_BF_VARS, false);
	    gManp->BigWidth.SafeSprintf(s3.data(), SIZEOF_BF_VARS, "%.20Re");
	    fprintf(fb, "-c%s,%s,%s", s1.data(), s2.data(), s3.data());
	    }
	else
	    fprintf(fb, "-c%24.24f,%24.24f,%24.24f", h, v, w);
	fprintf(fb, " -t%d", gManp->threshold);

	*info = '\0';
	BasicFractData(sb, FALSE);
	fprintf(fb, "%s\n", info);
	fprintf(fb, "Palette=\n");
	for (i = 0, k = 0; i < gManp->threshold; i++, k++)
	    {
	    if (k == 20)							// group into lumps of 20
		{
		k = 0;
		fprintf(fb, "\n");
		}
	    if (i >= MAXPALETTE)						// just in case
		{
		fprintf(fb, "\n");
		break;
		}
	    gManp->ConvertRGB2ASCII(gManp->TrueCol.PalettePtr[i], ascii);
	    fprintf(fb, "%s", ascii);
	    }
	fprintf(fb, "\n");
	fclose(fb);
	}
    }

/**************************************************************************
	Search the database list for a matching fractal name and find the subtype
	This is done because the subtype may change whenever we add new ones and would make PAR files point to the wrong subtype
**************************************************************************/

int	PertName2Subtype(int type, char *string)
    {
    int		i, NumPert;
    bool	flag = false;
    struct	PerturbationSpecificStuff	*PertPtr;

    NumPert = setup_Perturbation();				// sort the database alphabetically

    for (i = 0; i < NumPert; i++)
	{
	PertPtr = PerturbationSpecific + i;
	if (PertPtr->name == NULL)
	    break;
	if (strcmp(PertPtr->name, string) == 0)
	    {
	    flag = true;
	    break;
	    }
	}
    if (flag)
	return i;
    else return -1;	// name not found
    }

/**************************************************************************
	Search the database list for a matching fractal name and find the subtype
	This is done because the subtype may change whenever we add new ones and would make PAR files point to the wrong subtype
**************************************************************************/

int	Name2Subtype(int type, char *string)
    {
    struct	OscillatorSpecificStuff	*DatabasePtr;
    int		i, j;
    bool	flag = false;

    if (type == OSCILLATORS)
	setup_Oscillator();			// sort the oscillator database alphabetically
    else if (type == FRACTALMAPS)
	setup_FractalMaps();			// sort the fractal maps database alphabetically
    else if (type == SPROTTMAPS)
	setup_SprottMaps();			// sort the fractal maps database alphabetically
    else  if (type == SURFACES)
	setup_Surface();			// sort the surface database alphabetically
    else  if (type == KNOTS)
	setup_Knot();				// sort the knot database alphabetically
    else  if (type == CURVES)
	setup_Curve();				// sort the curve database alphabetically

    for (i = 0; i < MAXOSC; i++)
	{
	DatabasePtr = gManp->OscProcess.LoadDatabasePointer(type, i);
	if (DatabasePtr->name == NULL)
	    break;
	if (strcmp(DatabasePtr->name, string) == 0)
	    {
	    flag = true;
	    break;
	    }
	}
    if (flag)
	{
	for (j = 0; j < MAXOSC; j++)
	    {
	    if (OscPtrArray[j] == i)		// we have found the index value for that subtype
		OscillatorPtr = j;
	    }
	return i;
	}
    else return -1;	// name not found
    }

/**************************************************************************
	Get fractal type
**************************************************************************/

int	analyse_fractal(HWND hwnd, char *str, char *SaveString, char *PastQuote)
    {
    char	t[6];
    int		flags;

    strncpy(t, str, 3);
    t[3] = '\0';
    gManp->type = atoi(t);
    if (gManp->type == 0)
	gManp->type = MANDELFP;				// fix silly par files from the past
    if (gManp->type == '9')
	gManp->type = MATEIN;				// fix silly par files from the past

    if (gManp->type > MAXTYPE && gManp->type != FRACTPAR)	// edit MAXTYPE in fractalp.h if the total number of fractals changes
	return -1;

    gManp->InitFract(gManp->type);
    gManp->RebuildFractalMetadata(gManp->type, gManp->subtype);		// load all the metadata for parameters

    switch (gManp->type)				// set defaults
	{
	case MANDELFP:					// good old Mandelbrot
	case NEWTONAPPLE:				// newton apple
	case NEWTONFLOWER:				// Newton flower
	case MAGNET1:					// magnets 1 and 2
	case MAGNET2:
	case TEDDY:					// strange little fractal
	case NEWTONMSET:				// okay, I have a thing for Newtons
	case NEWTONJULIANOVA:
	case TALIS:
	case NEWTONCROSS:
	case QUADMAND:
	case IKEDA:
	case CROSSROADS:
	case ZIGZAG:
	case GARGOYLE:
	case CURLICUES:
	case CHUA:
	case BURNINGSHIP:
	case THORN:
	case POPCORN:
	case FPPOPCORN:
	case LPOPCORN:
	case FPPOPCORNJUL:
	case LPOPCORNJUL:
	case BUDDHABROT:
	    break;
	case NEWTON:					// Newton Fractal
	case NEWTBASIN:					// Newton Fractal (basin or stripe)
	    gManp->subtype = 'N';
	    gManp->special = 3;				// default cubic
	    strncpy(t, str + 3, 3);
	    t[3] = '\0';
	    gManp->subtype = atoi(t);
	    strncpy(t, str + 6, 3);
	    t[3] = '\0';
	    gManp->degree = atoi(t);
	    strncpy(t, str + 9, 3);
	    t[3] = '\0';
	    gManp->special = atoi(t);
	    break;
	case PERTURBATION:
	    {
	    char EnableBLA[12] = "";

	    gManp->subtype = PertName2Subtype(gManp->type, SaveString);
	    if (gManp->subtype < 0)
		{
		MessageBox(hwnd, SaveString, "Unknown Subtype", MB_ICONEXCLAMATION | MB_OK);
		MessageBeep(0);
		gManp->subtype = 0;
		}

	    gManp->EnableApproximation = true;

	    sscanf(PastQuote, "%d,%11s", &gManp->SlopeType, EnableBLA);

	    if (*EnableBLA)
		gManp->EnableApproximation = (strcmp(EnableBLA, "true") == 0);
	    break;
	    }
	case POWER:					// POWER fractals
	case NEWTONPOLYGON:				// Newton Polygon fractals 
	case NEWTONVARIATION:				// Newton Variation fractals
//	    gManp->degree = 5;				// Degree = 5
	    if (*(str + 3) != '\0')
		gManp->degree = atoi(str + 3);
	    break;
	case TIERAZON:					// Tierazon fractals
	case SLOPEDERIVATIVE:
	case SLOPEFORWARDDIFF:
	case MANDELDERIVATIVES:
	    if (*(str + 3) != '\0')
		gManp->subtype = atoi(str + 3);
	    break;
	case OSCILLATORS:
	case FRACTALMAPS:
	case SPROTTMAPS:
	case SURFACES:
	case KNOTS:
	case CURVES:
	    gManp->subtype = 1;
	    gManp->subtype = Name2Subtype(gManp->type, SaveString);	// search database for matching subtype
	    if (gManp->subtype < 0)
		{
		MessageBox(hwnd, SaveString, "Unknown Subtype", MB_ICONEXCLAMATION | MB_OK);
		MessageBeep(0);
		gManp->subtype = 0;				// use first one if not found
		}
	    sscanf(PastQuote, "%d,%d,%d,%d,%d,%lf,%lf,%lf,%lf", &gManp->xAxis, &gManp->yAxis, &gManp->zAxis, &flags, &gManp->MaxDimensions, &gManp->iterations, &VertBias, &dt, &zBias);
	    if (gManp->type == OSCILLATORS)
		DisplayLines = flags & 1;
	    else if (gManp->type == FRACTALMAPS || gManp->type == SPROTTMAPS)
		FractalMapColouring = flags & 1;
	    else
		RemoveHiddenPixels = flags & 1;
	    gManp->CoordSystem = (flags & 0x0001E) >> 1;
	    BlockAnimation = flags & 0x0020;
	    FindCentre = flags & 0x0040;
	    gManp->PerspectiveFlag = flags & 0x0080;
	    xSign = ySign = 1;
	    gManp->zAxis--;
	    if (gManp->xAxis < 0)
		{
		xSign = -1;
		gManp->xAxis = -gManp->xAxis - 1;
		}
	    else
		gManp->xAxis--;

	    if (gManp->yAxis < 0)
		{
		ySign = -1;
		gManp->yAxis = -gManp->yAxis - 1;
		}
	    else
		gManp->yAxis--;
	    Axes2Text(AxesText, gManp->xAxis + 1, gManp->yAxis + 1, gManp->zAxis + 1);
	    break;

	case FOURIER:					// Fourier Analysis fractals
	    steps = 400;
	    NumHarmonics = 60;
	    if (*(str + 3) != '\0')
		{
		strncpy(t, str + 3, 3);
		t[3] = '\0';
		gManp->subtype = atoi(t);
		}
	    if (*(str + 6) != '\0')
		{
		strncpy(t, str + 6, 3);
		t[3] = '\0';
		NumHarmonics = atoi(t);
		}
	    if (*(str + 9) != '\0')
		{
		strncpy(t, str + 9, 4);
		t[4] = '\0';
		steps = atoi(t);
		}
	    if (*(str + 13) != '\0')
		ReadSliders(str + 13);
	    break;
	case CUBIC:					// CUBIC fractals
	    gManp->subtype = 'B';			// CBIN */
	    gManp->special = WHITE;
	    strncpy(t, str + 3, 3);
	    t[3] = '\0';
	    gManp->subtype = atoi(t);
	    strncpy(t, str + 6, 3);
	    t[3] = '\0';
	    gManp->special = atoi(t);
	    break;
	case RATIONALMAP:				// Rational Map type fractal
	    gManp->subtype = 'A';			// RAT34
	    gManp->special = WHITE;
	    strncpy(t, str + 3, 3);
	    t[3] = '\0';
	    gManp->subtype = atoi(t);
	    strncpy(t, str + 6, 3);
	    t[3] = '\0';
	    gManp->special = atoi(t);
	    break;

	case BIFURCATION:				// Bifurcation Fractal
	case BIFSTEWART:
	case BIFLAMBDA:
	case BIFADSINPI:
	case BIFEQSINPI:
	case BIFMAY:
	case EXPFRACTAL:				// Exponential type fractal
	    strncpy(t, str + 3, 3);
	    t[3] = '\0';
	    gManp->subtype = atoi(t);
	    strncpy(t, str + 6, 3);
	    t[3] = '\0';
	    gManp->special = atoi(t);
	    break;

	case MALTHUS:					// Malthus fractal
	case TRIANGLES:					// Triangles fractal
	case GEOMETRY:					// Geometric shapes fractal
	case CIRCLES:					// Geometric shapes fractal
	case PASCALTRIANGLE:
	case APOLLONIUS:
	case APOLLONIUSIFS:
	case SIERPINSKIFLOWERS:
	    if (*(str + 3) != '\0')
		gManp->subtype = atoi(str + 3);
	    break;
	case FIBONACCI:					// Fibonacci Spirals
	    gManp->subtype = 'I';			// default linear
	    if (isalpha(*(str + 3)))
		gManp->subtype = toupper(*(str + 3));
	    break;
	case SPECIALNEWT:				// type SPECIALNEWT fractal
	    gManp->special = 16;			// special in SPECIALNEWT fractal used as multiple colour flag
	    if (*(str + 3) != '\0')
		gManp->special = atoi(str + 3);
	    break;

	case SINFRACTAL:				// sin() type fractal
	case MATEIN:					// MATEIN type fractal
	    gManp->subtype = 'R';			// REAL
	    gManp->special = WHITE;			// special colour
	    if (*(str + 3) != '\0')
		gManp->special = atoi(str + 3);
	    break;

	case CIRCLESQ:					// circle squared
	    gManp->threshold = 256;				// else poor picture
	    break;
	case LSYSTEM:
	    strncpy(t, str + 3, 3);
	    t[3] = '\0';
	    lsys_ptr = atoi(t);
	    strncpy(t, str + 6, 3);
	    t[3] = '\0';
	    gManp->degree = atoi(t);
	    if (*SaveString)				// there are spaces in the string and therefore we have already captured the formula string
		strcpy(LSYSFile, SaveString);
	    else
		strcpy(LSYSFile, str + 9);
	    break;
	case SCREENFORMULA:				// On Screen Formula fractals
	case FORMULA:					// formula files
	case FFORMULA:
	    if (*SaveString)				// there are spaces in the string and therefore we have already captured the formula string
		strcpy(gManp->FormulaString, SaveString);
	    else
		strcpy(gManp->FormulaString, str + 3);
	    if (ProcessFormulaString(gManp->FormulaString) == -1)
		return -1;
	    break;
	case IFS:
	    if (*SaveString)				// there are spaces in the string and therefore we have already captured the formula string
		strcpy(IFSFile, SaveString);
	    else
		strcpy(IFSFile, str + 6);
	    if (get_IFS_names(hwnd, IFSFile) < 0)	// load the IFS fractal names
		return -1;
	    strncpy(t, str + 3, 3);
	    t[3] = '\0';
	    lsys_ptr = atoi(t);				// get the specific fractal name pointer
	    strcpy(lsys_type, lptr[lsys_ptr]);
	    if (ifsload(hwnd, IFSFile) < 0)
		return -1;
	    break;
	}
    return 0;
    }

/**************************************************************************
	Get Julia location on Mandelbrot set
**************************************************************************/

void	GetImageSize(char *s)
    {
    char	*t;

    gManp->height = 0;			// need to be able to detect old script files
    t = s;
    while (*s && *s != ' ')
	{
	if (*s == ',')
	    *s = ' ';
	s++;
	}
    sscanf(t, "%ld %ld", &gManp->width, &gManp->height);
    if (gManp->height <= 0)		// old script file?
	gManp->height = (int)(((double)gManp->width + 0.5) / 1.7777777777);
    }

/**************************************************************************
	Get Functions
**************************************************************************/

int FindFunct(char *Str)
    {
    int		n;
    size_t	size;
    CTrigFn	TrigFn;

    size = strlen(Str);
    for (n = 0; n < gManp->Fractal.TotalFunctions; n++)
	{
	if (!strnicmp(TrigFn.FunctList[n], Str, size))
	    return n;
	}
    return -1;			// not found
    }

void	GetFunctions(char *s)
    {
    char	*t;
    char	fn1[16], fn2[16];
    int		numfn;
    int		FnPtr;
    CTrigFn	TrigFn;

    t = s;
    while (*s && *s != ' ')
	{
	if (*s == ',')
	    *s = ' ';
	s++;
	}
    numfn = sscanf(t, "%s %s", fn1, fn2);
    if (numfn == 1 || numfn == 2)
	if ((FnPtr = TrigFn.FindFunct(fn1, gManp->Fractal.TotalFunctions)) >= 0)
	    {
	    gManp->Fractal.Fn1 = TrigFn.FunctList[FnPtr];
	    gManp->Fractal.Fn1Index = FnPtr;
	    //	    dtrig0 = *FnctList[FnPtr].ptr;	// load function pointer 
	    }
	else
	    numfn = 0;					// handle -nan()

    if (numfn == 2)
	if ((FnPtr = TrigFn.FindFunct(fn2, gManp->Fractal.TotalFunctions)) >= 0)
	    {
	    gManp->Fractal.Fn2 = TrigFn.FunctList[FnPtr];
	    gManp->Fractal.Fn2Index = FnPtr;
	    //	    dtrig1 = *FnctList[FnPtr].ptr;	// load function pointer 
	    }
	else
	    numfn = 0;					// handle -nan()
    if (numfn == 1 || numfn == 2)
	gManp->Fractal.NumFunct = numfn;
    }

/**************************************************************************
	Get Parameters
**************************************************************************/

void	CManp::GetParamsList(char *s)
    {
    char	*t;

    t = s;
    while (*s)
	{
	if (!isdigit(*s) && *s != '.' && *s != '+' && *s != '-' && *s != 'e' && *s != 'E')
	    *s = ' ';
	s++;
	}

    if (type == OSCILLATORS || type == FRACTALMAPS || type == SPROTTMAPS || type == SURFACES || type == KNOTS || type == CURVES)			// these have a different database
	NumParams = sscanf(t, "%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf",
	    &gManp->param[0], &gManp->param[1], &gManp->param[2], &gManp->param[3], &gManp->param[4], &gManp->param[5], &gManp->param[6], &gManp->param[7], &gManp->param[8], &gManp->param[9],
	    &gManp->param[10], &gManp->param[11], &gManp->param[12], &gManp->param[13], &gManp->param[14], &gManp->param[15], &gManp->param[16], &gManp->param[17], &gManp->param[18], &gManp->param[19]);
    else if (type == SLOPEDERIVATIVE || type == SLOPEFORWARDDIFF || type == PERTURBATION)		// store all 15 parameters as param[15] is used to hold start colour (as a double)
	Fractal.NumParam = sscanf(t, "%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf", Fractal.ParamValue[0], Fractal.ParamValue[1], Fractal.ParamValue[2], Fractal.ParamValue[3],
	    Fractal.ParamValue[4], Fractal.ParamValue[5], Fractal.ParamValue[6], Fractal.ParamValue[7], Fractal.ParamValue[8], Fractal.ParamValue[9], Fractal.ParamValue[10], Fractal.ParamValue[11],
	    Fractal.ParamValue[12], Fractal.ParamValue[13], Fractal.ParamValue[14], Fractal.ParamValue[15]);
    else
	Fractal.NumParam = sscanf(t, "%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf", Fractal.ParamValue[0], Fractal.ParamValue[1], Fractal.ParamValue[2], Fractal.ParamValue[3], 
	    Fractal.ParamValue[4], Fractal.ParamValue[5], Fractal.ParamValue[6], Fractal.ParamValue[7], Fractal.ParamValue[8], Fractal.ParamValue[9]);
    }

/**************************************************************************
	Get Parameters
**************************************************************************/

void	CManp::GetBailout(char *s)
    {
    char	*t;
    int		count;

    t = s;
    while (*s)
	{
	if (!isdigit(*s) && *s != '.' && *s != '+' && *s != '-' && *s != 'e' && *s != 'E')
	    *s = ' ';
	s++;
	}

    count = sscanf(t, "%lf %d", &rqlim, &BailoutTestType);
    if (count == 1)
	BailoutTestType = BAIL_MOD;
    }

/**************************************************************************
	Analyse param file
**************************************************************************/

void	GetParam(HWND hwnd, LPSTR filename, LPSTR szSaveFileName)
    {
    std::vector<char> string(SIZEOF_BF_VARS * 3);	// times 3 because of x, y, width
    FILE	*fp;					// param file   
    char	s[200];
    char	ascii[5];
    int		ptr, ch, j;
    long	i;
    BOOL	eof = FALSE;
    long	LocalThreshold;

    ptr = 0;
    gManp->setup_defaults();
    if ((fp = fopen(filename, "r")) == NULL)
	{
	_snprintf_s(s, 200, _TRUNCATE, "Can't open param file: %s for read", filename);
	MessageBox(hwnd, s, "View", MB_ICONEXCLAMATION | MB_OK);
	MessageBeep(0);
	return;
	}

    ch = fgetc(fp);
    while (ch != '\0' && ch != '\n' && ch != EOF)
	{
	if (ptr >= (int)string.size() - 1)
	    {
	    _snprintf_s(s, 200, _TRUNCATE, "Parameter data too large in file: %s", filename);
	    MessageBox(hwnd, s, "View", MB_ICONEXCLAMATION | MB_OK);
	    fclose(fp);
	    return;
	    }

	string[ptr++] = (char)ch;
	ch = fgetc(fp);
	}
    // Add null to end string
    if (ch == EOF)
	{
	_snprintf_s(s, 200, _TRUNCATE, "Can't find info in file: %s", filename);
	MessageBox(hwnd, s, "View", MB_ICONEXCLAMATION | MB_OK);
	MessageBeep(0);
	fclose(fp);
	return;
	}

    string[ptr] = '\0';
    GetParamData(hwnd, filename, string.data(), szSaveFileName, FALSE);
    LocalThreshold = (gManp->threshold >= MAXPALETTE) ? MAXPALETTE - 1 : gManp->threshold;
    if (fgets(string.data(), MAXLINE, fp) == NULL)			// no palette
	InitTrueColourPalette(FALSE);					// process anything we found in the param list, eg palette file etc
    else
	{
	if (strncmp(string.data(), "Palette=", 8) == 0)			// we have a palette file
	    {
	    for (i = 0; i < LocalThreshold; i++)
		{
		for (j = 0; j < 4; j++)
		    {
		    ch = fgetc(fp);
		    if (ch == '\n')					// newline
			ch = fgetc(fp);
		    if (ch == EOF)
			{
			eof = TRUE;
			break;
			}
		    ascii[j] = ch;
		    }
		ascii[4] = '\0';
		if (eof)
		    break;
		gManp->ConvertASCII2RGB(gManp->TrueCol.PalettePtr[i], ascii);
		}
	    }
	}

    gManp->Plot.RefreshScreen();
    DisplayPalette(hwnd, gManp->TrueCol.DisplayPaletteFlag);
    InvalidateRect(hwnd, &gManp->r, FALSE);	// force repaint
    fclose(fp);
    }

/**************************************************************************
	Get Fwd Diff Params
**************************************************************************/

void	CManp::AnalyseFwdDiff(char *s)
    {
    char	*t;

    t = s;
    while (*t)
	{
	if (!isdigit(*t) && *t != '.' && *t != '+' && *t != '-' && *t != 'e' && *t != 'E')
	    *t = ' ';
	t++;
	}
    sscanf(s, "%lf %d %lf %lf %lf", &bump_transfer_factor, &PaletteStart, &lightDirectionDegrees, &bumpMappingDepth, &bumpMappingStrength);
    }

/**************************************************************************
	Remove quotes from filenames
**************************************************************************/

void	RemoveQuotes(char *out, char *in)
    {
    static  char	SaveString[2048];
    char	*p1, *p2;

    strcpy(SaveString, in);			// back up string
    if (p1 = str_find_ci(SaveString, "\""))	// found quotes in string, so lets extract the filename
	{
	p2 = str_find_ci(p1 + 1, "\"");
	if (p2)
	    {
	    strncpy(out, p1, p2 - p1);
	    *(out + (p2 - p1) - 1) = '\0';
	    }
	}
    else
	strcpy(out, in);
    }

/**************************************************************************
	Protect and unprotect strings
**************************************************************************/

void ProtectQuotedText(char *s)
    {
    bool inQuote = false;

    for (; *s; ++s)
	{
	if (*s == '"')
	    {
	    inQuote = !inQuote;
	    continue;
	    }

	if (inQuote)
	    {
	    if (*s == ' ')
		*s = '\x01';
	    else if (*s == '-')
		*s = '\x02';
	    }
	}
    }

void UnprotectQuotedText(char *s)
    {
    for (; *s; ++s)
	{
	if (*s == '\x01')
	    *s = ' ';
	else if (*s == '\x02')
	    *s = '-';
	}
    }

/**************************************************************************
	Analyse param data
**************************************************************************/

int	GetParamData(HWND hwnd, LPSTR filename, LPSTR string, LPSTR szSaveFileName, BOOL IsPNG)
    {						// Establish string and get the first token:
    char	*token;
    char	seps[] = " \t\n";
    char	TokChar;
    char	s[200];
    char	SaveString[2048];		// get filenames from quotes.
    std::vector<char> TempBuffer(SIZEOF_BF_VARS * 3);	// Save entire string to get anything within quotes.
							// SIZEOF_BF_VARS times 3 because of x, y, width
    char	*TrailingArgs = NULL;

    strcpy_s(TempBuffer.data(), TempBuffer.size(), string);	// get a copy in case of corruption by strtok()
    ProtectQuotedText(TempBuffer.data());			// Protect quoted text from the parser
    token = strtok(TempBuffer.data(), seps);
    while (token != NULL)
	{
	if (*token == '-') 
	    {
	    TokChar = *(token + 1);
	    *(token + 1) = TokChar = toupper(TokChar);
	    switch (TokChar)
		{
		case '~':				// used for colour smoothing
		    if (*(token + 2))
			sscanf(token + 2, "%lf", &gManp->ColourSpeed);
		    break;
		case '^':				// shift the palette by this amount
		    if (*(token + 2))
			gManp->PalOffset = atoi(token + 2);
		    break;
		case '@':				// divide the number of iterations by this amount
		    if (*(token + 2))
			sscanf(token + 2, "%lf", &gManp->IterDiv);
		    break;
		case '#':				// Lyapunov Sequence
		    if (*(token + 2))
			strcpy(gManp->LyapSequence, token + 2);
		    break;
		case '$':				// distance estimation
		    if (*(token + 2))
			gManp->AnalyseDistEst(token + 2);
		    gManp->_3dflag = FALSE;
		    break;
		case '2':				// rotate oscillator in 3D
		    if (*(token + 2))			// modify 3d parameters?
			gManp->analyse_3d(token + 2);
		    gManp->_3dflag = FALSE;
		    break;
		case '3':				// replay in 3D
		    if (*(token + 2))			// modify 3d parameters?
			gManp->analyse_3d(token + 2);
		    gManp->_3dflag = TRUE;
		    break;
		case 'A':				// replace palette map
		    strcpy(MAPFile, SaveString);
		    break;
		case 'B':				// biomorph colour
		    gManp->biomorph = atoi(token + 2);
		    break;
		case 'C':				// corner and width 
		    if (gManp->analyse_corner(token + 2) < 0)
			{
			gManp->BigNumFlag = FALSE;
			if (gManp->AspectRatio > 1.0)	// take aspect ration into account when plotting Julia
			    {
			    gManp->mandel_width = 4.0;
			    gManp->vert = -2.0;
			    gManp->hor = gManp->vert * gManp->AspectRatio;
			    }
			else
			    {
			    gManp->mandel_width = 4.0 / gManp->AspectRatio;
			    gManp->hor = -2.0;
			    gManp->vert = gManp->hor / gManp->AspectRatio;
			    }
			gManp->param[0] = 0.0;
			gManp->param[1] = 0.0;
			SAFE_SPRINTF(s, "Deep Zooming Limit (%d decimals) exceeded", SIZEOF_BF_VARS);
			MessageBox(hwnd, s, "ManpWin", MB_ICONEXCLAMATION | MB_OK);
			MessageBeep(0);
			return -1;
			}
		    break;
		case 'D':							// method
		    gManp->AnalyseMethod(token + 2);
		    if (gManp->OutsideMethod > TIERAZONFILTERS && gManp->OutsideMethod < TIERAZONCOLOURS)	// tierazon filters
			{
			gManp->FilterType = gManp->OutsideMethod - TIERAZONFILTERS;
			gManp->RGBFilter = (TierazonFilter[gManp->FilterType].rgb) ? TRUE : FALSE;
			}
		    else if (gManp->OutsideMethod > TIERAZONCOLOURS)				// tierazon colours)
			{
			gManp->ColourMethod = gManp->OutsideMethod - TIERAZONCOLOURS;
			gManp->RGBFilter = (TierazonColour[gManp->ColourMethod].rgb) ? TRUE : FALSE;
			}

		    // this next line causes problems with animating Tierazon filters. I'll leave it in for the moment
		    gManp->PlotType = (gManp->RGBFilter) ? FILTERPLOT : NOSYM;
		    break;
		case 'E':				// exit on completion
		    gManp->exitflag = TRUE;
		    gManp->AutoExitFlag = TRUE;
		    break;
		case 'F':				// fractal type
		    if (*(token + 2) != '\0')
			{
			UnprotectQuotedText(token);	// Unprotect it before real-world use

			RemoveQuotes(SaveString, token + 2);
			UnprotectQuotedText(SaveString);// Unprotect it before real-world use

			char *TrailingArgs = NULL;
			char *q1 = strchr(token, '"');
			if (q1)
			    {
			    char *q2 = strchr(q1 + 1, '"');
			    if (q2)
				{
				TrailingArgs = q2 + 1;
				if (*TrailingArgs == ',')
				    TrailingArgs++;
				}
			    }

			if (analyse_fractal(hwnd, token + 2, SaveString, TrailingArgs) < 0)
			    {
			    SAFE_SPRINTF(s, "Unknown Fractal <%d> in PAR file", gManp->type);
			    MessageBox(hwnd, s, "ManpWin", MB_ICONEXCLAMATION | MB_OK);
			    MessageBeep(0);
			    gManp->type = MANDELFP;
			    return -1;
			    }
			}
		    else
			gManp->type = MANDELFP;			// Mandelbrot by default
		    if (gManp->type == SCREENFORMULA)
			{
			//			strcpy(FormulaString, token + 6);	// remember to go past quote
			}
		    else if (gManp->type == LSYSTEM)
			{
			if (load_lsystems(hwnd, LSYSFile) >= 0)
			    {
			    //			    time_to_reinit = FALSE;
			    //			    time_to_restart = TRUE;
			    //			    time_to_load = FALSE;
			    }
			gManp->Fractal.FractName = fractalspecific[gManp->type].name;
			}
		    else if (gManp->type == IFS)
			{
			//			if (load_lsystems(hwnd, LSYSFile) >= 0)
			//			    {
			//			    }
			//			Fractal.FractName = fractalspecific[type].name;
			}
		    break;
		case 'G':				// get screen size
		    if (!IsPNG)			// if PNG, we already have set up a DIB at the right proportions
			{
			gManp->time_to_restart = TRUE;
			GetImageSize(token + 2);
			gManp->width = atoi(token + 2);
			//			height = (int)(((double) width + 0.5) / 1.333333333333333333333);
			NonStandardImage = TRUE;
			//			ClosePtrs();		// ready for next screen
			mainview(hwnd, TRUE);	// all screen specific stuff
			}
		    break;
		case 'H':				// stereo pairs
		    if (*(token + 2) != '\0')
			{
			gManp->pairflag = atoi(token + 2);
			if (gManp->pairflag < 2 || gManp->pairflag > 16)
			    gManp->pairflag = 2;
			}
		    else
			gManp->pairflag = 2;
		    break;
		case 'I':				// inside colour
		    gManp->analyse_inside(token + 2);
		    break;
		    //		case 'J':	See below 'Z'		// julia set
		case 'K':				// number of decomp colours
		    gManp->decomp = atoi(token + 2);
		    break;
		case 'L':				// log colour map 
		    if (*(token + 2) == '\0')
			gManp->logval = TRUE;
		    else
			gManp->logval = atoi(token + 2);
		    break;
		case 'M':				// Calc mode
		    gManp->calcmode = *(token + 2);
		    if (gManp->calcmode == 'G')
			gManp->blockindex = atoi((token + 3));	// for solid guessing
		    if (gManp->calcmode == 'T')
			gManp->fillcolor = atol((token + 3));	// for tesseral fill colour
		    if (gManp->calcmode == 'F')
			gManp->AnalyseFwdDiff(token + 2);
		    break;
		case 'N':				// functions
		    GetFunctions(token + 2);
		    break;
		case 'O':				// rotation angle
		    gManp->RotationAngle = atoi(token + 2);
		    if (gManp->RotationAngle != 0 && gManp->RotationAngle != 90 && gManp->RotationAngle != 180 && gManp->RotationAngle != 270)		// get original rotation centre otherwise we can simply remap
			gManp->AnalyseRotation(token + 2);
		    break;
		case 'P':				// periodicity
		    gManp->period_level = atoi(token + 2);
		    break;
		case 'Q':				// no sound
		    gManp->CoordSystem = atoi(token + 2);
		    break;
		case 'R':				// AutoStereo depth
		    if (toupper(*(token + 2)) == 'G')
			{
			gManp->grayflag = TRUE;
			if (*(token + 3) != '\0')
			    gManp->AutoStereo_value = atoi(token + 3);
			}
		    else if (*(token + 2) != '\0')
			gManp->AutoStereo_value = atoi(token + 2);
		    break;
		case 'S':				// SaveAs filename
		    UnprotectQuotedText(token);		// Unprotect it before real-world use
		    RemoveQuotes(szSaveFileName, trailing(token + 2));
		    UnprotectQuotedText(szSaveFileName);		    
		    gManp->AutoSaveFlag = TRUE;
		    break;
		case 'T':				// threshold 
		    gManp->threshold = atol(token + 2);
		    break;
		case 'U':				// Palette parameters (keep for hysterical reasons)

#ifdef DEBUG
		    _snprintf_s(s, 200, _TRUNCATE, " i%d,%d,%d,%d,%d,%d",
			RedStartInt, GreenStartInt, BlueStartInt, RedIncInt, GreenIncInt, BlueIncInt);
		    MessageBox(hwnd, s, "", MB_ICONEXCLAMATION | MB_OK);
#endif

		    if (*(token + 2))		// modify Palette parameters?
//			{
gManp->AnalysePalette(token + 2);
		    //			TrueColourFlag = TRUE;
		    //			}
		    //		    else
		    //			TrueColourFlag = FALSE;

#ifdef DEBUG
		    _snprintf_s(s, MAXLINE, _TRUNCATE, " o%d,%d,%d,%d,%d,%d", RedStartInt, GreenStartInt, BlueStartInt, RedIncInt, GreenIncInt, BlueIncInt);
		    MessageBox(hwnd, s, "", MB_ICONEXCLAMATION | MB_OK);
#endif
		    break;
		case 'V':				// bailout limit
		    gManp->GetBailout(token + 2);
		    break;
		case 'W':				// parameter list
		    gManp->GetParamsList(token + 2);
		    break;
		case 'X':				// Invert fractal
		    gManp->ProcessInvert(token + 2);
		    break;
		case 'Y':				// starting place in colour cycling
		    StartColourCycling = atoi(token + 2);
		    InitTrueColourPalette(FALSE);
		    break;
		case 'Z':				// parameters req
		case 'J':				// julia set
		    gManp->juliaflag = TRUE;
		    gManp->get_julia_loc(token + 2);
		    break;
		default:
#ifdef DEBUG
		    _snprintf_s(s, 200, _TRUNCATE,
			"Unknown Token <%s> in file: %s",
			token, filename);

		    MessageBox(hwnd, s, "Paul's Fractals",
			MB_ICONEXCLAMATION | MB_OK);
#endif
		    break;
		}
	    }
	
	token = strtok(NULL, seps);
	}

	gManp->RotationAngle = gManp->RotationAngle % 360;
    if (gManp->RotationAngle == 0 || gManp->RotationAngle == 90 || gManp->RotationAngle == 180 || gManp->RotationAngle == 270)		// save calcs in rotating, just remap
	gManp->RotationCentre = 0.0;

    if (gManp->logval)
	if (gManp->threshold >= MAXTHRESHOLD)
	    gManp->threshold = MAXTHRESHOLD;
    return 0;
    }

/**************************************************************************
	Find the name string for the subtype
**************************************************************************/

void	FindSubtypeName(char *SubTypeName, int type, int subtype)
    {
    if (type == OSCILLATORS)
	strcpy(SubTypeName, OscillatorSpecific[subtype].name);
    else if (type == FRACTALMAPS)
	strcpy(SubTypeName, FractalMapSpecific[subtype].name);
    else if (type == SPROTTMAPS)
	strcpy(SubTypeName, SprottMapSpecific[subtype].name);
    else  if (type == SURFACES)
	strcpy(SubTypeName, SurfaceSpecific[subtype].name);
    else  if (type == KNOTS)
	strcpy(SubTypeName, KnotSpecific[subtype].name);
    else  if (type == CURVES)
	strcpy(SubTypeName, CurveSpecific[subtype].name);
    else  if (type == PERTURBATION)
	strcpy(SubTypeName, PerturbationSpecific[subtype].name);
    }

/**************************************************************************
	Fractal data to string
**************************************************************************/

char	*FractData(void)
    {
    static	char	info[MAXDATALINE];	// extra SIZEOF_BF_VARS bytes for "-c", "-t" etc, spaces and values

    info[0] = '\0';
    StringBuilder sb(info, sizeof(info));

    if (gManp->BigNumFlag)
	{
	std::vector<char> s1(SIZEOF_BF_VARS);
	std::vector<char> s2(SIZEOF_BF_VARS);
	std::vector<char> s3(SIZEOF_BF_VARS);

	gManp->BigHor.ToString(s1.data(), (int)s1.size(), false);
	gManp->BigVert.ToString(s2.data(), (int)s2.size(), false);
	gManp->BigWidth.SafeSprintf(s3.data(), (int)s3.size(), "%.20Re");
	sb.append("-c%s,%s,%s -t%d", s1.data(), s2.data(), s3.data(), gManp->threshold);
	}
    else
	{
	sb.append("-c%24.24f,%24.24f,%24.24f -t%d", gManp->hor, gManp->vert, gManp->mandel_width, gManp->threshold);
	}

    gManp->RebuildFractalMetadata(gManp->type, gManp->subtype);		// load all the metadata for parameters
    BasicFractData(sb, FALSE);
    sb.append("\n");
    return info;
    }

/**************************************************************************
	Fractal data to string (used to build PNG file data)
**************************************************************************/

void	BasicFractData(StringBuilder& sb, BOOL CreateAnim)
    {
    //    char	s[1024];
    char	SubTypeName[1024];
    int		i, flags;

    sb.append(" -i%d,%d,%d,%d", gManp->TrueCol.inside_colour, gManp->TrueCol.InsideRed, gManp->TrueCol.InsideGreen, gManp->TrueCol.InsideBlue);
    if (gManp->calcmode == 'G')
	sb.append(" -MG%d", gManp->blockindex);	// for solid guessing
    else if (gManp->calcmode == 'T')
	sb.append(" -MT%ld", gManp->fillcolor);	// tesseral fill colour
    else if (gManp->calcmode == 'F')
	sb.append(" -MF%f,%d,%f,%f,%f", gManp->bump_transfer_factor, gManp->PaletteStart, gManp->lightDirectionDegrees, gManp->bumpMappingDepth, gManp->bumpMappingStrength);
    else
	sb.append(" -M%c", gManp->calcmode);

    if (NonStandardImage)			// not full screen view
	{
	sb.append(" -G%d,%d", gManp->width, gManp->height);
	}

    if (gManp->InsideMethod)
	{
	sb.append(" -DI%03d", gManp->InsideMethod);	// display method
	if (gManp->OutsideMethod > TIERAZONFILTERS)
	    {
	    sb.append(",%f,%d,%d", gManp->dStrands, gManp->nFDOption, gManp->UseCurrentPalette);	// Parameters for the Tierazon filters
	    }
	else if (gManp->OutsideMethod == POTENTIAL)
	    {
	    sb.append(",%f,%f,%f", gManp->potparam[0], gManp->potparam[1], gManp->potparam[2]);	// Parameters for potential
	    }
	}
    if (gManp->OutsideMethod)
	{
	sb.append(" -DO%03d", gManp->OutsideMethod);	// display method
	}
    //    if (palette_flag && !CreateAnim)
    //	{
    //	sb.append(" -a\"%s\"", MAPFile);
    //	}
    if (gManp->juliaflag)
	{
	sb.append(" -J%13.13f,%13.13f", gManp->j.x, gManp->j.y);
	}
    switch (gManp->type)
	{
	case MANDELFP:					// Mandelbrot
	case BIFURCATION:
	case BIFSTEWART:
	case BIFLAMBDA:
	case BIFADSINPI:
	case BIFEQSINPI:
	case BIFMAY:
	case QUADMAND:
	case IKEDA:
	case ZIGZAG:
	case GARGOYLE:
	case CURLICUES:
	case CHUA:
	case BURNINGSHIP:
	    //	case BUFFALO:
	    //	case PERPBUFFALO:
	    //	case MANDELBAR:
	    //	case CELTIC:
	    //	case MANDELBARCELTIC:
	    //	case PERPCELTIC:
	    //	case CUBICFLYINGSQUIRREL:
	case BUDDHABROT:
	case THORN:
	case POPCORN:
	case FPPOPCORN:
	case LPOPCORN:
	case FPPOPCORNJUL:
	case LPOPCORNJUL:
	    //	case PERPBURNINGSHIP:
	    sb.append(" -f%03d", gManp->type);
	    break;					// default
	case NEWTON:
	case NEWTBASIN:					// Newton Fractal (basin or stripe)
	    sb.append(" -f%03d%03d%03d%03d", gManp->type, gManp->subtype, gManp->degree, gManp->special);
	    break;
	case HENON:
	case MALTHUS:
	case GEOMETRY:
	case CIRCLES:
	case TRIANGLES:					// Triangles fractal
	case CROSSROADS:
	case PASCALTRIANGLE:
	case APOLLONIUS:
	case APOLLONIUSIFS:
	case SIERPINSKIFLOWERS:
	case MANDELDERIVATIVES:
	    sb.append(" -f%03d%d", gManp->type, gManp->subtype);
	    break;					// default
	case PERTURBATION:
	    FindSubtypeName(SubTypeName, gManp->type, gManp->subtype);
	    sb.append(" -f%03d\"%s\",%d,%s", gManp->type, SubTypeName, gManp->SlopeType, (gManp->EnableApproximation ? "true" : "false"));
	    break;
	case FRACTALMAPS:
	case SPROTTMAPS:
	case SURFACES:
	case KNOTS:
	case CURVES:
	case OSCILLATORS:
	    flags = ((gManp->type == OSCILLATORS) ? DisplayLines : ((gManp->type == FRACTALMAPS || gManp->type == SPROTTMAPS) ? FractalMapColouring : RemoveHiddenPixels)) + (gManp->CoordSystem << 1) + BlockAnimation + FindCentre + gManp->PerspectiveFlag;
	    FindSubtypeName(SubTypeName, gManp->type, gManp->subtype);
	    sb.append(" -f%03d\"%s\",%d,%d,%d,%d,%d,%f,%f,%f,%f", gManp->type, SubTypeName, (gManp->xAxis + 1)*xSign, (gManp->yAxis + 1)*ySign, gManp->zAxis + 1, flags, gManp->MaxDimensions, gManp->iterations, VertBias, dt, zBias);
	    break;
	case FOURIER:					// fourier;which type;harmonics;steps
	    sb.append(" -f%03d%03d%03d%04d%s", gManp->type, gManp->subtype, NumHarmonics, steps, WriteSliders());
	    break;
	case NEWTONVARIATION:				// Newton Variation fractals
	case NEWTONPOLYGON:				// Newton Polygon types
	case POWER:					// POWER types
	case BURNINGSHIPPOWER:				// Burning Ship of other powers besides quadratic
	    sb.append(" -f%03d%d", gManp->type, gManp->degree);
	    break;
	case LSYSTEM:					// lsys;which one in file;order;filename
	    sb.append(" -f%03d%03d%03d\"%s\"", gManp->type, lsys_ptr, gManp->degree, LSYSFile);
	    break;
	case CUBIC:					// CUBIC types
	case RATIONALMAP:				// Rational Maps
	case EXPFRACTAL:				// Exponential
	    sb.append(" -f%03d%03d%03d", gManp->type, gManp->subtype, gManp->special);
	    break;
	case SPECIALNEWT:
	case SINFRACTAL:				// sin() type fractal
	case MATEIN:					// MATEIN
	    sb.append(" -f%03d%d", gManp->type, gManp->special);
	    break;
	case FIBONACCI:					// Fibonacci Spirals
	    sb.append(" -f%03d%d", gManp->type, gManp->subtype);
	    break;
	case TIERAZON:					// Tierazon fractals
//	case MARCUSMANDEL:				// general Marcus fractal
	    sb.append(" -f%03d%03d", gManp->type, gManp->subtype);
	    break;
	case SCREENFORMULA:				// On Screen Formula fractals
	case FORMULA:					// formula files
	case FFORMULA:
	    sb.append(" -f%03d\"%s\"", gManp->type, gManp->FormulaString);
	    break;
	case IFS:					// IFS fractals
	    sb.append(" -f%03d%03d\"%s\"", gManp->type, lsys_ptr, IFSFile);
	    break;
	case LYAPUNOV:					// Lyapunov fractals
	    sb.append(" -f%03d -#%s", gManp->type, gManp->LyapSequence);
	    break;
	default:
	    sb.append(" -f%03d%d", gManp->type, gManp->subtype);
	}
    if (gManp->logval)
	{
	sb.append(" -l%d", gManp->logval);
	}
    sb.append(" -u%d,%d,%d,%d,%d,%d", gManp->TrueCol.RedStartInt, gManp->TrueCol.GreenStartInt, gManp->TrueCol.BlueStartInt, gManp->TrueCol.RedIncInt, gManp->TrueCol.GreenIncInt, gManp->TrueCol.BlueIncInt);
    if (gManp->pairflag)
	{
	sb.append(" -h%d", gManp->pairflag);
	}
    if (gManp->RotationAngle != NORMAL)
	{
	if (gManp->RotationAngle != 90 && gManp->RotationAngle != 180 && gManp->RotationAngle != 270)
	    sb.append(" -o%d,%lf,%lf", gManp->RotationAngle, gManp->RotationCentre.x, gManp->RotationCentre.y);
	else
	    sb.append(" -o%d", gManp->RotationAngle);
	}
    if (gManp->AutoStereo_value != 75)
	{
	sb.append(" -r%d", gManp->AutoStereo_value * stereo_sign);
	}
    if (gManp->PerspectiveFlag)
	{
	sb.append(" -2%lf,%lf,%lf,%lf,%lf,%lf", gManp->sclx, gManp->scly, gManp->sclz, gManp->x_rot, gManp->y_rot, gManp->z_rot);
	}
    if (gManp->_3dflag || ifs_type == 1)
	{
	sb.append(" -3%lf,%lf,%lf,%lf,%lf,%lf", gManp->sclx, gManp->scly, gManp->sclz, gManp->x_rot, gManp->y_rot, gManp->z_rot);
	}
    if (gManp->period_level != 1)
	{
	sb.append(" -p%d", gManp->period_level);
	}
    if (gManp->decomp > 0)
	{
	sb.append(" -k%d", gManp->decomp);
	}
    if (gManp->biomorph >= 0)
	{
	sb.append(" -b%d", gManp->biomorph);
	}
    if (gManp->distest > 0)
	{
	sb.append(" -$%d,%d", gManp->distest, gManp->distestwidth);
	}
    if ((gManp->CoordSystem != CARTESIAN))
	{
	sb.append(" -q%d", gManp->CoordSystem);
	}
    if (gManp->invert)
	{
	sb.append(" -x%f,%f,%f", gManp->f_radius, gManp->f_xcenter, gManp->f_ycenter);
	}
    if (StartColourCycling > 0)
	{
	sb.append(" -y%d", StartColourCycling);
	}
    //    fprintf(fb, " %%1 %%2 %%3\n");
    if (gManp->Fractal.NumFunct > 0)
	{
	if (gManp->Fractal.NumFunct == 1)
	    sb.append(" -n%s", gManp->Fractal.Fn1);
	else if (gManp->Fractal.NumFunct == 2)
	    sb.append(" -n%s,%s", gManp->Fractal.Fn1, gManp->Fractal.Fn2);
	}
    if (gManp->ColourSpeed != 0.0)		// used for colour smoothing
	{
	sb.append(" -~%lf", gManp->ColourSpeed);
	}
    if (gManp->PalOffset > 0)			// Kalles palette index offset
	{
	sb.append(" -^%d", gManp->PalOffset);
	}
    if (gManp->IterDiv != 1.0)			// divide ieration by this amount
	{
	sb.append(" -@%lf", gManp->IterDiv);
	}

    if (gManp->type == OSCILLATORS || gManp->type == FRACTALMAPS || gManp->type == SPROTTMAPS || gManp->type == SURFACES || gManp->type == KNOTS || gManp->type == CURVES)	// these have a different database
	{
	if (NumVariables > 0)						// variables are stored in gManp->param[10] to gManp->param[19]
	    {
	    sb.append(" -w");
	    for (i = 0; i < NumVariables + 10 - 1; i++)
		{
		sb.append("%g,", gManp->param[i]);
		}
	    sb.append("%g", gManp->param[i]);					// last one without the ','
	    }
	}
    // Slope/Pert use all 16 transport slots; param[15] stores PrePaletteColour.
    else if (gManp->type == SLOPEDERIVATIVE || gManp->type == SLOPEFORWARDDIFF || gManp->type == PERTURBATION)
	{
	sb.append(" -w");
	for (i = 0; i < NUMSLOPEPARAM - 1; i++)
	    {
	    sb.append("%g,", gManp->param[i]);
	    }
	sb.append("%g", gManp->param[i]);					// last one without the ','
	}
    else 
	{
	if (gManp->Fractal.NumParam > 0)
	    {
	    sb.append(" -w");
	    for (i = 0; i < gManp->Fractal.NumParam - 1; i++)
		{
		sb.append("%g,", gManp->param[i]);
		}
	    sb.append("%g", gManp->param[i]);					// last one without the ','
	    }
	}
    sb.append(" -v%lf,%d", *gManp->Fractal.rqlim, gManp->BailoutTestType);
    }

/**************************************************************************
	Get user data from keyboard - move into Maninit once we reduce the heavy global load
**************************************************************************/

void	CManp::setup_defaults(void)
    {
    int	i;

    UpdateDelay = (type == FIBONACCI || type == FOURIER) ? 10 : 1000;
    InsideMethod = NONE;
    OutsideMethod = NONE;
    special = GREEN;
    degree = 3;
    subtype = 0;
    gManp->exitflag = FALSE;
    RotationAngle = NORMAL;
    TrueCol.RandomDivisor = 128;
    fillcolor = -1;			// tesseral fillcolor: -1=normal 0 means don't fill     
    period_level = 1;
    TrueCol.inside_colour = 246;
    TrueCol.InsideRed = 40;		// values for r, g, b channels for inside colour
    TrueCol.InsideGreen = 40;
    TrueCol.InsideBlue = 40;
    threshold = 250;
    NonStandardImage = FALSE;
    _3dflag = FALSE;
    PerspectiveFlag = FALSE;
    grayflag = FALSE;
    CoordSystem = CARTESIAN;
    RGBFilter = FALSE;			// don't use the plotting routine for RGB filters
    PlotType = NOSYM;
    FilterType = 1;
    ColourMethod = 1;			// Tierazon colour methods
    BailoutTestType = BAIL_MOD;
    calcmode = 'G';
#ifdef TESTFWDDIFF
    calcmode = 'F';
#endif // TESTFWDDIFF

    // stuff for DwdDiff algorithm
    bump_transfer_factor = 1.0;
    PaletteStart = 0;
    lightDirectionDegrees = 45.0;
    bumpMappingDepth = 50.0;
    bumpMappingStrength = 75.0;
    LightHeight = 2.0;
    IterDiv = 1.0;
    PalOffset = 0;
    PertColourMethod = 0;
    EnableApproximation = true;
    oldcalcmode = calcmode;
    cycleflag = FALSE;
    pairflag = FALSE;
    blockindex = 2;
    biomorph = -1;
    decomp = 0;
    juliaflag = FALSE;
    RealTimeJuliaFlag = FALSE;
    logval = FALSE;
    Return2Start = FALSE;
    hor = -3.5;
    vert = -2.0;
    mandel_width = 4.0;
    BigNumFlag = FALSE;
    j = 0.0;
    x_rot = 10.0;
    y_rot = 5.0;
    z_rot = 0.0;
    sclx = 0.9;
    scly = 0.9;
    sclz = 0.6;
    ColourSpeed = 0.0;
    CurrentDelay = 20.0;		// delay in milliseconds
    dStrands = 0.08;
    RGBFilter = FALSE;
    AutoStereo_value = 75;
    invert = FALSE;
    f_radius = 1.0;			// inversion radius 
    f_xcenter = 0.0;			// inversion center 
    f_ycenter = 0.0;
    PaletteShift = 0;			// palette movement between animation frames
    distest = 0, distestwidth = 71;
    for (i = 0; i < 20; i++)
	gManp->param[i] = 0.0;

    potparam[0] = 255;
    potparam[1] = 820;
    potparam[2] = 20;

    if (logval)
	if (threshold >= MAXTHRESHOLD)
	    threshold = MAXTHRESHOLD;
    RealTimeJuliaFlag = 0;
    }




