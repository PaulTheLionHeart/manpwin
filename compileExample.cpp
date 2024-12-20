

// simple example of QD usage to illustrate linking process
// Alex Kaiser, LBNL, 6/3/2010

// Mandelbrot test added PHD 19/12/2024



#include <iostream>
#include <qd/qd_real.h>
#include <qd/fpu.h>

using namespace std; 

#define	WIDTH	50
#define HEIGHT	50

int main() {
	
	// ensure that 80-bit arithmetic is not in place
	// this call forces 64-bit arithmetic
	unsigned int old_cw;
	fpu_fix_start(&old_cw);

	cout.precision(60); 
/*	
	// simple read example
	//
	// qd_real readTest ; 
	// cin >> readTest ; 	
	// cout << "readTest = " << readTest << endl ;
	
	// simple demo
	qd_real x = "1.0" ;  
	x /= 3.0 ;
	qd_real y ; 
	y = pow( qd_real(2.0) , 3 ) ; 
	cout << "y = " << y << endl;
	cout << "x = " << x << endl; 
	
	
	qd_real a ; 
	qd_real b = qd_real("0.1");
	
	a = sqrt(b);
	cout << " sqrt(0.1) = " << a << endl;
	cout << " sqrt(0.1) * sqrt(0.1) = " << a * a << endl; 
*/
	qd_real zx, zy, cx, cy;	    // Z and C for Mandelbrot test
	qd_real deltax, deltay;
	qd_real bailout = 4.0;
	qd_real sqrx, sqry, zxy;
	qd_real	xmin[3] = {-2.0, -0.9, -0.75};
	qd_real ymax[3] = {2.0, 0.5, 0.073};
	qd_real width[3] = {4.0, 0.5, 0.005};
	qd_real	dx, dy;
	int	colour, iteration, max_iter = 160;

	for (int k = 0; k < 3; k++)
	    {
	    cx = xmin[k]; cy = ymax[k];
	    dx = width[k] / (double)WIDTH;
	    dy = width[k] / (double)HEIGHT;
	    deltax = dx;
	    deltay = dy;

	    for (int i = 0; i < HEIGHT; i++)
		{
		cx = xmin[k];
		for (int j = 0; j < WIDTH; j++)
		    {
		    iteration = 0;
		    zx = 0.0; zy = 0.0;
		    while (true)
			{
			sqrx = zx * zx; sqry = zy * zy; zxy = zx * zy;
			zx = sqrx - sqry + cx;
			zy = zxy + zxy + cy;
			if (iteration++ >= max_iter)
			    {
			    colour = 1;
			    break;
			    }
			if (sqrx + sqry >= bailout)
			    {
			    colour = 0;
			    break;
			    }
			}
		    if (colour) 
			cout << "..";
		    else 
			cout << "##";
		    cx += deltax;
		    }
		cout << endl;
		cy -= deltay;
		}
	    cout << endl;
	    cout << endl;
	    }

	fpu_fix_end(&old_cw); 
	return 0;
}


