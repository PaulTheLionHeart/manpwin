#include <windows.h>
#include <math.h>
/*
#ifndef _GCC_
#include "CFixedFloat.h"
#endif
*/
#ifndef __FLOATEXP_H__
#define __FLOATEXP_H__
#define MAX_PREC 1020

/*
#ifdef _GCC_
#define _ALIGN_(val,exp) exp += ((*((__int64*)&val) & 0x7FF0000000000000LL)>>52) - 1023; *((__int64*)&val) = (*((__int64*)&val) & 0x800FFFFFFFFFFFFFLL) | 0x3FF0000000000000LL;
#else
*/

/*
Float is a 32-bit IEEE 754 single precision Floating Point Number:
1 bit for the sign,
8 bits for the exponent, and
23 for the value.
Float has 7 decimal digits of precision.

Double is a 64-bit IEEE 754 double precision Floating Point Number:
1 bit for the sign,
11 bits for the exponent, and
52 bits for the value


*/
#define _ALIGN_(val,exp) exp += ((*((__int64*)&val) & 0x7FF0000000000000)>>52) - 1023; *((__int64*)&val) = (*((__int64*)&val) & 0x800FFFFFFFFFFFFF) | 0x3FF0000000000000;
//#endif
class floatexp
{
public:
	double val;
	__int64 exp;
	__inline floatexp &abs()
	{
		if(val<0)
			val=-val;
		return *this;
	}
	void initFromDouble(double a)
	{
		val=a;
		exp=0;
		_ALIGN_(val,exp)
	}
	__inline floatexp &X2()
	    {
	    exp++;
	    return *this;
	    }

	__inline void align()
	{
#ifdef _GCC_
		exp += ((*((__int64*)&val) & 0x7FF0000000000000LL)>>52) - 1023;
#else
		exp += ((*((__int64*)&val) & 0x7FF0000000000000)>>52) - 1023;
#endif
//		__int64 tmpval = (*((__int64*)&val) & 0x800FFFFFFFFFFFFF) | 0x3FF0000000000000;
//		memcpy(&val,&tmpval,sizeof(double));
#ifdef _GCC_
		*((__int64*)&val) = (*((__int64*)&val) & 0x800FFFFFFFFFFFFFLL) | 0x3FF0000000000000LL;
#else
		*((__int64*)&val) = (*((__int64*)&val) & 0x800FFFFFFFFFFFFF) | 0x3FF0000000000000;
#endif
	}
	__inline double setExp(double newval,__int64 newexp)
	{
//		__int64 tmpval = (*((__int64*)&newval) & 0x800FFFFFFFFFFFFF) | ((newexp+1023)<<52);
//		memcpy(&newval,&tmpval,sizeof(double));
//		return newval;
#ifdef _GCC_
		*((__int64*)&newval) = (*((__int64*)&newval) & 0x800FFFFFFFFFFFFFLL) | ((newexp+1023)<<52);
#else
		*((__int64*)&newval) = (*((__int64*)&newval) & 0x800FFFFFFFFFFFFF) | ((newexp+1023)<<52);
#endif
		return newval;
	}
	floatexp()
	{
		val = 0;
		exp = 0;
	}
	floatexp(double a)
	{
		initFromDouble(a);
	}
	__inline floatexp &operator =(const floatexp &a)
	{
		val=a.val;
		exp=a.exp;
		return *this;
	}
	__inline floatexp &operator =(int a)
	{	
		initFromDouble((double)a);
		return *this;
	}
	__inline floatexp &operator =(double a)
	{
		initFromDouble(a);
		return *this;
	}
	__inline floatexp operator *(const floatexp &a)
	{
		floatexp r;
		r.val = a.val*val;
		r.exp = a.exp+exp;
		_ALIGN_(r.val,r.exp)
		return r;
	}
	__inline floatexp operator /(const floatexp &a)
	{
		floatexp r;
		r.val = val/a.val;
		r.exp = exp - a.exp;
		_ALIGN_(r.val,r.exp)
		return r;
	}
	__inline floatexp &mul2()
	{
		exp++;
		return *this;
	}
	__inline floatexp &mul4()
	{
		exp+=2;
		return *this;
	}
	__inline floatexp operator +(const floatexp &a)
	{
		floatexp r;
		__int64 diff;
		if(exp>a.exp){
			diff = exp-a.exp;
			r.exp = exp;
			if(diff>MAX_PREC)
				r.val=val;
			else{
				double aval = setExp(a.val,-diff);
				r.val = val+aval;
			}
		}
		else{
			diff = a.exp-exp;
			r.exp = a.exp;
			if(diff>MAX_PREC)
				r.val=a.val;
			else{
				double aval = setExp(val,-diff);
				r.val = a.val+aval;
			}
		}
		_ALIGN_(r.val,r.exp)
		return r;
	}
	__inline floatexp operator -()
	{
		floatexp r=*this;
		r.val=-r.val;
		return r;
	}
	__inline floatexp &operator +=(const floatexp &a)
	{
#ifdef _GCC_
		floatexp r = *this+a;
		*this = r;
#else
		*this = *this+a;
#endif		
		return *this;
	}
	__inline floatexp operator -(const floatexp &a)
	{
		floatexp r;
		__int64 diff;
		if(exp>a.exp){
			diff = exp-a.exp;
			r.exp = exp;
			if(diff>MAX_PREC)
				r.val = val;
			else{
				double aval = setExp(a.val,-diff);
				r.val = val-aval;
			}
		}
		else{
			diff = a.exp-exp;
			r.exp = a.exp;
			if(diff>MAX_PREC)
				r.val=-a.val;
			else{
				double aval = setExp(val,-diff);
				r.val = aval-a.val;
			}
		}
		_ALIGN_(r.val,r.exp)
		return r;
	}
	__inline floatexp &operator -=(const floatexp &a)
	{
#ifdef _GCC_
		floatexp r = *this-a;
		*this = r;
#else
		*this = *this-a;
#endif
		return *this;
	}
	__inline BOOL operator >(const floatexp &a)
	{
		if(val>0){
			if(a.val<0)
				return TRUE;
			if(exp>a.exp)
				return TRUE;
			else if(exp<a.exp)
				return FALSE;
			return val>a.val;
		}
		else{
			if(a.val>0)
				return FALSE;
			if(exp>a.exp)
				return FALSE;
			else if(exp<a.exp)
				return TRUE;
			return val>a.val;
		}
	}
	__inline BOOL operator <(const floatexp &a)
	{
		if(val>0){
			if(a.val<0)
				return FALSE;
			if(exp>a.exp)
				return FALSE;
			else if(exp<a.exp)
				return TRUE;
			return val<a.val;
		}
		else{
			if(a.val>0)
				return TRUE;
			if(exp>a.exp)
				return TRUE;
			else if(exp<a.exp)
				return FALSE;
			return val<a.val;
		}
	}
	__inline BOOL operator <=(const int a)
	    {
	    floatexp aa(a);
	    return (*this < a || *this == a);
	    }
	__inline BOOL operator >=(const double a)
	    {
	    floatexp aa(a);
	    return (*this > a || *this == a);
	    }
	__inline BOOL operator ==(const floatexp &a)
	{
		if(exp!=a.exp)
			return FALSE;
		return val==a.val;
	}
	__inline bool iszero()
	{
		return (val==0 && exp==0);
	}
	__inline double todouble()
	{
		if(exp<-MAX_PREC || exp>MAX_PREC)
			return 0;
		return setExp(val,exp);
	}
	__inline double todouble(int nScaling)
	{
		if(!nScaling)
			return todouble();
		floatexp ret = *this;
		while(nScaling>9){
			ret.val*=1e10;
			_ALIGN_(ret.val,ret.exp)
			nScaling-=10;
		}
		while(nScaling>2){
			ret.val*=1e3;
			_ALIGN_(ret.val,ret.exp)
			nScaling-=3;
		}
		while(nScaling){
			ret.val*=1e1;
			_ALIGN_(ret.val,ret.exp)
			nScaling--;
		}
		if(ret.exp<-MAX_PREC || ret.exp>MAX_PREC)
			return 0;
		return setExp(ret.val,ret.exp);
	}
	__inline floatexp &operator /=(double a)
	{
		val/=a;
		_ALIGN_(val,exp)
		return *this;
	}
	__inline floatexp &operator *=(double a)
	{
		val*=a;
		_ALIGN_(val,exp)
		return *this;
	}
/*
#ifndef _GCC_
	__inline floatexp &operator =(const CFixedFloat &a)
	{
		exp=0;
		val=0;

		floatexp startexp=1;
		floatexp partmax = FIXEDFLOAT_PARTMAX;
		int nStart = 0;
		for(nStart=0;nStart<a.m_nValues;nStart++){
			if(a.m_pValues[nStart])
				break;
			startexp=startexp/partmax;
		}
		if(nStart>a.m_nValues)
			nStart=a.m_nValues;
		int n=nStart+24/FIXEDFLOAT_DIGITS;
		if(n>a.m_nValues-1)
			n=a.m_nValues-1;
		for(;n>=nStart && n>=0;n--)
			val = val/FIXEDFLOAT_PARTMAX + a.m_pValues[n];
		_ALIGN_(val,exp)
		*this = *this * startexp;

		if(a.m_bSign)
			val = -val;
		return *this;
	}
	__inline void ToFixedFloat(CFixedFloat &a)
	{
		char *szTmp = new char[150000];
		*szTmp=0;
		floatexp partmin = (double)1/(double)FIXEDFLOAT_PARTMAX;
		floatexp partmax = FIXEDFLOAT_PARTMAX;
		floatexp tmp = *this;
		BOOL bNegative=FALSE;
		if(tmp<0){
			bNegative=TRUE;
			tmp = -tmp;
		}
		while(tmp<partmin){
			if(!*szTmp){
				if(bNegative)
					strcpy(szTmp,"-0.0000000");
				else
					strcpy(szTmp,"0.0000000");
			}
			else
				strcat(szTmp,"00000000");
			tmp = tmp*partmax;
		}
		sprintf(szTmp+strlen(szTmp),"%.20f",tmp.todouble());
		int i, b;
		for(i=b=6;szTmp[i];i++)
			if(szTmp[i]!='.')
				szTmp[b++]=szTmp[i];
		szTmp[b]=0;
		a = szTmp;
		delete szTmp;
	}
#else
	floatexp setLongDouble(long double a)
	{
		__int64 val[2]={0};
		memcpy(val,(void*)&a,sizeof(val));
		exp = (val[1]&0x7FFF)-16383;
		val[1] = (val[1]&0x8000) + 16383;
		if(val[0] && val[1]){
			memcpy((void*)&a,val,sizeof(val));
			this->val = (double)a;
		}
		else{
			this->val=1;
			exp=-16383;
		}
		return *this;
	}
	long double toLongDouble()
	{
		long double ret = val;
		__int64 *v = (__int64*)&ret;
		v[1] = (v[1]&0x8000) | (exp + 16383);
		return ret;
	}
#endif
*/
};
#endif //__FLOATEXP_H__
