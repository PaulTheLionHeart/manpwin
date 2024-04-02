//////////////////////////////////////////////////////////////////////
//
// colour.h: interface for true colour palettes. implementation of the CTrueCol class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

#ifndef WIN95
#define WIN95	
#endif

#define IDC_START_RED	213
#define IDC_START_BLUE	214
#define IDC_START_GREEN	215
#define IDC_INC_GREEN	216
#define IDC_INC_RED	217
#define IDC_INC_BLUE	218
#define IDC_REVERSE	219
#define IDC_INVERT	220
#define IDC_CYCLE	221
#define IDC_FORWARD	222
#define IDC_CYCLE_START	223
#define IDC_RANDPAL	238
#define IDC_RAND_DIV    239
//#define IDC_GENPAL	166
#define IDC_DISPLAYPAL	167
#define IDC_REDSCROLL	230	// note that these scroll functions must stay in this order
#define IDC_GRNSCROLL	231
#define IDC_BLUSCROLL	232
#define IDC_RANDSCROLL  233
#define IDM_DEFAULTCOL	237

#define	MAXTHRESHOLD	50000000
#define	TWO_PI		6.28318530717958

#define	STRETCH		0
#define	REPEAT		1

#define max(a,b)    (((a) > (b)) ? (a) : (b))
#define min(a,b)    (((a) < (b)) ? (a) : (b))

//////////////////////////////////////////////////////////////////////
// Class definition
//////////////////////////////////////////////////////////////////////

class CTrueCol
    {
    public:
    CTrueCol();
    virtual		~CTrueCol();
    void		InitTrueColPal(BYTE, long, int, int, int, BOOL, BYTE[]);
//    BYTE		PalettePtr[MAXTHRESHOLD * 3];	// for maximum palette					yep, it's a bad cludge, but at least it doesn't crash
    BYTE		*PalettePtr = NULL;		// [MAXTHRESHOLD * 3];	// for maximum palette
    WORD		RandomDivisor;			// Scale random true colour palette generation
    BYTE		RandomColourFlag;		// Use random true colour palette generation
    BOOL		PalEditFlag;			// have we loaded a true colour triplets file or edited a palette?
    BOOL		DisplayPaletteFlag = true;	// Display palette 
    BYTE		ScriptPaletteFlag;		// Display palette in animation frames
    BOOL		Stretch;			// When increasing threshold beyond the size of the palette, do we stretch or repeat the palette?
    WORD		RedStartInt;			// parameters for creating true colour palettes
    WORD		GreenStartInt;
    WORD		BlueStartInt;
    WORD		RedIncInt;
    WORD		GreenIncInt;
    WORD		BlueIncInt;
    short		InsideRed;			// values for r, g, b channels for inside colour
    short		InsideGreen;			
    short		InsideBlue;
    int			inside_colour;			// normal 'lake' colour
    long		ColoursInPALFile;		// How many colours are in a palette file?
    long		LocalThreshold;
    long		FinalThreshold;
    bool		IsMAPFile;			// have we loaded a MAP file?
    BYTE		*DefaultPalettePtr;		// [MAXTHRESHOLD * 3];	// for maximum palette

    void		FillPalette(int FillType, BYTE *pal, long threshold);
    void		FinalisePalette(int level, long threshold);
    void		ToggleRandomColour(void);
    void		SetTrueColourPixel(int b, int g, int r, long threshold);
    private:
    };

