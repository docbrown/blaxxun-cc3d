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
#include "gul_vector.h"
#include "gust_pool.h"
#include "guge_normalize.h"
#include "guar_exact.h"
#include "gugr_basics.h"
#include "gugr_io.h"
#include "gugr_regularize.h"
#include "gugr_split.h"
#include "gugr_triangulate.h"

namespace gugr {

/*-------------------------------------------------------------------------
  Converts simple polygon to a DCEL (doubly connected edge list, winged edge
  list)
--------------------------------------------------------------------------*/
template< class T >
void PolygonToGraph( 
      const int n, Ptr< point2<T> > P,
      const T orgx, const T orgy,
      const T scalex, const T scaley,
      int fleft, int fright,
      graph_edge_list *E, graph_vertex_list *V )
{
  graph_vertex  *v, *vend, vdummy;
  graph_edge    *e = 0, *eend, edummy;
  int i;
  T x,y;
  graph_vertex_list hv;
  graph_edge_list he;
  unsigned long *cbuf = (unsigned long *)alloca(gul::rtr<double>::mantissa_length());
  
  he.head = eend = &edummy;
  hv.head = vend = &vdummy;
   
  for( i=0; i<n; i++ )
  {
    x = (P[i].x - orgx)/scalex;
    y = (P[i].y - orgy)/scaley;

    e = new graph_edge();
    he.Append(e);

    v = new graph_vertex( PrecPoint2D(
         PrecRational(coord2int<T>(x,cbuf),cbuf),
         PrecRational(coord2int<T>(y,cbuf),cbuf) ), e );
    hv.Append(v);
    
    e->f[0] = fleft;
    e->f[1] = fright;

    e->v[0] = v;
    e->e[0] = e->next;

    e->next->v[1] = v;
    e->next->e[1] = e;
  }
  e->v[1] = vend->prev;
  e->e[1] = eend->prev;
  eend->prev->e[0] = e;

  vend->prev->next = V->head;
  eend->prev->next = E->head;
  if( V->head != NULL ) V->head->prev = vend->prev;
  if( E->head != NULL ) E->head->prev = eend->prev;

  E->head = he.head; E->nElems += n;
  V->head = hv.head; V->nElems += n;
}
/*-------------------------------------------------------------------------
  Converts simple polygon to a DCEL (doubly connected edge list, winged edge
  list)
--------------------------------------------------------------------------*/
template
void PolygonToGraph( 
      const int n, Ptr< point2<double> > P,
      const double orgx, const double orgy,
      const double scalex, const double scaley,
      int fleft, int fright,
      graph_edge_list *E, graph_vertex_list *V );
template
void PolygonToGraph( 
      const int n, Ptr< point2<float> > P,
      const float orgx, const float orgy,
      const float scalex, const float scaley,
      int fleft, int fright,
      graph_edge_list *E, graph_vertex_list *V );


/*-------------------------------------------------------------------------
  Converts a chain of vertices to a DCEL (doubly connected edge list, 
  winged edge list)
--------------------------------------------------------------------------*/
void ChainToGraph( 
      const int n, const point2<double> *P,
      const double orgx, const double orgy,
      const double scalex, const double scaley,
      int fleft, int fright,
      graph_edge_list *E, graph_vertex_list2 *V )
{
  graph_vertex  *v, *vend, vdummy;
  graph_edge    *e = 0, *eend, edummy;
  int i;
  double x,y;
  graph_vertex_list2 hv;
  graph_edge_list he;
  unsigned long *cbuf = (unsigned long *)alloca(gul::rtr<double>::mantissa_length());
  
  he.head = eend = &edummy;
  hv.head = hv.tail = vend = &vdummy;
   
  for( i=n-1; i>0; i-- )
  {
    x = (P[i].x - orgx)/scalex;
    y = (P[i].y - orgy)/scaley;
 
    e = new graph_edge();
    he.Append(e);

    v = new graph_vertex( PrecPoint2D(
         PrecRational(coord2int(x,cbuf),cbuf),
         PrecRational(coord2int(y,cbuf),cbuf) ), e );         
    hv.AppendRight(v);
    
    e->f[0] = fleft;
    e->f[1] = fright;

    e->v[0] = v;
    e->e[0] = e->next;

    e->next->v[1] = v;
    e->next->e[1] = e;
  }

  x = (P[0].x - orgx)/scalex;
  y = (P[0].y - orgy)/scaley;
 
  v = new graph_vertex( PrecPoint2D(
         PrecRational(coord2int(x,cbuf),cbuf),
         PrecRational(coord2int(y,cbuf),cbuf) ), e );
  hv.AppendRight(v);

  e->v[1] = v;
  e->e[1] = NULL;
  eend->prev->e[0] = NULL;
  
  vend->next->prev = V->tail;
  eend->prev->next = E->head;
  if( V->tail != NULL ) V->tail->next = vend->next;
  if( E->head != NULL ) E->head->prev = eend->prev;

  E->head = he.head; E->nElems += n;
  V->tail = hv.tail; V->nElems += n;
  if( V->head == NULL ) V->head = vend->next;
}

/*------------------------------------------------------------------------
  Triangulates a polygon, with contours given as closed polylines
------------------------------------------------------------------------*/
EXP
bool TriangulatePolygon(
            int nContour, Ptr< polyline2<double> > contour, bool oddflag,
            int *nTA, Ptr< triangle2<double> > *TA )
{
  double minx,maxx,miny,maxy,dx,dy,scale;
  int n,i;
  Ptr< point2<double> > P;
  graph_edge_list E;
  graph_vertex_list V;
  triangle_list T;
  PrecRational farleft;   // constructor initializes farleft = 0
  
  if( nContour == 0 ) return false;
                                             /* ------ first contour ------- */
  if( (n = contour[0].n) <= 2 ) return false;
//  printf( "contour[0].n=%d\n", contour[0].n );
  P = contour[0].P;
  if( P[0] != P[n-1] ) return false;  // not closed
  
  guge::CalcBoundingBoxE<double>( n-1, P, minx, maxx, miny, maxy );
  for( i = 1; i < nContour; i++ )
  {
    if( (n = contour[i].n) <= 2 ) return false;
    P = contour[i].P;
    if( P[0] != P[n-1] ) return false;  // not closed
    guge::UpdateBoundingBoxE<double>( n-1, P, minx, maxx, miny, maxy );
  }  

  dx = maxx - minx;
  dy = maxy - miny;
  scale = dx > dy ? dx : dy;
  minx -= scale;         // after this the normalized points will lie between
  miny -= scale;         // 1 and 2

  SetDumpTransformation( minx, miny, scale, scale );
  for( i = 0; i < nContour; i++ ) 
  {
    PolygonToGraph( contour[i].n-1, (contour[i]).P, minx, miny, scale, scale,
                    0, 1, &E, &V );
//    DumpEdges( E.head );
   }
  
  OrientEdges( E.head );
  Regularize( &E, &V );
/*
  printf("After Regularization:\n" );
  printf("======================\n" );
  DumpVertices( V.head );
  DumpEdges( E.head );
*/
  DrawEdges( E.head );

  
  Triangulate( &E, &V, farleft, &T );
  ConvertTriangles( &T, minx, miny, scale, scale, nTA, TA );
  return true;
}

/*-----------------------------------------------------------------------
  Tesselate a polygon
------------------------------------------------------------------------*/
EXP
bool StripTriangulate(
            int nContour, Ptr< polyline2<double> > contour,
            int nStripsX, int nStripsY,
            double bx1, double by1, double bx2, double by2,
            Ptr< Ptr< int > > nTA,
            Ptr< Ptr< Ptr< triangle2<double> > > > TA )
{
  double minx,maxx,miny,maxy,dx,dy,scale;
  Ptr< point2<double> > P;
  graph_edge_list E;
  graph_vertex_list V;
  int i, j, n, nMaxE;
  Ptr< cut_info > CutsX, CutsY;
  PrecRational X1,X2,MinY,MaxY,FarMinY,FarMaxY,DeltaX;
  PrecRational Y1,Y2,MinX,MaxX,FarMinX,FarMaxX,DeltaY;
  Ptr< point2<double> > dom; 
  unsigned long *cbuf = (unsigned long *)alloca(gul::rtr<double>::mantissa_length());
  PrecRational One(ULong(1));
  int k;
  /*
  Ptr< Ptr< Ptr< line2<double> > > > L;
  Ptr< Ptr< int > > nL;
  */
  dom.reserve_place( reserve_stack(point2<double>,2), 2 );
  graph_vertex *Vy1Sl, *Vy1Sr, *Vy2Sl, *Vy2Sr, *Vx1Sa, *Vx1Sb, *Vx2Sa, *Vx2Sb;
  int nCutsX, nCutsY;
  PrecRational LeftX;
  graph_vertex *hvLeftT, *hvRightT;
  graph_edge *e, *er, **hE;
  vertex b;
  graph_vertex *a, *v, *v0;
  int match, iel, ier, cl, cr, nhE;
  bool oddflag;
  triangle_list T;

  nCutsX = nStripsX+2; nCutsY = nStripsY+2;
  
  if( (nCutsX < 4) || (nCutsY < 4) )
    return false; 

  minx = dom[0].x = bx1; miny = dom[0].y = by1;
  maxx = dom[1].x = bx2; maxy = dom[1].y = by2; 
  
  for( i = 0; i < nContour; i++ )
  {
    if( (n = contour[i].n) <= 2 ) return false;
    P = contour[i].P;
    if( P[0] != P[n-1] ) return false;  // not closed
    guge::UpdateBoundingBoxE<double>( n-1, P, minx, maxx, miny, maxy );
  }  

  CutsX.reserve_place( reserve_stack(cut_info,nCutsX), nCutsX );
  CutsY.reserve_place( reserve_stack(cut_info,nCutsY), nCutsY );
  /*
  L.reserve_pool( nCutsY ); nL.reserve_pool( nCutsY );
  for( i = 0; i < nCutsY; i++ )
  { L[i].reserve_pool( nCutsX ); nL[i].reserve_pool( nCutsX ); }
  */

  dx = maxx - minx;
  dy = maxy - miny;
  scale = dx > dy ? dx : dy;
  minx -= scale;         // after this the normalized points will lie between
  miny -= scale;         // 1 and 2
  
  guge::NormalizePointsE( 2, dom, minx, scale, miny, scale );

  for( i = 0; i < nContour; i++ ) 
  {
    PolygonToGraph( contour[i].n-1, contour[i].P, minx, miny, scale, scale,
                    0, 1, &E, &V );
  }
  SetDumpTransformation( minx, miny, scale, scale );
  
  OrientEdges( E.head );
  
  printf( "after construction of graph\n" );
  printf( "***************************\n" );
  DumpVertices( V.head );
  DumpEdges( E.head );

  X1 = PrecRational( coord2int(dom[0].x,cbuf),cbuf );
  X2 = PrecRational( coord2int(dom[1].x,cbuf),cbuf );
  MinY = PrecRational( ULong(1), -1 );
  MaxY = PrecRational(coord2int((double)2,cbuf),cbuf)+One;
  FarMinY = PrecRational(ULong(2),-1);
  FarMaxY = MaxY + One;
  DeltaX = (X2-X1)/PrecRational(ULong(nCutsX-2));
  
  Y1 = PrecRational(coord2int(dom[0].y,cbuf),cbuf);
  Y2 = PrecRational(coord2int(dom[1].y,cbuf),cbuf);
  MinX = MinY;
  MaxX = MaxY;
  FarMinX = FarMinY;
  FarMaxX = FarMaxY;
  DeltaY = (Y2-Y1)/PrecRational(ULong(nCutsY-2));
  
  CutsX[1].val = X1;
  for( i = 2; i < nCutsX-1; i++ )
  {
    CutsX[i].val = X1 + PrecRational(ULong(i-1))*DeltaX;
  }
  CutsX[nCutsX-1].val = X2;

  CutsY[1].val = Y1;
  for( i = 2; i < nCutsY-1; i++ )
  {
    CutsY[i].val = Y1 + PrecRational(ULong(i-1))*DeltaY;
  }
  CutsY[nCutsY-1].val = Y2;

  nMaxE = 10*E.nElems;      // this is a hack !!!!!!!!!!!!!!!!!!!!
  hE = (graph_edge **)alloca( sizeof(graph_edge *) * nMaxE );
  
                                           // split graph into vertical strips
  IntersectWithVerticals( &E, &V, DeltaX, nCutsX, CutsX );

                                // isolate first vertical strip and discard it
  DivideVerticalStrips( &CutsX[nCutsX-1], &CutsX[nCutsX-2], nMaxE,
                        MinY, MaxY, FarMinY, FarMaxY,
                        &Vy1Sr, &Vy1Sl, &Vy2Sr, &Vy2Sl );

  printf( "after disconnecting X-Strip %d and %d\n", nCutsX-1, nCutsX-2 );
  printf( "****************************************\n" );

  for( k = nCutsX-1; k >= 0; k-- )
  {
    printf( "X-Strip %d:\n", k );
    DumpVertices( CutsX[k].V.head );
    DumpEdges( CutsX[k].E.head );
  }
  
  CutsX[nCutsX-1].R.DeleteElems();
  CutsX[nCutsX-1].E.DeleteElems();
  CutsX[nCutsX-1].V.DeleteElems();

  for( i = nCutsX-2; i > 0; i-- )
  {
    DivideVerticalStrips( &CutsX[i], &CutsX[i-1], nMaxE,
                          MinY, MaxY, FarMinY, FarMaxY,
                          &Vy1Sr, &Vy1Sl, &Vy2Sr, &Vy2Sl );
                          
    printf( "after disconnecting X-Strip %d and %d\n", i, i-1 );
    printf( "****************************************\n" );
    for( k = i; k >= 0; k-- )
    {
      printf( "X-Strip %d:\n", k );
      DumpVertices( CutsX[k].V.head );
      DumpEdges( CutsX[k].E.head );
    }

    LeftX = CutsX[i].val;      // x value of the left side 

                               // split vertical strips into horizontal strips 
    IntersectWithHorizontals( &CutsX[i].E, &CutsX[i].V, DeltaY, nCutsY, CutsY );

                                         // isolate first strip and discard it
    DivideHorizontalStrips( &CutsY[nCutsY-1], &CutsY[nCutsY-2], nMaxE,
                            MinX, MaxX, FarMinX, FarMaxX,
                            &Vx1Sa, &Vx1Sb, &Vx2Sa, &Vx2Sb );   

    printf( "after disconnecting Y-Strip %d and %d\n", j, j-1 );
    printf( "==========================================\n" );
    for( k = nCutsY-1; k >= 0; k-- )
    {
      printf( "Y-Strip %d:\n", k );
      DumpVertices( CutsY[k].V.head );
      DumpEdges( CutsY[k].E.head );
    }

    CutsY[nCutsY-1].R.DeleteElems();
    CutsY[nCutsY-1].E.DeleteElems();
    CutsY[nCutsY-1].V.DeleteElems();
    hvLeftT = Vx1Sb; hvRightT = Vx2Sb;  // for next strip
    
                               // divide vertical strips into horizontal strips    
    for( j = nCutsY-2; j > 0; j-- )
    {
      DivideHorizontalStrips( &CutsY[j], &CutsY[j-1], nMaxE,
                              MinX, MaxX, FarMinX, FarMaxX,
                              &Vx1Sa, &Vx1Sb, &Vx2Sa, &Vx2Sb );

      printf( "after disconnecting Y-Strip %d and %d\n", j, j-1 );
      printf( "==========================================\n" );
      for( k = j; k >= 0; k-- )
      {
        printf( "Y-Strip %d:\n", k );
        DumpVertices( CutsY[k].V.head );
        DumpEdges( CutsY[k].E.head );
      }

                    // calculate face index of the right side of the left edge
      e = hvLeftT->e;
      a = e->v[0];
      b = vertex(PrecPoint2D(LeftX,FarMinY));
      
      nhE = EdgeCycle( e, a, hE );
      match = EdgeInsertPosition( a->v, b, nhE, hE, &iel, &ier, &cl, &cr );
      gul::Assert<gul::InternalError>( ndebug || (match == 2) );
      
      er = hE[ier];
      oddflag = (er->f[0] != 0);
                              // remove help edges from the corner edge cycles
      v0 = hvLeftT;
      v = a;
      er = hE[nhE-1];
      if( er->v[0] == v ) er->e[0] = e->e[0]; else er->e[1] = e->e[0];
      v->e = er;
      CutsY[j].V.Remove(v0); CutsY[j].E.Remove(e);
      delete v0; delete e;
      
      v0 = hvRightT;
      e = v0->e;
      v = e->v[1];
      nhE = EdgeCycle( e, v, hE );
      er = hE[nhE-1];
      if( er->v[0] == v ) er->e[0] = e->e[1]; else er->e[1] = e->e[1];
      v->e = er;
      CutsY[j].V.Remove(v0); CutsY[j].E.Remove(e);
      delete v0; delete e;
 
      v0 = Vx2Sa;
      e = v0->e;
      v = e->v[1];
      nhE = EdgeCycle( e, v, hE );
      er = hE[nhE-1];
      if( er->v[0] == v ) er->e[0] = e->e[1]; else er->e[1] = e->e[1];
      v->e = er;
      CutsY[j].V.Remove(v0); CutsY[j].E.Remove(e);
      delete v0; delete e;
 
      v0 = Vx1Sa;
      e = v0->e;
      v = e->v[0];
      nhE = EdgeCycle( e, v, hE );
      er = hE[nhE-1];
      if( er->v[0] == v ) er->e[0] = e->e[0]; else er->e[1] = e->e[0];
      v->e = er;
      CutsY[j].V.Remove(v0); CutsY[j].E.Remove(e);
      delete v0; delete e;
    
      hvLeftT = Vx1Sb;  hvRightT = Vx2Sb;   // for next loop
/*      
      printf( "after removing help edges" );
      printf( "=========================\n" );
      DumpVertices( CutsY[j].V.head );
      DumpEdges( CutsY[j].E.head );
  
      printf( "PoolTab[48]=%p, PoolTab[48]->head = %p\n", 
              PoolTab[48], PoolTab[48]->head );     
*/
      Regularize( &(CutsY[j].E), &(CutsY[j].V) );
/*
      if( (j == 1) && (i == 2) )
        DrawEdges( CutsY[j].E.head );
*/
      Triangulate( &(CutsY[j].E), &CutsY[j].V, FarMinX, &T );
      ConvertTriangles( &T, minx, miny, scale, scale, 
                        &(nTA[j-1][i-1]), &(TA[j-1][i-1]) );
      T.DeleteElems();
/*
      printf( "PoolTab[48]=%p, PoolTab[48]->head = %p\n", 
              PoolTab[48], PoolTab[48]->head );     
*/
      /*
      ConvertEdges( &(CutsY[j].E), minx, miny, scale, scale, 
                    &(nL[j][i]), &(L[j][i]) );
      */

      CutsY[j].R.DeleteElems();          // free strip
      CutsY[j].E.DeleteElems();
      CutsY[j].V.DeleteElems();
    }
    /*
    ConvertEdges( &(CutsY[0].E), minx, miny, scale, scale, // last strip
                  &(nL[0][i]), &(L[0][i]) );
    */
    CutsY[0].R.DeleteElems();
    CutsY[0].E.DeleteElems();
    CutsY[0].V.DeleteElems();

    CutsX[i].R.DeleteElems();
    CutsX[i].E.DeleteElems();
    CutsX[i].V.DeleteElems();
  }

  return true;
}

/*-----------------------------------------------------------------------
  Convert list of Edges to array of line segments (with double coordinates)
  (for debugging)
------------------------------------------------------------------------*/
void ConvertEdges( graph_edge_list *E, 
                   double orgx, double orgy, 
                   double scalex, double scaley,
                   int *nLA, Ptr< line2<double> > *LA  )
{
  graph_edge *e;
  double x1,x2,y1,y2;
  int i;
  Interval ix,iy;
  Ptr< line2<double> > L;
  
  *nLA = E->nElems;
  L.reserve_pool( E->nElems );
  e = E->head;

  i = 0;
  while( e != NULL )
  {
    ix = e->v[0]->v.v().m_x.GetBounds();
    iy = e->v[0]->v.v().m_y.GetBounds();
    x1 = (ix.m_low+ix.m_high)/2.0;
    y1 = (iy.m_low+iy.m_high)/2.0;

    ix = e->v[1]->v.v().m_x.GetBounds();
    iy = e->v[1]->v.v().m_y.GetBounds();
    x2 = (ix.m_low+ix.m_high)/2.0;
    y2 = (iy.m_low+iy.m_high)/2.0;

    x1 = cnv2coord(x1)*scalex + orgx; 
    x2 = cnv2coord(x2)*scalex + orgx; 
    y1 = cnv2coord(y1)*scaley + orgy; 
    y2 = cnv2coord(y2)*scaley + orgy; 

    L[i].P1.x = x1; L[i].P1.y = y1;
    L[i].P2.x = x2; L[i].P2.y = y2;
    i++;
    
    e = e->next;
  }
  *LA = L;
}

/*-----------------------------------------------------------------------
  Convert Triangles to array with double points
------------------------------------------------------------------------*/
void ConvertTriangles( triangle_list *TL, 
                       double orgx, double orgy, 
                       double scalex, double scaley,
                       int *nTA, Ptr< triangle2<double> > *TA  )
{
  Interval ix,iy;
  double x1,x2,x3,y1,y2,y3;
  triangle *t;
  int i;
  Ptr< triangle2<double> > T;  

  T.reserve_pool(TL->nElems);
  
  t = TL->head;
  i = 0;
  while( t != NULL )
  {
    ix = t->v[0].v().m_x.GetBounds();
    iy = t->v[0].v().m_y.GetBounds();
    x1 = (ix.m_low+ix.m_high)/2.0;
    y1 = (iy.m_low+iy.m_high)/2.0;

    ix = t->v[1].v().m_x.GetBounds();
    iy = t->v[1].v().m_y.GetBounds();
    x2 = (ix.m_low+ix.m_high)/2.0;
    y2 = (iy.m_low+iy.m_high)/2.0;

    ix = t->v[2].v().m_x.GetBounds();
    iy = t->v[2].v().m_y.GetBounds();
    x3 = (ix.m_low+ix.m_high)/2.0;
    y3 = (iy.m_low+iy.m_high)/2.0;

    x1 = cnv2coord(x1)*scalex + orgx; 
    x2 = cnv2coord(x2)*scalex + orgx; 
    x3 = cnv2coord(x3)*scalex + orgx; 

    y1 = cnv2coord(y1)*scaley + orgy; 
    y2 = cnv2coord(y2)*scaley + orgy; 
    y3 = cnv2coord(y3)*scaley + orgy; 

    T[i].P1.x = x1; T[i].P1.y = y1;
    T[i].P2.x = x2; T[i].P2.y = y2;
    T[i].P3.x = x3; T[i].P3.y = y3;
    i++;
    t = t->next;
/*
    fprintf( stdout, "%0.15e %0.15e %0.15e %0.15e %0.15e %0.15e\n",
                  x1, y1, 0.0, x2, y2, 0.0 );
    fprintf( stdout, "%0.15e %0.15e %0.15e %0.15e %0.15e %0.15e\n",
                  x2, y2, 0.0, x3, y3, 0.0 );
    fprintf( stdout, "%0.15e %0.15e %0.15e %0.15e %0.15e %0.15e\n",
                  x3, y3, 0.0, x1, y1, 0.0 );
*/
  }
  *TA = T;
  *nTA = i;

}

/**************************************************************************
  The following is just for debugging
**************************************************************************/

float Dump<float>::orgx;
float Dump<float>::orgy;
float Dump<float>::scalex;
float Dump<float>::scaley;

double Dump<double>::orgx;
double Dump<double>::orgy;
double Dump<double>::scalex;
double Dump<double>::scaley;

double orgx,orgy,scalex,scaley;

EXP
void (*DrawMarkFunc) ( point2<double> P1, point2<double> P2, 
                       point2<double> P3 );
EXP
void (*DrawLineFunc) ( point2<double> P1, point2<double> P2 );

void SetDumpTransformation( double aorgx, double aorgy, 
                            double ascalex, double ascaley )
{
  orgx = aorgx;
  orgy = aorgy;
  scalex = ascalex;
  scaley = ascaley;
}

void DumpVertices( graph_vertex *V )
{
  graph_vertex *v;
  double x1,y1;
  
  v = V;

  printf( "VERTICES\n" );
    
  while( v != NULL )
  {
    v->v.v().m_x.Dump(x1);
    v->v.v().m_y.Dump(y1);

    x1 = cnv2coord(x1)*scalex + orgx; 
    y1 = cnv2coord(y1)*scaley + orgy;  
    printf( "%p: (%0.8f, %0.8f), %p]\n", v, x1, y1, v->e );

    v = v->next;
  }

  fflush(stdout);
}

void DumpEdges( graph_edge *E )
{
  graph_edge *e;
  double x1,x2,y1,y2;
  int i = 1;
  
  e = E;

  printf( "EDGES\n" );
    
  while( e != NULL )
  {
    e->v[0]->v.v().m_x.Dump(x1);
    e->v[0]->v.v().m_y.Dump(y1);
    e->v[1]->v.v().m_x.Dump(x2);
    e->v[1]->v.v().m_y.Dump(y2);

    x1 = cnv2coord(x1)*scalex + orgx; 
    x2 = cnv2coord(x2)*scalex + orgx; 
    y1 = cnv2coord(y1)*scaley + orgy; 
    y2 = cnv2coord(y2)*scaley + orgy; 

    printf( "%p: [%d,%d] [%p=(%0.8f, %0.8f), %p]  [%p=(%0.8f, %0.8f), %p]\n", e,
            e->f[0], e->f[1], e->v[0], x1, y1, e->e[0],
            e->v[1], x2, y2, e->e[1] );

    i++;
    e = e->next;
  }

  fflush(stdout);
}

void DrawEdges( graph_edge *E )
{
  graph_edge *e;
  double x1,x2,y1,y2;
  double u = (scalex+scaley)/80.0, r;
  point2<double> m,n,d,beg,end;
  if( DrawLineFunc == 0 ) return;
  if( DrawMarkFunc == 0 ) return;
    
  e = E;

  while( e != NULL )
  {
    e->v[0]->v.v().m_x.Dump(x1);
    e->v[0]->v.v().m_y.Dump(y1);
    e->v[1]->v.v().m_x.Dump(x2);
    e->v[1]->v.v().m_y.Dump(y2);

    x1 = cnv2coord(x1)*scalex + orgx; 
    x2 = cnv2coord(x2)*scalex + orgx; 
    y1 = cnv2coord(y1)*scaley + orgy; 
    y2 = cnv2coord(y2)*scaley + orgy; 

    beg.x = x1;
    beg.y = y1;
    end.x = x2;
    end.y = y2;
      
    DrawLineFunc( beg, end );
      
    m.x = (x1+x2)/2.0;
    m.y = (y1+y2)/2.0;
    d.x = (x2-x1)/2.0;
    d.y = (y2-y1)/2.0;
    n.x = -d.y;
    n.y = d.x;
    r = 1.0 / sqrt( d.x*d.x + d.y*d.y );
    d = r*d;
    n = r*n;

    if( e->f[0] > 0 )
      DrawMarkFunc( m-u*d, m+u*d, m+u*n );
      
    if( e->f[1] > 0 )
      DrawMarkFunc( m-u*d, m+u*d, m-u*n );

    e = e->next;
  }
}



void DrawTriangles( FILE *out, triangle_list *T, double orgx, double orgy, 
                    double scalex, double scaley )
{
  Interval ix,iy;
  double x1,x2,x3,y1,y2,y3;
  triangle *t;
  
  fprintf( out, "LINE_LIST\n" );
  fprintf( out, "%d\n", T->nElems*3 );
  
  t = T->head;
  while( t != NULL )
  {
    ix = t->v[0].v().m_x.GetBounds();
    iy = t->v[0].v().m_y.GetBounds();
    x1 = (ix.m_low+ix.m_high)/2.0;
    y1 = (iy.m_low+iy.m_high)/2.0;

    ix = t->v[1].v().m_x.GetBounds();
    iy = t->v[1].v().m_y.GetBounds();
    x2 = (ix.m_low+ix.m_high)/2.0;
    y2 = (iy.m_low+iy.m_high)/2.0;

    ix = t->v[2].v().m_x.GetBounds();
    iy = t->v[2].v().m_y.GetBounds();
    x3 = (ix.m_low+ix.m_high)/2.0;
    y3 = (iy.m_low+iy.m_high)/2.0;

    x1 = cnv2coord(x1)*scalex + orgx; 
    x2 = cnv2coord(x2)*scalex + orgx; 
    x3 = cnv2coord(x3)*scalex + orgx; 

    y1 = cnv2coord(y1)*scaley + orgy; 
    y2 = cnv2coord(y2)*scaley + orgy; 
    y3 = cnv2coord(y3)*scaley + orgy; 

    fprintf( out, "%0.15e %0.15e %0.15e %0.15e %0.15e %0.15e\n",
                  x1, y1, 0.0, x2, y2, 0.0 );
    fprintf( out, "%0.15e %0.15e %0.15e %0.15e %0.15e %0.15e\n",
                  x2, y2, 0.0, x3, y3, 0.0 );
    fprintf( out, "%0.15e %0.15e %0.15e %0.15e %0.15e %0.15e\n",
                  x3, y3, 0.0, x1, y1, 0.0 );

    t = t->next;
  }
}


}

