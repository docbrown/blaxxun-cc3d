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
 
#ifndef GUGR_IO_H
#define GUGR_IO_H

#include <iostream.h>
#include <iomanip.h>

namespace gugr {

using gul::Assert;
using gul::Ptr;
using gul::rtr;
using guar::uint64;
using gul::point2;
using gul::line2;
using gul::polyline2;
using gul::triangle2;


template< class T >
inline int coord2int( const T& f, unsigned long *buf );

template<>
inline int coord2int<double>( const double& f, unsigned long *buf )
{
  uint64 i;
  unsigned long hi,lo;
  
  gul::Assert<gul::InternalError>( ndebug | (1.0 <= f && f <= 2.0) );
  i = (uint64)(f*gul::rtr<double>::epsilon_inv());
  i -= (uint64)gul::rtr<double>::epsilon_inv();
  hi = (i >> 32) & LL(0xffffffff);
  lo = i & LL(0xffffffff); 
  if( !hi )
  {
    if( !lo ) return 0;
    buf[0] = lo;
    return 1;
  }
  buf[0] = lo;
  buf[1] = hi;
  return 2; 
}

template<>
inline int coord2int<float>( const float& f, unsigned long *buf )
{
  unsigned long i;
  
  gul::Assert<InternalError>( ndebug | (1.0f <= f && f <= 2.0f) );
  i = (unsigned long)(f * gul::rtr<float>::epsilon_inv());
  i -= (unsigned long)gul::rtr<float>::epsilon_inv();
  if( !i ) return 0;
  buf[0] = i;
  return 1;
}


template< class T >
T cnv2coord( const T& i )
{
  return i * gul::rtr<T>::epsilon() + (T)1.0;
}


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
      graph_edge_list *E, graph_vertex_list *V );


/*------------------------------------------------------------------------
  output functions for debugging
------------------------------------------------------------------------*/
template< class T >
class Dump
{
public:
  static T orgx,orgy,scalex,scaley; 
public:
  static void set_transformation( T aorgx, T aorgy, T ascalex, T ascaley )
  {
    orgx = aorgx; 
    orgy = aorgy;
    scalex = ascalex;
    scaley = ascaley;
  }
  static void dump_vertice( graph_vertex *v )
  {
    double dx1,dy1;
    T x1,y1;
  
    if( v != NULL )
    {
      v->v.v().m_x.Dump(dx1);
      v->v.v().m_y.Dump(dy1);
      x1 = (T)dx1;
      y1 = (T)dy1; 

      x1 = cnv2coord<T>(x1)*scalex + orgx; 
      y1 = cnv2coord<T>(y1)*scaley + orgy;  
      std::cout << (void *)v << ": (" <<
                std::setprecision(8) << x1 << ", " << y1 << 
                "), " << (void *)v->e << "\n";
    }
    std::cout.flush();
  }
  static void dump_vertices( graph_vertex *V )
  {
    graph_vertex *v;
    double dx1,dy1;
    T x1,y1;
  
    v = V;
    std::cout << "VERTICES\n";
    while( v != NULL )
    {
      v->v.v().m_x.Dump(dx1);
      v->v.v().m_y.Dump(dy1);
      x1 = (T)dx1;
      y1 = (T)dy1; 

      x1 = cnv2coord<T>(x1)*scalex + orgx; 
      y1 = cnv2coord<T>(y1)*scaley + orgy;  
      std::cout << (void *)v << ": (" <<
                std::setprecision(8) << x1 << ", " << y1 << 
                "), " << (void *)v->e << "\n";
      v = v->next;
    }
    std::cout.flush();
  }
  static void dump_edges( graph_edge *E )
  {
    graph_edge *e;
    double dx1,dx2,dy1,dy2;
    T x1,x2,y1,y2;
    
    e = E;
    std::cout << "EDGES\n";    
    while( e != NULL )
    {
      e->v[0]->v.v().m_x.Dump(dx1);
      e->v[0]->v.v().m_y.Dump(dy1);
      e->v[1]->v.v().m_x.Dump(dx2);
      e->v[1]->v.v().m_y.Dump(dy2);

      x1 = (T)dx1; x2 = (T)dx2; y1 = (T)dy1; y2 = (T)dy2;
      
      x1 = cnv2coord<T>(x1)*scalex + orgx; 
      x2 = cnv2coord<T>(x2)*scalex + orgx; 
      y1 = cnv2coord<T>(y1)*scaley + orgy; 
      y2 = cnv2coord<T>(y2)*scaley + orgy; 

      std::cout << (void *)e << ": [" << e->f[0] << "," << e->f[1] << "] [" <<
                (void *)e->v[0] << "=(" << std::setprecision(8) <<
                x1 << ", " << y1 << "), " << (void *)e->e[0] <<
                "]  [" << (void *)e->v[1] << "=(" <<
                x2 << ", " << y2 << "), " << (void *)e->e[1];
      if( e->l.IsNULL() )
        std::cout << "]\n";
      else
      {
        e->l.v().m_x.Dump(dx1);
        e->l.v().m_y.Dump(dy1);
        x1 = (T)dx1; y1 = (T)dy1;
        x1 = cnv2coord<T>(x1)*scalex + orgx; 
        y1 = cnv2coord<T>(y1)*scaley + orgy; 
 
        e->l.dx().Dump(dx2);
        e->l.dy().Dump(dy2);
        x2 = (T)dx2; y2 = (T)dy2;

        std::cout << "]  L: (" << x1 << ", " << y1 << "), dx=" <<
                  x2 << ", dy=" << y2 << "\n";
      }
      e = e->next;
    }
    std::cout.flush();
  }
};

EXP
extern void (*DrawMarkFunc) ( point2<double> P1, point2<double> P2, 
                       point2<double> P3 );
EXP
extern void (*DrawLineFunc) ( point2<double> P1, point2<double> P2 );

void SetDumpTransformation( double aorgx, double aorgy, 
                       double ascalex, double ascaley );
void DumpVertices( graph_vertex *V );
void DumpEdges( graph_edge *E );
void DrawEdges( graph_edge *E );
void DrawTriangles( FILE *out, triangle_list *T, double orgx, double orgy, 
                    double scalex, double scaley );
/*-----------------------------------------------------------------------
  Convert Triangles to array with double points
------------------------------------------------------------------------*/
void ConvertTriangles( triangle_list *TL, 
                       double orgx, double orgy, 
                       double scalex, double scaley,
                       int *nTA, Ptr< triangle2<double> > *TA  );
/*-----------------------------------------------------------------------
  Convert list of Edges to array of line segments (with double coordinates)
  (for debugging)
------------------------------------------------------------------------*/
void ConvertEdges( graph_edge_list *E, 
                   double orgx, double orgy, 
                   double scalex, double scaley,
                   int *nLA, Ptr< line2<double> > *LA  );

}

#endif
