/*
    OtherFUNCTIONS.CPP a module for calculations of fractals that are non-standard 
    
    Written in Microsoft Visual 'C++' by Paul de Leeuw.

    This program is written in "standard" C. Hardware dependant code
    (console drivers & serial I/O) is in separate machine libraries.
*/

#include	"OtherFunctions.h"

/**************************************************************************
	Initialise functions for each pixel
**************************************************************************/

int	COtherFunctions::InitOtherFunctions(WORD type, Complex *z, Complex *q, HWND hwndIn, double *wpixels, CTrueCol *TrueColIn, CDib *DibIn, char *AntStatusIn, struct __timeb64 FrameEndIn, struct __timeb64 FrameStartIn)
    {
    hwnd = hwndIn;
    TrueCol = TrueColIn; 
    AntStatus = AntStatusIn; 
    FrameEnd = FrameEndIn; 
    FrameStart = FrameStartIn;
    Dib = DibIn;
    Plot.InitPlot(threshold, TrueCol, wpixels, xdots, ydots, xdots, ydots, Dib->BitsPerPixel, Dib, USEPALETTE + USEWPIXELS);

    switch (type)
	{
	case GARGOYLE:
	case CHUA:
	case CURLICUES:
	case CELLULAR:
	case ANT:
	case TOWER:
	case DEMOWALK:
	case PLASMA:
	case DIFFUSION:
	    break;		// nothing to do here

	}
    return 0;
    }

/**************************************************************************
	Run functions for each iteration
**************************************************************************/

int	COtherFunctions::RunOtherFunctions(WORD type, Complex *z, Complex *q, BYTE *SpecialFlag, long *iteration, int xdots, int ydots, double param[], long threshold, double hor, double vert, double mandel_width, double ScreenRatio, int *curpass, int *totpasses, 
		int user_data(HWND hwnd), HWND hwnd, int rotate(int dir))
    {
    switch (type)
	{
	case GARGOYLE:		//Numeric Gargoyles - Thanks to "The Loom of God" by Clifford A.Pickover pp 266 - 267.
	    {
	    double  r;						// random number between 0 and 1
	    BYTE    *c, *ch;					// pointers to arrays for holding 0 and 1 vakues
	    DWORD   colour;
	    int	    i, j, sum;
	    long    t;
	    int	    rule[10];

	    t = (long)param[0];
	    srand((unsigned)time(NULL));			// randomize things
	    for (i = 0; i < 10; i++)
		{
		rule[i] = (t % 10) ? 1 : 0;
		t /= 10;
		}

	    if ((c = new BYTE[xdots * ydots]) == NULL)
		return -1;
	    if ((ch = new BYTE[xdots * ydots]) == NULL)
		{
		delete[] c;
		return -1;
		}

	    for (i = 0; i < ydots; i++)
		for (j = 0; j < xdots; j++)
		    {
		    r = (double)rand() / 32767.0;
		    *(c + i * xdots + j) = (r < 0.5) ? 1 : 0;	// seed space with random 0s and 1s
		    }

	    for (;;)	// perform simulation based on a twisted majority rules to form gargoyle objects in 2D
		{
		for (i = 1; i < ydots - 1; i++)
		    {
		    if (user_data(hwnd) == -1)		// user pressed a key?
			return -1;
		    for (j = 1; j < xdots - 1; j++)
			{					// compute the sum of neighbouring cells
			sum =
			    *(c + (i + 1) * xdots + j + 1) +
			    *(c + (i - 1) * xdots + j - 1) +
			    *(c + (i - 1) * xdots + j + 1) +
			    *(c + (i + 1) * xdots + j - 1) +
			    *(c + (i + 1) * xdots + j + 0) +
			    *(c + (i - 1) * xdots + j + 0) +
			    *(c + (i + 0) * xdots + j + 1) +
			    *(c + (i + 0) * xdots + j - 1) +
			    *(c + (i + 0) * xdots + j + 0);

			*(ch + i * xdots + j) = (rule[sum]) ? 1 : 0;
			//		*(ch + i * xdots + j) = (sum >= 6 || sum == 4) ? 1 : 0;	// notice the "twist" in the rules that destabilises creachure boundaries
			colour = threshold * sum / 9;		// 9 is the max possible value of sum
			Plot.PlotPoint(j, i, colour);
			}
		    }
		memcpy(c, ch, xdots * ydots);
		}
	    delete[] c;
	    delete[] ch;
	    return 0;
	    }

	case CURLICUES:		// CURLICUES a module for Fractals by Dr. M. Ecker - Thanks to Marcus Rezende
	    {
	    int	    i;
	    double  d, skew;
	    double  x, y;
	    int	    x1, y1;
	    double  t, temp;
	    DWORD   colour;
	    double	xscale, yscale;
	    int	Passes = 1800000;

	    xscale = (double)(xdots - 1) / (mandel_width * ScreenRatio);
	    yscale = (double)(ydots - 1) / mandel_width;

	    Passes = (int)param[0];
	    d = param[1];
	    skew = param[2];
	    *totpasses = 20;
	    x = 0;
	    y = 0;
	    for (i = 1; i < Passes; i++)
		{
		*curpass = (int)((double)i * (double)*totpasses / (double)Passes);
		colour = threshold * (DWORD)i / Passes;
		temp = log((double)i) * d;
		t = exp(temp);
		x += sin(d * t);
		y += cos(d * t) + skew * sin(d * t);
		if (user_data(hwnd) == -1)		// user pressed a key?
		    return -1;
		x1 = (int)((x - hor) * xscale);
		y1 = (int)((vert + mandel_width - y) * yscale);
		Plot.PlotPoint(x1, y1, colour);
		}
	    return 0;
	    }

	case CHUA:		// CHUAFLIGHT "Mec-Spider's Web" a module Automatic Chua Flight Fractals - Thanks to Marcus Rezende
	    {
	    double	a, b, c;
	    int		f, g;
	    int		i;
	    double	x, y, x1, y1, z;
	    int		ix, iy;
	    double	xscale, yscale;
	    double	scale = 1;
	    int		iterations = 5000;

	    xscale = (double)(xdots - 1) / (mandel_width * ScreenRatio);
	    yscale = (double)(ydots - 1) / mandel_width;

	    iterations = (int)param[0];
	    //    scale = (int)param[1];
	    if (param[1] == 0)
		{
		srand((unsigned)time(NULL));					// Seed the random-number generator with current time 
		a = (double)rand() / (RAND_MAX);
		b = (double)rand() / (RAND_MAX);
		c = (double)rand() / (RAND_MAX);
		}
	    else
		{
		a = param[1];
		b = param[2];
		c = param[3];
		}
	    x = c * 20; y = b * 185; z = a; //REM THESE ARE THE INITIAL VALUES 'X' GOVERN THE SIZE OF THE IMAGES, here we re-use the initial variables

	// REM THESE TWO AUXILIARY VARIABLES BELOW 'f' AND 'g' WERE THE 'BIG DEAL' THAT WAS DISCOVERED BY A PROGRAMER IN ANP
	// REM THEY MADE POSSIBLE TO USE LINE ()-() INSTEAD OF THE COMPLEX INSTRUCTION LINE - STEP()

	    f = xdots / 2;
	    g = ydots / 2;
	    *totpasses = 10;
	    for (i = 0; i < iterations * 10; i++)
		{
		if (user_data(hwnd) == -1)		// user pressed a key?
		    return -1;
		*curpass = i / iterations;
		x1 = a * (y - x) + a * tan(x);			// three Chua's equations
		y1 = x1 - y + (z / 10);
		z = -(b * y1 - c / z);

		if (param[1] == 0)
		    {
		    Plot.genline((int)f, (int)g, (int)(x1 * scale + f), (int)(y1 * scale + g), ((DWORD)fabs(x1 * y1)) % threshold);
		    f = f + (int)(x1 * scale);				// new position of point
		    g = g + (int)(y1 * scale);
		    }
		else
		    {
		    ix = (int)((x - hor) * xscale);
		    iy = (int)((vert + mandel_width - y) * yscale);
		    f = (int)((x1 - hor) * xscale);
		    g = (int)((vert + mandel_width - y1) * yscale);
		    Plot.genline(f, g, ix, iy, ((DWORD)fabs(x * y)) % threshold);
		    }

		x = x1;
		y = y1;
		}

	    return 0;
	    }

	case CELLULAR:
	    /* Originally coded by Ken Shirriff. Modified beyond recognition by Jonathan Osuch.
	    Original or'd the neighborhood, changed to sum the neighborhood Changed prompts and error messages
	    Added CA types. Set the palette to some standard? CA colors
	    Changed *cell_array to near and used dstack so put_line and get_line could be used all the time
	    Made space bar generate next screen
	    Increased string/rule size to 16 digits and added CA types 9/20/92
	    */

	    {
	    int	    start_row;
	    int	    filled, notfilled;
	    WORD    cell_table[32];
	    WORD    init_string[16];
	    WORD    kr, k;
	    DWORD   lnnmbr;
	    WORD    i;
	    int	    t, t2;
	    long    randparam;
	    double  n;
	    char    buf[30];
	    char    gg[2400];

	    memset(gg, 48, 2400);
	    randparam = (long)(param[0]);
	    lnnmbr = (DWORD)(param[3]);
	    kr = (WORD)(param[2]);

	    switch (kr) 
		{
		case 21:
		case 31:
		case 41:
		case 51:
		case 61:
		case 22:
		case 32:
		case 42:
		case 23:
		case 33:
		case 24:
		case 25:
		case 26:
		case 27:
		    break;
		default:
		    abort_cellular(TYPEKR, 0);
		    return -1;
		    break;
		}

	    r = kr % 10;				// Number of nearest neighbors to sum
	    k = kr / 10;				// Number of different states, k=3 has states 0,1,2 
	    k_1 = k - 1;				// Highest state value, k=3 has highest state value of 2 
	    rule_digits = (r * 2 + 1) * k_1 + 1;	// Number of digits in the rule

	    if ((!rflag) && randparam == -1)
		--rseed;
	    if (randparam != 0 && randparam != -1) 
		{
		n = param[0];
		sprintf(buf, "%.16g", n);		// # of digits in initial string 
		t = (int)strlen(buf);
		if (t>16 || t <= 0) 
		    {
		    abort_cellular(STRING1, 0);
		    return -1;
		    }
		for (i = 0; i<16; i++)
		    init_string[i] = 0;			// zero the array 
		t2 = (int)((16 - t) / 2);
		for (i = 0; i<t; i++) 
		    {					// center initial string in array 
		    init_string[i + t2] = buf[i] - 48;	// change character to number
		    if (init_string[i + t2]>k_1) 
			{
			abort_cellular(STRING2, 0);
			return -1;
			}
		    }
		}

	    srand(rseed);
	    if (!rflag) ++rseed;

	    // generate rule table from parameter 1 
	    n = param[1];
	    if (n == 0) 
		{					// calculate a random rule 
		n = rand15() % k;
		for (i = 1; i<rule_digits; i++) 
		    {
		    n *= 10;
		    n += rand15() % k;
		    }
		param[1] = n;
		}
	    sprintf(buf, "%.*g", rule_digits, n);
	    t = (int)strlen(buf);
	    if (rule_digits < t || t < 0) 
		{					// leading 0s could make t smaller 
		abort_cellular(RULELENGTH, 0);
		return -1;
		}
	    for (i = 0; i<rule_digits; i++)		// zero the table 
		cell_table[i] = 0;
	    for (i = 0; i<t; i++) 
		{					// reverse order 
		cell_table[i] = buf[t - i - 1] - 48;	// change character to number 
		if (cell_table[i]>k_1) 
		    {
		    abort_cellular(TABLEK, 0);
		    return -1;
		    }
		}

	    start_row = 0;

	    filled = 0;
	    notfilled = 1 - filled;
		{
		if (rflag || randparam == 0 || randparam == -1) 
		    {
		    for (col = 0; col <= xdots; col++) 
			cell_array[filled][col] = rand15() % k;
		    }					// end of if random 

		else 
		    {
		    for (col = 0; col <= xdots; col++) 	// Clear from end to end
			cell_array[filled][col] = 0;
		    i = 0;
		    for (col = (xdots - 16) / 2; col<(xdots + 16) / 2; col++)	// insert initial 
			cell_array[filled][col] = init_string[i++];		// string 
		    }								// end of if not random 
		if (lnnmbr != 0)
		    lstscreenflag = 1;
		else
		    lstscreenflag = 0;
		Plot.OutputLine(xdots / 4, (WORD)start_row, (WORD)xdots-xdots / 4, (DWORD *)cell_array[filled]);
		// Pix->output_line(0, (WORD)start_row, (WORD)xdots, (DWORD *)cell_array[filled]);
		}
		start_row++;

		// This section calculates the starting line when it is not zero
		// This section can't be resumed since no screen output is generated 
		// calculates the (lnnmbr - 1) generation */
		if (lstscreenflag) 
		    {						    // line number != 0 & not resuming & not continuing 
		    for (row = start_row; row < (int)lnnmbr; row++) 
			{
			//      thinking(1,"Cellular thinking (higher start row takes longer)");
			thinking(hwnd, row);
			if (rflag || randparam == 0 || randparam == -1) 
			    {					    // Use a random border 
			    for (i = 0; i <= r; i++) 
				{
				cell_array[notfilled][i] = rand15() % k;
				cell_array[notfilled][xdots - i] = rand15() % k;
				}
			    }
			else 
			    {					    // Use a zero border 
			    for (i = 0; i <= r; i++) 
				{
				cell_array[notfilled][i] = 0;
				cell_array[notfilled][xdots - i] = 0;
				}
			    }

			t = 0;					    // do first cell 
			for (i = 0; i <= r + r; i++)
			    t += cell_array[filled][i];
			if (t>rule_digits || t<0) 
			    {
			    abort_cellular(BAD_T, t);
			    return(-1);
			    }
			cell_array[notfilled][r] = cell_table[t];

			// use a rolling sum in t
			for (col = r + 1; col< xdots - r; col++) 
			    {					    // now do the rest 
			    t = t + cell_array[filled][col + r] - cell_array[filled][col - r - 1];
			    if (t>rule_digits || t<0) 
				{
				abort_cellular(BAD_T, t);
				return(-1);
				}
			    cell_array[notfilled][col] = cell_table[t];
			    }

			filled = notfilled;
			notfilled = 1 - filled;
			if (user_data(hwnd)) 
			    {
			    abort_cellular(CELLULAR_DONE, 0);
			    return -1;
			    }
			}
		    start_row = 0;
		    lstscreenflag = 0;
		    }

		// This section does all the work 
		for (row = start_row; row <= ydots; row++) 
		    {
		    if (rflag || randparam == 0 || randparam == -1) 
			{					// Use a random border
			for (i = 0; i <= r; i++) 
			    {
			    cell_array[notfilled][i] = rand15() % k;
			    cell_array[notfilled][xdots - i] = rand15() % k;
			    }
			}
		    else 
			{					// Use a zero border 
			for (i = 0; i <= r; i++) 
			    {
			    cell_array[notfilled][i] = 0;
			    cell_array[notfilled][xdots - i] = 0;
			    }
			}

		    t = 0;					// do first cell 
		    for (i = 0; i <= r + r; i++)
			t += cell_array[filled][i];
		    if (t>rule_digits || t<0) 
			{
			abort_cellular(BAD_T, t);
			return(-1);
			}
		    cell_array[notfilled][r] = cell_table[t];

		    // use a rolling sum in t 
		    for (col = r + 1; col< xdots - r; col++) 
			{					// now do the rest 
			t = t + cell_array[filled][col + r] - cell_array[filled][col - r - 1];
			if (t>rule_digits || t<0) 
			    {
			    abort_cellular(BAD_T, t);
			    return(-1);
			    }
			cell_array[notfilled][col] = cell_table[t];
			}

		    filled = notfilled;
		    notfilled = 1 - filled;
		    Plot.OutputLine(xdots / 4, (WORD)row, (WORD)xdots - xdots / 4, (DWORD *)cell_array[filled]);
		    Plot.genline(0, row, xdots / 4, row, 0L);							// fill rest of the screen
		    Plot.genline(xdots - xdots / 4 - 1, row, xdots, row, 0L);
		    if (user_data(hwnd))
			{
			abort_cellular(CELLULAR_DONE, 0);
			return -1;
			}
		    }
		abort_cellular(CELLULAR_DONE, 0);
		return 1;
		}
	    return 0;
	    break;

	case DEMOWALK:			// standalone engine for "walk"
	    {
	    float stepsize;				// average stepsize
	    int xwalk, ywalk;				// current position
	    int xstep, ystep;				// current step
	    int steps;					// number of steps
	    int color;					// color to draw this step
	    float temp, tempadjust;			// temporary variables
	    int	count;

	    if (param[0] > 100.0 || param[0] < 0.01)
		param[0] = 1.0;

	    count = (int)param[1];			// number of steps

	    srand((unsigned)time(NULL));		// randomise things
	    if (!rflag) ++rseed;
	    tempadjust = (float)(RAND_MAX >> 2);	// adjustment factor 

	    xwalk = xdots / 2;				// start in the center of the image 
	    ywalk = ydots / 2;

	    stepsize = (float)(min(xdots, ydots) 	// calculate average stepsize 
		* (param[0] / 100.0));			// as a percentage of the image

	    color = max(0, min(colors, (int)param[1]));	// set the initial color 

	    for (steps = 0; steps < count; steps++) 
		{					// take threshold steps 
		temp = (float)rand();			// calculate the next xstep
		xstep = (int)(((temp / tempadjust) - 2.0) * stepsize);
		xstep = min(xwalk + xstep, xdots - 1);
		xstep = max(0, xstep);
		temp = (float)rand();			// calculate the next ystep 
		ystep = (int)(((temp / tempadjust) - 2.0) * stepsize);
		ystep = min(ywalk + ystep, ydots - 1);
		ystep = max(0, ystep);
		if (++color >= colors)			// rotate the colors, avoiding 
		    color = 1;				// the background color 0 
							// the draw_line function is borrowed from the 3D routines 
		Plot.genline(xwalk, ywalk, xstep, ystep, color);
		xwalk = xstep;				// remember where we were 
		ywalk = ystep;
		}
	    return(1);					// we're done 
	    }

	case PLASMA:
	    {
	    int i, k, n;
	    WORD rnd[4];

	    plasma_check = 0;
	    iparmx = (int)(param[0] * 8.0);
	    if (param[0] <= 0.0) iparmx = 16;
	    if (param[0] >= 100) iparmx = 800;

	    param[2] = 0;
	    param[3] = 0;

	    if ((!rflag) && param[2] == 1)
		--rseed;
	    if (param[2] != 0 && param[2] != 1)
		rseed = (int)(param[2]);
	    max_plasma = (WORD)param[3];		// max_plasma is used as a flag for potential 

	    if (max_plasma != 0)
		{
		max_plasma = (WORD)((1L << 16) - 1);
		}
	    srand(rseed);
	    if (!rflag) ++rseed;

	    if (colors == 256)				// set the (256-color) palette 
		set_Plasma_palette(rotate);			// skip this if < 256 colors 

	    if (colors > 16)
		shiftvalue = 18;
	    else
		{
		if (colors > 4)
		    shiftvalue = 22;
		else
		    {
		    if (colors > 2)
			shiftvalue = 24;
		    else
			shiftvalue = 25;
		    }
		}
	    if (max_plasma != 0)
		shiftvalue = 10;

	    if (max_plasma == 0)
		{
		pcolors = min(colors, max_colors);
		for (n = 0; n < 4; n++)
		    rnd[n] = 1 + (((rand15() / pcolors)*(pcolors - 1)) >> (shiftvalue - 11));
		}
	    else
		for (n = 0; n < 4; n++)
		    rnd[n] = rand16();
	    Plot.PlotPoint(0, 0, rnd[0]);
	    Plot.PlotPoint((WORD)(xdots - 1), 0, rnd[1]);
	    Plot.PlotPoint((WORD)(xdots - 1), (WORD)(ydots - 1), rnd[2]);
	    Plot.PlotPoint(0, (WORD)(ydots - 1), rnd[3]);

	    recur_level = 0;
	    if (param[1] == 0)
		subDivide(0, 0, xdots - 1, ydots - 1, user_data);
	    else
		{
		recur1 = i = k = 1;
		while (new_subD(0, 0, xdots - 1, ydots - 1, i, user_data) == 0)
		    {
		    k = k * 2;
		    if (k  >(int)max(xdots - 1, ydots - 1))
			break;
		    if (user_data(hwnd))
			{
			n = 1;
			goto done;
			}
		    i++;
		    }
		}
	    if (!user_data(hwnd))
		n = 0;
	    else
		n = 1;
	    done:
	    return(n);
	    }

	case DIFFUSION:
	    {
	    int	    xmax, ymax, xmin, ymin;	// Current maximum coordinates 
	    int	    border;			// Distance between release point and fractal 
	    int	    mode;			// Determines diffusion type:  0 = central (classic) 
						// 1 = falling particles 
						// 2 = square cavity   
	    int	    colorshift;			// If zero, select colors at random, otherwise shift the color every colorshift points 

	    int	    colorcount, currentcolor;

	    int	    i;
	    double  cosine, sine, angle;
	    int	    x, y;
	    float   r, radius/*, f_tmp*/;

	    x = y = -1;

	    border = (int)(param[0]);
	    mode = (int)(param[1]);
	    colorshift = (int)param[2];
	    colorshift = 0;
	    colorcount = colorshift;	    // Counts down from colorshift 
	    currentcolor = 1;		    // Start at color 1 (color 0 is probably invisible)

	    if (mode > 2)
		mode = 0;

	    if (border <= 0)
		border = 10;

	    srand(rseed);
	    if (!rflag) ++rseed;

	    if (mode == 0) 
		{
		xmax = xdots / 2 + border;  // Initial box
		xmin = xdots / 2 - border;
		ymax = ydots / 2 + border;
		ymin = ydots / 2 - border;
		}
	    if (mode == 1) 
		{
		xmax = xdots / 2 + border;  // Initial box 
		xmin = xdots / 2 - border;
		ymin = ydots - border;
		}
	    if (mode == 2) 
		{
		if (xdots>ydots)
		    radius = (float)(ydots - border);
		else
		    radius = (float)(xdots - border);
		}

	    if (mode == 0)		    // Single seed point in the center 
		Plot.PlotPoint((WORD)(xdots / 2), (WORD)(ydots / 2), currentcolor);  // Seed point

	    if (mode == 1)		    // Line along the bottom 
		for (i = 0; i <= xdots; i++)
		    Plot.PlotPoint((WORD)i, (WORD)(ydots - 3), currentcolor);

	    if (mode == 2)		    // Large square that fills the screen 
		{
		if (xdots>ydots) 
		    {
		    for (i = 0; i<ydots; i++) 
			{
			Plot.PlotPoint((WORD)(xdots / 2 - ydots / 2), (WORD)i, currentcolor);
			Plot.PlotPoint((WORD)(xdots / 2 + ydots / 2), (WORD)i, currentcolor);
			Plot.PlotPoint((WORD)(xdots / 2 - ydots / 2 + i), 3, currentcolor);
			Plot.PlotPoint((WORD)(xdots / 2 - ydots / 2 + i), (WORD)(ydots - 3), currentcolor);
			}
		    }
		else 
		    {
		    for (i = 0; i<xdots; i++) 
			{
			Plot.PlotPoint(0, (WORD)(ydots / 2 - xdots / 2 + i), currentcolor);
			Plot.PlotPoint((WORD)(xdots - 1), (WORD)(ydots / 2 - xdots / 2 + i), currentcolor);
			Plot.PlotPoint((WORD)i, (WORD)(ydots / 2 - xdots / 2), currentcolor);
			Plot.PlotPoint((WORD)i, (WORD)(ydots / 2 + xdots / 2), currentcolor);
			}
		    }
		}

	    while (1)
		{
		if (mode == 0)			// Release new point on circle just inside the box
		    {
		    angle = 2 * (double)rand() / (RAND_MAX / PI);
		    FPUsincos(&angle, &sine, &cosine);
		    x = (int)(cosine*(xmax - xmin) + xdots);
		    y = (int)(sine  *(ymax - ymin) + ydots);
		    x = x >> 1;			// divide by 2
		    y = y >> 1;
		    }
		if (mode == 1)			// Release new point on the line ymin somewhere between xmin and xmax
		    {
		    y = ymin;
		    x = RANDOM(xmax - xmin) + (xdots - xmax + xmin) / 2;
		    }
		if (mode == 2)			// Release new point on a circle inside the box with radius given by the radius variable
		    {
		    angle = 2 * (double)rand() / (RAND_MAX / PI);
		    FPUsincos(&angle, &sine, &cosine);
		    x = (int)(cosine*radius + xdots);
		    y = (int)(sine  *radius + ydots);
		    x = x >> 1;
		    y = y >> 1;
		    }
		    // Loop as long as the point (x,y) is surrounded by color 0 
		    // on all eight sides                                       
		while ((Plot.GetColour((WORD)(x + 1), (WORD)(y + 1)) == 0) && (Plot.GetColour((WORD)(x + 1), (WORD)y) == 0) &&
		    (Plot.GetColour((WORD)(x + 1), (WORD)(y - 1)) == 0) && (Plot.GetColour((WORD)x, (WORD)(y + 1)) == 0) &&
		    (Plot.GetColour((WORD)x, (WORD)(y - 1)) == 0) && (Plot.GetColour((WORD)(x - 1), (WORD)(y + 1)) == 0) &&
		    (Plot.GetColour((WORD)(x - 1), (WORD)y) == 0) && (Plot.GetColour((WORD)(x - 1), (WORD)(y - 1)) == 0))
		    {
		    // Erase moving point 
// come back		    if (show_orbit)
//		    Pix->PlotPixel((WORD)x, (WORD)y, 0);

		    if (mode == 0) 		    // Make sure point is inside the box 
			{
			if (x == xmax)
			    x--;
			else if (x == xmin)
			    x++;
			if (y == ymax)
			    y--;
			else if (y == ymin)
			    y++;
			}

		    if (mode == 1)	// Make sure point is on the screen below ymin, but we need a 1 pixel margin because of the next random step.
			{
			if (x>xdots - 1)
			    x--;
			else if (x<=1)
			    x++;
			if (y<ymin)
			    y++;
			}

		    // Take one random step 
		    x += RANDOM(3) - 1;
		    y += RANDOM(3) - 1;

		    // Check keyboard
		    if ((++plasma_check & 0x7f) == 1)
			if (user_data(hwnd))
			    {
			    plasma_check--;
			    return -1;
			    }

		    // Show the moving point
// come back		    if (show_orbit)
//		    Pix->PlotPixel((WORD)x, (WORD)y, RANDOM(colors - 1) + 1);

		    }			// End of loop, now fix the point 

		// If we're doing colorshifting then use currentcolor, otherwise pick one at random 

		Plot.PlotPoint((WORD)x, (WORD)y, colorshift ? currentcolor : RANDOM(colors - 1) + 1);

		// If we're doing colorshifting then check to see if we need to shift
		if (colorshift) 
		    {
		    if (!--colorcount) 
			{				// If the counter reaches zero then shift
			currentcolor++;			// Increase the current color and wrap 
			currentcolor %= colors;		// around skipping zero 
			if (!currentcolor) currentcolor++;
			colorcount = colorshift;	// and reset the counter 
			}
		    }

		// Is point too close to the edge?
		// If the new point is close to an edge, we may need to increase some limits so that the limits expand to match the growing fractal.

		if (mode == 0)
		    {
		    if (((x + border)>xmax) || ((x - border)<xmin) || ((y - border)<ymin) || ((y + border)>ymax))
			{
			// Increase box size, but not past the edge of the screen
			if (ymin != 1)
			    {
			    ymin--;
			    ymax++;
			    }
			if (xmin != 1)
			    {
			    xmin--;
			    xmax++;
			    }
			if ((ymin == 1) || (xmin == 1))
			    return 0;
			}
		    }
		if (mode == 1)			// Decrease ymin, but not past top of screen 
		    {
		    if (y - border < ymin)
			ymin--;
		    if (ymin == 0)
			return 0;
		    }
		if (mode == 2)			// Decrease the radius where points are released to stay away from the fractal.It might be decreased by 1 or 2
		    {
		    r = sqr((float)x - xdots / 2) + sqr((float)y - ydots / 2);
		    if (r <= border*border)
			return 0;
		    while ((radius - border)*(radius - border) > r)
			radius--;
		    }
		}
	    return 0;
	    }

    	case	ANT:
	    ant();
	    return 0;

	case	TOWER:
	    tower();
	    return 0;

	}


    return 0;
    }

/**************************************************************************
	Error Messages for Cellular
**************************************************************************/

void	COtherFunctions::abort_cellular(int err, int t)
    {
    int i;
    switch (err)
	{
	case BAD_T:
	     {
	     char msg[30];
	     sprintf(msg,"Bad t=%d, aborting\n", t);
	     MessageBox (hwnd, msg, "View", MB_ICONEXCLAMATION | MB_OK);
	     MessageBeep (0);
	     }
	     break;
	case BAD_MEM:
	     {
	     char msg[]={"Insufficient free memory for calculation" };
	     MessageBox (hwnd, msg, "View", MB_ICONEXCLAMATION | MB_OK);
	     MessageBeep (0);
	     }
	     break;
	case STRING1:
	     {
	     char msg[]={"String can be a maximum of 16 digits" };
	     MessageBox (hwnd, msg, "View", MB_ICONEXCLAMATION | MB_OK);
	     MessageBeep (0);
	     }
	     break;
	case STRING2:
	     {
	     char msg[]={"Make string of 0's through  's" };
	     msg[27] = k_1 + 48; /* turn into a character value */
	     MessageBox (hwnd, msg, "View", MB_ICONEXCLAMATION | MB_OK);
	     MessageBeep (0);
	     }
	     break;
	case TABLEK:
	     {
	     char msg[]={"Make Rule with 0's through  's" };
	     msg[27] = k_1 + 48; /* turn into a character value */
	     MessageBox (hwnd, msg, "View", MB_ICONEXCLAMATION | MB_OK);
	     MessageBeep (0);
	     }
	     break;
	case TYPEKR:
	     {
	     char msg[]={"Type must be 21, 31, 41, 51, 61, 22, 32, 42, 23, 33, 24, 25, 26, 27" };
	     MessageBox (hwnd, msg, "View", MB_ICONEXCLAMATION | MB_OK);
	     MessageBeep (0);
	     }
	     break;
	case RULELENGTH:
	     {
	     char msg[]={"Rule must be    digits long" };
	     i = rule_digits / 10;
	     if(i==0)
		msg[14] = rule_digits + 48;
	     else {
		msg[13] = i;
		msg[14] = rule_digits % 10 + 48;
	     }
         MessageBox (hwnd, msg, "View", MB_ICONEXCLAMATION | MB_OK);
         MessageBeep (0);
         }
         break;
      case CELLULAR_DONE:
         break;
   }
}

/**************************************************************************
	Display Thinking
**************************************************************************/

void	COtherFunctions::thinking(HWND hwnd, int count)

    {
    static	int	thinkstate, slow = 0;
    static	char	wheel[] = { '-', '\\', '|', '/' };
    char		s[100];

    if (slow++ % 40 == 0)
	{
	thinkstate = (thinkstate + 1) & 3;
	wsprintf(s, "Thinking. Level = <%d> %c", count, wheel[thinkstate]);
	SetWindowText(hwnd, s);
	}
    }

/**************************************************************************
	Returns a random 16 bit value that is never 0
**************************************************************************/

WORD COtherFunctions::rand16()
    {
    WORD value;
    value = rand15();
    value <<= 1;
    value += rand15() & 1;
    if (value < 1)
	value = 1;
    return(value);
    }

/**************************************************************************
	A few Plasma Routines
**************************************************************************/

int	COtherFunctions::new_subD(int x1, int y1, int x2, int y2, int recur, int user_data(HWND hwnd))
    {
    int	    x, y;
    int	    nx1;
    int	    nx;
    int	    ny1, ny;
    long    i, v;

    struct sub 
	{
	BYTE	t;	// top of stack 
	int	v[16];	// subdivided value 
	BYTE	r[16];	// recursion level 
	};

    struct sub subx, suby;

    recur1 = 320L >> recur;
    suby.t = 2;
    ny = suby.v[0] = y2;
    ny1 = suby.v[2] = y1;
    suby.r[0] = suby.r[2] = 0;
    suby.r[1] = 1;
    y = suby.v[1] = (ny1 + ny) >> 1;

    while (suby.t >= 1)
	{
	if ((++plasma_check & 0x0f) == 1)
	    if (user_data(hwnd))
		{
		plasma_check--;
		return(1);
		}
	while (suby.r[suby.t - 1] < recur)
	    {
	    //     1.  Create new entry at top of the stack 
	    //     2.  Copy old top value to new top value. This is largest y value.         
	    //     3.  Smallest y is now old mid point     
	    //     4.  Set new mid point recursion level   
	    //     5.  New mid point value is average of largest and smallest          

	    suby.t++;
	    ny1 = suby.v[suby.t] = suby.v[suby.t - 1];
	    ny = suby.v[suby.t - 2];
	    suby.r[suby.t] = suby.r[suby.t - 1];
	    y = suby.v[suby.t - 1] = (ny1 + ny) >> 1;
	    suby.r[suby.t - 1] = (int)max(suby.r[suby.t], suby.r[suby.t - 2]) + 1;
	    }
	subx.t = 2;
	nx = subx.v[0] = x2;
	nx1 = subx.v[2] = x1;
	subx.r[0] = subx.r[2] = 0;
	subx.r[1] = 1;
	x = subx.v[1] = (nx1 + nx) >> 1;

	while (subx.t >= 1)
	    {
	    while (subx.r[subx.t - 1] < recur)
		{
		subx.t++;				    // move the top ofthe stack up 1
		nx1 = subx.v[subx.t] = subx.v[subx.t - 1];
		nx = subx.v[subx.t - 2];
		subx.r[subx.t] = subx.r[subx.t - 1];
		x = subx.v[subx.t - 1] = (nx1 + nx) >> 1;
		subx.r[subx.t - 1] = (int)max(subx.r[subx.t],
		    subx.r[subx.t - 2]) + 1;
		}

	    if ((i = Plot.GetColour((WORD)nx, (WORD)y)) == 0)
		i = adjust(nx, ny1, nx, y, nx, ny);
	    v = i;
	    if ((i = Plot.GetColour((WORD)x, (WORD)ny)) == 0)
		i = adjust(nx1, ny, x, ny, nx, ny);
	    v += i;
	    if (Plot.GetColour((WORD)x, (WORD)y) == 0)
		{
		if ((i = Plot.GetColour((WORD)x, (WORD)ny1)) == 0)
		    i = adjust(nx1, ny1, x, ny1, nx, ny1);
		v += i;
		if ((i = Plot.GetColour((WORD)nx1, (WORD)y)) == 0)
		    i = adjust(nx1, ny1, nx1, y, nx1, ny);
		v += i;
		Plot.PlotPoint((WORD)x, (WORD)y, (WORD)((v + 2) >> 2));
		}

	    if (subx.r[subx.t - 1] == recur) subx.t = subx.t - 2;
	    }

	if (suby.r[suby.t - 1] == recur) suby.t = suby.t - 2;
	}
    return(0);
    }

void  COtherFunctions::subDivide(int x1, int y1, int x2, int y2, int user_data(HWND hwnd))
    {
    int x, y;
    long v, i;
    if ((++plasma_check & 0x7f) == 1)
	if (user_data(hwnd))
	    {
	    plasma_check--;
	    return;
	    }
    if (x2 - x1<2 && y2 - y1<2)
	return;
    recur_level++;
    recur1 = 320L >> recur_level;

    x = (x1 + x2) >> 1;
    y = (y1 + y2) >> 1;
    if ((v = Plot.GetColour((WORD)x, (WORD)y1)) == 0)
	v = adjust(x1, y1, x, y1, x2, y1);
    i = v;
    if ((v = Plot.GetColour((WORD)x2, (WORD)y)) == 0)
	v = adjust(x2, y1, x2, y, x2, y2);
    i += v;
    if ((v = Plot.GetColour((WORD)x, (WORD)y2)) == 0)
	v = adjust(x1, y2, x, y2, x2, y2);
    i += v;
    if ((v = Plot.GetColour((WORD)x1, (WORD)y)) == 0)
	v = adjust(x1, y1, x1, y, x1, y2);
    i += v;

    if (Plot.GetColour((WORD)x, (WORD)y) == 0)
	Plot.PlotPoint((WORD)x, (WORD)y, (WORD)((i + 2) >> 2));

    subDivide(x1, y1, x, y, user_data);
    subDivide(x, y1, x2, y, user_data);
    subDivide(x, y, x2, y2, user_data);
    subDivide(x1, y, x, y2, user_data);
    recur_level--;
    }

WORD	COtherFunctions::adjust(int xa, int ya, int x, int y, int xb, int yb)
    {
    long pseudorandom;
    pseudorandom = ((long)iparmx)*((rand15() - 16383));
    /*   pseudorandom = pseudorandom*(abs(xa-xb)+abs(ya-yb));*/
    pseudorandom = pseudorandom * recur1;
    pseudorandom = pseudorandom >> shiftvalue;
    pseudorandom = (((long)Plot.GetColour((WORD)xa, (WORD)ya) + (long)Plot.GetColour((WORD)xb, (WORD)yb) + 1) >> 1) + pseudorandom;
    if (max_plasma == 0)
	{
	if (pseudorandom >= pcolors)
	    pseudorandom = pcolors - 1;
	}
    else if (pseudorandom >= max_plasma)
	pseudorandom = max_plasma;
    if (pseudorandom < 1)
	pseudorandom = 1;
    Plot.PlotPoint((WORD)x, (WORD)y, (WORD)pseudorandom);
    return((WORD)pseudorandom);
    }

void	COtherFunctions::set_Plasma_palette(int rotate(int direction))
    {
    RGBTRIPLE	Red = { 255, 0, 0 };
    RGBTRIPLE	Green = { 0,255, 0 };
    RGBTRIPLE	Blue = { 0, 0,255 };
    int i;

    TrueCol->PalettePtr[0] = 0;
    TrueCol->PalettePtr[1] = 0;
    TrueCol->PalettePtr[2] = 0;
    for (i = 1; i <= 85; i++)
	{
	TrueCol->PalettePtr[i * 3 + 0] = (i*Green.rgbtRed + (86 - i)*Blue.rgbtRed) / 85;
	TrueCol->PalettePtr[i * 3 + 1] = (i*Green.rgbtGreen + (86 - i)*Blue.rgbtGreen) / 85;
	TrueCol->PalettePtr[i * 3 + 2] = (i*Green.rgbtBlue + (86 - i)*Blue.rgbtBlue) / 85;

	TrueCol->PalettePtr[i * 3 + 0 + 255] = (i*Red.rgbtRed + (86 - i)*Green.rgbtRed) / 85;
	TrueCol->PalettePtr[i * 3 + 1 + 255] = (i*Red.rgbtGreen + (86 - i)*Green.rgbtGreen) / 85;
	TrueCol->PalettePtr[i * 3 + 2 + 255] = (i*Red.rgbtBlue + (86 - i)*Green.rgbtBlue) / 85;

	TrueCol->PalettePtr[i * 3 + 0 + 510] = (i*Blue.rgbtRed + (86 - i)*Red.rgbtRed) / 85;
	TrueCol->PalettePtr[i * 3 + 0 + 510] = (i*Blue.rgbtGreen + (86 - i)*Red.rgbtGreen) / 85;
	TrueCol->PalettePtr[i * 3 + 0 + 510] = (i*Blue.rgbtBlue + (86 - i)*Red.rgbtBlue) / 85;
	}
    rotate(1);
    }

/**************************************************************************
	A few Diffusion Routines
**************************************************************************/

void	COtherFunctions::FPUsincos(double *Angle, double *Sin, double *Cos)
    {
    *Sin = sin(*Angle);
    *Cos = cos(*Angle);
    }

