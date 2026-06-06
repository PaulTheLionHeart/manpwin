/*
    SaveVars.cpp - a module to backup Bignum variables and restore after resizing the internal bignum parameters. 
    
    Written in Microsoft Visual 'C++' by Paul de Leeuw.
*/

#include	<math.h>
#include	<stdio.h>
#include	"big.h"
#include	"BigDouble.h"
#include	"BigComplex.h"
#include	"Manp.h"

/*************************************************************************
   Change precision of all global Bignum variables
*************************************************************************/

int	CManp::ChangeBigPrecision(int dec)
    {
    if (dec * 10 > MPFR_PREC_MAX)
	return -1;
    BigHor.ChangePrecision(dec);
    BigVert.ChangePrecision(dec);
    BigWidth.ChangePrecision(dec);
    BigBailout.ChangePrecision(dec);
    Big_xgap.ChangePrecision(dec);
    Big_ygap.ChangePrecision(dec);
    Big_xxmax.ChangePrecision(dec); 
    Big_yymax.ChangePrecision(dec); 
    Big_xxmin.ChangePrecision(dec); 
    Big_yymin.ChangePrecision(dec);

    zBig.x.ChangePrecision(dec);
    zBig.y.ChangePrecision(dec);
    qBig.x.ChangePrecision(dec);
    qBig.y.ChangePrecision(dec);
    cBig.x.ChangePrecision(dec);
    cBig.y.ChangePrecision(dec);
    return 0;
    }

/*************************************************************************
    Format Bignum derived from a string
    mpf_set_str() Bignum from strings in the format:
    MeN, where M = mantissa and N exponent
*************************************************************************/

void	CManp::ConvertString2Bignum(mpfr_t num, char *s)
    {
    mpfr_set_str(num, s, 10, MPFR_RNDN);
    }




