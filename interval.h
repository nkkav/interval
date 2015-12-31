/*
 * Filename: interval.h
 * Purpose : Interval ADT definitions and function prototypes.
 * Author  : Nikolaos Kavvadias (C) 2009, 2010, 2011, 2012, 2013, 2014, 2015
 *                                  2016
 * Date    : 28-Oct-2014
 * Revision: 0.2.1 (14/10/28)
 *           Added names to prototype parameters.
 *           0.2.0 (14/09/20)
 *           Added code from genmacros.h and utils.h in order to be 
 *           self-contained.
 *           Initial version.
 *           0.1.1 (09/07/23)
 *           Coping with modifications in "interval.c".
 */  
#ifndef INTERVAL_H
#define INTERVAL_H

/* genmacros.h: Arithmetic processing macros. */
#define IS_ODD(x)         ((x) & 0x1)
#define ABS(x)            ((x) >  0 ? (x) : (-x))
#define MAX(x, y)         ((x) >  (y) ? (x) : (y))
#define MIN(x, y)         ((x) <  (y) ? (x) : (y))
#define MIN3(x, y, z)     MIN(MIN((x), (y)), MIN((y), (z)))
#define MAX3(x, y, z)     MAX(MAX((x), (y)), MAX((y), (z)))
#define MIN4(x, y, z, a)  MIN(MIN3((x), (y), (z)), MIN3((y), (z), (a)))
#define MAX4(x, y, z, a)  MAX(MAX3((x), (y), (z)), MAX3((y), (z), (a)))
#define SIGNUM(x)         ((x) > 0 ? 1 : ((x) < 0 ? -1 : 0 ))
#define FLOOR(x)          ((int) (x))
#define CEILING(x)        ((x)==FLOOR(x) ? FLOOR(x) : SIGNUM(x)+FLOOR(x))

/* Ported from utils.h: Function prototypes. */
int   log2ceil(int);
int   ipow(int, int);

/* Struct definition of the interval entity. 
 * NOTE: The int should be replaced at a later point by an arbitrary-sized 
 * integer data type.
 */
typedef struct { 
  int supr;        /* Minimum (infimum) value bound of the interval. */
  int infm;        /* Maximum (supremum) value bound of the interval. */
} Interval;

/* ArithType specifies the arithmetic representation of an integer item. */
typedef enum {
  UNKNOWN_ARITH_TYPE = -1,    /* Unknown type */
  UNSIGNED_ARITH,             /* Unsigned integer */
  SIGNED_ARITH,               /* Signed (2's-complement) integer */
  SIGNED_POS_ARITH,           /* Signed and guaranteed to be positive */
  SIGNED_NEG_ARITH            /* Signed and guaranteed to be negative */
} ArithType;

/* Interval ADT API */
Interval INTERVAL(int u, int v);
Interval IntervalCopy(Interval x);
Interval IntervalEmpty(void);
Interval IntervalUniverse(int bw, ArithType ztyp);
Interval IntervalClamp(Interval x, int lo, int hi);
Interval IntervalAdd(Interval x, Interval y);
Interval IntervalSub(Interval x, Interval y);
Interval IntervalNeg(Interval x);
Interval IntervalMul(Interval x, Interval y, ArithType xtyp, ArithType ytyp);
Interval IntervalDiv(Interval x, Interval y, ArithType xtyp, ArithType ytyp);
Interval IntervalMod(Interval x, Interval y, ArithType xtyp);
Interval IntervalMux(Interval x, Interval y);
Interval IntervalSet(Interval x, Interval y);
Interval IntervalAnd(Interval x, Interval y);
Interval IntervalIor(Interval x, Interval y);
Interval IntervalXor(Interval x, Interval y);
Interval IntervalNot(Interval x);
Interval IntervalExpInteger(Interval x, int n);
Interval IntervalSqrt(Interval x);
Interval IntervalAbs(Interval x);
Interval IntervalMax(Interval x, Interval y);
Interval IntervalMin(Interval x, Interval y);
Interval IntervalUnion(Interval x, Interval y);
Interval IntervalIntersection(Interval x, Interval y);
int      ValueIsInInterval(Interval x, int v);
int      IntervalIsEmpty(Interval x);
int      IntervalIsPositive(Interval x);
int      IntervalIsNegative(Interval x);
Interval ValueToInterval(int v);
Interval IntervalBalanced(Interval x, ArithType xtyp);
int      IntervalIsBalanced(Interval x, ArithType xtyp);
int      IntervalIsSymmetric(Interval x);
Interval IntegerBitwidthToInterval(int n, ArithType xtyp);
int      IntervalToIntegerBitwidth(Interval x, ArithType xtyp);
void     IntervalPrint(FILE *outfile, Interval x);

#endif /* INTERVAL_H */
