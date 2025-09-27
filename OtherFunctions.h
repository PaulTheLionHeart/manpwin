/////////////////////////////////////////////////////////////////////
//
// OtherFunctions.h: fractals that aren't raster based.
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include <Windows.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/timeb.h>
#include "complex.h"
#include "Fractype.h"
#include "ManpWin.h"
#include "Pixel.h"
#include "Manp.h"
#include "polygon.h"
#include "preview.h"

// Error codes for Cellular
#define BAD_T         1
#define BAD_MEM       2
#define STRING1       3
#define STRING2       4
#define TABLEK        5
#define TYPEKR        6
#define RULELENGTH    7

#define CELLULAR_DONE 10

#define rand15()    rand()
#define RANDOM(x)   (rand()%(x))

#ifdef _WIN64
#define MAXIMUM_TOWER	    20001
#else
#define MAXIMUM_TOWER	    10001
#endif
#define MAXIMUM_POINTS	    10000

#define max(a,b)    (((a) > (b)) ? (a) : (b))
#define min(a,b)    (((a) < (b)) ? (a) : (b))

// stuff for Apollonius Fractal
#define SQRT3    1.732050807568877193

//////////////////////////////////////////////////////////////////////
// Class definition
//////////////////////////////////////////////////////////////////////

class COtherFunctions
    {
    public:
	int	InitOtherFunctions(WORD type, int subtypeIn, HWND hwndIn, CTrueCol *TrueColIn, CDib *DibIn, char *AntStatusIn, struct __timeb64 FrameEndIn, struct __timeb64 FrameStartIn, double mandel_widthIn, 
		double horIn, double vertIn, double ScreenRatioIn, int *totpassesIn, int *curpassIn, int user_dataIn(HWND hwnd), double *wpixelsIn, BYTE *DefaultPaletteIn, int CoordSystemIn, //COscProcess OscProcessIn, 
		int xAxisIn, int yAxisIn, int zAxisIn);
	int	COtherFunctions::RunOtherFunctions(WORD type, BYTE *SpecialFlag, long *iteration, int xdots, int ydots, double param[], long threshold, int rotate(int dir));

	// stuff for Triangles
	void	GetExpandPalette(BOOL);		// spread colours across palette
	void	GetPlotStars(BOOL);		// stars or polygons?
	void	GetFillPolygon(BOOL);		// fill polygons?
	void	GetjAngle(int);
	void	GetwAngle(int);			// sets the overall shape of the fractal
	void	GetDivisor(int);
	void	GetRemainder(int);		// who knows, I don't
	void	GetExponent(double);
	void	GetSides(int);			// sides of polygon
	void	GetPasses(int);			// number of passes

	// stuff for Circles
	void	GetUseDefaultPalette(BOOL);	// standard EGA palette
	void	GetFilledCircle(int);		// 1 = filled, 2 = unfilled, 3 = 3D

	// stuff for Pascal Triangle : Escher (Sierpinski)
	void	GetNumberIterations(int);
	void	Getmoda(int);
	void	Gethi(int);
	void	Getlo(int);
	void	GetCircleSize(int);

	// stuff for geometry
	void	GetCount(long in);
	void	GetSubtype(int in);


    private:
	CPlot	Plot;
	double *wpixels;
	COscProcess OscProcess;
	int	subtype;
	double	mandel_width;
	double	hor;
	double	vert;
	double	ScreenRatio;
	int	*totpasses;
	int	*curpass;
	int(*UserData)(HWND);
	BYTE	*DefaultPalette;
	int	CoordSystem;
	int	row, col;
	Complex	c, z;
	HWND	hwnd;


    // Stuff for Cellular
	void	abort_cellular(int err, int t);
	void	thinking(HWND hwnd, int count);

	WORD	cell_array[2][MAXHORIZONTAL];
	int	r, k_1, rule_digits;
	int	lstscreenflag;
	int	rflag = FALSE;
	int	rseed;

	// Stuff for DemoWalk
	int	colors = 256;

	// Stuff for Plasma
	WORD	rand16();
	void	subDivide(int x1, int y1, int x2, int y2, int user_data(HWND hwnd));
	int	new_subD(int x1, int y1, int x2, int y2, int recur, int user_data(HWND hwnd));
	WORD	adjust(int xa, int ya, int x, int y, int xb, int yb);
	void	set_Plasma_palette(int rotate(int direction));

	int	plasma_check;		// to limit kbd checking 
	int	iparmx;			// iparmx = parm.x * 16 
	int	shiftvalue;		// shift based on #colors 
	int	max_colors = 32000;
	int	recur1 = 1;
	int	pcolors;
	int	recur_level = 0;
	WORD	max_plasma;
	CTrueCol    *TrueCol;
	CDib	*Dib;

	// Stuff for Diffusion
	void	FPUsincos(double *Angle, double *Sin, double *Cos);

	// stuff for ant
#define RANDOMANT(n)    ((int)((long)((long)rand() * (long)(n)) >> 15)) // Generate Random Number 0 <= r < n
#define MAX_ANTS        256
#define XO              (xdots/2)
#define YO              (ydots/2)
#define DIRS            4
#define INNER_LOOP      100
	int	*incx[DIRS];         // tab for 4 directions
	int	*incy[DIRS];
	short	AntRseed;
	short	AntRflag;
	char	*AntStatus;
	struct __timeb64 	FrameEnd;
	struct __timeb64 	FrameStart;
	//	void	TurkMite2(int maxtur, size_t rule_len, char *ru, long maxpts, long wait);
	void	TurkMite1(int maxtur, size_t rule_len, char *ru, long maxpts, long wait);
	int	ant(void);
	int	tower(void);

	// stuff for Popcorn
	Complex g1(Complex x);
	Complex g2(Complex x);
	int	DoPopcorn();
	int	variation = 0;
	double	xmax = 1.7, xmin = -1.7, ymax = 1.7, ymin = -1.7;
	int	max_size = 1124;

	// stuff for Cross Roads
	int	DoCrossRoads();
	double  a = 2.23, b = 0.76, C = 0.3;
	double	Div[24] = { 3.0, 3.0, 1.3, 1.423, 1.3, 1.3, 1.3, 1.2, 1.3, 1.0, 1.3 };
	double	xscale, yscale;

	// stuff for ZigZag
	int	DoZigzag();

	// stuff for Triangles
	int	DoTriangle();
	int	IntPower(int base, int n);
	double	FloatPower(double base, int n);
	void	DrawBox(int x0, int y0, int x1, int y1, DWORD colour, BOOL fill, CPlot Plot);
	CPoly	polygon;			// polygon class

	BOOL	ExpandPalette;			// spread colours across palette
	BOOL	PlotStars;			// stars or polygons?
	BOOL	FillPolygon;			// fill polygons?
	int	jAngle, wAngle;			// sets the overall shape of the fractal
	int	Divisor, Remainder;		// who knows, I don't
	double	Exponent;
	int	Sides;				// sides of polygon

	// stuff for Circles
	int	DoCircles();
	void	ScaledCircle(double x, double y, double radius, DWORD colour);
	int	DoFordFroth();
	int	DoCurvedIFS();
	BOOL	UseDefaultPalette = TRUE;	// standard EGA palette
	int	Passes = 10;
	int	FilledCircle = 1;		// 1 = filled, 2 = unfilled, 3 = 3D

	// stuff for Sierpinski Flowers
	int	COtherFunctions::DoSierpinskiFlower();

	// stuff for Pascal Triangle : Escher (Sierpinski)
	int	DoPascal();
	int	NumberIterations;
	int	moda;
	int	hi;
	int	lo;
	int	CircleSize;

	// stuff for Apollonian Circles
	int	DoApolloniusIFS();

	// stuff for geometry
	int	DoGeometry();
	int	sides;				// number of sides of polygon or star
	long	count;				// number of polygons or stars
	
	// stuff for malthus
	int	DoMalthus();
	int	DisplayMalthus(double c1[], DWORD colour, int dimensions, CMatrix Mat);
	int	xAxis;
	int	yAxis;
	int	zAxis;

    };

