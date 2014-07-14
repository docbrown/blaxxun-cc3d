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

#ifndef GUGR_SPLIT_H
#define GUGR_SPLIT_H

namespace gugr {

template< class T >
struct GraphConvInfo
{
  T minx;    // normalization parameters
  T miny;
  T scale;
  
  guar::PrecRational MinX;      // point to the left of all points
  guar::PrecRational MaxX;
  guar::PrecRational FarMinX;   // point far to the left 
  guar::PrecRational FarMaxX;
  guar::PrecRational MinY;
  guar::PrecRational MaxY;
  guar::PrecRational FarMinY;
  guar::PrecRational FarMaxY;
};

struct GraphInfo
{
  gugr::graph_vertex      *P[2][2];
  int                face;

  gugr::graph_edge_list    E;
  gugr::graph_vertex_list  V;

  ~GraphInfo()
  {
    E.DeleteElems();
    V.DeleteElems();
  }
  void *operator new( size_t s )
  {
    size_t dummy;
    void *p = gust::PoolAlloc( s, &dummy );
    if( p == NULL ) throw gul::PoolAllocError();
    return(p);
  }  
  void operator delete( void *p, size_t s )
  {
    gust::PoolFree( p, s );
  }
};
typedef GraphInfo *GraphInfo4[4];


/*----------------------------------------------------------------------
  Special utility function, used in the tesselation of nurbs surfaces.
  It inserts the diagonal of a tesselation quad into the graph for this
  quad. It is assumed that width and height of quad are equal, i.e.
  x2-x1 = y2-y1
----------------------------------------------------------------------*/
void InsertDiagonal( 
              graph_edge_list *E, graph_vertex_list *V,
              graph_vertex *P11, graph_vertex *P22 );

/* -------------------------------------------------------------------------
  Calculates the intersection of a graph with horizontal cutting lines 
-------------------------------------------------------------------------- */
void IntersectWithHorizontals( 
          graph_edge_list *E, graph_vertex_list *V,
          const PrecRational &deltay,
          const int nS, gul::Ptr< cut_info > S );

/* -------------------------------------------------------------------------
  Divide graph into an independant upper and lower part
-------------------------------------------------------------------------- */
void DivideHorizontalStrips( 
              cut_info *Sa, cut_info *Sb, int maxE,
              const PrecRational& minx, const PrecRational maxx,
              const PrecRational& far_left, const PrecRational& far_right,
              graph_vertex **minSaV, graph_vertex **minSbV,
              graph_vertex **maxSaV, graph_vertex **maxSbV );
              
/* -------------------------------------------------------------------------
  Calculates the intersection of a graph with vertical cutting lines 
-------------------------------------------------------------------------- */
void IntersectWithVerticals( 
          graph_edge_list *E, graph_vertex_list *V,
          const PrecRational &deltax,
          const int nS, gul::Ptr< cut_info > S );

/* -------------------------------------------------------------------------
  Divide graph into an independant left and right part
-------------------------------------------------------------------------- */
void DivideVerticalStrips( 
              cut_info *Sb, cut_info *Sa, int maxE,
              const PrecRational& miny, const PrecRational maxy,
              const PrecRational& far_bottom, const PrecRational& far_top,
              graph_vertex **minSbV, graph_vertex **minSaV,
              graph_vertex **maxSbV, graph_vertex **maxSaV );


/* ---------------------------------------------------------------------------
  splits a graph into four independent parts
---------------------------------------------------------------------------- */
template< class T >
void SplitGraph( GraphInfo *G, T xmed, T ymed,
                 GraphConvInfo<T> *Gconv, GraphInfo4& sub );

/* ---------------------------------------------------------------------------
  inits a graph info
---------------------------------------------------------------------------- */
template< class T >
void InitGraph( 
       const PrecRational& X1, const PrecRational& X2,
       const PrecRational& Y1, const PrecRational& Y2,
       GraphConvInfo<T> *Gconv, graph_edge_list *E, graph_vertex_list *V,
       GraphInfo *G );

}
 
#endif
