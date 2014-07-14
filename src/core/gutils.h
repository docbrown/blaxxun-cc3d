/*=============================================================================

This code is licensed under the Web3D-blaxxun Community Source License,
provided in distribution file LICENSE.TXT and available online at
http://www.web3D.org/TaskGroups/x3d/blaxxun/Web3D-blaxxunCommunitySourceAgreement.html
and may only be used for non-commercial use as specified in that license.

THE WEB3D CONSORTIUM AND BLAXXUN DO NOT MAKE AND HEREBY DISCLAIM ANY EXPRESS
OR IMPLIED WARRANTIES RELATING TO THIS CODE, INCLUDING BUT NOT LIMITED TO,
WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY OR FITNESS FOR A PARTICULAR
PURPOSE, OR ANY WARRANTIES THAT MIGHT ARISE FROM A COURSE OF DEALING, USAGE
OR TRADE PRACTICE.  THE COMMUNITY SOURCE CODE IS PROVIDED UNDER THIS
AGREEMENT "AS IS," WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED,
INCLUDING, WITHOUT LIMITATION, WARRANTIES THAT THE COMMUNITY SOURCE CODE ARE
FREE OF DEFECTS, MERCHANTABLE, FIT FOR A PARTICULAR PURPOSE OR
NON-INFRINGING OR IN ANY WAY CONSTITUTE THE COMPLETE PRODUCT MARKETED UNDER
THE NAMES GIVEN SAID CODE.

==============================================================================*/
#ifndef _gutils_h
#define _gutils_h
/******************************************************************************
@doc

@module GUtils.h - GLView utility routines	|

Copyright (c) 1995	by Holger Grahn
All rights reserved

Purpose:
genric type defines

Template functions for min swap
PI ...
DEG2RAD

Classes:

Notes:

Changes:

$Revision: 1.9 $
$Log: gutils.h,v $
Revision 1.9  1999/07/06 16:55:13  tom
*** empty log message ***


Todo :

******************************************************************************/


// generic type defines

// unicode capable char type
#ifdef WIN32

typedef TCHAR gchar;

#else

typedef char gchar;

#endif

#define GFLOAT_IS_DOUBLE 0
#define GFLOAT_IS_FLOAT 1

typedef float gfloat;
typedef double gdouble;
typedef bool gbool;

#define gtrue true
#define gfalse false


// floating point control
//! init floating point control 
void GFPInit();

// terminitate floating point control
void GFPTerm();

// reset floating point control word, might be changed by other components
void GFPSetControl();

#ifdef _M_IX86

// C++ RTL library float to int conversion is very slow
// because it changes FPU rounding mode

inline int float2int(float f)
{
  int i;
  __asm {
    fld f
    fistp i
  }
  return i;
}

inline int float2int(double f)
{
  int i;
  __asm {
    fld f
    fistp i
  }
  return i;
}

#else

inline int float2int(float f)
{
  return (int)f;
}

inline int float2int(double f)
{
  return (int)f;
}

#endif

// float / double to int 
template<class T>
inline int
toInt(T a) { return( (int) a); }





#ifdef max
#undef max
#endif
#ifdef min
#undef min
#endif

//inline float max(float a,float b) { return( (a>b) ? a : b); }
//inline int max(int a,int b) { return( (a>b) ? a : b); }
//inline int max(int a,int b,int c) { return( (a>b) ? max(a,c) : max(b,c)); }
//inline float sqr(float a) { return(a*a); }

template<class T>
inline void
Swap(T& x, T& y) { T temp = x; x = y; y = temp; }

template<class T>
inline T
max(const T a, const T b) { return( (a>b) ? a : b); }


template<class T>
inline T
min(const T a, const T b) { return( (a<b) ? a : b); }



// limit a to range mi .. ma
template<class T>
inline T
limit(const T a, const T mi,const T ma) 
{ 
    if (a<mi) return(mi);
    return( (a>ma) ? ma : a); 
}


//template<class T>
//inline T
//min(const T& a, const T& b) { return( (a<b) ? a : b); }

template<class T>
inline T
sqr(const T a) { return (a*a); }

inline float max(float a,float b,float c) { return( (a>b) ? max(a,c) : max(b,c)); }


/* PI, DEG, RAD */
#ifndef _PI
#define _PI 3.141592653589793238462643383279502884197169399
#endif

#define Pi	_PI
#define PI	_PI
#define TWOPI	 (2*PI)
#define DEG(ANG) ((ANG)*PI/180.0)
#define DEG2RAD(ANG) ((ANG)*PI/180.0)
#define RAD2DEG(ANG) ((ANG)*180.0/PI)

const double Deg2Rad= PI/180.0;
const double Rad2Deg = 180.0/PI;


#define FLOAT_MAX (float)1e30

#define ELEMENTS(array) (sizeof(array)/sizeof((array)[0]))


#if 1
#define MALLOCT(NELEMENTS,OFTYPE)  (OFTYPE *) MALLOC(((NELEMENTS) * sizeof(OFTYPE)))
#define CALLOCT(NELEMENTS,OFTYPE)  (OFTYPE *) CALLOC(NELEMENTS, sizeof(OFTYPE))
#define REALLOCT(PTR,NELEMENTS,OFTYPE)  (OFTYPE *) REALLOC(PTR,(NELEMENTS) * sizeof(OFTYPE))
#define FREET(PTR) FREE(PTR)
#endif


#define EPS 0.000009
// #define iszero(f)  (((f)<0.000009)&&((f)> -0.000009))

inline gbool iszero(float f)  { return (((f)<0.000009)&&((f)> -0.000009)); }

// a cectangle in position size form
// x+dx is part of rectangle
class GRect {

public:

	float x,y,dx,dy;

	inline gbool IsEmpty() const { return(dx <=0 || dy <= 0); }

    //@cmember is Point p inside ?
    inline gbool Inside(float px,float py) const {
				return( (px>=x) && (px <= (x+dx))
						&& (py>=y) && (py <= (y+dy))
						);

	}

    //@cmember is Point p outside ?
	inline gbool Outside(float px,float py) const {
				return( (px < x) || (px  > (x+dx))
						|| (py < y) || (py  > (y+dy))
						);
	}

};

// a rectangle in position lower left +  size form
// note :  points x+dx,y+dy are not part of rectangle
//

class GRectInt {

public:

	int x,y,dx,dy;

	// the x coordinate of the right point (not part of rectangle)
	int Right() const { return x+dx; }
	int Top() const { return y+dy; }


	inline gbool IsEmpty() const { return(dx <=0 || dy <= 0); }

    //@cmember is Point p inside ?
    inline gbool Inside(int px,int py) const {
				return( (px>=x) && (px < (x+dx))
						&& (py>=y) && (py < (y+dy))
						);

	}

    //@cmember is Point p outside ?
	inline gbool Outside(int px, int py) const {
				return ((px < x) || (px  >= (x+dx))
						|| (py < y) || (py  >= (y+dy))
						);
	}

    //@cmember is BBox b outside ?
    inline gbool  Outside(const GRectInt &b) const {
				return ((b.Right() <= x) || (b.x >= Right())
						|| (b.Top() <= y) || (b.y >= Top())
						);

	};

};


void copylist(int *dest,const int *src,int cnt);

// append appendlist to list,
int append(int * &list,int & nlist, int & listmax, const int *appendlist,int nappendlist);

// return random number between 0 and 1
inline float Rnd() {   return ((float) rand() / RAND_MAX); }


//\@class Template PtrHandle class
//\@tcarg class | T | A class to maintain a pointer to a allocted datastructure

template<class T>
class PtrHandle {
protected :
	T* p;		// Pointer to data

public:
	// constructors
	PtrHandle() { p = 0;}
	PtrHandle(T* newP) { p = newP; }
	
	~PtrHandle() { Delete(); }
	
	void Delete() { if(p) { delete p; p = NULL; } }

	
	operator T*() { return(p); }
	
	PtrHandle<T>& operator= (T* newP) { 
		if (p!=newP) {
			Delete();
			p = newP; 
		}
		return(*this);
	}
	

};



// string utils 

// #define streq(a,b) (strcmp(a,b) == 0)
inline int streq(const char *a, const char *b)  { return strcmp(a,b) == 0; }
inline int strgt(const char *a, const char *b)  { return strcmp(a,b) >  0; }
inline int strlt(const char *a, const char *b)  { return strcmp(a,b) <  0; }
inline int strneq(const char *a, const char *b) { return strcmp(a,b) != 0; }

inline int strieq(const char *a, const char *b)  { return _stricmp(a,b) == 0; }


// check if string prefix occurs at the beggin of str
// return 1 if prefix, 0 if not 
int strprefix(const char *str, const char * prefix);
int striprefix(const char *str, const char * prefix);


// return true if c is the last char of s
int strsuffix(const char *s,char c);


// return entry in s, where suffix is at end
// null if s has not suffix
const char* strsuffix(const char *s,const char *suffix);
const char* strisuffix(const char *s,const char *suffix);


// get the first index of search in string, -1 if not found
int indexOf(const char *string,const char *search,int fromIndex);

// get the last index of search in string, -1 if not found
int lastIndexOf(const char *string,const char *search,int fromIndex);

// set out to the subsing of s[index1:index2]
// out must be large enought, empty string if bad data
char * setSubstring(const char *s, int index1, int index2,char *out);

// replace max cnt occurences of search in string by replace 
// out must be large enough
char * replace(const char *string,
			   const char *search,const char *replace,
			   int cnt,
			   char *out);

char * toLowerCase(char *string);
char * toUpperCase(char *string); 

//! parse a boolean strings
//! true TRUE on ON ==> true, else false 
gbool parseBool(const gchar* s);


//! mapping a define/enum value to a string 
typedef struct {
	int i;	//!< the value
	char *s; //!< read only string 
	} IS;


//! Lookup string for value in table 
//! return NULL if value not found 
const char * Lookup(const IS *table,int v);

//! Lookup value given string
//! Result -1 if not string not found
int Lookup(const IS *table,const char *v);


// UTF8 helpers
//actually meaning full if not compiled with fully unicode support 

// bad encoding
#define	UTF_INVALID	0x80000000

// EOF
#define UTF_EOF ((unsigned int)EOF)

//! get next character from utf8 encoded string and increment pointer 
unsigned int utf8nextcinc(const unsigned char * &strptr);

//! get length of utf8 encoded string 
int utf8strlen(const unsigned char * strptr); 

/*!
reads a sequence of one or more characters from a UTF-8 formatted string,
which it converts to a single UTC-4 (Unicode) value.
*/
unsigned int sgetu8(const unsigned char *strptr, int *chars);

/*
function of utf-8 library converts a UTC-4 (31-bit Unicode) integer to a sequence 
of one or more characters representing its UTF-8 value, and writes the result to 
a buffer (character string). It appends a NUL at the end of the buffer, so it can be
processed as a standard C string.
*/

char * sputu8(unsigned int c, char *strptr);
 


 
 


#endif




