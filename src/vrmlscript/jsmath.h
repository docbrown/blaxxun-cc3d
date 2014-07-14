/*
 * JsMath.h
 *
 *      Declaration of classes:
 *        JsMath
 *
 * Function support
 * 
 * Copyright (C) 1996, Silicon Graphics,  Inc.
 */
 
#ifndef _JS_MATH_
#define _JS_MATH_

#include <math.h>
#include <stdlib.h>

#ifdef WIN32
#undef max
#undef min
#endif

// HG 
#ifdef PI
#undef PI
#endif 

// Class that implements the JavaScript Math object

class JsMath {
  public:
    static double abs(double x)			{ return ::fabs(x); }
    static double acos(double x)		{ return ::acos(x); }
    static double asin(double x)		{ return ::asin(x); }
    static double atan(double x)		{ return ::atan(x); }
    static double ceil(double x)		{ return ::ceil(x); }
    static double cos(double x)			{ return ::cos(x); }    
    static double cosh(double x)		{ return ::cosh(x); }    
    static double exp(double x)			{ return ::exp(x); }
    static double floor(double x)		{ return ::floor(x); }
    static double log(double x)			{ return ::log(x); }
    static double log10(double x)		{ return ::log10(x); }
    static double max(double x, double y)	{ if (x>y) 
						    return x;
						  else
						    return y;
						}
    static double min(double x, double y)	{ if (x<y)
						    return x;
						  else
						    return y;
						}
    static double pow(double base, double exp)	{ return ::pow(base, exp); }
#if defined(WIN32) || defined(_MSC_VER)
    static double random()			{ return ((double)::rand()) / (double) RAND_MAX; }
#else
    static double random()			{ return ::drand48(); }
#endif
    static double round(double x)		{ if ((x - (long)x) < 0.5) 
						    return ::floor(x);
						  else
						    return ::ceil(x);
						}
    static double sin(double x)			{ return ::sin(x); }
    static double sinh(double x)		{ return ::sinh(x); }
    static double sqrt(double x)		{ return ::sqrt(x); }
    static double tan(double x)			{ return ::tan(x); }
    static double tanh(double x)		{ return ::tanh(x); }

    static double atan2(double a, double b)	{ return ::atan2(a, b); }

    /**
     * Generates a pseudorandom Gaussian distributed 
     * <CODE>double</CODE> value with mean 0.0 and standard 
     * deviation 1.0.
     * @return a Gaussian distributed <CODE>double</CODE>.
     */
	static double nextNextGaussian;
    static gbool haveNextNextGaussian; //  = false;

    static double randomGaussian() 
	{
        // See Knuth, ACP, Section 3.4.1 Algorithm C.
        if (haveNextNextGaussian) {
    	    haveNextNextGaussian = false;
    	    return nextNextGaussian;
    	} else {
            double v1, v2, s;
    	    do { 
                v1 = 2 * random() - 1; // between -1 and 1
            	v2 = 2 * random() - 1; // between -1 and 1 
                s = v1 * v1 + v2 * v2;
    	    } while (s >= 1);
    	    double multiplier = sqrt(-2 * log(s)/s);
    	    nextNextGaussian = v2 * multiplier;
    	    haveNextNextGaussian = true;
    	    return v1 * multiplier;
        }
	}
    
    static double E;
    static double LN10;
    static double LN2;
    static double PI;
    static double SQRT1_2;
    static double SQRT2;
};    

#endif /* _JS_MATH_ */
