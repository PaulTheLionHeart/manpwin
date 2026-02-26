#include <conio.h>

#include <dos.h>
#include <stdlib.h>
#include "polygon.h"
#include "Plot.h"

//DEFINE READ FUNCTION
void CPoly::read(int x, int y, int i)
    {
    p[i].x = x;
    p[i].y = y;
    }

void CPoly::init(int vertices, DWORD color)
    {
    v = vertices;
    p[vertices].x=p[0].x;
    p[vertices].y=p[0].y;
    xmin=xmax=p[0].x;
    ymin=ymax=p[0].y;
    colour = color;
    }

//FUNCTION FOR FINDING
void CPoly::calcs()
    { //MAX,MIN
    for(int i=0;i<v;i++)
	{
	if(xmin>p[i].x)
	xmin=p[i].x;
	if(xmax<p[i].x)
	xmax=p[i].x;
	if(ymin>p[i].y)
	ymin=p[i].y;
	if(ymax<p[i].y)
	ymax=p[i].y;
	}
    }
//DISPLAY FUNCTION
void CPoly::display(CPlot Plot)
    {
    char ch='y';
    double s;

    s=ymin+0.5;
    while(s<=ymax)
	{
	ints(s);
	sort((int)s,1, Plot);
	s++;
	}
    }

void CPoly::ints(double z) //DEFINE FUNCTION INTS
    {
    int x1,x2,y1,y2,temp;
    c=0;

    for(int i=0;i<v;i++)
	{
	x1=p[i].x;
	y1=p[i].y;
	x2=p[i+1].x;
	y2=p[i+1].y;
	if(y2<y1)
	    {
	    temp=x1;
	    x1=x2;
	    x2=temp;
	    temp=y1;
	    y1=y2;
	    y2=temp;
	    }
	if(z<=y2&&z>=y1)
	    {
	    if((y1-y2)==0)
		x=x1;
	    else
		{
		x=((x2-x1)*((int)z-y1))/(y2-y1);
		x=x+x1;
		}
	    if(x<=xmax && x>=xmin)
		inter[c++]=x;
	    }
	}
    }

void CPoly::sort(int z,int w, CPlot Plot) //SORT FUNCTION
    {
    int	temp, i, j;

    for(int i=0;i<c;i++)
	{
	for(j=i+1;j<c;j++)
	    {
	    if(inter[i]>inter[j])
		{
		temp=inter[i];
		inter[i]=inter[j];
		inter[j]=temp;
		}
	    }
	}

    if(w==1)
	{
	for(i=0;i<v;i++)
	    Plot.genline(p[i].x,p[i].y,p[i+1].x,p[i+1].y, colour);
	for(i=0; i<c;i+=2)
	    Plot.genline(inter[i],z,inter[i+1],z, colour);
	}
    }

