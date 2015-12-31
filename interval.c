/*
 * Filename: interval.c
 * Purpose : Implementation of the interval ADT API. The implementation of 
 *           intervals is partially based on:
 *           "Variable Size Analysis and Validation of Computation Quality", 
 *           by H. Yamashita, H. Yasuura, F.N. Eko and C. Yun.
 * Author  : Nikolaos Kavvadias (C) 2009, 2010, 2011, 2012, 2013, 2014, 2015
 *                                  2016
 * Date    : 20-Sep-2014
 * Revision: 0.2.0 (20/09/14)
 *           Updated for github; self-contained version not depending on 
 *           external files (genmacros.h, utils.c, utils.h).
 *           0.1.2 (11/08/09)
 *           Changes to IntervalIntersection in order to take account the case 
 *           of produced empty intervals.
 *           0.1.1 (23/07/09)
 *           Added: IntervalIsSymmetric, IntervalMod, IntervalSet, IntervalCopy, 
 *           IntervalUniverse. Changed the vmax and vmin Interval struct fields 
 *           to the more formal supr (supremum) and infm (infimum).
 *           0.1.0 (22/07/09)
 *           Initial version. Implemented the backbone of the interval 
 *           arithmetic API: INTERVAL, IntervalAdd, IntervalSub, IntervalNeg,
 *           IntervalMul, IntervalDiv, IntervalMux, IntervalAnd, IntervalIor, 
 *           IntervalXor, IntervalNot, IntervalExpInteger, IntervalSqrt, 
 *           IntervalAbs, IntervalMax, IntervalMin, IntervalUnion,
 *           IntervalIntersection, ValueIsInInterval, IntervalIsEmpty,
 *           IntervalIsPositive, IntervalIsNegative, ValueToInterval, 
 *           IntervalBalanced, IntervalIsBalanced, IntegerBitwidthToInterval, 
 *           IntervalToIntegerBitwidth, IntervalPrint.
 */    

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "interval.h"

/* INTERVAL: 
 * Construct an interval specified by a minimum (u) and a maximum (v) integer 
 * value. Values u and v are considered to be included in the interval. 
 */
Interval INTERVAL(int u, int v)
{
  Interval RV;
  RV.infm = u;
  RV.supr = v;
  return (RV);
}

/* IntervalCopy: 
 * Return a copy of the given input interval.
 */
Interval IntervalCopy(Interval x)
{
  Interval z;
  z.supr = x.supr;
  z.infm = x.infm;
  return (z);
}

/* IntervalEmpty: 
 * Return an empty interval; interval [1,0] is produced.
 */
Interval IntervalEmpty(void)
{
  Interval z;
  z.supr = 0;
  z.infm = 1;
  return (z);
}

/* IntervalUniverse: 
 * Returns the entire interval for a given arithmetic representation type (ztyp) 
 * and for the specified bitwidth (bw).
 */
Interval IntervalUniverse(int bw, ArithType ztyp)
{
  Interval z;
  
  if (bw > 0 && bw <= 32)
  {
    IntegerBitwidthToInterval(bw, ztyp);
  }
  else
  {
    fprintf(stderr, "Error: Given maximum bitwidth should be less than or equal 32.\n");
    exit(1);
  }
  
  return (z);
}

/* IntervalClamp: 
 * Return a saturated version of the given interval for the specified lower (lo)
 * and higher (hi) bounds.
 */
Interval IntervalClamp(Interval x, int lo, int hi)
{
  Interval z;
  z.supr = MIN(x.supr, hi);
  z.infm = MAX(x.infm, lo);
  return (z);
}

/* IntervalAdd: 
 * Return the interval of the result of adding the intervals of two integers.
 */
Interval IntervalAdd(Interval x, Interval y)
{
  Interval z;
  z.supr = x.supr + y.supr;
  z.infm = x.infm + y.infm;
  return (z);
}

/* IntervalSub: 
 * Return the interval of the result of performing subtraction on two integer 
 * intervals.
 */
Interval IntervalSub(Interval x, Interval y)
{
  Interval z;
  z.supr = x.supr - y.infm;
  z.infm = x.infm - y.supr;
  return (z);
}

/* IntervalNeg: 
 * Return a negated interval by negating the supremum and infimum fields.
 */
Interval IntervalNeg(Interval x)
{
  Interval z;
  z.supr = -x.supr;
  z.infm = -x.infm;
  return (z);
}

/* IntervalMul: 
 * Return the interval of the result of performing multiplication on two integer 
 * intervals. The result is not truncated. xtyp, ytyp provide the arithmetic 
 * representation type for x and y, respectively.
 */
Interval IntervalMul(Interval x, Interval y, ArithType xtyp, ArithType ytyp)
{
  Interval z;
  
  if ((xtyp == UNSIGNED_ARITH) && 
      (ytyp == UNSIGNED_ARITH))
  {
    z.supr = x.supr * y.supr;
    z.infm = x.infm * y.infm;
  }
  else if ((xtyp == UNSIGNED_ARITH) && 
           (ytyp == SIGNED_ARITH || ytyp == SIGNED_POS_ARITH || ytyp == SIGNED_NEG_ARITH))
  {
    z.supr = MAX(x.supr * y.supr, x.infm * y.supr);
    z.infm = MIN(x.supr * y.infm, x.infm * y.infm);    
  }
  else if ((xtyp == SIGNED_ARITH || xtyp == SIGNED_POS_ARITH || xtyp == SIGNED_NEG_ARITH) && 
           (ytyp == UNSIGNED_ARITH))
  {
    z.supr = MAX(x.supr * y.supr, x.supr * y.infm);
    z.infm = MIN(x.infm * y.supr, x.infm * y.infm);
  }
  else if ((xtyp == SIGNED_ARITH || xtyp == SIGNED_POS_ARITH || xtyp == SIGNED_NEG_ARITH) && 
           (ytyp == SIGNED_ARITH || ytyp == SIGNED_POS_ARITH || ytyp == SIGNED_NEG_ARITH))
  {
    z.supr = MAX4(x.infm * y.infm, x.infm * y.supr, x.supr * y.infm, x.supr * y.supr);
    z.infm = MIN4(x.infm * y.infm, x.infm * y.supr, x.supr * y.infm, x.supr * y.supr);
  }
  else
  {
    fprintf(stderr, "Error: Unknown ArithType arithmetic representation.\n");
    exit(1);
  }
  
  return (z);
}

/* IntervalDiv: 
 * Return the interval of the result of performing division (quotient only) 
 * between two integer intervals. xtyp, ytyp provide the arithmetic 
 * representation type for x and y, respectively.
 */
Interval IntervalDiv(Interval x, Interval y, ArithType xtyp, ArithType ytyp)
{
  Interval z;
  
  if (ValueIsInInterval(y, 0) == 1)
  {
    fprintf(stderr,"Error: Division with zero in IntervalDiv.\n");
    exit(1);
  } else if ((xtyp == UNSIGNED_ARITH) && (ytyp == UNSIGNED_ARITH))
  {
    z.supr = x.infm / y.supr;
    z.infm = x.supr / y.infm;
  }
  else if ((xtyp == SIGNED_ARITH || xtyp == SIGNED_POS_ARITH || xtyp == SIGNED_NEG_ARITH) && 
           (ytyp == SIGNED_ARITH || ytyp == SIGNED_POS_ARITH || ytyp == SIGNED_NEG_ARITH))
  {
    z.supr = 
      MAX4(x.infm / y.infm, x.infm / y.supr, x.supr / y.infm, x.supr / y.supr);
    z.infm = 
      MIN4(x.infm / y.infm, x.infm / y.supr, x.supr / y.infm, x.supr / y.supr);
  }
  else
  {
    fprintf(stderr, "Error: Unknown ArithType arithmetic representation.\n");
    exit(1);
  }
  
  return (z);
}

/* IntervalMod: 
 * Return the interval of the result of performing the modulus on two integer 
 * intervals. xtyp provides the arithmetic representation type for x.
 */
Interval IntervalMod(Interval x, Interval y, ArithType xtyp)
{
  Interval z;
  
  if (xtyp == UNSIGNED_ARITH)
  {
    z.supr = MAX(x.supr, MAX(y.supr,-y.infm)-1);
    z.infm = 0;
  }
  else if (xtyp == SIGNED_ARITH)
  {
    z.supr = MAX(MAX(x.supr,-x.infm),MAX(y.supr,-y.infm)-1);
    z.infm = -MIN(MAX(x.supr,-x.infm),MAX(y.supr,-y.infm)-1);
  }
  else
  {
    fprintf(stderr, "Error: Unknown ArithType arithmetic representation.\n");
    exit(1);
  }
  
  return (z);
}

/* IntervalMux: 
 * Return the interval of the result of z = ((a) relop (b) ? (x) : (y)), where 
 * relop is a relational operator: 
 * "==" (muxeq), "!=" (muxne), 
 * "<" (muxlt), "<=" (muxle), 
 * ">" (muxgt), ">=" (muxge)
 */
Interval IntervalMux(Interval x, Interval y)
{
  Interval z;
  z.supr = MAX(x.supr, y.supr);
  z.infm = MIN(x.infm, y.infm);
  return (z);
}

/* IntervalSet: 
 * Return the interval of the result of z = x relop y, where relop is a 
 * relational operator: 
 * "==" (seteq), "!=" (setne), 
 * "<" (setlt), "<=" (setle), 
 * ">" (setgt), ">=" (setge)
 */
Interval IntervalSet(Interval x, Interval y)
{
  Interval z;

  if ((IntervalIsEmpty(x) == 0) && (IntervalIsEmpty(y) == 0))
  {
    z = INTERVAL(0, 1);
  }
  else
  {
    fprintf(stderr, "Error: Empty input interval(s).\n");
    exit(1);
  }

  return (z);
}

/* IntervalAnd: 
 * Return the interval of the result of z = x AND y.
 */
Interval IntervalAnd(Interval x, Interval y)
{
  Interval z;
  z.supr = MAX(x.supr, y.supr);
  z.infm = MIN(x.infm, y.infm);
  return (z);
}

/* IntervalIor: 
 * Return the interval of the result of z = x IOR y.
 */
Interval IntervalIor(Interval x, Interval y)
{
  Interval z;
  z.supr = MAX(x.supr, y.supr);
  z.infm = MIN(x.infm, y.infm);
  return (z);
}

/* IntervalXor: 
 * Return the interval of the result of z = x XOR y.
 */
Interval IntervalXor(Interval x, Interval y)
{
  Interval z;
  z.supr = MAX(x.supr, y.supr);
  z.infm = MIN(x.infm, y.infm);
  return (z);
}

/* IntervalNot: 
 * Return the interval of the result of z = NOT x.
 */
Interval IntervalNot(Interval x)
{
  Interval z;
  return (z);
}

/* IntervalExpInteger: 
 * Return the interval of the result of z = x ** n (n-th integer power of x). 
 * n is an integer and its interval representation is [n,n].
 */
Interval IntervalExpInteger(Interval x, int n)
{
  Interval z;
  
  // if n is even and a >= 0 or if n is odd
  if ((!IS_ODD(n) && (x.infm >= 0)) || IS_ODD(n))
  {
    z.supr = pow(x.supr, n);
    z.infm = pow(x.infm, n);
  }
  // if n is even and b <= 0
  else if ((!IS_ODD(n) && (x.supr <= 0)) || IS_ODD(n))
  {
    z.supr = pow(x.infm, n);
    z.infm = pow(x.supr, n);
  }
  // otherwise
  else
  {
//    z.supr = POS_INFINITY;     // This is the ENTIRE interval [-INF,+INF]
//    z.infm = NEG_INFINITY;
  }
  
  return (z);
}

/* IntervalSqrt: 
 * Return the interval of the result of z = sqrt(x).
 */
Interval IntervalSqrt(Interval x)
{
  Interval z;
  
  // if all values in interval x are >= 0
  if ((x.infm >=0) && (x.supr >= 0))
  {
    z.supr = CEILING(sqrt(x.supr));
    z.infm = FLOOR(sqrt(x.infm));
  }
  // otherwise
  else
  {
    fprintf(stderr,"Error: Cannot compute IntervalSqrt for a non-positive interval.\n");
    exit(1);
  }
  
  return (z);
}

/* IntervalAbs: 
 * Return the interval of the result of computing the absolute value of interval 
 * x: 
 * z = abs(x).
 */
Interval IntervalAbs(Interval x)
{
  Interval z;
  z.supr = MAX(ABS(x.supr), ABS(x.infm));
  z.infm = 0;
  return (z);
}

/* IntervalMax: 
 * Return the interval of the result of computing the maximum value of intervals 
 * x and y: 
 * z = max(x, y).
 */
Interval IntervalMax(Interval x, Interval y)
{
  Interval z;
  z.supr = MAX(x.supr, y.supr);
  z.infm = MAX(x.infm, y.infm);
  return (z);
}

/* IntervalMin: 
 * Return the interval of the result of computing the minimum value of intervals 
 * x and y: 
 * z = min(x, y).
 */
Interval IntervalMin(Interval x, Interval y)
{
  Interval z;
  z.supr = MIN(x.supr, y.supr);
  z.infm = MIN(x.infm, y.infm);
  return (z);
}

/* IntervalUnion: 
 * Return the union (actually the so-called "interval hull" which produces a 
 * contiguous interval) of intervals x and y. The union operator formally 
 * produces two distinct intervals.
 */
Interval IntervalUnion(Interval x, Interval y)
{
  Interval z;
  z.supr = MAX(x.supr, y.supr);
  z.infm = MIN(x.infm, y.infm);
  return (z);
}

/* IntervalIntersection: 
 * Return the intersection of intervals x and y. In case the intersection of 
 * x and y is the empty interval, the [1,0] interval (the default empty 
 * interval) is returned.
 */
Interval IntervalIntersection(Interval x, Interval y)
{
  Interval z = IntervalEmpty();
  
  // Check if the intersection is an empty interval
  if (!IntervalIsEmpty(x) && !IntervalIsEmpty(y))
  {
    if ((x.supr < y.infm) || (y.supr < x.infm))
    {
      return (z);
    }
    else
    {
      z.supr = MIN(x.supr, y.supr);
      z.infm = MAX(x.infm, y.infm);
    }
  }
  
  return (z);
}

/* ValueIsInInterval: 
 * Query whether the given value v is in interval x or not.
 * Returns 1 if v is in x; 0 otherwise. 
 */
int ValueIsInInterval(Interval x, int v)
{
  int v_found = 0;
  
  if ((x.infm <= v) && (x.supr >= v))
  {
    v_found = 1;
  }  

  return (v_found);
}

/* IntervalIsEmpty: 
 * Query whether the given interval is an empty set (i.e. containing no values).
 * Returns 1 if the interval x is empty; 0 otherwise. 
 */
int IntervalIsEmpty(Interval x)
{
  int is_empty = 0;
  if (x.infm > x.supr)
  {
    is_empty = 1;
  }
  return (is_empty);
}

/* IntervalIsPositive: 
 * Query whether the given interval is strictly positive (i.e. lies in the 
 * domain of positive integers). The interval may contain integer ZERO.
 * Returns 1 if the interval x is positive; 0 otherwise. 
 */
int IntervalIsPositive(Interval x)
{
  int is_positive = 0;
  if ((x.infm >= 0) && (x.supr >= 0))
  {
    is_positive = 1;
  }
  return (is_positive);
}

/* IntervalIsNegative: 
 * Query whether the given interval is strictly negative (i.e. lies in the 
 * domain of negative integers). The interval may contain integer ZERO.
 * Returns 1 if the interval x is negative; 0 otherwise. 
 */
int IntervalIsNegative(Interval x)
{
  int is_negative = 0;
  if ((x.infm <= 0) && (x.supr <= 0))
  {
    is_negative = 1;
  }
  return (is_negative);
}

/* ValueToInterval: 
 * Convert a given integer value v to a degenerate interval of the form [v,v].
 * Returns the computed interval. 
 */
Interval ValueToInterval(int v)
{
  Interval z = INTERVAL(v,v);
  return (z);
}

/* IntervalBalanced: 
 * Given an "unbalanced" interval (of the form [m,n], where m!=n and m,n>0 or 
 * m<0<=n and |m|=n+1), it is converted to a "balanced" interval of the form 
 * [0,2**n-1] for unsigned or [-2**(n-1),2**(n-1)+1] for signed integer 
 * arithmetic.
 * xtyp provides the arithmetic type for the assumed integer arithmetic.
 */
Interval IntervalBalanced(Interval x, ArithType xtyp)
{
  Interval z;
  int n;
  
  if ((xtyp == UNSIGNED_ARITH) && (IntervalIsPositive(x) == 1))
  {
    z.supr = ABS(pow(2,log2ceil(x.supr))) - 1;
    z.infm = 0;
  }
  else if (xtyp == SIGNED_ARITH)
  {
    n = MAX(pow(2,log2ceil(ABS(x.supr))),pow(2,log2ceil(ABS(x.infm))));
    z.supr = pow(2,log2ceil(n)) - 1;
    z.infm = -pow(2,log2ceil(n));
  }
  else
  {
    fprintf(stderr, "Error: Unknown ArithType arithmetic representation.\n");
    exit(1);
  }
  
  return (z);
}

/* IntervalIsBalanced: 
 * Query whether the given interval is balanced, i.e. [0,2**n-1] for unsigned or 
 * [-2**(n-1),2**(n-1)+1] for signed integer arithmetic.
 * Returns 1 if the interval x is balanced; 0 otherwise. 
 */
int IntervalIsBalanced(Interval x, ArithType xtyp)
{
  int is_balanced = 0;
  
  if (xtyp == UNSIGNED_ARITH)
  {
    if ((x.infm == 0) && (x.supr == pow(2,IntervalToIntegerBitwidth(x, xtyp))-1))
    {
      is_balanced = 1;
    }
    else
    {
      is_balanced = 0;
    }
  }
  else if (xtyp == SIGNED_ARITH)
  {
    if ((x.infm == -pow(2,IntervalToIntegerBitwidth(x, xtyp))) && 
        (x.supr == pow(2,IntervalToIntegerBitwidth(x, xtyp))-1))
    {
      is_balanced = 1;
    }
    else
    {
      is_balanced = 0;
    }
  }
  else
  {
    fprintf(stderr, "Error: Integer representation should be SIGNED_ARITH or UNSIGNED_ARITH.\n");
    exit(1);
  }

  return (is_balanced);
}

/* IntervalIsSymmetric: 
 * Query whether the given interval is symmetric, i.e. [-n,n] for any given 
 * arithmetic (even a non fixed-point one).
 * Returns 1 if the interval x is symmetric; 0 otherwise. 
 * NOTE: For non-exact arithmetic representations, the comparison operation 
 * should be carefully designed.
 */
int IntervalIsSymmetric(Interval x)
{
  int is_symmetric = 0;
  
  if (x.infm == -x.supr)
  {
    is_symmetric = 1;
  }

  return (is_symmetric);
}

/* IntegerBitwidthToInterval: 
 * Convert the bitwidth of a signed (2's complement) or unsigned integer number 
 * to the corresponding interval. A bitwidth of n-bits would be converted to 
 * [0,2**n-1] for an unsigned integer or [-2**(n-1),2**(n-1)-1] for a signed 
 * integer. 
 * xtyp provides the arithmetic type for the assumed integer.
 */
Interval IntegerBitwidthToInterval(int n, ArithType xtyp)
{ 
  // FIXME: pow vs ipow (06/05/2010)
  Interval z;
  if (xtyp == UNSIGNED_ARITH)
  {
    z.supr = ipow(2,n) - 1;
    z.infm = 0;
  }
  else
  {
    z.supr = ipow(2,n-1) - 1;
    z.infm = -ipow(2,n-1);
  }
  
  return (z);
}

/* IntervalToIntegerBitwidth: 
 * Convert the given interval to the corresponding minimum bitwidth necessary 
 * for the representation of signed (2's complement) or unsigned integers. 
 * xtyp provides the arithmetic type for the assumed integer representation.
 */
int IntervalToIntegerBitwidth(Interval x, ArithType xtyp)
{
  int n;
  int xrange;
  
  // Calculate the value range of the interval.
  if (IntervalIsEmpty(x) == 1)
  {
    xrange = 0;
  }
  else 
  {
    xrange = x.supr - x.infm + 1;
  }
  
  if (xrange == 0)
  {
    n = 0;
    fprintf(stderr, "Error: Unable to compute bitwidth for an empty interval.\n");
    exit(1);
  }
  else
  {
    // Compute the bitwidth for unsigned and signed integer representation.
    if ((xtyp == UNSIGNED_ARITH) || (xtyp == SIGNED_ARITH))
    {
      // Find the binary logarithm (integer ceiling) of the range of interval x.
      n = log2ceil(xrange);
    }
    else
    {
      fprintf(stderr, "Error: Integer representation should be SIGNED_ARITH or UNSIGNED_ARITH.\n");
      exit(1);
    }
  }

  return (n);  
}

/* IntervalPrint: 
 * Print the specified interval to outfile. 
 */
void IntervalPrint(FILE *outfile, Interval x)
{
  fprintf(outfile, "[%d,%d]", x.infm, x.supr);
//  fprintf(outfile, "[%dll,%dll]", x.infm, x.supr);
}

/* utils.c: Functions ported from utils.c. */

/* log2ceil:
 * Function to calculate the ceiling of the binary logarithm of a given positive 
 * integer n.
 */
int log2ceil(int inpval)
{
  unsigned int max = 1; // exp=0 => max=2^0=1
  unsigned int logval = 0;

  if (inpval < 0) {
    fprintf(stderr, "Error: Result of log2 computation is NAN.\n");
    exit(1);
  }
  else if (inpval == 0) {
    fprintf(stderr, "Error: Result of log2 computation is MINUS_INFINITY.\n");
    exit(1);
  }
  // inpval is positive
  else {
    // log computation loop
    while (max < inpval) {
      // increment exponent
      logval = logval + 1;
      //  max = 2^logval
      max = max * 2;
    }
  }
  // exponent that gives (2^logval) >= inpval
  return (logval);
}

/* ipow:
 * Simple approach to compute an integer powering function.
 */
int ipow(int base, int exponent)
{
  int temp;
  int i;
  
  temp = 1;  
  for (i = 0; i < exponent; i++) {
     temp *= base; 
  }
  return (temp);
}

