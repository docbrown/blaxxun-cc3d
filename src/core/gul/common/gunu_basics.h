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
 
#ifndef GUNU_BASICS_H
#define GUNU_BASICS_H

namespace gunu {

using gul::Ptr;
using gul::point;
using gul::curve;

/*************************************************************************

  BASICAL NURBS FUNCTIONS

**************************************************************************/

/*-------------------------------------------------------------------------
  Search for the knotspan containing 'u' in a knot vector 'U' (clamped,degree
  'p','n'+1 knots (see "The NURBS book"))
-------------------------------------------------------------------------*/
template< class T >
inline int FindSpan( const T u, const int n, const int p, 
                     Ptr< T > U )
{
  int low,mid,high;
  
/* --- search knotspan (from right):   */

  if( u == U[n+1] )
  {
    if( u == U[n+p+1] ) // clamped
      return(n);
                      
    return( n+1 );   // unclamped, knot span = n+1
  }
     
  low = p;
  high = n+1;
  mid = (low+high) / 2 ;  

  while( (u < U[mid]) || (u >= U[mid+1]) ) 
  {
    if( u < U[mid] )
      high = mid;
    else
      low = mid;
      
    mid = (low+high) / 2;
  }
  
  return(mid);     
}                                   

/*-------------------------------------------------------------------------
  Finds the knotspan containing 'u' in a knot vector 'U' (clamped,degree
  'p','n'+1 knots (see "The NURBS book")), and counts the multiplicity
  of 'u'
-------------------------------------------------------------------------*/
template< class T >
inline int FindSpanMultip( const T u, const int n,  const int p, 
                           Ptr< T > U, int *s )
{
  int l,low,mid,high;
  
/* --- Knotenspanne suchen (von rechts):   */

  if( u == U[n+1] )
  {
    if( u == U[n+p+1] ) // clamped
    {
      *s = p + 1;
      return(n);
    }
                      
    l = n;           // if unclamped
    while( l >= 0 )  //   count multiplicity
    {
      if( U[l] != u ) break;
      l--;
    }
    *s = n+1 - l;
    return( n+1 );   // knot span = n+1
  }
     
  low = p;
  high = n+1;
  mid = (low+high) / 2 ;  

  while( (u < U[mid]) || (u >= U[mid+1]) ) 
  {
    if( u < U[mid] )
      high = mid;
    else
      low = mid;
      
    mid = (low+high) / 2;
  }
  
  l = mid;
  while( l >= 0 )
  {
    if( U[l] != u ) break;
    l--;
  }
  *s = mid - l;

  return(mid);     
}                                   

/*-------------------------------------------------------------------------
  Counts the multiplicity of a knot 'u'
-------------------------------------------------------------------------*/
template< class T >
inline int GetKnotMultiplicity( const T u, Ptr< T > U, const int span )
{
  int l;

  l = span;

  while( l >= 0 )
  {
    if( U[l] != u )
      break; 
    l--;
  }

  return(span-l);
}

/*---------------------------------------------------------------------
  Normalizes a clamped knot vector 'U', so that it starts with 0.0 and
  ends with 1.0
---------------------------------------------------------------------*/
template< class T >
inline void NormalizeKnotVector( const int n, const int p, Ptr< T >& U )
{
  T u0,su;
  int i;
  
  u0 = U[0];
  su = U[n+1] - u0;  
  if( (u0 == 0.) && (su == 1.) ) return;

  for( i = 0; i <= p; i++ )
    U[i] = 0.;
  for( i = p+1; i <= n; i++ )
    U[i] = (U[i] - u0) / su;
  for( i = n+1; i <= n+p+1; i++ )
    U[i] = 1.;
}     
#ifdef _MSC_VER
/*---------------------------------------------------------------------
  Normalizes a clamped knot vector 'U', so that it starts with 0.0 and
  ends with 1.0
---------------------------------------------------------------------*/
template
inline void NormalizeKnotVector( const int n, const int p, Ptr<float>& U );
template
inline void NormalizeKnotVector( const int n, const int p, Ptr<double>& U );
#endif

/*---------------------------------------------------------------------
  Normalizes a clamped knot vector 'U', so that it starts with 0.0 and
  ends with 1.0
---------------------------------------------------------------------*/
template< class T >
inline void NormalizeKnotVector( const int n, const int p, Ptr< T >& U,
                                 T& offsetU, T& scaleU, Ptr< T >& retU )
{
  T u0,su;
  int i;
  
  offsetU = u0 = U[0];
  scaleU = su = U[n+1] - u0;  
  if( (u0 == 0.) && (su == 1.) ) return;

  for( i = 0; i <= p; i++ )
    retU[i] = 0.;
  for( i = p+1; i <= n; i++ )
    retU[i] = (U[i] - u0) / su;
  for( i = n+1; i <= n+p+1; i++ )
    retU[i] = 1.;
}     
#ifdef _MSC_VER
/*---------------------------------------------------------------------
  Normalizes a clamped knot vector 'U', so that it starts with 0.0 and
  ends with 1.0
---------------------------------------------------------------------*/
template
inline void NormalizeKnotVector( const int n, const int p, Ptr<float>& U,
                           float& offsetU, float& scaleU, Ptr<float>& retU );
template
inline void NormalizeKnotVector( const int n, const int p, Ptr<double>& U,
                        double& offsetU, double& scaleU, Ptr<double>& retU );
#endif

/* --------------------------------------------------------------------
  Checks whether a knot vector is valid, clamped and normalized
---------------------------------------------------------------------*/
template< class T >
inline bool ValidateKnotVec( const int n, const int p, Ptr<T>& knt,
                      bool& is_clamped, bool& is_normalized )
{
  int m, multi, i;
  bool clamped0=false, clamped1=false;
  T u;

  m =  knt.nElems() - 1;
  if( m != n+p+1 ) return false;

  u = knt[0];
  multi = 1;
  for( i = 1; i <= m; i++ )
  {
    if( knt[i] < knt[i-1] ) 
      return false;    // knots must be increasing

    if( knt[i] == u )
      multi++;
    else
    {
      u = knt[i];
      multi = 1;
    }

    if( multi > p )
    {
      if( multi == p+1 )
      {
        if( i == p )
        {
          clamped0 = true;
          continue;
        }
        if( i == m )
        {
          clamped1 = true;
          continue;
        }
      }
      return false;
    }
  }

  is_clamped = clamped0 && clamped1; 
  is_normalized = ((knt[p] == (T)0.0) && (knt[n+1] == (T)1.0));

  return true;
}
#ifdef _MSC_VER
/* --------------------------------------------------------------------
  Checks whether a knot vector is valid, clamped and normalized
---------------------------------------------------------------------*/
template
inline bool ValidateKnotVec( const int n, const int p, Ptr<float>& knt,
                             bool& is_clamped, bool& is_normalized );
template
inline bool ValidateKnotVec( const int n, const int p, Ptr<double>& knt,
                             bool& is_clamped, bool& is_normalized );
#endif

/*--------------------------------------------------------------------------
  Constructs a uniform clamped knot vector ( u_{i+1}-u_{i}) is equal
  for all inner nodes
--------------------------------------------------------------------------*/ 
template< class T >
void UniformKnotVector( const int n, const int p, Ptr< T > U );
                               

/*------------------------------------------------------------------
  Calculates the non vanishing NURBS basis functions (see "The NURBS book")
-------------------------------------------------------------------*/
template< class T >
void CalcBasisFunctions( const T u, const int i,  const int p, Ptr< T > U,
                         Ptr< T > N )
{
  Ptr< T > left,right;
  T saved,temp;
  int j,r;
  
  left.reserve_place( reserve_stack(T,p+1), p+1 );
  right.reserve_place( reserve_stack(T,p+1), p+1 );
  
  N[0] = 1.0;
  
  for( j = 1; j <= p; j++ )
  {
    left[j] = u - U[i+1-j];
    right[j] = U[i+j] - u;
    saved = 0.0;
    
    for( r = 0; r < j; r++ )
    {
      temp = N[r] / (right[r+1] + left[j-r]);
      N[r] = saved + (right[r+1] * temp);
      saved = left[j-r] * temp;
    }
    
    N[j] = saved;
  }  
}

/*------------------------------------------------------------------------
  Calculates a point on a NURBS curve (see "The NURBS Book")
----------------------------------------------------------------------- */
template< class T, class HP, class EP >
void CurvePoint( const T u, const int n, const int p, Ptr< T > U,
                 Ptr< HP > Pw, EP *C );


/*------------------------------------------------------------------------
  Calculates a point on a NURBS surface (see "The NURBS Book")
----------------------------------------------------------------------- */
template< class T, class HP, class EP > 
EXP
void SurfacePoint( 
                const T u, const T v, 
                const int nu, const int pu, Ptr< T > U,
                const int nv, const int pv, Ptr< T > V,                    
                Ptr< Ptr< HP > > Pw, EP *retS );

/*------------------------------------------------------------------------
  Calculates a point on a NURBS surface (as a point with homogeneous coordinates
----------------------------------------------------------------------- */
template< class T, class EP >
void BSPSurfacePoint( 
                     const T u, const T v, 
                     const int nu, const int pu, Ptr< T > U,
                     const int nv, const int pv, Ptr< T > V,                    
                     Ptr< Ptr< EP > > Pw, EP *retS );

/*------------------------------------------------------------------------
  Calculates a point on a NURBS curve (the curve is passed as a structure)
----------------------------------------------------------------------- */
template< class T, class HP >
void EvaluateCurve( const curve< T, HP > *Curv, 
                    const T u, point<T> *C );



}

#endif
