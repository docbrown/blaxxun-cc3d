/* 
 * This code is licensed under the Web3D-blaxxun Community Source License, 
 * provided in distribution file LICENSE.TXT and available online at 
 * http://www.web3D.org/TaskGroups/x3d/blaxxun/Web3D-blaxxunCommunitySourceAgreement.html 
 * and may only be used for non-commercial use as specified in that license. 
 * 
 * THE WEB3D CONSORTIUM AND BLAXXUN DO NOT MAKE AND HEREBY DISCLAIM ANY EXPRESS 
 * OR IMPLIED WARRANTIES RELATING TO THIS CODE, INCLUDING BUT NOT LIMITED TO, 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY OR FITNESS FOR A PARTICULAR 
 * PURPOSE, OR ANY WARRANTIES THAT MIGHT ARISE FROM A COURSE OF DEALING, USAGE 
 * OR TRADE PRACTICE.  THE COMMUNITY SOURCE CODE IS PROVIDED UNDER THIS 
 * AGREEMENT "AS IS," WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED, 
 * INCLUDING, WITHOUT LIMITATION, WARRANTIES THAT THE COMMUNITY SOURCE CODE ARE 
 * FREE OF DEFECTS, MERCHANTABLE, FIT FOR A PARTICULAR PURPOSE OR 
 * NON-INFRINGING OR IN ANY WAY CONSTITUTE THE COMPLETE PRODUCT MARKETED UNDER 
 * THE NAMES GIVEN SAID CODE. 
 */ 

/* LIBGUL - Geometry Utility Library
 * Copyright (c) 1999 by Norbert Irmer and Blaxxun Interactive 
 * All rights reserved 
 */ 

#ifndef GUAR_EXACT_H
#define GUAR_EXACT_H

#include <math.h>
// #include <new.h>

namespace guar {

// using namespace gust;
// using gust::PoolAlloc;
// using gust::PoolFree;
// using gul::Error;
// using gul::PoolAllocError;
// using gul::IntervalDivZeroError;

#ifdef _MSC_VER
  typedef unsigned _int64 uint64;
  #define isinf(f) (!_finite((f)))
  // 64-Bit Integer constants
  #define LL(i) i##i64
#else
  typedef unsigned long long uint64;
  // 64-Bit Integer constants
  #define LL(i) i##LL
#endif

/* ---------------------------------------------------------
  Converts int-string to double
----------------------------------------------------------- */

template< class T >
void IntTo( const int na, const unsigned long *a, T& t )
{
  int i;

  if( na == 0 )
  {
    t = (T)0.0;
    return;
  }
  t = (T)a[na-1];
  
  for( i = na-2; i >= 0; i-- )
  {
    t = t*(T)(LL(0x100000000)) + (T)a[i];
  }  
}

/* ----------------------------------------------------------------
  Calculates the sum of two operands
------------------------------------------------------------------ */
void IntAdd( const int na, const unsigned long *a, 
             const int nb, const unsigned long *b, 
             int *nc, unsigned long *c );

/* ---------------------------------------------------------------
  Calculates c=a-b with length of c = max(length(a),length(b)).
  If a < b the result c is a negative number (two-complement, i.e.
  not(fabs(c))+1), and the fuction returns -1, else it returns 0.
------------------------------------------------------------------ */
int IntSub( const int na, const unsigned long *a, 
            const int nb, const unsigned long *b, 
            int *nc, unsigned long *c );
                 
/* ---------------------------------------------------------------
  Short Multiplication
---------------------------------------------------------------- */
unsigned long IntMultShort( 
         const int na, const unsigned long *a, const unsigned long b,
         unsigned long *c );
         
/* ----------------------------------------------------------------
  Calculates the product of two operands, slow but simple
  TODO: Multiplication with FFT
------------------------------------------------------------------ */
void IntMult( const int na, const unsigned long *a, 
              const int nb, const unsigned long *b,
              int *nc, unsigned long *c );

/* ---------------------------------------------------------
  Divide a int-string by a single int
----------------------------------------------------------- */
unsigned long IntDivShort( 
       const int na, const unsigned long *a, const unsigned long b, 
       int *nq, unsigned long *q );

/* ---------------------------------------------------------
  Divide a int-string by a int-string
----------------------------------------------------------- */
int IntDiv( const int na, const unsigned long *a, 
            const int nb, const unsigned long *b, 
            int *nq, unsigned long *q, 
            int *nr, unsigned long *r );



/* ---------------------------------------------------------
  Converts double to int-string
----------------------------------------------------------- */
int DoubleToInt( const double d, const int na, unsigned long *a,
                 int *retLen );

/* ---------------------------------------------------------
  Converts int-string to double
----------------------------------------------------------- */
double IntToDouble( const int na, const unsigned long *a );



struct Interval
{
  friend Interval operator+( const Interval &a, const Interval &b );
  friend Interval operator-( const Interval &a, const Interval &b );
  friend Interval operator*( const Interval &a, const Interval &b );
  friend Interval operator/( const Interval &a, const Interval &b );
  friend int Test( const Interval &a, const Interval &b );

  double         m_low;
  double         m_high;

  Interval() { m_low = 0.0; m_high = 0.0; }
  Interval(double val) { m_low = val; m_high = val; }
  Interval(double low, double high) { m_low = low; m_high = high; }
};

/* -------------------------------------------------------------
  wrapper class, because i don't want implicit double -> long
  conversion
---------------------------------------------------------------*/
class ULong
{
  unsigned long l;
public:
  explicit ULong( unsigned long ul ) : l(ul) { }
  operator unsigned long() const { return l; }
};

class PrecRational
{
public:
  friend PrecRational operator*( const PrecRational& a, const PrecRational& b );
  friend PrecRational operator+( const PrecRational& A, const PrecRational& B );
  friend PrecRational operator/( const PrecRational& A, const PrecRational& B );
  friend PrecRational operator-( const PrecRational& A, const PrecRational& B );
  friend int Compare( const PrecRational& A, const PrecRational &B );

  struct PrecRationalRep
  {
    int            m_na;  /* number of words for a */
    unsigned long *m_a;   /* numerator   */

    int            m_nb;  /* number of words for b */
    unsigned long *m_b;   /* denominator */

    size_t         m_size_a;  /* allocation size in byte of a */ 
    size_t         m_size_b;  /* allocation size of byte of b */ 

    int            m_refcount;    /* Referencecounter */ 

    Interval       m_i;

    int            m_sign   : 2;  /* -1 = negative, 0 = positive or zero */
    unsigned int   m_bounds : 1;  /* lower and upper bound initialized */

    PrecRationalRep()
    {
      m_refcount = 1;
      m_sign = m_bounds = m_na = m_nb = m_size_a = m_size_b = 0;
      m_a = m_b = NULL;
    }

/*
    PrecRationalRep( const double f ) : m_i(f)
    {
      unsigned long *a;
      int sign,len;
      size_t bsize;
         
      m_refcount = 1;
      m_sign = m_bounds = m_na = m_nb = m_size_a = m_size_b = 0; 
      m_a = m_b = NULL;

      if( f == 0.0 ) {m_bounds=1; return; }
      
      a = (unsigned long *)PoolAlloc( 128*sizeof(unsigned long), &bsize );
      if( a == NULL ) throw PoolAllocError();

      sign = DoubleToInt( f, 128, a, &len );
      if( len == 0 ) {PoolFree(a,128*sizeof(unsigned long)); return;}

      m_a = (unsigned long *)PoolAlloc( len*sizeof(unsigned long), &bsize );
      if( m_a == NULL ) throw PoolAllocError();
      memcpy( m_a, a, len*sizeof(unsigned long) );

      PoolFree(a,128*sizeof(unsigned long)); 
  
      m_sign = sign;
      m_i.m_low = f; m_i.m_high = f; m_bounds = 1;
      m_na = len;
      m_size_a = bsize;
    }
*/

    PrecRationalRep( int sign, int na, unsigned long *a )
    {
      size_t bsize;

      m_refcount = 1;
      m_sign = m_bounds = m_na = m_nb = m_size_b = m_size_a = 0; 
      m_a = m_b = 0;

      if( na == 0 ) return;
      
      m_a = (unsigned long *)gust::PoolAlloc( na*sizeof(unsigned long), &bsize );
      if( m_a == NULL ) throw gul::PoolAllocError();
      for( int i=0; i<na; i++ ) m_a[i] = a[i];

      m_sign = sign;
      m_na = na;
      m_size_a = bsize;
    }
    
    PrecRationalRep( int sign, unsigned long a )
    {
      size_t bsize;

      m_a = (unsigned long *)gust::PoolAlloc( sizeof(unsigned long), &bsize );
      if( m_a == NULL ) throw gul::PoolAllocError();
      m_a[0] = a;

      m_refcount = 1;
      m_bounds = m_nb = m_size_b = 0; 
      m_b = 0;

      m_sign = sign;
      m_na = 1;
      m_size_a = bsize;
    }
 
            // this only reserves memory, but leaves it uninitialized
    PrecRationalRep( int sign, int na, int nb )
    {
      size_t bsize;

      m_refcount = 1;
      m_sign = m_bounds = m_na = m_nb = m_size_a = m_size_b = 0; 
      m_a = m_b = NULL;

      if( na != 0 )       
      {
        m_sign = sign;
        m_a = (unsigned long *)gust::PoolAlloc( na*sizeof(unsigned long),&bsize);
        if( m_a == NULL ) throw gul::PoolAllocError();
        m_size_a = bsize;
        m_na = na;
      }
      if( nb != 0 )       
      {     
        m_b = (unsigned long *)gust::PoolAlloc( nb*sizeof(unsigned long),&bsize);
        if( m_b == NULL ) throw gul::PoolAllocError();
        m_size_b = bsize;
        m_nb = nb;
      }
    }

    ~PrecRationalRep()
    {
      if( (m_size_a != 0) && (m_a != NULL) ) gust::PoolFree( m_a, m_size_a );      
      if( (m_size_b != 0) && (m_b != NULL) ) gust::PoolFree( m_b, m_size_b );
    }

    template< class T >
    void dump( T& num, T& den )
    { 
      if( m_na == 0 )
      {  num = (T)0.0; den = (T)1.0; return; }
      else
        IntTo<T>( m_na, m_a, num );

      if( m_sign ) 
        num = -num;

      if( m_nb == 0 )
        den = (T)1.0;
      else
        IntTo<T>( m_nb, m_b, den );  
    }

    void calc_bounds( void )
    { 
      Interval inum,iden;
      double num,den;

      dump( num, den );
      if( num != 0.0 )
      {
        inum.m_low = num - DBL_EPSILON*fabs(num);
        inum.m_high = num + DBL_EPSILON*fabs(num);
      }
      else
      {
        inum.m_low = inum.m_high = 0.0;
      }
      
      if( den != 1.0 )
      {
        iden.m_low = den - DBL_EPSILON*fabs(den);
        iden.m_high = den + DBL_EPSILON*fabs(den);
      }
      else
      {
        iden.m_low = iden.m_high = 1.0;
      }

      m_i = inum/iden;

      m_bounds = 1;
    }
   
    void negative( void )    /* for internal use only, no checks !!!! */ 
    {      
      if( m_na != 0 )
        m_sign ^= -1;
    }
    
    void reciprocal( void )  /* for internal use only, no checks !!!! */
    {
      size_t bsize;
      unsigned long *p;
      int i;
      
      if( m_na == 0 ) throw gul::Error();
      
      if( (m_na == 1) && (m_a[0] == 1) )
      {
        gust::PoolFree( m_a, m_size_a );
        m_na = 0;
        m_size_a = 0;
        m_a = NULL;
      }
      
      if( m_size_b == 0 )
      {
        m_b = (unsigned long *)gust::PoolAlloc( sizeof(unsigned long), &bsize );
        if( m_b == NULL ) throw gul::PoolAllocError();
        m_nb = 1;
        m_b[0] = 1;
        m_size_b = bsize;
      }

      i = m_na; m_na = m_nb; m_nb = i;
      bsize = m_size_a; m_size_a = m_size_b; m_size_b = bsize;
      p = m_a; m_a = m_b; m_b = p;
    }
  };



public:
  PrecRationalRep   *m;
  
  PrecRational( void )
  {
    size_t bsize;
    void *buf = gust::PoolAlloc( sizeof(PrecRationalRep), &bsize );
    if( buf == NULL ) throw gul::PoolAllocError();
    m = new(buf) PrecRationalRep;
  }
/*  
  PrecRational( const double f )
  {
    size_t bsize;
    void *buf = PoolAlloc( sizeof(PrecRationalRep), &bsize );
    if( buf == NULL ) throw PoolAllocError();
    m = new(buf) PrecRationalRep(f);
  }
*/
  explicit PrecRational( int na, unsigned long *a, int sign = 0 )
  {
    size_t bsize;
    void *buf = gust::PoolAlloc( sizeof(PrecRationalRep), &bsize );
    if( buf == NULL ) throw gul::PoolAllocError();
    m = new(buf) PrecRationalRep(sign,na,a);
  }

  explicit PrecRational( ULong a, int sign = 0 )
  {
    size_t bsize;
    void *buf = gust::PoolAlloc( sizeof(PrecRationalRep), &bsize );
    if( buf == NULL ) throw gul::PoolAllocError();
    m = new(buf) PrecRationalRep(sign,a);
  }

  explicit PrecRational( int sign, int size_a, int size_b )
  {
    size_t bsize;
    void *buf = gust::PoolAlloc( sizeof(PrecRationalRep), &bsize );
    if( buf == NULL ) throw gul::PoolAllocError();
    m = new(buf) PrecRationalRep( sign, size_a, size_b );
  }

  PrecRational( const PrecRational& other )
  {
    other.m->m_refcount++;
    m = other.m;
  }

  PrecRational& operator=( const PrecRational& other )
  {
    other.m->m_refcount++;

    if( --m->m_refcount == 0 ) 
    {
      m->~PrecRationalRep();
      gust::PoolFree( m, sizeof(PrecRationalRep) );      
    }

    m = other.m;

    return( *this );    
  } 

  ~PrecRational() 
  {
    if( --m->m_refcount == 0 ) 
    {
      m->~PrecRationalRep();
      gust::PoolFree( m, sizeof(PrecRationalRep) );      
    }
  }

  PrecRational GetCopy() const
  {
    PrecRational C(m->m_sign,m->m_na,m->m_nb);
    int i;
    
    if( m->m_na )
      for( i = 0; i < m->m_na; i++ )
        C.m->m_a[i] = m->m_a[i];
    if( m->m_nb )
      for( i = 0; i < m->m_nb; i++ )
        C.m->m_b[i] = m->m_b[i];
    return C;
  }

  operator int() const
  {
    int i;
    
    if( (m->m_na != 0) && (m->m_nb != 0) )
    {
      PrecRational Q,R;
  
      DivMod( &Q, &R );

      if( Q.m->m_na )
      {
        i = Q.m->m_a[0] & 0xefffffff;
        if( Q.m->m_sign ) i = -i;
      }
      else
        i = 0;
    }
    else
    {
      if( m->m_na )  
      {
        i = m->m_a[0] & 0xefffffff;
        if( m->m_sign ) i = -i;
      }
      else
        i = 0;
    }

    return(i);
  }

  void CalcBounds( void ) const { m->calc_bounds(); } 
  
  inline const Interval& GetBounds( void ) const
  {
    if( !m->m_bounds ) m->calc_bounds();
    return(m->m_i);
  }
  
  template< class T >
  void Dump( T& num, T& den ) const { m->dump( num, den ); }

  template< class T >
  void Dump( T& t ) const 
  {
    T n,d;
    
    Dump(n,d);
    
    t = n/d;
  } 

  bool IsZero( void ) const { return(m->m_na == 0); }

  int  Test( void ) const
  {
    if( m->m_sign ) return(-1);
    else if( m->m_na == 0 ) return(0);
    return(1);
  }

  void DivMod( PrecRational *q, PrecRational *r ) const;
};

/* ---------------------------------------------------------------
  functions for intervall arithmetic
--------------------------------------------------------------- */

inline Interval operator+( const Interval &a, const Interval &b )
{
  Interval c;
  
  c.m_low = a.m_low + b.m_low;
  if( !isinf(c.m_low) ) c.m_low = c.m_low - DBL_EPSILON*fabs(c.m_low);

  c.m_high = a.m_high + b.m_high;
  if( !isinf(c.m_high) ) c.m_high = c.m_high + DBL_EPSILON*fabs(c.m_high);
    
  return( c );
}

inline Interval operator-( const Interval &a, const Interval &b )
{
  Interval c(-b.m_high,-b.m_low);
  
  return(a+c);
}

inline Interval operator/( const Interval &a, const Interval &b )
{
  if( (b.m_low < 0.0) && (b.m_high > 0.0) )
    throw gul::IntervalDivZeroError();

  Interval c(1.0/b.m_high,1.0/b.m_low);
  
  return(a*c);
}

inline int Test( const Interval &a, const Interval &b )
{
  Interval c = a-b;
  
  if( c.m_high < 0.0 ) return(-1);
  if( c.m_low > 0.0 ) return(+1);

  return(0);   /* dunno :) */
}


/* ---------------------------------------------------------------
  functions for fractions arithmetic
--------------------------------------------------------------- */

inline PrecRational operator/( const PrecRational& A, const PrecRational& B )
{
  PrecRational c;
  
  if( A.m == B.m )
    return PrecRational(ULong(1));
  
  B.m->reciprocal();
  c = A * B;
  B.m->reciprocal();

  return(c);
}

inline PrecRational operator-( const PrecRational& A, const PrecRational& B )
{
  PrecRational c;  // (= 0)

  if( A.m == B.m )
    return c;

  if( A.m->m_na == 0 )   /* if A=0, A+B returns B !!! */
  {
    if( B.m->m_na != 0 )
    {
      c = B.GetCopy();
      c.m->m_sign = B.m->m_sign ^ -1;
    }
    return c;
  }
    
  B.m->negative();
  c = A + B;
  B.m->negative();

  return(c);
}

inline bool operator<( const PrecRational& A, int b )
{
  PrecRational B,C;
  unsigned long buf;
  
  if( b < 0 )
  {
    B.m->m_sign = -1;
    buf = -b;
    B.m->m_a = &buf;
    B.m->m_na = 1; 
  }
  else if( b > 0 )
  {
    buf = b;
    B.m->m_a = &buf;
    B.m->m_na = 1; 
  }
  
  C = A - B;

  return( C.m->m_sign != 0 );
}

inline bool operator<=( const PrecRational& A, int b )
{
  PrecRational B,C;
  unsigned long buf;
  
  if( b < 0 )
  {
    B.m->m_sign = -1;
    buf = -b;
    B.m->m_a = &buf;
    B.m->m_na = 1; 
  }
  else if( b > 0 )
  {
    buf = b;
    B.m->m_a = &buf;
    B.m->m_na = 1; 
  }
  
  C = A - B;

  if( C.m->m_sign || C.m->m_na == 0)
    return(true);

  return( false );
}

inline bool operator>=( const PrecRational& A, int b )
{
  return( !(A < b) );
}

inline int Compare( const PrecRational& A, const PrecRational &B )
{
  Interval i;
  
  if( A.m == B.m )
    return 0;

  if( !A.m->m_bounds ) A.CalcBounds();
  if( !B.m->m_bounds ) B.CalcBounds();  
  i = A.m->m_i - B.m->m_i;
  if( i.m_high < 0.0 ) return(-1); 
  else if( i.m_low > 0.0 ) return(1); 

  PrecRational c = A - B;
  if( c.m->m_sign ) return(-1);
  if( c.m->m_na == 0 ) return(0);

  return(1);
}

class PrecPoint2D
{
public:
  PrecRational m_x;
  PrecRational m_y;

  PrecPoint2D() { }
  explicit PrecPoint2D( const PrecPoint2D& o ) : m_x(o.m_x), m_y(o.m_y) { }
  explicit PrecPoint2D( ULong x, ULong y ) : m_x(x), m_y(y) { }
  explicit PrecPoint2D( const PrecRational& x, const PrecRational& y ) : 
                                                        m_x(x), m_y(y) { }
};

}

#endif


