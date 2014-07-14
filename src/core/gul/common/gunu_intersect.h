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
 
#ifndef GUNU_INTERSECT_H
#define GUNU_INTERSECT_H

namespace gunu {

using gul::Ptr;
using gul::rtr;
using gul::rtl;
using gul::point;
using gul::hpoint;
using gul::line2;
using gul::cross_product;
using gul::euclid;

/*-----------------------------------------------------------------------
  Estimate the (u,v) parameter values of a point P for a NURBS surface F,
  assuming that A = F(0,0), B = F(1,0), C = F(0,1)
------------------------------------------------------------------------*/
template< class T >
inline void BarycentricUV( 
                    const point<T> A, const point<T> B, const point<T> C,
                    const point<T> P, T *u, T *v )
{
  point<T> AB,AC,AP;
  T fABC, fACP, fABP;

  AB = B - A;
  AC = C - A;
  fABC = cross_product( AB, AC );    /* Flaecheninhalt Parallelogramm */
  AP = P - A;
  fACP = cross_product( AC, AP );    /* Flaecheninhalt Parallelogramm */
  fABP = cross_product( AB, AP );

  *v = fABP / fABC;
  *u = fACP / fABC;
}    

template< class T >
struct IntersectionLineInfo
{
  point<T> P1;
  point<T> P2;

  line2<T> F1;
  line2<T> F2;
};

template< class T >
struct SurfaceInfo
{
  int      nu;
  int      pu;
  Ptr< T > U; 
  int      nv;
  int      pv;
  Ptr< T > V;
  Ptr< Ptr< hpoint<T> > > Pw;
  void    *reserved[2];   /* for boundaries */

  point<T> P00;    /* Eckpunkte der Flaeche (um zu verhindern,  */
  point<T> P01;    /* das an T-Stellen der Tesselierung Loecher */
  point<T> P10;    /* entstehen)                                */
  point<T> P11;

  void *operator new( size_t s )
  {
    size_t dummy;

    void *p = gust::PoolAlloc( s, &dummy );
    if( p == NULL ) throw gul::PoolAllocError();
    return(p);
  }  
  void operator delete( void *p, size_t s )
  {
    gust::PoolFree( p, s );
  }
};

template< class T >
class TessInfo
{
public:
  T x1;   /* Bounding Box */
  T x2;
  T y1;
  T y2;
  T z1;
  T z2;

  T u1;   /* Parameterwerte der Flaeche (bezogen auf  */
  T u2;   /* die Originalflaeche)                     */
  T v1;
  T v2;
  
  struct
  {
    int linearized : 1;
    int divided    : 1;
  }f;  
  
  SurfaceInfo<T>         *org;    /* Originalzustand (ungeteilt); */
  TessInfo               *sub[4]; /* Flaeche in 4 Teilflaechen zerteilt */

  void operator delete( void *p, size_t s )

  {

    gust::PoolFree( p, s );

  }

  void *operator new( size_t s )
  {
    size_t dummy;
    void *p = gust::PoolAlloc( s, &dummy );
    if( p == NULL ) throw gul::PoolAllocError();
    return(p);
  }  

  TessInfo()
  {
    f.linearized = f.divided = 0;
    org = new SurfaceInfo<T>();
    for( int i = 0; i < 4; i++ ) sub[i] = 0;
  }
  ~TessInfo()
  {
    int i;

//    if( f.divided )
//    {
      for( i = 0; i < 4; i++ )
      {
        delete sub[i];
        sub[i] = 0;
      }
//    } 
//    else
//    {  
      delete org;   /* Speicher fuer ungeteilte Flaeche freigeben */
      org = 0;
//    }  
  }
};


/*--------------------------------------------------------------------
  Check if a NURBS surface can be approximated with two triangles through
  its four corners. If not the surface is subdivided into four parts
----------------------------------------------------------------------*/
template< class T >
inline void LinearizeOrDivide( TessInfo<T> *A, const T tol, 
                               const rtl<T> t, bool need_bbox = false );

/*-------------------------------------------------------------------
  Approximate NURBS-Surface with triangles
--------------------------------------------------------------------*/
template< class T >
void LinearizeSurface( 
                 int current_iter, int max_iter,
                 TessInfo<T> *A,
                 const T tol, const rtl<T>& t,
                 void    outfunc( TessInfo<T> *, void * ),                                   
                 void   *outfunc_data );

/*---------------------------------------------------------------------
  Intersect linear approximation of two NURBS-Surfaces
----------------------------------------------------------------------*/
template< class T >
void IntersectSurfaces( 
                 TessInfo<T> *A, TessInfo<T> *B,
                 T tol, const rtl<T>& t,
                 void    outfunc( IntersectionLineInfo<T> *, void * ),                                   
                 void   *outfunc_data );



}

#endif
