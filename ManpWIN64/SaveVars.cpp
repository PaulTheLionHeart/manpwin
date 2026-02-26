/*
    SaveVars.cpp a module to backup Bignam variables and restore after resizing the internal bignum parameters. 
    
    Written in Microsoft Visual 'C++' by Paul de Leeuw.

    This program is written in "standard" C. Hardware dependant code
    (console drivers & serial I/O) is in separate machine libraries.
*/

#include	<math.h>
#include	<stdio.h>
#include	"big.h"
#include	"BigDouble.h"
#include	"BigComplex.h"

/**************** Big Number Globals *********************/
extern	BigDouble	Big_xxmax, Big_yymax, Big_xxmin, Big_yymin, Big_xgap, Big_ygap, BigBailout, BigHor, BigVert, BigWidth;
extern	BigComplex	zBig, cBig, qBig;
/*************************************************************************
   Change precision of all global Bignum variables
*************************************************************************/

int	ChangeBigPrecision(int dec)
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
    Format string derived from a Bignum
    mpf_get_str() generates strings without decimal points and gives the exponent
    so we need to format it as a normal number
*************************************************************************/
/*
void	ConvertBignum2String(char *s, mpfr_t num)
    {
    char    FormatString[24];

    sprintf(FormatString, "%%.%dRf", decimals + PRECISION_FACTOR);
    mpfr_sprintf(s, FormatString, num);
    }
*/
/*************************************************************************
    Format Bignum derived from a string
    mpf_set_str() Bignum from strings in the format:
    MeN, where M = mantissa and N exponent
*************************************************************************/

void	ConvertString2Bignum(mpfr_t num, char *s)
    {
    mpfr_set_str(num, s, 10, MPFR_RNDN);
//    gmp_sscanf(s, "%Rf", num);
    }




