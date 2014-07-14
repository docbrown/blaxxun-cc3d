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
/******************************************************************************
@doc
@module PointA.cpp - Pointarray classes implementation	|

Copyright (c) 1995	by Holger Grahn
All rights reserved

Purpose:
Methods for
	BasicPointArray
  	PointArray
	PointCurve


Classes:

Notes:

Changes:

Todo :
	change to malloc/realloc ?

******************************************************************************/


#include "stdafx.h"

//#include <iostream.h>
//#include "point.h"
//#include "pointa.h"

//#include "astream.h"

#if 0
BasicPointArray::BasicPointArray(int n)
{ maxp = np = n; p = new Point[np];
};

BasicPointArray::BasicPointArray(int n, const Point &iT)
{
  register int i = maxp = np = n;
  register Point* dst = p = new Point[maxp];
  while(i--) *dst++ = iT;
};


BasicPointArray::BasicPointArray(const BasicPointArray& a)
{
  register int i= maxp = np = a.np;
  register Point* dst = p = new Point[maxp];
  register const Point* src = a.p;
  if (src) while (i--) *dst++ = *src++;
};


BasicPointArray::BasicPointArray(int n, const Point *iT)
{
  register int i= maxp = np = n;
  register Point* dst = p = new Point[maxp];
  register const Point* src = iT;
  if (src) while (i--) *dst++ = *src++;
};

BasicPointArray::~BasicPointArray()
{ delete[] p; };

void BasicPointArray::Flush()
{ delete[] p; maxp=np=0; p = 0; };


BasicPointArray&
BasicPointArray::Set(const BasicPointArray& a)
{
  if(this != &a){
	Resize(a.np);
    register int i=a.np;
    register Point* dst = p ;
    register const Point* src = a.p;
    if (src) while (i--) *dst++ = *src++;
  }
  return *this;
};

BasicPointArray&
BasicPointArray::Set(int n, const Point *iT)
{
  if (p != iT) {
	Resize(n);
    register int i=n;
    register Point* dst = p ;
    register const Point* src = iT;
    if (src) while (i--) *dst++ = *src++;
  } else Resize(n);
  return *this;
};

BasicPointArray&
BasicPointArray::Set(int n, const Point &iT)
{
  Resize(n);
  register int i=n;
  register Point* dst = p ;
  while (i--) *dst++ = iT;
  return *this;
};



BasicPointArray&
BasicPointArray::operator=(const BasicPointArray& a)
{
  	Set(a);
    return *this;
};


// resize Array to hold elements [0..N-1]
// if exact, buffer size is really N, otherwise buffer may larger
//
void
BasicPointArray::Resize(int N,int exact)
{
  if (!exact) {
     if (N<=maxp) {  // new size fits in buffer
	    np=N;		 // set length
	    return;
     }
  } else {
    if (N==maxp) {
	np=N;
	return;
	}
  }

  Point* newArray = new Point[N];		// allocate new buffer
  register int i = (N<=np) ? N:np;
  register Point* src = p;
  register Point* dst = newArray;
  // COPY all
  if (src) {
    while (i--) *dst++ = *src++;
    delete[] p;
  }
  p = newArray;
  maxp=np = N;
};


void
BasicPointArray::Swap(int i,int j)
{ Point  tmp(p[i]);
  p[i] = p[j];
  p[j] = tmp;
};


BasicPointArray&
BasicPointArray::Append(const BasicPointArray& a)
{
  register int i= a.np;
  register Point* dst;
  register Point* src = a.p;
  int start = np;
  Resize(np+a.np);	// expand me
  dst = & p[start];	// point after old end
  while (i--) *dst++ = *src++;
  return(*this);
};

/* change to inline 

BasicPointArray&
BasicPointArray::Append(const Point &a,int expand)
{
  register int i= np;
  if (expand) {
	   if (np==maxp) {  // make array bigger
		 Resize((np+8) + (np/2));
		 np=i+1;
	   }

  }
  else Resize(np+1); // expand me
  p[i]	= a;
  return(*this);
};

*/

BasicPointArray&
BasicPointArray::Insert(int ofs, const BasicPointArray& a)
{
  register int i;
  register Point* dst;
  register Point* src;
  int start = np;

  if (!a.p) return(*this);
  if (ofs <0) ofs = np-ofs;
  if (ofs<0) ofs = 0;
  Resize(np+a.np);	// expand me

  i = start-ofs;
  // shift right to make space
  dst = & p[ofs+a.np +i-1];
  src = & p[ofs+ i-1];
  while (i--) *dst-- = *src --;

  // copy over a
  dst = & p[ofs];
  src = a.p;
  i= a.np;
  while (i--) *dst++ = *src++;
  return(*this);
};


BasicPointArray&
BasicPointArray::Delete(int ofs, int cnt)
{
  register int i;
  register Point* dst;
  register Point* src;

  if (!p) return(*this); /* empty */
  if (ofs <0) ofs = np-ofs;
  if (ofs<0) ofs = 0;
  if (cnt <0) cnt = np -ofs; // delete to end
  if ((ofs+cnt) > np) {
	// Error invalid numbers
	cnt = np -ofs;
  }

  // shift left
  dst = & p[ofs];
  src = & p[ofs+cnt];
  i= np-ofs-cnt;
  while (i--) *dst++ = *src++;
  Resize(np-cnt);  // make smaller
  return(*this);
}

#endif

/*
void  BasicPointArray::print(ostream &os)
{
  register int i = np;
  register Point* dst = p ;
  os << "(";
  while(i--)  {
     os << *dst++;
     if (i>0) os << ", ";
  }
  os << ")";
};
*/

int BasicPointArray::io(AStream &a)
{ int i;

  Point newpoint;

  if (a.BeginList()) {
   if (a.isOutput()) {
     for(i=0;i<np;i++) {
	    if (i>0) a.ListSep();
	    if ((i%5) == 4) a.nl();
	    p[i].io(a);
     }
   } else {
     np=0;
     /* read while ( until closing paren */
     while (a.Skip() != a.list_close) {			// to do: check for bad status 
	   if (a.eof) break;

	   newpoint.io(a);
	   if (np==maxp) {
		 // make array bigger
		 SetMaxLength((np+8) + (np/2));
		 //TRACE("Point array: expanding from %d to %d\n",oldnp,maxp);
	   }
	   Append(newpoint);
	   // to do:list separator to do
     }
  }

  a.EndList();
  }
  return (a.stat);
}



#if 0

#define  IMPLEMENT_ArrayUnaryOp(OPMNAME,OP,ARGDECL,ARG) \
							\
PointArray::OPNAME(ARGDECL)				\
{							\
  register int i = np;				\
  register Point* dst = p ;				\
  while(i--) *dst++ OP ARG;				\
};

///////////////////////////////////////////////////////////////////////////////



PointArray& PointArray::operator+=(const Point &Arg)
{
  register int i = np;
  register Point* dst = p ;
  while(i--) *dst++ += Arg;
  return (*this);
};


PointArray& PointArray::operator-=(const Point &Arg)
{
  register int i = np;
  register Point* dst = p ;
  while(i--) *dst++ -= Arg;
  return (*this);
};


PointArray& PointArray::operator*=(const Point &Arg)
{
  register int i = np;
  register Point* dst = p ;
  while(i--) *dst++ *= Arg;
  return (*this);
};


PointArray& PointArray::operator/=(const Point &Arg)
{
  register int i = np;
  register Point* dst = p ;
  while(i--) *dst++ /= Arg;
  return (*this);
};


PointArray& PointArray::operator+=(const PointArray& Arg)
{
  register int i = np;
  register Point* src = Arg.p ;
  register Point* dst = p ;
  if (i>Arg.np) i=Arg.np;
  while (i--) *dst++ += *src++;
  return (*this);
};


PointArray& PointArray::operator*=(const PointArray& Arg)
{
  register int i = np;
  register Point* src = Arg.p ;
  register Point* dst = p ;
  if (i>Arg.np) i=Arg.np;
  while (i--) *dst++ *= *src++;
  return (*this);
};

#endif


// Perform  Result[*] = this * ta * arg * tb:
PointArray& PointArray::Interpolate(float t1,const PointArray& Arg,float t2,
			PointArray& Result)
{
  register int i = np;
  register Point* src1 = p ;

  register Point* src2 = Arg.p ;

  if (i>Arg.np) i=Arg.np;	// size mismatch

  Result.Resize(i);

  register Point* dest = Result.p ;

  while (i--)
   {
	src1->Interpolate(t1,*src2,t2,*dest);
	src1++;
	src2++;
	dest ++;
   }


  return (Result);

};

#if 0
PointArray& PointArray::operator*= (const Matrix& Arg)
{
  register int i = np;
  register Point* dst = p ;
  while(i--) {  *dst *= Arg; dst ++; }
  return (*this);
}
#endif


// Compute Boundingbox for all points
BBox& PointArray::ComputeBBox (BBox &box) const
{
  register int i = np;
  register Point* pp = p ;
  if (i == 0) {
     box.SetEmpty();
  }
  else {
    box.Set(*pp++);
    i--;
    while(i--) box.Union(*pp++); ;
  }
  return (box);
}

// Find index of point in this with minum distance to p
// note distance2 is sqared
// return
//  >= 0 index
// -1 if nothing can be found
int PointArray::FindNearest(const Point &pt,float &distance2)
{ 
  if (Length() <=0) return(-1);

  const Point *pp = &p[np-1];
  
  int nearestIndex = -1;
  float nearest = FLOAT_MAX;

  do {
	float d = sqr(pp->x-pt.x) + sqr(pp->y-pt.y)+sqr(pp->z -pt.z);
	// to optimize ? change to manhatten distance ?
	if (d<nearest) {	// nearer ?
	    nearest = d;
		nearestIndex = pp - p;
		if (d<EPS) break;
	}
	pp--;
  } while (pp>=p);
  
  distance2=nearest;
  
  return(nearestIndex);
}

void PointArray::Normalize()
{
  register int i = np;
  register Point* pp = p ;
  if (i == 0) {
     return;
  }
  else {
    while(i--) { pp->Normalize(); pp++; }
  }
  return;
}


// PointCurve

// compute length of curve
double PointCurve::Len()
{
  register int i = np;
  register Point* cur = p ;
  register Point* prev= p ;
  double len = 0.0;
  if (i<=1) return(len);
  prev=cur;
  cur++; i--;

  while(i--)
  {
     Point p= *cur++ - *prev++;
     len += p.Len();
  }
  return (len);
};

// Eval point on curve for parameter t [0..1]
Point PointCurve::Eval(float t)
{
   if (np<2) return(Point(0.0));

   if (t>=1.0) return(p[np-1]);

   int i = (int) (t * (float)(np-1));

   if (i<0) return(p[0]);
   if (i>np) return(p[np-1]);

   float delta = 1.0 /  (float) (np-1);
   float tseg =  (t - (delta*i)) / delta;
   Point R;
   p[i].Interpolate(1.0-tseg, p[i+1],tseg,R);
   return(R);
}



///////////////////////////////////////////////////////////////////////////////

/*
PointArrayXYZ::PointArrayXYZ(int n) : x(n),y(n),z(n)
{ 
};

PointArrayXYZ::PointArrayXYZ(int n, const Point &iT) 
: x(n,iT.x),y(n,iT.y),z(n,iT.z)
{
 
};


PointArrayXYZ::PointArrayXYZ(const PointArrayXYZ& a)
: x(a.x),y(a.y),z(a.z)
{
 
};


PointArrayXYZ::PointArrayXYZ(int n, const Point *iT)
{
  Set(n,iT);
};
*/


PointArrayXYZ::~PointArrayXYZ()
{
	free(p); p=0;
}

void PointArrayXYZ::Flush()
{ 
	free(p); maxp= np =0; p = 0;

};

PointArrayXYZ&
PointArrayXYZ::Set(const PointArrayXYZ& a)
{
	if(this != &a) {

	  Resize(a.np);

	  register int i=a.np;

      float* dstx = x;
      float* dsty = y;
      float* dstz = z;

	  const float* srcx = a.x;
	  const float* srcy = a.y;
      const float* srcz = a.z;


      if (a.p) {
	  // could do 16 block copy 	
	  while (i--) {
		  *dstx++ = *srcx++;
	  }
	  i=a.np;
	  while (i--) {
		  *dsty++ = *srcy++;
	  }
	  i=a.np;
	  while (i--) {
		  *dstz++ = *srcz++;
	  }

	  }

	}
	return *this;
};


PointArrayXYZ&
PointArrayXYZ::Set(const BasicPointArray& a)
{
	Set(a.Length(),a.Data());
 
	return *this;
};

PointArrayXYZ&
PointArrayXYZ::Set(int n, const Point *iT)
{
	  
	Resize(n);
    register int i=n;

    float* dstx = x;
    float* dsty = y;
    float* dstz = z;

    register const Point* src = iT;
    if (src) 
	while (i--) {
	
		*dstx++ = src->x;
		*dsty++ = src->y;
		*dstz++ = src->z;
		src++;
	
    }
	return *this;
};


BasicPointArray&
PointArrayXYZ::Get(BasicPointArray &a) const
{
	  
	a.Resize(Length());

    register int i=Length();

    const float* srcx = x;
    const float* srcy = y;
    const float* srcz = z;

    register  Point* dst = a.DataRW();
    if (srcx) 
	while (i--) {
		dst->x = *srcx++;
		dst->y = *srcy++;
		dst->z = *srcz++;
		dst++;
	
    }
	return a;
};


/*

PointArrayXYZ&
PointArrayXYZ::Set(int n, const Point &a)
{
	x.Set(n,a.x);
	y.Set(n,a.y);
	z.Set(n,a.z);
	return *this;

};

*/

PointArrayXYZ&
PointArrayXYZ::operator=(const PointArrayXYZ& a)
{
  	Set(a);
    return *this;
};


PointArrayXYZ&
PointArrayXYZ::operator=(const BasicPointArray& a)
{
  	Set(a);
    return *this;
};

// from crt/heap.h 

/*
 * Unit of allocation. All allocations are of size n * _GRANULARITY. Note
 * that _GRANULARITY must be a power of 2, or it cannot be used with the
 * _ROUND2 macro.
 */
#if defined (_M_MRX000) || defined (_M_ALPHA) || defined (_M_PPC) || defined (_M_MPPC)
#define _GRANULARITY    8
#else  /* defined (_M_MRX000) || defined (_M_ALPHA) || defined (_M_PPC) || defined (_M_MPPC) */
#define _GRANULARITY    4
#endif  /* defined (_M_MRX000) || defined (_M_ALPHA) || defined (_M_PPC) || defined (_M_MPPC) */

/*
 * Macros to round numbers
 *
 * _ROUND2 = rounds a number up to a power of 2
 * _ROUND = rounds a number up to any other numer
 *
 * n = number to be rounded
 * pow2 = must be a power of two value
 * r = any number
 */

#define _ROUND2(n,pow2) \
        ( ( n + pow2 - 1) & ~(pow2 - 1) )

#define _ROUND(n,r) \
        ( ( (n/r) + ((n%r)?1:0) ) * r)


#define _BLOCKSIZE 4
#define _BLOCKSIZE_BYTES (_BLOCKSIZE *sizeof(float))
#define _EXTRA_ALLOC_BYTES (_BLOCKSIZE_BYTES - _GRANULARITY)
#define _EXTRA_ALLOC (_EXTRA_ALLOC_BYTES /  _BLOCKSIZE)

// ALIGN float * to 16 byte boundary 
#define PALIGN(P) \
	(float *) _ROUND2((DWORD) (p),_BLOCKSIZE_BYTES )



// resize Array to hold elements [0..N-1]
// if exact, buffer size is really N, otherwise buffer may larger
//
void
PointArrayXYZ::Resize(int N,int exact)
{

	if (!exact) {
		if (N<=maxp) {  // new size fits in buffer
			np=N;   // set length
			return;
		}
	} else {
		if (_ROUND2(N,_BLOCKSIZE)==maxp) {
			np=N;// set length
			return;
		}
	}

	// allocate at least a multiple of 4 floats 
	int newLength;
    if ( N > 0 )
       newLength = _ROUND2(N,_BLOCKSIZE);
    else
       newLength = _BLOCKSIZE;

	if (newLength == 1024) newLength += _BLOCKSIZE;

    
	p = realloc(p,_EXTRA_ALLOC + newLength*3);

	maxp = newLength;
	np = N;
	
	// get component pointers
	x = PALIGN(p);
	y=x+maxp;
	z=y+maxp;
};


#if 0
void
PointArrayXYZ::Swap(int i,int j)
{ 	x.Swap(i,j);
	y.Swap(i,j);
	z.Swap(i,j);
};


PointArrayXYZ&
PointArrayXYZ::Append(const BasicPointArray& a)
{
	register int i= a.Length();
	register const Point* src = a.Data();

	int start = Length();

	Resize(Length()+a.Length());	// expand me

	// point after old end

	float* dstx = &x[start];
    float* dsty = &y[start];
    float* dstz = &z[start];

    
    if (src) 
	while (i--) {
		*dstx++ = src->x;
		*dsty++ = src->y;
		*dstz++ = src->z;
		src++;
	
    }
    return(*this);
};

PointArrayXYZ&
PointArrayXYZ::Append(const PointArrayXYZ& a)
{
	Resize(Length()+a.Length());	// expand me

	x.Append(a.x);
	y.Append(a.y);
	z.Append(a.z);

    return(*this);
};

PointArrayXYZ&
PointArrayXYZ::Append(const Point& a,int /* expand */ )
{
	Resize(Length()+1);	// expand me

	x.Append(a.x);
	y.Append(a.y);
	z.Append(a.z);

    return(*this);
};

#endif



/* change to inline 

BasicPointArray&
BasicPointArray::Append(const Point &a,int expand)
{
  register int i= np;
  if (expand) {
	   if (np==maxp) {  // make array bigger
		 Resize((np+8) + (np/2));
		 np=i+1;
	   }

  }
  else Resize(np+1); // expand me
  p[i]	= a;
  return(*this);
};


PointArrayXYZ&
PointArrayXYZ::Insert(int ofs, const PointArrayXYZ& a)
{
	Resize(Length()+a.Length());	// expand me

	x.Insert(ofs,a.x);
	y.Insert(ofs,a.y);
	z.Insert(ofs,a.z);

    return(*this); 
	
};


PointArrayXYZ&
PointArrayXYZ::Delete(int ofs, int cnt)
{

	x.Delete(ofs,cnt);
	y.Delete(ofs,cnt);
	z.Delete(ofs,cnt);

    return(*this); 

}
*/



///////////////////////////////////////////////////////////////////////////////

// PointArray4
/*
PointArray4::PointArray4(int n) : x(n),y(n),z(n)
{ 
};

PointArray4::PointArray4(int n, const Point &iT) 
: x(n,iT.x),y(n,iT.y),z(n,iT.z)
{
 
};


PointArray4::PointArray4(const PointArray4& a)
: x(a.x),y(a.y),z(a.z)
{
 
};


PointArray4::PointArray4(int n, const Point *iT)
{
  Set(n,iT);
};
*/


PointArray4::~PointArray4()
{
	free(p); p=0;
}

void PointArray4::Flush()
{ 
	free(p); maxp= np =0; p = 0;

};

PointArray4&
PointArray4::Set(const PointArray4& a)
{
	if(this != &a) {

	  Resize(a.np);

	  register int i=a.np;

      float* dstx = x;

	  const float* srcx = a.x;


      if (a.p) {
	  // could do 16 block copy 	
		i*=P4_STRIDE;	
		while (i--) {
		  *dstx++ = *srcx++;
		}

	  }

	}
	return *this;
};


PointArray4&
PointArray4::Set(const BasicPointArray& a)
{
	Set(a.Length(),a.Data());
 
	return *this;
};

PointArray4& 
PointArray4::Set(const BasicPointArray& a,float scaleFactor)
{
	Set(a.Length(),a.Data(),scaleFactor);
 
	return *this;
};

PointArray4&
PointArray4::Set(int n, const Point *iT)
{
	  
	Resize(n);
    register int i=n;

    register float* dst = x;

    register const Point* src = iT;
    if (src) {

	
	while (i>3) { // full 4 loops
		int j=0;
		register float x,y,z;
		x=src[0].x;
		y=src[0].y;
		z=src[0].z;


		dst[j+0] = x; dst[j+4] = y; x=src[1].x;
		dst[j+8] = z; y=src[1].y; z=src[1].z;
		
		j++;
		dst[j+0] = x; dst[j+4] = y; x=src[2].x;
		dst[j+8] = z; y=src[2].y; z=src[2].z;
		
		j++;
		dst[j+0] = x; dst[j+4] = y; x=src[3].x;
		dst[j+8] = z; y=src[3].y; z=src[3].z;
		
		j++;
		dst[j+0] = x; dst[j+4] = y; 
		dst[j+8] = z; 
		
		src+=4;
		dst+=P4_STRIDE;
		i-=4;
	}

	while (i>0) {
		/* unroll 
		for (int j=0;j<4;j++) {
			dst[j+0] = src->x; dst[j+4] = src->y; dst[j+8] = src->z; src++;
			i--;
			if (i<=0) break;
		}
		*/
		/*
		int j=0;

		dst[j+0] = src->x; dst[j+4] = src->y; dst[j+8] = src->z; src++;
		i--; if (i<=0) break;
		
		j++;
		dst[j+0] = src->x; dst[j+4] = src->y; dst[j+8] = src->z; src++;
		i--; if (i<=0) break;
		
		j++;
		dst[j+0] = src->x; dst[j+4] = src->y; dst[j+8] = src->z; src++;
		i--; if (i<=0) break;
		
		j++;
		dst[j+0] = src->x; dst[j+4] = src->y; dst[j+8] = src->z; src++;
		i--; //if (i<=0) break;
		*/
		int j=0;

		dst[j+0] = src[0].x; dst[j+4] = src[0].y; dst[j+8] = src[0].z; 
		i--; if (i<=0) break;
		
		j++;
		dst[j+0] = src[1].x; dst[j+4] = src[1].y; dst[j+8] = src[1].z; 
		i--; if (i<=0) break;
		
		j++;
		dst[j+0] = src[2].x; dst[j+4] = src[2].y; dst[j+8] = src[2].z; 
		i--; if (i<=0) break;
		
		j++;
		dst[j+0] = src[3].x; dst[j+4] = src[3].y; dst[j+8] = src[3].z; 
		i--; //if (i<=0) break;
		
		src+=4;
		dst+=P4_STRIDE;
	
	}
	}
	return *this;
};

PointArray4&
PointArray4::Set(int n, const Point *iT,float scaleFactor)
{
	  
	Resize(n);
    register int i=n;

    float* dst = x;

    register const Point* src = iT;
    if (src) 
	while (i>0) {

		for (int j=0;j<4;j++) {
			dst[j+0] = src->x * scaleFactor; dst[j+4] = src->y * scaleFactor; dst[j+8] = src->z * scaleFactor; src++;
			i--;
			if (i<=0) break;
		}
		dst+=P4_STRIDE;
	
    }
	return *this;
};


BasicPointArray&
PointArray4::Get(BasicPointArray &a) const
{
	  
	a.Resize(Length());

    register int i=Length();


    const float* dst = x;

    register  Point* src = a.DataRW();
    if (src) 
	while (i>0) {
		for (int j=0;j<4;j++) {
			src->x= dst[j+0], src->y = dst[j+4]; 
		    src->z = dst[j+8]; src++;
			i--;
			if (i<=0) break;
		}
		dst+=P4_STRIDE;
	
    }
	return a;
};


/*

PointArray4&
PointArray4::Set(int n, const Point &a)
{
	x.Set(n,a.x);
	y.Set(n,a.y);
	z.Set(n,a.z);
	return *this;

};

*/

PointArray4&
PointArray4::operator=(const PointArray4& a)
{
  	Set(a);
    return *this;
};


PointArray4&
PointArray4::operator=(const BasicPointArray& a)
{
  	Set(a);
    return *this;
};



// resize Array to hold elements [0..N-1]
// if exact, buffer size is really N, otherwise buffer may larger
//
void
PointArray4::Resize(int N,int exact)
{

	if (!exact) {
		if (N<=maxp) {  // new size fits in buffer
			np=N;   // set length
			return;
		}
	} else {
		if (_ROUND2(N,_BLOCKSIZE)==maxp) {
			np=N;// set length
			return;
		}
	}

	// allocate at least a multiple of 4 floats 
	int newLength;
    if ( N > 0 )
       newLength = _ROUND2(N,_BLOCKSIZE);
    else
       newLength = _BLOCKSIZE;

	if (newLength == 1024) newLength += _BLOCKSIZE;

    
	p = realloc(p,_EXTRA_ALLOC + newLength*3);

	maxp = newLength;
	np = N;
	
	// get component pointers
	x = PALIGN(p);
};


#if 0
void
PointArray4::Swap(int i,int j)
{ 	x.Swap(i,j);
	y.Swap(i,j);
	z.Swap(i,j);
};


PointArray4&
PointArray4::Append(const BasicPointArray& a)
{
	register int i= a.Length();
	register const Point* src = a.Data();

	int start = Length();

	Resize(Length()+a.Length());	// expand me

	// point after old end

	float* dstx = &x[start];
    float* dsty = &y[start];
    float* dstz = &z[start];

    
    if (src) 
	while (i--) {
		*dstx++ = src->x;
		*dsty++ = src->y;
		*dstz++ = src->z;
		src++;
	
    }
    return(*this);
};

PointArray4&
PointArray4::Append(const PointArray4& a)
{
	Resize(Length()+a.Length());	// expand me

	x.Append(a.x);
	y.Append(a.y);
	z.Append(a.z);

    return(*this);
};

PointArray4&
PointArray4::Append(const Point& a,int /* expand */ )
{
	Resize(Length()+1);	// expand me

	x.Append(a.x);
	y.Append(a.y);
	z.Append(a.z);

    return(*this);
};

#endif



/* change to inline 

BasicPointArray&
BasicPointArray::Append(const Point &a,int expand)
{
  register int i= np;
  if (expand) {
	   if (np==maxp) {  // make array bigger
		 Resize((np+8) + (np/2));
		 np=i+1;
	   }

  }
  else Resize(np+1); // expand me
  p[i]	= a;
  return(*this);
};


PointArray4&
PointArray4::Insert(int ofs, const PointArray4& a)
{
	Resize(Length()+a.Length());	// expand me

	x.Insert(ofs,a.x);
	y.Insert(ofs,a.y);
	z.Insert(ofs,a.z);

    return(*this); 
	
};


PointArray4&
PointArray4::Delete(int ofs, int cnt)
{

	x.Delete(ofs,cnt);
	y.Delete(ofs,cnt);
	z.Delete(ofs,cnt);

    return(*this); 

}
*/

FloatArrayAligned::~FloatArrayAligned()
{
	free(p); p=0;
}

void FloatArrayAligned::Flush()
{ 
	free(p); maxp= np =0; p = 0;

};

// FloatArrayAligned(int n,float t);


// resize Array to hold elements [0..N-1]
// if exact, buffer size is really N, otherwise buffer may larger
//
void
FloatArrayAligned::Resize(int N,int exact)
{

	if (!exact) {
		if (N<=maxp) {  // new size fits in buffer
			np=N;   // set length
			return;
		}
	} else {
		if (_ROUND2(N,(_BLOCKSIZE*2))==maxp) {
			np=N;// set length
			return;
		}
	}

	// allocate at least a multiple of 4 floats 
	int newLength;
    if ( N > 0 )
       newLength = _ROUND2(N,(_BLOCKSIZE*2));
    else
       newLength = _BLOCKSIZE;

	if (newLength == 1024) newLength += (_BLOCKSIZE*2);

    
	p = realloc(p,_EXTRA_ALLOC + newLength);

	maxp = newLength;
	np = N;
	
	// get component pointers
	x = PALIGN(p);
};





