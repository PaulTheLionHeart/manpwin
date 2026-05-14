////////////////////////////
// ANIM.H header file
//
// Animated GIF and cursor specific stuff
////////////////////////////

#ifndef ANIM_H
#define ANIM_H

#include <vector>
//#include "Dib.h"

#define	MAXANIM		100000	// maximum animation frames

#define	LPBimage(lpbi)	((BYTE *)lpbi+lpbi->biSize+(long)(lpbi->biClrUsed*sizeof(RGBQUAD)))
#define	LPBlinewidth(lpbi) (ComputeWidthBytes((WORD)lpbi->biWidth, lpbi->biBitCount))
#define	LPBwidth(lpbi)	(lpbi->biWidth)

enum	AnimType    {SAVEFRAME, ANIMATE, GIFFILE};

struct	AnimStruct
    {
    std::vector<char> animFrameData;		// Pointer to fractal data
    char	FrameFilename[MAX_PATH];
    char	FrameInfo[MAX_PATH];
    double	ParamValue = 1.0;		// used to display value for animated frame
    double	DelayMultiplier = 1.0;		// used to display value for animated frame
    bool	IsParamAnim = false;		// are we animating a parameter?
    bool	IsMorphAnim = false;		// are we morphing?
    bool	valid = false;			// has the frame been initialised?
    int		width, height;
    int		bitsPerPixel;			// 24
    size_t 	pitch;				// bytes per row
    std::vector<uint8_t> pixels;
    };

extern	bool	BuildDibFromAnimFrame(const AnimStruct& A, CDib& Dib);
extern	void	InitAnimationFrameStructure(int Frames);
#endif
