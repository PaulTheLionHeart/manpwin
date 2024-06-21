/*
    SYM.CPP a module to handle symmetry.
    
    Written in Microsoft Visual 'C++' by Paul de Leeuw.

    This program is written in "standard" C. Hardware dependant code
    (console drivers & serial I/O) is in separate machine libraries.
*/

#include "Pixel.h"

/**************************************************************************
Symmetry plot setup x axis
**************************************************************************/

int	CPixel::xsym_split(int xaxis_row, int xaxis_between/*, int *symmetry*/)

    {
    int	i;

    if ((worksym & 0x11) == 0x10)
	return(1);
    if ((worksym & 1) != 0)
	iystop = (yystart + yystop) / 2;
    else
	{
	worksym |= 0x10;
	if (xaxis_row <= yystart || xaxis_row >= yystop)
	    return(1);					/* axis not in window */
	i = xaxis_row + (xaxis_row - yystart);
	if (xaxis_between)
	    ++i;
	if (i > yystop)	/* split into 2 pieces, bottom has the symmetry */
	    {
	    if (num_worklist >= MAXCALCWORK - 1)		/* no room to split */
		return(1);
	    iystop = xaxis_row - (yystop - xaxis_row);
	    if (!xaxis_between)
		--iystop;
	    add_worklist(xxstart, xxstop, iystop + 1, yystop, iystop + 1, workpass, 0);
	    yystop = iystop;
	    return(1);	/* tell set_symmetry no sym for current window */
	    }
	if (i < yystop)		/* split into 2 pieces, top has the symmetry */
	    {
	    if (num_worklist >= MAXCALCWORK - 1)		/* no room to split */
		return(1);
	    add_worklist(xxstart, xxstop, i + 1, yystop, i + 1, workpass, 0);
	    yystop = i;
	    }
	iystop = xaxis_row;
	worksym |= 1;
	}
    *symmetry = 0;
    return(0);				/* tell set_symmetry its a go */
    }

/**************************************************************************
Symmetry plot setup y axis
**************************************************************************/

int	CPixel::ysym_split(int yaxis_col, int yaxis_between/*, int *symmetry*/)

    {
    int	i;

    if ((worksym & 0x22) == 0x20)
	return(1);
    if ((worksym & 2) != 0)
	ixstop = (xxstart + xxstop) / 2;
    else
	{
	worksym |= 0x20;
	if (yaxis_col <= xxstart || yaxis_col >= xxstop)
	    return(1);				/* axis not in window */
	i = yaxis_col + (yaxis_col - xxstart);
	if (yaxis_between)
	    ++i;
	if (i > xxstop)	/* split into 2 pieces, right has the symmetry */
	    {
	    if (num_worklist >= MAXCALCWORK - 1)		/* no room to split */
		return(1);
	    ixstop = yaxis_col - (xxstop - yaxis_col);
	    if (!yaxis_between)
		--ixstop;
	    add_worklist(ixstop + 1, xxstop, yystart, yystop, yystart, workpass, 0);
	    xxstop = ixstop;
	    return(1);	/* tell set_symmetry no sym for current window */
	    }
	if (i < xxstop)		/* split into 2 pieces, left has the symmetry */
	    {
	    if (num_worklist >= MAXCALCWORK - 1)	/* no room to split */
		return(1);
	    add_worklist(i + 1, xxstop, yystart, yystop, yystart, workpass, 0);
	    xxstop = i;
	    }
	ixstop = yaxis_col;
	worksym |= 2;
	}
    *symmetry = 0;
    return(0);				/* tell set_symmetry its a go */
    }

/**************************************************************************
	Calculate and assign best symmetry 
**************************************************************************/

int	CPixel::FindSymmetry(BYTE _3dflag, int decomp, BYTE pairflag, int method, BOOL invert, int CoordSystem, double param[], WORD degree, WORD type,
    int subtype, BYTE calcmode, int RotationAngle, CFract *Fractal, double hor, double vert, double mandel_width, BYTE BigNumFlag, BigDouble *Big_xxmin,
    BigDouble *Big_xxmax, BigDouble *Big_yymin, BigDouble *Big_yymax, BigDouble BigHor, BigDouble BigVert, BigDouble BigWidth, double ScreenRatio, double *xxmin, 
    double *xxmax, double *yymin, double *yymax, WORD special, BYTE juliaflag)

    {
    int	fract_sym;
    
    //worksym = worksymIn; /yystart = yystartIn; yystop = yystopIn;
    *symmetry = 1;							// symmetry flag

    if (_3dflag || decomp > 0 || pairflag || method > 0 || invert)
	fract_sym = NOSYM;
    else if (CoordSystem != CARTESIAN)
	fract_sym = NOSYM;
    else
	{
	switch (type)
	    {
	    case NEWTONVARIATION:
	    case NEWTON:						// Newton fractal 
		degree = (int)param[0];					// fall through - Newton degree is initalised via a different dialogue box and doesn't use param[0]
/*		if (param[1] != 0.0 || param[2] != 0.0)			// perturbation not zero
		    fract_sym = NOSYM;
		else */if (degree % 2 == 0)				// special degree 
		    fract_sym = XYAXIS;
		else
		    fract_sym = XAXIS;
		break;
	    case MANDELFP:						// mandelbrot set 
		if (param[0] != 0.0 || param[1] != 0.0)
		    fract_sym = NOSYM;
		else if (juliaflag)
		    fract_sym = ORIGIN;
		else
		    fract_sym = XAXIS;
		break;
	    case POWER:							// POWER fractals 
		degree = (int)param[0];
		if (param[1] != 0.0 || param[2] != 0.0)			// perturbation not zero
		    fract_sym = NOSYM;
		else if (juliaflag)
		    {
//		    if (degree % 2 == 0)
//			fract_sym = ORIGIN;
//		    else
			fract_sym = NOSYM;
		    }
		else
		    {
//		    if ((degree - 1) % 2 == 0)
//			fract_sym = XYAXIS;
//		    else
			fract_sym = XAXIS;
		    }
		break;
	    case NOVA:
		degree = (int)param[0];
		fract_sym = XAXIS;
		break;
	    case NEWTONPOLYGON:						// Newton Polygon fractals 
	    case NEWTONFLOWER:
	    case NEWTONMSET:
	    case NEWTONCROSS:
	    case QUARTET1:
	    case QUARTET2:
	    case QUARTET3:
		degree = (int)param[0];
		if (param[1] != 0.0 || param[2] != 0.0)			// perturbation not zero
		    fract_sym = NOSYM;
		else if (juliaflag)
		    fract_sym = NOSYM;
		else
		    {
//		    if ((degree - 1) % 2 == 0)
//			fract_sym = XYAXIS;
//		    else
			fract_sym = XAXIS;
		    }
		break;
	    case CUBIC:							// CUBIC fractals 
		if (juliaflag || param[2] != 0.0 || param[3] != 0.0)
		    fract_sym = NOSYM;
		else
		    fract_sym = XAXIS;					// XYAXIS;
		break;
	    case SPECIALNEWT:						// type SPECIALNEWT fractal 
		if (juliaflag || special				// if special != 0, phase used
		    || param[1] != 0.0 || param[2] != 0.0)		// perturbation not zero
		    fract_sym = NOSYM;
		else
		    fract_sym = XAXIS;					// XYAXIS;
		break;
	    case CIRCLESQ:						// circle squared 
		fract_sym = XAXIS;					// XYAXIS;
		break;
	    case EXPFRACTAL:						// exponential fractal 
	    case BARNSLEYM3:
	    case MANDELLAMBDAFP:
	    case SPIDERFP:
	    case MANOWARFP:
	    case TETRATEFP:
		if (param[0] != 0.0 || param[1] != 0.0)
		    fract_sym = NOSYM;
		else if (juliaflag)
		    fract_sym = NOSYM;
		else
		    fract_sym = XAXIS;
		break;
	    case TETRATION:
		if (param[0] != 0.0 || param[1] != 0.0 || param[2] != 0.0)
		    fract_sym = NOSYM;
		else if (juliaflag)
		    fract_sym = NOSYM;
		else
		    fract_sym = XAXIS;
		break;
	    case SINFRACTAL:						// sin() type fractal 
	    case MATEIN:						// MATEIN?? fractal 
	    case BARNSLEYM1:
		if (param[0] != 0.0 || param[1] != 0.0)
		    fract_sym = NOSYM;
		else if (juliaflag)
		    fract_sym = NOSYM;
		else
		    fract_sym = XAXIS;					//bXYAXIS;
		break;
	    case BARNSLEYM2:
		if (param[0] != 0.0 || param[1] != 0.0)
		    fract_sym = NOSYM;
		else if (juliaflag)
		    fract_sym = NOSYM;
		else
		    fract_sym = YAXIS;
		break;
	    case FORMULA:
		fract_sym = NOSYM;					// for now anyway
//		*yymax = vert + mandel_width;
//		fract_sym = *symmetry;
		break;
//	    case SCREENFORMULA:
//		fract_sym = symmetry;
//		break;
	    case UNITYFP:
		fract_sym = XYAXIS;
		break;
	    case SIERPINSKI:
	    case TEST:
//	    case MANDALA:
//	    case MARCUSIFS:
	    case BUDDHABROT:
	    case COMPLEXNEWTON:
	    case COMPLEXBASIN:
	    case SQR1OVERTRIGFP:
	    case SQRTRIGFP:
	    case TRIGPLUSTRIGFP:
	    case TRIGXTRIGFP:
	    case TRIGSQRFP:
	    case LAMBDATRIGFP:
	    case PERTURBATION:
	    case KLEINIAN:
	    case SLOPEDERIVATIVE:
	    case SLOPEFORWARDDIFF:
		fract_sym = NOSYM;
		break;
	    case NEWTONAPPLE:
	    case MAGNET1:
	    case MAGNET2:
	    case TEDDY:
	    case NEWTONJULIANOVA:
	    case TALIS:
	    case RAMONSIN:
	    case RAMONCOS:
	    case FORMULA05:
	    case SHARON:
	    case QUAD:
		if (param[0] != 0.0 || param[1] != 0.0)
		    fract_sym = NOSYM;
		else if (juliaflag)
		    fract_sym = NOSYM;
		else
		    fract_sym = fractalspecific[type].symmetry;
		break;
/*
	    case MARCUSMANDEL:
		if (param[0] != 0.0 || param[1] != 0.0)
		    fract_sym = NOSYM;
		else if (juliaflag)
		    fract_sym = NOSYM;
		else
		    fract_sym = NOSYM;
		break;
*/
	    case TIERAZON:
	    case MANDELDERIVATIVES:
		if (juliaflag)
		    fract_sym = NOSYM;
	// Most only have real and imaginary perturbation of Z in param[0] and param[1] and the param count is 2
	// However, if we have a power in param[0], these get moved to param[1] and param[2] giving a param count greater than 2
		else if (Fractal->NumParam == 2 && (param[0] != 0.0 || param[1] != 0.0))
		    fract_sym = NOSYM;
		else if (Fractal->NumParam >= 3 && (param[1] != 0.0 || param[2] != 0.0))
		    fract_sym = NOSYM;
		else							// only use symmetry if there are no offsets
		    {
		    if (type == TIERAZON)
			fract_sym = TierazonSpecific[subtype].symmetry;
		    else
			fract_sym = MandelDerivSpecific[subtype].symmetry;
		    }
		break;
	    default:			// who knows? 
		fract_sym = NOSYM;
		break;
	    }
	}

    if (calcmode == 'F')
	 fract_sym = NOSYM;
	//    fract_sym = NOSYM;					// debug
    if (RotationAngle == 90 || RotationAngle == 270)			// if rotated image by 90 or 270 degrees - then reverse symmetry
	{
	switch (fract_sym)
	    {
	    case XAXIS:
		fract_sym = YAXIS;
		break;
	    case YAXIS:
		fract_sym = XAXIS;
		break;
	    }
	}
    else if (RotationAngle != 0)					// if rotated image by intermediate angle
	fract_sym = NOSYM;

//    if (type != FORMULA)
	setsymmetry(fract_sym, /*thread, */1, _3dflag, decomp, pairflag, method, invert, CoordSystem, param, degree, type, subtype, calcmode, RotationAngle, Fractal, hor,
	    vert, mandel_width, xxmax, xxmin, yymin, yymax, special, juliaflag, PlotType, BigNumFlag, Big_xxmin, Big_xxmax, Big_yymin, Big_yymax, BigHor, BigVert, BigWidth, 
	    ScreenRatio /*ixstop, xxstop, iystop, yystop*/);

    return 0;
    }

    /**************************************************************************
    Set up proper symmetrical plot functions
    **************************************************************************/

    void	CPixel::setsymmetry(int sym, int uselist, BYTE _3dflag, int decomp, BYTE pairflag, int method, BOOL invert, int CoordSystem, double param[],
	WORD degree, WORD type, int subtype, BYTE calcmode, int RotationAngle, CFract *Fractal, double hor, double vert, double mandel_width, double *xxmax, double *xxmin,
	double *yymin, double *yymax, WORD special, BYTE juliaflag, int *PlotType, BYTE BigNumFlag, BigDouble *Big_xxmin, BigDouble *Big_xxmax, BigDouble *Big_yymin,
	BigDouble *Big_yymax, BigDouble BigHor, BigDouble BigVert, BigDouble BigWidth, double ScreenRatio)

	{
	int		xaxis_row, yaxis_col;			// pixel number for origin 
	int		xaxis_between, yaxis_between;		// if axis between 2 pixels, not on one 
	double	ftemp;
	BigDouble	t1, xAbs, yAbs;

	if (sym == NOPLOT)
	    {
	    *PlotType = NOPLOT;
	    return;
	    }
	else if (sym == NOSYM && *PlotType < FILTERPLOT)	// don't splatter filkters etc
	    *PlotType = NOSYM;

	xaxis_row = yaxis_col = -1;
	if (BigNumFlag)
	    {
	    *Big_xxmin = BigHor;
	    *Big_xxmax = BigHor + BigWidth * ScreenRatio;	// aspect ratio 
	    *Big_yymin = BigVert;
	    *Big_yymax = BigVert + BigWidth;

	    xAbs = *Big_xxmin + *Big_xxmax;
	    yAbs = *Big_yymin + *Big_yymax;

	    if (yAbs.BigAbs() < Big_yymin->BigAbs() + Big_yymax->BigAbs())	// axis is on screen 
		{
		t1 = -(*Big_yymax) / (*Big_yymin - *Big_yymax);
		ftemp = (double)mpfr_get_d(t1.x, MPFR_RNDN) * (ydots - 1) + 0.25;
		xaxis_row = (int)ftemp;
		xaxis_between = (int)(ftemp - xaxis_row >= 0.5);
		if (uselist == 0 && (!xaxis_between || (xaxis_row + 1) * 2 != (int)ydots))
		    xaxis_row = -1;						// can't split screen, so dead center or not at all 
		}
	    if (xAbs.BigAbs() < Big_xxmin->BigAbs() + Big_xxmax->BigAbs())	// axis is on screen 
		{
		t1 = -(*Big_xxmin) / (*Big_xxmax - *Big_xxmin);
		ftemp = (double)mpfr_get_d(t1.x, MPFR_RNDN) * (xdots - 1) + 0.25;
		yaxis_col = (int)ftemp;
		yaxis_between = (int)(ftemp - yaxis_col >= 0.5);
		if (uselist == 0 && (!yaxis_between || (yaxis_col + 1) * 2 != (int)xdots))
		    yaxis_col = -1;						// can't split screen, so dead center or not at all
		}
	    }
	else
	    {
	    *xxmin = hor;
	    *xxmax = hor + mandel_width * ScreenRatio;			// aspect ratio 
	    *yymin = vert;
	    *yymax = vert + mandel_width;

	    if (fabs(*yymin + *yymax) < fabs(*yymin) + fabs(*yymax))	// axis is on screen 
		{
		ftemp = (0.0 - *yymax) / (*yymin - *yymax) * (ydots - 1) + 0.25;
		xaxis_row = (int)ftemp;
		xaxis_between = (int)(ftemp - xaxis_row >= 0.5);
		if (uselist == 0 && (!xaxis_between || (xaxis_row + 1) * 2 != (int)ydots))
		    xaxis_row = -1;						// can't split screen, so dead center or not at all 
		}
	    if (fabs(*xxmin + *xxmax) < fabs(*xxmin) + fabs(*xxmax))	// axis is on screen 
		{
		ftemp = (0.0 - *xxmin) / (*xxmax - *xxmin) * ((int)xdots - 1) + 0.25;
		yaxis_col = (int)ftemp;
		yaxis_between = (int)(ftemp - yaxis_col >= 0.5);
		if (uselist == 0 && (!yaxis_between || (yaxis_col + 1) * 2 != (int)xdots))
		    yaxis_col = -1;						// can't split screen, so dead center or not at all 
		}
	    }
	switch (sym)								// symmetry switch 
	    {
	    case XAXIS:							// X-axis Symmetry 
		if (xsym_split(xaxis_row, xaxis_between/*, symmetry*/) == 0)
		    {
		    if (type == NEWTON)					// Newton type fractal 
			{
			if (subtype == 'S' || subtype == 'B')
			    *PlotType = XAXISBASIN;
			else
			    *PlotType = XAXIS;
			}
		    else
			*PlotType = XAXIS;
		    }
		else
		    *PlotType = NOSYM;
		break;
	    case YAXIS:							// Y-axis Symmetry 
		if (ysym_split(yaxis_col, yaxis_between/*, symmetry*/) == 0)
		    *PlotType = YAXIS;
		else
		    *PlotType = NOSYM;
		break;
	    case XYAXIS:							// X-axis AND Y-axis Symmetry
		xsym_split(xaxis_row, xaxis_between/*, symmetry*/);
		ysym_split(yaxis_col, yaxis_between/*, symmetry*/);
		switch (worksym & 3)
		    {
		    case 1:							// just xaxis symmetry 
			if (type == NEWTON)					// Newton type fractal
			    {
			    if (subtype == 'S' || subtype == 'B')
				*PlotType = XAXISBASIN;
			    else
				*PlotType = XAXIS;
			    }
			else
			    *PlotType = XAXIS;
			break;
		    case 2:							// just yaxis symmetry
			if (type == NEWTON)					// Newton type fractal 
			    {
			    if (subtype == 'S' || subtype == 'B')
				{						// got no routine for this case 
				ixstop = xxstop;				// fix what split should not have done
				*symmetry = 1;
				}
			    else
				*PlotType = YAXIS;
			    }
			else
			    *PlotType = YAXIS;
			break;
		    case 3:							// both axes 
			if (type == NEWTON)					// Newton type fractal 
			    {
			    if (subtype == 'S' || subtype == 'B')
				*PlotType = XYAXISBASIN;
			    else
				*PlotType = XYAXIS;
			    }
			else
			    *PlotType = XYAXIS;
		    }
		break;
	    case ORIGIN:							// Origin Symmetry 
		if (xsym_split(xaxis_row, xaxis_between/*, symmetry*/) == 0 && ysym_split(yaxis_col, yaxis_between/*, symmetry*/) == 0)
		    {
		    *PlotType = ORIGIN;
		    ixstop = xxstop;					// didn't want this changed 
		    }
		else
		    {
		    iystop = yystop;					// in case first split worked 
		    *symmetry = 1;
		    worksym = 0x30;					// let it recombine with others like it 
		    }
		break;
	    default:							// no symmetry 
		break;
	    }
	}

    /**************************************************************************
	Init symmetry for standard plotting mode fractals
**************************************************************************/
/*
void	CCalcMode::InitCalcModeSym(int subtypeIn, WORD typeIn, BYTE _3dflagIn, BYTE pairflagIn, int decompIn, int methodIn, BOOL invertIn, double paramIn[], BYTE degreeIn, 
	double *xxminIn, double *xxmaxIn, double *yyminIn, double *yymaxIn, double horIn, double vertIn, double mandel_widthIn, double	ScreenRatioIn, int CoordSystemIn, 
	WORD specialIn, int orientationIn, CFract *FractalIn, BYTE BigNumFlagIn, BigDouble *Big_xxmaxIn, BigDouble *Big_xxminIn, BigDouble *Big_yymaxIn, 
	BigDouble *Big_yyminIn, BigDouble BigHorIn, BigDouble BigVertIn, BigDouble BigWidthIn)
    {
    subtype = subtypeIn; type = typeIn; _3dflag = _3dflagIn; pairflag = pairflagIn; decomp = decompIn; method = methodIn; invert = invertIn; degree = degreeIn;
    xxmin = xxminIn; xxmax = xxmaxIn; yymin = yyminIn; yymax = yymaxIn; hor = horIn; vert = vertIn; mandel_width = mandel_widthIn; ScreenRatio = ScreenRatioIn;
    CoordSystem = CoordSystemIn; special = specialIn; orientation = orientationIn; Fractal = FractalIn; BigNumFlag = BigNumFlagIn; Big_xxmax = Big_xxmaxIn; 
    Big_xxmin = Big_xxminIn; Big_yymax = Big_yymaxIn; Big_yymin = Big_yyminIn; BigHor = BigHorIn; BigVert = BigVertIn; BigWidth = BigWidthIn;
    for (int i = 0; i < NUMPERTPARAM; i++)
	param[i] = paramIn[i];

    }

*/