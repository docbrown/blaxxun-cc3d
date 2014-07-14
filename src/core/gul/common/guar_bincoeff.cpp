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
#include "guar_bincoeff.h"

float **gul::rtr<float>::BinCoeff = 0;
int gul::rtr<float>::BinCoeff_Pmax = -1;

double **gul::rtr<double>::BinCoeff = 0;
int gul::rtr<double>::BinCoeff_Pmax = -1;


namespace gul {

/* -------------------------------------------------------------------------
 Create table with binomial coefficients (using the triangle method of Pascal)
 B[0][0] = 1;
 B[p][k] = B[p-1][k-1] + B[p-1][k]
 These are getting large very fast, so 'p' shouldn't be to big
--------------------------------------------------------------------------- */

void rtr<float>::InitBinCoeff( const int Pmax ) 
{
  float s1,s2;
  int p,k;

  if( Pmax <= BinCoeff_Pmax )
    return;
  
  BinCoeff = (float **)realloc( BinCoeff, sizeof(float *) * (Pmax+1) );
  if( BinCoeff == NULL ) { throw AllocError(); }

  for( p = BinCoeff_Pmax+1; p <= Pmax; p++ ) 
  {
    BinCoeff[p] = (float *)malloc( sizeof(float) * (p+1) );
    if( BinCoeff[p] == 0 ) { throw AllocError(); }

    if( p == 0 )
    {
      BinCoeff[0][0] = 1;
      continue;
    }

    for( k = 0; k <= p; k++ )
    {
      if( k == 0 )
        s1 = 0;
      else
        s1 = BinCoeff[p-1][k-1];

      if( k > p-1 )
        s2 = 0;
      else
        s2 = BinCoeff[p-1][k];
    
      BinCoeff[p][k] = s1 + s2;  
    }      
  }
  BinCoeff_Pmax = Pmax;
}

/* -------------------------------------------------------------------------
 destroy table with binomial coefficients 
-------------------------------------------------------------------------- */

void rtr<float>::ExitBinCoeff()
{
  int i;

  if( BinCoeff == 0 )
    return;

  for( i = 0; i <= BinCoeff_Pmax; i++ )
    free( BinCoeff[i] ); 

  free( BinCoeff );

  BinCoeff = 0;
  BinCoeff_Pmax = -1;  
}

/************************************************************************
  Same again for "double", because i wasn't able to formulate this as
  a template with Visual C++ :(
************************************************************************/

/* -------------------------------------------------------------------------
 destroy table with binomial coefficients 
-------------------------------------------------------------------------- */

void rtr<double>::ExitBinCoeff()
{
  int i;

  if( BinCoeff == 0 )
    return;

  for( i = 0; i <= BinCoeff_Pmax; i++ )
    free( BinCoeff[i] ); 

  free( BinCoeff );

  BinCoeff = 0;
  BinCoeff_Pmax = -1;  
}

/* -------------------------------------------------------------------------
 Create table with binomial coefficients (using the triangle method of Pascal)
 B[0][0] = 1;
 B[p][k] = B[p-1][k-1] + B[p-1][k]
 These are getting large very fast, so 'p' shouldn't be to big
--------------------------------------------------------------------------- */
void rtr<double>::InitBinCoeff( const int Pmax ) 
{
  double s1,s2;
  int p,k;

  if( Pmax <= BinCoeff_Pmax )
    return;
  
  BinCoeff = (double **)realloc( BinCoeff, sizeof(double *) * (Pmax+1) );
  if( BinCoeff == NULL ) { throw AllocError(); }

  for( p = BinCoeff_Pmax+1; p <= Pmax; p++ ) 
  {
    BinCoeff[p] = (double *)malloc( sizeof(double) * (p+1) );
    if( BinCoeff[p] == 0 ) { throw AllocError(); }

    if( p == 0 )
    {
      BinCoeff[0][0] = 1;
      continue;
    }

    for( k = 0; k <= p; k++ )
    {
      if( k == 0 )
        s1 = 0;
      else
        s1 = BinCoeff[p-1][k-1];

      if( k > p-1 )
        s2 = 0;
      else
        s2 = BinCoeff[p-1][k];
    
      BinCoeff[p][k] = s1 + s2;  
    }      
  }
  BinCoeff_Pmax = Pmax;
}


}

