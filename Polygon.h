// Polygon.h: polygon class.
//
//////////////////////////////////////////////////////////////////////

#include	"Plot.h"

#define	DWORD	unsigned long

//Declaration of class point
class Point
    {
    public:
    int x,y;
    };

class CPoly
    {
    private:
    Point p[20];
    int inter[20],x,y;
    int v,xmin,ymin,xmax,ymax;
    public:
    int c;
    DWORD colour;
    void read(int x, int y, int i);
    void calcs();
    void init(int vertices, DWORD color);
    void display(CPlot Plot);
    void ints(double);
//    void intsx(double);
    void sort(int);
    void sort(int, int, CPlot Plot);
    };
