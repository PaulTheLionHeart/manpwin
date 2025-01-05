#ifndef _QD_QD_CONFIG_H
#define _QD_QD_CONFIG_H  1

#define QD_API 

/* Set to 1 if using VisualAge C++ compiler for __fmadd builtin. */
#define QD_VCAPP_BUILTINS_H 0

/* If fused multiply-add is available, define to correct macro for
   using it.  It is invoked as QD_FMA(a, b, c) to compute fl(a * b + c). 
   If correctly rounded multiply-add is not available (or if unsure), 
   keep it undefined.*/
/* #undef QD_FMA */

/* If fused multiply-subtract is available, define to correct macro for
   using it.  It is invoked as QD_FMS(a, b, c) to compute fl(a * b - c). 
   If correctly rounded multiply-add is not available (or if unsure), 
   keep it undefined.*/
/* #undef QD_FMS */

/* Set the following to 1 to define commonly used function
   to be inlined.  This should be set to 1 unless the compiler 
   does not support the "inline" keyword, or if building for 
   debugging purposes. */
#define QD_INLINE 1

/* Set the following to 1 to use ANSI C++ standard header files
   such as cmath, iostream, etc.  If set to zero, it will try to 
   include math.h, iostream.h, etc, instead. */
#define QD_HAVE_STD 1

/* Set the following to 1 to make the addition and subtraction
   to satisfy the IEEE-style error bound 

      fl(a + b) = (1 + d) * (a + b)

   where |d| <= eps.  If set to 0, the addition and subtraction
   will satisfy the weaker Cray-style error bound

      fl(a + b) = (1 + d1) * a + (1 + d2) * b

   where |d1| <= eps and |d2| eps.           */
#define QD_IEEE_ADD 0

/* Set the following to 1 to use slightly inaccurate but faster
   version of multiplication. */
#define QD_SLOPPY_MUL 0

/* Set the following to 1 to use slightly inaccurate but faster
   version of division. */
#define QD_SLOPPY_DIV 0

#endif /* _QD_QD_CONFIG_H */
