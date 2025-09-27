/****************************************************
    PERTFILTERS.CPP a module to explore Perturbation filter functions
    Thanks to Claude Heiland-Allen https://fractalforums.org/programming/11/perturbation-code-for-cubic-and-higher-order-polynomials/2783,
    SuperHeal https://fractalforums.org/index.php?topic=5524.new#new
    and Shirom Makkad https://github.com/ShiromMakkad/MandelbrotPerturbation
    for their inspiration and methods of calculating perturbation fractals
    Written in Microsoft Visual C++ by Paul de Leeuw.
****************************************************/

#include "PertEngine.h"
#include "Potential.h"

int	CPerturbation::ColourProcessing(Complex z, long iteration, int x, int y, CTrueCol *TrueCol, CTZfilter *TZfilter, double bailout)
    {
    int		index;
    double	magnitude = 0.0;
    double	FloatIteration;
    double	rqlim2 = sqrt(bailout);
    double	log_zn, nu;
    int		SlopeIndex;
    CPlot	Plot;
    CPotential	Pot;

    Plot.InitPlot(MaxIteration, TrueCol, wpixels, xdots, height, xdots, height, Dib->BitsPerPixel, Dib, USEPALETTE);

    if (PertColourMethod != 0 && iteration < MaxIteration)		// Kalles colour method
	{
	double  iter = (double)iteration;
	if (PertColourMethod == 1)
	    iter = sqrt(iter);
	else if (PertColourMethod == 2)
	    iter = pow(iter, (double)1 / (double)3);
	else if (PertColourMethod == 3)
	    iter = log(iter);
	iteration = (int)iter;
	}
    if (IterDiv != 1.0 && iteration < MaxIteration)
	iteration = (int)((double)iteration / IterDiv);
    if (PalOffset && iteration < MaxIteration)
	iteration = (iteration + PalOffset) % TrueCol->ColoursInPALFile;
    if (wpixels)						// it's not Null, so we must have initialsed it for fwd diff slope
	{
	FloatIteration = 0.0;
	if (iteration < MaxIteration)
	    {
	    log_zn = log(z.x * z.x + z.y * z.y) / SlopeDegree;
	    nu = log(log_zn / log(SlopeDegree)) / log(SlopeDegree);
	    FloatIteration = iteration + 2 - nu;
	    }
	else
	    FloatIteration = MaxIteration;
	if (FloatIteration > MaxIteration)
	    FloatIteration = MaxIteration;
	if (FloatIteration < 0)
	    FloatIteration = 0;
	SlopeIndex = (((DWORD)height - 1 - y) * (DWORD)Dib->DibWidth) + (DWORD)x + width * thread;
	if (x >= 0 && x < Dib->DibWidth - 1 && y >= 0 && y < Dib->DibHeight - 1 && wpixels)
	    *(wpixels + SlopeIndex) = FloatIteration;
	}

    if (biomorph >= 0)						// biomorph
	{
	if (iteration == MaxIteration)
	    index = MaxIteration;
	else
	    {
	    if (fabs(z.x) < rqlim2 || fabs(z.y) < rqlim2)
		index = biomorph;
	    else
		index = iteration % 256;
	    }
	}
    else if (OutsideMethod == NONE && InsideMethod == NONE)		// no filter
	{
	if (iteration == MaxIteration)
	    index = MaxIteration;
	else
	    index = iteration;
	}
    else
	{
	switch (OutsideMethod)
	    {
	    case PERT1:						// something Shirom Makkad added
		if (iteration == MaxIteration)
		    index = MaxIteration;
		else
		    index = (int)((iteration - log2(log2(ZCoordinateMagnitudeSquared))) * 5) % 256; //Get the index of the color array that we are going to read from. 
		break;
	    case PERT2:						// something Shirom Makkad added
		if (iteration == MaxIteration)
		    index = MaxIteration;
		else
		    index = (int)(iteration - (log(0.5*(ZCoordinateMagnitudeSquared)) - log(0.5*log(256))) / log(2)) % 256;
		break;
	    case REAL:						// "real"
		if (iteration == MaxIteration)
		    index = MaxIteration;
		else
		    index = iteration + (long)z.x + 7;
		break;
	    case IMAG:	    					// "imag"
		if (iteration == MaxIteration)
		    index = MaxIteration;
		else
		    index = iteration + (long)z.y + 7;
		break;
	    case MULT:						// "mult"
		if (iteration == MaxIteration)
		    index = MaxIteration;
		else if (z.y)
		    index = (long)((double)iteration * (z.x / z.y));
		else
		    index = iteration;
		break;
	    case SUM:						// "sum"
		if (iteration == MaxIteration)
		    index = MaxIteration;
		else
		    index = iteration + (long)(z.x + z.y);
		break;
	    case POTENTIAL:
		magnitude = sqr(z.x) + sqr(z.y);
		index = Pot.potential(magnitude, iteration, MaxIteration, TrueCol, 256, potparam);
		break;
	    case ATAN:						// "atan"
		if (iteration == MaxIteration)
		    index = MaxIteration;
		else
		    index = (long)fabs(atan2(z.y, z.x)*180.0 / PI);
		break;
	    default:						// make sure we set all the inside pixels unless there is an inside filter (later on in the next switch)
		if (OutsideMethod >= TIERAZONFILTERS)		// suite of Tierazon filters and colouring schemes
		    {
		    TZfilter->EndTierazonFilter(z, (long *)&iteration, TrueCol);
		    index = iteration;
		    }
		else
		    {
		    //			if (abs(PaletteShift) <= 1)
		    if (iteration == MaxIteration)
			index = MaxIteration;
		    else
			index = iteration;
		    //			else
		    //			    index = (BYTE)(((long)(FloatIteration * abs(PaletteShift))) % 256);
		    //			index = iteration % 256;
		    }
		break;
	    }
	switch (InsideMethod)
	    {
	    case ZMAG:
		if (iteration == MaxIteration)			// Zmag
		    index = (int)((z.CSumSqr()) * (MaxIteration >> 1) + 1);
		//		    else
		//			index = iteration;
				    //			index = iteration % 256;
		break;
	    case BOF60:
		if (iteration == MaxIteration)
		    index = (int)(sqrt(min_orbit) * 75.0);
		//		    else
		//			index = iteration;
		break;
	    case BOF61:
		if (iteration == MaxIteration)
		    index = min_index;
		//		    else
		//			index = iteration;
		break;
	    default:
		break;
	    }
	}
    if (index > MaxIteration)
	index = MaxIteration;
    if (index < 0)
	index = 0;
    if (SlopeType != DERIVSLOPE || (iteration == MaxIteration && InsideMethod > 0) || EnableApproximation)
	{
	if (*PlotType == FILTERPLOT)
	    Plot.FilterPoint(x + width * thread, height - 1 - y, index, &(TZfilter->FilterRGB));
	else
	    Plot.PlotPoint(x + width * thread, height - 1 - y, index);
	}
    return 0;
    }

int	CPerturbation::ColourProcessingExp(ExpComplex ExpW, long iteration, int x, int y, CTrueCol *TrueCol, CTZfilter *TZfilter, double bailout)
    {
    int		index;
    double	magnitude = 0.0;
    double	FloatIteration = 0.0;
    double	rqlim2 = sqrt(bailout);
    double	log_zn, nu;
    int		SlopeIndex;
    CPlot	Plot;
    CPotential	Pot;

    Plot.InitPlot(MaxIteration, TrueCol, wpixels, xdots, height, xdots, height, Dib->BitsPerPixel, Dib, USEPALETTE);
    if (wpixels)						// it's not Null, so we must have initialsed it
	{
	if (iteration < MaxIteration)
	    {
	    double	SumSqW;
	    floatexp	ExpSumSqW = ExpW.x * ExpW.x + ExpW.y * ExpW.y;
	    SumSqW = ExpSumSqW.todouble();
	    log_zn = log(SumSqW) / SlopeDegree;
	    nu = log(log_zn / log(SlopeDegree)) / log(SlopeDegree);
	    FloatIteration = iteration + 2 - nu;
	    }
	else
	    FloatIteration = MaxIteration;
	SlopeIndex = (((DWORD)height - 1 - y) * (DWORD)xdots) + (DWORD)x + width * thread;
	if (x >= 0 && x < Dib->DibWidth - 1 && y >= 0 && y < Dib->DibHeight - 1 && wpixels)
	    *(wpixels + SlopeIndex) = FloatIteration;
	}

    if (biomorph >= 0)						// biomorph
	{
	if (iteration == MaxIteration)
	    index = MaxIteration;
	else
	    {
	    if (ExpW.x.abs() < rqlim2 || ExpW.y.abs() < rqlim2)
		index = biomorph;
	    else
		index = iteration % 256;
	    }
	}
    else
	{
	floatexp	temp = 0.0;
	switch (OutsideMethod)
	    {
	    case NONE:						// no filter
		if (iteration == MaxIteration)
		    index = MaxIteration;
		else
//			index = iteration;
//			index = iteration % 256;
		    index = iteration % TrueCol->ColoursInPALFile;
		break;
	    case PERT1:						// something Shirom Makkad added
		if (iteration == MaxIteration)
		    index = MaxIteration;
		else
		    {
		    ZCoordinateMagnitudeSquared = ExpZCoordinateMagnitudeSquared.todouble();
		    index = (int)((iteration - log2(log2(ZCoordinateMagnitudeSquared))) * 5) % 256; //Get the index of the color array that we are going to read from.
		    }
		break;
	    case PERT2:						// something Shirom Makkad added
		if (iteration == MaxIteration)
		    index = MaxIteration;
		else
		    {
		    ZCoordinateMagnitudeSquared = ExpZCoordinateMagnitudeSquared.todouble();
		    index = (int)(iteration - (log(0.5*(ZCoordinateMagnitudeSquared)) - log(0.5*log(256))) / log(2)) % 256;
		    }
		break;
	    case ZMAG:
		if (iteration == MaxIteration)			// Zmag
		    index = (int)((ExpW.CSumSqr()) * (MaxIteration >> 1) + 1);
		else
		    index = iteration;
		//			index = iteration % 256;
		break;
	    case REAL:						// "real"
		if (iteration == MaxIteration)
		    index = MaxIteration;
		else
		    index = iteration + (int)(ExpW.x.todouble()) + 7;
		break;
	    case IMAG:	    					// "imag"
		if (iteration == MaxIteration)
		    index = MaxIteration;
		else
		    index = iteration + (int)(ExpW.y.todouble()) + 7;
		break;
	    case MULT:						// "mult"
		if (iteration == MaxIteration)
		    index = MaxIteration;
		else if (!(temp == ExpW.y))
		    {
		    temp = ExpW.x / ExpW.y;
		    index = (long)((double)iteration * temp.todouble());
		    }
		else
		    index = iteration;
		break;
	    case SUM:						// "sum"
		if (iteration == MaxIteration)
		    index = MaxIteration;
		else
		    {
		    temp = ExpW.x + ExpW.y;
		    index = iteration + (int)(temp.todouble());
		    }
		break;
	    case ATAN:						// "atan"
		if (iteration == MaxIteration)			// silly bug in BigTrig
		    index = MaxIteration;
		else
		    {
		    Complex	w;
		    w.x = ExpW.x.todouble();
		    w.y = ExpW.y.todouble();
		    index = (long)fabs(atan2(w.y, w.x)*180.0 / PI);
		    }
		break;
	    case POTENTIAL:
		magnitude = ExpW.CSumSqr();
		index = Pot.potential(magnitude, iteration, MaxIteration, TrueCol, 256, potparam);
		break;
	    default:
		if (OutsideMethod >= TIERAZONFILTERS)			// suite of Tierazon filters and colouring schemes
		    {
		    Complex	tempComplex;

		    tempComplex.x = ExpW.x.todouble();
		    tempComplex.y = ExpW.y.todouble();
		    TZfilter->EndTierazonFilter(tempComplex, (long *)&iteration, TrueCol);
		    index = iteration;
		    }
		else						// no filter
		    {
		    if (iteration == MaxIteration)
			index = MaxIteration;
		    else
			index = iteration % 256;
		    }
		break;
	    }
	}

    if (SlopeType != DERIVSLOPE || (iteration == MaxIteration && InsideMethod > 0) || EnableApproximation)
	{
	if (*PlotType == FILTERPLOT)
	    Plot.FilterPoint(x + width * thread, height - 1 - y, index, &(TZfilter->FilterRGB));
	else
	    Plot.PlotPoint(x + width * thread, height - 1 - y, index);
	}
    return 0;
    }
