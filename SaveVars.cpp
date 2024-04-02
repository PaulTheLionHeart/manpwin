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

void	ConvertBignum2String(char *s, mpfr_t num)
    {
    char    FormatString[24];

    sprintf(FormatString, "%%.%dRf", decimals + PRECISION_FACTOR);
    mpfr_sprintf(s, FormatString, num);
    }

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

/////////////////////////////////////////////////////
//  This is a cludge. analyse_corner() resets all the 
//  internal parameters of Bignum and allows ongoing
//  increases in the number of decimals
/////////////////////////////////////////////////////

int	ReinitBigVars(int dec)
    {
//    char	*buf = NULL;
//    char	*s1 = NULL;
//    char	*s2 = NULL;
//    char	*s3 = NULL;
//    int		flag = 0;

#ifdef DEBUG
    BigDouble	a1, a2, a3, a4, a5, a6, a7, a8, a9;
    a1 = -0.0123;
    a2 = 12345.67890;
    a3 = -2.5;
    a4 = -123.456;
    a5 = 6.2;
    a6 - 0.0;
    a7 = 0.0003;
    a8 = 0.1;
    a9 = -0.05;
#endif // DEBUG

//    s1 = new char[SIZEOF_BF_VARS];
//    s2 = new char[SIZEOF_BF_VARS];
//    s3 = new char[SIZEOF_BF_VARS];
//    buf = new char[SIZEOF_BF_VARS * 3];

#ifdef DEBUG
    ConvertBignum2String(s1, a1.x);
    ConvertBignum2String(s2, a2.x);
    ConvertBignum2String(s3, a3.x);

    ConvertBignum2String(s1, a4.x);
    ConvertBignum2String(s2, a5.x);
    ConvertBignum2String(s3, a6.x);

    ConvertBignum2String(s1, a7.x);
    ConvertBignum2String(s2, a8.x);
    ConvertBignum2String(s3, a9.x);
#endif // DEBUG
//    ChangeBigPrecision(dec);							// increase precision of Big numbers
//    ConvertBignum2String(s1, BigHor.x);						// store them in text strings so we don't splatter them during the resizing of the bignum parameters
//    ConvertBignum2String(s2, BigVert.x);
//    ConvertBignum2String(s3, BigWidth.x);
//    sprintf(buf, "%s,%s,%s, %f, %f", s1, s2, s3, param[0], param[1]);		// ensure that real and imag perturbation is unchanged
//    flag = analyse_corner(buf);
//    if (s1) delete [] s1;
//    if (s2) delete [] s2;
//    if (s3) delete [] s3;
//    if (buf) delete [] buf;
    return  ChangeBigPrecision(dec);
    }




