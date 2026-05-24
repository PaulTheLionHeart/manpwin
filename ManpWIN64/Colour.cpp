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

CTrueCol::CTrueCol()			// Persistent colour-system initialisation
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
    for (size_t i = 0; i < ColoursInPALFile; ++i)
	{
	DefaultPalettePtr[i].rgbtRed = default_palette[3 * i + 2];
	DefaultPalettePtr[i].rgbtGreen = default_palette[3 * i + 1];
	DefaultPalettePtr[i].rgbtBlue = default_palette[3 * i + 0];
	}
    }

CTrueCol::~CTrueCol()
    {
//    if (PalettePtr) { delete[] PalettePtr; PalettePtr = NULL; }
    }

//////////////////////////////////////////////////////////////////////
// Global declaration
//////////////////////////////////////////////////////////////////////

//CTrueCol    TrueCol;

//////////////////////////////////////////////////////////////////////
//	Generate True Colour COL
//////////////////////////////////////////////////////////////////////

void CTrueCol::BuildCOLColourSource(BYTE RandFlag, long threshold, int StartColourCycling, int logval)
    {
    long    i, cycle, temp;
    double  RedStart, GreenStart, BlueStart;
    double  RedInc, GreenInc, BlueInc;
    float   size;
    static  WORD Randomise;

    if (RandFlag)
	{
	srand((unsigned)time(NULL) + Randomise);

	if (RandomColourFlag)
	    {
	    gManp->TrueCol.RedIncInt = rand() / RandomDivisor;
	    gManp->TrueCol.GreenIncInt = rand() / RandomDivisor;
	    gManp->TrueCol.BlueIncInt = rand() / RandomDivisor;
	    gManp->TrueCol.RedStartInt = rand() / RandomDivisor;
	    gManp->TrueCol.GreenStartInt = rand() / RandomDivisor;
	    gManp->TrueCol.BlueStartInt = rand() / RandomDivisor;
	    }

	Randomise = rand();
	}

    RedStart = (float)(gManp->TrueCol.RedStartInt) / 100.0;
    GreenStart = (float)(gManp->TrueCol.GreenStartInt) / 100.0;
    BlueStart = (float)(gManp->TrueCol.BlueStartInt) / 100.0;

    RedInc = (float)(gManp->TrueCol.RedIncInt) / 100.0;
    GreenInc = (float)(gManp->TrueCol.GreenIncInt) / 100.0;
    BlueInc = (float)(gManp->TrueCol.BlueIncInt) / 100.0;

    LocalThreshold = (threshold >= MAXPALETTE) ? MAXPALETTE - 1	: threshold;

    temp = ((long)StartColourCycling > LocalThreshold) ? LocalThreshold	: StartColourCycling;

    for (i = 1L; i < LocalThreshold; ++i)
	{
	cycle = temp + i;

	if (cycle > LocalThreshold)
	    cycle -= LocalThreshold;

	size = (float)cycle / (float)((logval != 0) ? 256 : LocalThreshold);
	PalettePtr[i].rgbtBlue = (BYTE)(127.0 * sin(TWO_PI * (size + RedStart) * RedInc)) + 128;
	PalettePtr[i].rgbtGreen = (BYTE)(127.0 * sin(TWO_PI * (size + GreenStart) * GreenInc)) + 128;
	PalettePtr[i].rgbtRed = (BYTE)(127.0 * sin(TWO_PI * (size + BlueStart) * BlueInc)) + 128;
	}

    PalettePtr[LocalThreshold].rgbtBlue = (BYTE)InsideBlue;
    PalettePtr[LocalThreshold].rgbtGreen = (BYTE)InsideGreen;
    PalettePtr[LocalThreshold].rgbtRed = (BYTE)InsideRed;
    }

//////////////////////////////////////////////////////////////////////
//	Build Palette MAP
//////////////////////////////////////////////////////////////////////

void CTrueCol::BuildDefaultPaletteSource(long threshold)
    {
    for (int i = 0; i < ColoursInPALFile; i++)
	PalettePtr[i] = DefaultPalettePtr[i];

    FillPalette(REPEAT, PalettePtr, threshold);
    }

//////////////////////////////////////////////////////////////////////
//	Build Palette from sources (MAP table or COL palette generator)
//////////////////////////////////////////////////////////////////////

void	CTrueCol::BuildIterationColourMap(BYTE RandFlag, long threshold, int StartColourCycling, int logval, BOOL UseFractintPalette)
    {
    ColoursInPALFile = (long)colourCount;
    if (PalEditFlag)
	{
	if (Stretch)
	    FillPalette(STRETCH, PalettePtr, threshold);
	else
	    FillPalette(REPEAT, PalettePtr, threshold);
	return;
	}

    if (IsMAPFile)
	{
	if (UseFractintPalette)
	    BuildDefaultPaletteSource(threshold);
	// else:
	// external MAP file already populated PalettePtr[] when MAP file loaded or from PAR/PNG/KFR sources
	}
    else
	{
	BuildCOLColourSource(RandFlag, threshold, StartColourCycling, logval);
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
	    gManp->TrueCol.PalettePtr[i] = gManp->TrueCol.PalettePtr[i % level];
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
    gManp->TrueCol.PalettePtr[LocalThreshold].rgbtBlue = (BYTE)b;
    gManp->TrueCol.PalettePtr[LocalThreshold].rgbtGreen = (BYTE)g;
    gManp->TrueCol.PalettePtr[LocalThreshold].rgbtRed = (BYTE)r;
    }

/**************************************************************************
   Allow palette to move in zoom animations
**************************************************************************/

void	CTrueCol::MovePalette(CTrueCol *TrueCol, int PaletteShift, int threshold)
    {
    int		i, j, AbsPalShift;
    RGBTRIPLE	TempPal;
    long	thresh = TrueCol->FinalThreshold;

    if (PaletteShift == 0)
	return;

    AbsPalShift = abs(PaletteShift);
    for (j = 0; j < AbsPalShift; j++)
	{
	if (PaletteShift > 0)
	    {
	    TempPal = TrueCol->PalettePtr[(thresh - 1) % MAXPALETTE];		// load last entry for 'ron
	    for (i = thresh - 2; i >= 0; i--)
		TrueCol->PalettePtr[(i + 1) % MAXPALETTE] = TrueCol->PalettePtr[i % MAXPALETTE];
	    TrueCol->PalettePtr[0] = TempPal;					// restore last entry to the beginning
	    }
	else
	    {
	    TrueCol->PalettePtr[0] = TempPal;					// load first entry for 'ron
	    for (i = 0; i < thresh - 1; i++)
		TrueCol->PalettePtr[i % MAXPALETTE] = TrueCol->PalettePtr[(i + 1) % MAXPALETTE];
	    TrueCol->PalettePtr[(thresh - 1) % MAXPALETTE] = TempPal;
	    }
	}
    }

