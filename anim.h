////////////////////////////
// ANIM.H header file
//
// Animated GIF and cursor specific stuff
////////////////////////////

#include "Dib.h"

#define	MAXANIM		100000	// maximum animation frames

#define	LPBimage(lpbi)	((BYTE *)lpbi+lpbi->biSize+(long)(lpbi->biClrUsed*sizeof(RGBQUAD)))
#define	LPBlinewidth(lpbi) (WIDTHBYTES((WORD)lpbi->biWidth*lpbi->biBitCount))
#define	LPBwidth(lpbi)	(lpbi->biWidth)

enum	AnimType    {SAVEFRAME, ANIMATE, GIFFILE};

struct	AnimStruct
    {
    char	*animFrameData;			// Pointer to fractal data
    char	FrameFilename[MAX_PATH];
    char	FrameInfo[MAX_PATH];
    double	ParamValue;			// used to display value for animated frame
    double	DelayMultiplier;		// used to display value for animated frame
    BOOL	IsParamAnim;			// are we animating a parameter?
    BOOL	IsMorphAnim;			// are we morphing?
    CDib	animDIB;
    };
