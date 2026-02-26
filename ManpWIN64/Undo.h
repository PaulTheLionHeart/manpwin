// Undo.h: interface for the Undo struct.
//
//////////////////////////////////////////////////////////////////////

#include "manp.h"
#include "big.h"

//#define	BIGNUMZSIZE	0x1000

struct	UNDO
{
BYTE	BigNumFlag;					// True if bignum used
WORD	type;						// fractal type
char	UndoBig_hor[SIZEOF_BF_VARS];
char	UndoBig_vert[SIZEOF_BF_VARS];
char	UndoBig_width[SIZEOF_BF_VARS];
int	dec;
DOUBLE	hor;						// horizontal address
DOUBLE	vert;						// vertical address
DOUBLE	mandel_width;					// width of display
};
