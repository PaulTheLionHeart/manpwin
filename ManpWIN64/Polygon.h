/*
    Polygon.h - interface for the CPoly class - polygon class.

    Written in Microsoft Visual C++ by Paul de Leeuw.
*/

#include	"Plot.h"

#ifndef POLYGON_H
#define POLYGON_H

#define	DWORD	unsigned long
#define	MAXPOINT    20

//Declaration of class point
class Point
    {
    public:
    int x,y;
    };

class CPoly
    {
    private:
    Point p[MAXPOINT];
    int inter[MAXPOINT],x,y;
    int v,xmin,ymin,xmax,ymax;
    public:
    int c;
    DWORD colour;
    void read(int x, int y, int i);
    void calcs();
    void init(int vertices, DWORD color);
    void display(CPlot Plot);
    void ints(double);
    void sort(int, int, CPlot Plot);
    };
#endif
