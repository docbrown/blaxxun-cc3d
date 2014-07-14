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
#include "gul_error.h"
#include "guar_exact.h"
#include "gugr_basics.h"



namespace gugr {


/* ----------------------------------------------------------------
  Calculates the line on which a graph edge lies
------------------------------------------------------------------ */

void graph_edge::CalcLine( void )
{
  PrecRational dx,dy,adx,ady;

  dx = v[1]->v.v().m_x - v[0]->v.v().m_x;
  dy = v[1]->v.v().m_y - v[0]->v.v().m_y;
  if( !dx.IsZero() ) ady = dy/dx; else ady = PrecRational(ULong(1));
  if( !dy.IsZero() ) adx = dx/dy; else adx = PrecRational(ULong(1));
  l = line( v[0]->v.v(),adx,ady);
}

/* ----------------------------------------------------------------
   gets the edges incident to vertex 'v'
------------------------------------------------------------------ */

int IncidentEdges( const graph_vertex *v, graph_edge **A )
{
  graph_edge *a0,*a;
  int i;
  
  A[0] = a0 = a = v->e;

  if( a->v[0] == v )
    a = a->e[0];
  else
    a = a->e[1];

  i = 1;
  while( a != a0 )
  {
    A[i] = a;
    i++;

    if( a->v[0] == v )
      a = a->e[0];
    else
      a = a->e[1];
  }

  return(i);
}

/* ----------------------------------------------------------------
   gets the edges incident to edge 'e' at vertex 'v'
------------------------------------------------------------------ */

int EdgeCycle( graph_edge *e, graph_vertex *v, graph_edge **A )
{
  graph_edge *a0,*a;
  int i;
  
  A[0] = a0 = a = e;

  if( a->v[0] == v )
    a = a->e[0];
  else
    a = a->e[1];

  i = 1;
  while( a != a0 )
  {
    A[i] = a;
    i++;

    if( a->v[0] == v )
      a = a->e[0];
    else
      a = a->e[1];
  }

  return(i);
}

/* --------------------------------------------------------------------
  Orient edges, so that y2 > y1 or x2 < x1 if y2 = y1
--------------------------------------------------------------------- */

void OrientEdges( graph_edge *E )
{
  graph_edge *e, *tmpe;
  graph_vertex *tmpv;
  int tmpf, sign;
  
  e = E;
  while( e != NULL )
  {
    sign = Compare( e->v[0]->v.v().m_y, e->v[1]->v.v().m_y );
    if( !sign ) sign = -Compare( e->v[0]->v.v().m_x, e->v[1]->v.v().m_x );

    if( sign > 0 )  /* v1 > v2 */
    {
      tmpv = e->v[0];
      e->v[0] = e->v[1];
      e->v[1] = tmpv;

      tmpe = e->e[0];
      e->e[0] = e->e[1];
      e->e[1] = tmpe;

      tmpf = e->f[0];
      e->f[0] = e->f[1];
      e->f[1] = tmpf;
    }

    e = e->next;
  }
}

/* ------------------------------------------------------------------------
  Determine on which side of AB the point C is
-------------------------------------------------------------------------- */

int DetermineOrientation( const PrecPoint2D& A, const PrecPoint2D& B, 
                          const PrecPoint2D& C )
{
  const Interval& ix1 = A.m_x.GetBounds();
  const Interval& iy1 = A.m_y.GetBounds();
  const Interval& ix2 = B.m_x.GetBounds();
  const Interval& iy2 = B.m_y.GetBounds();
  const Interval& ix = C.m_x.GetBounds();
  const Interval& iy = C.m_y.GetBounds();
  Interval i;

  i = (ix2-ix1)*(iy-iy1) - (ix-ix1)*(iy2-iy1);
  if( i.m_low > 0.0 ) return(+1);
  else if( i.m_high < 0.0 ) return(-1);

  const PrecRational& x1 = A.m_x;
  const PrecRational& y1 = A.m_y;
  const PrecRational& x2 = B.m_x;
  const PrecRational& y2 = B.m_y;
  const PrecRational& x = C.m_x;
  const PrecRational& y = C.m_y;
  PrecRational det;

  det = (x2-x1)*(y-y1) - (x-x1)*(y2-y1);
  return( det.Test() );
}

/* --------------------------------------------------------------------
  Compares angle(AB,AC) to angle(AB,AD)

  Remarks:

  code_x = 3 if X is to the left of AB
           2 if X is on AB
           1 if X is to the right of AB
           0 if it was not necessary to calculate the cross-product
  match = 1 if C lies on AB
        = 2 if D lies on AB
        = 0 if both C and D are not on AB             
---------------------------------------------------------------------- */

int CompareAngles(
  const PrecPoint2D& A, const PrecPoint2D& B,
  const PrecPoint2D& C, const PrecPoint2D& D,
  int *match, int *code_c, int *code_d )
{
  const Interval& iAx = A.m_x.GetBounds();
  const Interval& iAy = A.m_y.GetBounds();
  const Interval& iBx = B.m_x.GetBounds();
  const Interval& iBy = B.m_y.GetBounds();
  const Interval& iCx = C.m_x.GetBounds();
  const Interval& iCy = C.m_y.GetBounds();
  const Interval& iDx = D.m_x.GetBounds();
  const Interval& iDy = D.m_y.GetBounds();
  Interval iABx,iABy, iACx,iACy, iADx,iADy;
  Interval iABcAC,iABdAC; 
  Interval iABcAD,iABdAD;
  bool flag1 = false, flag2 = false;
  int sideC,sideD,sign;
  const PrecRational& rAx = A.m_x;
  const PrecRational& rAy = A.m_y;
  const PrecRational& rBx = B.m_x;
  const PrecRational& rBy = B.m_y;
  const PrecRational& rCx = C.m_x;
  const PrecRational& rCy = C.m_y;
  const PrecRational& rDx = D.m_x;
  const PrecRational& rDy = D.m_y;
  PrecRational rABx,rABy, rACx,rACy, rADx,rADy;
  PrecRational rABcAC, rABdAC;   
  PrecRational rABcAD, rABdAD;
  Interval iLambda;
  PrecRational rLambda;   

  *match = 0;
  *code_c = 0;
  *code_d = 0;
  
  iABx = iBx - iAx;
  iABy = iBy - iAy;
  iACx = iCx - iAx;
  iACy = iCy - iAy;
  
  iABcAC = iABx*iACy - iABy*iACx;
  
  if( iABcAC.m_low > 0.0 ) sideC = +1;
  else if( iABcAC.m_high < 0.0 ) sideC = -1;
  else
  {
    flag1 = true;
    rABx = rBx - rAx;
    rABy = rBy - rAy;
    rACx = rCx - rAx;
    rACy = rCy - rAy;

    rABcAC = rABx*rACy - rABy*rACx;
    sideC = rABcAC.Test();
  }

  *code_c = sideC + 2;
  
  if( sideC == 0 )    /* sideC == 0 => check orientation with dot product */
  {
    iABdAC = iABx*iACx + iABy*iACy;
    if( iABdAC.m_low > 0.0 )        /* 0=angle(AB,AC) < angle(AB,AD) */
    { *match = 1; return(-1); }   
    else if( iABdAC.m_high >= 0.0 )
    {    
      rABdAC = rABx*rACx + rABy*rACy;
      if( rABdAC.Test() > 0.0 )     /* 0=angle(AB,AC) < angle(AB,AD) */
      { *match = 1; return(-1); } 
    }
    /* now angle(AB,AC) == 180 if sideC == 0 */
  }
 
  iADx = iDx - iAx;
  iADy = iDy - iAy;

  iABcAD = iABx*iADy - iABy*iADx;
  
  if( iABcAD.m_low > 0.0 ) sideD = +1;
  else if( iABcAD.m_high < 0.0 ) sideD = -1;
  else
  {
    flag2 = true;
    rADx = rDx - rAx;
    rADy = rDy - rAy;
    if( !flag1 )
    {
      rABx = rBx - rAx;
      rABy = rBy - rAy;
    }
    rABcAD = rABx*rADy - rABy*rADx;
    sideD = rABcAD.Test();
  }

  *code_d = sideD + 2;
  
  if( sideD == 0 ) /* sideD == 0 => check orientation with dot product */
  {
    if( sideC == 0 ) { *match = 2; return(+1); }

    iABdAD = iABx*iADx + iABy*iADy;
    if( iABdAD.m_low > 0.0 )      /* 0=angle(AB,AD) < angle(AB,AC) */
    { *match = 2; return(+1); }   
    else if( iABdAD.m_high >= 0.0 ) /* exact calculation if necessary */
    {    
      rABdAD = rABx*rADx + rABy*rADy;
      if( rABdAD.Test() > 0.0 )     /* 0=angle(AB,AD) < angle(AB,AC) */
      { *match = 2; return(+1); } 
    }
    /* now angle(AB,AD) == 180 if sideD == 0 */

    if( sideC > 0 ) return(-1);
    return(+1);
  }

  if( sideC == 0 )
  {
    if( sideD > 0 ) return(+1);
    return(-1);
  }
  
  if( sideC > sideD ) return(-1);
  else if( sideC < sideD ) return(+1);

  /* intersect Line(C,AB) and Line(A,AD) */
  
  iLambda = (iADx*iACy - iACx*iADy)/(iABx*iADy - iABy*iADx);
  if( iLambda.m_low > 0.0 )
  {
     if( sideC > 0 ) return(+1);  /* angle(AB,AC) > angle(AB,AD) */
     return(-1);
  }
  if( iLambda.m_high < 0.0 )
  {
    if( sideC > 0 ) return(-1);  /* angle(AB,AC) < angle(AB,AD) */
    return(+1);
  }
  
  if( !flag1 )
  {
    rACx = rCx - rAx;
    rACy = rCy - rAy;
  }
  if( !flag2 )
  {
    if( !flag1 )
    {
      rABx = rBx - rAx;
      rABy = rBy - rAy;
    }
    rADx = rDx - rAx;
    rADy = rDy - rAy;
  }
  
  rLambda = (rADx*rACy - rACx*rADy)/(rABx*rADy - rABy*rADx);
  sign = rLambda.Test();

  if( sign > 0 )
  {
     if( sideC > 0 ) return(+1);  /* angle(AB,AC) > angle(AB,AD) */
     return(-1);
  }
  if( sideC > 0 ) return(-1);  /* angle(AB,AC) < angle(AB,AD) */
  return(+1);
}

/* --------------------------------------------------------------------
  Find the insertion positon for a oriented line segment in an edge 
  cycle of a DCEL 

  Remarks:

  code_edge = 1 if angle(ab,edge) > 180
            = 2 if angle(ab,edge) = 0 or 180
            = 3 if angle(ab,edge) < 180
  match = 1 if ab and left are colinear 
        = 2 if ab and right are colinear
        = 0 if none of both edges lies on ab         
---------------------------------------------------------------------*/

int EdgeInsertPosition(
         const vertex& a, const vertex& b,
         int nE, graph_edge **E,
         int *eleft, int *eright,
         int *code_left, int *code_right )
{
  vertex v1,v2,v3; 
  int left,right,mid,sign,s1,s2,s3,tmp;
  int match = 0;
  
  left = 0;
  right = nE-1;

  if( right > 0 )
  {
    if( E[left]->v[0]->v == a ) v1 =  E[left]->v[1]->v;
    else v1 = E[left]->v[0]->v;
    if( E[right]->v[0]->v == a ) v2 =  E[right]->v[1]->v;
    else v2 = E[right]->v[0]->v;

    sign = CompareAngles( a.v(), b.v(), v1.v(), v2.v(),
                          &match, &s1, &s2 );
    if( sign > 0 )
    {
      if( match )
      {
        if( right-1 != left ) s1 = 0;
        left = right-1;
      }
      else
      {
        mid = (left+right)/2;

        while( mid != left ) 
        {
          if( E[mid]->v[0]->v == a ) v3 =  E[mid]->v[1]->v;
          else v3 = E[mid]->v[0]->v;

          sign = CompareAngles( a.v(), b.v(), v1.v(), v3.v(), 
                                &match, &s1, &s3 );
          if( match )
          {
            right = mid;
            s2 = s3;
            if( mid-1 != left ) s1 = 0;
            left = mid-1;
            break;
          } 

          if( sign > 0 )
          {
            right = mid;
            s2 = s3;
          }
          else if( sign < 0 )
          {
            left = mid;
            v1 = v3;
            s1 = s3;
          }
          else 
            throw InternalError();   /* this cannot be */

          mid = (left+right)/2;
        }
      }
    }
    else
    {
      gul::Assert< gul::InternalError >( ndebug ||
                           ((sign != 0) && (left == 0) && (right = nE-1)) );
 
      tmp = left; left = right; right = tmp;
      tmp = s1; s1 = s2; s2 = tmp;
    }
  }
  else
  {
    right = left;
    s1 = s2 = 0;
  }

  if( s1 == 0 )
  {
    if( E[left]->v[0]->v == a ) v1 =  E[left]->v[1]->v;
    else v1 = E[left]->v[0]->v;

    s1 = DetermineOrientation( a.v(), b.v(), v1.v() ) + 2;
  }
  if( s2 == 0 )
  {
    if( right != left )
    {
      if( E[right]->v[0]->v == a ) v2 =  E[right]->v[1]->v;
      else v2 = E[right]->v[0]->v;

      s2 = DetermineOrientation( a.v(), b.v(), v2.v() ) + 2;
    }
    else
      s2 = s1;
  }

  *eleft = left;
  *eright = right; 
  *code_left = s1;
  *code_right = s2;
  return(match);
}

}
