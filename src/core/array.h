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
#ifndef _ARRAY_H
#define _ARRAY_H
/******************************************************************************
@doc

@module Array.h - Template based Array class |

Copyright (c) 1993 - 1995	by Holger Grahn
All rights reserved

Purpose:

Template based Array class
Designed to easily interchange with C array notation
and "by value" elements
Derived class NumericArray supports common arithmetic operators


Classes:
Array<T>
	NumericArray<T>
		IntArray
		FloatArray


Notes:

Changes:
    change ARRAY_EXPANSION
            if < limit, only double size
    MALLOC, CALLOC, FREE debug routines


Todo :
Automatic Expansion + Expansion factor
Implementation change :
	use malloc, realloc, free instead of new & delete ?
******************************************************************************/

//#include <iostream.h>




class AStream;

// Auto expand on Append(T)
#define ARRAY_EXPANSION 1024



//\@class Template array class
//\@tcarg class | T | A class to store in array (by value)

template<class T>
class Array {
protected :
	T * p;		//!< Pointer to data
	int np;		//!< actual length
	int maxp;	//!< maximum allocated length np<=maxp
    int byRef;	//!< if true, memory not owned by array 

public:
	// constructors
	Array() : np(0),maxp(0),byRef(0) { p = 0;};
	Array(int n) { maxp = np = n; byRef = 0;
                     if (n == 0) p = 0; 
					 else { p = alloc(np); }
                  };
	explicit Array(int n, const T iT);
	
	explicit Array(int n, const T* iT);

	Array(const Array<T>&);


	//! allocation functions 
    T* alloc(int n);  
    T* realloc(T* pT, int n);
    void free(T* pT);


	//! destructor
	~Array();
	
	//! empty array, free up any resources
	void Flush();

	//! assignment with array
	Array<T>&	operator=(const Array<T>&);

	//!assignment 

	Array<T>& Set(const Array<T>& a);
	Array<T>& Set(int n, const T *iT);
	Array<T>& Set(int n, const T val);

	// move data pointers from a to this
	// a empty after the operation
	Array<T>& SetByMove(const Array<T>& a);


	//! set to reference to other memory pointer
	Array<T>& SetByRef(const Array<T>& a);
    Array<T>& SetByRef(int n, const T *iT);
	
	//! make array unique, copy memory if needed
	Array<T>& UnRef();

	//! access RO
	inline const T*	Data() const {return p;}

	//! access RW , danger if ByRef, use DataRW() instead 
	inline const T*	Data() {return p;}

	//! access data RW, UnRef if needed
	inline T*		DataRW() { UnRef(); return p;}
	
	//! get current length
    inline int	Length() const { return np; }

	//! get allocated (maximum) length
	inline int	MaxLength() const { return maxp; }
	
	//! get the byRef flag
    inline int	ByRef() const { return byRef; }

	//! access RW , danger if ByRef, use DataRW() instead 
	inline operator T*()	{ return p; }

	//! access R
	inline operator const T*() const { return p; }

	inline T&		operator()(int i)	{ return p[i]; };
	inline T		operator()(int i) const { return p[i]; };
	
	inline T&		operator[](int i)
	{ /* if (i<0 || i>=np) callerror(Array,T,1,"Array index out of range."); */
		// if (byRef) TRACE("Warning write access on byRef Array \n");
		return p[i]; 
	};
	
	inline T		operator[](int i) const
	{  /* if (i<0 || i>=np) callerror(Array,T,1,"Array index out of range."); */
		return p[i]; 
	};

	//!  unsigned int index can lead to faster operation
	inline T&		operator[](unsigned int i)
	{ /* if (i<0 || i>=np) callerror(Array,T,1,"Array index out of range."); */
		return p[i]; 
	};
	
	inline T		operator[](unsigned int i) const
	{  /* if (i<0 || i>=np) callerror(Array,T,1,"Array index out of range."); */
		return p[i]; 
	};


	//! set length to n, make sure buffer fits, exact = 1 buffer will be reallocated
	void Resize(int N,int exact=0);

	//! set length of array 
	void SetLength(int NewLength) { Resize(NewLength); }

	//! set Maximum Length of array, usefull to preallocate space 
	void SetMaxLength(int NewLength) { 
               int l=Length();
				   Resize(NewLength);
				   if (l<maxp) np=l;
				};

	//! swap elements i & j
	void		Swap(int i,int j);
	
	//! append array 
	Array<T>&	Append(const Array<T>&);
	Array<T>&	Append(int n, const T*);
	Array<T>&	Append(T);
	
	//! insert array  at ofs
	Array<T>&	Insert(int ofs, const Array<T>& a);

	//! delete at offset ofs cnt elements
	Array<T>&	Delete(int ofs, int cnt);

	//! search elment 
	int	Search(const T val,int start=0);

	// io
	int io(AStream &a);
};


template<class T>
T* Array<T>::alloc(int n)   { return((T*) MALLOC(n*sizeof(T)));  }

template<class T>
T* Array<T>::realloc(T* pT, int n) { return((T*) REALLOC(pT, n*sizeof(T))); }

template<class T>
void Array<T>::free(T* pT)  { if (pT) FREE(pT); }

// constructors

template<class T>
Array<T>::Array(int n, const T iT)
{
  register int i = maxp = np = n;
  byRef=0;
  if (i == 0) { p = 0; }
  else {
  	register T* dst = p = alloc(i);
  	while(i--) *dst++ = iT;
  }
};

template<class T>
Array<T>::Array(const Array<T>& a)
{
  register int i = maxp = np = a.np;
  byRef=0;
  if (i == 0) {
     p= 0;
  } else {
  	register T* dst = p = alloc(i);
  	register const T* src = a.p;
  	while (i--) *dst++ = *src++;
  }
};

template<class T>
Array<T>::Array(int n, const T *iT)
{
  register int i= maxp = np = n;
  byRef = 0;
  if (i == 0) {
     p= 0;
  } else {
  	register T* dst = p = alloc(i);
  	register const T* src = iT;
        if (src) while (i--) *dst++ = *src++;
  }
};

template<class T>
Array<T>::~Array() { 
	if (!byRef) free(p); p=0; byRef= 0; 
}

template<class T>
void Array<T>::Flush()	
{ 
	if (!byRef) free(p); 
	maxp= np =0; p = 0; byRef = 0;
}


template<class T>
Array<T>&
Array<T>::Set(const Array<T>& a)
{
  if(this != &a){
	 Resize(a.np);
    register int i=a.np;
    register T* dst = p ;
    register const T* src = a.p;
    if (src) while (i--) *dst++ = *src++;
  }
  return *this;
};

// "copy" array contents from a to self
// by taking over "a" data ptrs
// a will be empty after this operation
template<class T>
Array<T>&
Array<T>::SetByMove(const Array<T>& a)
{
  if(this != &a){
	Flush();
	np=a.np;
	p=a.p;
	maxp=a.maxp;
	a.np=a.maxp=0; a.p = 0;
  }
  return *this;
};

template<class T>
Array<T>&
Array<T>::Set(int n, const T *iT)
{
  if (p != iT) {
    Resize(n);
    register int i=n;
    register T* dst = p ;
    register const T* src = iT;
    if (src) while (i--) *dst++ = *src++;
  } else Resize(n);
  return *this;
};



template<class T>
Array<T>&
Array<T>::Set(int n, const T val)
{
    Resize(n);
    register int i=n;
    register T* dst = p ;
    while (i--) *dst++ = val;
  return *this;
};

// "copy" array contents from a to self
// by taking over "a" data ptrs
// and set byRef flag

template<class T>
Array<T>&
Array<T>::SetByRef(const Array<T>& a)
{
  if(this != &a){
	Flush();
	np=a.np;
	p=(T *)a.p;
	maxp=a.maxp;
	byRef=1;
  }
  return *this;
};

// "copy" array contents from a to self
// by taking over "a" data ptrs
// and set byRef flag

template<class T>
Array<T>&
Array<T>::SetByRef(int n, const T *iT)
{
	Flush();
	np=n;
	p= (T *)iT;
	maxp=n;
	byRef=1;
	return *this;
};

// own memory
// 
template<class T>
Array<T>& 
Array<T>::UnRef()
{
  if (byRef) 
	  Resize(np,1);
  return *this;
}



template<class T>
Array<T>&
Array<T>::operator=(const Array<T>& a)
{
  return Set(a);
};

template<class T>
void
Array<T>::Resize(int N,int exact)
{
  //ASSERT(byRef == 0);
  
  if (!exact) {
     if (N<=maxp) {  // new size fits in buffer
	    np=N;   // set length
	    return;
     }
  } else {
    if (N==maxp && !byRef) {
	   np=N;
	   return;
	}
  }
  
  if (!byRef) {   // we own the memory, call realloc
	p = realloc(p,N);
  } else {

   T* newArray = alloc(N);
   register int i = (N<=np) ? N:np;
   register const T* src = p;
   register T* dst = newArray;
   if (src) {
      while (i--) *dst++ = *src++;
      if (!byRef) free(p);
   }
   p = newArray;
   byRef=0;
  }
  maxp = np = N;
};

template<class T>
void
Array<T>::Swap(int i,int j)
{ T  tmp(p[i]);
  p[i] = p[j];
  p[j] = tmp;
};

template<class T>
Array<T>&
Array<T>::Append(const Array<T>& a)
{

  return(Append(a.np,a.p));
};


template<class T>
Array<T>&
Array<T>::Append(int n,const T* iT)
{
  register int i= n;
  register T* dst;
  register const T* src = iT;
  int start = np;
  // check for overflow ,test
  if ((np+n) > maxp) {
  	if (n < ARRAY_EXPANSION) {
        // new 9.04.96
        if (maxp<ARRAY_EXPANSION) 
          SetMaxLength(np+max(n,maxp*2));
	   	else SetMaxLength(maxp+ARRAY_EXPANSION);
  	}
  	else SetMaxLength(np+n);	// expand me
  }
  np+=n;

  dst = & p[start];	// point after old end
  while (i--) *dst++ = *src++;
  return(*this);
};

template<class T>
Array<T>&
Array<T>::Append(T a)
{
  register int i= np;
  if (np == maxp) {
      // new 9.04.96
      if (maxp<ARRAY_EXPANSION) 
          Resize(np+max(1,maxp*2));
      else
        Resize(np+ARRAY_EXPANSION /* 1 */); // expand me
  }  
  UnRef();
  p[i]	= a;
  np = i+1;
  return(*this);
};

template<class T>
Array<T>&
Array<T>::Insert(int ofs, const Array<T>& a)
{
  register int i;
  register T* dst;
  register T* src;
  int start = np;

  if (!a) return(*this);
  if (ofs <0) ofs = np-ofs;
  if (ofs<0) ofs = 0;

  Resize(np+a.np);	// expand me
  UnRef();

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

template<class T>
Array<T>&
Array<T>::Delete(int ofs, int cnt)
{
  register int i;
  register T* dst;
  register T* src;

  if (!p) return(*this); /* empty */

  UnRef();

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

template<class T>
int
Array<T>::Search(const T val,int start)
{
    register int i=np;
    if (start >= np) return(-1);

    register T* src = p+start ;
    do {
	if ( *src == val) return( src-p);
	src ++;
    } while ( i-- > 0);
	return(-1);
};


/*
template<class T>
void  Array<T>::print(ostream &os)
{
  register int i = np;
  register T* dst = p ;
  os << "(";
  while(i--)  {
     os << *dst++;
     if (i>0) os << ", ";
  }
  os << ")";
};
*/
// io
template<class T>
int 
Array<T>::io(AStream &a)
{ int i;

  T  newelement;

  if (a.BeginList()) {
   if (a.isOutput()) {
     for(i=0;i<np;i++) {
	    if (i>0) a.ListSep();
	    if ((i % 16) == 15) a.nl();
	    p[i].io(a);
	    //a.io(p[i]);
     }
   } else {
     np=0;
     /* read while ( until closing paren */
     while (a.Skip() != a.list_close) {
   	   if (a.eof) break;
	   newelement.io(a);
	   //a.io(newelement);
	   if (np==maxp)  SetMaxLength((np+8) + (np/2)); // make array bigger 10 + 50 %
	   Append(newelement);  // to do: ? no separate copy
	   // to do:list separator
     }
  }

  a.EndList();
  }
  return (a.stat);
}

// Macro to define a templates unary operator

#define  IMPLEMENT_ArrayUnaryOp(OPMNAME,OP,ARGDECL,ARG) \
template<class T>					\
Array<T>::OPNAME(ARGDECL)				\
{							\
  register int i = np;				\
  register T* dst = p ; 				\
  while(i--) *dst++ OP ARG;				\
};

///////////////////////////////////////////////////////////////////////////////

template<class T>
class NumericArray : public Array<T>
{
public:
	NumericArray() : Array<T>()	{};
	NumericArray(int n) : Array<T>(n) {};
	NumericArray(int n, const T iT): Array<T>(n,iT) {};
	NumericArray(int n, const T *iT): Array<T>(n,iT) {};
	NumericArray(const NumericArray & arg): Array<T>(arg) {};

	NumericArray<T>&    operator+=(const T Arg);
	NumericArray<T>&    operator-=(const T Arg);
	NumericArray<T>&    operator*=(const T Arg);
	NumericArray<T>&    operator/=(const T Arg);
   /* */
	NumericArray<T>&    operator+=(const NumericArray<T>& Arg);
	NumericArray<T>&    operator-=(const NumericArray<T>& Arg);
	NumericArray<T>&    operator*=(const NumericArray<T>& Arg);
	NumericArray<T>&    operator/=(const NumericArray<T>& Arg);
};

template<class T>
NumericArray<T>& NumericArray<T>::operator*=(const T Arg)
{
  register int i = np;
  register T* dst = p ;
  while (i--) *dst++ *= Arg;
  return (*this);
};

template<class T>
NumericArray<T>& NumericArray<T>::operator+=(const T Arg)
{
  register int i = np;
  register T* dst = p ;
  while(i--) *dst++ += Arg;
  return (*this);
};

template<class T>
NumericArray<T>& NumericArray<T>::operator-=(const T Arg)
{
  register int i = np;
  register T* dst = p ;
  while(i--) *dst++ -= Arg;
  return (*this);
};


template<class T>
NumericArray<T>& NumericArray<T>::operator/=(const T Arg)
{
  register int i = np;
  register T* dst = p ;
  while(i--) *dst++ /= Arg;
  return (*this);
};

template<class T>
NumericArray<T>& NumericArray<T>::operator+=(const NumericArray<T>& Arg)
{
  register int i = np;
  register T* src = Arg.p ;
  register T* dst = p ;
  if (i>Arg.np) i=Arg.np;
  while (i--) *dst++ += *src++;
  return (*this);
};

template<class T>
NumericArray<T>& NumericArray<T>::operator*=(const NumericArray<T>& Arg)
{
  register int i = np;
  register T* src = Arg.p ;
  register T* dst = p ;
  if (i>Arg.np) i=Arg.np;
  while (i--) *dst++ *= *src++;
  return (*this);
};

///////////////////////////////////////////////////////////////////////////////
// Predifined Arrayclasses
// one Reason : because int & float have no io member function
//

#if 1
class IntArray : public NumericArray<int> {
public:
	// constructors
	IntArray() : NumericArray<int>()	{};
	IntArray(int n) : NumericArray<int>(n) {};
	IntArray(int n, const int iT): NumericArray<int>(n,iT) {};
	IntArray(int n, const int *iT): NumericArray<int>(n,iT) {};
	IntArray(const IntArray & arg): NumericArray<int>(arg) {};
	int io(AStream &a);

};


class FloatArray : public NumericArray<float> {
public:
	// constructors
	FloatArray() : NumericArray<float>()	{};
	FloatArray(int n) : NumericArray<float>(n) {};
	FloatArray(int n, float iT): NumericArray<float>(n,iT) {};
	FloatArray(int n, float *iT): NumericArray<float>(n,iT) {};
	FloatArray(const FloatArray & arg): NumericArray<float>(arg) {};
	int io(AStream &a);
};

/*
Idea:
	refcounted memory blocks are avoiding data copy

	  ArrayRef
			refCnt

	
	  ArrayRef<T>
			Array<T> *array:

	
	  void Flush() {
			array->unref();
			array = NULL;
	  }
	  Set (ArrayRef<T> a) {
			Flush();
			array = a.array;
			array.ref();
	  }
				

*/


///////////////////////////////////////////////////////////////////////////////
#endif
#endif

