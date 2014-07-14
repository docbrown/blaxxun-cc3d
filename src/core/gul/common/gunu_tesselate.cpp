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
void LinearizeTrimmedSurface( 
            int current_iter, int max_iter,
            TessInfo<T> *A,
            gugr::GraphInfo *G, gugr::GraphConvInfo<T> *Gconv,
            const T tol, const rtl<T>& t,
            void outfunc1( gunu::TessInfo<T> *, void * ),                                   
            void outfunc2( gunu::TessInfo<T> *, gugr::GraphInfo *, gugr::GraphConvInfo<T> *, 
                           void * ),
            void *usrdata )
{
  int nA,i;
  gunu::TessInfo<T> **pA;  
  gugr::GraphInfo *Gsub[4];

  if( !A->f.linearized && !A->f.divided )
  {      
    if( current_iter < max_iter )
      gunu::LinearizeOrDivide( A, tol, t );
    else
      A->f.linearized = 1;
  }

  if( !A->f.linearized )
  {
    for( i = 0; i < 4; i++ ) Gsub[i] = new gugr::GraphInfo();
    gugr::SplitGraph( G, (A->u1+A->u2)/(T)2.0, (A->v1+A->v2)/(T)2.0, Gconv, Gsub );
    
    nA = 4;
    pA = A->sub;

    for( i = 0; i < nA; i++ )          /* Rekursion */
    {
      if( (Gsub[i]->V.nElems==4) && (Gsub[i]->E.nElems==4) )
      {
        if( Gsub[i]->face > 0 ) 
          gunu::LinearizeSurface( current_iter+1, max_iter, pA[i], tol, t,
                                  outfunc1, usrdata );
      }
      else
      {
        gunu::LinearizeTrimmedSurface<T>( current_iter+1, max_iter, 
                pA[i], Gsub[i], Gconv, tol, t, outfunc1, outfunc2, usrdata );
      }
     
      delete pA[i];
      pA[i] = 0;      
      delete Gsub[i];
      Gsub[i] = 0;
    }
  }
  else       /* ------- output ------------------ */  
  {
    if( (G->V.nElems==4) && (G->E.nElems==4) )
    {
      if( G->face > 0 ) outfunc1( A, usrdata );
    }
    else
    {
      outfunc2( A, G, Gconv, usrdata );
    }    
  }
}

/*-------------------------------------------------------------------
  approximate trimmed NURBS surface with triangles
--------------------------------------------------------------------*/

template
void LinearizeTrimmedSurface( 
       int current_iter, int max_iter,
       TessInfo<float> *A,
       gugr::GraphInfo *G, gugr::GraphConvInfo<float> *Gconv,
       const float tol, const rtl<float>& t,
       void outfunc1( TessInfo<float> *, void * ),                                   
       void outfunc2( TessInfo<float> *, gugr::GraphInfo *, gugr::GraphConvInfo<float> *, 
                      void * ),
       void *usrdata );
template
void LinearizeTrimmedSurface( 
       int current_iter, int max_iter,
       TessInfo<double> *A,
       gugr::GraphInfo *G, gugr::GraphConvInfo<double> *Gconv,
       const double tol, const rtl<double>& t,
       void outfunc1( TessInfo<double> *, void * ),                                   
       void outfunc2( TessInfo<double> *, gugr::GraphInfo *, gugr::GraphConvInfo<double> *, 
                      void * ),
       void *usrdata );

/*-------------------------------------------------------------------------
  approximate trimmed NURBS surface with triangles, another user interface
  function
-------------------------------------------------------------------------*/

template< class T >
EXP
void TessQuadCb( TessInfo<T> *A, void *usrdata )
{
  gul::point<T> P00,P01,P10,P11,T00,T01,T10,T11;
  int nu,nv,pu,pv;
  TessCbData<T> *cb = (TessCbData<T> *)usrdata;

  P00 = A->org->P00;
  P01 = A->org->P01;
  P10 = A->org->P10;
  P11 = A->org->P11;

  if( cb->need_normals == 0 )
  {
    cb->quadfunc( cb->usrdata, 
                 &P00, &P01, &P10, &P11, NULL, NULL, NULL, NULL,
                 NULL, NULL, NULL, NULL );
  }
  else                             /* Calc Normals */
  {
    nu = A->org->nu;
    pu = A->org->pu;
    nv = A->org->nv;
    pv = A->org->pv;
    Ptr< Ptr < hpoint<T> > >& Pw = A->org->Pw;

    CalcNormal_U0V0<T>( nu, pu, nv, pv, Pw, &T00, cb->t );
    CalcNormal_U1V0<T>( nu, pu, nv, pv, Pw, &T01, cb->t );
    CalcNormal_U0V1<T>( nu, pu, nv, pv, Pw, &T10, cb->t );
    CalcNormal_U1V1<T>( nu, pu, nv, pv, Pw, &T11, cb->t );

    cb->quadfunc( cb->usrdata, &P00, &P01, &P10, &P11,
                  &T00, &T01, &T10, &T11, &A->u1, &A->u2, &A->v1, &A->v2 );
  }
}
template EXP void TessQuadCb( TessInfo<float> *A, void *usrdata );
template EXP void TessQuadCb( TessInfo<double> *A, void *usrdata );

/*--------------------------------------------------------------------- 
  function values and normals in the inside of a quad are calculated
  
 V1 *----------* U1 = U2   with triangular interpolation 
    |        / |           (Barycentric coordinates)
    |      /   |
    |    /     |
    |  /       |
    |/         /
    *----------* V2
 W1 = W2

--------------------------------------------------------------------- */

template< class T >
struct vertex_convert_cache
{
  point<T>  p;
  point<T>  n;
  point2<T> d;  // domain coordinates

  void *operator new( size_t s )
  {
    size_t dummy;
    void *p = gust::PoolAlloc( s, &dummy );
    if( p == NULL ) throw gul::PoolAllocError();
    return(p);
  }
  void operator delete( void *p, size_t s )
  {
    if( p != 0 ) gust::PoolFree( p, s );
  }
};

template< class T >
inline vertex_convert_cache<T> *FillVertexCache( 
         vertex_rep *vert, const int code, bool normals,
         const T& orgx, const T& orgy, const T& scale,
         const T& u0, const T& v0, const T& w, const T& h, const T& a,
         const point<T>& W1, const point<T>& V2,
         const point<T>& V1, const point<T>& U1,
         const point<T>& W1n, const point<T>& V2n,
         const point<T>& V1n, const point<T>& U1n )
{
  guar::Interval iu,iv;
  T u,v,ru,rv,tu,tv,tw;
  vertex_convert_cache<T> *buf;
  
  vert->reserved = buf = new vertex_convert_cache<T>();
      
  iu = vert->v.m_x.GetBounds();
  iv = vert->v.m_y.GetBounds();
  u = (T)((iu.m_low+iu.m_high)/2.0);
  v = (T)((iv.m_low+iv.m_high)/2.0);
  u = buf->d.x = gugr::cnv2coord(u)*scale + orgx; 
  v = buf->d.y = gugr::cnv2coord(v)*scale + orgy; 
  ru = u-u0;
  rv = v-v0;
  if( code > 0 )  // vertex lies above quad diagonal
  {
    tu = h*ru/a;
    tw = w*(h-rv)/a;
    tv = (T)1.0 - tu - tw;
    buf->p = tu*U1 + tv*V1 + tw*W1;
    if( normals ) buf->n = tu*U1n + tv*V1n + tw*W1n;       
  }
  else if( code < 0 )  // below diagonal
  {
    tu = w*rv/a;
    tw = h*(w-ru)/a;
    tv = (T)1.0 - tu - tw;
    buf->p = tu*U1 + tv*V2 + tw*W1;
    if( normals ) buf->n = tu*U1n + tv*V2n + tw*W1n;
  }
  else       // on diagonal 
  {
    tw = (w-ru)/w;        /* interpolate linearly on diagonal */
    tu = (T)1.0 - tw;
    buf->p = tw*W1 + tu*U1;
    if( normals ) buf->n = tw*W1n + tu*U1n;
  }
  return buf;
}

template< class T >
EXP
void TessTriCb( TessInfo<T> *A, gugr::GraphInfo *G, gugr::GraphConvInfo<T> *Gconv, 
                void *data )
{
  gugr::triangle_list Tri;
  gugr::triangle *tri,*tri_next;
  TessCbData<T> *cb = (TessCbData<T> *)data;
  point<T> T00,T01,T10,T11;
  vertex_convert_cache<T> *c1,*c2,*c3;
 
  const point<T>& P00 = A->org->P00;
  const point<T>& P01 = A->org->P01;
  const point<T>& P10 = A->org->P10;
  const point<T>& P11 = A->org->P11;
  const int& nu = A->org->nu;
  const int& pu = A->org->pu;  
  const int& nv = A->org->nv;
  const int& pv = A->org->pv;
  Ptr< Ptr< hpoint<T> > >& Pw = A->org->Pw;

  const T& orgx = Gconv->minx;
  const T& orgy = Gconv->miny;
  const T& scale = Gconv->scale;

  const T& u0 = A->u1;
  const T& u1 = A->u2;
  const T& v0 = A->v1;
  const T& v1 = A->v2;
  
  const bool& normals = cb->need_normals;
  void *usrdata = cb->usrdata;

  T w = u1-u0; // width of quad  (u)
  T h = v1-v0; // height of quad (v)
  T a = w*h;   // area of quad

  /*
  cout << "before insertion of diagonal\n";
  cout << "***************************\n";
  gugr::Dump<T>::dump_vertices( G->V.head );
  gugr::Dump<T>::dump_edges( G->E.head );
  */

  gugr::InsertDiagonal( &(G->E), &(G->V), G->P[0][0], G->P[1][1] );

  /* 
  cout << "after insertion of diagonal\n";
  cout << "***************************\n";
  gugr::Dump<T>::dump_vertices( G->V.head );
  gugr::Dump<T>::dump_edges( G->E.head );
  */

  gugr::Regularize( &(G->E), &(G->V) );

  /*
  cout << "after regularization\n";
  cout << "***************************\n";
  gugr::Dump<T>::dump_vertices( G->V.head );
  gugr::Dump<T>::dump_edges( G->E.head );
  */

  gugr::Triangulate( &(G->E), &(G->V), Gconv->FarMinX, &Tri );
//  Tri.DeleteElems();  

  if( normals )     /* calc Normals */
  {
    CalcNormal_U0V0<T>( nu, pu, nv, pv, Pw, &T00, cb->t );
    CalcNormal_U1V0<T>( nu, pu, nv, pv, Pw, &T01, cb->t );
    CalcNormal_U0V1<T>( nu, pu, nv, pv, Pw, &T10, cb->t );
    CalcNormal_U1V1<T>( nu, pu, nv, pv, Pw, &T11, cb->t );
  }

  tri = Tri.head;
  while( tri != 0 )
  {
    if( tri->v[0].m_rep->reserved == 0 )
      c1 = FillVertexCache( 
                     tri->v[0].m_rep, tri->code0, normals, orgx, orgy, scale,
                     u0, v0, w, h, a, P00, P01, P10, P11, T00, T01, T10, T11 );
    else
      c1 = (vertex_convert_cache<T> *)tri->v[0].m_rep->reserved;
      
    if( tri->v[1].m_rep->reserved == 0 )
      c2 = FillVertexCache( 
                     tri->v[1].m_rep, tri->code1, normals, orgx, orgy, scale,
                     u0, v0, w, h, a, P00, P01, P10, P11, T00, T01, T10, T11 );
    else
      c2 = (vertex_convert_cache<T> *)tri->v[1].m_rep->reserved;

    if( tri->v[2].m_rep->reserved == 0 )
      c3 = FillVertexCache( 
                     tri->v[2].m_rep, tri->code2, normals, orgx, orgy, scale,
                     u0, v0, w, h, a, P00, P01, P10, P11, T00, T01, T10, T11 );
    else
      c3 = (vertex_convert_cache<T> *)tri->v[2].m_rep->reserved;

    /*
    cout << "(" << c1->d.x << ", " << c1->d.y << ")  (" <<
              c2->d.x << ", " << c2->d.y << ")  (" <<
              c3->d.x << ", " << c3->d.y << ")\n";              
    */

    cb->trifunc( usrdata, &c1->p, &c2->p, &c3->p,  &c1->n, &c2->n, &c3->n,
                 &c1->d, &c2->d, &c3->d );    
    tri = tri->next;
  }
  tri = Tri.head;
  while( tri != 0 )   // delete vertex_convert_caches
  {
    delete ((vertex_convert_cache<T> *)tri->v[0].m_rep->reserved);
    tri->v[0].m_rep->reserved = 0;
    delete ((vertex_convert_cache<T> *)tri->v[1].m_rep->reserved);
    tri->v[1].m_rep->reserved = 0;
    delete ((vertex_convert_cache<T> *)tri->v[2].m_rep->reserved);
    tri->v[2].m_rep->reserved = 0;

    tri_next = tri->next;
    delete tri;     // delete triangle rec;
    tri = tri_next;
  }
}
template EXP void TessTriCb( TessInfo<float> *A, gugr::GraphInfo *G, gugr::GraphConvInfo<float> *Gconv, void *data );
template EXP void TessTriCb( TessInfo<double> *A, gugr::GraphInfo *G, gugr::GraphConvInfo<double> *Gconv, void *data );


}

