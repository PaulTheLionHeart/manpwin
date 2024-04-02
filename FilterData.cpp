// FilterData.cpp: data for the Tierazon filters.
//
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////
// database of Tierazon filters
//////////////////////////////////////////////////////////////////

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
	' ', FALSE,
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
	' ', FALSE,
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
	' ', FALSE,
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

