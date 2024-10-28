/*
    USER.CPP a module with user option type routines for ManpWIN.
    
    Written in Microsoft Visual 'C++' by Paul de Leeuw.

    This program is written in "standard" C. Hardware dependant code
    (console drivers & serial I/O) is in separate machine libraries.
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
#include	".\parser\TrigFn.h"

/**************** Big Number Globals *********************/

extern	int	dec;
extern	BYTE	BigNumFlag;		// True if bignum used
extern	BigDouble   BigHor, BigVert, BigWidth;

/**************** Other Globals *********************/

extern	CTrueCol    TrueCol;

extern	char	LSYSFile[];
extern	char	lptr[][100];
extern	char	lsys_type[];
extern	char	IFSFile[];		// IFS file
extern	char	MAPFile[];		// colour map file
extern	char	AxesText[];		// used to convert axes and display them

extern	int	ifs_type;
extern	BYTE	orig_palette[];		// loaded palette

extern	RECT 	r;
extern	char	FormulaString[];	// used to hold the full formula

extern	WORD	type;			// M=mand, N=Newton etc
extern	int	subtype;		
extern	int	PaletteShift;		// fractional palette addressing
extern	int	height, xdots, ydots, width, bits_per_pixel;
extern	double	ScreenRatio;		// ratio of width / height for the screen

extern	WORD	special;		// special colour, phase etc
extern	WORD	degree;			// power
extern	int	cycles;			// Bif types, num cycles before plot
extern	int	period_level;		// 0 for no periodicity checking
extern	BYTE	screenflag;		// replay saved screen
extern	int	biomorph;		// biomorph colour
extern	BYTE	cycleflag;		// do colour cycling
extern	int	decomp;			// number of decomposition colours
extern	BYTE	_3dflag;		// replay saved file 3D
extern	BYTE	PerspectiveFlag;	// display using perspective
extern	BOOL	ZoomEdge;		// Zooming process
extern	BYTE	orbit_flag;		// display orbits?
extern	BYTE	exitflag;		// exit on completion
extern	BYTE	juliaflag;		// Julia implementation of fractal
extern	BYTE	RealTimeJuliaFlag;	// Display Julia set in real time
extern	char	floatflag;		// floating point maths
extern	BYTE	grayflag;		// use grey value not colour number
extern	BYTE	pairflag;		// stereo pair flag and window size
extern	BYTE	calcmode;		// trace type B, G, 1, 2
extern	BYTE	oldcalcmode;		// store values during 3D transformations etc
extern	int	logval;			// log colour map starting value

extern	long    fillcolor;		// tesseral fillcolor: -1=normal 0 means don't fill     
extern	BOOL	RunAnimation;		// are we in the middle of an animation run?
extern	WORD	UpdateDelay;		// delay in milliseconds

extern	long	threshold;		// maximum iterations
extern	int	AutoStereo_value;	// AutoStereo depth value
extern	int	eye_dots;		// eye dots for AutoStereo
extern	int	stereo_sign;
extern	double	HenonA, HenonXStart, HenonYStart, HenonStep;
extern	int	HenonPoints;
//extern	int	method;			// inside and outside filters
extern	int	InsideMethod;		// the number of the inside filter
extern	int	OutsideMethod;		// the number of the outside filter
extern	int	FilterType;		// data for Tierazon filters
extern	int	ColourMethod;		// Tierazon colour methods

extern	BOOL	RGBFilter;		// If true, we use the plotting routine for RGB filters (no plotting of iteration)
extern	int	PlotType;
extern	double	CurrentDelay;		// delay in milliseconds for rotation

extern	double	x_rot;			// angle display plane to x axis
extern	double	y_rot;			// angle display plane to y axis
extern	double	z_rot;			// angle display plane to z axis
extern	double	sclx, scly, sclz;	// scale

extern	double	hor;			// horizontal address
extern	double	vert;			// vertical address
extern	double	mandel_width;		// width of display
extern	double	closenuff;		// periodicity bailout
extern	double	dStrands;		// for Tierazon filters
extern	int	nFDOption;		// Fractal Dimension option for Tierazon filters
extern	BOOL	UseCurrentPalette;	// do we use the ManpWIN palette for Tierazon filters? If false, generate internal filter palette

extern	double	c_imag;			// imaginary part of offset/
//extern	long	c_imag_int;		// imaginary part of offset

extern	int	ixstart, ixstop, iystart, iystop;	// start, stop here
extern	int	xxstart,xxstop;			// these are same as worklist,
extern	int	yystart,yystop,yybegin;		// declared as separate items

extern	int	blockindex;		// for solid guessing blocksize
extern	int	lsys_ptr;

extern	WORD	steps, NumHarmonics;	// for Fourier Analysis

extern	BOOL	NonStandardImage;	// has user changed image size?
extern	BOOL	AutoExitFlag;
extern	BOOL	AutoSaveFlag;
extern	int	time_to_restart;	// time to restart?

extern	RECT 	WARect;			// this is the usable screen taking taskbar into account
extern	double	rqlim;			// bailout limit
extern	int	BailoutTestType;	// type of bailout test
extern	double	param[];

extern	int	StartColourCycling;	// we can start the colour cycling from any point..good for cycling animations
	BOOL	invert = FALSE;		// invert fractal
extern	double	f_radius, f_xcenter, f_ycenter;	// inversion radius, center 
extern	int	xAxis, yAxis, zAxis;	// numerical values for axes for chaotic oscillators
extern	int	MaxDimensions;
extern	int	OscillatorPtr;		// points to current subtype
extern	int	OscPtrArray[];		// array of pointers to specific oscillators or fractal maps
extern	double	iterations;		// used in Chaotic Oscillators and Fractal Maps
extern	double	VertBias;		// allow vertical stretching of the image
extern	double	zBias;			// allow stretching of the image in the z direction
extern	double	dt;			// delta time
extern	BOOL	DisplayLines;		// used in Chaotic Oscillators
extern	BOOL	FractalMapColouring;	// used in Fractal Maps
extern	BOOL	RemoveHiddenPixels;	// used in Surfaces
extern	int	NumParams;		// number of parameters in this oscillator or fractal map
extern	int	NumVariables;		// number of variables in this oscillator or fractal map
extern	int	xSign, ySign;		// sign of x and y axes - allow reflection of image across axis
extern	int	CoordSystem;		// transform between Cartesian, Spherical and Cylindrical co-ordinate systems
extern	int	FindCentre;		// statistical average or contained volume
extern	BOOL	BlockAnimation;		// used for 2D or less to preventanimating non-existent dimensions
extern	BOOL	Return2Start;		// do we want to go backwards to the start?
extern	int	distest, distestwidth;	// distance estimation
extern	double	potparam[];		// potential parameters
extern	char	LyapSequence[];		// hold the AB sequence for Lyapunov fractals

// stuff for DwdDiff algorithm
extern	double	bump_transfer_factor;
extern	int	PaletteStart;
extern	double	lightDirectionDegrees;
extern	double	bumpMappingDepth;
extern	double	bumpMappingStrength;
extern	int	SlopeType;
extern	double	LightHeight;
extern	int	RotationAngle;		// rotate image in degrees
extern	Complex	RotationCentre;		// centre of rotation
extern	int	PalOffset;		// begin palette here
extern	double	IterDiv;		// divide ieration by this amount
extern	int	PertColourMethod;	// some ideas from Kalles

extern	void	AnalysePalette(char *);
extern	void	AnalyseDistEst(char *s);

extern	void	Axes2Text(char *text, int x, int y, int z);
extern 	void	set_palette(void), get_julia_loc(char *), analyse_3d(char *), AnalyseRotation(char *s), analyse_inside(char *),
		draw3D(HWND), stereo_init(void), AnalyseMethod(char *);

extern	int	analyse_corner(char *);

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
extern	int	ProcessInvert(char *);
extern	int	ProcessFormulaString(char *);
extern	int	ifsload(HWND, char *);
extern	int	get_IFS_names(HWND, char *);		// get the IFS fractal names
extern	char	*strtok1(register char *, register const char *);	// see maninit.c 
extern	void	ConvertBignum2String(char *s, mpfr_t num);
extern	int	setup_Oscillator(void);			// sort the oscillator database alphabetically
extern	int	setup_FractalMaps();			// sort the fractal maps database alphabetically
extern	int	setup_SprottMaps(void);			// count how many Sprott map fractals there are
extern	int	setup_Surface();			// sort the surface database alphabetically
extern	int	setup_Knot();				// sort the knot database alphabetically
extern	int	setup_Curve();				// sort the curve database alphabetically
extern	int	setup_Perturbation(void);		// count how many Perturbation fractals there are
//extern	void	FilterPoint(WORD, WORD, DWORD);

int	GetParamData(HWND, LPSTR, LPSTR, LPSTR, BOOL);
void	BasicFractData(char *, BOOL);

//extern	double 	*wpixels;			// an array of doubles holding slope modified iteration counts
extern	CDib		Dib;
extern	CPlot		Plot;				// image plotting routines 
extern	CFract		Fractal;
extern	COscProcess	OscProcess;
extern	CMatrix		Mat;				// transformation and roatation matrix
extern	CBigMatrix	BigMat;				// transformation and roatation matrix
extern	Complex		j;

static	bool OldPertFormat;				// used to get param values for old format perturbation par files

/**************************************************************************
	Reduce binary 24 bits to 4 ASCII characters
**************************************************************************/

void	ConvertRGB2ASCII(BYTE r, BYTE g, BYTE b, char *buf)

    {
    BYTE    s[4], a;
    int	    i, k;

    s[0] = b & 0x3f;
    s[1] = (b & 0xc0) >> 6;
    a	 = (g & 0x0f) << 2;
    s[1] |= a;
    s[2] = (g & 0xf0) >> 4;
    a	 = (r & 0x03) << 4;
    s[2] |= a;
    s[3] = r >> 2;

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

void	ConvertASCII2RGB(BYTE *r, BYTE *g, BYTE *b, char *buf)

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
	    k -= ('A'-10);
	else if (k < '_' || k > 'z') 
	    error = TRUE;
	else                     
	    k -= ('_'-36);
	s[i] = (error) ? 0 :(char)k;
	}

    *r = s[3] << 2;
    a = (s[2] & 0xc0) >> 4;
    *r |= a;
    *g = (s[2] & 0x0f) << 4;
    a = s[1] >> 2;
    *g |= a;
    *b = (s[1] << 6) | s[0];
    }

/**************************************************************************
	Write batch file with current parameters
**************************************************************************/

void	output_batch(double h, double v, double w, HWND hwnd, LPSTR filename)

    {
    FILE	*fb;				// batch file   
    char	s[400];
    char	info[MAXDATALINE];
    char	*s1 = NULL;
    char	*s2 = NULL;
    char	*s3 = NULL;
    char	ascii[6];
    long	i, k;

    if ((fb = fopen(filename, "w")) == NULL)
	{
	wsprintf(s, "\nCan't open file: %s for save", filename);
	MessageBox (hwnd, s, "View", MB_ICONEXCLAMATION | MB_OK);
	MessageBeep (0);
	return;
	}
    else
	{
	if (BigNumFlag)
	    {
	    s1 = new char[SIZEOF_BF_VARS];
	    s2 = new char[SIZEOF_BF_VARS];
	    s3 = new char[SIZEOF_BF_VARS];
	    ConvertBignum2String(s1, BigHor.x);
	    ConvertBignum2String(s2, BigVert.x);
//	    ConvertBignum2String(s3, BigWidth.x);
	    mpfr_sprintf(s3, "%.20Re", BigWidth.x);
	    fprintf(fb, "-c%s,%s,%s", s1, s2, s3);
	    if (s1) delete [] s1;
	    if (s2) delete [] s2;
	    if (s3) delete [] s3;
	    }
	else
	    fprintf(fb, "-c%24.24f,%24.24f,%24.24f", h, v, w);
	fprintf(fb, " -t%d", threshold);

	*info = '\0';
	BasicFractData(info, FALSE);
	fprintf(fb, "%s\n", info);
	fprintf(fb, "Palette=\n");
	for (i = 0, k = 0; i < threshold; i++, k++)
	    {
	    if (k == 20)							// group into lumps of 20
		{
		k = 0;
		fprintf(fb, "\n");
		}
	    if (i >= MAXTHRESHOLD)						// just in case
		{
		fprintf(fb, "\n");
		break;
		}
	    ConvertRGB2ASCII(*(TrueCol.PalettePtr + i * 3 + 0), *(TrueCol.PalettePtr + i * 3 + 1), *(TrueCol.PalettePtr + i * 3 + 2), ascii);
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
	DatabasePtr = OscProcess.LoadDatabasePointer(type, i);
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
    type = atoi(t);
    if (type == 0)
	type = MANDELFP;				// fix silly par files from the past
    if (type == '9')
	type = MATEIN;					// fix silly par files from the past

    if (type > MAXTYPE && type != FRACTPAR)		// edit MAXTYPE in fractalp.h if the total number of fractals changes
	return -1;

    switch (type)					// set defaults
	{
	case MANDELFP:					// good old Mandelbrot
	case NEWTON:					// Newton Fractal
	case NEWTBASIN:					// Newton Fractal (basin or stripe)
	case FIBONACCI:					// Fibonacci Spirals
	case NEWTONAPPLE:				// newton apple
	case NEWTONFLOWER:				// Newton flower
	case MAGNET1:					// magnets 1 and 2
	case MAGNET2:
	case TEDDY:					// strange little fractal
	case NEWTONMSET:				// okay, I have a thing for Newtons
	case NEWTONJULIANOVA:
	case TALIS:
	case NEWTONCROSS:
	case BIFURCATION:
	case BIFSTEWART:
	case BIFLAMBDA:
	case BIFADSINPI:
	case BIFEQSINPI:
	case BIFMAY:
	case QUADMAND:
	case IKEDA:
	case CROSSROADS:
	case PASCALTRIANGLE:
	case APOLLONIUS:
	case APOLLONIUSIFS:
	case SIERPINSKIFLOWERS:
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
//	case BUFFALO:
//	case PERPBUFFALO:
	case MANDELBAR:
//	case CELTIC:
//	case MANDELBARCELTIC:
//	case PERPCELTIC:
//	case CUBICFLYINGSQUIRREL:
	case BUDDHABROT:
//	case PERPBURNINGSHIP:
	case OSCILLATORS:
	case FRACTALMAPS:
	case SPROTTMAPS:
	case SURFACES:
	case KNOTS:
	case CURVES:
	    break;
//	case BIFURCATION:				// Bifurcation Fractal
//	    subtype = 'B';
//	    special = 50;				// cycles before display
//	    break;
	case CUBIC:					// CUBIC fractals
	    subtype = 'B';				// CBIN */
	    special = WHITE;
	    break;
	case POWER:					// POWER fractals
	case BURNINGSHIPPOWER:				// Burning Ship of other powers besides quadratic
	    special = 3;				// power = 3
	    break;
	case NEWTONPOLYGON:				// Newton Polygon fractals
	    degree = 5;					// Degree = 5
	    break;
	case NEWTONVARIATION:				// Newton Variation fractals
	    degree = 3;					// Degree = 3
	    break;
	case FOURIER:					// Fourier Analysis fractals
	    break;
	case SPECIALNEWT:				// type SPECIALNEWT fractal
	    special = 16;				// special in type 5 fractal is multiple colour flag
	    break;
	case RATIONALMAP:				// Rational Map type fractal
	    subtype = 'A';				// RAT34
	    special = WHITE;
	    break;
	case SINFRACTAL:				// sin() type fractal
	case EXPFRACTAL:				// Exponential type fractal
	case MATEIN:					// MATEIN type fractal
	    subtype = 'R';				// REAL
	    special = WHITE;				// special colour
	    break;
//	default:
//	    return -1;					// dunno this fractal
	}
    switch (type)
	{
	case NEWTON:					// Newton Fractal
	case NEWTBASIN:					// Newton Fractal (basin or stripe)
	    subtype = 'N';
	    special = 3;				// default cubic
	    strncpy(t, str + 3, 3);
	    t[3] = '\0';
	    subtype = atoi(t);
	    strncpy(t, str + 6, 3);
	    t[3] = '\0';
	    degree = atoi(t);
	    strncpy(t, str + 9, 3);
	    t[3] = '\0';
	    special = atoi(t);
//	    fractalspecific[type].isNewton = TRUE;
/*
	    if (isalpha(*(str + 3)))
		{
		subtype = (int)toupper(*(str + 3));
		if (*(str + 3) != '\0')
		    degree = atoi(str + 4);
		}
	    else
		{
		if (*(str + 3) != '\0')
		    degree = atoi(str + 3);
		}
*/
	    break;
	case PERTURBATION:
	    if (!isdigit(*(str + 3)))				// new format
		{
		subtype = 1;					// default Mandelbrot
		subtype = PertName2Subtype(type, SaveString);	// search database for matching subtype
		if (subtype < 0)
		    {
		    MessageBox(hwnd, SaveString, "Unknown Subtype", MB_ICONEXCLAMATION | MB_OK);
		    MessageBeep(0);
		    subtype = 0;				// use first one if not found
		    }
		sscanf(PastQuote, "%d,%lf,%lf,%lf,%d", &SlopeType, &lightDirectionDegrees, &bumpMappingDepth, &bumpMappingStrength, &PaletteStart);
		OldPertFormat = false;
		}
	    else
		{
		OldPertFormat = true;
		if (*(str + 1) != '\0')
		    subtype = atoi(str + 3);
		else
		    subtype = 0;
		}

	    break;
	case POWER:					/* POWER fractals */
	case NEWTONPOLYGON:				/* Newton Polygon fractals */
	case NEWTONVARIATION:				/* Newton Variation fractals */
	    if (*(str + 1) != '\0')
		degree = atoi(str + 3);
	    break;
	case TIERAZON:					// Tierazon fractals
	case SLOPEDERIVATIVE:
	case SLOPEFORWARDDIFF:
	case MANDELDERIVATIVES:
	    if (*(str + 1) != '\0')
		subtype = atoi(str + 3);
	    break;
	case OSCILLATORS:
	case FRACTALMAPS:
	case SPROTTMAPS:
	case SURFACES:
	case KNOTS:
	case CURVES:
	    subtype = 1;				// default Mandelbrot
//	    strncpy(t, str + 3, 3);
	    subtype = Name2Subtype(type, SaveString);	// search database for matching subtype
	    if (subtype < 0)
		{
		MessageBox(hwnd, SaveString, "Unknown Subtype", MB_ICONEXCLAMATION | MB_OK);
		MessageBeep(0);
		subtype = 0;				// use first one if not found
		}
	    sscanf(PastQuote, "%d,%d,%d,%d,%d,%lf,%lf,%lf,%lf", &xAxis, &yAxis, &zAxis, &flags, &MaxDimensions, &iterations, &VertBias, &dt, &zBias);
	    if (type == OSCILLATORS)
		DisplayLines = flags & 1;
	    else if (type == FRACTALMAPS || type == SPROTTMAPS)
		FractalMapColouring = flags & 1;
	    else
		RemoveHiddenPixels = flags & 1;
	    CoordSystem = (flags & 0x0001E) >> 1;
	    BlockAnimation = flags & 0x0020;
	    FindCentre = flags & 0x0040;
	    PerspectiveFlag = flags & 0x0080;
//	    OscProcess.DisplayAxisImages = flags & AXISIMAGEDISP;
	    xSign = ySign = 1;
	    zAxis--;
	    if (xAxis < 0)
		{
		xSign = -1;
		xAxis = -xAxis - 1;
		}
	    else
		xAxis--;

	    if (yAxis < 0)
		{
		ySign = -1;
		yAxis = -yAxis - 1;
		}
	    else
		yAxis--;
//	    t[3] = '\0';
//	    subtype = atoi(t);
    	    Axes2Text(AxesText, xAxis + 1, yAxis + 1, zAxis + 1);
	    break;
        
	case FOURIER:					// Fourier Analysis fractals
	    steps = 400;
	    NumHarmonics = 60;
	    if (*(str + 3) != '\0')
		{
		strncpy(t, str + 3, 3);
		t[3] = '\0';
		subtype = atoi(t);
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
	case BIFURCATION:				// Bifurcation Fractal
	case CUBIC:					// CUBIC fractals
	case RATIONALMAP:				// Rational Map type fractal
	case EXPFRACTAL:				// Exponential type fractal
	    strncpy(t, str + 3, 3);
	    t[3] = '\0';
	    subtype = atoi(t);
	    strncpy(t, str + 6, 3);
	    t[3] = '\0';
	    special = atoi(t);
/*
	    if (isalpha(*(str + 3)))
		{
		subtype = toupper(*(str + 3));
		if (*(str + 4) != '\0')
		    special = atoi(str + 4);
		}
	    else
		{
		if (*(str + 3) != '\0')
		    special = atoi(str + 3);
		}
*/
	    break;
        
	case MALTHUS:					/* Malthus fractal */
	case TRIANGLES:					/* Triangles fractal */
	case GEOMETRY:					/* Geometric shapes fractal */
	case CIRCLES:					/* Geometric shapes fractal */
	case PASCALTRIANGLE:
	case APOLLONIUS:
	case APOLLONIUSIFS:
	case SIERPINSKIFLOWERS:
	    if (*(str + 3) != '\0')
		subtype = atoi(str + 3);
	    break;

	case FIBONACCI:					/* Fibonacci Spirals */
	    subtype = 'I';				// default linear
	    if (isalpha(*(str + 3)))
		subtype = toupper(*(str + 3));
	    break;
        
	case SPECIALNEWT:				/* type SPECIALNEWT fractal */
							/* special in SPECIALNEWT fractal used as multiple colour flag */
	case SINFRACTAL:				/* sin() type fractal */
	case MATEIN:					/* MATEIN type fractal */
	    if (*(str + 1) != '\0')
		special = atoi(str + 3);
	    break;

	case CIRCLESQ:					/* circle squared */
	    threshold = 256;				/* else poor picture */
//	    hor *= 40.0;
//	    vert *= 40.0;
//	    mandel_width *= 40.0;
	    break;
	case LSYSTEM:
	    strncpy(t, str + 3, 3);
	    t[3] = '\0';
	    lsys_ptr = atoi(t);
	    strncpy(t, str + 6, 3);
	    t[3] = '\0';
	    degree = atoi(t);
	    if (*SaveString)				// there are spaces in the string and therefore we have already captured the formula string
		strcpy(LSYSFile, SaveString);
	    else
		strcpy(LSYSFile, str + 9);
	    break;
	case SCREENFORMULA:				// On Screen Formula fractals
	case FORMULA:					// formula files
	case FFORMULA:
	    if (*SaveString)				// there are spaces in the string and therefore we have already captured the formula string
		strcpy(FormulaString, SaveString);
	    else
		strcpy(FormulaString, str + 3);
	    if (ProcessFormulaString(FormulaString) == -1)
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
	Get user data from keyboard
**************************************************************************/

/*
void	analyse_command(argc, argv)

int	argc;
char	**argv;
*/

void	setup_defaults(void)

    {
    int	i;

    UpdateDelay = (type == FIBONACCI || type == FOURIER) ? 10 : 1000;
//    method = NONE;
    InsideMethod = NONE;
    OutsideMethod = NONE;
    special = GREEN;
    degree = 3;
//    cycles = 50;				// cycles before display
    subtype = 0;
//    subtype = ' ';
    exitflag = FALSE;
    RotationAngle = NORMAL;
//    PaletteFileFlag = FALSE;
//    TrueColourFlag = FALSE;
    TrueCol.RandomDivisor = 128;
//    TrueCol.DisplayPaletteFlag = TRUE;
    fillcolor = -1;				// tesseral fillcolor: -1=normal 0 means don't fill     
    period_level = 1;
    TrueCol.inside_colour = 246;
    TrueCol.InsideRed = 40;			// values for r, g, b channels for inside colour
    TrueCol.InsideGreen = 40;
    TrueCol.InsideBlue = 40;			
    threshold = 250;
    screenflag = FALSE;
    NonStandardImage = FALSE;
    _3dflag = FALSE;
    PerspectiveFlag = FALSE;
//    palette_flag = FALSE;
//    dumpflag = FALSE;
    grayflag = FALSE;
    CoordSystem = CARTESIAN;
    RGBFilter = FALSE;				// don't use the plotting routine for RGB filters
    PlotType = NOSYM;
    FilterType = 1;
    ColourMethod = 1;				// Tierazon colour methods
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
    PalOffset = 0;
    oldcalcmode = calcmode;
    cycleflag = FALSE;
    pairflag = FALSE;
    blockindex = 2;
    biomorph = -1;
    decomp = 0;
    juliaflag = FALSE;
//    juliaflag = FALSE;
    RealTimeJuliaFlag = FALSE;
    logval = FALSE;
    Return2Start = FALSE;
    //    HenonXStart = 0.098;
//    HenonYStart = 0.061;
//    HenonPoints = 5000;
//    HenonA = 1.111;
//    HenonStep = 0.005;
    hor = -3.5;
    vert = -2.0;
    mandel_width = 4.0;
    BigNumFlag = FALSE;
    j = 0;
    x_rot = 10.0;
    y_rot = 5.0;
    z_rot = 0.0;
    sclx = 0.9;
    scly = 0.9;
    sclz = 0.6;
    CurrentDelay = 20.0;			// delay in milliseconds
    dStrands = 0.08;
    RGBFilter = FALSE;
    AutoStereo_value = 75;
    invert = FALSE;
    f_radius = 1.0;			// inversion radius 
    f_xcenter = 0.0;			// inversion center 
    f_ycenter = 0.0; 
    PaletteShift = 0;			// fractional palette addressing
    distest = 0, distestwidth = 71;
    for (i = 0; i < 20; i++)
	param[i] = 0.0;

    potparam[0] = 255;
    potparam[1] = 820;
    potparam[2] = 20;

    if (logval)
	if (threshold >= MAXTHRESHOLD)
	    threshold = MAXTHRESHOLD;
    }

/**************************************************************************
	Get Julia location on Mandelbrot set
**************************************************************************/

void	GetImageSize(char *s)

    {
    char	*t;

    height = 0;			// need to be able to detect old script files
    t = s;
    while(*s && *s != ' ')
	{
	if (*s == ',')
	    *s = ' ';
	s++;
	}
    sscanf(t, "%ld %ld", &width, &height);
    if (height <= 0)		// old script file?
	height = (int)(((double) width + 0.5) / 1.7777777777);
    }

/**************************************************************************
	Get Initial conditions
**************************************************************************/

extern	char	*InitCond[];

int FindInitCond(char *Str)
    {
    int		n;
    size_t	size;

    size = strlen(Str); 
    for(n = 0; InitCond[n]; n++) 
	{
        if(!strnicmp(InitCond[n], Str, size)) 
	    return n;
	}
    return -1;			// not found
    }

/**************************************************************************
	Get Screen Parser Formulae
**************************************************************************/

extern	char	*DirectFormula[];

int FindFormula(char *Str)
    {
    int		n;
    size_t	size;

    size = strlen(Str); 
    for(n = 0; DirectFormula[n]; n++) 
	{
        if(!strnicmp(DirectFormula[n], Str, size)) 
	    return n;
	}
    return -1;			// not found
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
    for(n = 0; n < Fractal.TotalFunctions; n++) 
	{
        if(!strnicmp(TrigFn.FunctList[n], Str, size))
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
    while(*s && *s != ' ')
	{
	if (*s == ',')
	    *s = ' ';
	s++;
	}
    numfn = sscanf(t, "%s %s", fn1, fn2);
    if (numfn == 1 || numfn == 2)
	if ((FnPtr = TrigFn.FindFunct(fn1, Fractal.TotalFunctions)) >= 0)
	    {
	    Fractal.Fn1 = TrigFn.FunctList[FnPtr];
	    Fractal.Fn1Index = FnPtr;
	    //	    dtrig0 = *FnctList[FnPtr].ptr;	// load function pointer 
	    }
	else
	    numfn = 0;					// handle -nan()

    if (numfn == 2)
	if ((FnPtr = TrigFn.FindFunct(fn2, Fractal.TotalFunctions)) >= 0)
	    {
	    Fractal.Fn2 = TrigFn.FunctList[FnPtr];
	    Fractal.Fn2Index = FnPtr;
	    //	    dtrig1 = *FnctList[FnPtr].ptr;	// load function pointer 
	    }
	else
	    numfn = 0;					// handle -nan()
    if (numfn == 1 || numfn == 2)
	Fractal.NumFunct = numfn;
    }

/**************************************************************************
	Get Parameters
**************************************************************************/

void	GetParamsList(char *s)

    {
    char	*t;

    t = s;
    while(*s)
	{
	if (!isdigit(*s) && *s != '.' && *s != '+' && *s != '-' && *s != 'e' && *s != 'E')
	    *s = ' ';
	s++;
	}

    if (type == OSCILLATORS || type == FRACTALMAPS || type == SPROTTMAPS || type == SURFACES || type == KNOTS || type == CURVES)			// these have a different database
	NumParams = sscanf(t, "%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf", 
		&param[0], &param[1], &param[2], &param[3], &param[4], &param[5], &param[6], &param[7], &param[8], &param[9],
		&param[10], &param[11], &param[12], &param[13], &param[14], &param[15], &param[16], &param[17], &param[18], &param[19]);
    else
	Fractal.NumParam = sscanf(t, "%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf", Fractal.ParamValue[0], Fractal.ParamValue[1], Fractal.ParamValue[2], Fractal.ParamValue[3], Fractal.ParamValue[4], 
										Fractal.ParamValue[5], Fractal.ParamValue[6], Fractal.ParamValue[7], Fractal.ParamValue[8], Fractal.ParamValue[9]);
    }

/**************************************************************************
	Get Parameters
**************************************************************************/

void	GetBailout(char *s)

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
						// we don't allocate this because we have no idea of the size before we open the file
    char	string[SIZEOF_BF_VARS*3];	// times 3 because of x, y, width
    FILE	*fp;				// param file   
    char	s[200];
    char	ascii[5];
    int		ptr, ch, j;
    long	i;
    BOOL	eof = FALSE;
    long	LocalThreshold;

    ptr = 0;
    setup_defaults();
    if ((fp = fopen(filename, "r")) == NULL)
	{
	wsprintf(s, "Can't open param file: %s for read", filename);
	MessageBox (hwnd, s, "View", MB_ICONEXCLAMATION | MB_OK);
	MessageBeep (0);
	return;
	}

    ch = fgetc(fp);
    while (ch != '\0' && ch != '\n' && ch != -1)
	{
	*(string + ptr++) = (char) ch;
	ch = fgetc(fp);
	}
					    // Add null to end string
    if (ch == -1)
	{
	wsprintf(s, "Can't find info in file: %s", filename);
	MessageBox (hwnd, s, "View", MB_ICONEXCLAMATION | MB_OK);
	MessageBeep (0);
	fclose(fp);
	return;
	}

    *(string + ptr) = '\0';
//    fread(string, 1, 255, fp);
    GetParamData(hwnd, filename, string, szSaveFileName, FALSE);  
    LocalThreshold = (threshold >= MAXTHRESHOLD) ? MAXTHRESHOLD - 1 : threshold;
    if (fgets(string, MAXLINE, fp) == NULL)				// no palette
	InitTrueColourPalette(FALSE);					// process anything we found in the param list, eg palette file etc
    else			
	{
	if (strncmp(string, "Palette=", 8) == 0)			// we have a palette file
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
		ConvertASCII2RGB((TrueCol.PalettePtr + i * 3 + 0), (TrueCol.PalettePtr + i * 3 + 1), (TrueCol.PalettePtr + i * 3 + 2), ascii);
		}
	    }
	}

    Plot.RefreshScreen();
//    if (TrueCol.DisplayPaletteFlag)
	DisplayPalette(hwnd, TrueCol.DisplayPaletteFlag);
    InvalidateRect(hwnd, &r, FALSE);	// force repaint
    fclose(fp);
    }

/**************************************************************************
	Get Fwd Diff Params
**************************************************************************/

void	AnalyseFwdDiff(char *s)

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
	p2 = str_find_ci(p1+1, "\"");
	if(p2)
	    {
	    strncpy(out, p1, p2-p1);
	    *(out + (p2-p1) - 1) = '\0';
	    }
	}
    else
	strcpy(out, in);
    }

/**************************************************************************
	Analyse param data
**************************************************************************/

int	GetParamData(HWND hwnd, LPSTR filename, LPSTR string, LPSTR szSaveFileName, BOOL IsPNG)  

    {						// Establish string and get the first token:
    char	*token;
    char	seps[]   = " \t\n";
    char	i;
    char	s[200];
    char	SaveString[2048];		// get filenames from quotes.
    char	TempBuffer[SIZEOF_BF_VARS * 3];	// Save entire string to get anything within quotes.
						// SIZEOF_BF_VARS times 3 because of x, y, width
    char	*p;				// get the beginning of the string
    char	*q;				// current position in the string
    
    strcpy(TempBuffer, string);			// get a copy in case of corruption by strtok()
    q = p = string;				// remember where the beginning of the string is to count how far in the filename is

    token = strtok(string, seps);
    while (token != NULL)
	{
	if (*token == '-') 
	    {
	    i = *(token + 1);
	    *(token + 1) = i = toupper(i);
	    switch (i) 
		{
		case '~':				// some ideas from Kalles
		    if (*(token + 2))
			PertColourMethod = atoi(token + 2);
		    break;
		case '^':				// shift the palette by this amount
		    if (*(token + 2))
			PalOffset = atoi(token + 2);
		    break;
		case '@':				// divide the number of iterations by this amount
		    if (*(token + 2))
			sscanf(token + 2, "%lf", &IterDiv);
		    break;
		case '#':				// Lyapunov Sequence
		    if (*(token + 2))
			strcpy(LyapSequence, token + 2);
		    break;
		case '$':				// distance estimation
		    if (*(token + 2))
			AnalyseDistEst(token + 2);
		    _3dflag = FALSE;
		    break;
		case '2':				// rotate oscillator in 3D
		    if (*(token + 2))			// modify 3d parameters?
			analyse_3d(token + 2);
		    _3dflag = FALSE;
		    break;
		case '3':				// replay in 3D
		    if (*(token + 2))			// modify 3d parameters?
			analyse_3d(token + 2);
		    _3dflag = TRUE;
		    break;
		case 'A':				// replace palette map
		    RemoveQuotes(MAPFile, trailing(TempBuffer + 2 + (token - p)));
		    break;
		case 'B':				// biomorph colour
		    biomorph = atoi(token + 2);
		    break;
		case 'C':				// corner and width 
		    if (analyse_corner(token + 2) < 0)
			{
			double  AspectRatio = (double)xdots / (double)ydots;
			BigNumFlag = FALSE;
			if (AspectRatio > 1.0)	// take aspect ration into account when plotting Julia
			    {
			    mandel_width = 4.0;
			    vert = -2.0;
			    hor = vert * AspectRatio;
			    }
			else
			    {
			    mandel_width = 4.0 / AspectRatio;
			    hor = -2.0;
			    vert = hor / AspectRatio;
			    }
			param[0] = 0.0;
			param[1] = 0.0;
			sprintf(s, "Deep Zooming Limit (%d decimals) exceeded", SIZEOF_BF_VARS);
			MessageBox(hwnd, s, "ManpWin", MB_ICONEXCLAMATION | MB_OK);
			MessageBeep(0);
			return -1;
			}
		    break;
		case 'D':							// method
		    AnalyseMethod(token + 2);
		    if (OutsideMethod > TIERAZONFILTERS && OutsideMethod < TIERAZONCOLOURS)	// tierazon filters
			{
			FilterType = OutsideMethod - TIERAZONFILTERS;
			RGBFilter = (TierazonFilter[FilterType].rgb) ? TRUE : FALSE;
			}
		    else if (OutsideMethod > TIERAZONCOLOURS)				// tierazon colours)
			{
			ColourMethod = OutsideMethod - TIERAZONCOLOURS;
			RGBFilter = (TierazonColour[ColourMethod].rgb) ? TRUE : FALSE;
			}

			// this next line causes problems with animating Tierazon filters. I'll leave it in for the moment
		    PlotType = (RGBFilter) ? FILTERPLOT : NOSYM;
		    break;
		case 'E':				// exit on completion
		    exitflag = TRUE;
		    AutoExitFlag = TRUE;
		    break;
		case 'F':				// fractal type
		    if (*(token + 2) != '\0')
			{
			RemoveQuotes(SaveString, trailing(TempBuffer + 2 + (token - p)));
			q = token + 8 + strlen(SaveString);	// pointer to the end of the formula string to ensure we don't trap on minus signs in the formula
			if (analyse_fractal(hwnd, token + 2, SaveString, q) < 0)
			    {
			    sprintf(s, "Unknown Fractal <%d> in PAR file", type);
			    MessageBox (hwnd, s, "ManpWin", MB_ICONEXCLAMATION | MB_OK);
			    MessageBeep (0);
			    type = MANDELFP;
			    return -1;
			    }
			}
		    else
			type = MANDELFP;			// Mandelbrot by default
		    if (type == SCREENFORMULA)
			{
//			strcpy(FormulaString, token + 6);	// remember to go past quote
			}
		    else if (type == LSYSTEM)
			{
			if (load_lsystems(hwnd, LSYSFile) >= 0)
			    {
//			    time_to_reinit = FALSE;
//			    time_to_restart = TRUE;
//			    time_to_load = FALSE;
			    }
			Fractal.FractName = fractalspecific[type].name;
			}
		    else if (type == IFS)
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
			time_to_restart = TRUE;
			GetImageSize(token + 2);
			width = atoi(token + 2);
//			height = (int)(((double) width + 0.5) / 1.333333333333333333333);
			NonStandardImage = TRUE;
//			ClosePtrs();		// ready for next screen
			mainview(hwnd, TRUE);	// all screen specific stuff
			}
		    break;
		case 'H':				// stereo pairs
		    if (*(token + 2) != '\0')
			{
			pairflag = atoi(token + 2);
			if (pairflag < 2 || pairflag > 16)
			    pairflag = 2;
			}
		    else
			pairflag = 2;
		    break;
		case 'I':				// inside colour
		    analyse_inside(token + 2);
		    break;
//		case 'J':	See below 'Z'		// julia set
		case 'K':				// number of decomp colours
		    decomp = atoi(token + 2);
		    break;
		case 'L':				// log colour map 
		    if (*(token + 2) == '\0')
			logval = TRUE;
		    else
			logval = atoi(token + 2);
		    break;
		case 'M':				// Calc mode
		    calcmode = *(token + 2);
		    if (calcmode == 'G')
			blockindex = atoi((token + 3));	// for solid guessing
		    if (calcmode == 'T')
			fillcolor = atol((token + 3));	// for tesseral fill colour
		    if (calcmode == 'F')
			AnalyseFwdDiff(token + 2);
		    break;
		case 'N':				// functions
		    GetFunctions(token + 2);
		    break;
		case 'O':				// rotation angle
		    RotationAngle = atoi(token + 2);
		    if (RotationAngle != 0 && RotationAngle != 90 && RotationAngle != 180 && RotationAngle != 270)		// get original rotation centre otherwise we can simply remap
			AnalyseRotation(token + 2);
		    break;
		case 'P':				// periodicity
		    period_level = atoi(token + 2);
		    break;
		case 'Q':				// no sound
		    CoordSystem = atoi(token + 2);
		    break;
		case 'R':				// AutoStereo depth
		    if (toupper(*(token + 2)) == 'G')
			{
			grayflag = TRUE;
			if (*(token + 3) != '\0')
			    AutoStereo_value = atoi(token + 3);
			}
		    else if (*(token + 2) != '\0')
			AutoStereo_value = atoi(token + 2);
		    break;
		case 'S':				// SaveAs filename
//		    RemoveQuotes(szSaveFileName, trailing(token + 2));
		    RemoveQuotes(szSaveFileName, trailing(TempBuffer + 2 + (token - p)));
		    AutoSaveFlag = TRUE;
		    break;
		case 'T':				// threshold 
		    threshold = atol(token + 2);
		    break;
		case 'U':				// Palette parameters (keep for hysterical reasons)

#ifdef DEBUG
wsprintf(s, " i%d,%d,%d,%d,%d,%d", 
		RedStartInt, GreenStartInt, BlueStartInt, RedIncInt, GreenIncInt, BlueIncInt);
MessageBox (hwnd, s, "", MB_ICONEXCLAMATION | MB_OK);
#endif

		    if (*(token + 2))		// modify Palette parameters?
//			{
			AnalysePalette(token + 2);
//			TrueColourFlag = TRUE;
//			}
//		    else
//			TrueColourFlag = FALSE;

#ifdef DEBUG
wsprintf(s, " o%d,%d,%d,%d,%d,%d", 
		RedStartInt, GreenStartInt, BlueStartInt, RedIncInt, GreenIncInt, BlueIncInt);
MessageBox (hwnd, s, "", MB_ICONEXCLAMATION | MB_OK);
#endif
		    break;
		case 'V':				// bailout limit
		    GetBailout(token + 2);
		    break;
		case 'W':				// parameter list
		    GetParamsList(token + 2);
		    break;
		case 'X':				// Invert fractal
		    ProcessInvert(token + 2);	
		    break;
		case 'Y':				// starting place in colour cycling
		    StartColourCycling = atoi(token + 2);
		    InitTrueColourPalette(FALSE);
		    break;
		case 'Z':				// parameters req
		case 'J':				// julia set
		    juliaflag = TRUE;
		    get_julia_loc(token + 2);
		    break;
		default:
		    if (token > q)			// we are past a formula string
			{
#ifdef DEBUG
			wsprintf(s, "Faulty Token <%s> in file: %s for read tok=%x, q=%x", token, filename, token, q);
#else
			wsprintf(s, "Faulty Token <%s> in file: %s for read", token, filename);
#endif
			MessageBox (hwnd, s, "Paul's Fractals", MB_ICONEXCLAMATION | MB_OK);
    			MessageBeep (0);
			}
#ifdef DEBUG
		    else
			{
			wsprintf(s, "Inside saved string tok=%x, q=%x", token, q);
			MessageBox (hwnd, s, "Paul's Fractals", MB_ICONEXCLAMATION | MB_OK);
			}
#endif
		    break;
		}
	    }
	else if((i != 'F') && (i != 'S') && (i != 'A'))		// must be a space in a filename or a required string
	    {
	    if (token > q)					// we are past a formula string
		{
#ifdef DEBUG
		wsprintf(s, "Faulty Token <%s> (not '-') in file: %s for read tok=%x, q=%x", token, filename, token, q);
#else
		wsprintf(s, "Faulty Token <%s> (not '-') in file: %s for read", token, filename);
#endif
		MessageBox (hwnd, s, "View", MB_ICONEXCLAMATION | MB_OK);
		MessageBeep (0);         
		}
#ifdef DEBUG
	    else
		{
		wsprintf(s, "Inside saved string 2 tok=%x, q=%x", token, q);
		MessageBox (hwnd, s, "Paul's Fractals", MB_ICONEXCLAMATION | MB_OK);
		}
#endif
	    }
	token = strtok(NULL, seps);
	}

    RotationAngle = RotationAngle % 360;
    if (RotationAngle == 0 || RotationAngle == 90 || RotationAngle == 180 || RotationAngle == 270)		// save calcs in rotating, just remap
	RotationCentre = 0.0;
    else
	{
	z_rot = (double)(RotationAngle % 360);
	if (BigNumFlag)
	    BigMat.InitTransformation(RotationCentre.x, RotationCentre.y, 0.0, 0.0, 0.0, z_rot);
	else
	    Mat.InitTransformation(RotationCentre.x, RotationCentre.y, 0.0, 0.0, 0.0, z_rot);
	}
    if (logval)
	if (threshold >= MAXTHRESHOLD)
	    threshold = MAXTHRESHOLD;
    if (OldPertFormat)					// we had to make sure we loaded the param values before we copied them into appropriate parameters
	{
	SlopeType = (int)param[0];
	PaletteStart = (int)param[1];
	lightDirectionDegrees = param[2];
	bumpMappingDepth = param[3];
	bumpMappingStrength = param[4];
	if (subtype == 57)				// all param used for coefficients, so set a default
	    LightHeight = 2.0;
	else
	    LightHeight = param[6];
	if (subtype == 1)
	    degree = (int)param[7];
	else if (subtype == 11)
	    degree = (int)param[5];
	}
//    if (!IsPNG)
//	{
//	TrueColourFlag = FALSE;				// colour map takes priority over true colour parameters
//	FilePalette(hwnd, MAPFile, "Fractal Parameter File: Get Colour Map");
//	}
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
    char	*s1;
    char	*s2;
    char	*s3;
    static	char	info[SIZEOF_BF_VARS*3+12];	// extra 12 bytes for "-c", "-t", spaces and threshold

    if (BigNumFlag)
	{
	s1 = new char[SIZEOF_BF_VARS];
	s2 = new char[SIZEOF_BF_VARS];
	s3 = new char[SIZEOF_BF_VARS];
	ConvertBignum2String(s1, BigHor.x);
	ConvertBignum2String(s2, BigVert.x);
	mpfr_sprintf(s3, "%.20Re", BigWidth.x);
//	ConvertBignum2String(s3, BigWidth.x);
	sprintf(info, "-c%s,%s,%s -t%d", s1, s2, s3, threshold);
	if (s1) delete[] s1;
	if (s2) delete[] s2;
	if (s3) delete[] s3;
	}
    else
	{
//    if (mandel_width < 0.00000001)
//	sprintf(info, "-c%15.15f,%15.15f,%-15.6g,%6.6f,%6.6f -t%d", hor, vert, mandel_width, pert_real, pert_imag, threshold);
//    else
//	sprintf(info, "-c%15.15f,%15.15f,%15.15f,%6.6f,%6.6f -t%d", hor, vert, mandel_width, pert_real, pert_imag, threshold);
	    sprintf(info, "-c%24.24f,%24.24f,%24.24f -t%d", hor, vert, mandel_width, threshold);
	}
    BasicFractData(info, FALSE);
    strcat(info, "\n");
    return info;
    }

/**************************************************************************
	Fractal data to string (used to build PNG file data)
**************************************************************************/

void	BasicFractData(char *info, BOOL CreateAnim)

    {
    char	s[1024];
    char	SubTypeName[1024];
    int		i, flags;

    sprintf(s, " -i%d,%d,%d,%d", TrueCol.inside_colour, TrueCol.InsideRed, TrueCol.InsideGreen, TrueCol.InsideBlue);
    strcat(info, s);
    if (calcmode == 'G')
	sprintf(s, " -MG%d", blockindex);	// for solid guessing
    else if (calcmode == 'T')
	sprintf(s, " -MT%ld", fillcolor);	// tesseral fill colour
    else if (calcmode == 'F')
	sprintf(s, " -MF%f,%d,%f,%f,%f", bump_transfer_factor, PaletteStart, lightDirectionDegrees, bumpMappingDepth, bumpMappingStrength);
    else
	sprintf(s, " -M%c", calcmode);
    strcat(info, s);

    if (NonStandardImage)			// not full screen view
	{
	sprintf(s, " -G%d,%d", width, height);
	strcat(info, s);
	}
    		
    if (InsideMethod)
	{
	sprintf(s, " -DI%03d", InsideMethod);	// display method
	strcat(info, s);
	if (OutsideMethod > TIERAZONFILTERS)
	    {
	    sprintf(s, ",%f,%d,%d", dStrands, nFDOption, UseCurrentPalette);	// Parameters for the Tierazon filters
	    strcat(info, s);
	    }
	else if (OutsideMethod == POTENTIAL)
	    {
	    sprintf(s, ",%f,%f,%f", potparam[0], potparam[1], potparam[2]);	// Parameters for potential
	    strcat(info, s);
	    }
	}
    if (OutsideMethod)
	{
	sprintf(s, " -DO%03d", OutsideMethod);	// display method
	strcat(info, s);
	}
    //    if (palette_flag && !CreateAnim)
//	{
//	sprintf(s, " -a\"%s\"", MAPFile);
//	strcat(info, s);
//	}
    if (juliaflag)
	{
	sprintf(s, " -J%13.13f,%13.13f", j.x, j.y);
	strcat(info, s);
	}
    switch(type)
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
	case MANDELBAR:
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
	    sprintf(s, " -f%03d", type);
	    break;					// default
	case NEWTON:
	case NEWTBASIN:					// Newton Fractal (basin or stripe)
	    sprintf(s, " -f%03d%03d%03d%03d", type, subtype, degree, special);
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
	    sprintf(s, " -f%03d%d", type, subtype);
	    break;					// default
	case PERTURBATION:
	    FindSubtypeName(SubTypeName, type, subtype);
	    sprintf(s, " -f%03d\"%s\",%d,%lf,%lf,%lf,%d", type, SubTypeName, SlopeType, lightDirectionDegrees, bumpMappingDepth, bumpMappingStrength, PaletteStart);
	    break;
	case FRACTALMAPS:
	case SPROTTMAPS:
	case SURFACES:
	case KNOTS:
	case CURVES:
	case OSCILLATORS:
	    flags = ((type == OSCILLATORS) ? DisplayLines : ((type == FRACTALMAPS || type == SPROTTMAPS) ? FractalMapColouring : RemoveHiddenPixels)) + (CoordSystem << 1) + BlockAnimation + FindCentre + PerspectiveFlag;
	    FindSubtypeName(SubTypeName, type, subtype);
	    sprintf(s, " -f%03d\"%s\",%d,%d,%d,%d,%d,%f,%f,%f,%f", type, SubTypeName, (xAxis+1)*xSign, (yAxis+1)*ySign, zAxis+1, flags, MaxDimensions, iterations, VertBias, dt, zBias);
	    break;
	case FOURIER:					// fourier;which type;harmonics;steps
	    sprintf(s, " -f%03d%03d%03d%04d%s", type, subtype, NumHarmonics, steps, WriteSliders());
	    break;
	case NEWTONVARIATION:				// Newton Variation fractals
	case NEWTONPOLYGON:				// Newton Polygon types
	case POWER:					// POWER types
	case BURNINGSHIPPOWER:				// Burning Ship of other powers besides quadratic
	    sprintf(s, " -f%03d%d", type, degree);
	    break;
	case LSYSTEM:					// lsys;which one in file;order;filename
	    sprintf(s, " -f%03d%03d%03d\"%s\"", type, lsys_ptr, degree, LSYSFile);
	    break;
	case CUBIC:					// CUBIC types
	case RATIONALMAP:				// Rational Maps
	case EXPFRACTAL:				// Exponential
	    sprintf(s, " -f%03d%03d%03d", type, subtype, special);
	    break;
	case SPECIALNEWT:
	case SINFRACTAL:				// sin() type fractal
	case MATEIN:					// MATEIN
	    sprintf(s, " -f%03d%d", type, special);
	    break;
	case FIBONACCI:					// Fibonacci Spirals
	    sprintf(s, " -f%03d%d", type, subtype);
	    break;
	case TIERAZON:					// Tierazon fractals
//	case MARCUSMANDEL:				// general Marcus fractal
	    sprintf(s, " -f%03d%03d", type, subtype);
	    break;
	case SCREENFORMULA:				// On Screen Formula fractals
	case FORMULA:					// formula files
	case FFORMULA:
	    sprintf(s, " -f%03d\"%s\"", type, FormulaString);
	    break;
	case IFS:					// IFS fractals
	    sprintf(s, " -f%03d%03d\"%s\"", type, lsys_ptr, IFSFile);
	    break;
	case LYAPUNOV:					// Lyapunov fractals
	    sprintf(s, " -f%03d -#%s", type, LyapSequence);
	    break;
	default:
	    sprintf(s, " -f%03d%d", type, subtype);
	}
    strcat(info, s);
    if (logval)
	{
	sprintf(s, " -l%d", logval);
	strcat(info, s);
	}
    sprintf(s, " -u%d,%d,%d,%d,%d,%d", TrueCol.RedStartInt, TrueCol.GreenStartInt, TrueCol.BlueStartInt, TrueCol.RedIncInt, TrueCol.GreenIncInt, TrueCol.BlueIncInt);
    strcat(info, s);
    if (pairflag)
	{
	sprintf(s, " -h%d", pairflag);
	strcat(info, s);
	}
    if (RotationAngle != NORMAL)
	{
	if (RotationAngle != 90 && RotationAngle != 180 && RotationAngle != 270)
	    sprintf(s, " -o%d,%lf,%lf", RotationAngle, RotationCentre.x, RotationCentre.y);
	else
	    sprintf(s, " -o%d", RotationAngle);
	strcat(info, s);
	}
    if (AutoStereo_value != 75)
	{
	sprintf(s, " -r%d", AutoStereo_value * stereo_sign);
	strcat(info, s);
	}
    if (PerspectiveFlag)
	{
	sprintf(s, " -2%lf,%lf,%lf,%lf,%lf,%lf", sclx, scly, sclz, x_rot, y_rot, z_rot);
	strcat(info, s);
	}
    if (_3dflag || ifs_type == 1)
	{
	sprintf(s, " -3%lf,%lf,%lf,%lf,%lf,%lf", sclx, scly, sclz, x_rot, y_rot, z_rot);
	strcat(info, s);
	}
    if (period_level != 1)
	{
	sprintf(s, " -p%d", period_level);
	strcat(info, s);
	}
    if (decomp > 0)
	{
	sprintf(s, " -k%d", decomp);
	strcat(info, s);
	}
    if (biomorph >= 0)
	{
	sprintf(s, " -b%d", biomorph);
	strcat(info, s);
	}
    if (distest > 0)
	{
	sprintf(s, " -$%d,%d", distest, distestwidth);
	strcat(info, s);
	}
    if ((CoordSystem != CARTESIAN))
	{
	sprintf(s, " -q%d", CoordSystem);
	strcat(info, s);
	}
    if (invert)
	{
	sprintf(s, " -x%f,%f,%f", f_radius, f_xcenter, f_ycenter);
	strcat(info, s);
	}
    if (StartColourCycling > 0)
	{
	sprintf(s, " -y%d", StartColourCycling);
	strcat(info, s);
	}
//    fprintf(fb, " %%1 %%2 %%3\n");
    if (Fractal.NumFunct > 0)
	{
	if (Fractal.NumFunct  == 1)
	    sprintf(s, " -n%s", Fractal.Fn1);
	else if (Fractal.NumFunct == 2)
	    sprintf(s, " -n%s,%s", Fractal.Fn1, Fractal.Fn2);
	strcat(info, s);
	}
    if (PertColourMethod != 0)		// Kalles colour method
	{
	sprintf(s, " -~%d", PertColourMethod);
	strcat(info, s);
	}
    if (PalOffset > 0)			// begin palette here
	{
	sprintf(s, " -^%d", PalOffset);
	strcat(info, s);
	}
    if (IterDiv != 1.0)			// divide ieration by this amount
	{
	sprintf(s, " -@%lf", IterDiv);
	strcat(info, s);
	}

    if (type == OSCILLATORS ||type == FRACTALMAPS || type == SPROTTMAPS ||type == SURFACES || type == KNOTS || type == CURVES)	// these have a different database
	{
	if (NumVariables > 0)						// variables are stored in param[10] to param[19]
	    {
	    sprintf(s, " -w");
	    strcat(info, s);
	    for (i = 0; i < NumVariables + 10 - 1; i++)
		{
		sprintf(s, "%g,", param[i]);
		strcat(info, s);
		}
	    sprintf(s, "%g", param[i]);					// last one without the ','
	    strcat(info, s);
	    }
	}
    else
	{
	if (Fractal.NumParam > 0)
	    {
	    sprintf(s, " -w");
	    strcat(info, s);
	    for (i = 0; i < Fractal.NumParam - 1; i++)
		{
		sprintf(s, "%g,", param[i]);
		strcat(info, s);
		}
	    sprintf(s, "%g", param[i]);			// last one without the ','
	    strcat(info, s);
	    }
	}
    sprintf(s, " -v%lf,%d", *Fractal.rqlim, BailoutTestType);
    strcat(info, s);
    }

/**************************************************************************
	Fractal palette to string
**************************************************************************/

void	SaveTextPalette(char *PaletteBuffer)

    {
    int		i;
    char	*p;
    char	ascii[6];
    long	LocalThreshold;

    LocalThreshold = (threshold >= MAXTHRESHOLD) ? MAXTHRESHOLD - 1 : threshold;

    p = PaletteBuffer;
    for (i = 0; i < LocalThreshold; i++)
	{
	ConvertRGB2ASCII(*(TrueCol.PalettePtr + i * 3 + 0), *(TrueCol.PalettePtr + i * 3 + 1), *(TrueCol.PalettePtr + i * 3 + 2), ascii);
	strncpy(p, ascii, 4);
	p += 4;
	}
    *p = '\0';				// remember to terminate string so strlen() knows where to end
    }

/**************************************************************************
	String to Fractal palette
**************************************************************************/

void	LoadTextPalette(char *PaletteBuffer, int length)

    {
    int		i, j, count;
    char	*p;
    char	ascii[6];
    char	ch;
    BOOL	EndOfData = FALSE;
    long	LocalThreshold;

    LocalThreshold = (threshold >= MAXTHRESHOLD) ? MAXTHRESHOLD - 1 : threshold;

    p = PaletteBuffer;
    count = 0;
    for (i = 0; i < LocalThreshold; i++)
	{
	for (j = 0; j < 4; j++)
	    {
	    ch = *p++;
	    if (count++ > length)
		{
		EndOfData = TRUE;
		break;
		}
	    ascii[j] = ch;
	    }
	ascii[4] = '\0';
	if (EndOfData)
	    break;
	ConvertASCII2RGB((TrueCol.PalettePtr + i * 3 + 0), (TrueCol.PalettePtr + i * 3 + 1), (TrueCol.PalettePtr + i * 3 + 2), ascii);
	}
    }

