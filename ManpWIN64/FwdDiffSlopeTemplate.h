/*
    FWDDIFFSLOPETEMPLATE - a module for determining the slope using forward differencing calculations of fractals.

    Written in Microsoft Visual 'C++' by Paul de Leeuw.

     https://github.com/hrkalona/Fractal-Zoomer/blob/master/src/fractalzoomer/core/ThreadDraw.java#L3640
     https://github.com/hrkalona/Fractal-Zoomer/blob/master/src/fractalzoomer/main/app_settings/BumpMapSettings.java
*/

#include <windows.h>
#include <functional>
#include "Complex.h"
#include "ManpWIN.h"
#include "Manp.h"

#pragma once

/**************************************************************************
   Initialise Functions
**************************************************************************/

template<typename TComplex>
void InitFwdT(BYTE juliaflag, TComplex c, TComplex* z, TComplex* q, TComplex* a, TComplex* b, TComplex* v, TComplex* a2, TComplex* aa3, Complex j, int subtype, char variety, double* param)
    {
    TComplex t2, t3;

    if (juliaflag)
	{
        *q = j;
        *z = c;
	}
    else
	{
        *q = c;

        if (subtype == 4)   // Sine
            *z = param[6];

        else if (subtype == 16)  // Newton
            *z = c;

        else if (subtype == 15)  // Art Matrix
	    {
            TComplex temp;

            if (variety == 'B')
		{
                t3 = *q * 3.0;
                t2 = q->CSqr();

                *a = (t2 + 1.0) / t3;

                temp = a->CCube() * 2.0;
                *b = (t2 - 2.0) / t3 + temp;
		}
            else if (variety == 'C' || variety == 'F')
		{
                *a = *q;
                temp = q->CCube();

                if (variety == 'C')
                    *b = temp + temp + *q;
                else
		    {
                    *b = (temp - *q) * 2.0;
                    *a2 = *a + *a;
		    }
		}
            else if (variety == 'K')
		{
                *a = 0.0;
                *v = 0.0;
                *b = *q;
		}

            *aa3 = a->CSqr() * 3.0;
            *z = -(*a);
	    }
        else
	    {
            *z = 0.0;
	    }
	}
    }

/**************************************************************************
    Calculate functions
**************************************************************************/

template<typename TComplex>
void DoSlopeFwdDiffFnT(TComplex* z, TComplex* q, TComplex* b, TComplex* aa3, TComplex* zNewton, int& SlopeDegree, int subtype, char variety, WORD *degree, double* param)
    {
    TComplex sqr, temp, temp1, poly;
    TComplex unity = 1.0;
//    auto real_imag = z->x * z->y;
//    auto t = z->x * z->y;
    int k;
    int degree1, degree2;

    *degree = (int)param[5];

    switch (subtype)
	{
	case 0:						// Mandelbrot
	    {
	    SlopeDegree = 2;
	    auto real_imag = z->x * z->y;
	    sqr.x = z->x * z->x;
	    sqr.y = z->y * z->y;
	    real_imag = z->x * z->y;
	    z->x = q->x + sqr.x - sqr.y;
	    z->y = q->y + real_imag + real_imag;
	    break;
	    }

	case 1:						// Burning Ship
	    {
	    SlopeDegree = 2;
	    sqr.x = z->x * z->x;
	    sqr.y = z->y * z->y;
	    auto t = z->x * z->y;
	    auto real_imag = fabs(t);
	    z->x = sqr.x - sqr.y + q->x;
	    z->y = real_imag + real_imag - q->y;
	    break;
	    }

	case 2:						// Cubic
	    SlopeDegree = 3;
	    *z = z->CCube() + *q;
	    break;

	case 3:						// Power
	    SlopeDegree = *degree;
	    temp = 1.0;
	    for (k = 0; k < *degree - 1; k++)
		temp *= *z;
	    *z = temp * *z + *q;
	    break;

	case 4:						// Sin
	    SlopeDegree = 2;
	    if ((int)param[5] == 0)
		*z = z->CSin() * *q;
	    else
		*z = z->CSin() + *q;
	    break;

	case 5:
	    SlopeDegree = 2;
	    *z = z->CSin() + unity / *q;
	    break;

	case 6:
	    SlopeDegree = 2;
	    *z = z->CExp() + *q;
	    break;

	case 7:
	    SlopeDegree = *degree;
	    temp = 1.0;
	    for (k = 0; k < *degree - 1; k++)
		temp *= *z;
	    *z = temp * *z + unity / *q;
	    break;

	case 8:
	    degree1 = (int)param[6];
	    degree2 = (int)param[7];
	    SlopeDegree = *degree;
	    *z = z->CPolynomial(*degree) + z->CPolynomial(degree1) + z->CPolynomial(degree2) + *q;
	    break;

	case 9:
	    SlopeDegree = 3;
	    *z = z->CCube() + (*q - 1.0) * *z + *q;
	    break;

	case 10:
	    SlopeDegree = 3;
	    *z = z->CCube() / 4.0 + *z + *q;
	    break;

	case 11:					// Sin(z^n)
	    SlopeDegree = 2;
	    temp = z->CPolynomial(*degree);
	    *z = temp.CSin() + *q;
	    break;

	case 12:					// Sinh
	    SlopeDegree = 2;
	    *z = z->CSinh() + *q;
	    break;

	case 13:					// Sinh(z^n)
	    SlopeDegree = *degree;
	    temp = z->CPolynomial(*degree);
	    *z = temp.CSinh() + *q;
	    break;

	case 14:					//  z1 = conj(z)); z = z1*z1+c; [Tricorn or Mandelbar]
	    {
	    SlopeDegree = 2;
	    sqr.x = z->x * z->x;
	    sqr.y = z->y * z->y;
	    auto real_imag = z->x * z->y;
	    z->x = sqr.x - sqr.y + q->x;
	    z->y = q->y - real_imag - real_imag;
	    break;
	    }

	case 15:					// Art Matrix Cubic
	    SlopeDegree = 3;
	    if (variety == 'K')
		*z = z->CCube() + *b;
	    else
		{
		temp = z->CCube() + *b;
		*z = temp - *aa3 * *z;
		}
	    break;

	case 16:					// Newton
	    SlopeDegree = *degree;
	    temp = *z;
	    poly = z->CPolynomial(*degree - 1);
	    *z = *z - (poly * *z - *q - 1.0) / (poly * *degree);
	    *zNewton = *z - temp;
	    break;
	}
    }

/**************************************************************************
	Slope Fractal 
**************************************************************************/

template<typename TComplex, typename TReal>
int RunSlopeFwdDiffT(int user_data(HWND), Complex j, TReal mandel_width, TReal hor, TReal vert, TReal xgap, TReal ygap, double rqlim, long threshold, BYTE juliaflag, FwdDiffContext& ctx)
    {
    TComplex z, z2, c, q;
    TComplex a, b, v, a2, aa3;
    TComplex zNewton = 0.0;
    TComplex t2;

    TComplex OldZ, OlderZ;
//    TComplex OldZ2, OlderZ2;

    double iterations;
    double log_zn, nu;

    DWORD index;
    long ColourPtr;
    bool SpecialFlag = false;
    int  SlopeDegree = 2;

    double ScreenRatio = (double)ctx.xdots / (double)ctx.ydots;

    TReal temp_x = ScreenRatio / (double)(ctx.xdots - 1);
    TReal temp_y = 1.0 / (double)(ctx.ydots - 1);

    xgap = temp_x * mandel_width;
    ygap = temp_y * mandel_width;

    int totalPixels = (int)ctx.pixelOrder->size();

    PlotMode mode = currentMode;
    int chunk = (currentMode == PlotMode::Tile) ? 1024 : CHUNK_SIZE;

    const double epsilon = 1e-9;   // tweak if needed

    while (true)
	{
        int start = ctx.workIndex->fetch_add(chunk, std::memory_order_relaxed);
        if (start >= totalPixels)
            break;

        for (int k = 0; k < chunk; k++)
	    {
            int idx = start + k;
            if (idx >= totalPixels)
                break;

            int x, y;

            if (mode == PlotMode::Scanline)
		{
                y = (idx / ctx.xdots);
                x = idx % ctx.xdots;
		}
            else
		{
                x = (*ctx.pixelOrder)[idx].first;
                y = ctx.ydots - 1 - (*ctx.pixelOrder)[idx].second;
		}

            if ((idx & 0x3FF) == 0)
		gManp->UpdateProgress(ctx.workIndex, totalPixels, ctx.StatusBarInfo, ctx.NumThreads);

            if (user_data(ctx.hwnd) < 0)
                return -1;

            c.y = vert + mandel_width - y * ygap;
            c.x = hor + x * xgap;

            gPixelsDone.fetch_add(1, std::memory_order_relaxed);

	    SpecialFlag = false;

            // --- INIT ---
            InitFwdT(juliaflag, c, &z, &q, &a, &b, &v, &a2, &aa3, j, ctx.subtype, ctx.variety, ctx.param);

            // --- Forward diff companion ---
            z2 = z;
            z2.x += epsilon;

            iterations = 0.0;

            for (;;)
		{
                if (ctx.subtype == 16)
		    {
                    OlderZ = OldZ;
                    OldZ = z;
		    }

                iterations++;
                if (iterations >= threshold)
                    break;

                if (AbortRequested())
                    return -1;

                // --- CORE ITERATION ---
                DoSlopeFwdDiffFnT(&z,  &q, &b, &aa3, &zNewton, SlopeDegree, ctx.subtype, ctx.variety, ctx.degree, ctx.param);

                // --- BAILOUT (ONLY z) ---
                if (ctx.subtype == 15)
		    {
                    if (z.CSumSqr() > 100.0)
                        break;

                    if (ctx.variety == 'F')
			{
                        if (q.CSumSqr() < 0.111111)
			    {
			    SpecialFlag = true;
			    iterations = SPECIALPIXEL;
			    break;
			    }
                        v = z + a2;
			}
                    else if (ctx.variety == 'K')
                        v = z - v;
                    else
                        v = z - a;

                    if (v.CSumSqr() <= 0.000001)
			{
			SpecialFlag = true;
			iterations = SPECIALPIXEL;
                        break;
			}
		    }
                else if (ctx.subtype == 16)
		    {
                    double d = zNewton.CSumSqr();
                    if (d < MINSIZE)
                        break;
		    }
                else if (z.CSumSqr() >= rqlim)
                    break;
		}

            // --- SMOOTHING ---
            if (iterations < threshold)
		{
                if (ctx.subtype == 16)
		    {
                    double t;
                    a = OldZ - OlderZ;
                    b = z - OldZ;

                    log_zn = log(MINSIZE) - log(a.CSumSqr());
                    t = log(b.CSumSqr()) - log(a.CSumSqr());
                    nu = log_zn / t;

                    iterations = iterations + nu;
		    }
                else
		    {
                    // --- FORWARD DIFF SLOPE ---
                    TComplex delta = z2 - z;

                    double mag = sqrt((double)(delta.CSumSqr()));

                    log_zn = log(mag) / SlopeDegree;
                    nu = log(log_zn / log(SlopeDegree)) / log(SlopeDegree);

                    iterations = iterations + 1 - nu;
		    }

                if (ctx.subtype == 15 && SpecialFlag)
                    iterations = SPECIALPIXEL;
		}

            index = ((DWORD)y * (DWORD)ctx.width) + (DWORD)x;

            if (x >= 0 && x < ctx.xdots && y >= 0 && y < ctx.ydots)
		(*ctx.wpixels)[index] = (float)iterations;

            if ((long)iterations >= threshold)
                ColourPtr = threshold;
            else
		{
                if (abs(ctx.PaletteShift) <= 1)
                    ColourPtr = (long)iterations;
                else
                    ColourPtr = ((long)(iterations * ctx.PaletteShift)) % 256;
		}

            if (AbortRequested())
                return -1;
		
	    if (SpecialFlag)
		{
		ctx.Plot->OutRGBpoint(x, y, ctx.SpecialColour);
		}
	    else
		ctx.Plot->PlotPoint(x, y, ColourPtr);
	    }
	}

    return 0;
    }

