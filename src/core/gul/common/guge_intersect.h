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
 
#ifndef GUGE_INTERSECT_H
#define GUGE_INTERSECT_H

namespace guge
{

using gul::point;
using gul::triangle;

/* ----------------------------------------------------------------------
  calculate the intersection point of two lines (in 3D). It must exist,
  or the result will be wrong.
----------------------------------------------------------------------- */  
template< class T >
bool RegularIntersectLines(
  const point<T>& A,     /* point on line 1 */
  const point<T>& B,     /* direction vector of line 1 */
  const point<T>& a,     /* point on line 2 */
  const point<T>& b,     /* direction vector of line 2 */
  T *lambda,  /* parameter value of intersect. point (for line 1) */
  T *mu       /* parameter value of intersect. point (for line 2) */
);

/* ---------------------------------------------------------------------
  calculate the intersection line segment of two triangles
----------------------------------------------------------------------- */  
template< class T >
bool IntersectTriangles( const triangle<T>& tri0, const triangle<T>& tri1,
                         point<T> *retP1, point<T> *retP2 );

}

#endif
