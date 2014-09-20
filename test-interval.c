/*
 * Filename: test-interval.c
 * Purpose : Test the interval ADT implementation.
 * Author  : Nikolaos Kavvadias (C) 2009, 2010, 2011, 2012, 2013, 2014
 * Date    : 20-Sep-2014
 * Revision: 0.2.0 (20/09/14)
 *           Minor addition to utilize unused variable c.
 *           0.1.0 (22/07/09)
 *           Initial version.
 */    

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "interval.h"

/* main:
 */
int main (void)
{ 
  Interval a, b, c, d, e, f;
  int i, n;
  
  // Construct the input intervals
  a = INTERVAL(0, 1);
  b = INTERVAL(2, 5);
  c = INTERVAL(7, 7);
  d = INTERVAL(-8, 7);
    
  // Test the basic operations on intervals
  e = IntervalAdd(a, b);
  printf("Testing IntervalAdd: ");
  IntervalPrint(stdout, a);
  printf(" + ");
  IntervalPrint(stdout, b);
  printf(" = ");
  IntervalPrint(stdout, e);
  printf("\n");

  e = IntervalAdd(a, c);
  printf("Testing IntervalAdd: ");
  IntervalPrint(stdout, a);
  printf(" + ");
  IntervalPrint(stdout, c);
  printf(" = ");
  IntervalPrint(stdout, e);
  printf("\n");
  
  e = IntervalSub(a, a);
  printf("Testing IntervalSub: ");
  IntervalPrint(stdout, a);
  printf(" - ");
  IntervalPrint(stdout, a);
  printf(" = ");
  IntervalPrint(stdout, e);
  printf("\n");

  e = IntervalNeg(a);
  printf("Testing IntervalNeg: ");
  printf("neg ");
  IntervalPrint(stdout, a);
  printf(" = ");
  IntervalPrint(stdout, e);
  printf("\n");
  
  e = IntervalMul(b, d, SIGNED_ARITH, SIGNED_ARITH);
  printf("Testing IntervalMul: ");
  IntervalPrint(stdout, b);
  printf(" * ");
  IntervalPrint(stdout, d);
  printf(" = ");
  IntervalPrint(stdout, e);
  printf("\n");
 
  f = INTERVAL(25, 178);
  e = IntervalDiv(f, b, UNSIGNED_ARITH, UNSIGNED_ARITH);
  printf("Testing IntervalDiv for UNSIGNEDs: ");
  IntervalPrint(stdout, f);
  printf(" / ");
  IntervalPrint(stdout, b);
  printf(" = ");
  IntervalPrint(stdout, e);
  printf("\n");

  f = INTERVAL(25, 178);
  e = IntervalDiv(f, b, SIGNED_ARITH, SIGNED_ARITH);
  printf("Testing IntervalDiv for SIGNEDs: ");
  IntervalPrint(stdout, f);
  printf(" / ");
  IntervalPrint(stdout, b);
  printf(" = ");
  IntervalPrint(stdout, e);
  printf("\n");

  f = INTERVAL(25, 178);
  e = IntervalMod(f, b, UNSIGNED_ARITH);
  printf("Testing IntervalMod for UNSIGNEDs: ");
  IntervalPrint(stdout, f);
  printf(" / ");
  IntervalPrint(stdout, b);
  printf(" = ");
  IntervalPrint(stdout, e);
  printf("\n");
  
  f = INTERVAL(25, 178);
  e = IntervalMod(f, b, SIGNED_ARITH);
  printf("Testing IntervalMod for SIGNEDs: ");
  IntervalPrint(stdout, f);
  printf(" / ");
  IntervalPrint(stdout, b);
  printf(" = ");
  IntervalPrint(stdout, e);
  printf("\n");

  f = INTERVAL(15, 244);
  e = IntervalSqrt(f);
  printf("Testing IntervalSqrt: ");
  printf("sqrt ");
  IntervalPrint(stdout, f);
  printf(" = ");
  IntervalPrint(stdout, e);
  printf("\n");

  f = INTERVAL(-32, 63);
  e = IntervalAbs(f);
  printf("Testing IntervalAbs: ");
  printf("abs ");
  IntervalPrint(stdout, f);
  printf(" = ");
  IntervalPrint(stdout, e);
  printf("\n");

  e = IntervalMax(a, b);
  printf("Testing IntervalMax: ");
  printf("max "); 
  IntervalPrint(stdout, a);
  printf(" , ");
  IntervalPrint(stdout, b);
  printf(" = ");
  IntervalPrint(stdout, e);
  printf("\n");

  e = IntervalMin(a, b);
  printf("Testing IntervalMin: ");
  printf("min "); 
  IntervalPrint(stdout, a);
  printf(" , ");
  IntervalPrint(stdout, b);
  printf(" = ");
  IntervalPrint(stdout, e);
  printf("\n");

  i = 11;
  e = IntegerBitwidthToInterval(i, UNSIGNED_ARITH);
  printf("Testing IntegerBitwidthtoInterval for UNSIGNEDs: ");
  printf("BW2IA "); 
  printf("%d ", i);
  printf(" = ");
  IntervalPrint(stdout, e);
  printf("\n");
  
  i = 24;
  e = IntegerBitwidthToInterval(i, SIGNED_ARITH);
  printf("Testing IntegerBitwidthtoInterval for SIGNEDs: ");
  printf("BW2IA "); 
  printf("%d ", i);
  printf(" = ");
  IntervalPrint(stdout, e);
  printf("\n");

  f = INTERVAL(0, 1023);
  n = IntervalToIntegerBitwidth(f, UNSIGNED_ARITH);
  printf("Testing IntervalToIntegerBitwidth for UNSIGNEDs: ");
  printf("IA2BW "); 
  IntervalPrint(stdout, f);
  printf(" = ");
  printf("%d ", n);
  printf("\n");
  
  f = INTERVAL(-64, 7);
  n = IntervalToIntegerBitwidth(f, SIGNED_ARITH);
  printf("Testing IntervalToIntegerBitwidth for SIGNEDs: ");
  printf("IA2BW "); 
  IntervalPrint(stdout, f);
  printf(" = ");
  printf("%d ", n);
  printf("\n");

  f = INTERVAL(5, 38);
  e = IntervalBalanced(f, UNSIGNED_ARITH);
  printf("Testing IntervalBalanced for UNSIGNEDs: ");
  printf("BALANCE "); 
  IntervalPrint(stdout, f);
  printf(" = ");
  IntervalPrint(stdout, e);
  printf("\n");
  
  f = INTERVAL(-65, 121);
  e = IntervalBalanced(f, SIGNED_ARITH);
  printf("Testing IntervalBalanced for SIGNEDs: ");
  printf("BALANCE "); 
  IntervalPrint(stdout, f);
  printf(" = ");
  IntervalPrint(stdout, e);
  printf("\n");
  
  return 0;
}
