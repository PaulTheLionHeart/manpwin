/*
    VARIABLES.CPP a module to load the variables used in plotting routines.
    Written in Microsoft Visual 'C++' by Paul de Leeuw.
*/


#include	"variables.h"

void	CMyVariables::InitVariables(Complex zIn, Complex qIn, Complex jIn, Complex cIn, double horIn, double vertIn, double xgapIn, double ygapIn, int methodIn, WORD typeIn, int time_to_zoomIn, int time_to_restartIn, int time_to_reinitIn, int time_to_quitIn, CTZfilter *TZfilterIn, RGBTRIPLE *FilterRGBIn, int orientationIn, BYTE pairflagIn, 
    double yymaxIn, BYTE _3dflagIn, long andcolorIn, int decompIn, long colorIn, BYTE BigNumFlagIn, int NonStandardFractalIn, int reset_periodIn, int groundIn, int stereo_signIn, int AutoStereo_valueIn, double x_rotIn, double y_rotIn, double z_rotIn, double sclxIn, double sclyIn, double sclzIn, 
    long (*dofractIn)(HWND hwnd, CMyVariables *), long (*BigCalcFracIn)(HWND, int, int, CMyVariables *, int user_data(HWND hwnd)))

    {
    z = zIn;					// basic complex variables
    q = qIn;
    j = jIn;
    c = cIn;

    hor = horIn;				// horizontal address
    vert = vertIn;				// vertical address 
    xgap = xgapIn;				// gap between pixels 
    ygap = ygapIn;				// gap between pixels 

    method = methodIn;				// inside and outside filters
    type = typeIn;				// M=mand, J=Julia 1,2,4->

    time_to_zoom = time_to_zoomIn;		// time to zoom in or out?
    time_to_restart = time_to_restartIn;	// time to restart?
    time_to_reinit = time_to_reinitIn;		// time to reinitialize?
    time_to_quit = time_to_quitIn;		// time to quit?

    TZfilter = TZfilterIn;			// Tierazon filters
    FilterRGB = FilterRGBIn;			// Tierazon filters

    orientation = orientationIn;		// 0, 90, 180 or 270 degrees
    pairflag = pairflagIn;			// stereo pair
    yymax = yymaxIn;				// max value of vert

    _3dflag = _3dflagIn;			// generate image in 3D
    andcolor = andcolorIn;			// skip color zero
    decomp = decompIn;				// number of decomposition colours 
    color = colorIn;				// used for Newton root locations 
//    phaseflag = phaseflagIn;			// 3 phases for type SPECIALNEWT fractals 
    BigNumFlag = BigNumFlagIn;			// True if bignum used
//    special = specialIn;			// special colour for CBIN fractals
    NonStandardFractal = NonStandardFractalIn;	// does fractal use standard plotting mode?
    reset_period = reset_periodIn;		// periodicity checking
    ground = groundIn; 				// set limits on separation
    stereo_sign = stereo_signIn;		// Variable stereo_sign is used to convert valleys into hills. (Negative value for - R flag).
    AutoStereo_value = AutoStereo_valueIn;	// AutoStereo depth value
    x_rot = x_rotIn;				// angle display plane to x axis
    y_rot = y_rotIn;
    z_rot = z_rotIn;
    sclx = sclxIn;				// scale
    scly = sclyIn;
    sclz = sclzIn;
    dofract = dofractIn;
    BigCalcFrac = BigCalcFracIn;




//    DWORD 	*wpixels;					// the device-independent bitmap  pixels
/*
    Complex	temp0;						// some temp ones!!!
    Complex	temp1;
    Complex	temp2;
    Complex	temp3;
    Complex	temp4;
    Complex	temp5;
    Complex	tempsqr;
    Complex	tempxy;

    double	tmpfl0;						// some temp floats just in case
    double	tmpfl1;
    double	tmpfl2;
    double	tmpfl3;
    double	xgap;						// gap between pixels
    double	ygap;
    int		row, col, currow, curcol;
    int		oldcol;
    int		oldrow;
    long	andcolor;
    int		PhoenixDegree;

    int		time_to_zoom;					// time to zoom in or out?
    int		time_to_restart;				// time to restart?
    int		time_to_reinit;					// time to reinitialize?
    int		time_to_quit;					// time to quit?
    int 	time_to_load;					// time to load file?
    int		time_to_break;					// time to break out of animation?
								// some general storage
    long	color, oldcolor;
    int		iteration;
    int		reset_period;					// periodicity checking
    int		ThreadNum;
    int		TotalThreads;




    long	iteration;					// globals for speed
    int		method;						// inside and outside filters
    BOOL	ExpandStarTrailColours = TRUE;			// use the first 16 colours if false, else expand across the whole iteration range
    BOOL	invert = FALSE;					// invert fractal

    BYTE	SpecialFlag;					// tell graphing routine of special conditions
    WORD	special;					// special colour for CBIN fractals
    int		biomorph;					// biomorph colour

    int		FilterType;					// data for Tierazon filters

    _CMPLX	old;						// to allow Fractint fractals to work with filters
    double	dem_delta, dem_width;				// distance estimator variables 
    double	dem_toobig;
    int		CoordSystem;

    double	dStrands;

    long	threshold;
    int		decomp;						// number of decomposition colours 
    long	color;						// used for Newton root locations 
    BYTE	orbit_flag;					// display orbits? 
    BYTE	juliaflag;					// Julia implementation of fractal 
    BYTE	phaseflag;					// 3 phases for type SPECIALNEWT fractals 
    WORD	type;						// M=mand, J=Julia 1,2,4-> 
    int		subtype;					// B=basin, S=stripe, N=normal 
    long	oldcolour;					// for periodicity checking 
    int		reset_period;					// periodicity checking 
    int		period_level;					// 0 for no periodicity checking 
    double	closenuff;					// periodicity bailout 
    _CMPLX	parm, parm2;					// got to get the stuff into the routines someway
    double	param[];
    //extern	_CMPLX	New, old;
    double	rqlim;
    int		distest;
    BYTE	BigNumFlag;					// True if bignum used
    int		NonStandardFractal;				// does fractal use standard plotting mode?
    BYTE	pairflag;					// stereo pair
    double	yymax;						// max value of vert
    BYTE	calcmode;					// trace type B, G, 1, 2 
    BYTE	_3dflag;					// generate image in 3D

    int		oldrow = -1;
    int		oldcol = -1;
    long	andcolor;

    int		orientation;					// 0, 90, 180 or 270 degrees

    int		logval;						// log colour map 
    BYTE 	*logtable;					// log value table for col comp

    int		float_decomposition(double, double);
    int		potential(double, int);
    long	BigCalcFrac(HWND, int, int);
    void	projection(int, int, long);
    void	do_stereo_pairs(int, int, long);
    void	draw_right_image(short);

    BOOL	UseCurrentPalette;				// do we use the ManpWIN palette? If false, generate internal filter palette
    Complex	invertz2(Complex  &);
    COscProcess	OscProcess;

    CTrueCol    TrueCol;


    Complex	z, q, j, c, deriv;

    */


//    long	*wpixels;					// pointer to an array of colours for each pixel
    // Bignum variables
//    BigComplex	zBig, qBig, cBig;
//    BigDouble	BigBailout;

//    CDib	ThreadDib;					// Device Independent Bitmap
//    HWND	hwnd;

    };

