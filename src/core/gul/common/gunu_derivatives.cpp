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

// #if !defined(_MSC_VER) || defined(NEED_NEW_H)
//  #include <new.h>
// #endif

#include "gul_types.h"
#include "gul_vector.h"
#include "guar_bincoeff.h"
#include "gunu_basics.h"
#include "gunu_derivatives.h"


namespace gunu {

using gul::rtr;
using gul::point;
using gul::point1;
using gul::point2;
using gul::hpoint;
using gul::hpoint1;
using gul::hpoint2;
using gul::set;
using gul::euclid;
using gul::ortho;

/*-------------------------------------------------------------------------
  Calculates the first 'd' derivatives of a curve at a point 'u'. The results
  are homogeneous points (like the control points), returned in 'CK[]'
  (see "The NURBS Book")
--------------------------------------------------------------------------- */  
template< class T, class EP >
void BSPCurveDerivatives( 
           const T u, const int n, const int p, Ptr< T > U,
           Ptr< EP > Pw, const int d, Ptr< EP > CK )
{
  int du,k,j,span;
  EP v1;
  Ptr< Ptr< T > > ders;
    
  du = Min( d, p );

  ders.reserve_place( reserve_stack(Ptr< T >,du+1), du+1 );
  for( j = 0; j < p+1; j++ )
    ders[j].reserve_place( reserve_stack(T,p+1), p+1 );

  for( k = p+1; k <= d; k++ )
    set( CK[k], (T)0.0 );   /* Ableitungen > Grad = 0 */

  span = FindSpan( u, n, p, U );
  CalcDersBasisFuns( u, span, p, du, U, ders );
  
  for( k = 0; k <= du; k++ )
  {
    set( CK[k], (T)0.0 );
    for( j = 0; j <= p; j++ )
    {
      v1 = ders[k][j] * Pw[span-p+j];
      CK[k] = CK[k] + v1;
    }
  }
}
/*-------------------------------------------------------------------------
  Calculates the first 'd' derivatives of a curve at a point 'u'. The results
  are homogeneous points (like the control points), returned in 'CK[]'
  (see "The NURBS Book")
--------------------------------------------------------------------------- */  
template
void BSPCurveDerivatives( 
           const float u, const int n, const int p, Ptr< float > U,
           Ptr< point<float> > Pw, const int d, Ptr< point<float> > CK );
template
void BSPCurveDerivatives( 
           const double u, const int n, const int p, Ptr< double > U,
           Ptr< point<double> > Pw, const int d, Ptr< point<double> > CK );
template
void BSPCurveDerivatives( 
           const float u, const int n, const int p, Ptr< float > U,
           Ptr< point2<float> > Pw, const int d, Ptr< point2<float> > CK );
template
void BSPCurveDerivatives( 
           const double u, const int n, const int p, Ptr< double > U,
           Ptr< point2<double> > Pw, const int d, Ptr< point2<double> > CK );

/*---------------------------------------------------------------------------
  Calculates mixed partial derivatives of a NURBS surface. Let 'k' and 'l' be
  the number of derivatives in 'u' and 'v' direction, then all mixed ders with
  'k+l <= d' are calculated. They are returned in 'SKL[k][l]'.  
  (see "The NURBS Book")
------------------------------------------------------------------------- */  
template< class T, class EP >
void BSPSurfaceDerivatives(
                const T u, const T v,
                const int nu, const int pu, Ptr< T > U,
                const int nv, const int pv, Ptr< T > V,
                Ptr< Ptr< EP > > Pw,
                const int d, Ptr< Ptr< EP > > SKL )
{                
  int du,dv,k,l,uspan,vspan,s,r,dd;
  EP v1;
  Ptr< EP > temp;
  Ptr< Ptr< T > > Nu,Nv;

  du = Min( d, pu );
  dv = Min( d, pv );

  Nu.reserve_place( reserve_stack(Ptr< T >,du+1), du+1 );
  for( k = 0; k < pu+1; k++ )
    Nu[k].reserve_place( reserve_stack(T,pu+1), pu+1 );

  Nv.reserve_place( reserve_stack(Ptr< T >,dv+1), dv+1 );
  for( k = 0; k < pv+1; k++ )
    Nu[k].reserve_place( reserve_stack(T,pv+1), pv+1 );

  temp.reserve_place( reserve_stack(EP,pv+1), pv+1 );

  for( k = pu+1; k <= d; k++ )
    for( l = 0; l <= d - k; l++ )
      set( SKL[k][l], (T)0.0 );
      
  for( l = pv+1; l <= d; l++ )
    for( k = 0; k <= d - l; k++ )
      set( SKL[k][l], (T)0.0 );
      
  uspan = FindSpan( u, nu, pu, U );
  CalcDersBasisFuns( u, uspan, pu, du, U, Nu );
 
  vspan = FindSpan( v, nv, pv, V );
  CalcDersBasisFuns( v, vspan, pv, dv, V, Nv );
  
  for( k = 0; k <= du; k++ )
  {
    for( s = 0; s <= pv; s++ )
    {
      set( temp[s], (T)0.0 );
      for( r = 0; r <= pu; r++ )
      {
        v1 = Nu[k][r] * Pw[vspan-pv+s][uspan-pu+r];
        temp[s] = temp[s] + v1;
      }  
    }
    dd = Min( d - k, dv );
    for( l = 0; l <= dd; l++ )
    {
      set( SKL[k][l], (T)0.0 );
      for( s = 0; s <= pv; s++ )
      {
        v1 = Nv[l][s] * temp[s];
        SKL[k][l] = SKL[k][l] + v1;
      }
    }
  }
}  
/*---------------------------------------------------------------------------
  Calculates mixed partial derivatives of a NURBS surface. Let 'k' and 'l' be
  the number of derivatives in 'u' and 'v' direction, then all mixed ders with
  'k+l <= d' are calculated. They are returned in 'SKL[k][l]'.  
  (see "The NURBS Book")
------------------------------------------------------------------------- */
template
void BSPSurfaceDerivatives(
                const float u, const float v,
                const int nu, const int pu, Ptr< float > U,
                const int nv, const int pv, Ptr< float > V,
                Ptr< Ptr< point<float> > > Pw,
                const int d, Ptr< Ptr< point<float> > > SKL );           
template
void BSPSurfaceDerivatives(
                const double u, const double v,
                const int nu, const int pu, Ptr< double > U,
                const int nv, const int pv, Ptr< double > V,
                Ptr< Ptr< point<double> > > Pw,
                const int d, Ptr< Ptr< point<double> > > SKL );

template
void BSPSurfaceDerivatives(
                const float u, const float v,
                const int nu, const int pu, Ptr< float > U,
                const int nv, const int pv, Ptr< float > V,
                Ptr< Ptr< hpoint<float> > > Pw,
                const int d, Ptr< Ptr< hpoint<float> > > SKL );           
template
void BSPSurfaceDerivatives(
                const double u, const double v,
                const int nu, const int pu, Ptr< double > U,
                const int nv, const int pv, Ptr< double > V,
                Ptr< Ptr< hpoint<double> > > Pw,
                const int d, Ptr< Ptr< hpoint<double> > > SKL );

/*-------------------------------------------------------------------------
  Calculates the first 'd' partial derivatives of the projection of a NURBS
  curve into euclidian space (3-dimensions), so the calculated ders are points
  in 3-dimensional space   
  (see "The NURBS Book")
------------------------------------------------------------------------- */  
template< class T, class HP, class EP >
void CurveDerivatives(
                    const T u, const int n, const int p,
                    Ptr< T > U, Ptr< HP > Pw, const int d,
                    Ptr< EP > CK )             
{
  Ptr< HP > CKh;
  EP v,v1;
  T w0;
  int k,i;

  CKh.reserve_place( reserve_stack(HP,d+1), d+1 );
     
  BSPCurveDerivatives<T,HP>( u, n, p, U, Pw, d, CKh );

  w0 = CKh[0].w;
  
  for( k = 0; k <= d; k++ )
  {
    v = ortho( CKh[k] );
    
    for( i = 1; i <= k; i++ )
    {
      v1 = (guar::BinCoeff<T>(k,i) * CKh[i].w) * CK[k-i];
      v = v - v1;
    }
    CK[k] = ((T)1.0 / w0) * v;       
  }
}
/*-------------------------------------------------------------------------
  Calculates the first 'd' partial derivatives of the projection of a NURBS
  curve into euclidian space (3-dimensions), so the calculated ders are points
  in 3-dimensional space   
  (see "The NURBS Book")
------------------------------------------------------------------------- */  
template
void CurveDerivatives(
                    const float u, const int n, const int p,
                    Ptr< float > U, Ptr< hpoint<float> > Pw, const int d,
                    Ptr< point<float> > CK );
template
void CurveDerivatives(
                    const double u, const int n, const int p,
                    Ptr< double > U, Ptr< hpoint<double> > Pw, const int d,
                    Ptr< point<double> > CK );

template
void CurveDerivatives(
                    const float u, const int n, const int p,
                    Ptr< float > U, Ptr< hpoint2<float> > Pw, const int d,
                    Ptr< point2<float> > CK );
template
void CurveDerivatives(
                    const double u, const int n, const int p,
                    Ptr< double > U, Ptr< hpoint2<double> > Pw, const int d,
                    Ptr< point2<double> > CK );

/*-------------------------------------------------------------------------
  Calculates the first 'd' mixed partial derivatives of the projection of a
  NURBS surface into euclidian space (3-dimensions), so the calculated ders
  are points in 3-dimensional space. They are returned in 'SKL[k][l]'.  
  (see "The NURBS Book")
------------------------------------------------------------------------- */
template< class T, class HP, class EP >          
void SurfaceDerivatives(
                const T u, const T v,
                const int nu, const int pu, Ptr< T > U,
                const int nv, const int pv, Ptr< T > V,
                Ptr< Ptr < HP > > Pw,
                const int d, Ptr< Ptr < EP > > SKL )
{
  int i,j,k,l;
  EP v1,v2,vh;
  T w00;
  Ptr< Ptr < HP > > SKLh;
  
  SKLh.reserve_place( reserve_stack(Ptr < HP >,d+1), d+1 );
  for( i = 0; i < d+1; i++ )
    SKLh[i].reserve_place( reserve_stack(HP,d+1), d+1 );
    
  BSPSurfaceDerivatives<T,HP>( u, v, nu, pu, U, nv, pv, V, Pw, d, SKLh );

  w00 = SKLh[0][0].w;
  
  for( k = 0; k <= d; k++ )
  {
    for( l = 0; l <= d-k; l++ )
    {
      v1 = ortho( SKLh[k][l] );

      for( j = 1; j <= l; j++ )
      {
        vh = (guar::BinCoeff<T>(l,j) * SKLh[0][j].w) * SKL[k][l-j]; 
        v1 = v1 - vh;
      }  
      for( i = 1; i <= k; i++ )
      {
        vh = (guar::BinCoeff<T>(k,i) * SKLh[i][0].w) * SKL[k-i][l];
        v1 = v1 - vh;

        set( v2, (T)0.0 );
        for( j = 1; j <= l; j++ )
        {
          vh = (guar::BinCoeff<T>(l,j) * SKLh[i][j].w) * SKL[k-i][l-j];
          v2 = v2 + vh;
        }
        vh =  guar::BinCoeff<T>(k,i) * v2;
        v1 = v1 - vh;
      }
      SKL[k][l] = ((T)1.0 / w00) * v1;
    }    
  }
}
/*-------------------------------------------------------------------------
  Calculates the first 'd' mixed partial derivatives of the projection of a
  NURBS surface into euclidian space (3-dimensions), so the calculated ders
  are points in 3-dimensional space. They are returned in 'SKL[k][l]'.  
  (see "The NURBS Book")
------------------------------------------------------------------------- */
template
void SurfaceDerivatives(
                const float u, const float v,
                const int nu, const int pu, Ptr< float > U,
                const int nv, const int pv, Ptr< float > V,
                Ptr< Ptr < hpoint<float> > > Pw,
                const int d, Ptr< Ptr < point<float> > > SKL );
template
void SurfaceDerivatives(
                const double u, const double v,
                const int nu, const int pu, Ptr< double > U,
                const int nv, const int pv, Ptr< double > V,
                Ptr< Ptr < hpoint<double> > > Pw,
                const int d, Ptr< Ptr < point<double> > > SKL );

template
void SurfaceDerivatives(
                const float u, const float v,
                const int nu, const int pu, Ptr< float > U,
                const int nv, const int pv, Ptr< float > V,
                Ptr< Ptr < hpoint1<float> > > Pw,
                const int d, Ptr< Ptr < point1<float> > > SKL );
template
void SurfaceDerivatives(
                const double u, const double v,
                const int nu, const int pu, Ptr< double > U,
                const int nv, const int pv, Ptr< double > V,
                Ptr< Ptr < hpoint1<double> > > Pw,
                const int d, Ptr< Ptr < point1<double> > > SKL );


}













