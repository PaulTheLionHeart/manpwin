/*
   PASCAL.CPP a module for Pascal Fractals
   Thanks to Marcus Rezende 

   Written in Microsoft Visual C++ by Paul de Leeuw.
*/

/**************************************************************************
    Pascal Triangle
**************************************************************************/

#include "OtherFunctions.h"

#define		PASCALSIZE	240
#define		TRIANGLESIZE	80
#define		ABS(a)		((a >= 0) ? a : -a)

int	COtherFunctions::DoPascal()

    {
    int		i, j;
    unsigned	__int64	a[PASCALSIZE];			// use 64 bit unsigned integers for maximum significant figures
    unsigned	__int64	b[PASCALSIZE];
		__int64	c[TRIANGLESIZE][TRIANGLESIZE];

    if (lo > threshold)
	lo = threshold - 1;
    if (hi > threshold)
	hi = threshold - 1;

    if (subtype == 'A')
	{
	if (NumberIterations > PASCALSIZE - 1)
	    NumberIterations = PASCALSIZE - 1;
	for (i = 0; i < PASCALSIZE; i++)
	    {
	    a[i] = 0;					// initialise arrays
	    b[i] = 0;
	    }
	}
    else
	{
	if (NumberIterations > TRIANGLESIZE - 1)
	    NumberIterations = TRIANGLESIZE - 1;
	for (i = 0; i < TRIANGLESIZE; i++)
	    for (j = 0; j < TRIANGLESIZE; j++)
		c[i][j] = 0;
	}
    switch (subtype)
	{
	case 'A':					// Pascal
	    a[1] = 1;
	    a[2] = 0;
	    for (i = 2; i <= NumberIterations; i++)
		{
		for (j = 2; j <= NumberIterations; j++)
		    {
		    b[j] = a[j - 1] + a[j];
		    if (b[j])
			Plot.DisplayFilledCircle(j * CircleSize + xdots / 2 - i * CircleSize / 2, ydots / 2 - CircleSize * NumberIterations / 2 + i * CircleSize, CircleSize / 2, (b[j] % moda == 0) ? lo : hi);
		    }
		memcpy(&a,&b, sizeof(__int64)*(NumberIterations +1));	// swap a and b arrays
		}
	    break;
	case 'B':   					// Bell	http://en.wikipedia.org/wiki/Bell_number
	    c[1][1] = 1;
	    c[2][1] = 1;
	    c[2][2] = 2;
	    for (i = 2; i <= NumberIterations; i++)
		{
		c[i][1] = c[i - 1][i - 1];
//		DisplayFilledCircle(CircleSize + xdots / 2 - i * CircleSize / 2, ydots / 2 - CircleSize * n / 2 + i * CircleSize, CircleSize / 2, (c[i][1] % moda == 0) ? lo : hi);
		for (j = 2; j <= i; j++)
		    {
		    c[i][j] = c[i][j - 1] + c[i - 1][j - 1];
		    Plot.DisplayFilledCircle(xdots / 2 + i * CircleSize / 2 - j * CircleSize, ydots / 2 - CircleSize * NumberIterations / 2 + i * CircleSize, CircleSize / 2, (c[i][j] % moda == 0) ? lo : hi);
		    }
		}
	    break;
	case 'C':   					// Catalan
	    c[1][1] = 1;
	    c[2][1] = 1;
	    c[2][2] = 2;
	    for (i = 2; i <= NumberIterations; i++)
		{
		c[i][1] = 1;
//		DisplayFilledCircle(CircleSize + xdots / 2 - i * CircleSize / 2, ydots / 2 - CircleSize * n / 2 + i * CircleSize, CircleSize / 2, (c[i][1] % moda == 0) ? lo : hi);
		for (j = 2; j <= i; j++)
		    {
		    c[i][j] = c[i][j - 1] + c[i - 1][j];
		    Plot.DisplayFilledCircle(xdots / 2 + i * CircleSize / 2 - j * CircleSize, ydots / 2 - CircleSize * NumberIterations / 2 + i * CircleSize, CircleSize / 2, (c[i][j] % moda == 0) ? lo : hi);
		    }
		}
	    break;
	case 'D':   					// Dudley	http://en.wikipedia.org/wiki/Dudley_triangle
	    Plot.DisplayFilledCircle(xdots / 2 + CircleSize / 2 - CircleSize, ydots / 2 - CircleSize * NumberIterations / 2 * CircleSize, CircleSize / 2, hi);
	    c[1][1] = 2;
	    c[2][1] = 2;
	    c[2][2] = 2;
	    for (i = 1; i <= NumberIterations; i++)
		{
//		c[i][1] = c[i - 1][i - 1];
//		DisplayFilledCircle(CircleSize + xdots / 2 - i * CircleSize / 2, ydots / 2 - CircleSize * n / 2 + i * CircleSize, CircleSize / 2, (c[i][1] % moda == 0) ? lo : hi);
		for (j = 1; j <= i; j++)
		    {
		    c[i][j] = j * (j + 1) % (i + 2);
		    Plot.DisplayFilledCircle(xdots / 2 + i * CircleSize / 2 - j * CircleSize, ydots / 2 - CircleSize * NumberIterations / 2 + i * CircleSize, CircleSize / 2, (c[i][j] % moda == 0) ? lo : hi);
		    }
		}
	    break;
	case 'E':   					// Floyd	
	    Plot.DisplayFilledCircle(xdots / 2 + CircleSize / 2 - CircleSize, ydots / 2 - CircleSize * NumberIterations / 2 * CircleSize, CircleSize / 2, hi);
	    c[1][1] = 2;
	    c[2][1] = 2;
	    c[2][2] = 2;
	    for (i = 1; i <= NumberIterations; i++)
		{
		c[i][1] = c[i - 1][i - 1] + 1;
//		DisplayFilledCircle(CircleSize + xdots / 2 - i * CircleSize / 2, ydots / 2 - CircleSize * n / 2 + i * CircleSize, CircleSize / 2, (c[i][1] % moda == 0) ? lo : hi);
		for (j = 1; j <= i; j++)
		    {
		    c[i][j] = c[i][j - 1] + 1;
		    Plot.DisplayFilledCircle(xdots / 2 + i * CircleSize / 2 - j * CircleSize, ydots / 2 - CircleSize * NumberIterations / 2 + i * CircleSize, CircleSize / 2, (c[i][j] % moda == 0) ? lo : hi);
		    }
		}
	    break;
	case 'F':   					// Hosoya	http://en.wikipedia.org/wiki/Hosoya%27s_triangle
	    c[1][1] = 1;
	    c[1][2] = 0;
	    c[1][3] = 0;
	    c[2][1] = 1;
	    c[2][2] = 1;
	    c[2][3] = 0;
	    for (i = 2; i <= NumberIterations; i++)
		{
//		c[i][1] = c[i - 1][i - 1];
//		DisplayFilledCircle(CircleSize + xdots / 2 - i * CircleSize / 2, ydots / 2 - CircleSize * n / 2 + i * CircleSize, CircleSize / 2, (c[i][1] % moda == 0) ? lo : hi);
		for (j = 3; j <= i; j++)
		    {
		    c[i][j] = c[i - 1][j] + c[i - 2][j];
    		    if (c[i][j] == 0)
			c[i][j] = c[i - 1][j - 1] + c[i - 2][j - 2];

		    Plot.DisplayFilledCircle(xdots / 2 + i * CircleSize / 2 - j * CircleSize, ydots / 2 - CircleSize * NumberIterations / 2 + i * CircleSize, CircleSize / 2, (c[i][j] % moda == 0) ? lo : hi);
		    }
		}
	    break;
	case 'G':   					// Leibniz	http://en.wikipedia.org/wiki/Leibniz_triangle
	    double	d[TRIANGLESIZE][TRIANGLESIZE];

	    d[1][1] = 1;
	    d[2][1] = 0.5;
	    d[2][2] = 0.5;

	    c[1][1] = (__int64)(1 / d[1][1]);
	    c[2][1] = (__int64)(1 / d[2][1]);
	    c[2][2] = (__int64)(1 / d[2][2]);
	    for (i = 2; i <= NumberIterations; i++)
		{
		d[i][1] = 1.0 / i;
		c[i][1] = (__int64)(1 / d[i][1] + 0.5);
		for (j = 2; j <= i; j++)
		    {
		    d[i][j] = fabs(d[i - 1][j - 1] - d[i][j - 1]);
		    c[i][j] = (__int64)(1 / d[i][j] + 0.5);
//		    DisplayFilledCircle(xdots / 2 + i * CircleSize / 2 - j * CircleSize, ydots / 2 - CircleSize * n / 2 + i * CircleSize, CircleSize / 2, (c[i][j] % moda == 0) ? lo : hi);
		    Plot.DisplayFilledCircle(xdots / 2 - i * CircleSize / 2 + j * CircleSize, ydots / 2 - CircleSize * NumberIterations / 2 + i * CircleSize, CircleSize / 2, (c[i][j] % moda == 0) ? lo : hi);
		    }
		}
	    break;
	case 'H':   					// Lozanic	http://en.wikipedia.org/wiki/Lozani%C4%87%27s_triangle
	    c[0][0] = 1;
	    c[1][1] = 1;
	    for (i = 1; i <= NumberIterations; i++)
		{
		c[i][0] = 1;
//		DisplayFilledCircle(CircleSize + xdots / 2 - i * CircleSize / 2, ydots / 2 - CircleSize * n / 2 + i * CircleSize, CircleSize / 2, (c[i][1] % moda == 0) ? lo : hi);
		for (j = 1; j <= i; j++)
		    {
		    c[i][j] = c[i - 1][j - 1] + c[i - 1][j];
//IF r / 2 = r \ 2 AND k / 2 > k \ 2 THEN GOTO 8 ELSE GOTO 9
		    if (i % 2 == 0 && j % 2 != 0)
			{
			int	l;
			double	numerator, denominator1, denominator2, pronum, prodem1, prodem2;

			numerator = i / 2 - 1;
			denominator1 = (j - 1) / 2;
			denominator2 = numerator - denominator1;

			pronum = 1;			    // factorial of numerator
			for (l = 1; l <= numerator; l++)
			    pronum *= l;

			prodem1 = 1;			    // factorial of denominator1
			for (l = 1; l <= denominator1; l++)
			    prodem1 *= l;

			prodem2 = 1;			    // factorial of denominator2
			for (l = 1; l <= denominator1; l++)
			    prodem2 *= l;
			c[i][j] -= (__int64)(pronum / prodem1 / prodem2);
			}
//		    else
		    Plot.DisplayFilledCircle(xdots / 2 + i * CircleSize / 2 - j * CircleSize, ydots / 2 - CircleSize * NumberIterations / 2 + i * CircleSize, CircleSize / 2, (c[i][j] % moda == 0) ? lo : hi);
		    }
		}
	    break;
	case 'I':   					// Stirling (1st)	http://www.jstor.org/discover/10.2307/3026928?uid=3737536&uid=2129&uid=2&uid=70&uid=4&sid=21102292820281
	    c[0][0] = 1;
	    c[1][0] = 0;
	    c[1][1] = 1;
	    c[2][1] = -1;
	    c[2][2] = 1;
	    c[3][1] = 2;
	    c[3][2] = -3;
	    c[3][3] = 1;
	    for (i = 1; i <= NumberIterations; i++)
		{
//		c[i][1] = c[i - 1][i - 1];
//		DisplayFilledCircle(CircleSize + xdots / 2 - i * CircleSize / 2, ydots / 2 - CircleSize * n / 2 + i * CircleSize, CircleSize / 2, (c[i][1] % moda == 0) ? lo : hi);
		for (j = 1; j <= i; j++)
		    {
		    c[i][j] = -(i - 1) * c[i - 1][j] + c[i - 1][j - 1];
    		    if (i == j)
			c[i][j] = 1;
		    Plot.DisplayFilledCircle(xdots / 2 + i * CircleSize / 2 - j * CircleSize, ydots / 2 - CircleSize * NumberIterations / NumberIterations + i * CircleSize, CircleSize / 2, (ABS(c[i][j]) % moda == 0) ? lo : hi);
		    }
		}
	    break;
	case 'J':   					// Stirling (2nd)	http://en.wikipedia.org/wiki/Stirling_numbers_of_the_second_kind
	    c[0][0] = 1;
	    c[1][0] = 0;
	    c[1][1] = 1;
	    c[2][1] = 1;
	    c[2][2] = 1;
	    c[3][1] = 2;
	    c[3][2] = 3;
	    c[3][3] = 1;
	    for (i = 1; i <= NumberIterations; i++)
		{
//		c[i][1] = c[i - 1][i - 1];
//		DisplayFilledCircle(CircleSize + xdots / 2 - i * CircleSize / 2, ydots / 2 - CircleSize * n / 2 + i * CircleSize, CircleSize / 2, (c[i][1] % moda == 0) ? lo : hi);
		for (j = 1; j <= i; j++)
		    {
		    c[i][j] = j * c[i - 1][j] + c[i - 1][j - 1];
    		    if (i == j)
			c[i][j] = 1;
		    Plot.DisplayFilledCircle(xdots / 2 + i * CircleSize / 2 - j * CircleSize, ydots / 2 - CircleSize * NumberIterations / 2 + i * CircleSize, CircleSize / 2, (ABS(c[i][j]) % moda == 0) ? lo : hi);
		    }
		}
	    break;
	}
    return 0;
    }

/**************************************************************************
    Get parameters for Triangles
**************************************************************************/

void	COtherFunctions::GetNumberIterations(int in)
    {
    NumberIterations = in;
    }
void	COtherFunctions::Getmoda(int in)
    {
    moda = in;
    }
void	COtherFunctions::Gethi(int in)
    {
    hi = in;
    }
void	COtherFunctions::Getlo(int in)
    {
    lo = in;
    }
void	COtherFunctions::GetCircleSize(int in)
    {
    CircleSize = in;
    }


