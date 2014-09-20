/*
 * Filename: interval.h
 * Purpose : Interval ADT definitions and function prototypes.
 * Author  : Nikolaos Kavvadias (C) 2009, 2010, 2011, 2012, 2013, 2014
 * Date    : 20-Sep-2014
 * Revision: 0.2.0 (20/09/14)
 *           Added code from genmacros.h and utils.h in order to be 
 *           self-contained.
 *           Initial version.
 *           0.1.1 (23/07/09)
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

/* utils.h: Function prototypes. */
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
Interval INTERVAL(int, int);
Interval IntervalCopy(Interval);
Interval IntervalEmpty(void);
Interval IntervalUniverse(int, ArithType);
Interval IntervalClamp(Interval, int, int);
Interval IntervalAdd(Interval, Interval);
Interval IntervalSub(Interval, Interval);
Interval IntervalNeg(Interval);
Interval IntervalMul(Interval, Interval, ArithType, ArithType);
Interval IntervalDiv(Interval, Interval, ArithType, ArithType);
Interval IntervalMod(Interval, Interval, ArithType);
Interval IntervalMux(Interval, Interval);
Interval IntervalSet(Interval, Interval);
Interval IntervalAnd(Interval, Interval);
Interval IntervalIor(Interval, Interval);
Interval IntervalXor(Interval, Interval);
Interval IntervalNot(Interval);
Interval IntervalExpInteger(Interval, int);
Interval IntervalSqrt(Interval);
Interval IntervalAbs(Interval);
Interval IntervalMax(Interval, Interval);
Interval IntervalMin(Interval, Interval);
Interval IntervalUnion(Interval, Interval);
Interval IntervalIntersection(Interval, Interval);
int      ValueIsInInterval(Interval, int);
int      IntervalIsEmpty(Interval);
int      IntervalIsPositive(Interval);
int      IntervalIsNegative(Interval);
Interval ValueToInterval(int);
Interval IntervalBalanced(Interval, ArithType);
int      IntervalIsBalanced(Interval, ArithType);
int      IntervalIsSymmetric(Interval);
Interval IntegerBitwidthToInterval(int, ArithType);
int      IntervalToIntegerBitwidth(Interval, ArithType);
void     IntervalPrint(FILE *, Interval);

#endif /* INTERVAL_H */
