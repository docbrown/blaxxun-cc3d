

#ifdef _XXXX_KATMAI 

#include <xmmintrin.h> 

#endif


class Point4 {
public:
	
	//! the 3d coordinates
	union  {
   		 __m128 vec;
		 struct { float x,y,z,w; };
	};

	// constructors
	Point(float v_=0.0) { x=v_; y=v_; z=v_; w=1.0f; }

	Point(const float v[3]) { x=v[0]; y=v[1]; z=v[2]; w=1.0f;}

	Point(float x_, float y_, float z_=0.0, float w_=1.0) { x=x_; y=y_; z=z_; w=w_; }
	Point(const Point &p) { x=p.x; y=p.y; z=p.z; w=p.w; }

	// Access
	float& operator[] (int i) {
		//if (i == 0) return x; else if (i == 1) return y; else return z;
		return *((&x)+i); // assume continous memory
	}

	const float& operator[] (int i) const {
		// if (i == 0) return x; else if (i == 1) return y;else return z;
		return *((&x)+i); // assume continous memory
	}

#ifdef _D3D
    operator D3DVECTOR&() { return * ((D3DVECTOR *) &x); } 
    operator const D3DVECTOR&() const  { return * ((D3DVECTOR *) &x); } 

    // convert to/ from D3D colorvalue 
	void Set(const D3DCOLORVALUE &p) { x=p.r; y=p.g; z=p.b; }
	void Get(D3DCOLORVALUE &p,D3DVALUE alpha=1.0) const { p.r=x; p.g=y; p.b =z; p.a = alpha;}

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


	//! normalize point (seen as vector)
	inline Point& Normalize()	{ 
		float l=Len();
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

	//! return Dot Product
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

	//! interpolate between this & b
	inline Point& Interpolate (float ta, const Point& B,float tb, Point &Result);

	//! interpolate between a & b
	friend inline Point Interpolate (const Point& A, float ta, const Point& B,float tb);

	//! interpolate between a & b
	friend inline Point& Interpolate (const Point& A, float ta, const Point& B,float tb,Point &Result);

	//! update minmax
	void MinMax(Point &mi,Point &ma) const;


    //! Determine the dominant axis of point or normal
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


	//!  Extract projected coordinates of the point to the dominant coordinate plane.
    void Project(float& px, float& py, int axis) const {

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
	
    //!  Extract projected coordinates indices of the point to the dominant coordinate plane.
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


//@class same as Point, but stores a direction vector 
class Vector	: public Point {
public:
	// constructors

	Vector(float v_=0.0) { x=v_; y=v_; z=v_; w=0.0f; }
	Vector(float x_, float y_, float z_=0.0, float w_=0.0) { x=x_; y=y_; z=z_; w=w_;}
	Vector(const Point &p) { x=p.x; y=p.y; z=p.z; w=p.w; }
	Vector(const Vector &p) { x=p.x; y=p.y; z=p.z; w=p.w; }

	Vector Reflect(const Vector & Normal) {
		return (Normal * ( 2.0 * DotProd(Normal,- *this )) + *this);
		}


//	friend Vector operator* (const Matrix&, const Vector&);

};





