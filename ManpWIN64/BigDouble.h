#pragma once

#include	<math.h>
#include	"mpfr.h"
#include	"../qdlib/dd_real.h"
#include	"../qdlib/qd_real.h"

#define		FALSE	0
#define		TRUE	1
#define		zerotol 1.e-50    // 1.e-14
#define		SAFETYMARGIN	4			// Multiply number of digits by a safety margin 

//#define	errorcode -1.e60

extern	int	decimals;

class BigDouble
    {
    public:
	BigDouble(void);
	~BigDouble(void);
	BigDouble(const double & value)
	    {
	    bitcount = decimals * SAFETYMARGIN;
	    mpfr_init2(x, bitcount);
	    mpfr_set_d(x, value, MPFR_RNDN);
	    }

	BigDouble(const BigDouble & Big)		// Copy Constructor
	    {
	    bitcount = decimals * SAFETYMARGIN;
	    mpfr_init2(x, bitcount);
	    mpfr_set(x, Big.x, MPFR_RNDN);
	    }

	BigDouble & operator =(const BigDouble &);	// Assignment Operator
	BigDouble & operator =(const double &);		// Assignment to a double Operator
	BigDouble & operator+=(const BigDouble &);
	BigDouble & operator+=(const double &);
	BigDouble & operator-=(const BigDouble &);
	BigDouble & operator-=(const double &);
	BigDouble & operator*=(const BigDouble &);
	BigDouble & operator*=(const double &);

	BigDouble BigSqr(void) const;			// square is faster than * because of symmetry
	BigDouble BigSqrt(void) const;			// square root
	BigDouble BigInvert(void) const;		// invert
	BigDouble BigAbs(void) const;			// abs()
	BigDouble BigX2(void) const;			// double   r = 2*n
	BigDouble BigHalf(void) const;			// half	    r - n/2
	BigDouble BigCos() const;			// cos
	BigDouble BigSin() const;			// sin
	BigDouble BigTan() const;			// tan
	BigDouble BigSinh() const;			// sinh
	BigDouble BigCosh() const;			// cosh
	BigDouble BigTanh() const;			// tanh
	BigDouble BigLog() const;			// log
	BigDouble BigLog10() const;			// log10
	long	  BigDoubleToInt(void);			// convert a big double to int
	double	  BigDoubleToDouble(void)  const;	// convert a big double to double
	int	  BigDouble2DD(dd_real *out);		// convert a big double to double-double
	int	  BigDouble2QD(qd_real *out);		// convert a big double to quad-double
	int	  DD2BigDouble(const dd_real& in);	// convert a double-double to big double 
	int	  QD2BigDouble(const qd_real& in);	// convert a double-double to big double 
	void	  SetPi();				// generate pi to the number of digits we require

	void	  SafeSprintf(char* buf, size_t bufSize, char* fmt) const;
	void	  ToString(char* buf, size_t bufSize, bool scientific = false) const;

	int	  ChangePrecision(int n);		// increase precision to n digits -1 means precision is out of range
	void	  MatchPrecision(const BigDouble& other);	// match the precision of an extisting bignum

	BigDouble operator +(const BigDouble &) const;	// Addition Operator
	BigDouble operator +(const double &) const;	// add to double Operator
	BigDouble operator -(const BigDouble &) const;	// Subtraction Operator
	BigDouble operator -(const double &) const;	// subtract from double Operator
	BigDouble operator -(void) const;		// unary minus
	BigDouble operator *(const BigDouble &) const;	// Multiplication Operator
	BigDouble operator *(const double &) const;	// Multiply by double Operator
	BigDouble operator /(const BigDouble &) const;	// Division Operator
	BigDouble operator /(const double &) const;	// divide by double Operator
	
	bool operator==(const BigDouble &) const;
	bool operator<(const BigDouble &) const;
	bool operator>(const BigDouble &) const;
	bool operator==(const double &) const;
	bool operator<(const double &) const;
	bool operator>(const double &) const;
	bool operator>=(const BigDouble &) const;
	bool operator<=(const BigDouble &) const;
	bool operator!=(const BigDouble &) const;

	// NEW: exact integer constructors
	void MulInt(BigDouble& result, int n) const;
	void MulInt(BigDouble& result, long n) const;

	BigDouble operator^(double) const;
	BigDouble operator^(const BigDouble &) const;

	BigDouble& operator++();			// prefix
	BigDouble operator++(int);			// postfix

	BigDouble Sin() const;
	BigDouble Cos() const;
	BigDouble Exp() const;
	BigDouble Sinh() const;
	BigDouble Cosh() const;

	mpfr_t		x;				// pointer to BigNum
	mpfr_prec_t	bitcount;
    };

/**************************************************************************
	Some helpers to conform with the naming conventions of other types of arithmetic such as DD/QD
**************************************************************************/

inline BigDouble sin(const BigDouble& x)
    {
    return x.Sin();
    }

inline BigDouble cos(const BigDouble& x)
    {
    return x.Cos();
    }

inline BigDouble fabs(const BigDouble& x)
    {
    return x.BigAbs();   
    }

inline BigDouble log10(const BigDouble& x)
    {
    return x.BigLog10();
    }

inline BigDouble sqrt(const BigDouble& x)
    {
    return x.BigSqrt();
    }

// ---------------------------------------------------------------------------
// BigDouble mixed arithmetic with built-in types (double + BigDouble)
//
// Why this exists:
// ----------------
// C++ does NOT automatically reverse operators. Even though we implement
// member operators such as:
//
//     BigDouble::operator+(double)
//     BigDouble::operator-(double)
//     BigDouble::operator*(double)
//     BigDouble::operator/(double)
//
// those only cover:
//
//     BigDouble + double
//
// The Tierazon template engine (and other generic math code) also produces:
//
//     double + BigDouble
//
// e.g.
//     z->x = param[0] + z->x;
//     z->x = param[0] - z->x;
//     z->x = param[0] * z->x;
//     z->x = param[0] / z->x;
//
// Since 'double' is a built-in type, we cannot attach member functions to it.
// Therefore, we must provide NON-MEMBER operator overloads for symmetry.
//
// Thread safety:
// --------------
// These functions are thread-safe because:
//   - they use only local variables (no shared/static state)
//   - MPFR operates on per-instance data (each BigDouble owns its mpfr_t)
//
// Summary:
// --------
// These overloads allow BigDouble to behave like a native numeric type in
// templated code, supporting full bidirectional arithmetic with doubles.
//
// Supported operations:
// ---------------------
//     double + BigDouble
//     double - BigDouble
//     double * BigDouble
//     double / BigDouble
// ---------------------------------------------------------------------------

BigDouble operator-(double lhs, const BigDouble& rhs);
BigDouble operator+(double lhs, const BigDouble& rhs);
BigDouble operator*(double lhs, const BigDouble& rhs);
BigDouble operator/(double lhs, const BigDouble& rhs);
