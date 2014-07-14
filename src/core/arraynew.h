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
#ifndef _ArrayNew_H
#define _ArrayNew_H
/******************************************************************************
@doc

@module ArrayNew.h - Template based ArrayNew class |

Copyright (c) 1993 - 1995	by Holger Grahn
All rights reserved

Purpose:

Template based ArrayNew class
Designed to easily interchange with C ArrayNew notation
and "by value" elements
Derived class NumericArrayNew supports common arithmetic operators


Classes:
ArrayNew<T>
	NumericArrayNew<T>
		IntArrayNew
		FloatArrayNew


Notes:

Changes:
    change ArrayNew_EXPANSION
            if < limit, only double size
    MALLOC, CALLOC, FREE debug routines


Todo :
Automatic Expansion + Expansion factor
Implementation change :
	use malloc, realloc, free instead of new & delete ?
******************************************************************************/

//#include <iostream.h>



#ifdef __HIGHC__
pragma Off(Auto_class_member_instantiation);
//pragma On(Print_template_usage);
#endif

class AStream;
// Auto expand on Append(T)
#define ArrayNew_EXPANSION 1024



//\@class Template ArrayNew class
//\@tcarg class | T | A class to store in ArrayNew (by value)

template<class T>
class ArrayNew {
protected :
	T * p;		// Pointer to data
	int np;		// actual length
	int maxp;	// maximum allocated length np<=maxp
    int byRef;	// if true, memory not owned by ArrayNew 

public:
	// constructors
	ArrayNew() : np(0),maxp(0),byRef(0) { p = 0;};
	ArrayNew(int n) { maxp = np = n; byRef = 0;
                     if (n == 0) p = 0; 
					 else { p = alloc(np); }
                  };
	ArrayNew(int n, const T iT);
	ArrayNew(int n, const T* iT);
	ArrayNew(const ArrayNew<T>&);


	// allocation functions 
    T* alloc(int n);  
    T* realloc(T* pT, int n);
    void free(T* pT);


	//@cmember destructor
	~ArrayNew();
	
	void Flush();

	// assignment
	ArrayNew<T>&	operator=(const ArrayNew<T>&);

	// Set

	ArrayNew<T>& Set(const ArrayNew<T>& a);
	ArrayNew<T>& SetByMove(const ArrayNew<T>& a);
	ArrayNew<T>& Set(int n, const T *iT);
	ArrayNew<T>& Set(int n, const T val);

	ArrayNew<T>& SetToRef(const ArrayNew<T>& a);
    ArrayNew<T>& SetToRef(int n, const T *iT);
	ArrayNew<T>& UnRef();

	// access
	const T*	Data() const {return p;}
	T*		Data() {return p;}
	
    int	Length() const { return np; }
	int	MaxLength() const { return maxp; }
    int	ByRef() const { return byRef; }

	operator T*()	{ return p; }
	operator const T*() const { return p; }


	T&		operator()(int i)	{ return p[i]; };
	T		operator()(int i) const { return p[i]; };
	
	T&		operator[](int i)
	{ /* if (i<0 || i>=np) callerror(ArrayNew,T,1,"ArrayNew index out of range."); */
		return p[i]; 
	};
	
	T		operator[](int i) const
	{  /* if (i<0 || i>=np) callerror(ArrayNew,T,1,"ArrayNew index out of range."); */
		return p[i]; 
	};

	// set length to n, make sure buffer fits, exact = 1 buffer will be reallocated
	void Resize(int N,int exact=0);

	// Set Length of ArrayNew 
	void SetLength(int NewLength) { Resize(NewLength); }

	// Set Maximum Length of ArrayNew, usefull to preallocate space 
	void SetMaxLength(int NewLength) { 
               int l=Length();
				   Resize(NewLength);
				   if (l<maxp) np=l;
				};

	void		Swap(int i,int j);
	
	ArrayNew<T>&	Append(const ArrayNew<T>&);
	ArrayNew<T>&	Append(int n, const T*);
	ArrayNew<T>&	Append(T);
	
	ArrayNew<T>&	Insert(int ofs, const ArrayNew<T>& a);
	ArrayNew<T>&	Delete(int ofs, int cnt);

	int	Search(const T val,int start=0);

	// io
	int io(AStream &a);
};


template<class T>
T * ArrayNew<T>::alloc(int n)   
{ 
	return new T[n];  
}

template<class T>
T * ArrayNew<T>::realloc(T * pT,int N)
{

   T* newArrayNew = alloc(N);
   register int i = (N<=np) ? N:np;
   register const T* src = pT;
   register T* dst = newArrayNew;
   if (src) {
      while (i--) *dst++ = *src++;
      if (!byRef) free(pT);
   }
   return(newArrayNew);
}

template<class T>
void ArrayNew<T>::free(T* pT)  
{  
	delete[] pT; 
}



// constructors

template<class T>
ArrayNew<T>::ArrayNew(int n, const T iT)
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
ArrayNew<T>::ArrayNew(const ArrayNew<T>& a)
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
ArrayNew<T>::ArrayNew(int n, const T *iT)
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
ArrayNew<T>::~ArrayNew() { 
	if (!byRef) free(p); p=0; byRef= 0; 
}

template<class T>
void ArrayNew<T>::Flush()	
{ 
	if (!byRef) free(p); maxp= np =0; p = 0; byRef = 0;
}


template<class T>
ArrayNew<T>&
ArrayNew<T>::Set(const ArrayNew<T>& a)
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

// "copy" ArrayNew contents from a to self
// by taking over "a" data ptrs
// a will be empty after this operation
template<class T>
ArrayNew<T>&
ArrayNew<T>::SetByMove(const ArrayNew<T>& a)
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
ArrayNew<T>&
ArrayNew<T>::Set(int n, const T *iT)
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
ArrayNew<T>&
ArrayNew<T>::Set(int n, const T val)
{
    Resize(n);
    register int i=n;
    register T* dst = p ;
    while (i--) *dst++ = val;
  return *this;
};

// "copy" ArrayNew contents from a to self
// by taking over "a" data ptrs
// and set byRef flag

template<class T>
ArrayNew<T>&
ArrayNew<T>::SetToRef(const ArrayNew<T>& a)
{
  if(this != &a){
	Flush();
	np=a.np;
	p=a.p;
	maxp=a.maxp;
   byRef=1;
  }
  return *this;
};

// "copy" ArrayNew contents from a to self
// by taking over "a" data ptrs
// and set byRef flag

template<class T>
ArrayNew<T>&
ArrayNew<T>::SetToRef(int n, const T *iT)
{
	Flush();
	np=n;
	p=iT;
	maxp=n;
   byRef=1;
  return *this;
};

// own memory
// 
template<class T>
ArrayNew<T>& 
ArrayNew<T>::UnRef()
{
  if (byRef) Resize(np,1);
  return *this;
}



template<class T>
ArrayNew<T>&
ArrayNew<T>::operator=(const ArrayNew<T>& a)
{
  return Set(a);
};

template<class T>
void
ArrayNew<T>::Resize(int N,int exact)
{
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

   T* newArrayNew = alloc(N);
   register int i = (N<=np) ? N:np;
   register const T* src = p;
   register T* dst = newArrayNew;
   if (src) {
      while (i--) *dst++ = *src++;
      if (!byRef) free(p);
   }
   p = newArrayNew;
   byRef=0;
  }
  maxp = np = N;
};

template<class T>
void
ArrayNew<T>::Swap(int i,int j)
{ T  tmp(p[i]);
  p[i] = p[j];
  p[j] = tmp;
};

template<class T>
ArrayNew<T>&
ArrayNew<T>::Append(const ArrayNew<T>& a)
{

  return(Append(a.np,a.p));
};


template<class T>
ArrayNew<T>&
ArrayNew<T>::Append(int n,const T* iT)
{
  register int i= n;
  register T* dst;
  register const T* src = iT;
  int start = np;
  // check for overflow ,test
  if ((np+n) > maxp) {
  	if (n < ArrayNew_EXPANSION) {
        // new 9.04.96
        if (maxp<ArrayNew_EXPANSION) 
          SetMaxLength(np+max(n,maxp*2));
	   	else SetMaxLength(maxp+ArrayNew_EXPANSION);
  	}
  	else SetMaxLength(np+n);	// expand me
  }
  np+=n;

  dst = & p[start];	// point after old end
  while (i--) *dst++ = *src++;
  return(*this);
};

template<class T>
ArrayNew<T>&
ArrayNew<T>::Append(T a)
{
  register int i= np;
  if (np == maxp) {
      // new 9.04.96
      if (maxp<ArrayNew_EXPANSION) 
          Resize(np+max(1,maxp*2));
      else
        Resize(np+ArrayNew_EXPANSION /* 1 */); // expand me
  }  
  p[i]	= a;
  np = i+1;
  return(*this);
};

template<class T>
ArrayNew<T>&
ArrayNew<T>::Insert(int ofs, const ArrayNew<T>& a)
{
  register int i;
  register T* dst;
  register T* src;
  int start = np;

  if (!a) return(*this);
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

template<class T>
ArrayNew<T>&
ArrayNew<T>::Delete(int ofs, int cnt)
{
  register int i;
  register T* dst;
  register T* src;

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

template<class T>
int
ArrayNew<T>::Search(const T val,int start)
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
void  ArrayNew<T>::print(ostream &os)
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
ArrayNew<T>::io(AStream &a)
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
	   if (np==maxp)  SetMaxLength((np+8) + (np/2)); // make ArrayNew bigger 10 + 50 %
	   Append(newelement);  // to do: ? no separate copy
	   // to do:list separator
     }
  }

  a.EndList();
  }
  return (a.stat);
}

// Macro to define a templates unary operator

#define  IMPLEMENT_ArrayNewUnaryOp(OPMNAME,OP,ARGDECL,ARG) \
template<class T>					\
ArrayNew<T>::OPNAME(ARGDECL)				\
{							\
  register int i = np;				\
  register T* dst = p ; 				\
  while(i--) *dst++ OP ARG;				\
};

///////////////////////////////////////////////////////////////////////////////

template<class T>
class NumericArrayNew : public ArrayNew<T>
{
public:
	NumericArrayNew() : ArrayNew<T>()	{};
	NumericArrayNew(int n) : ArrayNew<T>(n) {};
	NumericArrayNew(int n, const T iT): ArrayNew<T>(n,iT) {};
	NumericArrayNew(int n, const T *iT): ArrayNew<T>(n,iT) {};
	NumericArrayNew(const NumericArrayNew & arg): ArrayNew<T>(arg) {};

	NumericArrayNew<T>&    operator+=(const T Arg);
	NumericArrayNew<T>&    operator-=(const T Arg);
	NumericArrayNew<T>&    operator*=(const T Arg);
	NumericArrayNew<T>&    operator/=(const T Arg);
   /* */
	NumericArrayNew<T>&    operator+=(const NumericArrayNew<T>& Arg);
	NumericArrayNew<T>&    operator-=(const NumericArrayNew<T>& Arg);
	NumericArrayNew<T>&    operator*=(const NumericArrayNew<T>& Arg);
	NumericArrayNew<T>&    operator/=(const NumericArrayNew<T>& Arg);
};

template<class T>
NumericArrayNew<T>& NumericArrayNew<T>::operator*=(const T Arg)
{
  register int i = np;
  register T* dst = p ;
  while (i--) *dst++ *= Arg;
  return (*this);
};

template<class T>
NumericArrayNew<T>& NumericArrayNew<T>::operator+=(const T Arg)
{
  register int i = np;
  register T* dst = p ;
  while(i--) *dst++ += Arg;
  return (*this);
};

template<class T>
NumericArrayNew<T>& NumericArrayNew<T>::operator-=(const T Arg)
{
  register int i = np;
  register T* dst = p ;
  while(i--) *dst++ -= Arg;
  return (*this);
};


template<class T>
NumericArrayNew<T>& NumericArrayNew<T>::operator/=(const T Arg)
{
  register int i = np;
  register T* dst = p ;
  while(i--) *dst++ /= Arg;
  return (*this);
};

template<class T>
NumericArrayNew<T>& NumericArrayNew<T>::operator+=(const NumericArrayNew<T>& Arg)
{
  register int i = np;
  register T* src = Arg.p ;
  register T* dst = p ;
  if (i>Arg.np) i=Arg.np;
  while (i--) *dst++ += *src++;
  return (*this);
};

template<class T>
NumericArrayNew<T>& NumericArrayNew<T>::operator*=(const NumericArrayNew<T>& Arg)
{
  register int i = np;
  register T* src = Arg.p ;
  register T* dst = p ;
  if (i>Arg.np) i=Arg.np;
  while (i--) *dst++ *= *src++;
  return (*this);
};

///////////////////////////////////////////////////////////////////////////////
// Predifined ArrayNewclasses
// one Reason : because int & float have no io member function
//

#if 0
class IntArrayNew : public NumericArrayNew<int> {
public:
	// constructors
	IntArrayNew() : NumericArrayNew<int>()	{};
	IntArrayNew(int n) : NumericArrayNew<int>(n) {};
	IntArrayNew(int n, const int iT): NumericArrayNew<int>(n,iT) {};
	IntArrayNew(int n, const int *iT): NumericArrayNew<int>(n,iT) {};
	IntArrayNew(const IntArrayNew & arg): NumericArrayNew<int>(arg) {};
	int io(AStream &a);

};


class FloatArrayNew : public NumericArrayNew<float> {
public:
	// constructors
	FloatArrayNew() : NumericArrayNew<float>()	{};
	FloatArrayNew(int n) : NumericArrayNew<float>(n) {};
	FloatArrayNew(int n, float iT): NumericArrayNew<float>(n,iT) {};
	FloatArrayNew(int n, float *iT): NumericArrayNew<float>(n,iT) {};
	FloatArrayNew(const FloatArrayNew & arg): NumericArrayNew<float>(arg) {};
	int io(AStream &a);
};




///////////////////////////////////////////////////////////////////////////////
#endif
#endif

