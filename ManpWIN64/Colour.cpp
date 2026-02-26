//////////////////////////////////////////////////////////////////////////////////////////
//
// COLOUR.CPP a module to edit true colour palettes. implementation of the CTrueCol class.
//
//////////////////////////////////////////////////////////////////////////////////////////
// Author:-
//	Paul de Leeuw
//	pdeleeuw at deleeuw dot com dot au	( replace "at" "dot" by the normal characters.)
//	10/07/2007
//	This Class includes code to create a true palette and to modify it
//////////////////////////////////////////////////////////////////////////////////////////

//#include <wx/aboutdlg.h>
#include <stdio.h>
#include <windows.h>
#include <windowsx.h>
#include <time.h>
#include <math.h>
#include "colour.h"
#include "manp.h"

extern	size_t	colourCount;
extern	BYTE	default_palette[];

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTrueCol::CTrueCol()			// device independent bitmap
    {
    RandomDivisor = 128;
    RandomColourFlag = FALSE;
    Stretch = FALSE;
    DisplayPaletteFlag = TRUE;
    ScriptPaletteFlag = FALSE;
    PalEditFlag = FALSE;
    RedStartInt = 60;
    GreenStartInt = 120;
    BlueStartInt = 30;
    RedIncInt = 100;
    GreenIncInt = 255;
    BlueIncInt = 200;
    InsideRed = 50;				// values for r, g, b channels for inside colour
    InsideGreen = 50;
    InsideBlue = 50;
    ColoursInPALFile = 256;			// assume minimum palette size of 256
    IsMAPFile = false;				// have we loaded a MAP file?
    PalettePtr.resize(MAXPALETTE); 
    DefaultPalettePtr.resize(MAXPALETTE);
    }

CTrueCol::~CTrueCol()
    {
//    if (PalettePtr) { delete[] PalettePtr; PalettePtr = NULL; }
    }

//////////////////////////////////////////////////////////////////////
// Global declaration
//////////////////////////////////////////////////////////////////////

CTrueCol    TrueCol;

//////////////////////////////////////////////////////////////////////
//	Initialise True Colour Palette
//////////////////////////////////////////////////////////////////////

/*
void CTrueCol::LoadDefaultPalette(BYTE default_palette[])
    {
    const int nBytes = sizeof(default_palette);
    const int nCols = nBytes / 3;

    DefaultPalettePtr.clear();
    DefaultPalettePtr.reserve(nCols);

    for (int i = 0; i < nCols; ++i)
	{
	RGBTRIPLE c;
	c.rgbtBlue = default_palette[i * 3 + 0];
	c.rgbtGreen = default_palette[i * 3 + 1];
	c.rgbtRed = default_palette[i * 3 + 2];
	DefaultPalettePtr.push_back(c);
	}
    }
*/

void	CTrueCol::InitTrueColPal(BYTE RandFlag, long threshold, int StartColourCycling, int logval, int bits_per_pixel, BOOL UseFractintPalette, BYTE default_palette[])

    {
    long	i, cycle, temp;
    double	RedStart, GreenStart, BlueStart, RedInc, GreenInc, BlueInc;
    float	size;
    static	WORD	Randomise;

    ColoursInPALFile = (long)colourCount;
    if (PalEditFlag)
	{
	if (Stretch)
	    FillPalette(STRETCH, PalettePtr, threshold);
	else
	    FillPalette(REPEAT, PalettePtr, threshold);
	return;
	}

    for (size_t i = 0; i < ColoursInPALFile; ++i)
	{
	DefaultPalettePtr[i].rgbtRed = default_palette[3 * i + 2];
	DefaultPalettePtr[i].rgbtGreen = default_palette[3 * i + 1];
	DefaultPalettePtr[i].rgbtBlue = default_palette[3 * i + 0];
	}

    if (UseFractintPalette)
	{
	for (int i = 0; i < ColoursInPALFile; i++)
	    PalettePtr[i] = DefaultPalettePtr[i];
//	LoadDefaultPalette(default_palette);
	FillPalette(REPEAT, PalettePtr, threshold);
	}
    else
	{
	if (RandFlag)
	    {
	    srand((unsigned)time(NULL) + Randomise);// Seed the random-number generator with current time so
						// that the numbers will be different every time we run.

	    if (RandomColourFlag)
		{
		TrueCol.RedIncInt = rand() / RandomDivisor;
		TrueCol.GreenIncInt = rand() / RandomDivisor;
		TrueCol.BlueIncInt = rand() / RandomDivisor;
		TrueCol.RedStartInt = rand() / RandomDivisor;
		TrueCol.GreenStartInt = rand() / RandomDivisor;
		TrueCol.BlueStartInt = rand() / RandomDivisor;
		}

	    Randomise = rand();			// to prevent the same value within the second!!
	    }

	RedStart = (float)(TrueCol.RedStartInt) / 100.0;
	GreenStart = (float)(TrueCol.GreenStartInt) / 100.0;
	BlueStart = (float)(TrueCol.BlueStartInt) / 100.0;
	RedInc = (float)(TrueCol.RedIncInt) / 100.0;
	GreenInc = (float)(TrueCol.GreenIncInt) / 100.0;
	BlueInc = (float)(TrueCol.BlueIncInt) / 100.0;

	LocalThreshold = (threshold >= MAXPALETTE) ? MAXPALETTE - 1 : threshold;
	temp = ((long)StartColourCycling > LocalThreshold) ? LocalThreshold : StartColourCycling;
	for (i = 1L; i < LocalThreshold; ++i)
	    {
	    cycle = temp + i;
	    if (cycle > LocalThreshold)
		cycle -= LocalThreshold;
	    size = (float)cycle / (float)((logval != 0) ? 256 : LocalThreshold);
	    PalettePtr[i].rgbtRed = (BYTE)(127.0 * sin(TWO_PI * (size + RedStart) * RedInc)) + 128;
	    PalettePtr[i].rgbtGreen = (BYTE)(127.0 * sin(TWO_PI * (size + GreenStart) * GreenInc)) + 128;
	    PalettePtr[i].rgbtBlue = (BYTE)(127.0 * sin(TWO_PI * (size + BlueStart) * BlueInc)) + 128;
	    }

	PalettePtr[LocalThreshold].rgbtBlue = (BYTE)InsideBlue;
	PalettePtr[LocalThreshold].rgbtGreen = (BYTE)InsideGreen;
	PalettePtr[LocalThreshold].rgbtRed = (BYTE)InsideRed;
	// set up pointer to correct palette
//    PalettePtr = TRUE_PALETTE;
	bits_per_pixel = 24;
	}
    }

/**************************************************************************
	Fill palette values for iteration greater than 256
**************************************************************************/

void	CTrueCol::FillPalette(int FillType, std::vector<RGBTRIPLE> &pal, long threshold)

    {
    long	i, j;

    LocalThreshold = (threshold >= MAXPALETTE) ? MAXPALETTE - 1 : threshold;
    if (FillType == REPEAT)
	{
	if (LocalThreshold > ColoursInPALFile)
	    {
	    for (i = ColoursInPALFile; i < LocalThreshold; i++)
		{
		pal[i] = pal[i % ColoursInPALFile];
		}
	    }
	}
    else
	{
	std::vector<RGBTRIPLE> buffer {0};		// store old palette
	buffer.resize(ColoursInPALFile);
	buffer = pal;
//	memcpy(buffer, pal, ColoursInPALFile * 3);
//	for (i = 16; i < LocalThreshold; i++)	// start a little way into the palette to get the starting colours
	for (i = 0; i < LocalThreshold; i++)	
	    {
	    j = (long)(((double)i * (double)ColoursInPALFile) / (double)LocalThreshold);
	    pal[i] = buffer[j];
	    }
	}
    }

/**************************************************************************
	Fill palette values for iteration greater than 256
**************************************************************************/

void	CTrueCol::FinalisePalette(int level, long threshold)

    {
    int	    i;
    
    LocalThreshold = (threshold >= MAXPALETTE) ? MAXPALETTE - 1 : threshold;
    if (LocalThreshold > level)
	{
	for (i = level; i < LocalThreshold; i++)
	    {
	    TrueCol.PalettePtr[i] = TrueCol.PalettePtr[i % level];
	    }
	}
    }

/**************************************************************************
	Toggle flag to show whether to display a random palette or not
**************************************************************************/

void	CTrueCol::ToggleRandomColour(void)
    {
    RandomColourFlag = !RandomColourFlag;       
    }

/**************************************************************************
	Toggle flag to show whether to display a random palette or not
**************************************************************************/

void	CTrueCol::SetTrueColourPixel(int b, int g, int r, long threshold)
    {
    LocalThreshold = (threshold >= MAXPALETTE) ? MAXPALETTE - 1 : threshold;
    TrueCol.PalettePtr[LocalThreshold].rgbtBlue = (BYTE)b;
    TrueCol.PalettePtr[LocalThreshold].rgbtGreen = (BYTE)g;
    TrueCol.PalettePtr[LocalThreshold].rgbtRed = (BYTE)r;
    }

