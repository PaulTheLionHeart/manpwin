/*
    FilterData.cpp - database of Tierazon filters

    Written in Microsoft Visual 'C++' by Paul de Leeuw.
*/

#include	<windows.h>
#include	"filter.h"

struct TierazonFilterStuff TierazonFilter[]=
    {
	{
	"0.) None",
	' ', FALSE,
	},
	{
	"01.) 30*pow((1-d_real/dStrands),n_color); [stalks]", 
	'2', TRUE,
	},
	{
	"02.) 5*pow((1-temp/dStrands),n_color); [stalks && bubbles]", 
	'2', TRUE,
	},
	{
	"03.) Moving Average I",
	' ', FALSE,
	},
	{
	"04.) Moving Average II",
	' ', FALSE,
	},
	{
	"05.) [Linas Vepstas] n = (n-log(log(sum_sqrs_z()))/log(2))", 
	'2', TRUE,
	},
	{
	"06.) Bubbles I [F1] and the log(log(..)) algorithm [F2]", 
	'2', TRUE,
	},
	{
	"07.) 30*pow((1 - zx*zx/dStrands),n_color); [dual bubbles]", 
	'2', TRUE,
	},
	{
	"08.) 10*pow((1-temp/dStrands),n_color); [bubbles II]", 
	'2', TRUE,
	},
	{
	"09.) 100*pow((1-d_real/dStrands),n_color); [stalks II]", 
	'2', TRUE,
	},
	{
	"10.) Rings I",
	' ', FALSE,
	},
	{
	"11.) Rings II, mod 2",
	'2', TRUE,
	},
	{
	"12.) Rings III, mod 2",
	' ', FALSE,
	},
	{
	"13.) Hi Lo Strands I",
	'2', TRUE,
	},
	{
	"14.) Hi Lo Rings",
	'2', TRUE,
	},
	{
	"15.) abs(sin(x*x*100)*abs(sin(y*y*100)*30 && log(log(...))", 
	' ', FALSE,
	},
	{
	"16.) abs(sin(x*x*100)*abs(sin(y*y*100)*30 && atan method I", 
	' ', FALSE,
	},
	{
	"17.) atan & stalks II",
	'2', TRUE,
	},
	{
	"18.) atan & bubbles III",
	'2', TRUE,
	},
	{
	"19.) atan & bubbles IV, factor 2",
	'2', TRUE,
	},
	{
	"20.) atan & bubbles V, factor 2",
	'2', TRUE,
	},
	{
	"21.) atan & bubbles, flower petals I, factor 2", 
	'2', TRUE,
	},
	{
	"22.) atan & bubbles, figure 8 I, factor 2",
	'2', TRUE,
	},
	{
	"23.) atan & bubbles, figure 8 II modulas 2, factor 2", 
	'2', TRUE,
	},
	{
	"24.) atan & bubbles, flower petals II, factor 2", 
	'2', TRUE,
	},
	{
	"25.) sine & bubbles, flower petals I modulas 2, factor 2", 
	'2', TRUE,
	},
	{
	"26.) atan & bubbles VI, factor 2",
	' ', FALSE,
	},
	{
	"27.) Iterations FDimensions",
	'2', TRUE,
	},
	{
	"28.) Standard Deviation",
	'2', TRUE,
	},
	{
	"29.) Stalks && Square Bubbles I",
	'2', TRUE,
	},
	{
	"30.) Stalks && Square Bubbles II",
	'2', TRUE,
	},
	{
	"31.) Weights",
	'2', TRUE,
	},
	{
	"32.) Weights II",
	'2', TRUE,
	},
	{
	"33.) Stalks FDimension",
	'2', TRUE,
	},
	{
	"34.) Stalks Standard Deviation",
	'2', TRUE,
	},
	{
	"35.) Hi/ Lo Stalks FDimension",
	'2', TRUE,
	},
	{
	"36.) Hi/ Lo Stalks Std Deviation",
	'2', TRUE,
	},
	{
	"37.) Hi/ Lo Stalks FDimension II",
	'2', TRUE,
	},
	{
	"38.) atan FDimension",
	'2', TRUE,
	},
	{
	"39.) x-ray Stalks",
	'2', TRUE,
	},
	{
	"40.) x-ray Bubbles",
	'2', TRUE,
	},
	{
	"41.) x-ray Rings",
	'2', TRUE,
	},
	{
	"42.) x-ray Hi/ Lo Stalks",
	'2', TRUE,
	},
	{
	"43.) atan counting",
	'2', TRUE,
	},
	{
	"44.) diamonds",
	'2', TRUE,
	},
	{
	"45.) wolf",
	'2', TRUE,
	},
	{
	"46.) bubbles IV",
	'2', TRUE,
	},
	{
	"47.) Bubbles V",
	'2', TRUE,
	},
	{
	"48.) FD Bubbles",
	'2', TRUE,
	},
	{
	"49.) FD Bubbles II",
	'2', TRUE,
	},
	{
	"50.) 100*(1-(zx/zy)/dStrands); [Jette]",
	'2', TRUE,
	},
	{
	"51.) Stalks && Bubbles I",
	'2', TRUE,
	},
	{
	"52.) Stalk && Bubbles II",
	'2', TRUE,
	},
	{
	"53.) Flarium 07, i+=(int)(sin(fabs(dzx_save)*fabs(dzy_save))*10)",
	'2', TRUE,
	},
	{
	"54.) Flarium 08, i+=(int)(sin(fabs(dzx_save)/fabs(dzy_save))*10)",
	'2', TRUE,
	},
	{
	"55.) Flarium 13, i=(int)((fabs(atan(z.real()/z.imag()))+fabs(atan(dzx_save/dzy_save)))*100)",
	    '2', TRUE,
	},
	{
	"56.) Flarium 16, Delta Slope, On Change;, ...J+=FF; plus atan method",
	    '2', TRUE,
	},
	{
	"57.) Flarium 17, Delta Slope, On Change;, ...J+=FF",
	'2', TRUE,
	},
	{
	"58.) Flarium 18, Delta Slope, No Change;, ...J+=FF",
	'2', TRUE,
	},
	{
	"59.) Flarium 30, (atan(fabs(x_rmax * x_rmin)/fabs(y_rmax * y_rmin)) * 40)",
	'2', TRUE,
	},
	{
	"60.) Flarium 31, if (fabs(dzx) <= dStrands || fabs(dzy) <= dStrands) rr=ri",
	'2', TRUE,
	},
	{
	"61.) Flarium 32, if (dzx*dzx+dzy*dzy < limit) rr+=3",
	'2', TRUE,
	},
	{
	"62.) Flarium 33, Combination Filters 1,2, && 3",
	'2', TRUE,
	},
	{
	"63.) Flarium 34, if (fabs(dzx) > dStrands || fabs(dzy) > dStrands)",
	'2', TRUE,
	},
	{
	"64.) Flarium 35, if (fabs(dzx) > dStrands &&&& fabs(dzy) > dStrands)",
	'2', TRUE,
	},
	{
	"65.) Flarium 36, if (fabs(dzx) > limit || fabs(dzy) > limit)",
	'2', TRUE,
	},
	{
	"66.) Flarium 37, if (fabs(dzx) < limit || fabs(dzy) < limit)",
	'2', TRUE,
	},
	{
	"67.) Flarium 38, if (fabs(1/dzx*dzy) > limit)",
	'2', TRUE,
	},
	{
	"68.) Flarium 39, if (fabs(dzx) > limit || fabs(dzy) > limit)",
	'2', TRUE,
	},
	{
	"69.) Flarium 40, ri = rr + atan(fabs(dzx_save/dzy_save)) * 30",
	'2', TRUE,
	},
	{
	"70.) Flarium 41, rr+=log(dzx*dzx+dzy*dzy)*nBay100",
	    '2', TRUE,
	},
	{
	"71.) Flarium 46, i = (int)rr; rr+=log(dzx*dzx+dzy*dzy)*(1+nBay100)",
	'2', TRUE,
	},
	{
	"72.) Flarium 47, i = (int)(rr + atan(fabs(dzx_save/dzy_save)) * (10+nBay1000))",
	'2', TRUE,
	},
	{
	"73.) Flarium 48, if (fabs(dzx) < z.squares() || fabs(dzy) < z.squares())  rr+=nFF",
	'2', TRUE,
	},
	{
	"74.) Flarium 49, rr += atan(fabs(dzy/dzx))*atan(fabs(dzx/dzy))*2",
	'2', TRUE,
	},
	{
	"75.) Flarium 69, Clouds",	// Special orbit-density Clouds filter from Flarium
	'2', TRUE,
	},





	NULL, NULL, NULL
    };

struct TierazonFilterStuff TierazonColour[]=
    {
	{
	"0.) None",
	' ', TRUE,
	},
	{
	"01.) sin(cx*cx*100)*sin(cy*cy*100*100*bf2", 
	'2', TRUE,
	},
	{
	"02.) sin(x*100)*sin(y*100)*100*bf2", 
	'2', TRUE,
	},
	{
	"03.) (atan(zy/zx)-atan(zx/zy))*100*bf2",
	'2', TRUE,
	},
	{
	"04.) (atan(y/x)-atan(x/y))*100*bf2",
	'2', TRUE,
	},
	{
	"05.) atan(y/x)+atan(x/y)*100*bf2", 
	'2', TRUE,
	},
	{
	"06.) atan(x/y)*10*bf2", 
	'2', TRUE,
	},
	{
	"07.) sin(x*y*100+cx*cy*10)*100*bf2", 
	'2', TRUE,
	},
	{
	"08.) Stalks", 
	'2', TRUE,
	},
	{
	"09.) sin(x*x*60)*100*bf2, sin(y*y*60)*100*bf2", 
	'2', TRUE,
	},
	{
	"10.) sqrs*cos(x*x/y*y)*1000*bf2",
	'2', TRUE,
	},
	{
	"11.) sin(function)*nBay100",
	'2', TRUE,
	},
	{
	"12.) cos(fabs(dzx*dzy/dzx_save*dzy_save)*10)*100*nBay100",
	'2', TRUE,
	},
	{
	"13.) (atan(1/fabs(c.real()/c.imag()))-atan(1/fabs(c.imag()/c.real())))*100*nBay100",
	'2', TRUE,
	},
	{
	"14.) atan(fabs(z.real()/z.imag()))*200*nBay100s",
	'2', TRUE,
	},
	{
	"15.) temp = atan(fabs(c.real()/c.imag())) * atan(fabs(z.real()/z.imag()))", 
	'2', TRUE,
	},
	{
	"16.) Squares [Bay Fact 1]", 
	'2', TRUE,
	},
	{
	"17.) Circles [Bay Fact 1]",
	'2', TRUE,
	},
	{
	"18.) Radial [Bay Fact 1]",
	'2', TRUE,
	},
	{
	"19.) Checkerboard Squares [Bay Fact 1, Bay Fact 2]",
	'2', TRUE,
	},
	{
	"20.) 5000* Bay Fact 2 *(z.real()*z.imag())",
	'2', TRUE,
	},
	{
	"21.) 2500* Bay Fact 2 *fabs(z.squares()-c.squares())", 
	'2', TRUE,
	},
	{
	"22.) 5000* Bay Fact 2 *fabs(z.imag()*c.imag())",
	'2', TRUE,
	},
	{
	"23.) 5000* Bay Fact 2 *fabs(z.real()*z.imag()-c.squares())", 
	'2', TRUE,
	},
	{
	"24.) 5000* Bay Fact 2 *fabs(z.real()*z.imag())", 
	'2', TRUE,
	},
	{
	"25.) 2500* Bay Fact 2 *fabs(z.squares()-c.squares())", 
	'2', TRUE,
	},
	{
	"26.) RGB + Filter",
	'2', TRUE,
	},
	{
	"27.) GRB + Filter",
	'2', TRUE,
	},
	{
	"28.) BGR + 5000* Bay Fact 2 *fabs(z.real()*z.imag())",
	'2', TRUE,
	},
	{
	"29.) BRG + 5000* Bay Fact 2 *fabs(z.real()*z.imag())",
	'2', TRUE,
	},
	{
	"30.) RGB + 5000* Bay Fact 2 *fabs(z.real()*z.imag())",
	'2', TRUE,
	},
	{
	"31.) RGB + 5 * Bay Fact 2 *fabs(z.real()*z.imag())",
	'2', TRUE,
	},
	{
	"32.) RGB + 128 + 5 * Bay Fact 2 *fabs(z.real()*z.imag())",
	'2', TRUE,
	},
	{
	"33.) RGB + 5000*cos(last z values)*Bay Fact 2",
	'2', TRUE,
	},
	{
	"34.) RGB + 5000*cos(last z values)*Bay Fact 2 [Part II]",
	'2', TRUE,
	},
	{
	"35.) RGB + 100*atan(z.real/z.imag)*Bay Fact 2",
	'2', TRUE,
	},
	{
	"36.) RGB + 50 (last z values) * Bay Fact 2",
	'2', TRUE,
	},
	NULL, NULL, NULL
    };

char	*TierazonFDOptions[] = 		// list of option 
    {
    "01.) FD_0 of x",
    "02.) FD_0 of y",
    "03.) FD_0x + FD_0y ",
    "04.) FD_0 Sum of Squares",
    "05.) FD_0x * FD_0y ",
    "06.) FD of x",
    "07.) FD of y",
    "08.) FD_x + FD_y ",
    "09.) FD Sum of Squares",
    "10.) FD_x * FD_y ",
    "11.) sum of squares",
    "12.) difference",
    "13.) average FD",
    "14.) average T",
    "15.) difference T * 2",
    "16.) sum of squares sFD",
    "17.) gray dFD",
    "18.) gray T",
    "19.) gray sFD",
    "20.) sum of squares II sFD",
    };

