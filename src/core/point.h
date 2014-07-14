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
#ifndef _point_h
#define _point_h

/******************************************************************************
@doc
@module point.h - Generic Point + Vector (= Normal) class |	

Copyright (c) 1995 - 1998	by Holger Grahn
All rights reserved

Purpose:
Generic Point + Vector (= Normal) class	

Classes:
<c Point>		three element vector x,y,z
	<c Vector>

<c Plane>		a plane defined by normal and distance from origin

<c BBox>		a box defined by min and may points

<c Ray>

Notes:

Changes:

Todo :

******************************************************************************/



//#include "array.h"
#include <math.h>

#include "gutils.h"


class AStream;
class Matrix;
//class ostream;




// assuming Point = point
// used for static initializer
typedef struct { 
	float x,y,z; 
} point_;


/*/////////////////////////////////////////////////////////////////////////////

Point
@class
a 3d point without virtual functions
*/

class Point {
	public:
	
	//! the 3d coordinates
	float x,y,z;

	// constructors
	inline Point(float v_=0.0) { x=v_; y=v_; z=v_; }
	inline Point(const float v[3]) { x=v[0]; y=v[1]; z=v[2];}
	inline Point(float x_, float y_, float z_=0.0) { x=x_; y=y_; z=z_; }
	inline Point(const Point &p) { x=p.x; y=p.y; z=p.z; }

	// Access
	inline float& operator[] (int i) {
		//if (i == 0) return x; else if (i == 1) return y; else return z;
		return *((&x)+i); // assume continous memory
	}

	inline const float& operator[] (int i) const {
		// if (i == 0) return x; else if (i == 1) return y;else return z;
		return *((&x)+i); // assume continous memory
	}

#ifdef _D3D
    inline operator D3DVECTOR&() { return * ((D3DVECTOR *) &x); } 
    inline operator const D3DVECTOR&() const  { return * ((D3DVECTOR *) &x); } 

    // convert to/ from D3D colorvalue 
	inline void Set(const D3DCOLORVALUE &p) { x=p.r; y=p.g; z=p.b; }
	inline void Get(D3DCOLORVALUE &p,D3DVALUE alpha=1.0) const { p.r=x; p.g=y; p.b =z; p.a = alpha;}

#endif

	/* assigments */
	inline void Set(float x_=0.0, float y_=0.0, float z_=0.0) { x=x_; y=y_; z=z_; }
	inline void Set(const Point &p) { x=p.x; y=p.y; z=p.z; }
	inline void Set(const float v[3]) { x=v[0]; y=v[1]; z=v[2]; }

	inline Point & Zero() { x=y=z=0.0; return(*this); }
	inline Point & One() { x=y=z=1.0; return(*this); }

	const char *ClassName() const { return ("Point"); }


	/* misc */
	inline float Len2() const { return (x*x + y*y + z*z); } 	// squared length
	inline float Len() const { return sqrt(Len2()); }		// vector length
	inline float Length() const { return sqrt(Len2()); }		// vector length
	inline float ManhattanLen() const { return (fabs(x)+fabs(y)+fabs(z)); } // vector length

	/* operators */

	// Unary operators returning a new point
	Point operator- () const;
	Point operator+ () const;


	// Assignment operators
	Point& operator+= (const Point& A);
	Point& operator-= (const Point& A);
	Point& operator*= (const Point& A);
	Point& operator/= (const Point& A);
	Point& operator+= (float A);
	Point& operator-= (float A);
	Point& operator*= (float A);
	Point& operator/= (float A);


	// normalize point
	inline Point& Normalize()	{ float l=Len();
		if (l != 0.0) l= 1.0/l; else l=1.0;
		x *= l; y *=l; z*=l; return(*this);
		}

	// Binary operators returning a new point
	friend Point operator+ (const Point& A, const Point& B);
	friend Point operator- (const Point& A, const Point& B);
	friend Point operator* (const Point& A, const Point& B);
	friend Point operator* (const Point& A, float B);
	friend Point operator* (float A, const Point& B);
	friend Point operator* (const Matrix&, const Point&);
	friend Point operator/ (const Point& A, const Point& B);
	friend Point operator/ (const Point& A, float B);

	friend int operator< (const Point& A, const Point& B);

	friend int operator== (const Point& A, const Point& B);
	friend int operator!= (const Point& A, const Point& B);

	friend int Equal(const Point& A, const Point& B,float Epsilon=EPS);

	Point& operator*= (const Matrix &A) { Set(A* *this); return(*this); }

	inline float Dot(const Point& B) const
	{
		return x * B.x + y * B.y + z * B.z;
	}

	// return Dot Product

	friend float DotProd(const Point& A, const Point& B);
	
	//! set to crossprod of a & b
	inline Point& Cross (const Point& A, const Point& B)
	{
	   Set(A.y * B.z - A.z * B.y,
			A.z * B.x - A.x * B.z,
			A.x * B.y - A.y * B.x);
	return(*this);
	}


	//! return Cross Product
	friend Point CrossProd (const Point& A, const Point& B);

	//! return Cross Product normalized
	friend Point NormCrossProd (const Point& A, const Point& B);

	inline Point& Interpolate (float ta, const Point& B,float tb, Point &Result);

	friend inline Point Interpolate (const Point& A, float ta, const Point& B,float tb);
	friend inline Point& Interpolate (const Point& A, float ta, const Point& B,float tb,Point &Result);

	// update minmax
	void MinMax(Point &mi,Point &ma) const;


    // Determine the dominant axis of point or normal
    inline int Dominant() const  {
        int axis = 0;
        float tMax = fabs(x);
        if (fabs(y) > tMax) { 
	        tMax = fabs(y);	  
	        axis = 1;
        }
        if (fabs(z) > tMax)	
	        axis = 2;		
        return(axis);
    }


	//! Extract projected coordinates of the point to the dominant coordinate plane.
    inline void Project(float& px, float& py, int axis) const {

	switch (axis) {
	  case 0:
	    px = y; py = z;
	    break;
	  case 1:
	    px = x; py = z;
	    break;
	  default:
	    px = x; py = y;
	    break;
	}
    }
	
    //! Extract projected coordinates indices of the point to the dominant coordinate plane.
    inline void ProjectIndex(int& px, int& py, int axis) const {

	switch (axis) {
	  case 0:
	    px = 1; py = 2;
	    break;
	  case 1:
	    px = 0; py = 2;
	    break;
	  default:
	    px = 0; py = 1;
	    break;
	}
    }


	void Rotate(const Point& AxisDir, float angle);
	void Rotate(const Point& AxisPt, const Point& AxisDir,float angle);

	friend class Point RotateOnly(const Matrix& M, const Point& v);

	//! return angle between vectors 
    float Angle(const Point &v1) const {
		// return (double)acos(dot(v1)/v1.length()/v.length());

		float xx = y*v1.z - z*v1.y;
		float yy = z*v1.x - x*v1.z;
		float zz = x*v1.y - y*v1.x;
		float cross = sqrt(xx*xx + yy*yy + zz*zz);

		return fabs(atan2(cross, Dot(v1)));
    }



	/* io */
	//friend ostream& operator<< (ostream &os,const Point &a);

	int io(AStream &a);
};

inline float Magnitude(const Point& v)
{
	return sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
}

inline Point Normalize(const Point& A)
{
	return A / Magnitude(A);
}

inline float DotProd (const Point& A, const Point& B)
{
	return A.x * B.x + A.y * B.y + A.z * B.z;
}

inline Point Point::operator+ () const
{
	return *this;
}

inline Point Point::operator- () const
{
	return Point(-x, -y, -z);
}

inline Point& Point::operator+= (const Point& A)
{
	x += A.x;  y += A.y; z += A.z;
	return *this;
}

inline Point& Point::operator-= (const Point& A)
{
	x -= A.x;  y -= A.y;  z -= A.z;
	return *this;
}

inline Point& Point::operator+= (float A)
{
	x += A;  y += A;  z += A;
	return *this;
}

inline Point& Point::operator-= (float A)
{
	x -= A;  y -= A;  z -= A;
	return *this;
}

inline Point& Point::operator*= (float A)
{
	x *= A;  y *= A;  z *= A;
	return *this;
}

inline Point& Point::operator/= (float A)
{
	x /= A;  y /= A;  z /= A;
	return *this;
}

inline Point& Point::operator*= (const Point& A)
{
	x *= A.x;  y *= A.y;  z *= A.z;
	return *this;
}

inline Point& Point::operator/= (const Point& A)
{
	x /= A.x;  y /= A.y;  z /= A.z;
	return *this;
}

inline Point operator+ (const Point& A, const Point& B)
{
	return Point(A.x + B.x, A.y + B.y, A.z + B.z);
}

inline Point operator- (const Point& A, const Point& B)
{
	return Point(A.x - B.x, A.y - B.y, A.z - B.z);
}

inline Point operator* (const Point& A, const Point& B)
{
	return Point(A.x * B.x, A.y * B.y, A.z * B.z);
}

inline Point operator* (const Point& A, float B)
{
	return Point(A.x * B, A.y * B, A.z * B);
}

inline Point operator* (float A, const Point& B)
{
	return B * A;
}

inline Point operator/ (const Point& A, const Point& B)
{
	return Point(A.x / B.x, A.y / B.y, A.z / B.z);
}

inline Point operator/ (const Point& A, float B)
{
	return Point(A.x / B, A.y / B, A.z / B);
}

inline int operator< (const Point& A, const Point& B)
{
	return A.x < B.x && A.y < B.y && A.z < B.z;
}

inline int operator== (const Point& A, const Point& B)
{
	return (A.x == B.x) && (A.y == B.y) && (A.z == B.z);
}

inline int operator!= (const Point& A, const Point& B)
{
	return !(A == B);
}

inline int Equal(const Point& A, const Point& B,float Epsilon)
{
	return	fabs(A.x - B.x) <= Epsilon &&
		fabs(A.y -  B.y) <= Epsilon &&
		fabs(A.z - B.z) <= Epsilon;
}

inline Point CrossProd (const Point& A, const Point& B)
{
	return Point(A.y * B.z - A.z * B.y,
			A.z * B.x - A.x * B.z,
			A.x * B.y - A.y * B.x);
}

inline Point NormCrossProd (const Point& A, const Point& B)
{
	return (Point(A.y * B.z - A.z * B.y,
			A.z * B.x - A.x * B.z,
			A.x * B.y - A.y * B.x).Normalize());
}


inline Point& Point::Interpolate (float ta, const Point& B,float tb, Point &Result)
{
  Result = *this;
  Result *= ta;
  Result += B*tb;
  return(Result);
}

// a + (b-a) * t  == a * (1-t) + b * t
inline Point& Interpolate (const Point& A, float ta, const Point& B,float tb, Point &Result)
{ Result = A;
  Result *= ta;
  Result += B*tb;
  return(Result);
}

inline Point Interpolate (const Point& A, float ta, const Point& B,float tb)
{ Point result(A);
  result *= ta;
  result += B*tb;
  return(result);
}


// update minmax
inline void Point::MinMax(Point &mi,Point &ma) const
{
  if (x < mi.x) mi.x = x; else if (x>ma.x) ma.x=x;
  if (y < mi.y) mi.y = y; else if (y>ma.y) ma.y=y;
  if (z < mi.z) mi.z = z; else if (z>ma.z) ma.z=z;
}


//! same as Point, but stores a direction vector 
class Vector	: public Point {
public:
	// constructors

	Vector(float v_=0.0) { x=v_; y=v_; z=v_; }
	Vector(float x_, float y_, float z_=0.0) { x=x_; y=y_; z=z_; }
	Vector(const Point &p) { x=p.x; y=p.y; z=p.z; }
	Vector(const Vector &p) { x=p.x; y=p.y; z=p.z; }

	Vector Reflect(const Vector & Normal) {
		return (Normal * ( 2.0 * DotProd(Normal,- *this )) + *this);
		}


//	friend Vector operator* (const Matrix&, const Vector&);

};



//! a 3D plane stored as normal + offset from origin
class Plane 	{
public:
	Vector n;		//!< Normal to plane
	float d;		//!< distance to origin

	inline Plane() {}

	//! set by normal and parameter D
	inline Plane(const Vector &normal,float D) : n(normal),d(D) {}
	
	//! set by plane equatation
	inline Plane(float A,float B,float C,float D): n(A,B,C),d(D) {}

	//! set by other plane
	inline Plane(const Plane &p): n(p.n),d(p.d) {}

	//! get the normal part
	inline Vector& Normal() { return(n); }

	//! set by point on plane  &  normal vector 
	inline void Set(const Point &p, const Vector &N) {
		n=N;
		n.Normalize();
		d = -DotProd(p,n);
	}

	//! set by Plane
	inline void Set(const Plane &p) { n = p.n, d=p.d; }

	
	//! set by normal vector and distance from origin
	inline void Set(const Vector &normal,float D) { n.Set(normal), d = D; }

	//! set by 3 points on Plane (triangle)
	inline void Set(const Point &p0,const Point &p1,const Point &p2) { Set(Plane3(p0,p1,p2)); }

	//! set by 4 parames = (normal vector and distance from origin) 
	inline void Set(float A,float B,float C,float D) { n.Set(A,B,C),d=D; }

	//! set to Z Plane
	inline void SetZ() { Set(0,0,1,0); }

	//! intersect plane with a ray,
	//! 1 : intersection, t contains parameter along ray
	//! 0 : no intersection 
	int Intersect(const Point &rayP,const Vector &rayD,float &t)
	{
		float dot = DotProd(n,rayD);
		//if (fabs(dot) < EPSILON) 
		if (iszero(dot)) 
			return(0);
	    // compute the t-parameter
		t = (d - DotProd(n,rayP)) / dot;
		return(1);
	}



	friend Plane operator* (const Matrix&, const Plane&);
	// compute plane from 3 pts 
	friend Plane Plane3(const Point &p0,const Point &p1,const Point &p2);

	int io(AStream &a);

};

#define BBOX_MAX FLT_MAX 
#define BBOX_MIN FLT_MIN

/*
! Bounding box
holds a axis aligned extent as two points
*/

class BBox {
public:
	//! box points
	Point mi,ma;

	//! constructor, set to empty
	BBox() : mi(BBOX_MAX),ma(BBOX_MIN) {};

	//! constructor, set to single point
	BBox(const Point &p) : mi(p),ma(p) {} ;

	//! constructor, set by two to single point
	BBox(const Point &pMin,const Point &pMax) : mi(pMin),ma(pMax) {} ;


	//! constructor, set to BBox
	BBox(const BBox &b) { mi=b.mi,ma=b.ma; }

   	//! Set Bounding Box to single point
	inline void Set(const Point &p) { mi.Set(p); ma.Set(p); }

    //! Set Bounding Box to new size, ma is recomputed
	inline void SetSize(const Point &p) { ma = mi + p; }

    //! Set Bounding Box to other bbox
	inline void Set(const BBox &b) { mi=b.mi,ma=b.ma; }

    //! Set Bounding Box to  bbox in center, size notation, size must be positive 
	inline void SetCenterSize(const Point &center,const Point &size) { 
		Point s2 = size * 0.5;
		mi = center - s2;
		ma = center + s2;
	}

	//! Set Bounding Box to Emtpy State
	inline void SetEmpty() { mi.Set(BBOX_MAX); ma.Set(BBOX_MIN); }

	//! Is Bounding box empty ?
	inline gbool Empty() const { return(ma.x < mi.x); }
	inline gbool IsEmpty() const { return(ma.x < mi.x); }

	//! return size of box
	inline Point Size() const { return(ma-mi); }

	//! scale box about center 
	void ScaleCenter(const Point &scale) 
			{ Point s(Size());
			  Point center(mi+s * 0.5);
			  s *= scale*0.5;
			  mi.Set(center-s);
			  ma.Set(center+s);
			 }
	 

	//! return center of box
	inline Point Center() const { return(mi + (ma-mi)*0.5); }

    
    //! Union this with Point p 

	inline BBox& Union(const Point &p) { p.MinMax(mi,ma); return(*this); }

    //! Union this with BBox b
	inline 
	BBox& Union(const BBox &b) { b.mi.MinMax(mi,ma);
								b.ma.MinMax(mi,ma);
								return(*this);
							  }

    //! is Point p inside ?
    inline gbool Inside(const Point &p) const {
				return( (p.x>=mi.x) && (p.x <=ma.x)
						&& (p.y>=mi.y) && (p.y <= ma.y)
						&& (p.z>=mi.z) && (p.z <= ma.z));

	}

    //! is BBox b inside ?
    inline gbool Inside(const BBox &b) const { return( Inside(b.mi) && Inside(b.ma)); }

    //! is Point p outside ?
	inline gbool Outside(const Point &p) const {
				return( (p.x < mi.x) || (p.x  > ma.x)
						|| (p.y < mi.y) || (p.y > ma.y)
						|| (p.z < mi.z) || (p.z > ma.z));

	}

    //! is BBox b outside ?
    inline gbool  Outside(const BBox &b) const {
				return( (b.ma.x < mi.x) || (b.mi.x	> ma.x)
						|| (b.ma.y < mi.y) || (b.mi.y > ma.y)
						|| (b.ma.z < mi.z) || (b.mi.z > ma.z));

	};

	//! Transform boundingbox by Matrix
	friend BBox operator* (const Matrix&, const BBox& b);

	//! Transform boundingbox by Matrix
	BBox& operator *= (const Matrix &m);

	//!  >0 if BBOX inside the space of the plane , 2 a point is on plane
	int InsidePlane(const Plane &plane) const ;
											 

	int io(AStream &a);


};

///////////////////////////////////////////////////////////////////////////////

//! a Ray starting at p, going into direction d
class Ray {
public:
	//! startpoint
	Point p;
	
	//! direction vector 
	Vector d;
	
	int dominantAxis; //!< the dominat Axis of d 

	inline Ray() { };

	// construct given point  & direction
	inline Ray(const Point &P,const Vector &D) : p(P),d(D) { 
		dominantAxis = d.Dominant(); 
	};

    //! Evaluate point on ray for parameter t
	inline Point Eval(float t)  const { return(p + d*t); };

	//! set ray given 2 points 
    void SetByPoints(const Point &start,const Point &end) {
            p = start;
            d = end - start;
			dominantAxis = d.Dominant(); 
    }

	//! set by point and direction vector 
    void Set(const Point &start,const Vector &dir) {
            p = start;
            d = dir;
			dominantAxis = d.Dominant(); 
    }

	
    //! Find parameter t for point pt on ray 
/*
	float Map(const Point &pt) const  {
		for (int i = 0; i < 3; i++)
		   if (fabs(d[i]) > EPS) return (pt[i] - p[i]) / d[i];
		return(0.0);
		   };
*/

	//! use dominantAxis for higher precision
	inline float Map(const Point &pt) const  {
		 return (pt[dominantAxis] - p[dominantAxis]) / d[dominantAxis];
		};
	
    //! Reflect Ray with respect to surface normal Normal
	Vector Reflect(const Vector & Normal) const {
		return (Normal * ( 2.0 * DotProd(Normal,-d)) +d);
		}

    //! Normalize direction vector 
	inline void Normalize() { 
		d.Normalize(); 		
		dominantAxis = d.Dominant(); 
	}

	Ray& operator *= (const Matrix &m) { 
			p*=m;
			d=RotateOnly(m,d); 
			dominantAxis = d.Dominant(); 
			return(*this);
	};
};


//! a Ray starting at p, going into direction d
// in addition to ray an range is stored, and direction is normalized
class RayRange : public Ray {

public:

    float tmin,tmax;        //!< the valid ray range 


    // constructors 

    inline RayRange() : Ray(),tmin(0),tmax(1) {};

    inline RayRange(const Point &P,const Vector &D, float _tmin =0.0, float _tmax=1.0) : Ray(P,D),tmin(_tmin),tmax(_tmax) {};

	//! set ray given 2 points 
    inline void SetByPoints(const Point &start,const Point &end) {
            p = start;
            d = end - start;
            tmin = 0.0;
            tmax =  d.Length();
            d.Normalize();
			dominantAxis = d.Dominant(); 
    }

	//! set by point and direction vector 
    inline void Set(const Point &start,const Vector &dir) {
            p = start;
            d = dir;
            tmin = 0.0;
            tmax =  d.Length();
            d.Normalize();
			dominantAxis = d.Dominant(); 
    }

    //! is t in valid range ?
    inline gbool InRange(float t) { return(t>=tmin && t <=tmax); }

    //! Normalize direction, adapt range
	void Normalize() { 
        float magnitude = d.Length();
        
        if (magnitude > 0.0) {
            float f = (1.0/ magnitude); // scale direction to unit
            d *= f;
            tmin *= magnitude;  // adapt range
            tmax *= magnitude;
        }
		dominantAxis = d.Dominant(); 
    }

    // is this ok ???? 
   	RayRange& operator *= (const Matrix &m) { 
            p*=m;
			d=RotateOnly(m,d); 
            Normalize();
            return(*this);
    };

};


#endif


