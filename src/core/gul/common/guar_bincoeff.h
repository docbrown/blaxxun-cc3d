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
 
#ifndef GUAR_BINCOEFF_H
#define GUAR_BINCOEFF_H


namespace guar {

/* -------------------------------------------------------------------------
 Create table with binomial coefficients (using the triangle method of Pascal)
 B[0][0] = 1;
 B[p][k] = B[p-1][k-1] + B[p-1][k]
 These are getting large very fast, so 'p' shouldn't be to big
--------------------------------------------------------------------------- */
// template< class T > void InitBinCoeff( const int Pmax );

/*------------------------------------------------------------------------
  Delivers the binomial coefficient p over k
-------------------------------------------------------------------------*/  
template< class T > 
inline T BinCoeff( const int p, const int k )
{  
  if( (k > p) || (k < 0) || (p < 0) )
    return 0;
  
  if( p > gul::rtr<T>::BinCoeff_Pmax )
  {
    gul::rtr<T>::InitBinCoeff( p ); 
  }  

  return gul::rtr<T>::BinCoeff[p][k];
}

}

#endif
