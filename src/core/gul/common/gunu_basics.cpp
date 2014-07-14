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
 
#include "stdafx.h"

#include "gul_types.h"
#include "gul_vector.h"
#include "gunu_basics.h"


namespace gunu {

using gul::point1;
using gul::point2;
using gul::hpoint;
using gul::hpoint1;
using gul::hpoint2;
using gul::set;
using gul::euclid;

/*************************************************************************

  BASICAL NURBS FUNCTIONS

**************************************************************************/

/*--------------------------------------------------------------------------
  Constructs a uniform clamped knot vector ( u_{i+1}-u_{i}) is equal
  for all inner nodes
--------------------------------------------------------------------------*/ 
template< class T >
void UniformKnotVector( const int n, const int p, Ptr< T > U )
{
  int segs,i;
  T seglen;
  
  for( i = 0; i <= p; i++ )
  {
    U[i] = 0.;
    U[n+1+i] = 1.;
  }

  segs = n-p+1;
  seglen = (T)1. / ((T)segs);
  for( i = p+1; i <= n; i++ )
    U[i] = U[i-1] + seglen;
}
/*--------------------------------------------------------------------------
  Constructs a uniform clamped knot vector ( u_{i+1}-u_{i}) is equal
  for all inner nodes
--------------------------------------------------------------------------*/ 
template void UniformKnotVector( const int n, const int p, Ptr< float > U );
template void UniformKnotVector( const int n, const int p, Ptr< double > U );

/*------------------------------------------------------------------------
  Calculates a point on a NURBS curve (see "The NURBS Book")
----------------------------------------------------------------------- */
template< class T, class HP, class EP >
void CurvePoint( const T u, const int n, const int p, Ptr< T > U,
                 Ptr< HP > Pw, EP *C )
{
  Ptr< T > N;
  HP v1,h;
  int span,i;

  N.reserve_place( reserve_stack(T,p+1), p+1 );
  
  span = FindSpan( u, n, p, U );
  CalcBasisFunctions<T>( u, span, p, U, N );

  set( h, (T)0.0 );
  
  for( i = 0; i <= p; i++ )
  {
    v1 = N[i] * Pw[span-p+i];
    h = h + v1;
  } 
  *C = euclid( h );
}
/*------------------------------------------------------------------------
  Calculates a point on a NURBS curve (see "The NURBS Book")
----------------------------------------------------------------------- */
template void CurvePoint( 
          const float u, const int n, const int p, Ptr< float > U,
          Ptr< hpoint<float> > Pw, point<float> *C );
template void CurvePoint( 
          const double u, const int n, const int p, Ptr< double > U,
          Ptr< hpoint<double> > Pw, point<double> *C );
         
template void CurvePoint( 
          const float u, const int n, const int p, Ptr< float > U,
          Ptr< hpoint2<float> > Pw, point2<float> *C );
template void CurvePoint( 
          const double u, const int n, const int p, Ptr< double > U,
          Ptr< hpoint2<double> > Pw, point2<double> *C );

/*------------------------------------------------------------------------
  Calculates a point on a NURBS surface (see "The NURBS Book")
----------------------------------------------------------------------- */
template< class T, class HP, class EP > 
EXP
void SurfacePoint( 
                const T u, const T v, 
                const int nu, const int pu, Ptr< T > U,
                const int nv, const int pv, Ptr< T > V,                    
                Ptr< Ptr< HP > > Pw, EP *retS )
{                          
  Ptr< T > Nu,Nv;
  int uspan,vspan,uind,vind,l,k;
  HP S,temp,v1;

  Nu.reserve_place( reserve_stack(T,pu+1), pu+1 );
  Nv.reserve_place( reserve_stack(T,pv+1), pv+1 );
 
  uspan = FindSpan( u, nu, pu, U );
  CalcBasisFunctions( u, uspan, pu, U, Nu );
  
  vspan = FindSpan( v, nv, pv, V );
  CalcBasisFunctions( v, vspan, pv, V, Nv );

  uind = uspan-pu;
  set( S, (T)0.0 );
  for( l = 0; l <= pv; l++ )
  {
    set( temp, (T)0.0 );
    vind = vspan - pv + l;
    for( k = 0; k <= pu; k++ )
    {
      v1 = Nu[k] * Pw[vind][uind+k]; /* meine Kontrollpunktmatrix ist falschrum,
                                        muss daher transponiert werden */
      temp = temp + v1;
    }
    v1 = Nv[l] * temp;
    S = S + v1;
  }   

  *retS = euclid( S );
}          
/*------------------------------------------------------------------------
  Calculates a point on a NURBS surface (see "The NURBS Book")
----------------------------------------------------------------------- */
template EXP void SurfacePoint( 
                const float u, const float v, 
                const int nu, const int pu, Ptr< float > U,
                const int nv, const int pv, Ptr< float > V,                    
                Ptr< Ptr< hpoint<float> > > Pw, point<float> *retS );
template EXP void SurfacePoint( 
                const double u, const double v, 
                const int nu, const int pu, Ptr< double > U,
                const int nv, const int pv, Ptr< double > V,                    
                Ptr< Ptr< hpoint<double> > > Pw, point<double> *retS );

template EXP void SurfacePoint( 
                const float u, const float v, 
                const int nu, const int pu, Ptr< float > U,
                const int nv, const int pv, Ptr< float > V,                    
                Ptr< Ptr< hpoint1<float> > > Pw, point1<float> *retS );
template EXP void SurfacePoint( 
                const double u, const double v, 
                const int nu, const int pu, Ptr< double > U,
                const int nv, const int pv, Ptr< double > V,                    
                Ptr< Ptr< hpoint1<double> > > Pw, point1<double> *retS );

/*------------------------------------------------------------------------
  Calculates a point on a NURBS surface (as a point with homogeneous coordinates
----------------------------------------------------------------------- */
template< class T, class EP >
void BSPSurfacePoint( 
                     const T u, const T v, 
                     const int nu, const int pu, Ptr< T > U,
                     const int nv, const int pv, Ptr< T > V,                    
                     Ptr< Ptr< EP > > Pw, EP *retS )
{                          
  Ptr< T > Nu,Nv;
  int uspan,vspan,uind,vind,l,k;
  EP S,temp,v1;

  Nu.reserve_place( reserve_stack(T,pu+1), pu+1 );
  Nv.reserve_place( reserve_stack(T,pv+1), pv+1 );
  
  uspan = FindSpan( u, nu, pu, U );
  CalcBasisFunctions( u, uspan, pu, U, Nu );
  
  vspan = FindSpan( v, nv, pv, V );
  CalcBasisFunctions( v, vspan, pv, V, Nv );

  uind = uspan-pu;
  set( S, (T)0.0 );
  for( l = 0; l <= pv; l++ )
  {
    set( temp, (T)0.0 );
    vind = vspan - pv + l;
    for( k = 0; k <= pu; k++ )
    {
      v1 = Nu[k] * Pw[vind][uind+k]; /* meine Kontrollpunktmatrix ist falschrum,
                                        muss daher transponiert werden */
      temp = temp + v1;
    }
    v1 = Nv[l] * temp;
    S = S + v1;
  }   

  *retS = S;
}          
/*------------------------------------------------------------------------
  Calculates a point on a NURBS surface (as a point with homogeneous coordinates
----------------------------------------------------------------------- */
template void BSPSurfacePoint( 
                     const float u, const float v, 
                     const int nu, const int pu, Ptr< float > U,
                     const int nv, const int pv, Ptr< float > V,                    
                     Ptr< Ptr< point<float> > > Pw, point<float> *retS );
template void BSPSurfacePoint( 
                     const double u, const double v, 
                     const int nu, const int pu, Ptr< double > U,
                     const int nv, const int pv, Ptr< double > V,                    
                     Ptr< Ptr< point<double> > > Pw, point<double> *retS );

template void BSPSurfacePoint( 
                     const float u, const float v, 
                     const int nu, const int pu, Ptr< float > U,
                     const int nv, const int pv, Ptr< float > V,                    
                     Ptr< Ptr< point1<float> > > Pw, point1<float> *retS );
template void BSPSurfacePoint( 
                     const double u, const double v, 
                     const int nu, const int pu, Ptr< double > U,
                     const int nv, const int pv, Ptr< double > V,                    
                     Ptr< Ptr< point1<double> > > Pw, point1<double> *retS );

template void BSPSurfacePoint( 
                     const float u, const float v, 
                     const int nu, const int pu, Ptr< float > U,
                     const int nv, const int pv, Ptr< float > V,                    
                     Ptr< Ptr< hpoint<float> > > Pw, hpoint<float> *retS );
template void BSPSurfacePoint( 
                     const double u, const double v, 
                     const int nu, const int pu, Ptr< double > U,
                     const int nv, const int pv, Ptr< double > V,                    
                     Ptr< Ptr< hpoint<double> > > Pw, hpoint<double> *retS );

/*------------------------------------------------------------------------
  Calculates a point on a NURBS curve (the curve is passed as a structure)
----------------------------------------------------------------------- */
template< class T, class HP >
void EvaluateCurve( const curve< T, HP > *Curv, 
                    const T u, point<T> *C )
{
  CurvePoint( u, Curv->cpt.n, Curv->p, Curv->knt.U, Curv->cpt.Pw, C );
}
/*------------------------------------------------------------------------
  Calculates a point on a NURBS curve (the curve is passed as a structure)
----------------------------------------------------------------------- */
template 
void EvaluateCurve( const curve< float, hpoint<float> > *Curv, 
                    const float u, point<float> *C );
template 
void EvaluateCurve( const curve< double, hpoint<double> > *Curv,
                    const double u, point<double> *C );

}




