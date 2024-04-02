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
    PalettePtr = new BYTE[MAXTHRESHOLD * 3];
    }

CTrueCol::~CTrueCol()
    {
    if (PalettePtr) { delete[] PalettePtr; PalettePtr = NULL; }
    }

//////////////////////////////////////////////////////////////////////
// Global declaration
//////////////////////////////////////////////////////////////////////

CTrueCol    TrueCol;

//////////////////////////////////////////////////////////////////////
//	Initialise True Colour Palette
//////////////////////////////////////////////////////////////////////

void	CTrueCol::InitTrueColPal(BYTE RandFlag, long threshold, int StartColourCycling, int logval, int bits_per_pixel, BOOL UseFractintPalette, BYTE default_palette[])

    {
    long	i, cycle, temp;
    double	RedStart, GreenStart, BlueStart, RedInc, GreenInc, BlueInc;
    float	size;
    static	WORD	Randomise;

    if (PalEditFlag)
	{
	if (Stretch)
	    FillPalette(STRETCH, PalettePtr, threshold);
	else
	    FillPalette(REPEAT, PalettePtr, threshold);
	return;
	}

    DefaultPalettePtr = default_palette;

    if (UseFractintPalette)
	{
	memcpy(PalettePtr, default_palette, 256 * 3);
	//	FillPalette(STRETCH, PalettePtr);
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

	LocalThreshold = (threshold >= MAXTHRESHOLD) ? MAXTHRESHOLD - 1 : threshold;
	temp = ((long)StartColourCycling > LocalThreshold) ? LocalThreshold : StartColourCycling;
	for (i = 1L; i < LocalThreshold; ++i)
	    {
	    cycle = temp + i;
	    if (cycle > LocalThreshold)
		cycle -= LocalThreshold;
	    size = (float)cycle / (float)((logval != 0) ? 256 : LocalThreshold);
	    PalettePtr[i * 3 + 0] = (BYTE)(127.0 * sin(TWO_PI * (size + RedStart) * RedInc)) + 128;
	    PalettePtr[i * 3 + 1] = (BYTE)(127.0 * sin(TWO_PI * (size + GreenStart) * GreenInc)) + 128;
	    PalettePtr[i * 3 + 2] = (BYTE)(127.0 * sin(TWO_PI * (size + BlueStart) * BlueInc)) + 128;
	    }

	*(PalettePtr + LocalThreshold * 3 + 0) = (BYTE)InsideBlue;
	*(PalettePtr + LocalThreshold * 3 + 1) = (BYTE)InsideGreen;
	*(PalettePtr + LocalThreshold * 3 + 2) = (BYTE)InsideRed;
	// set up pointer to correct palette
//    PalettePtr = TRUE_PALETTE;
	bits_per_pixel = 24;
	}
    }

/**************************************************************************
	Fill palette values for iteration greater than 256
**************************************************************************/

void	CTrueCol::FillPalette(int FillType, BYTE *pal, long threshold)

    {
    long	i, j;

    LocalThreshold = (threshold >= MAXTHRESHOLD) ? MAXTHRESHOLD - 1 : threshold;
    if (FillType == REPEAT)
	{
	if (LocalThreshold > ColoursInPALFile)
	    {
	    for (i = ColoursInPALFile; i < LocalThreshold; i++)
		{
		*(pal + i * 3 + 0) = *(pal + (i % ColoursInPALFile) * 3 + 0);
		*(pal + i * 3 + 1) = *(pal + (i % ColoursInPALFile) * 3 + 1);
		*(pal + i * 3 + 2) = *(pal + (i % ColoursInPALFile) * 3 + 2);
		}
	    }
	}
    else
	{
	BYTE    *buffer = NULL;		// store old palette
	
	buffer = new BYTE[ColoursInPALFile * 3];

	memcpy(buffer, pal, ColoursInPALFile * 3);
//	for (i = 16; i < LocalThreshold; i++)	// start a little way into the palette to get the starting colours
	for (i = 0; i < LocalThreshold; i++)	
	    {
	    j = (long)(((double)i * (double)ColoursInPALFile) / (double)LocalThreshold);
	    *(pal + i * 3 + 0) = *(buffer + j * 3 + 0);
	    *(pal + i * 3 + 1) = *(buffer + j * 3 + 1);
	    *(pal + i * 3 + 2) = *(buffer + j * 3 + 2);
	    }
	if (buffer) { delete[] buffer; buffer = NULL; }
	}
    }

/**************************************************************************
	Fill palette values for iteration greter than 256
**************************************************************************/

void	CTrueCol::FinalisePalette(int level, long threshold)

    {
    int	    i;
    
    LocalThreshold = (threshold >= MAXTHRESHOLD) ? MAXTHRESHOLD - 1 : threshold;
    if (LocalThreshold > level)
	{
	for (i = level; i < LocalThreshold; i++)
	    {
	    *(TrueCol.PalettePtr + i * 3 + 0) = *(TrueCol.PalettePtr + (i % level) * 3 + 0);
	    *(TrueCol.PalettePtr + i * 3 + 1) = *(TrueCol.PalettePtr + (i % level) * 3 + 1);
	    *(TrueCol.PalettePtr + i * 3 + 2) = *(TrueCol.PalettePtr + (i % level) * 3 + 2);
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
    LocalThreshold = (threshold >= MAXTHRESHOLD) ? MAXTHRESHOLD - 1 : threshold;
    *(TrueCol.PalettePtr + LocalThreshold * 3 + 2) = (BYTE)b;
    *(TrueCol.PalettePtr + LocalThreshold * 3 + 1) = (BYTE)g;
    *(TrueCol.PalettePtr + LocalThreshold * 3 + 0) = (BYTE)r;
    }

