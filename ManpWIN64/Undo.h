/*
    Undo.h - interface for the Undo struct.

    Written in Microsoft Visual C++ by Paul de Leeuw.
*/

#include "big.h"

struct	UNDO
    {
    BYTE	BigNumFlag;					// True if bignum used
    WORD	type;						// fractal type
    char	UndoBig_hor[SIZEOF_BF_VARS];
    char	UndoBig_vert[SIZEOF_BF_VARS];
    char	UndoBig_width[SIZEOF_BF_VARS];
    int	dec;
    double	hor;						// horizontal address
    double	vert;						// vertical address
    double	mandel_width;					// width of display
    };
