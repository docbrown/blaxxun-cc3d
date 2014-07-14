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
 
#ifndef GUNU_DIVIDE_H
#define GUNU_DIVIDE_H

namespace gunu {

using gul::curve;
using gul::surface;

/*----------------------------------------------------------------------
  Splits a NURBS curve at point 'u' into two parts
----------------------------------------------------------------------- */ 
template< class T, class HP >
void SplitCurve( const curve<T,HP> *Curve, const T u,
                 curve<T,HP> *C1, curve<T,HP> *C2 );

/*----------------------------------------------------------------------
  Splits a NURBS surface at point (u,v) into four parts
----------------------------------------------------------------------- */ 
template< class T, class HP >
void SplitSurface( 
            const surface<T,HP> *Surf, const T u, const T v, 
            surface<T,HP> *S11, surface<T,HP> *S12, 
            surface<T,HP> *S21, surface<T,HP> *S22 );

/*----------------------------------------------------------------------
  Extracts a part of a NURBS curve (ranging from 'u1' to 'u2')
----------------------------------------------------------------------*/  
template< class T, class HP >
void ExtractCurve( 
         const T u1, const T u2,
         const int n, const int p, 
         Ptr< T > U, Ptr< HP > Pw,
         int *retn, Ptr< T > *retU, Ptr< HP > *retPw );

/*----------------------------------------------------------------------
  Extracts a part of a NURBS surface (ranging from 'u1' to 'u2' and 
  'v1' to 'v2')
----------------------------------------------------------------------*/  
template< class T, class HP >
void ExtractSurface( 
         const T u1, const T u2,
         const T v1, const T v2,
         const int nu, const int pu, Ptr< T > U,
         const int nv, const int pv, Ptr< T > V,
         Ptr< Ptr< HP > > Pw,
         int *ret_nu, Ptr< T > *retU, 
         int *ret_nv, Ptr< T > *retV, 
         Ptr< Ptr< HP > > *retPw );

}

#endif     
