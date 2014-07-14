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
#include "gust_new.h"
#include "gul_vector.h"
#include "guge_linear.h"
#include "guge_normalize.h"
#include "gunu_basics.h"
#include "gunu_derivatives.h"
#include "gunu_divide.h"
#include "gunu_intersect.h"
#include "gunu_linearize.h"
#include "guar_exact.h"
#include "gugr_basics.h"
#include "gugr_split.h"
#include "gugr_io.h"
#include "gugr_regularize.h"
#include "gugr_triangulate.h"

#include "gunu_tesselate.h"

// #include <crtdbg.h>

using gul::rtl;
using gul::List;
using gul::List2;
using gul::ListNode;
using gul::point;
using gul::hpoint;
using gul::point2;
using gul::hpoint2;
using gul::curve;
using gul::Min;
using gul::set;
using gul::is_zero;
using gul::normalize;
using gul::euclid;
using guar::PrecRational;
using gugr::graph_vertex;
using gugr::graph_edge;
using gugr::cut_info;
using gugr::IntersectWithVerticals;
using gugr::DivideVerticalStrips;
using gugr::IntersectWithHorizontals;
using gugr::DivideHorizontalStrips;
using gugr::EdgeCycle;
using gugr::graph_vertex_list;
using gugr::graph_edge_list;
using gugr::vertex_rep;
using gugr::coord2int;

/*-------------------------------------------------------------------
  approximate trimmed NURBS surface with triangles
--------------------------------------------------------------------*/
namespace gunu {

template< class T >
EXP
void LinearizeSurface( 
            int max_iter,
            const int nu, const int pu, Ptr< T >& KnotsU,
            const int nv, const int pv, Ptr< T >& KnotsV,
            Ptr< Ptr< hpoint<T> > >& Pw,
            const T tol, const rtl<T>& t,
            Ptr< Ptr< point2<T> > >& contour,
            void (*outfunc1)( TessInfo<T> *, void * ),                                  
            void (*outfunc2)( TessInfo<T> *, gugr::GraphInfo *, gugr::GraphConvInfo<T> *,
                              void * ),
            void *usrdata )
{
  TessInfo<T>       A;
  gugr::GraphConvInfo<T>  Gconv;
  graph_edge_list   E;
  graph_vertex_list V;
  gugr::GraphInfo         G;
  PrecRational MinX,MaxX,FarMinX,FarMaxX,MinY,MaxY,FarMinY,FarMaxY,X1,X2,Y1,Y2;
  T x1,x2,y1,y2,minx,maxx,miny,maxy,dx,dy,scale;
  int i,nContour,n;
  unsigned long *cbuf = (unsigned long *)alloca(gul::rtr<T>::mantissa_length());
  PrecRational One(guar::ULong(0x10000000));

  if( (nContour = contour.nElems()) < 1 ) return;
  
  minx = x1 = miny = y1 = (T)0.0;   /* --- initialise graph info --- */
  maxx = x2 = maxy = y2 = (T)1.0; 
  
  for( i = 0; i < nContour; i++ )
  {
    if( (n = contour[i].nElems()) <= 2 ) return;
    if( contour[i][0] != contour[i][n-1] ) return;  // not closed
    guge::UpdateBoundingBoxE<T>( n-1, contour[i], minx, maxx, miny, maxy );
  }  
  dx = maxx - minx;
  dy = maxy - miny;
  scale = dx > dy ? dx : dy;
  minx -= scale;         // after this the normalized points will lie between
  miny -= scale;         // 1 and 2
  
  Gconv.minx = minx; Gconv.miny = miny; Gconv.scale = scale;

  x1 = (x1 - minx)/scale;   x2 = (x2 - minx)/scale;
  y1 = (y1 - miny)/scale;   y2 = (y2 - miny)/scale;

  for( i = 0; i < nContour; i++ ) 
  {
    gugr::PolygonToGraph<T>( 
                   contour[i].nElems()-1, contour[i], minx, miny, scale,scale,
                   1, 0, &E, &V );
  }

  /*
  gugr::SetDumpTransformation( minx, miny, scale, scale );
  gugr::Dump<T>::set_transformation( minx, miny, scale, scale );
  */

  gugr::OrientEdges( E.head );

  /*
  cout << "after construction of graph\n";
  cout << "***************************\n";
  gugr::Dump<T>::dump_vertices( V.head );
  gugr::Dump<T>::dump_edges( E.head );
  */

  X1 = PrecRational( coord2int(x1,cbuf),cbuf );
  X2 = PrecRational( coord2int(x2,cbuf),cbuf );
  Gconv.MinY = PrecRational( guar::ULong(0x10000000), -1 );
  Gconv.MaxY = PrecRational(coord2int((T)2,cbuf),cbuf)+One;
  Gconv.FarMinY = PrecRational(guar::ULong(0x20000000),-1);
  Gconv.FarMaxY = Gconv.MaxY + One;
  
  Y1 = PrecRational(coord2int(y1,cbuf),cbuf);
  Y2 = PrecRational(coord2int(y2,cbuf),cbuf);
  Gconv.MinX = Gconv.MinY;
  Gconv.MaxX = Gconv.MaxY;
  Gconv.FarMinX = Gconv.FarMinY;
  Gconv.FarMaxX = Gconv.FarMaxY;
  
  gugr::InitGraph<T>( X1, X2, Y1, Y2, &Gconv, &E, &V, &G );  // initialise graph info
  E.DeleteElems();
  V.DeleteElems();
                    /* --- initialise nurbs surface info --- */
  A.org->nu = nu;
  A.org->pu = pu;  
  A.org->U =  KnotsU;
  A.org->nv = nv;
  A.org->pv = pv;  
  A.org->V =  KnotsV;
  A.org->Pw = Pw;
  /*
  guge::CalcBoundingBox( (A.org->nu+1)*(A.org->nv+1), 
               A.org->Pw[0], A.x1, A.x2, A.y1, A.y2, A.z1, A.z2 ); 
  */

  A.org->P00 = gul::euclid( A.org->Pw[0][0] );
  A.org->P01 = gul::euclid( A.org->Pw[0][A.org->nu] );
  A.org->P10 = gul::euclid( A.org->Pw[A.org->nv][0] );
  A.org->P11 = gul::euclid( A.org->Pw[A.org->nv][A.org->nu] );

  A.u1 = 0.0;
  A.u2 = 1.0; 
  A.v1 = 0.0;
  A.v2 = 1.0; 

  /*
  _CrtSetReportMode( _CRT_WARN, _CRTDBG_MODE_DEBUG );
  _RPT1( _CRT_WARN, "number of pool items = %d\n", 
         gust::pooldbg::count );
  _RPT2( _CRT_WARN, "rationals = %d, reps = %d\n", 
         PrecRational::count, PrecRational::PrecRationalRep::count );
  _RPT3( _CRT_WARN, "graph_vertex's = %d, vertex's = %d, vertex_rep's = %d\n", 
         gugr::graph_vertex::count, gugr::vertex::count, 
         gugr::vertex_rep::count );
  */

  LinearizeTrimmedSurface( 1, max_iter, &A, &G, &Gconv, tol, t, 
                           outfunc1, outfunc2, usrdata );
}

/*-------------------------------------------------------------------
  approximate trimmed NURBS surface with triangles
--------------------------------------------------------------------*/
template EXP
void LinearizeSurface( 
       int max_iter,
       const int nu, const int pu, Ptr< float >& KnotsU,
       const int nv, const int pv, Ptr< float >& KnotsV,
       Ptr< Ptr < gul::hpoint<float> > >& Pw,
       const float tol, const gul::rtl<float>& t,
       Ptr< Ptr< gul::point2<float> > >& contour,
       void outfunc1( gunu::TessInfo<float> *, void * ),                                   
       void outfunc2( gunu::TessInfo<float> *, gugr::GraphInfo *, gugr::GraphConvInfo<float> *,
                      void * ),
       void *usrdata );
template EXP
void LinearizeSurface( 
       int max_iter,
       const int nu, const int pu, Ptr< double >& KnotsU,
       const int nv, const int pv, Ptr< double >& KnotsV,
       Ptr< Ptr < hpoint<double> > >& Pw,
       const double tol, const rtl<double>& t,
       Ptr< Ptr< point2<double> > >& contour,
       void outfunc1( TessInfo<double> *, void * ),                                   
       void outfunc2( TessInfo<double> *, gugr::GraphInfo *, gugr::GraphConvInfo<double> *,
                      void * ),
       void *usrdata );

}

