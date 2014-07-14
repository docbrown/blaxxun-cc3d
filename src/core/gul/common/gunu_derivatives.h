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
 
#ifndef GUNU_DERIVATIVES_H
#define GUNU_DERIVATIVES_H

namespace gunu {

using gul::Ptr;
using gul::rtl;
using gul::point;
using gul::hpoint;
using gul::curve;
using gul::Min;

/*------------------------------------------------------------------------
  Calculate NURBS basis functions and their first 'n' derivatives.
  Only the non-vanishing basis functions are calculated (see "The NURBS Book")
------------------------------------------------------------------------*/  
template< class T >
inline void CalcDersBasisFuns( 
            const T u, const int i, const int p, const int n, Ptr< T > U, 
            Ptr< Ptr< T > > ders )
{
  T saved,temp,d;
  int j,k,r,s1,s2,rk,pk,j1,j2;

  Ptr< Ptr< T > > ndu,a;
  Ptr< T > left,right;
  
  ndu.reserve_place( reserve_stack(Ptr< T >,p+1), p+1 );
  for( j = 0; j < p+1; j++ )
    ndu[j].reserve_place( reserve_stack(T,p+1), p+1 );
 
  a.reserve_place( reserve_stack(Ptr< T >,2), 2 );
  for( j = 0; j < 2; j++ )
    a[j].reserve_place( reserve_stack(T,p+1), p+1 );

  left.reserve_place( reserve_stack(T,p+1), p+1 );
  right.reserve_place( reserve_stack(T,p+1), p+1 );
  
  ndu[0][0] = 1.0;
  
  for( j = 1; j <= p; j++ )
  {
    left[j] = u - U[i+1-j];
    right[j] = U[i+j]-u;
    saved = 0.0;
    for( r = 0; r < j; r++ )
    {
      ndu[j][r] = right[r+1] + left[j-r];
      temp = ndu[r][j-1]/ndu[j][r];
      
      ndu[r][j] = saved + right[r+1]*temp;
      saved = left[j-r]*temp;
    }
    ndu[j][j] = saved;
  }

  
  for( j = 0; j <= p; j++ )
  {
    ders[0][j] = ndu[j][p];        /* Basis Funktionen = 0.Ableitung */
  }

  for( r = 0; r <= p; r++ )
  {
    s1 = 0;    s2 = 1;
    a[0][0] = 1.0;

    for( k = 1; k <= n; k++ )
    {
      d = 0.0;
      rk = r-k;  pk = p-k;
      
      if( r >= k )
      {
        a[s2][0] = a[s1][0] / ndu[pk+1][rk];
        d = a[s2][0] * ndu[rk][pk];
      }  
      if( rk >= -1 )
        j1 = 1;
      else
        j1 = -rk;  
      if( r-1 <= pk )
        j2 = k-1;
      else
        j2 = p-r;  

      for( j = j1; j <= j2; j++ )
      {
        a[s2][j] = (a[s1][j] - a[s1][j-1]) / ndu[pk+1][rk+j];
        d += a[s2][j] * ndu[rk+j][pk];
      }  
      if( r <= pk )
      {
        a[s2][k] = -a[s1][k-1] / ndu[pk+1][r];
        d += a[s2][k] * ndu[r][pk];
      }  
      ders[k][r] = d;
      j = s1; s1 = s2; s2 = j;
    }        
  }
  
  r = p;
  for( k = 1; k <= n; k++ )
  {
    for( j = 0; j <= p; j++ )
      ders[k][j] *= (T)r;
    r *= p-k;
  }    
}


/*-------------------------------------------------------------------------
  Calculates the first 'd' derivatives of a curve at a point 'u'. The results
  are homogeneous points (like the control points), returned in 'CK[]'
  (see "The NURBS Book")
--------------------------------------------------------------------------- */  
template< class T, class EP >
void BSPCurveDerivatives( 
           const T u, const int n, const int p, Ptr< T > U,
           Ptr< EP > Pw, const int d, EP *CK );

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
                const int d, Ptr< Ptr< EP > > SKL );

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
                    EP *CK );

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
                const int d, Ptr< Ptr < EP > > SKL );

/*---------------------------------------------------------------------
  Calculates the normal vector of a NURBS surface at u=0,v=0
---------------------------------------------------------------------*/
template< class T, class HP, class EP >
void CalcNormal_U0V0( 
           const int nu, const int pu, 
           const int nv, const int pv,
           Ptr< Ptr< HP > >& Pw, EP *Normal, const rtl<T>& t )
{
  int i,j,k,m;
  EP v1,v2,a,b;

  set(a,(T)0);
  set(b,(T)0);

  m = Min(pu,pv);
  
  for( k=0; k<m; k++ )
  {
    i=0;
    j=k+1;
    
    while( i <= k )
    {
      v1 = euclid( Pw[i][j] );
      v2 = euclid( Pw[i][0] );
      a = v1 - v2;
      if( !is_zero(a,t) )
      {
        k = m;     /* Vektor in Tangetialebene gefunden, fertig */
        break;
      }
      i++;
      j--;  
    }
  }

  for( k=0; k<m; k++ )
  {
    i=0;
    j=k+1;
    
    while( i <= k )
    {
      v1 = euclid( Pw[j][i] );
      v2 = euclid( Pw[0][i] );
      b = v1 - v2;
      if( !is_zero(b,t) )
      {
        k = m;     /* Vektor in Tangetialebene gefunden, fertig */
        break;
      }
      i++;
      j--;  
    }
  }

  v1 = cross_product( a, b );
  normalize( Normal, v1 );
}


/*---------------------------------------------------------------------
  Calculates the normal vector of a NURBS surface at u=1,v=0
---------------------------------------------------------------------*/
template< class T, class HP, class EP >
void CalcNormal_U1V0( 
           const int nu, const int pu, 
           const int nv, const int pv,
           Ptr< Ptr< HP > >& Pw, EP *Normal, const rtl<T>& t  )
{
  int i,j,k,m;
  EP v1,v2,a,b;

  set(a,(T)0);
  set(b,(T)0);

  m = Min(pu,pv);
  
  for( k=0; k<m; k++ )
  {
    i=0;
    j=k+1;
    
    while( i <= k )
    {
      v1 = euclid( Pw[i][nu] );
      v2 = euclid( Pw[i][nu-j] );
      a =  v1 - v2;
      if( !is_zero(a,t) )
      {
        k = m;     /* Vektor in Tangetialebene gefunden, fertig */
        break;
      }
      i++;
      j--;  
    }
  }

  for( k=0; k<m; k++ )
  {
    i=0;
    j=k+1;
    
    while( i <= k )
    {
      v1 = euclid( Pw[j][nu-i] );
      v2 = euclid( Pw[0][nu-i] );
      b = v1 - v2;
      if( !is_zero(b,t) )
      {
        k = m;     /* Vektor in Tangetialebene gefunden, fertig */
        break;
      }
      i++;
      j--;  
    }
  }

  v1 = cross_product( a, b );
  normalize( Normal, v1 );
}


/*---------------------------------------------------------------------
  Calculates the normal vector of a NURBS surface at u=0,v=1
---------------------------------------------------------------------*/
template< class T, class HP, class EP >
void CalcNormal_U0V1( 
           const int nu, const int pu, 
           const int nv, const int pv,
           Ptr< Ptr< HP > >& Pw, EP *Normal, const rtl<T>& t  )
{
  int i,j,k,m;
  EP v1,v2,a,b;

  set(a,(T)0);
  set(b,(T)0);
  
  m = Min(pu,pv);
  
  for( k=0; k<m; k++ )
  {
    i=0;
    j=k+1;
    
    while( i <= k )
    {
      v1 = euclid( Pw[nv-i][j] );
      v2 = euclid( Pw[nv-i][0] );
      a = v1 - v2;
      if( !is_zero(a,t) )
      {
        k = m;     /* Vektor in Tangetialebene gefunden, fertig */
        break;
      }
      i++;
      j--;  
    }
  }

  for( k=0; k<m; k++ )
  {
    i=0;
    j=k+1;
    
    while( i <= k )
    {
      v1 = euclid( Pw[nv][i] );
      v2 = euclid( Pw[nv-j][i] );
      b =  v1 - v2;
      if( !is_zero(b,t) )
      {
        k = m;     /* Vektor in Tangetialebene gefunden, fertig */
        break;
      }
      i++;
      j--;  
    }
  }

  v1 = cross_product( a, b );
  normalize( Normal, v1 );
}


/*---------------------------------------------------------------------
  Calculates the normal vector of a NURBS surface at u=1,v=1
---------------------------------------------------------------------*/
template< class T, class HP, class EP >
inline void CalcNormal_U1V1( 
           const int nu, const int pu, 
           const int nv, const int pv,
           Ptr< Ptr< HP > >& Pw, EP *Normal, const rtl<T>& t  )
{
  int i,j,k,m;
  EP v1,v2,a,b;

  set(a,(T)0);
  set(b,(T)0);
  
  m = Min(pu,pv);
  
  for( k=0; k<m; k++ )
  {
    i=0;
    j=k+1;
    
    while( i <= k )
    {
      v1 = euclid( Pw[nv-i][nu] );
      v2 = euclid( Pw[nv-i][nu-j] );
      a = v1 - v2;
      if( !is_zero(a,t) )
      {
        k = m;     /* Vektor in Tangetialebene gefunden, fertig */
        break;
      }
      i++;
      j--;  
    }
  }

  for( k=0; k<m; k++ )
  {
    i=0;
    j=k+1;
    
    while( i <= k )
    {
      v1 = euclid( Pw[nv][nu-i] );
      v2 = euclid( Pw[nv-j][nu-i] );
      b = v1 - v2;
      if( !is_zero(b,t) )
      {
        k = m;     /* Vektor in Tangetialebene gefunden, fertig */
        break;
      }
      i++;
      j--;  
    }
  }

  v1 = cross_product( a, b );
  normalize( Normal, v1 );
}

}

#endif
