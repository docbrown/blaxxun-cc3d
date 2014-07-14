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
 
#ifndef GUL_TYPES_H
#define GUL_TYPES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <float.h>
#include <math.h>

#ifdef _MSC_VER
  #include <malloc.h>
  #ifdef DLLBUILD
    #ifdef LIBBUILD
      #define EXP __declspec(dllexport)
    #else
      #define EXP __declspec(dllimport)
    #endif
  #else
    #define EXP
  #endif
#endif

#include "gul_error.h"
#include "gust_pool.h"
#include "gust_new.h"

// #if !defined(_MSC_VER) || defined(NEED_NEW_H)
//  #include <new.h>
// #endif

namespace gul
{

#ifndef NDEBUG
  const bool ndebug=false;
#else
  const bool ndebug=true;
#endif
  
  typedef float  flt;
  typedef double dbl;

  template<class T> struct rtr  /* rational number traits */
  { 
  };     

  template<> struct rtr<float>
  {
    static float epsilon() { return FLT_EPSILON; }
    static float epsilon_inv() { return 1.0f/FLT_EPSILON; }
    static size_t mantissa_length() { return 4; } // in multiples of 4
    static float pi() { return 3.14159265358979323846f; }
    static float root2_2() { return 1.41421356237309514547f; }
    static float root2_3() { return 1.7320508076887719318f; }
    static float golden_r() { return 0.61803399f; }
    static float golden_c() { return 1.0f-golden_r(); }

    static float  **BinCoeff;
    static int      BinCoeff_Pmax;
    static void     InitBinCoeff( const int Pmax );
    static void     ExitBinCoeff();

    static float fabs( const float a ) { return ::fabsf(a); }
    static float sqrt( const float a ) { return ::sqrtf(a); }

    static int id() { return(1); }
  };

  template<> struct rtr<double>
  {
    static double epsilon() { return DBL_EPSILON; }
    static float epsilon_inv() { return 1.0/DBL_EPSILON; }
    static size_t mantissa_length() { return 8; } // in multiples of 4
    static double pi() { return 3.14159265358979323846; }
    static double root2_2() { return 1.41421356237309514547; }
    static double root2_3() { return 1.7320508076887719318; }
    static double golden_r() { return 0.61803399; }
    static double golden_c() { return 1.0-golden_r(); }

    static double  **BinCoeff;
    static int       BinCoeff_Pmax;
    static void      InitBinCoeff( const int Pmax );
    static void      ExitBinCoeff();

    static double fabs( const double a ) { return ::fabs(a); }
    static double sqrt( const double a ) { return ::sqrt(a); }

    static int id() { return(2); }
  };

  template<class T> struct rtl  /* tolerances for comparisons - struct */
  { 
    T zero_tol;
    T zero_tol_v2;
    T zero_tol_v3;
    T zero_tol_v4;
    T point_coincidence_tol;
    T knot_coincidence_tol;
    T knot_uniqueness_tol;
    
    rtl() { }

    rtl( T zt, T pct, T kct, T kut )
    {
      zero_tol = zt;
      zero_tol_v2 = zero_tol / rtr<T>::root2_2();
      zero_tol_v3 = zero_tol / rtr<T>::root2_3();
      zero_tol_v4 = zero_tol / 2.0;
      point_coincidence_tol = pct;
      knot_coincidence_tol = kct;
      knot_uniqueness_tol = kut;
    }

    rtl( const rtl& a )
    {
      zero_tol = a.zero_tol;
      zero_tol_v2 = a.zero_tol_v2;
      zero_tol_v3 = a.zero_tol_v3;
      zero_tol_v4 = a.zero_tol_v4;
      point_coincidence_tol = a.point_coincidence_tol;
      knot_coincidence_tol = a.knot_coincidence_tol;
      knot_uniqueness_tol = a.knot_uniqueness_tol;
    }
  };     

  template< class T >
  inline T Max( const T a, const T b ) { return a > b ? a : b; } 
  template< class T >
  inline T Min( const T a, const T b ) { return a < b ? a : b; } 


  /* -------------------------------------------------------------------------
    Pointer with reference counter semantic
   -------------------------------------------------------------------------- */

  enum Storage
  {
    none,
    heap,
    pool
  };


  template <class T> 
  class Ptr
  { 
    template< class M >
    struct Ptr_rep
    {
     T           *m_ptr;
     int          m_refs;
     unsigned int m_type : 2;     /* storage class */
     unsigned int m_size : 30;
     int          m_nelems;

     Ptr_rep( T *p, int i, unsigned int t, unsigned int s, int n ) :
                     m_ptr(p), m_refs(i), m_type(t), m_size(s), m_nelems(n) { }
     inline ~Ptr_rep()
     { 
        T *p = m_ptr;
        for( int i = 0; i < m_nelems; i++ )
        {
          p->~T();
          p++;
        }
        gust::release((void *)m_ptr, m_type, m_size);
     }

     void realloc(size_t n) 
     { 
       if( m_type == heap )
       {
         m_ptr = (T *)::realloc( m_ptr, sizeof(T)*n );
         m_size = sizeof(T)*n;
         m_nelems = n;
       }
       else if( m_size < sizeof(T)*n )
         throw InternalError();  /* something went wrong */
     }
   };

   Ptr_rep<T>  *m_rep;

  public:
    explicit Ptr( T *p = 0, unsigned int t = 0, unsigned int s = 0, int n = 0 )
    { 
      size_t dummy;

      if( p != 0 )    
      {
        m_rep = (Ptr_rep<T> *)gust::PoolAlloc( sizeof(Ptr_rep<T>), &dummy );
        if( m_rep == 0 ) throw gul::AllocError();

        new(m_rep) Ptr_rep<T>(p,1,t,s,n);
      }
      else
        m_rep = 0;
    }

    Ptr( const Ptr& other )
    {
      if( other.m_rep ) other.m_rep->m_refs++;
      m_rep = other.m_rep;
    }

    Ptr& operator=( const Ptr& other )
    {
      other.m_rep->m_refs++;

      if( (m_rep != 0) && (--m_rep->m_refs == 0) ) 
      {
        m_rep->~Ptr_rep<T>();
        gust::PoolFree( m_rep, sizeof(Ptr_rep<T>) );      
      }

      m_rep = other.m_rep;

      return( *this );    
    } 

    ~Ptr() 
    {
      if( (m_rep != 0) && (--m_rep->m_refs == 0) ) 
      {
        m_rep->~Ptr_rep<T>();
        gust::PoolFree( m_rep, sizeof(Ptr_rep<T>) );      
      }
    }

    int nElems()   /* Ptr remembers the number of element to which it points ! */
    { 
      if( m_rep != 0 ) 
        return m_rep->m_nelems; 
      else 
        return 0;
    }

    void reset( T *p, int r, unsigned int t, unsigned int s, int n )
    {
      size_t dummy;
    
      if( m_rep == 0 )
      {
        m_rep = (Ptr_rep<T> *)gust::PoolAlloc( sizeof(Ptr_rep<T>), &dummy );
        if( m_rep == 0 ) throw gul::AllocError();
      }
      else
      {
        if( --(m_rep->m_refs) == 0 )
        {
          m_rep->~Ptr_rep<T>();
        }
        else
        {
          m_rep = (Ptr_rep<T> *)gust::PoolAlloc( sizeof(Ptr_rep<T>), &dummy );
          if( m_rep == 0 ) throw gul::AllocError();
        }
      }
      new(m_rep) Ptr_rep<T>(p,r,t,s,n);   
    }

    void use_pointer( T *p, int n )
    {
      size_t dummy;
    
      if( m_rep == 0 )
      {
        m_rep = (Ptr_rep<T> *)gust::PoolAlloc( sizeof(Ptr_rep<T>), &dummy );
        if( m_rep == 0 ) throw gul::AllocError();
      }
      else
      {
        if( --(m_rep->m_refs) == 0 )
        {
          m_rep->~Ptr_rep<T>();
        }
        else
        {
          m_rep = (Ptr_rep<T> *)gust::PoolAlloc( sizeof(Ptr_rep<T>), &dummy );
          if( m_rep == 0 ) throw gul::AllocError();
        }
      }
      new(m_rep) Ptr_rep<T>(p,1,none,0,n);   
    }

    void reserve_global( size_t n )
    {
      size_t s;
      T *p = (T *)gust::reserve_global( n*sizeof(T), &s );
    
      reset( p, 1, heap, s, n );
      for( size_t i = 0; i < n; i++ ) new(p++) T();  /* call constructors */
    }

    void reserve_pool( size_t n )
    {
      size_t s;
      T *p = (T *)gust::reserve_pool( n*sizeof(T), &s );
    
      reset( p, 1, pool, s, n );
      for( size_t i = 0; i < n; i++ ) new(p++) T();  /* call constructors */
    }

    void reserve_place( void *buf, size_t n )
    {
      T *p = (T *)buf;
      reset( p, 1, none, n*sizeof(T), n );              /* call constructors */
      for( size_t i = 0; i < n; i++ ) new(p++) T();
    }


    void reset()       // delete all elements
    {
      if( m_rep == 0 ) return;

      if( --(m_rep->m_refs) == 0 )
      {
        m_rep->~Ptr_rep<T>();
        gust::PoolFree( m_rep, sizeof(Ptr_rep<T>) ); 
      }
      m_rep = 0;
    }
 
    T& operator*() const { return *(m_rep->m_ptr); }
    T& operator[](size_t i) const { return (m_rep->m_ptr)[i]; }

//  T* operator->() const { return m_rep->m_ptr; }
    T* get() const { return m_rep->m_ptr; }
//  operator T*() const { return m_rep->m_ptr; }
 
    T* release()
    {
      if( (m_rep == 0) || (m_rep->m_refs != 1) )
        throw Error();
      T *p = m_rep->m_ptr;
      gust::PoolFree( m_rep, sizeof(Ptr_rep<T>) ); 
      m_rep = 0;
      return(p);
    }


    void realloc(size_t n) const  // low level, not for classes !
    { if(m_rep != 0) m_rep->realloc(n); else throw InternalError();  }
  };



  typedef int sub_range[2];

  template< class T >
  struct vec4
  {
    T m[4];
    T& operator[] (size_t i) { return m[i]; }
  };

  template< class T >
  struct mat4x4
  {
    vec4<T> m[4];    
    vec4<T>& operator[] (size_t i) { return m[i]; }
  };
  
  template <class T>
  struct bounding_box
  {
    T x1;
    T x2;
    T y1;
    T y2;
    T z1;
    T z2;
  };

  template <class T>
  struct point
  {
    T x;
    T y;
    T z;
    static int dim() { return 3; }
  };
  template <class T>
  struct point2
  {
    T x;
    T y;
    static int dim() { return 2; }
  };
  template <class T>
  struct point1
  {
    T x;
    static int dim() { return 1; }
  };
  
  template <class T>
  struct line
  {
    point<T> P1;
    point<T> P2;
  };
  template<class T>
  struct line2
  {
    point2<T> P1;
    point2<T> P2;
  };

  template<class T>
  struct hpoint
  {
    T x;
    T y;
    T z;
    T w;
    static int dim() { return 4; }
//    hpoint<T>& operator=( const hpoint<T>& a )
//    { x = a.x; y = a.y; z = a.z; w = a.w; return *this; }
  };
  template<class T>
  struct hpoint2
  {
    T x;
    T y;
    T w;
    static int dim() { return 3; }
  };
  template<class T>
  struct hpoint1
  {
    T x;
    T w;
    static int dim() { return 2; }
  };
  
  template<class T>
  struct triangle
  {
    point<T> P1;
    point<T> P2;
    point<T> P3;
  };

  template<class T>
  struct triangle2
  {
    point2<T> P1;
    point2<T> P2;
    point2<T> P3;
  };
  
  template<class T>
  struct knot_vec
  {
    int         m;
    Ptr< T >    U;
  };

  template< class T >
  struct polyline
  {
    int              n;
    Ptr< point<T> >  P;
  };

  template<class T>
  struct polyline2
  {
    int              n;
    Ptr< point2<T> > P;
  };

  template<class T>
  struct polynom_term
  {
    int            ix;
    int            iy;  
    int            iz;
    T              c;
  };

  template<class T>
  struct polynom_data
  {
    int                     n;
    int                     p;
    Ptr< polynom_term<T> >  t;
  };

  template< class HP >
  struct point_vec
  {
    int              n;
    Ptr< HP >        Pw;
  };

  template<class T, class HP>
  struct curve
  {
    int             p;
    knot_vec<T>     knt;
    point_vec<HP>   cpt;
  };

  template<class HP>
  struct point_net
  {  
    int               nu;
    int               nv;
    Ptr< Ptr< HP > >  Pw;
  };

  template<class T, class HP> 
  struct surface
  {
    int            pu;
    int            pv;
    knot_vec<T>    knu;
    knot_vec<T>    knv;
    point_net<HP>  net;
  };  

  template<class T, class HP> 
  struct trimmed_surface
  {
    surface<T,HP>       surf;
    int                 nBounds;
    Ptr< polyline2<T> > bounds;
  };

                                        /* simple list templates */
  template< class T > struct ListNode
  {
    T el;
    ListNode *next;
    ListNode *prev;

    ListNode() { }
    ListNode( const T& a ) : el(a) { }
    
    void *operator new( size_t s )
    {
      size_t dummy;
      void *p = gust::PoolAlloc( s, &dummy );
      if( p == NULL ) throw PoolAllocError();
      return(p);
    }
    void operator delete( void *p, size_t s )
    {
      gust::PoolFree( p, s );
    }
  };

  template <class T> class List   // simple List
  {
  public:
    T   *head;
    int  nElems;
  
    List() { head = NULL; nElems = 0; }

    bool IsEmpty(void) { return(nElems == 0); }

    void Append( T *node )
    {
      node->next = head; 
      node->prev = NULL;
      if( head != NULL ) head->prev = node; 
      head = node;
      nElems++;
    }

    void Remove( T *node )
    {
      if( node->prev != NULL ) node->prev->next = node->next;
      if( node->next != NULL ) node->next->prev = node->prev;
      if( head == node ) head = node->next;
      nElems--;
    }
    void DeleteElems( void )
    {
      T *n = head, *nnext;
      while( n != NULL ) { nnext = n->next; delete n; n = nnext; }
      head = NULL; nElems = 0;
    }
    void ReleaseElems( void )
    {
      head = NULL; nElems = 0;
    }

  };

  template <class T> class List2   // simple List, bidirectional
  {
  public:
    T   *tail;
    T   *head;
    int  nElems;
  
    List2() { head = NULL; tail = NULL; nElems = 0; }

    T *First(void) { return head; }
    T *Last(void)  { return tail; }
  
    bool IsEmpty(void) { return(nElems == 0); }
  
    void AppendLeft( T *node )
    {
      node->next = head; 
      node->prev = NULL;
      if( head != NULL ) { head->prev = node; } else { tail = node; } 
      head = node;
      nElems++;
    }
    void AppendRight( T *node )
    {
      node->prev = tail; 
      node->next = NULL;
      if( tail != NULL ) { tail->next = node; } else { head = node; } 
      tail = node;
      nElems++;
    }
    void Remove( T *node )
    {
      if( node->prev != NULL ) node->prev->next = node->next;
      if( node->next != NULL ) node->next->prev = node->prev;
      if( head == node ) head = node->next;
      if( tail == node ) tail = node->prev;
      nElems--;
    }
    void DeleteElems( void )
    {
      T *n = head, *nnext;
      while( n != NULL ) { nnext = n->next; delete n; n = nnext; }
      head = tail = NULL; nElems = 0;
    }
    void ReleaseElems( void )
    {
      head = tail = NULL; nElems = 0;
    }
  };

}

#ifdef _MSC_VER    // i hate it ! :(((( 
#include "gul_vector.h"

namespace gul {

template point2<float> euclid( const hpoint2<float>& a );

}
#endif 


#endif
