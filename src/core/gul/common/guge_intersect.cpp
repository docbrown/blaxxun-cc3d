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
#include "guge_intersect.h"

namespace guge {

using gul::Max;
using gul::Min;
using gul::cross_product;

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
)
{  
  T A0,A1,A2,B0,B1,B2;
  T a0,a1,a2,b0,b1,b2;
  T N1,N2,N3;
  
/* bequemere Notation: */

  A0 = A.x;  A1 = A.y;  A2 = A.z;
  B0 = B.x;  B1 = B.y;  B2 = B.z;
  a0 = a.x;  a1 = a.y;  a2 = a.z;
  b0 = b.x;  b1 = b.y;  b2 = b.z;
  
  N1 = B1*b0 - b1*B0;
  N2 = B2*b0 - b2*B0; 
  N3 = B2*b1 - b2*B1;

  if( (gul::rtr<T>::fabs(N1) >= gul::rtr<T>::fabs(N2)) && 
      (gul::rtr<T>::fabs(N1) >= gul::rtr<T>::fabs(N3)) ) /* Berechnung ohne z */
  {
    *lambda = (b0*(a1-A1) - b1*(a0-A0)) / N1; 
    *mu = (B0*(a1-A1) - B1*(a0-A0)) / N1; 
  }
  else
  {
    if( gul::rtr<T>::fabs(N2) >= gul::rtr<T>::fabs(N3) )  /* Berechnung ohne y */
    {
      *lambda = (b0*(a2-A2) - b2*(a0-A0)) / N2;
      *mu = (B0*(a2-A2) - B2*(a0-A0)) / N2;
    }
    else
    {
      if( gul::rtr<T>::fabs(N3) != 0.0 )   /* Berechnung ohne x */
      {
        *lambda = (b1*(a2-A2) - b2*(a1-A1)) / N3;
        *mu = (B1*(a2-A2) - B2*(a1-A1)) / N3;
      }
      else
      {
        return false;  /* kein Schnittpunkt, oder Richtungsvektoren */ 
                       /* linear abhaengig                          */
      }      
    }
  }  
  return true;
}
/* ----------------------------------------------------------------------
  calculate the intersection point of two lines (in 3D). It must exist,
  or the result will be wrong.
----------------------------------------------------------------------- */
template
bool RegularIntersectLines(
  const point<float>& A,     /* point on line 1 */
  const point<float>& B,     /* direction vector of line 1 */
  const point<float>& a,     /* point on line 2 */
  const point<float>& b,     /* direction vector of line 2 */
  float *lambda,  /* parameter value of intersect. point (for line 1) */
  float *mu       /* parameter value of intersect. point (for line 2) */
);
template
bool RegularIntersectLines(
  const point<double>& A,     /* point on line 1 */
  const point<double>& B,     /* direction vector of line 1 */
  const point<double>& a,     /* point on line 2 */
  const point<double>& b,     /* direction vector of line 2 */
  double *lambda,  /* parameter value of intersect. point (for line 1) */
  double *mu       /* parameter value of intersect. point (for line 2) */
);

/* ---------------------------------------------------------------------
  calculate the intersection line segment of two triangles
----------------------------------------------------------------------- */  
template< class T >
bool IntersectTriangles( const triangle<T>& tri0, const triangle<T>& tri1,
                         point<T> *retP1, point<T> *retP2 )
{
  point<T> v11, v12, v13, v21, v22, v23, n;
  T A,B,C,a,b,c,R,r,N;
  point<T> g,t;
  point<T> P1,P2,P3;
  int init;
  T l1,l2,r1,r2,l,lambda,mu;
  bool result;

/* --- Ebenengleichung 1.Dreieck bestimmen ------------------------------ */
/* Ax + By + Cz = R */

  v11 = tri0.P2 - tri0.P1;   /* 1.Richtungsvektor */
  v12 = tri0.P3 - tri0.P1;   /* 2.Richtungsvektor */
  v13 = tri0.P3 - tri0.P2;   /* (fuer spaeter)    */
  n = cross_product( v11, v12 ); /* Normalenvektor Ebene */
  A = n.x;  B = n.y;  C = n.z;
  R = n * tri0.P1;           /* Aufpunkt einbauen */
  
/* --- Ebenengleichung 2.Dreieck bestimmen ------------------------------ */
/* ax + by + cz = r */
  v21 = tri1.P2 - tri1.P1;   /* 1.Richtungsvektor */
  v22 = tri1.P3 - tri1.P1;   /* 2.Richtungsvektor */
  v23 = tri1.P3 - tri1.P2;   /* (fuer spaeter)    */
  n = cross_product( v21, v22 ); /* Normalenvektor Ebene */
  a = n.x;  b = n.y;  c = n.z;
  r = n * tri1.P1;           /* Aufpunkt einbauen */
   
/* --- Schnittgerade G : x = g + lambda * t bestimmen ------------------ */

/* falls Schnittgerade nicht parallel zur X-Achse g.x = 0 und t.x = 1 setzen, */
/* ==> g.y,g.z und t.x,t.z                                                    */
   
  N = B*c - b*C;           /* Systemdeterminante */
  
  if( gul::rtr<T>::fabs(N) != 0.0 )     /* nicht parallel zur X-Achse */  
  {
    g.x = 0.0;
    g.y = (R*c - r*C)/N;
    g.z = (B*r - b*R)/N;
    t.x = 1.0;
    t.y = (C*a - c*A)/N;
    t.z = (A*b - a*B)/N;    
  }
  else
  {
    N = A*c - a*C;        
    
    if( gul::rtr<T>::fabs(N) != 0.0 )   /* nicht parallel zur y-Achse */
    {
      g.x = (R*c - r*C) / N;
      g.y = 0.0;
      g.z = (A*r - a*R) / N;
      t.x = (C*b - c*B) / N;
      t.y = 1.0;
      t.z = (B*a - b*A) / N;
    }
    else
    {
      N = A*b - a*B;
          
      if( gul::rtr<T>::fabs(N) != 0.0 )   /* nicht parallel zur z-Achse */
      {
        g.x = (R*b - r*B) / N;
        g.y = (A*r - a*R) / N;
        g.z = 0.0;
        t.x = (B*c - b*C) / N;
        t.y = (C*a - c*A) / N;
        t.z = 1.0;      
      }
      else        /* Normalenvektoren der beiden Ebenen linear abhaengig */
      {
        return false;
      }  
    }
  }

/* Schnittgerade mit dem Rand des ersten Dreiecks schneiden:          */
/* Ergebnis Intervall [l1,r1] im Parameterbereich  der Schnittgeraden */

/* bequeme Notation: */

  P1 = tri0.P1;
  P2 = tri0.P2;
  P3 = tri0.P3;

  init = 0;

/* Schnittgerade: X = g + mu * t                       */
/* Geradengleichung Seite P1P2:  X = P1 + lambda * v11 */

  result = RegularIntersectLines( P1, v11, g, t, &lambda, &mu );
  if( (result != 0) && (lambda >= 0.0) && (lambda <= 1.0) )
  {
    init = 1;
    l1 = mu;
    r1 = mu;
  }      
  
/* Geradengleichung Seite P1P3:  X = P1 + lambda * v12 */

  result = RegularIntersectLines( P1, v12, g, t, &lambda, &mu );
  if( (result != 0)  && (lambda >= 0.0) && (lambda <= 1.0) )
  {
    if( init != 0 )
    {
      if( mu < l1 ) l1 = mu;
      else if( mu > r1 ) r1 = mu;
      init += 1;
    }
    else
    {
      init = 1;
      l1 = mu;
      r1 = mu;
    }      
  } 

/* Geradengleichung Seite P2P3:  X = P2 + lambda * v13 */

  result = RegularIntersectLines( P2, v13, g, t, &lambda, &mu );
  if( (result != 0)  && (lambda >= 0.0) && (lambda <= 1.0) )
  {
    if( init != 0 )
    {
      if( mu < l1 ) l1 = mu;
      else if( mu > r1 ) r1 = mu;
      init += 1;
    }
    else
    {
      init = 1;
      l1 = mu;
      r1 = mu;
    }      
  } 

  if( init < 1 )
    return false;


/* Schnittgerade mit dem Rand des zweiten Dreiecks schneiden:        */
/* Ergebnis Intervall [l2,r2] im Parameterbereich der Schnittgeraden */

/* bequeme Notation: */

  P1 = tri1.P1;
  P2 = tri1.P2;
  P3 = tri1.P3;

  init = 0;

/* Schnittgerade: X = g + mu * t                       */
/* Geradengleichung Seite P1P2:  X = P1 + lambda * v21 */

  result = RegularIntersectLines( P1, v21, g, t, &lambda, &mu );
  if( (result != 0) && (lambda >= 0.0) && (lambda <= 1.0) )
  {
    init = 1;
    l2 = mu;
    r2 = mu;
  }      
  
/* Geradengleichung Seite P1P3:  X = P1 + lambda * v22 */

  result = RegularIntersectLines( P1, v22, g, t, &lambda, &mu );
  if( (result != 0)  && (lambda >= 0.0) && (lambda <= 1.0) )
  {
    if( init != 0 )
    {
      if( mu < l2 ) l2 = mu;
      else if( mu > r2 ) r2 = mu;
      init += 1;
    }
    else
    {
      init = 1;
      l2 = mu;
      r2 = mu;
    }      
  } 

/* Geradengleichung Seite P2P3:  X = P2 + lambda * v23 */

  result = RegularIntersectLines( P2, v23, g, t, &lambda, &mu );
  if( (result != 0)  && (lambda >= 0.0) && (lambda <= 1.0) )
  {
    if( init != 0 )
    {
      if( mu < l2 ) l2 = mu;
      else if( mu > r2 ) r2 = mu;
      init += 1;
    }
    else
    {
      init = 1;
      l2 = mu;
      r2 = mu;
    }      
  } 
  if( init < 1 )
    return false;

/* Durchschnitt der Parameterintervalle liefert Schnittsegment: */ 

  l = Max( l1, l2 );
  r = Min( r1, r2 );
  if( r < l )
    return(0);
  
/* Endpunkte bestimmen: */

  P3 = l * t;
  *retP1 = g + P3;
  P3 = r * t;
  *retP2 = g + P3;
  
/*
  printf( 
"Intersection-Segment: (%8.2f, %8.2f, %8.2f) -> (%8.2f, %8.2f, %8.2f)\n",
    P1.x, P1.y, P1.z, P2.x, P2.y, P2.z );
*/

  return true;
}
/* ---------------------------------------------------------------------
  calculate the intersection line segment of two triangles
----------------------------------------------------------------------- */  
template
bool IntersectTriangles( 
          const triangle<float>& tri0, const triangle<float>& tri1,
          point<float> *retP1, point<float> *retP2 );
template
bool IntersectTriangles( 
          const triangle<double>& tri0, const triangle<double>& tri1,
          point<double> *retP1, point<double> *retP2 );

}

