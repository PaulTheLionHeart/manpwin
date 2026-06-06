/*
    PIXEL.CPP - a module for the per pixel calculations of fractals. 
    
    Written in Microsoft Visual 'C++' by Paul de Leeuw.
*/

#include	<math.h>
#include	"manp.h"
#include	"colour.h"
#include	"Fract.h"
#include	"fractype.h"
#include	"fractalp.h"
#include	"filter.h"
#include	"complex.h"
#include	"dib.h"
#include	"Slope.h"
#include	"OscProcess.h"
#include	"pixel.h"
#include	"Potential.h"
#include	"PixelTemplate.h"

/**************************************************************************
    Small wrappers needed for formula
**************************************************************************/

// double
int CPixel::InitFormula(Complex* z, Complex* q)
    {
    return m_parser.FormPerPixelFloat(z, q);
    }

int CPixel::RunFormula(Complex* z, Complex* q)
    {
    return m_parser.FormulaFloat(z, q);
    }

// DD
int CPixel::DDInitFormula(DDComplex* z, DDComplex* q)
    {
    return m_parser.DDFormPerPixel(z, q);
    }

int CPixel::DDRunFormula(DDComplex* z, DDComplex* q)
    {
    return m_parser.DDFormula(z, q);
    }

//QD
int CPixel::QDInitFormula(QDComplex* z, QDComplex* q)
    {
    return m_parser.QDFormPerPixel(z, q);
    }

int CPixel::QDRunFormula(QDComplex* z, QDComplex* q)
    {
    return m_parser.QDFormula(z, q);
    }

/**************************************************************************
	Initialise fractal
**************************************************************************/

// double version
int	CPixel::InitFractal(Complex *z, Complex *q)
    {
    if (fractalspecific[type].flags & FUNCTIONINPIXEL)
	return(InitFunctions(type, z, q));
    else if (fractalspecific[type].flags & FRACTINTINPIXEL)
	return(InitFractintFunctions(type, z, q));
    else if (fractalspecific[type].flags & TRIGINPIXEL)
	return(InitFractintTrigFunctions(type, z, q));
    else if (fractalspecific[type].flags & FORMULAINPIXEL)
	return (InitFormula(z, q));
    //    else if (fractalspecific[type].flags & OTHERFNINPIXEL)
    //	return(InitOtherFunctions(type, z, q));
    else if (fractalspecific[type].per_pixel() < 0)
	return -1;
    return 0;
    }

// DD version
int	CPixel::DDInitFractal(DDComplex *z, DDComplex *q)
    {
    if (fractalspecific[type].flags & FUNCTIONINPIXEL)
	return(DDInitFunctions(type, z, q));
    else if (fractalspecific[type].flags & FRACTINTINPIXEL)
	return(DDInitFractintFunctions(type, z, q));
    else if (fractalspecific[type].flags & TRIGINPIXEL)
	return(DDInitFractintTrigFunctions(type, z, q));
    else if (fractalspecific[type].flags & FORMULAINPIXEL)
	return (DDInitFormula(z, q));
    //    else if (fractalspecific[type].flags & OTHERFNINPIXEL)
    //	return(InitOtherFunctions(type, z, q));
    //    else if (fractalspecific[type].per_pixel() < 0)
    //	return -1;
    return 0;
    }

// QD version
int	CPixel::QDInitFractal(QDComplex *z, QDComplex *q)
    {
    if (fractalspecific[type].flags & FUNCTIONINPIXEL)
	return(QDInitFunctions(type, z, q));
    else if (fractalspecific[type].flags & FRACTINTINPIXEL)
	return(QDInitFractintFunctions(type, z, q));
    else if (fractalspecific[type].flags & TRIGINPIXEL)
	return(QDInitFractintTrigFunctions(type, z, q));
    else if (fractalspecific[type].flags & FORMULAINPIXEL)
	return (QDInitFormula(z, q));
    //    else if (fractalspecific[type].flags & OTHERFNINPIXEL)
    //	return(InitOtherFunctions(type, z, q));
    else if (fractalspecific[type].per_pixel() < 0)
	return -1;
    return 0;
    }

//big version
int    CPixel::BigInitFractal(void)
    {
    BOOL    IsBig = FALSE;
    int	    i;

    for (i = 0; BigFractalSpecific[i].big_calctype; i++)
	if (type == BigFractalSpecific[i].type)	// check the list of "allowed" fractals.    
	    {
	    BigFractPtr = i;
	    IsBig = TRUE;
	    break;
	    }
    if (!IsBig)
	return -1;
    if (fractalspecific[type].flags & FUNCTIONINPIXEL)
	return(BigInitFunctions(type, &zBig, &qBig));
    else if (fractalspecific[type].flags & FRACTINTINPIXEL)
	return(BigInitFractintFunctions(type, &zBig, &qBig));
    else if (/*fractalspecific[type].flags & FRACTINTINPIXEL || */fractalspecific[type].flags & TRIGINPIXEL || fractalspecific[type].flags & FORMULAINPIXEL)    // Bignum versions not yet available
	return -1;
    else if (BigFractalSpecific[BigFractPtr].big_per_pixel() < 0)
	return -1;
    return 0;
    }

/**************************************************************************
	Run fractal
**************************************************************************/

// double version
int	CPixel::RunFractal(Complex *z, Complex *q)
    {
    if (fractalspecific[type].flags & FUNCTIONINPIXEL)
	return(RunFunctions(type, z, q, &SpecialFlag, &iteration));
    else if (fractalspecific[type].flags & FRACTINTINPIXEL)
	return(RunFractintFunctions(type, z, q, &SpecialFlag, &iteration));
    else if (fractalspecific[type].flags & TRIGINPIXEL)
	return(RunFractintTrigFunctions(type, z, q, &SpecialFlag, &iteration));
    else if (fractalspecific[type].flags & FORMULAINPIXEL)
	return (RunFormula(z, q));
//    else if (fractalspecific[type].flags & OTHERFNINPIXEL)
//	return(RunOtherFunctions(type, z, q, &SpecialFlag, iteration));
    else
	return fractalspecific[type].calctype();
    }

// DD version
int	CPixel::DDRunFractal(DDComplex *z, DDComplex *q)
    {
    if (fractalspecific[type].flags & FUNCTIONINPIXEL)
	return(DDRunFunctions(type, z, q, &SpecialFlag, &iteration));
    else if (fractalspecific[type].flags & FRACTINTINPIXEL)
	return(DDRunFractintFunctions(type, z, q, &SpecialFlag, &iteration));
    else if (fractalspecific[type].flags & TRIGINPIXEL)
	return(DDRunFractintTrigFunctions(type, z, q, &SpecialFlag, &iteration));
    else if (fractalspecific[type].flags & FORMULAINPIXEL)
	return (DDRunFormula(z, q));
    //    else if (fractalspecific[type].flags & OTHERFNINPIXEL)
    //	return(RunOtherFunctions(type, z, q, &SpecialFlag, iteration));
    else
	return fractalspecific[type].calctype();
    }

// QD version
int	CPixel::QDRunFractal(QDComplex *z, QDComplex *q)
    {
    if (fractalspecific[type].flags & FUNCTIONINPIXEL)
	return(QDRunFunctions(type, z, q, &SpecialFlag, &iteration));
    else if (fractalspecific[type].flags & FRACTINTINPIXEL)
	return(QDRunFractintFunctions(type, z, q, &SpecialFlag, &iteration));
    else if (fractalspecific[type].flags & TRIGINPIXEL)
	return(QDRunFractintTrigFunctions(type, z, q, &SpecialFlag, &iteration));
    else if (fractalspecific[type].flags & FORMULAINPIXEL)
	return (QDRunFormula(z, q));
    //    else if (fractalspecific[type].flags & OTHERFNINPIXEL)
    //	return(RunOtherFunctions(type, z, q, &SpecialFlag, iteration));
    else
	return fractalspecific[type].calctype();
    }

// Big version
int    CPixel::BigRunFractal(void)
    {
    if (fractalspecific[type].flags & FUNCTIONINPIXEL)
	return(BigRunFunctions(type, &zBig, &qBig, &SpecialFlag, &iteration));
    else if (fractalspecific[type].flags & FRACTINTINPIXEL)
	return(BigRunFractintFunctions(type, &zBig, &qBig, &SpecialFlag, &iteration));
    else
	return BigFractalSpecific[BigFractPtr].big_calctype();
    }

/**************************************************************************
	Initialise Tierazon Filters and Julia
**************************************************************************/

void CPixel::InitTierazonFiltersDouble()
    {
    if (OutsideMethod >= TIERAZONFILTERS)
	{
	TZfilter.InitFilter(OutsideMethod, threshold, dStrands, nFDOption, UseCurrentPalette);
	TZfilter.LoadFilterQ(q);
	}
    }

void CPixel::InitTierazonFiltersDD()
    {
    if (OutsideMethod >= TIERAZONFILTERS)
	{
	DDComplex DDTemp = qDD;

	Complex tempComplex;
	tempComplex.x = to_double(DDTemp.x);
	tempComplex.y = to_double(DDTemp.y);

	TZfilter.InitFilter(OutsideMethod, threshold, dStrands, nFDOption, UseCurrentPalette);
	TZfilter.LoadFilterQ(tempComplex);
	}
    }

void CPixel::InitTierazonFiltersQD()
    {
    if (OutsideMethod >= TIERAZONFILTERS)
	{
	QDComplex QDTemp = qQD;

	Complex tempComplex;
	tempComplex.x = to_double(QDTemp.x);
	tempComplex.y = to_double(QDTemp.y);

	TZfilter.InitFilter(OutsideMethod, threshold, dStrands, nFDOption, UseCurrentPalette);
	TZfilter.LoadFilterQ(tempComplex);
	}
    }

void CPixel::InitTierazonFiltersBig()
    {
    if (OutsideMethod >= TIERAZONFILTERS)
	{
	Complex tempComplex = qBig.CBig2Double();

	TZfilter.InitFilter(OutsideMethod, threshold, dStrands, nFDOption, UseCurrentPalette);
	TZfilter.LoadFilterQ(tempComplex);
	}
    }

void CPixel::SetupDoubleJulia()
    {
    if (juliaflag)
	{
	q = j;
	z = (invert) ? Invertz2T<Complex, double>(c, f_radius, f_xcenter, f_ycenter) : c;
	}
    else
	{
	q = (invert) ? Invertz2T<Complex, double>(c, f_radius, f_xcenter, f_ycenter) : c;
	z = 0;
	}
    }

void CPixel::SetupDDJulia()
    {
    if (juliaflag)
	{
	qDD.x = j.x;
	qDD.y = j.y;
	zDD = (invert) ? Invertz2T<DDComplex, dd_real>(cDD, f_radius, f_xcenter, f_ycenter) : cDD;
	}
    else
	{
	qDD = (invert) ? Invertz2T<DDComplex, dd_real>(cDD, f_radius, f_xcenter, f_ycenter) : cDD;
	zDD = 0.0;
	}
    }

void CPixel::SetupQDJulia()
    {
    if (juliaflag)
	{
	qQD.x = j.x;
	qQD.y = j.y;
	zQD = (invert) ? Invertz2T<QDComplex, qd_real>(cQD, f_radius, f_xcenter, f_ycenter) : cQD;
	}
    else
	{
	qQD = (invert) ? Invertz2T<QDComplex, qd_real>(cQD, f_radius, f_xcenter, f_ycenter) : cQD;
	zQD = 0.0;
	}
    }

void CPixel::SetupBigJulia()
    {
    if (juliaflag)
	{
	qBig.x = j.x;
	qBig.y = j.y;
	zBig = (invert) ? Invertz2T<BigComplex, BigDouble>(cBig, f_radius, f_xcenter, f_ycenter) : cBig;
	}
    else
	{
	qBig = (invert) ? Invertz2T<BigComplex, BigDouble>(cBig, f_radius, f_xcenter, f_ycenter) : cBig;
	zBig = 0.0;
	}
    }

/**************************************************************************
	Run fractal engine per pixel
**************************************************************************/

long	CPixel::dofract(HWND hwnd, int row, int col)
    {
//    int	real_iteration;				// actual count for orbit deletion

    Complex	saved;
    DDComplex	DDSaved;
    QDComplex	QDSaved;
    BigComplex	BigSaved;
    int		result;
    int		hooper = 0;
    double	close;
    double	TempZ;
    double	magnitude;
    double	min_orbit;			// orbit value closest to origin
    long	min_index;			// iteration of min_orbit
    double	tantable[16];			// used for Star Trails
    DDComplex	DDTemp;
    QDComplex	QDTemp;
    Complex	tempComplex;

    if (!BigNumFlag)
	{
	if (OutsideMethod == STARTRAIL)
	    {
	    int	i;
	    for (i = 0; i < 16; i++)
		tantable[i] = 0.0;
	    }

	InitTierazonFiltersDouble();
	SetupDoubleJulia();
	if (InitFractal(&z, &q) < 0) return BLUE;
	}
    else if (precision <= DDPRECISION)
	{
	if (cBig.x.BigDouble2DD(&cDD.x) < 0) return 0L;
	if (cBig.y.BigDouble2DD(&cDD.y) < 0) return 0L;
	InitTierazonFiltersDD();
	SetupDDJulia();
	DDInitFractal(&zDD, &qDD);
	}
    else if (precision <= QDPRECISION)
	{
	if (cBig.x.BigDouble2QD(&cQD.x) < 0) return 0L;
	if (cBig.y.BigDouble2QD(&cQD.y) < 0) return 0L;
	InitTierazonFiltersQD();
	SetupQDJulia();
	QDInitFractal(&zQD, &qQD);
	}
    else
	{
	InitTierazonFiltersBig();
	SetupBigJulia();
	if (BigInitFractal() < 0) return BLUE;
	}

// ============================================================
// PRE-LOOP RUNTIME INITIALISATION
// ============================================================

// --- iteration state ---
    iteration = 0;
    real_iteration = 0;
    FloatIteration = 0.0;

    // --- flags ---
    SpecialFlag = FALSE;
    phaseflag = 0;
    hooper = 0;

    // --- periodicity ---
    if (period_level == 0 || InsideMethod == ZMAG || OutsideMethod == STARTRAIL)
	oldcolour = 32767;
    else
	oldcolour = 240;

    savedand = 1;				// begin checking every other cycle
    savedincr = 1;				// start checking the very first time 

    close = 0.01;

    // --- periodicity state -
    saved = 0.0;
    DDSaved = 0.0;
    QDSaved = 0.0;
    BigSaved = 0.0;

    // --- orbit tracking (per arithmetic) ---
    switch (MathType)
	{
	case DOUBLEFLOAT:
	    magnitude = 0.0;
	    min_orbit = 100000.0;
	    break;

	case DOUBLEDOUBLE:
	    magnitude = 0.0;
	    min_orbit = 100000.0;
	    break;

	case QUADDOUBLE:
	    magnitude = 0.0;
	    min_orbit = 100000.0;
	    break;

	case ARBITRARYPREC:
	    magnitude = 0.0;
	    min_orbit = 100000.0;
	    break;
	}
    
    if (!BigNumFlag && distest)
	{
//	rqlim = rqlim_save;		    /* start with regular bailout */
//	if (distest != 1 || colors == 2)    /* not doing regular outside colors */
	if (distest != 1)		    /* not doing regular outside colors */
	    if (rqlim < DEM_BAILOUT)	    /* so go straight for dem bailout */
		rqlim = DEM_BAILOUT;
	deriv.x = 1;
	deriv.y = 0;
	magnitude = 0.0;
	dem_color = -1;
	}

    if (InsideMethod == BOF60 || InsideMethod == BOF61)
	{
	magnitude = 0.0;
	min_orbit = 100000.0;
	}

    if (OutsideMethod == POTENTIAL)
	rqlim = (potparam[2] > 0) ? potparam[2] : 4.0;

    for EVER
	{
	if (calcmode == 'F')
	    {
	    switch (MathType)
		{
		case DOUBLEFLOAT:
		    OlderZ = OldZ; OldZ = z; break;

		case DOUBLEDOUBLE:
		    DDOlderZ = DDOldZ; DDOldZ = zDD; break;

		case QUADDOUBLE:
		    QDOlderZ = QDOldZ; QDOldZ = zQD;  break;

		case ARBITRARYPREC:
		    BigOlderZ = BigOldZ; BigOldZ = zBig; break;
		}
	    }
	// --- iteration limit ---
	if (FloatIteration >= threshold)
	    break;

	// --- update counters ---
	iteration++;
	FloatIteration++;
	if (!BigNumFlag && distest)			// --- distance estimator path (no point implementing anywhere except double)
	    {			
	    double ftemp;
	 // Distance estimator for points near Mandelbrot set
	 // Original code by Phil Wilson, hacked around by PB
	 // Algorithms from Peitgen & Saupe, Science of Fractal Images, p.198
	    ftemp = 2 * (z.x * deriv.x - z.y * deriv.y) + 1;
	    deriv.y = 2 * (z.y * deriv.x + z.x * deriv.y);
//	    ftemp = 2 * (old.x * deriv.x - old.y * deriv.y) + 1;
//	    deriv.y = 2 * (old.y * deriv.x + old.x * deriv.y);
	    deriv.x = ftemp;
//	    if (sqr(deriv.x)+sqr(deriv.y) > dem_toobig)
//		break;
	 // if above exit taken, the later test vs dem_delta will place this point on the boundary, because mag(old)<bailout just now
	    result = RunFractal(&z, &q);
	    if (result == -1)				// divide by zero error
		return(BLUE);				// division by zero (Was Blue)
	    if (result)
		{
		if (dem_color < 0)			// note "regular" color for later
		    {
		    dem_color = iteration;
//		    dem_new = new;
		    dem_new = z;
		    }

		if (rqlim >= DEM_BAILOUT		// exit if past real bailout
		    || magnitude >= (rqlim = DEM_BAILOUT) // reset to real bailout
		    || magnitude == 0)			// exit if type doesn't "floatbailout"
		    break;
//		old = new;				// carry on till past real bailout
		}
	    }
	else						// the usual case
	    {
	    // --- arithmetic-specific fractal step ---
	    switch (MathType)
		{
		case DOUBLEFLOAT:
		    result = RunFractal(&z, &q);
		    break;

		case DOUBLEDOUBLE:
		    result = DDRunFractal(&zDD, &qDD);
		    break;

		case QUADDOUBLE:
		    result = QDRunFractal(&zQD, &qQD);
		    break;

		case ARBITRARYPREC:
		    result = BigRunFractal();
		    break;
		}

	    if (result == -1)				// divide by zero error
		return(BLUE);				// division by zero (Was Blue)
	    else if (result == 1)			// escape time
		break;
	    }

	// --- coordinate transform (double only at this stage) ---
	if (MathType == DOUBLEFLOAT)
	    {
	    TempPt = z;
	    OscProcess.ChangeCoordSystem(&z.x, &z.y, &TempZ, TempPt.x, TempPt.y, (double)iteration, CoordSystem);
	    }

	// result = 0 so continue iteration
	
	    // --- feature processing ---
	if (MathType == DOUBLEFLOAT)		// Step 1 — Double(fully wired)
	    {
	    // StarTrail
	    if (OutsideMethod == STARTRAIL)
		{
		if (iteration < 16)
		    tantable[iteration - 1] = z.y / (z.x + 0.000001);
		}

	    // EPSCROSS
	    else if (InsideMethod == EPSCROSS)
		{
		hooper = 0;

		if (fabs(z.x) < close)
		    {
		    hooper = 1; // close to y axis
		    break;
		    }

		if (fabs(z.y) < close)
		    {
		    hooper = 2; // close to x axis
		    break;
		    }
		}

	    // BOF60 / BOF61
	    else if (InsideMethod == BOF60 || InsideMethod == BOF61)
		{
		magnitude = z.CSumSqr();

		if (magnitude < min_orbit)
		    {
		    min_orbit = magnitude;
		    min_index = (long)iteration + 1L;
		    }
		}

	    // Tierazon
	    else if (OutsideMethod >= TIERAZONFILTERS)
		{
		TZfilter.DoTierazonFilter(z, &iteration);
		}
	    }
	//  Step 2 — DD
	else if (MathType == DOUBLEDOUBLE)		    //  Step 2 — DD
	    {
	    // EPSCROSS
	    if (InsideMethod == EPSCROSS)
		{
		hooper = 0;

		if (abs(zDD.x) < close)
		    {
		    hooper = 1;
		    break;
		    }

		if (abs(zDD.y) < close)
		    {
		    hooper = 2;
		    break;
		    }
		}

	    // BOF60 / BOF61
	    else if (InsideMethod == BOF60 || InsideMethod == BOF61)
		{
		magnitude = zDD.CSumSqr();

		if (magnitude < min_orbit)
		    {
		    min_orbit = magnitude;
		    min_index = (long)iteration + 1L;
		    }
		}

	    // Tierazon
	    else if (OutsideMethod >= TIERAZONFILTERS)
		{
		DDTemp = zDD;
		tempComplex;
		tempComplex.x = to_double(DDTemp.x);
		tempComplex.y = to_double(DDTemp.y);
		TZfilter.DoTierazonFilter(tempComplex, &iteration);
		}
	    }
	else if (MathType == QUADDOUBLE)			// Step 3 — QD
	    {
	    if (InsideMethod == EPSCROSS)
		{
		hooper = 0;

		if (abs(zQD.x) < close)
		    {
		    hooper = 1;
		    break;
		    }

		if (abs(zQD.y) < close)
		    {
		    hooper = 2;
		    break;
		    }
		}
	    else if (InsideMethod == BOF60 || InsideMethod == BOF61)
		{
		magnitude = zQD.CSumSqr();

		if (magnitude < min_orbit)
		    {
		    min_orbit = magnitude;
		    min_index = (long)iteration + 1L;
		    }
		}
	    else if (OutsideMethod >= TIERAZONFILTERS)
		{
		QDTemp = zQD;
		tempComplex;
		tempComplex.x = to_double(QDTemp.x);
		tempComplex.y = to_double(QDTemp.y);
		TZfilter.DoTierazonFilter(tempComplex, &iteration);
		}
	    }
	else if (MathType == ARBITRARYPREC)	    // Step 4 — Big
	    {
	    if (InsideMethod == EPSCROSS)
		{
		hooper = 0;

		Complex temp = zBig.CBig2Double();

		if (fabs(temp.x) < close)
		    {
		    hooper = 1;
		    break;
		    }

		if (fabs(temp.y) < close)
		    {
		    hooper = 2;
		    break;
		    }
		}
	    else if (InsideMethod == BOF60 || InsideMethod == BOF61)
		{
		magnitude = zBig.CSumSqr();

		if (magnitude < min_orbit)
		    {
		    min_orbit = magnitude;
		    min_index = (long)iteration + 1L;
		    }
		}
	    else if (OutsideMethod >= TIERAZONFILTERS)
		{
		tempComplex = zBig.CBig2Double();
		TZfilter.DoTierazonFilter(tempComplex, &iteration);
		}
	    }

	// --- periodicity check ---
	switch (MathType)
	    {
	    case DOUBLEFLOAT:
		{
		if (iteration > oldcolour)
		    {
		    if (!(iteration & savedand))
			{
			if (z == saved)
			    {
			    iteration = threshold;
			    break;
			    }

			saved = z;
			}
		    }
		}
		break;

	    case DOUBLEDOUBLE:
		if (iteration > oldcolour)			// check periodicity
		    {
		    if ((iteration & savedand) == 0)		// time to save a new value
			{
			DDSaved = zDD;
			if (--savedincr == 0)			// time to lengthen the periodicity?
			    {
			    savedand = (savedand << 1) + 1;	// longer periodicity
			    savedincr = 4;			// restart counter
			    }
			}
		    else		     			// check against an old save
			{
			dd_real   xAbs = DDSaved.x - zDD.x;
			dd_real   yAbs = DDSaved.y - zDD.y;

			if (abs(xAbs) < DDCloseEnough)
			    if (abs(yAbs) < DDCloseEnough)
				iteration = threshold;
			}
		    }
		break;

	    case QUADDOUBLE:
		if (iteration > oldcolour)
		    {
		    if ((iteration & savedand) == 0)		// time to save a new value
			{
			QDSaved = zQD;
			if (--savedincr == 0)			// time to lengthen the periodicity?
			    {
			    savedand = (savedand << 1) + 1;	// longer periodicity
			    savedincr = 4;			// restart counter
			    }
			}
		    else		     			// check against an old save
			{
			qd_real   xAbs = QDSaved.x - zQD.x;
			qd_real   yAbs = QDSaved.y - zQD.y;

			if (abs(xAbs) < QDCloseEnough)
			    if (abs(yAbs) < QDCloseEnough)
				iteration = threshold;
			}
		    }
		break;

	    case ARBITRARYPREC:
		// leave as-is for now (or plug original logic later)
		if (iteration > oldcolour)			// check periodicity
		    {
		    if ((iteration & savedand) == 0)		// time to save a new value
			{
			BigSaved = zBig;
			if (--savedincr == 0)			// time to lengthen the periodicity?
			    {
			    savedand = (savedand << 1) + 1;	// longer periodicity
			    savedincr = 4;			// restart counter
			    }
			}
		    else		     			// check against an old save
			{
			BigDouble   xAbs = BigSaved.x - zBig.x;
			BigDouble   yAbs = BigSaved.y - zBig.y;

			if (xAbs.BigAbs() < BigCloseEnough)
			    if (yAbs.BigAbs() < BigCloseEnough)
				iteration = threshold;
			}
		    }

		break;
	    }

	}

    // --- periodicity state update ---
    if (iteration >= threshold && period_level)
	oldcolour = 0;		// check periodicity immediately next time
    else
	oldcolour = iteration + 10;	// check when past this+10 next time

//    if (juliaflag && ShowOrbits)
//	plot_orbits(special, NUM_ORBITS);

 // --- Tierazon finalisation ---
    if (OutsideMethod >= TIERAZONFILTERS)
	{
	switch (MathType)
	    {
	    case DOUBLEFLOAT:
		TZfilter.EndTierazonFilter(z, &iteration, TrueCol);
		break;

	    case DOUBLEDOUBLE:
		tempComplex.x = to_double(zDD.x);
		tempComplex.y = to_double(zDD.y);
		TZfilter.EndTierazonFilter(tempComplex, &iteration, TrueCol);
		break;

	    case QUADDOUBLE:
		tempComplex.x = to_double(zQD.x);
		tempComplex.y = to_double(zQD.y);
		TZfilter.EndTierazonFilter(tempComplex, &iteration, TrueCol);
		break;

	    case ARBITRARYPREC:
		tempComplex = zBig.CBig2Double();
		TZfilter.EndTierazonFilter(tempComplex, &iteration, TrueCol);
		break;
	    }

	return iteration;
	}
    else if (OutsideMethod == STARTRAIL)		// --- StarTrail ---
	{
	int i;
	double diff;
	iteration = 0;
	for(i=1; i < 16; i++)
	    {
	    diff = tantable[0] - tantable[i];
	    if(fabs(diff) < 0.05)
		{
		iteration = (ExpandStarTrailColours) ? (int)((long)i * (long) threshold / 16L) : i;
		break;
		}
	    }
	}

    // --- special colour --
    if (SpecialFlag)
	iteration = (type == CUBIC) ? SPECIALINDEX : special;		// flag for special colour

    // --- distance estimation (double only) ---
    if (MathType == DOUBLEFLOAT && distest) 
	{
	double	dist, temp;

	if (iteration >= threshold)
	    return threshold;

	dist = sqr(z.x) + sqr(z.y);
	temp = log(dist);
	dist = dist * sqr(temp) / (sqr(deriv.x) + sqr(deriv.y));
	if (dist < dem_delta)		/* point is on the edge */
	    {
	    if (distest > 0)
		{
		iteration = threshold;
//		return threshold + 1;
//		goto plot_pixel;	/* no further adjustments apply */
		}
	    else
		{
//		goto plot_inside;	/* show it as an inside point */
		iteration = 0 - distest;	/* show boundary as specified color */
		if (iteration > threshold)
		    iteration = threshold;
//		goto plot_pixel;		/* no further adjustments apply */
		}
	    }
//	if (colors == 2)
//	    {
//	    color = !inside;		/* the only useful distest 2 color use */
//	    goto plot_pixel;		/* no further adjustments apply */
//	    }
	if (distest > 1)		/* pick color based on distance */
	    {
	    iteration = (long)sqrt(dist / dem_width + 1);
	    iteration &= INT_MAX;		/* oops - color can be negative */
//	    return iteration;		/* no further adjustments apply */
//	    goto plot_pixel;		/* no further adjustments apply */
	    }
//	iteration = dem_color;		/* use pixel's "regular" color */
//	new = dem_new;
//	z = dem_new;
	}



    // --- final colouring ---
    if (iteration < threshold)
	{
	switch (MathType)
	    {
	    case DOUBLEFLOAT:
		iteration = DoFilterT<Complex, double>(iteration, z, OutsideMethod, hooper, threshold, colours, decomp, biomorph,
		    rqlim, special, logval, logtable, colors, TrueCol, potparam);
		break;

	    case DOUBLEDOUBLE:
		tempComplex.x = to_double(zDD.x);
		tempComplex.y = to_double(zDD.y);

		iteration = DoFilterT<Complex, double>(iteration, tempComplex, OutsideMethod, hooper, threshold, colours, decomp, biomorph,
		    rqlim, special, logval, logtable, colors, TrueCol, potparam);
		break;

	    case QUADDOUBLE:
		tempComplex.x = to_double(zQD.x);
		tempComplex.y = to_double(zQD.y);

		iteration = DoFilterT<Complex, double>(iteration, tempComplex, OutsideMethod, hooper, threshold, colours, decomp, biomorph,
		    rqlim, special, logval, logtable, colors, TrueCol, potparam);
		break;

	    case ARBITRARYPREC:
		tempComplex = zBig.CBig2Double();

		iteration = DoFilterT<Complex, double>(iteration, tempComplex, OutsideMethod, hooper, threshold, colours, decomp, biomorph,
		    rqlim, special, logval, logtable, colors, TrueCol, potparam);
		break;
	    }
	}
    else
	{
	if (InsideMethod == BOF60)
	    iteration = (int)(sqrt(min_orbit) * 75.0);
	else if (InsideMethod == BOF61)
	    iteration = min_index;
	else
	    iteration = threshold;
	}

    // --- phase adjustments ---
    if ((type == NEWTON || type == NEWTBASIN) && subtype != 'N')
	iteration = color;

    if ((type == SPECIALNEWT || type == MATEIN) && special != 0)
	{
	if (phaseflag == 1)
	    iteration += special;
	else if (phaseflag == 2)
	    iteration += (special << 1);
	}

    // --- forward difference (placeholder / fwd diff) ---
    if (calcmode == 'F' || smoothing)
	{
	SlopeError = 1.0e-9;
	// Step 1: compute (template)
	switch (MathType)
	    {
	    case DOUBLEFLOAT:
		FloatIteration = ComputeFloatIterationT<Complex>(FloatIteration, SlopeError, z, OldZ, OlderZ, type, subtype, degree, rqlim, threshold);
		break;

	    case DOUBLEDOUBLE:
		FloatIteration = ComputeFloatIterationT<DDComplex>(FloatIteration, SlopeError, zDD, DDOldZ, DDOlderZ, type, subtype, degree, rqlim, threshold);
		break;

	    case QUADDOUBLE:
		FloatIteration = ComputeFloatIterationT<QDComplex>(FloatIteration, SlopeError, zQD, QDOldZ, QDOlderZ, type, subtype, degree, rqlim, threshold);
		break;

	    case ARBITRARYPREC:
		FloatIteration = ComputeFloatIterationT<BigComplex>(FloatIteration, SlopeError, zBig, BigOldZ, BigOlderZ, type, subtype, degree, rqlim, threshold);
		break;
	    }

	// Step 2: pixel adjustments (PUT IT HERE)
	if ((type == SPECIALNEWT || type == MATEIN) && special != 0)
	    {
	    if (phaseflag == 1)
		FloatIteration += special;
	    else if (phaseflag == 2)
		FloatIteration += (special << 1);

	    if (color > threshold)
		color = threshold;
	    }
	if ((long)FloatIteration >= threshold)
	    FloatIteration = INSIDEPIXEL;
	else if (SpecialFlag)
	    FloatIteration = SPECIALPIXEL;

	// Step 3: store
	DWORD	index = ((DWORD)row * (DWORD)width) + (DWORD)col;
	if (col >= 0 && col < xdots - 1 && row >= 0 && row < ydots - 1)
	    gManp->wpixels[index] = (float)FloatIteration;
	}
	
    return(iteration);
    }

/************************************************************************
	Calculate Fractal using a "standard" mode
************************************************************************/

long	CPixel::calc_frac(HWND hwnd, int row, int col, int user_data(HWND hwnd))
    {
    if (pairflag)							// half size screens: only do every second row / col
	if (row % pairflag || col % pairflag)
	    if (row != (int)ydots - 1)				// must trigger for last line
		return(threshold);

    if (RotationAngle == 0 || RotationAngle == 90 || RotationAngle == 180 || RotationAngle == 270)
	{
	// --- handle row change ---
	if (row != oldrow)
	    {
	    if (pairflag && row)
		draw_right_image((short)(oldrow));
	    }

	if (BigNumFlag)
	    {
	    ComputeRotatedCoordsT(row, col, oldrow, oldcol, RotationAngle, cBig.x, cBig.y, Big_xgap, Big_ygap, Big_yymax, BigHor);
	    }
	else
	    {
	    ComputeRotatedCoordsT(row, col, oldrow, oldcol, RotationAngle, c.x, c.y, xgap, ygap, yymax, hor);
	    }
	}
    else
	{
	if (BigNumFlag)
	    {
	    BigDouble  zero = 0.0;
	    BigDouble brow(row);
	    BigDouble bcol(col);
	    BigMat.DoTransformation(&cBig.x, &cBig.y, &zero, Big_xgap * bcol + BigHor, Big_yymax - Big_xgap * brow, 0.0);
	    }
	else
	    {
	    double  zero = 0.0;
	    Mat.DoTransformation(&(c.x), &(c.y), &zero, col * xgap + hor, yymax - row * ygap, 0.0);
	    }
	}

    if (user_data(hwnd) == -1)
	return(-1);

    bool useExtended = (fractalspecific[type].flags & USEDOUBLEDOUBLE);

    if (type == CIRCLESQ || type == FPCIRCLE)
	{
	color = ((int)(floor(c.CSumSqr())) & 0x00ff);
	}
    else
	{
	color = dofract(hwnd, row, col);
	}

    reset_period = 0;

    if (color > threshold && decomp <= threshold)		// for small thresholds, we can still have higher decomp levels
	color = threshold;
    if (calcmode == 'B')
	{
	if (color >= colours)				// don't use color 0 unless from inside
	    if (colours < 16)
		color &= andcolor;
	    else
		color = ((color - 1) % andcolor) + 1;  // skip color zero 
	}
/*
    if ((type == SPECIALNEWT || type == MATEIN) && special != 0)  // split colours
	{
	if (phaseflag == 1)				// second phase
	    *color += special;
	else if (phaseflag == 2)			// third phase
	    *color += (special << 1);
	}						// default first phase
*/
    
    if (*ghMutex != NULL)
	WaitForSingleObject(*ghMutex, INFINITE);  // no time-out interval
    if (_3dflag)
	projection(col, row, color);
    else if (pairflag)
	do_stereo_pairs(col, row, color);
    // we need to take symmetry into account when calculating smoothing.
    //  else if (smoothing)
    //	    {
    //	    RGBTRIPLE SmoothCol = GetSmoothedColour(FloatIteration, ColourSpeed, *TrueCol, &Plot);
    //	    Plot.OutRGBpoint(col/* + xStart*/, ydots - 1 - row, SmoothCol);
    //	    }
    else
	plot((WORD)col, (WORD)row, color);
    if (*ghMutex != NULL)
	ReleaseMutex(*ghMutex);
    return(color);
    }

/**************************************************************************
	Plot orbits
**************************************************************************/

void	CPixel::plot_orbits(RGBTRIPLE colour, int count)
    {
    int	    i, r, c;

    if (colour.rgbtBlue == 0 && colour.rgbtGreen == 0 && colour.rgbtRed == 0)
	return;										// nothing to do
    z = 0;
    for (i = 0; i < count; i++)
	{
	RunFractal(&z, &q);
	r = ydots - (int)((z.y - vert) / ygap);
	c = (int)((z.x - hor) / xgap);
	Plot.OutRGBpoint(c, r, colour);
	}
    }

/**************************************************************************
	Init Distance Estimation
**************************************************************************/

void	CPixel::InitDistEst(double *xxmin, double *xxmax, double *yymin, double *yymax, double *xx3rd, double *yy3rd, int *distestwidth, int distest)
    {
    double	ftemp;
//    double	dem_delta, dem_width;	// distance estimator variables 
//    double	dem_toobig;
//    int		/*distest = 0, */distestwidth = 71;
    double	delxx, delxx2, delyy, delyy2;

    *xxmin = hor;
    *xxmax = hor + mandel_width * ScreenRatio;
    *yymin = vert;
    *yymax = vert + mandel_width;

    *xx3rd = *xxmin;
    *yy3rd = *yymin;

    delxx = (*xxmax - *xx3rd) / (xdots - 1); // calculate stepsizes
    delyy = (*yymax - *yy3rd) / (ydots - 1);
    delxx2 = (*xx3rd - *xxmin) / (ydots - 1);
    delyy2 = (*yy3rd - *yymin) / (xdots - 1);

    dem_delta = sqr(delxx) + sqr(delyy2);
    if ((ftemp = sqr(delyy) + sqr(delxx2)) > dem_delta)
	dem_delta = ftemp;
    if (*distestwidth == 0)
	*distestwidth = 71;
    ftemp = *distestwidth;
    dem_delta *= sqr(ftemp) / 10000;		// multiply by thickness desired
    dem_width = (sqrt(sqr(*xxmax - *xxmin) + sqr(*xx3rd - *xxmin)) * ydots / xdots + sqrt(sqr(*yymax - *yymin) + sqr(*yy3rd - *yymin))) / distest;
    }
