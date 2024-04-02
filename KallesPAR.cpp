/*
    KALLESPAR.CPP a module to import KFR files from Kalles Fraktaler.
    
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

#define	INSIDE	TRUE
#define	OUTSIDE	FALSE

extern	double	hor;			// horizontal address 
extern	double	vert;			// vertical address 
extern	double	mandel_width;		// width of display 
extern	double	ScreenRatio;		// ratio of width / height for the screen
extern	double	rqlim;			// bailout level
extern	long	threshold;		// maximum iterations 
extern	WORD	type;			// M=mand, N=Newton etc 
extern	CTrueCol    TrueCol;		// palette info
extern	char	MAPFile[];		// colour map file
extern	BYTE	juliaflag;		// Julia implementation of fractal 

extern	WORD	degree;			// special colour, phase etc 
extern	int	subtype;		// B=basin, S=stripe, N=normal 
extern	struct	FractintFilterStuff	FractintFilter[];	// default values for each 
extern	double	IterDiv;		// divide ieration by this amount
extern	double	RotationAngle;		// rotate image in degrees
extern	int	SlopeType;
extern	double	lightDirectionDegrees;
extern	double	bumpMappingDepth;
extern	double	bumpMappingStrength;
extern	int	PalOffset;		// begin palette here
extern	int	PertColourMethod;	// some ideas from Kalles

// Big num declarations **********************************************************
extern	BYTE	BigNumFlag;		// True if bignum used
extern	int	decimals, precision;
extern	BigDouble	BigBailout, BigHor, BigVert, BigWidth;
// Big num declarations **********************************************************

extern	void	InitFract(int);
extern	int	getprecbf_mag(void);
extern	void	ConvertString2Bignum(mpfr_t, char *);
extern	char	*trailing(char *instr);
extern	int	Strlicmp(const char *, const char *);
extern	char	*strtok1(register char *, register const char *);	// required to use different
extern	void	ConvertBignum2String(char *s, mpfr_t num);

struct FractintFilterStuff				// database of Fractint Outside filters
    {
    char    *name;				// name of the fractal 
    char    method;				// only allow '1', '2', blinds or spiral 
    };

/**************************************************************************
	Find the fractal type and convert from Kalles format to ManpWIN format
**************************************************************************/

int	FindKallesType(int power, int FractType)

    {
    juliaflag = FALSE;
    type = PERTURBATION;
    rqlim = 1000;

    switch (FractType)
	{
	case 0:
	    if (power == 2)
		subtype = 0;			// Mandelbrot
	    else
		{
		param[2] = (double)power;	// power
		degree = power;
		subtype = 1;
		}
	    break;
	case 1:
	    if (power == 2)
		subtype = 2;			// Burning Ship
	    else if (power == 3)
		subtype = 3;			// cubic Burning Ship
	    else if (power == 4)
		subtype = 4;			// quartic Burning Ship
	    else if (power == 5)
		subtype = 5;			// quintic Burning Ship
	    break;
	case 2:
	    if (power == 2)
		subtype = 12;			// Buffalo
	    else if (power == 3)
		subtype = 13;			// cubic Buffalo
	    else if (power == 4)
		subtype = 14;			// quartic Buffalo
	    else if (power == 5)
		subtype = 15;			// quintic Buffalo
	    break;
	case 3:
	    if (power == 2)
		subtype = 6;			// celtic
	    else if (power == 3)
		subtype = 7;			// cubic celtic
	    else if (power == 4)
		subtype = 8;			// quartic celtic
	    else if (power == 5)
		subtype = 9;			// quintic celtic
	    break;
	case 4:
	    if (power == 2)
		subtype = 10;			// Mandelbar (Tricorn)
	    else
		{
		param[1] = (double)power;	// Mandelbar power
		degree = power;
		subtype = 11;
		}
	    break;
	case 5:
	    subtype = 16;			// Mandelbar Celtic
	    break;
	case 6:
	    subtype = 17;			// Perpendicular Mandelbrot
	    break;
	case 7:
	    subtype = 18;			// Perpendicular Burning Ship
	    break;
	case 8:
	    subtype = 19;			// Perpendicular Celtic
	    break;
	case 9:
	    subtype = 20;			// Perpendicular Buffalo
	    break;
	case 10:
	    subtype = 21;			// Cubic Quasi Burning Ship
	    break;
	case 11:
	    subtype = 22;			// Cubic Partial Burning Ship Real
	    break;
	case 12:
	    subtype = 23;			// Cubic Partial Burning Ship Imag
	    break;
	case 13:
	    subtype = 24;			// Cubic Flying Squirrel
	    break;
	case 14:
	    subtype = 25;			// Cubic Quasi Perpendicular
	    break;
	case 15:
	    subtype = 26;			// 4th Burning Ship Partial Imag
	    break;
	case 16:
	    subtype = 27;			// 4th Burning Ship Partial Real
	    break;
	case 17:
	    subtype = 28;			// 4th Burning Ship Partial Real Mandelbar
	    break;
	case 18:
	    subtype = 29;			// 4th Celtic Burning Ship Partial Imag
	    break;
	case 19:
	    subtype = 30;			// 4th Celtic Burning Ship Partial Real
	    break;
	case 20:
	    subtype = 31;			// 4th Celtic Burning Ship Partial Real Mandelbar
	    break;
	case 21:
	    subtype = 32;			// 4th Buffalo Partial Imag
	    break;
	case 22:
	    subtype = 33;			// 4th Celtic Mandelbar
	    break;
	case 23:
	    subtype = 34;			// 4th False Quasi Perpendicular
	    break;
	case 24:
	    subtype = 35;			// 4th False Quasi Heart
	    break;
	case 25:
	    subtype = 36;			// 4th Celtic False Quasi Perpendicular
	    break;
	case 26:
	    subtype = 37;			// 4th Celtic False Quasi Heart
	    break;
	case 27:
	    subtype = 44;			// 5th Burning Ship Partial
	    break;
	case 28:
	    subtype = 45;			// 5th Burning Ship Partial Mandelbar
	    break;
	case 29:
	    subtype = 46;			// 5th Celtic Mandelbar
	    break;
	case 30:
	    subtype = 47;			// 5th Quasi Burning Ship (BS/Buffalo Hybrid)
	    break;
	case 31:
	    subtype = 48;			// 5th Quasi Perpendicular
	    break;
	case 32:
	    subtype = 49;			// 5th Quasi Heart
	    break;
	case 33:
	    subtype = 50;			// FT_Simon100A_plain
	    break;
	case 34:
	    subtype = 38;			// 4th Imag Quasi Perpendicular / Heart
	    break;
	case 35:
	    subtype = 39;			// 4th Real Quasi Perpendicular
	    break;
	case 36:
	    subtype = 40;			// 4th Real Quasi Heart
	    break;
	case 37:
	    subtype = 41;			// 4th Celtic Imag Quasi Perpendicular / Heart
	    break;
	case 38:
	    subtype = 42;			// 4th Celtic Real Quasi Perpendicular
	    break;
	case 39:
	    subtype = 43;			// 4th Celtic Real Quasi Heart
	    break;
	case 40:
	    subtype = 51;			// Cubic FT_Simon100A_plain
	    break;
	case 41:				// HPDZ Buffalo
	    subtype = 58;
	    break;
	case 42:
	    subtype = 53;			// TheRedshiftRider 1
	    param[1] = 1.0;
	    param[2] = 0.0;
	    param[3] = 3.0;
	    param[4] = 1.0;
	    break;
	case 43:
	    subtype = 53;			// TheRedshiftRider 2
	    param[1] = 1.0;
	    param[2] = 0.0;
	    param[3] = 3.0;
	    param[4] = 0.0;
	    break;
	case 44:
	    subtype = 53;			// TheRedshiftRider 3
	    param[1] = 2.0;
	    param[2] = 0.0;
	    param[3] = 3.0;
	    param[4] = 1.0;
	    break;
	case 45:
	    subtype = 53;			// TheRedshiftRider 4
	    param[1] = 1.0;
	    param[2] = 0.0;
	    param[3] = 4.0;
	    param[4] = 1.0;
	    break;
	case 46:
	    subtype = 53;			// TheRedshiftRider 5
	    param[1] = 1.0;
	    param[2] = 0.0;
	    param[3] = 4.0;
	    param[4] = 0.0;
	    break;
	case 47:
	    subtype = 53;			// TheRedshiftRider 6
	    param[1] = 1.0;
	    param[2] = 0.0;
	    param[3] = 5.0;
	    param[4] = 1.0;
	    break;
	case 48:
	    subtype = 53;			// TheRedshiftRider 7
	    param[1] = 1.0;
	    param[2] = 0.0;
	    param[3] = 5.0;
	    param[4] = 0.0;
	    break;
	case 49:
	    subtype = 53;			// TheRedshiftRider 8
	    param[1] = 1.0;
	    param[2] = 0.0;
	    param[3] = 6.0;
	    param[4] = 1.0;
	    break;
	case 50:
	    subtype = 53;			// TheRedshiftRider 9
	    param[1] = 1.0;
	    param[2] = 0.0;
	    param[3] = 6.0;
	    param[4] = 0.0;
	    break;
	case 51:
	    subtype = 52;			// SimonBrot2 4th
	    break;
	case 52:				// Talis: Zn+1 = ((Zn ^ 2) / (1 + Zn)) + C;
	    subtype = 54;
	    break;
	case 53:				// Talis Cubic: Zn+1 = ((Zn ^ 3) / (1 + Zn^2)) + C;
	    subtype = 55;
	    break;
	case 54:				// Talis Quartic: Zn+1 = ((Zn ^ 3) / (1 + Zn^2)) + C;
	    subtype = 56;
	    break;
	case 55:				// Polynomial
	    subtype = 57;
	    break;
	case 58:				// HPDZ Buffalo	
	    subtype = 41;
	    break;

	default:
	    subtype = 0;
	}
    return 0;
    }

/**************************************************************************
	Convert ManpWIN format to Kalles format
**************************************************************************/

int	ConvertToKallesType(int subtype, WORD degree, int *power, int *FractalType)

    {
    juliaflag = FALSE;
    type = PERTURBATION;
    rqlim = 1000;

    switch (subtype)
	{
	case 0:					// Mandelbrot
	    *FractalType = 0;
	    *power = 2;
	    break;
	case 1:					// power
	    *FractalType = 0;
	    *power = degree;
	    break;
	case 2:					// Burning Ship
	    *FractalType = 1;
	    *power = 2;
	    break;
	case 3:					// cubic Burning Ship
	case 4:					// quartic Burning Ship
	case 5:					// quintic Burning Ship
	    *FractalType = 1;
	    *power = degree;
	    break;
	case 6:					// celtic
	    *FractalType = 3;
	    *power = 2;
	    break;
	case 7:					// cubic celtic
	case 8:					// quartic celtic
	case 9:					// quintic celtic
	    *FractalType = 3;
	    *power = degree;
	    break;
	case 10:				// Mandelbar (Tricorn)
	    *FractalType = 4;
	    *power = 2;
	    break;
	case 11:				// Mandelbar Power
	    *FractalType = 4;
	    *power = degree;
	    break;
	case 12:				// Buffalo
	    *FractalType = 2;
	    *power = 2;
	    break;
	case 13:				// cubic Buffalo
	case 14:				// quartic Buffalo
	case 15:				// quintic Burning Ship
	    *FractalType = 2;
	    *power = degree;
	    break;
	case 16:				// Mandelbar Celtic
	    *FractalType = 5;
	    break;
	case 17:				// Perpendicular Mandelbrot
	    *FractalType = 6;
	    break;
	case 18:				// Perpendicular Burning Ship
	    *FractalType = 7;
	    break;
	case 19:				// Perpendicular Celtic
	    *FractalType = 8;
	    break;
	case 20:				// Perpendicular Buffalo
	    *FractalType = 9;
	    break;
	case 21:				// Cubic Quasi Burning Ship
	    *FractalType = 10;
	    break;
	case 22:				// Cubic Partial Burning Ship Real
	    *FractalType = 11;
	    break;
	case 23:				// Cubic Partial Burning Ship Imag
	    *FractalType = 12;
	    break;
	case 24:				// Cubic Flying Squirrel
	    *FractalType = 13;
	    break;
	case 25:				// Cubic Quasi Perpendicular
	    *FractalType = 14;
	    break;
	case 26:				// 4th Burning Ship Partial Imag
	    *FractalType = 15;
	    break;
	case 27:				// 4th Burning Ship Partial Real
	    *FractalType = 16;
	    break;
	case 28:				// 4th Burning Ship Partial Real Mandelbar
	    *FractalType = 17;
	    break;
	case 29:				// 4th Celtic Burning Ship Partial Imag
	    *FractalType = 18;
	    break;
	case 30:				// 4th Celtic Burning Ship Partial Real
	    *FractalType = 19;
	    break;
	case 31:				// 4th Celtic Burning Ship Partial Real Mandelbar
	    *FractalType = 20;
	    break;
	case 32:				// 4th Buffalo Partial Imag
	    *FractalType = 21;
	    break;
	case 33:				// 4th Celtic Mandelbar
	    *FractalType = 22;
	    break;
	case 34:				// 4th False Quasi Perpendicular
	    *FractalType = 23;
	    break;
	case 35:				// 4th False Quasi Heart
	    *FractalType = 24;
	    break;
	case 36:				// 4th Celtic False Quasi Perpendicular
	    *FractalType = 25;
	    break;
	case 37:				// 4th Celtic False Quasi Heart
	    *FractalType = 26;
	    break;
	case 38:				// 4th Imag Quasi Perpendicular / Heart
	    *FractalType = 34;
	    break;
	case 39:				// 4th Real Quasi Perpendicular
	    *FractalType = 35;
	    break;
	case 40:				// 4th Real Quasi Heart
	    *FractalType = 36;
	    break;
	case 41:				// 4th Celtic Imag Quasi Perpendicular / Heart
	    *FractalType = 37;
	    break;
	case 42:				// 4th Celtic Real Quasi Perpendicular
	    *FractalType = 38;
	    break;
	case 43:				// 4th Celtic Real Quasi Heart
	    *FractalType = 39;
	    break;
	case 44:				// 5th Burning Ship Partial
	    *FractalType = 27;
	    break;
	case 45:				// 5th Burning Ship Partial Mandelbar
	    *FractalType = 28;
	    break;
	case 46:				// 5th Celtic Mandelbar
	    *FractalType = 29;
	    break;
	case 47:				// 5th Quasi Burning Ship (BS/Buffalo Hybrid)
	    *FractalType = 30;
	    break;
	case 48:				// 5th Quasi Perpendicular
	    *FractalType = 31;
	    break;
	case 49:				// 5th Quasi Heart
	    *FractalType = 32;
	    break;
	case 50:				// FT_Simon100A_plain
	    *FractalType = 33;
	    break;
	case 51:				// Cubic FT_Simon100A_plain
	    *FractalType = 40;
	    break;
	case 52:				// SimonBrot2 4th
	    *FractalType = 51;
	    break;
	case 53:				// TheRedshiftRider
	    if (param[1] == 1.0)
		{
		if (param[3] == 3.0 && param[4] == 1.0)
		    *FractalType = 42;
		if (param[3] == 3.0 && param[4] == 0.0)
		    *FractalType = 43;
		if (param[3] == 4.0 && param[4] == 1.0)
		    *FractalType = 45;
		if (param[3] == 4.0 && param[4] == 0.0)
		    *FractalType = 46;
		if (param[3] == 5.0 && param[4] == 1.0)
		    *FractalType = 47;
		if (param[3] == 5.0 && param[4] == 0.0)
		    *FractalType = 48;
		if (param[3] == 6.0 && param[4] == 1.0)
		    *FractalType = 49;
		if (param[3] == 6.0 && param[4] == 0.0)
		    *FractalType = 50;
		}
	    else if (param[1] == 2.0)
		*FractalType = 44;
	    else
		*FractalType = 42;
	    break;
	case 54:				// Talis: Zn+1 = ((Zn ^ 2) / (1 + Zn)) + C;
	    *FractalType = 52;
	    break;
	case 55:				// Talis Cubic: Zn+1 = ((Zn ^ 3) / (1 + Zn^2)) + C;
	    *FractalType = 53;
	    break;
	case 56:				// Talis Quartic: Zn+1 = ((Zn ^ 3) / (1 + Zn^2)) + C;
	    *FractalType = 54;
	    break;
	case 57:				// Polynomial
	    *FractalType = 55;
	    break;
	case 58:				// HPDZ Buffalo	
	    *FractalType = 41;
	    break;

	default:				// just do a Mandelbrot
	    *FractalType = 0;
	    *power = 2;
	}
    return 0;
    }

/**************************************************************************
	Load Palette Map
**************************************************************************/

static int ProcessColours(char *value)

    {
    int		i;
    int		PalSize;
    char	*tok;
    char	seps[] = ",";
    BYTE	swap;

    tok = strtok1(value, seps);
    i = 0;
    while (tok)
	{
	*(TrueCol.PalettePtr + i) = (BYTE)(atoi(tok)); 
	tok = strtok1(NULL, seps);
	i++;
	}
    PalSize = i / 3;
    TrueCol.ColoursInPALFile = PalSize;
    for (i = 0; i < PalSize; i++)		// better swap colours to be "accurate"
	{
	swap = *(TrueCol.PalettePtr + i * 3);
	*(TrueCol.PalettePtr + i * 3) = *(TrueCol.PalettePtr + i * 3 + 2);
	*(TrueCol.PalettePtr + i * 3 + 2) = swap;
	}

    TrueCol.FillPalette(REPEAT, TrueCol.PalettePtr, threshold);
    return(0);
    }

/**************************************************************************
	Process Corners
**************************************************************************/

int	ProcessLocation(char *real, char *imag, char *zoom)

    {
    double	Magnification;
    BigDouble   BigMag, OneOverMag;

    BigNumFlag = FALSE;
    sscanf(real, "%lf", &hor);
    sscanf(imag, "%lf", &vert);
    sscanf(zoom, "%lf", &Magnification);

    if (Magnification < 0.0)
	Magnification = -Magnification;

    if (Magnification > 1.0E13)			// we can do a BigNum calculation here to allow deeper zooming
	{
	ConvertString2Bignum(BigMag.x, zoom);
	OneOverMag = BigMag.BigInvert();
	BigWidth = OneOverMag + OneOverMag;
	BigNumFlag = TRUE;
	}
    else
	{
	mandel_width = 4.0 / Magnification;
	vert = -(vert + mandel_width / 2.0);
	hor -= mandel_width * ScreenRatio / 2.0;
	}

    precision = getprecbf_mag();
    if (BigNumFlag)
	{
	decimals = precision + PRECISION_FACTOR;	// for deep zooming, we'd better have enough precision
	BigMag.ChangePrecision(precision);
	BigHor.ChangePrecision(precision);
	BigVert.ChangePrecision(precision);
	BigWidth.ChangePrecision(precision);
	OneOverMag.ChangePrecision(precision);

	BigBailout = rqlim;
	ConvertString2Bignum(BigHor.x, real);
	ConvertString2Bignum(BigVert.x, imag);
	ConvertString2Bignum(BigMag.x, zoom);
	if (mpfr_sgn(BigMag.x) == 0)	// no naughty division
	    return -1;
	OneOverMag = BigMag.BigInvert();
	BigWidth = OneOverMag * 4.0;
	BigVert = -BigVert - BigWidth.BigHalf();
	BigHor = BigHor - BigWidth * ScreenRatio / 2.0;
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
	Write the Kalles par file
**************************************************************************/

int	WriteKallesFile(HWND hwnd, char *filename)
    {
    double	Zreal, Zimag, zoom;
    BigDouble   BigMag, OneOverMag, BigZreal, BigZimag;
    int		FractalType, power, Slopes;
    FILE	*fp;
    char	s[200];
    char	*s1, *s2, *s3;

    if (type == MANDELFP)
	{
	FractalType = 0;
	power = 2;
	}
    else if (type == POWER)
	{
	FractalType = 0;
	power = degree;
	}
    else if (type == PERTURBATION)
	ConvertToKallesType(subtype, degree, &power, &FractalType);
    else
	{
	wsprintf(s, "Can only write Kalles File if type = Perturbation. Type = <%d>", type);
	MessageBox(hwnd, s, "MANPWIN", MB_ICONEXCLAMATION | MB_OK);
	return -1;
	}

    Slopes = (SlopeType == NOSLOPE) ? 0 : 1;

    if ((fp = fopen(filename, "w")) == NULL)
	{
	wsprintf(s, "Can't Open Kalles File: <%s>", filename);
	MessageBox(hwnd, s, "MANPWIN", MB_ICONEXCLAMATION | MB_OK);
	return -1;
	}

    if (BigNumFlag)
	{
	s1 = new char[SIZEOF_BF_VARS];
	s2 = new char[SIZEOF_BF_VARS];
	s3 = new char[SIZEOF_BF_VARS];

	BigZreal = BigHor + (BigWidth * ScreenRatio) / 2;
	BigZimag = -BigVert - BigWidth / 2.0;
	OneOverMag = BigWidth.BigInvert();
	BigMag = OneOverMag * 4.0;
	ConvertBignum2String(s1, BigZreal.x);
	ConvertBignum2String(s2, BigZimag.x);
	ConvertBignum2String(s3, BigMag.x);
	fprintf(fp, "Re: %s\nIm: %s\nZoom: %s\n", s1, s2, s3);

	if (s1) { delete[] s1; s1 = NULL; }
	if (s2) { delete[] s2; s2 = NULL; }
	if (s3) { delete[] s3; s3 = NULL; }
	}
    else
	{
	Zreal = hor + (mandel_width * ScreenRatio) / 2;
	Zimag = -vert - mandel_width / 2.0;
	zoom = 4.0 / mandel_width;
	fprintf(fp, "Re: %lf\nIm: %lf\nZoom: %lf\n", Zreal, Zimag, zoom);
	}

    fprintf(fp, "Iterations: %ld\nFractalType: %d\nPower: %d\n", threshold, FractalType, power);
    fprintf(fp, "Colors: ");
    for (int i = 0; i < TrueCol.ColoursInPALFile; i++)
	fprintf(fp, "%d, %d, %d, ", *(TrueCol.PalettePtr + i * 3 + 2), *(TrueCol.PalettePtr + i * 3 + 1), *(TrueCol.PalettePtr + i * 3));
    fprintf(fp, "\n");
    if (Slopes)
	fprintf(fp, "Slopes: %d\n", Slopes);
    fprintf(fp, "Smooth: %d\n", (PertColourMethod == 4) ? 1 : 0);
    if (PalOffset)
	fprintf(fp, "ColorOffset: %d\n", PalOffset);
    if (IterDiv != 1.0)
	fprintf(fp, "IterDiv: %lf\n", IterDiv);

    fclose(fp);
    return 0;
    }

#define BUFFERSIZE  20000			// big enough for palette 1024 * 4 * 3 and a bit left over

/**************************************************************************
	Read the Kalles par file
**************************************************************************/

int	ReadKallesFile(HWND hwnd, char *filename)
    {
    int		i;
    double	SeedR = 0.0, SeedI = 0.0;
    BigDouble	BigxCentre, BigyCentre, BigZoom;
    int		linenum = 0, length;
    int		TempType = 0, Power = 2;
    char	RealStr[BUFFERSIZE], ImagStr[BUFFERSIZE], ZoomStr[BUFFERSIZE];		// hold corner and zoom
    char	buffer[BUFFERSIZE];
    FILE	*fp;
    char	s[200];
    
    SlopeType = 0;
    IterDiv = 1.0;
    RotationAngle = 0.0;
    if ((fp = fopen(filename, "r")) == NULL)
	{
	wsprintf(s, "Can't Open Kalles File: <%s>", filename);
	MessageBox (hwnd, s, "MANPWIN", MB_ICONEXCLAMATION | MB_OK);
	return -1;
	}

     while(fgets(buffer, BUFFERSIZE, fp))		// Max line length BUFFERSIZE chars
	{
	linenum++;
	if ((i = (int)strlen(buffer)) > BUFFERSIZE - 2)
	    {
	    wsprintf(s, "Par Data full line: <%d>", linenum);
	    MessageBox (hwnd, s, "ManpWIN", MB_ICONEXCLAMATION | MB_OK);
	    break;
	    }

	if (length = Strlicmp(buffer, "Re: "))
	    {
	    strcpy(RealStr, buffer + length);
	    }
	if (length = Strlicmp(buffer, "Im: "))
	    {
	    strcpy(ImagStr, buffer + length);
	    }
	if (length = Strlicmp(buffer, "Zoom: "))
	    {
	    strcpy(ZoomStr, buffer + length);
	    }
	if (length = Strlicmp(buffer, "Iterations: "))
	    {
	    threshold = atol(buffer + length);
	    if (threshold < 0L)
		threshold = MAXTHRESHOLD;
	    }
	if (length = Strlicmp(buffer, "IterDiv: "))
	    {
	    strcpy(s, buffer + length);
	    sscanf(s, "%lf", &IterDiv);
	    if (IterDiv <= 0.0)
		IterDiv = 1.0;
	    }
	if (length = Strlicmp(buffer, "Rotate: "))
	    {
	    strcpy(s, buffer + length);
	    sscanf(s, "%lf", &RotationAngle);
	    }
	if (length = Strlicmp(buffer, "MultiColors: "))
	    {
	    }
	if (length = Strlicmp(buffer, "Colors: "))
	    ProcessColours(buffer + length);
	if (length = Strlicmp(buffer, "Slopes: "))
	    SlopeType = atoi(buffer + length);			// slope = 0 no slope, slope = 1 FWDGIFF
	if (length = Strlicmp(buffer, "SlopePower: "))
	    bumpMappingDepth = atoi(buffer + length);
	if (length = Strlicmp(buffer, "SlopeRatio: "))
	    bumpMappingStrength = atoi(buffer + length);
	if (length = Strlicmp(buffer, "SlopeAngle: "))
	    lightDirectionDegrees = atoi(buffer + length);

	if (length = Strlicmp(buffer, "SeedR: "))
	    {
	    strcpy(s, buffer + length);
	    sscanf(s, "%lf", &SeedR);
	    }
	if (length = Strlicmp(buffer, "SeedI: "))
	    {
	    strcpy(s, buffer + length);
	    sscanf(s, "%lf", &SeedI);
	    }
	if (length = Strlicmp(buffer, "Power: "))
	    Power = atoi(buffer + length);
	if (length = Strlicmp(buffer, "ColorOffset: "))
	    PalOffset = atoi(buffer + length);
//	if (length = Strlicmp(buffer, "IterDiv: "))
//	    IterDiv = atoi(buffer + length);
	if (length = Strlicmp(buffer, "Smooth: "))
	    {
	    int i = atoi(buffer + length);
	    PertColourMethod = (i == 0) ? 0 : 4;
	    }
	if (length = Strlicmp(buffer, "FractalType: "))		// must be the only one which is after strlwr
	    TempType = atoi(buffer + length);
	}
    ProcessLocation(RealStr, ImagStr, ZoomStr);
    FindKallesType(Power, TempType);
//    if (TempType == 0)						// mandelbrot (refer fractalp.cpp)
//	{
//	param[2] = SeedR;
//	param[3] = SeedI;
//	}
//    else if (TempType == 1)					// power
//	{
//	param[3] = SeedR;
//	param[4] = SeedI;
//	}
    fclose(fp);
    return 0;
    }


