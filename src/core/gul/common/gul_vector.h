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
 
#ifndef GUAR_VECTOR_H
#define GUAR_VECTOR_H

namespace gul
{

/**********************************************************************
  Homogeneus 3-D points
**********************************************************************/

                                            /* -- set all components ------ */
template< class T >
inline void set( hpoint<T>& c, T a )
{
  c.x = a; c.y = a; c.z = a; c.w = a;
}
                               /* ------ add two homogeneous vectors ------ */
template <class T>
inline hpoint<T> operator+ ( const hpoint<T>& a, const hpoint<T>& b )
{
  hpoint<T> c = {a.x+b.x, a.y+b.y, a.z+b.z, a.w+b.w};
  return c;
}
                         /* --- invert the sign of a homogeneous vector --- */
template <class T>
inline hpoint<T> operator- ( const hpoint<T>& a )
{
  hpoint<T> c = {-a.x, -a.y, -a.z, -a.w};
  return c;
}
      /* --- calculate the difference a - b  of homogeneous vectors a,b --- */

template <class T>
inline hpoint<T> operator- ( const hpoint<T>& a, const hpoint<T>& b )
{
  hpoint<T> c = {a.x-b.x, a.y-b.y, a.z-b.z, a.w-b.w};
  return c;
}
                   /* ---- multiply a scalar with a homogeneous vector ---- */
template <class T>
inline hpoint<T> operator* ( const T op, const hpoint<T>& a )
{
  hpoint<T> c = {op*a.x, op*a.y, op*a.z, op*a.w};
  return c;
}
            /* --- calculate the dot product of two homogeneus vectors ---- */
template <class T>
inline T operator* ( const hpoint<T>& a, const hpoint<T>& b )
{
  return a.x*b.x + a.y*b.y + a.z*b.z + a.w*b.w;
}



            /*----------------------------------------------------------------
              project a homogeneous point into euclidian space (3-dimensional).
              (hyperplane with w = 1)
             ----------------------------------------------------------------*/
template <class T>
inline point<T> euclid( const hpoint<T>& a )
{
  point<T> c = {a.x/a.w, a.y/a.w, a.z/a.w };
  return c;
}

/**********************************************************************
  Homogeneus 2-D points
**********************************************************************/

                                            /* -- set all components ------ */
template< class T >
inline void set( hpoint2<T>& c, T a )
{
  c.x = a; c.y = a; c.w = a;
}
                               /* ------ add two homogeneous vectors ------ */
template <class T>
inline hpoint2<T> operator+ ( const hpoint2<T>& a, const hpoint2<T>& b )
{
  hpoint2<T> c = {a.x+b.x, a.y+b.y, a.w+b.w};
  return c;
}
                         /* --- invert the sign of a homogeneous vector --- */
template <class T>
inline hpoint2<T> operator- ( const hpoint2<T>& a )
{
  hpoint2<T> c = {-a.x, -a.y, -a.w};
  return c;
}
      /* --- calculate the difference a - b  of homogeneous vectors a,b --- */
template <class T>
inline hpoint2<T> operator- ( const hpoint2<T>& a, const hpoint2<T>& b )
{
  hpoint2<T> c = {a.x-b.x, a.y-b.y, a.w-b.w};
  return c;
}
                   /* ---- multiply a scalar with a homogeneous vector ---- */
template <class T>
inline hpoint2<T> operator* ( const T op, const hpoint2<T>& a )
{
  hpoint2<T> c = {op*a.x, op*a.y, op*a.w};
  return c;
}
            /* --- calculate the dot product of two homogeneus vectors ---- */
template <class T>
inline T operator* ( const hpoint2<T> a, const hpoint2<T>& b )
{
  return a.x*b.x + a.y*b.y + a.w*b.w;
}
            /*----------------------------------------------------------------
              project a homogeneous point into euclidian space (3-dimensional).
              (hyperplane with w = 1)
             ----------------------------------------------------------------*/

template <class T>
inline point2<T> euclid( const hpoint2<T>& a )
{
  point2<T> c = { a.x/a.w, a.y/a.w };
  return c;
}


/**********************************************************************
  Homogeneus 1-D points
**********************************************************************/

                                            /* -- set all components ------ */
template< class T >
inline void set( hpoint1<T>& c, T a )
{
  c.x = a; c.w = a;
}
                               /* ------ add two homogeneous vectors ------ */
template <class T>
inline hpoint1<T> operator+ ( const hpoint1<T>& a, const hpoint1<T>& b )
{
  hpoint1<T> c = {a.x+b.x, a.w+b.w};
  return c;
}
                         /* --- invert the sign of a homogeneous vector --- */
template <class T>
inline hpoint1<T> operator- ( const hpoint1<T>& a )
{
  hpoint1<T> c = {-a.x, -a.w};
  return c;
}
      /* --- calculate the difference a - b  of homogeneous vectors a,b --- */
template <class T>
inline hpoint1<T> operator- ( const hpoint1<T>& a, const hpoint1<T>& b )
{
  hpoint1<T> c = {a.x-b.x, a.w-b.w};
  return c;
}
                   /* ---- multiply a scalar with a homogeneous vector ---- */
template <class T>
inline hpoint1<T> operator* ( const T op, const hpoint1<T>& a )
{
  hpoint1<T> c = {op*a.x, op*a.w};
  return c;
}
            /* --- calculate the dot product of two homogeneus vectors ---- */
template <class T>
inline T operator* ( const hpoint1<T>& a, const hpoint1<T>& b )
{
  return a.x*b.x + a.w*b.w;
}
            /*----------------------------------------------------------------
              project a homogeneous point into euclidian space (3-dimensional).
              (hyperplane with w = 1)
             ----------------------------------------------------------------*/
template <class T>
inline point1<T> euclid( const hpoint1<T>& a )
{
  point1<T> c = {a.x/a.w};
  return c;
}








/*************************************************************************
  3-D points
*************************************************************************/

                                            /* -- set all components ------ */
template< class T >
inline void set( point<T>& c, T a )
{
  c.x = a; c.y = a; c.z = a;
}
                         /* --- calculate the sum a + b  of vectors a,b --- */
template <class T>
inline point<T> operator+ ( const point<T>& a, const point<T>& b )
{
  point<T> c = {a.x+b.x, a.y+b.y, a.z+b.z};
  return c;
}
                                     /* --- invert the sign of a vector --- */
template <class T>
inline point<T> operator- ( const point<T>& a )
{
  point<T> c = {-a.x, -a.y, -a.z};
  return c;
}
                  /* --- calculate the difference a - b  of vectors a,b --- */
template <class T>
inline point<T> operator- ( const point<T>& a, const point<T>& b )
{
  point<T> c = {a.x-b.x, a.y-b.y, a.z-b.z};
  return c;
}
                               /* ---- multiply a scalar with a vector ---- */
template <class T>
inline point<T> operator* ( const T op, const point<T>& a  )
{
  point<T> c = {op*a.x, op*a.y, op*a.z};
  return c;
}
                       /* --- calculate the dot product of two vectors ---- */
template <class T>
inline T operator* ( const point<T>& a, const point<T>& b )
{
  return a.x*b.x + a.y*b.y + a.z*b.z;
}

/*************************************************************************
  2-D points
*************************************************************************/

                                           /* -- test if unequal ------ */
template< class T >
inline bool operator!= ( const point2<T>& a, const point2<T>& b )
{
  if( (a.x != b.x) || (a.y != b.y) ) return true;
  return false;
}
                                           /* -- test if equal ------ */
/* template< class T >
inline bool operator== ( const point2<T> a, const point2<T> b )
{
  if( (a.x == b.x) && (a.y == b.y) ) return true;
  return false;
} */

                                        /* -- set all components ------ */
template< class T >
inline void set( point2<T>& c, T a )
{
  c.x = a; c.y = a;
}
                         /* --- calculate the sum a + b  of vectors a,b --- */
template <class T>
inline point2<T> operator+ ( const point2<T>& a, const point2<T>& b )
{
  point2<T> c = {a.x+b.x, a.y+b.y};
  return c;
}
                                     /* --- invert the sign of a vector --- */
template <class T>
inline point2<T> operator- ( const point2<T>& a )
{
  point2<T> c = {-a.x, -a.y};
  return c;
}
                  /* --- calculate the difference a - b  of vectors a,b --- */
template <class T>
inline point2<T> operator- ( const point2<T>& a, const point2<T>& b )
{
  point2<T> c = {a.x-b.x, a.y-b.y};
  return c;
}
                               /* ---- multiply a scalar with a vector ---- */
template <class T>
inline point2<T> operator* ( const T op, const point2<T>& a  )
{
  point2<T> c = {op*a.x, op*a.y};
  return c;
}
                       /* --- calculate the dot product of two vectors ---- */
template <class T>
inline T operator* ( const point2<T>& a, const point2<T>& b )
{
  return a.x*b.x + a.y*b.y;
}

/*************************************************************************
  1-D points
*************************************************************************/
                                            /* -- set all components ------ */
template< class T >
inline void set( point1<T>& c, T a )
{
  c.x = a;
}
                         /* --- calculate the sum a + b  of vectors a,b --- */
template <class T>
inline point1<T> operator+ ( const point1<T>& a, const point1<T>& b )
{
  point1<T> c = {a.x+b.x};
  return c;
}
                                     /* --- invert the sign of a vector --- */
template <class T>
inline point1<T> operator- ( const point1<T>& a )
{
  point1<T> c = {-a.x};
  return c;
}
                  /* --- calculate the difference a - b  of vectors a,b --- */
template <class T>
inline point1<T> operator- ( const point1<T>& a, const point1<T>& b )
{
  point1<T> c = {a.x-b.x};
  return c;
}
                               /* ---- multiply a scalar with a vector ---- */
template <class T>
inline point1<T> operator* ( const T op, const point1<T>& a  )
{
  point1<T> c = {op*a.x};
  return c;
}
                       /* --- calculate the dot product of two vectors ---- */
template <class T>
inline T operator* ( const point1<T> a, const point1<T>& b )
{
  return a.x*b.x;
}


/***************************************************************************
 special functions
***************************************************************************/

template< class T >
inline point<T> ortho( const hpoint<T>& a )
{
  point<T> c = { a.x, a.y, a.z };
  return c;
} 
template< class T >
inline point2<T> ortho( const hpoint2<T>& a )
{
  point2<T> c = { a.x, a.y };
  return c;
} 
template< class T >
inline point1<T> ortho( const hpoint1<T>& a )
{
  point1<T> c = { a.x };
  return c;
} 

                     /* --- calculate the cross product of two vectors ---- */
template<class T> 
inline point<T> cross_product( const point<T>& a, const point<T>& b )
{
  point<T> c = { a.y*b.z-b.y*a.z,
                 b.x*a.z-a.x*b.z,
                 a.x*b.y-b.x*a.y };
  return c;
}
template<class T> 
inline point<T> cross_product( const point2<T>& a, const point2<T>& b )
{
  point<T> c = { (T)0.0,
                 (T)0.0,
                 a.x*b.y-b.x*a.y };
  return c;
}
                /*--- calculate the length (euclidian norm) of a vector ---  */ 
template< class T >
inline T length( const point<T>& a )
{
  return rtr<T>::sqrt(a.x*a.x + a.y*a.y + a.z*a.z);
}

                                 /* -- check, if vector is (almost) zero --- */
template< class T >
inline bool is_zero( const point<T>& a, const rtl<T>& t )
{
  T x,y,z;

  x = rtr<T>::fabs(a.x);
  if( x > t.zero_tol ) return false;
  y = rtr<T>::fabs(a.y);
  if( y > t.zero_tol ) return false;    
  z = rtr<T>::fabs(a.z);
  if( z > t.zero_tol ) return false;
    
  if( (x <= t.zero_tol_v3) && (y <= t.zero_tol_v3) && 
      (z <= t.zero_tol_v3) )
    return true;

  if( rtr<T>::sqrt(x*x + y*y + z*z) > t.zero_tol ) return false;

  return true;
}
template< class T >
inline bool is_zero( const point2<T>& a, const rtl<T>& t )
{
  T x,y;

  x = rtr<T>::fabs(a.x);
  if( x > t.zero_tol ) return false;
  y = rtr<T>::fabs(a.y);
  if( y > t.zero_tol ) return false;    
  
  if( (x <= t.zero_tol_v2) && (y <= t.zero_tol_v2) ) 
    return true;

  if( rtr<T>::sqrt(x*x + y*y) > t.zero_tol ) return false;

  return true;
}

     /* check, if the difference of two homogeneous vectors is (almost) zero */
template< class T >
inline bool dist_is_zero( const hpoint<T>& a, const hpoint<T>& b,
                          const rtl<T>& t )
{
  T x,y,z,w;
  
  x = rtr<T>::fabs(a.x-b.x);
  if( x > t.zero_tol ) return false;
  y = rtr<T>::fabs(a.y-b.y);
  if( y > t.zero_tol ) return false;    
  z = rtr<T>::fabs(a.z-b.z);
  if( z > t.zero_tol ) return false;
  w = rtr<T>::fabs(a.w-b.w);
  if( w > t.zero_tol ) return false;

  if( (x <= t.zero_tol_v4) && (y <= t.zero_tol_v4) &&
      (z <= t.zero_tol_v4) && (w <= t.zero_tol_v4) )
    return true;

  if( rtr<T>::sqrt( x*x + y*y + z*z + w*w ) > t.zero_tol ) return false;

  return true;      
}
                  /* check, if a vector has a greater length than tolerance */
template< class T >
inline bool longer_than( const point<T>& a, const T tolerance )
{
  T x,y,z,tol_v3;

  x = rtr<T>::fabs(a.x);
  if( x > tolerance ) return true;
  y = rtr<T>::fabs(a.y);
  if( y > tolerance ) return true;    
  z = rtr<T>::fabs(a.z);
  if( z > tolerance ) return true;

  tol_v3 = tolerance / rtr<T>::root2_3();

  if( (x <= tol_v3) && (y <= tol_v3) && (z <= tol_v3) ) return false;
  
  if( rtr<T>::sqrt(x*x + y*y + z*z) > tolerance ) return true;

  return false;  
}
template< class T >
inline bool longer_than( const point2<T>& a, const T tolerance )
{
  T x,y,tol_v2;

  x = rtr<T>::fabs(a.x);
  if( x > tolerance ) return true;
  y = rtr<T>::fabs(a.y);
  if( y > tolerance ) return true;    

  tol_v2 = tolerance / rtr<T>::root2_2();

  if( (x <= tol_v2) && (y <= tol_v2) ) return false;
  
  if( rtr<T>::sqrt(x*x + y*y) > tolerance ) return true;

  return false;  
}

   /* check, if the difference of two vectors has a greater length than tol. */
template< class T >
inline bool dist_longer_than( const point<T>& a, const point<T>& b, 
                              const T tolerance )
{
  T x,y,z,tol_v3;

  x = rtr<T>::fabs(a.x-b.x);
  if( x > tolerance ) return true;
  y = rtr<T>::fabs(a.y-b.y);
  if( y > tolerance ) return true;    
  z = rtr<T>::fabs(a.z-b.z);
  if( z > tolerance ) return true;

  tol_v3 = tolerance / rtr<T>::root2_3;

  if( (x <= tol_v3) && (y <= tol_v3) && (z <= tol_v3) ) return false;
  
  if( rtr<T>::sqrt(x*x + y*y + z*z) > tolerance ) return true;

  return false;  
}

                                /* normalize a vector (so that length = 1) */ 
template< class T >
inline T normalize( point<T> *c, const point<T>& a ) 
{
  T d;
  
  d = rtr<T>::sqrt( a.x*a.x + a.y*a.y + a.z*a.z );
    
  c->x = a.x / d;
  c->y = a.y / d;
  c->z = a.z / d;

  return d;
}    
template< class T >
inline T normalize( point2<T> *c, const point2<T>& a ) 
{
  T d;
  
  d = rtr<T>::sqrt( a.x*a.x + a.y*a.y );
    
  c->x = a.x / d;
  c->y = a.y / d;

  return d;
}    

                                   /* calculate the distance of two points */   
template< class T >
inline T distance( const point<T>& a, const point<T>& b )
{
  T x = a.x-b.x, y = a.y-b.y, z = a.z-b.z, d;
  
  d = rtr<T>::sqrt( x*x + y*y + z*z );
  return d;
}


}    /* namespace "gul" */



#endif
