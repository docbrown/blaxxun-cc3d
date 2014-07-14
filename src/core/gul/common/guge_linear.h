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
 
#ifndef GUGE_LINEAR_H
#define GUGE_LINEAR_H

namespace guge {

using gul::Ptr;
using gul::rtl;
using gul::rtr;
using gul::hpoint;
using gul::point;
using gul::normalize;
using gul::cross_product;
using gul::longer_than;
using gul::euclid;
using gul::is_zero;


/*--------------------------------------------------------------------------
  check, if a curve can be approximated with a line through its endpoints
---------------------------------------------------------------------------*/
template< class T >
inline bool IsLinear( const int n, Ptr< point<T> > P, const T tol, const rtl<T> t )
{
  point<T> A,B,b,u0,L,X,H,R;
  int i;
  bool bzero;

  A = P[0];
  B = P[n];

  b =  B - A;
  bzero = is_zero( b, t );
  
  if( !bzero )
  {
    normalize( &u0, b );
    L = cross_product( u0, A );

    for( i = 1; i < n; i++ )
    {
      X = P[i];

      H = cross_product( u0, X );
      R = H - L;

      if( longer_than( R, tol ) )
        return false;
    }
  }
  else
  {
    for( i = 0; i <= n; i++ )
    {
      X = P[i];
      R = X - A;

      if( longer_than( R, tol ) )
        return false;
    }
  }
  return true;
}

/*--------------------------------------------------------------------------
  check, if a curve (given by its homogeneous control points), can be 
  approximated with a line through its endpoints
---------------------------------------------------------------------------*/
template< class T, class HP, class EP >
inline bool ProjectionIsLinear( const int n, Ptr< HP > Pw, 
                                const T tol, const rtl<T> t )
{
  EP A,B,b,u0,X;
  point<T> L,H,R;
  int i;
  bool bzero;

  A = euclid( Pw[0] );
  B = euclid( Pw[n] );

  b = B - A;
  bzero = is_zero( b, t );
  
  if( !bzero )
  {
    normalize( &u0, b );
    L = cross_product( u0, A );

    for( i = 1; i < n; i++ )
    {
      X = euclid( Pw[i] );

      H = cross_product( u0, X );
      R = H - L;

      if( longer_than( R, tol ) )
        return false;
    }
  }
  else
  {
    for( i = 0; i <= n; i++ )
    {
      X = euclid( Pw[i] );
      B = X - A;

      if( longer_than( B, tol ) )
        return false;
    }
  }
  return true;
}

/*-------------------------------------------------------------------------
  check, if a surface can be approximated with two triangles through its
  four corners
---------------------------------------------------------------------------*/
template< class T >
inline bool ProjectionIsPlanar( 
          const int nu, const int nv, Ptr< Ptr< hpoint<T> > > Pw,
          const T tol, const rtl<T> t )
{
  point<T> A,B,C,D,b,c,d,h,n0,u0,X,H,L,R;
  T r,l;
  bool is_point,is_plane, bzero,czero,dzero;
  int i,j;
  
  A = euclid( Pw[0][0] );
  B = euclid( Pw[nv][0] );
  C = euclid( Pw[nv][nu] );
  D = euclid( Pw[0][nu] );

  b = B - A;
  bzero = is_zero( b, t );
  c = C - A;
  czero = is_zero( c, t );
  d = D - A;
  dzero = is_zero( d, t );      

  is_point = false;
  is_plane = false;
  
  if( bzero && czero && dzero )               /* Punkt */
    is_point = true;
  else                                                             /* Ebene */
  {
    if( (!bzero) && (!czero) )
    {
      h = cross_product( b, c );
      is_plane = !is_zero( h, t );  /* b || c ??? */      
    }
       
    if( is_plane )
    {
      normalize( &n0, h );  
    }  
    else
    {  
      if( (!bzero) && (!dzero) )
      {
        h = cross_product( b, d );
        is_plane = !is_zero( h, t );  /* b || d ??? */ 
      }               

      if( is_plane )
      {
        normalize( &n0, h );
      }
      else
      {
        if( (!czero) && (!dzero) )
        {
          h = cross_product( c, d );
          is_plane = !is_zero( h, t );  /* c || d ??? */ 
        }               

        if( is_plane )
        {
          normalize( &n0, h );
        }
        else                                                     /* Gerade */ 
          if( !bzero )
            normalize( &u0, b );
          else
            if( !czero )
              normalize( &u0, c );
            else
              normalize( &u0, d );              
      }
    }            
  } 

  if( is_plane )
  {
    r = n0 * A;
    if( r < 0 )
    {
      r = -r;
      n0 = -n0;
    }  
    
    for( j = 0; j <= nv; j++ )
      for( i = 0; i <= nu; i++ )
      {
        X = euclid( Pw[j][i] );

        l = n0 * X;
        if( gul::rtr<T>::fabs( l - r ) > tol ) return false;
      }
  }                 
  else if( !is_point )
  {
    L = cross_product( u0, A );

    for( j = 0; j <= nv; j++ )
      for( i = 0; i <= nu; i++ )
      {
        X = euclid( Pw[j][i] );

        H = cross_product( u0, X );
        R = H - L;

        if( longer_than( R, tol ) )
          return false;
      }
  }
  else
  {
    for( j = 0; j <= nv; j++ )
      for( i = 0; i <= nu; i++ )
      {
        X = euclid( Pw[j][i] );
        R = X - A;

        if( longer_than( R, tol ) )
	        return false;
      }
  }
  return true;
}


}

#endif
