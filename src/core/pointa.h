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
// Class Definition : PointArray + Numeric PointArray
#ifndef _POINTA_H
#define _POINTA_H



class BasicPointArray : public NumericArray<Point> {

public:

	BasicPointArray() : NumericArray<Point>()	{};
	BasicPointArray(int n) : NumericArray<Point>(n) {};
	BasicPointArray(int n, const Point iT): NumericArray<Point>(n,iT) {};
	BasicPointArray(int n, const Point *iT): NumericArray<Point>(n,iT) {};
	BasicPointArray(const BasicPointArray & arg): NumericArray<Point>(arg) {};

#ifdef _D3D
	BasicPointArray(int n, const D3DVECTOR *iT): NumericArray<Point>(n,(const Point *) iT) {};
#endif


	//!  set point element at index 
	inline BasicPointArray& Set1(int index, const Point &a) 
	{
		UnRef();
		p[index] = a;
		return *this;
	};

	int	io(AStream &a);

};




#if 0
class BasicPointArray {
protected :
	Point *	 p;
	int np;
	int maxp;
public:
	// constructors
	BasicPointArray() : np(0) { p = 0; maxp=0; };
	BasicPointArray(int n);
	BasicPointArray(int n, const Point &iT);
	BasicPointArray(int n, const Point* iT);
	BasicPointArray(const BasicPointArray&);

	// destructor
	~BasicPointArray();
	void Flush();

	// assignment
	BasicPointArray&	operator=(const BasicPointArray&);
	BasicPointArray& Set(const BasicPointArray& a);
	BasicPointArray& Set(int n, const Point *iT);
	BasicPointArray& Set(int n, const Point &iT);

	// access
	inline const Point*	Data() const {return p;}
	inline Point*		Data() {return p;}

	inline int	Length() const { return np; }
	inline int	MaxLength() const { return maxp; }

	Point*		P() {return p;}
	int	Np() const { return np; }

	// operator int()	{ return np; }	// for boolean comparison gives amgiuous expression
	operator Point*()	{ return p; }
	operator const Point* () const { return p; }


	Point&		operator()(int i)	{ return p[i]; };
	Point		operator()(int i) const { return p[i]; };
	inline Point&		operator[](int i)
	{ /* if (i<0 || i>=np) callerror(Array,T,1,"Array index out of range."); */
	return p[i]; };

	inline Point		operator[](int i) const
	{  /* if (i<0 || i>=np) callerror(Array,T,1,"Array index out of range."); */
	return p[i]; };

	// misc
	void		Resize(int,int exact=0);
	void SetLength(int NewLength) { Resize(NewLength); }

	void SetMaxLength(int NewLength) { int l=Length();
				Resize(NewLength);
				if (l<maxp) np=l;
				};


	void		Swap(int i,int j);

	BasicPointArray&	Append(const BasicPointArray&);
	inline BasicPointArray&	Append(const Point&,int expand=0);
	BasicPointArray&	Insert(int ofs, const BasicPointArray& a);
	BasicPointArray&	Delete(int ofs, int cnt);


	// io
	//void	print(ostream&	/* os=cout HIGHC ?*/ );
	int		io(AStream &a);

};

// append a single element, if expand automaticall expand by 50% if np=maxp
inline BasicPointArray&
BasicPointArray::Append(const Point &a,int expand)
{
  int i= np;
  if (expand) {
	   if (np==maxp) {  // make array bigger
		 Resize((np+16) + (np/2));
	   }
	   np=i+1;		 // assert(i<maxp)
  }
  else Resize(np+1); // expand me
  p[i]	= a;
  return(*this);
};

#endif


class PointArray : public BasicPointArray
{
public:
	PointArray() : BasicPointArray()	{};
	PointArray(int n) : BasicPointArray(n) {};
	PointArray(int n, const Point &iT): BasicPointArray(n,iT) {};
	PointArray(int n, const Point *iT): BasicPointArray(n,iT) {};
	PointArray(const PointArray & arg): BasicPointArray(arg) {};
#ifdef _D3D
	PointArray(int n, const D3DVECTOR *iT): BasicPointArray(n,iT) {};
#endif


#if 0
	PointArray&    operator+=(const Point &Arg);
	PointArray&    operator-=(const Point &Arg);
	PointArray&    operator*=(const Point &Arg);
	PointArray&    operator/=(const Point &Arg);
/* */
	PointArray&    operator+=(const PointArray& Arg);
	PointArray&    operator-=(const PointArray& Arg);
	PointArray&    operator*=(const PointArray& Arg);
	PointArray&    operator/=(const PointArray& Arg);
#endif


	// Interpolate between two array, return reference to result
	// Perform  Result = this * t1 * Arg * t2:

	PointArray& Interpolate(float t1,
			const PointArray& Arg,float t2,
			PointArray& Result);

	// Transform by Matrix
	//PointArray& operator*= (const Matrix& matrxix);
	
   PointArray& Mult(const Matrix& matrxix);

	// Compute Boundingbox for all points
	BBox& ComputeBBox (BBox &box) const;
  
	// Return index of nearest point to p
    int FindNearest(const Point &p,float &distance2);

	// treat all elemens as vector && normalize vector
	void Normalize();


};


// a point curve is a set of connected points
//
class PointCurve : public PointArray
{
public:
	PointCurve() : PointArray()	{};
	PointCurve(int n) : PointArray(n) {};
	PointCurve(int n, const Point &iT): PointArray(n,iT) {};
	PointCurve(int n, const Point *iT): PointArray(n,iT) {};
	PointCurve(const PointCurve & arg): PointArray(arg) {};

	// compute length of curve
	double Len();

	// Eval point on curve for parameter t [0..1]
	Point Eval(float t);

};


#if 0
class BasicPointArray {
protected :
	Point *	 p;
	int np;
	int maxp;
public:
	// constructors
	BasicPointArray() : np(0) { p = 0; maxp=0; };
	BasicPointArray(int n);
	BasicPointArray(int n, const Point &iT);
	BasicPointArray(int n, const Point* iT);
	BasicPointArray(const BasicPointArray&);

	// destructor
	~BasicPointArray();
	void Flush();

	// assignment
	BasicPointArray&	operator=(const BasicPointArray&);
	BasicPointArray& Set(const BasicPointArray& a);
	BasicPointArray& Set(int n, const Point *iT);
	BasicPointArray& Set(int n, const Point &iT);

	// access
	const Point*	Data() const {return p;}
	Point*		Data() {return p;}

	int	Length() const { return np; }
	int	MaxLength() const { return maxp; }

	Point*		P() {return p;}
	int	Np() const { return np; }

	// operator int()	{ return np; }	// for boolean comparison gives amgiuous expression
	operator Point*()	{ return p; }
	operator const Point* () const { return p; }


	Point&		operator()(int i)	{ return p[i]; };
	Point		operator()(int i) const { return p[i]; };
	Point&		operator[](int i)
	{ /* if (i<0 || i>=np) callerror(Array,T,1,"Array index out of range."); */
	return p[i]; };
	Point		operator[](int i) const
	{  /* if (i<0 || i>=np) callerror(Array,T,1,"Array index out of range."); */
	return p[i]; };

	// misc
	void		Resize(int,int exact=0);
	void SetLength(int NewLength) { Resize(NewLength); }

	void SetMaxLength(int NewLength) { int l=Length();
				Resize(NewLength);
				if (l<maxp) np=l;
				};


	void		Swap(int i,int j);

	BasicPointArray&	Append(const BasicPointArray&);
	inline BasicPointArray&	Append(const Point&,int expand=0);
	BasicPointArray&	Insert(int ofs, const BasicPointArray& a);
	BasicPointArray&	Delete(int ofs, int cnt);


	// io
	//void	print(ostream&	/* os=cout HIGHC ?*/ );
	int		io(AStream &a);

};

// append a single element, if expand automaticall expand by 50% if np=maxp
inline BasicPointArray&
BasicPointArray::Append(const Point &a,int expand)
{
  int i= np;
  if (expand) {
	   if (np==maxp) {  // make array bigger
		 Resize((np+16) + (np/2));
	   }
	   np=i+1;		 // assert(i<maxp)
  }
  else Resize(np+1); // expand me
  p[i]	= a;
  return(*this);
};

#endif


//
// Katmai Style structure of array

class PointArrayXYZ 
{

public:
	float *p; // malloced flat float array

	int np;		// actual length (in points)
	int maxp;	// maximum allocated length np<=maxp


	float *x; // katmai aligned 
	float *y; // katmai aligned 
	float *z; // katmai aligned 
	float *w; // katmai aligned 

private :
	float* alloc(int n)   { return((float*) MALLOC(n*sizeof(float)));  }
	float* realloc(float* pT, int n) { return((float*) REALLOC(pT, n*sizeof(float))); }
	void free(float* pT)  { if (pT) FREE(pT); }


public:

	// constructors
	PointArrayXYZ() : p(NULL),np(0),maxp(0),x(NULL),y(NULL),z(NULL)  { };

	PointArrayXYZ(int n) : p(NULL),np(0),maxp(0) { Resize(n); }

	PointArrayXYZ(int n, const Point &iT);
	PointArrayXYZ(int n, const Point* iT);
	PointArrayXYZ(const PointArrayXYZ&);
	PointArrayXYZ(const BasicPointArray&);

#ifdef _D3D
	PointArrayXYZ(int n, const D3DVECTOR *iT);
#endif



	// destructor
	~PointArrayXYZ();

	void Flush();

	// assignment
	PointArrayXYZ&	operator=(const PointArrayXYZ& a);
	PointArrayXYZ& Set(const PointArrayXYZ& a);

	PointArrayXYZ&	operator=(const BasicPointArray&);
	PointArrayXYZ& Set(const BasicPointArray& a);

	BasicPointArray& Get(BasicPointArray &a) const;


	PointArrayXYZ& Set(int n, const Point *iT);
	
	PointArrayXYZ& Set(int n, const Point &iT);

	// set point at index 
	PointArrayXYZ& Set1(int index, const Point &a) {
		x[index] = a.x,	y[index] = a.y,	z[index] = a.z;
		return *this;
	}


	float *			X() {return x;}
	float *			Y() {return y;}
	float *			Z() {return z;}


	float&		X(int i) { return x[i]; };
	float&		Y(int i) { return y[i]; };
	float&		Z(int i) { return z[i]; };

	float		X(int i) const { return x[i]; };
	float		Y(int i) const { return y[i]; };
	float		Z(int i) const { return z[i]; };


	Point		operator()(int i) const { return Point(x[i],y[i],z[i]); };
	Point		operator[](int i) const
	{  /* if (i<0 || i>=np) callerror(Array,T,1,"Array index out of range."); */
		return Point(x[i],y[i],z[i]);  
	};

    int	Length() const { return np; }
	int	MaxLength() const { return maxp; }


	// allocate / reallocate 
	void Resize(int,int exact=0);

	// Set Length of array 
	void SetLength(int NewLength) { Resize(NewLength); }

	// Set Maximum Length of array, usefull to preallocate space 
	void SetMaxLength(int NewLength) { 
               int l=Length();
				   Resize(NewLength);
				   if (l<maxp) np=l;
				};


	void		Swap(int i,int j);

	PointArrayXYZ&	Append(const BasicPointArray&);
	PointArrayXYZ&	Append(const PointArrayXYZ&);
	PointArrayXYZ&	Append(const Point&,int expand=0);

	PointArrayXYZ&	Insert(int ofs, const PointArrayXYZ& a);
	PointArrayXYZ&	Delete(int ofs, int cnt);


	// io
	int		io(AStream &a);



#if 0
	PointArrayXYZ&    operator+=(const Point &Arg);
	PointArrayXYZ&    operator-=(const Point &Arg);
	PointArrayXYZ&    operator*=(const Point &Arg);
	PointArrayXYZ&    operator/=(const Point &Arg);
	PointArrayXYZ&    operator+=(const PointArrayXYZ& Arg);
	PointArrayXYZ&    operator-=(const PointArrayXYZ& Arg);
	PointArrayXYZ&    operator*=(const PointArrayXYZ& Arg);
	PointArrayXYZ&    operator/=(const PointArrayXYZ& Arg);
#endif


	// Interpolate between two array, return reference to result
	// Perform  Result = this * t1 * Arg * t2:

	PointArrayXYZ& Interpolate(float t1,
			const PointArrayXYZ& Arg,float t2,
			PointArrayXYZ& Result);

	// Transform by Matrix
	//PointArrayXYZ& operator*= (const Matrix& matrxix);
	
   PointArrayXYZ& Mult(const Matrix& matrxix);

	// Compute Boundingbox for all points
	BBox& ComputeBBox (BBox &box) const;
  
	// Return index of nearest point to p
    int FindNearest(const Point &p,float &distance2);

	// treat all elemens as vector && normalize vector
	void Normalize();

};


//
// Katmai Style structure of array xxxx yyy zzzz

class PointArray4 
{

private:

	float *p;	// malloced flat float array

	int np;		// actual length (in points)
	int maxp;	// maximum allocated length np<=maxp

	float *x; // katmai aligned 

private :
	float* alloc(int n)   { return((float*) MALLOC(n*sizeof(float)));  }
	float* realloc(float* pT, int n) { return((float*) REALLOC(pT, n*sizeof(float))); }
	void free(float* pT)  { if (pT) FREE(pT); }


public:

	// constructors
	PointArray4() : p(NULL),np(0),maxp(0),x(NULL)  { };

	PointArray4(int n) : p(NULL),np(0),maxp(0) { Resize(n); }

	PointArray4(int n, const Point &iT);
	PointArray4(int n, const Point* iT);
	PointArray4(const PointArray4&);
	PointArray4(const BasicPointArray&);

#ifdef _D3D
	PointArray4(int n, const D3DVECTOR *iT);
#endif
// offset to next block of xxxx yyyy zzzz
#define P4_STRIDE 3*4 



	// destructor
	~PointArray4();

	void Flush();

	// assignment
	PointArray4& operator=(const PointArray4& a);
	PointArray4& Set(const PointArray4& a);

	PointArray4& operator=(const BasicPointArray&);
	PointArray4& Set(const BasicPointArray& a);

	PointArray4& Set(const BasicPointArray& a,float scaleFactor);

	BasicPointArray& Get(BasicPointArray &a) const;


	PointArray4& Set(int n, const Point *iT);

	PointArray4& Set(int n, const Point *iT,float scaleFactor);

	PointArray4& Set(int n, const Point &iT);

	// set point at index 
	inline PointArray4& Set1(int index, const Point &a) {
  	    index =  (index>>2)*P4_STRIDE + (index & 3);

		x[index] = a.x,	x[index+4] = a.y,	x[index+8] = a.z;

		return *this;
	}

	inline PointArray4& Set1(int index, float vx,float vy, float vz) {
  	    index =  (index>>2)*P4_STRIDE + (index & 3);

		x[index] = vx,	x[index+4] = vy,	x[index+8] = vz;

		return *this;
	}


	inline float *			Data() {return x;}


	inline Point		operator()(int i) const { 
								int j= (i>>2)*P4_STRIDE;
								i =  j + (i & 3);
								return Point(x[i],x[i+4],x[i+8]); 
								};

	inline Point		operator[](int i) const
	{  /* if (i<0 || i>=np) callerror(Array,T,1,"Array index out of range."); */
				i =  (i>>2)*P4_STRIDE + (i & 3);
				return Point(x[i],x[i+4],x[i+8]); 
	};

    int	Length() const { return np; }
	int	MaxLength() const { return maxp; }


	// allocate / reallocate 
	void Resize(int,int exact=0);

	// Set Length of array 
	void SetLength(int NewLength) { Resize(NewLength); }

	// Set Maximum Length of array, usefull to preallocate space 
	void SetMaxLength(int NewLength) { 
               int l=Length();
				   Resize(NewLength);
				   if (l<maxp) np=l;
				};


	void		Swap(int i,int j);

	PointArray4&	Append(const BasicPointArray&);
	PointArray4&	Append(const PointArray4&);
	PointArray4&	Append(const Point&,int expand=0);

	PointArray4&	Insert(int ofs, const PointArray4& a);
	PointArray4&	Delete(int ofs, int cnt);


	// io
	int		io(AStream &a);



#if 0
	PointArray4&    operator+=(const Point &Arg);
	PointArray4&    operator-=(const Point &Arg);
	PointArray4&    operator*=(const Point &Arg);
	PointArray4&    operator/=(const Point &Arg);
	PointArray4&    operator+=(const PointArray4& Arg);
	PointArray4&    operator-=(const PointArray4& Arg);
	PointArray4&    operator*=(const PointArray4& Arg);
	PointArray4&    operator/=(const PointArray4& Arg);
#endif


	// Interpolate between two array, return reference to result
	// Perform  Result = this * t1 * Arg * t2:

	PointArray4& Interpolate(float t1,
			const PointArray4& Arg,float t2,
			PointArray4& Result);

	// Transform by Matrix
	//PointArray4& operator*= (const Matrix& matrxix);
	
   PointArray4& Mult(const Matrix& matrxix);

	// Compute Boundingbox for all points
	BBox& ComputeBBox (BBox &box) const;
  
	// Return index of nearest point to p
    int FindNearest(const Point &p,float &distance2);

	// treat all elemens as vector && normalize vector
	void Normalize();

};


class FloatArrayAligned
{

// public:

private:
	float *p;	// malloced flat float array

	int np;		// actual length (in points)
	int maxp;	// maximum allocated length np<=maxp


	float *x;	// katmai aligned , points into p

private :
	float* alloc(int n)   { return((float*) MALLOC(n*sizeof(float)));  }
	float* realloc(float* pT, int n) { return((float*) REALLOC(pT, n*sizeof(float))); }
	void free(float* pT)  { if (pT) FREE(pT); }


public:

	// constructors
	FloatArrayAligned() : p(NULL),np(0),maxp(0),x(NULL)  { };

	FloatArrayAligned(int n) : p(NULL),np(0),maxp(0) { Resize(n); }

	FloatArrayAligned(int n,float t);

	// destructor
	~FloatArrayAligned();

	void Flush();


	float *		Data() {return x;}



    int	Length() const { return np; }
	int	MaxLength() const { return maxp; }


	// allocate / reallocate 
	void Resize(int,int exact=0);

	// Set Length of array 
	void SetLength(int NewLength) { Resize(NewLength); }

	// Set Maximum Length of array, usefull to preallocate space 
	void SetMaxLength(int NewLength) { 
					int l=Length();
					Resize(NewLength);
				    if (l<maxp) np=l;
				};

	operator float*()	{ return x; }
	operator const float*() const { return x; }

	float&		operator()(int i)	{ return x[i]; };
	float		operator()(int i) const { return x[i]; };
	
	inline float&		operator[](int i)
	{ /* if (i<0 || i>=np) callerror(Array,T,1,"Array index out of range."); */
		return x[i]; 
	};
	
	inline float		operator[](int i) const
	{  /* if (i<0 || i>=np) callerror(Array,T,1,"Array index out of range."); */
		return x[i]; 
	};
	
	inline float&		operator[](unsigned int i)
	{ /* if (i<0 || i>=np) callerror(Array,T,1,"Array index out of range."); */
		return x[i]; 
	};
	
	inline float		operator[](unsigned int i) const
	{  /* if (i<0 || i>=np) callerror(Array,T,1,"Array index out of range."); */
		return x[i]; 
	};

	// resize and set to array

	FloatArrayAligned&
	Set(int n, const float *iT)
	{
		if (p != iT) {
			Resize(n);
			register int i=n;
			register float* dst = Data() ;
			register const float* src = iT;
			if (src) while (i--) *dst++ = *src++;
		} else Resize(n);
		return *this;
	};

	// resize and set to constant
	FloatArrayAligned&
	Set(int n, float val)
	{
		ASSERT(n>=0);
		Resize(n);
		register int i=n;
		register float* dst = Data() ;
		while (i--) *dst++ = val;
		return *this;
	};

};


class PlaneArray : public NumericArray<Plane> {

public:

	PlaneArray() : NumericArray<Plane>()	{};
	PlaneArray(int n) : NumericArray<Plane>(n) {};
	PlaneArray(int n, const Plane iT): NumericArray<Plane>(n,iT) {};
	PlaneArray(int n, const Plane *iT): NumericArray<Plane>(n,iT) {};
	PlaneArray(const NumericArray<Plane> & arg): NumericArray<Plane>(arg) {};



	//!  set element at index 
	PlaneArray& Set1(int index, const Plane &a) 
	{
		UnRef();
		p[index] = a;
		return *this;
	};

	//int	io(AStream &a);

};





#endif

