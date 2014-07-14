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

#ifndef GUL_MATRIX_H
#define GUL_MATRIX_H

namespace gul {

template< class T >
mat4x4<T> operator*( const mat4x4<T>& a, const mat4x4<T>& b )
{
  int i,j,k;
  mat4x4<T> c;
  T sum;
  
  for( i = 0; i < 4; i++)
  {
    for( j =0; j < 4; j++)
    {
      sum = (T)0.0;

      for( k = 0; k < 4; k++)
        sum += a[i][k]*b[k][j];

      c[i][j] = sum;
    }
  }    
  return c;
}

template< class T >
vec4<T> operator*( mat4x4<T>& a, vec4<T>& b )
{
  int i,k;
  vec4<T> c;
  T sum;
  
  for( i = 0; i < 4; i++)
  {
    sum = (T)0.0;

    for( k = 0; k < 4; k++)
      sum += a[i][k]*b[k];

    c[i] = sum;
  }
  return c;
}

template< class T >
vec4<T> operator*( vec4<T>& b, mat4x4<T>& a )
{
  int i,k;
  vec4<T> c;
  T sum;
  
  for( i = 0; i < 4; i++)
  {
    sum = (T)0.0;

    for( k = 0; k < 4; k++)
      sum += b[k] * a[k][i];

    c[i] = sum;
  }
  return c;
}

template< class T >
void CubeVerts( const T& xmin, const T& xmax,
                const T& ymin, const T& ymax,
                const T& zmin, const T& zmax,
                vec4<T> *v )
{
  v[0][0] = xmin;
  v[0][1] = ymin;
  v[0][2] = zmin;
  v[0][3] = (T)1.0;

  v[1][0] = xmax;
  v[1][1] = ymin;
  v[1][2] = zmin;
  v[1][3] = (T)1.0;

  v[2][0] = xmax;
  v[2][1] = ymax;
  v[2][2] = zmin;
  v[2][3] = (T)1.0;

  v[3][0] = xmin;
  v[3][1] = ymax;
  v[3][2] = zmin;
  v[3][3] = (T)1.0;


  v[4][0] = xmin;
  v[4][1] = ymin;
  v[4][2] = zmax;
  v[4][3] = (T)1.0;

  v[5][0] = xmax;
  v[5][1] = ymin;
  v[5][2] = zmax;
  v[5][3] = (T)1.0;

  v[6][0] = xmax;
  v[6][1] = ymax;
  v[6][2] = zmax;
  v[6][3] = (T)1.0;

  v[7][0] = xmin;
  v[7][1] = ymax;
  v[7][2] = zmax;
  v[7][3] = (T)1.0;
}







}

#endif
