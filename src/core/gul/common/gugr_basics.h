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

#ifndef GUGR_BASICS_H
#define GUGR_BASICS_H

namespace gugr {

// using namespace gust;
// using namespace guar;
// using gul::Error;
using gul::ndebug;
using gul::Assert;
using gul::InternalError;
using gul::PoolAllocError;
using gul::List;
using gul::List2;
using gul::bounding_box;
using guar::Interval;
using guar::ULong;
using guar::PrecRational;
using guar::PrecPoint2D;
using guar::Compare;
using gust::PoolAlloc;
using gust::PoolFree;

struct vertex_rep
{
  PrecPoint2D            v;
  int                    m_refcount;
  void                  *reserved;  /* used internally as a pointer to a buffer
                                       for conversions */
  vertex_rep() 
  { 
    m_refcount = 1;
    reserved = 0;
  }

  explicit vertex_rep( const PrecPoint2D &av ) : v(av) 
  { 
    m_refcount = 1;
    reserved = 0;
  }

  ~vertex_rep()
  {
  }
  
  void *operator new( size_t s )
  {
    size_t dummy;
    void *p = PoolAlloc( s, &dummy );
    if( p == NULL ) throw PoolAllocError();
    return(p);
  }
  void operator delete( void *p, size_t s )
  {
    PoolFree( p, s );
  }
};

struct vertex
{
  friend bool operator==( const vertex& a, const vertex& b );
  
  vertex_rep *m_rep;
  
  vertex() 
  { 
    m_rep = NULL;
  }

  explicit vertex( const vertex &other )
  {
    if( other.m_rep ) other.m_rep->m_refcount++;
    m_rep = other.m_rep;
  }

  explicit vertex( const PrecPoint2D &av )
  {
    m_rep = new vertex_rep( av );
  }

  ~vertex() 
  { 
    if( (m_rep != NULL) && (--m_rep->m_refcount == 0) ) delete m_rep;
  }

  vertex& operator=( const vertex& other )  {
    if( other.m_rep ) other.m_rep->m_refcount++;
    if( (m_rep != NULL) && (--m_rep->m_refcount == 0) ) {
      delete m_rep;
    }
    m_rep = other.m_rep;
    return( *this );    
  }

  PrecPoint2D& v() const { return(m_rep->v); }
};

inline bool operator==( const vertex& a, const vertex& b )
{
  return( a.m_rep == b.m_rep );
}

struct line_rep
{
  PrecPoint2D     v;
  PrecRational    dx;
  PrecRational    dy;
  int             m_refcount;

  line_rep() { m_refcount = 1; }

  line_rep( const PrecPoint2D &av, const PrecRational& adx,
            const PrecRational &ady ) : v(av), dx(adx), dy(ady)
  { m_refcount = 1; }

  void *operator new( size_t s )
  {
    size_t dummy;
    void *p = PoolAlloc( s, &dummy );
    if( p == NULL ) throw PoolAllocError();
    return(p);
  }
  
  void operator delete( void *p, size_t s )
  {
    PoolFree( p, s );
  }
};

struct line
{
  line_rep *m_rep;

  line() { m_rep = NULL; }

  explicit line( const line &other )
  {
    if( other.m_rep ) other.m_rep->m_refcount++;
    m_rep = other.m_rep;
  }

  ~line() { if( (m_rep != NULL)&&(--m_rep->m_refcount == 0) ) delete m_rep; }


  explicit line( const PrecPoint2D &av, const PrecRational& adx,
                 const PrecRational &ady )
  {
    m_rep = new line_rep( av, adx, ady );
  }


  line& operator=( const line& other ) {
    if( other.m_rep ) other.m_rep->m_refcount++;
    if( (m_rep != NULL) && (--m_rep->m_refcount == 0) ) {
      m_rep->~line_rep();
      PoolFree( m_rep, sizeof(line_rep) );      
    }
    m_rep = other.m_rep;
    return( *this ); }
  PrecPoint2D& v() const { return(m_rep->v); }
  PrecRational& dx() const { return(m_rep->dx); }
  PrecRational& dy() const { return(m_rep->dy); }
  bool IsNULL( void ) const { return(m_rep == NULL); }
};

struct graph_edge;

struct graph_vertex
{
  vertex                 v;
  graph_edge            *e;
  
  void                  *reserved[2];

  graph_vertex          *next;
  graph_vertex          *prev;

  graph_vertex() 
  {
  }
  explicit graph_vertex( const PrecPoint2D& av, graph_edge *ae ) : v(av) 
  { 
    e = ae;
  }
  explicit graph_vertex( const vertex& av, graph_edge *ae ) 
  { 
    v = av; e = ae;
  }
                     /* sets the reserved[1] field, only for internal use! */
  explicit graph_vertex( const vertex& av, graph_edge *ae, void *code ) 
  { 
    v = av; e = ae; reserved[1] = code;
  }

  ~graph_vertex()
  {
  }

  void *operator new( size_t s )
  {
    size_t dummy;
    void *p = PoolAlloc( s, &dummy );
    if( p == NULL ) throw PoolAllocError();
    return(p);
  }
  void operator delete( void *p, size_t s )
  {
    PoolFree( p, s );
  }
};

typedef List<graph_vertex> graph_vertex_list;
typedef List2<graph_vertex> graph_vertex_list2;

struct graph_edge
{
  struct graph_vertex   *v[2];
  struct graph_edge     *e[2];
  int                    f[2];

  line                   l;
  
  void                  *reserved[2];

  struct  graph_edge    *next;
  struct  graph_edge    *prev;
 
  graph_edge() { };

  explicit graph_edge( graph_vertex *v0, graph_vertex *v1 )
  { 
    v[0] = v0; v[1] = v1;
  }

  bool IsHorizontal(void)
  { 
    if( l.IsNULL() ) CalcLine();
    return( l.dy().IsZero() );
  }
    
  void CalcLine( void );

//  graph_edge( graph_vertex *v1, graph_vertex *v2 ) { v[0] = v1; v[1] = v2; }

  void *operator new( size_t s )
  {
    size_t dummy;
    void *p = PoolAlloc( s, &dummy );
    if( p == NULL ) throw PoolAllocError();
    return(p);
  }
  
  void operator delete( void *p, size_t s )
  {
    PoolFree( p, s );
  }
};


typedef List<graph_edge> graph_edge_list;

typedef struct _cut_record
{
  graph_edge            *e;
  PrecRational         val;
  graph_vertex          *v;
  
  struct _cut_record    *next;
  struct _cut_record    *prev;

  void *operator new( size_t s )
  {
    size_t dummy;
    void *p = PoolAlloc( s, &dummy );
    if( p == NULL ) throw PoolAllocError();
    return(p);
  }  
  void operator delete( void *p, size_t s )
  {
    PoolFree( p, s );
  }
}
cut_record;

typedef List<cut_record> cut_record_list;

typedef struct _cut_info
{
  PrecRational        val;
  cut_record_list       R;
  graph_edge_list       E;
  graph_vertex_list     V;
}
cut_info;

struct triangle
{
  vertex v[3];
  int code0 : 2;  
  int code1 : 2;  
  int code2 : 2;  

  struct triangle    *next;
  struct triangle    *prev;

  void *operator new( size_t s )
  {
    size_t dummy;
    void *p = PoolAlloc( s, &dummy );
    if( p == NULL ) throw PoolAllocError();
    return(p);
  }
  void operator delete( void *p, size_t s )
  {
    PoolFree( p, s );
  }
};

typedef List<triangle> triangle_list;

struct monpoly_info
{
  graph_vertex_list2 Vl;
  graph_vertex_list2 Vr;
  graph_edge_list    E;

  /* ------------------------------------------------------------------------
    Appends a edge to the left chain of a monotone polygon. If this closes
    the polygon the function returns true
  ------------------------------------------------------------------------ */
  inline bool AppendLeft( const graph_vertex& v1, const graph_vertex& v2 )
  {
    graph_vertex *vl1,*vl2;
    graph_edge *e;
  
    if( Vl.IsEmpty() )
    {
      vl2 = new graph_vertex(v2.v,NULL,v2.reserved[1]);
      Vl.AppendLeft(vl2);
    }
    else
      vl2 = Vl.head;
    
    e = new graph_edge();
    E.Append(e);
    
    vl1 = new graph_vertex(v1.v,e,v1.reserved[1]);
    Vl.AppendLeft(vl1);
 
    e->v[0] = vl1;
    e->v[1] = vl2;    
                                        /* if v is endpoint of right chain: */
    if((!Vr.IsEmpty())&&(Vr.head->v == v1.v)) 
      return(true);

    return(false);
  }
  /* ------------------------------------------------------------------------
    Appends a vertex to the right chain of a monotone polygon. If this closes
    the polygon the function returns true
  -------------------------------------------------------------------------- */
  inline bool AppendRight( const graph_vertex& v1, const graph_vertex& v2 )
  {
    graph_vertex *vr1,*vr2;
    graph_edge *e;
  
    if( Vr.IsEmpty() )
    {
      vr2 = new graph_vertex(v2.v,NULL,v2.reserved[1]);
      Vr.AppendLeft(vr2);
    }
    else
      vr2 = Vr.head;
    
    e = new graph_edge();
    E.Append(e);
    
    vr1 = new graph_vertex(v1.v,e,v1.reserved[1]);
    Vr.AppendLeft(vr1);
 
    e->v[0] = vr1;
    e->v[1] = vr2;    
  
    if((!Vl.IsEmpty())&&(Vl.head->v == v1.v)) /*if v is endpoint of left chain*/
      return(true);

    return(false);
  }
};




/* ----------------------------------------------------------------
   gets the edges incident to vertex 'v'
------------------------------------------------------------------ */
int IncidentEdges( const graph_vertex *v, graph_edge **A );

/* ----------------------------------------------------------------
   gets the edges incident to edge 'e' at vertex 'v'
------------------------------------------------------------------ */
int EdgeCycle( graph_edge *e, graph_vertex *v, graph_edge **A );

/* --------------------------------------------------------------------
  Orient edges, so that y2 > y1 or x2 < x1 if y2 = y1
--------------------------------------------------------------------- */
void OrientEdges( graph_edge *E );

/* ------------------------------------------------------------------------
  Determine on which side of AB the point C is
-------------------------------------------------------------------------- */
int DetermineOrientation( const PrecPoint2D& A, const PrecPoint2D& B, 
                          const PrecPoint2D& C );

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
  int *match, int *code_c, int *code_d );
 
/* --------------------------------------------------------------------
  Find the insertion positon for a oriented line segment in an edge 
  cycle of a DCEL 

  Remarks:

  code_edge = 3 if angle(ab,edge) < 180
            = 2 if angle(ab,edge) = 0 or 180
            = 1 if angle(ab,edge) > 180
  match = 1 if ab and left are colinear 
        = 2 if ab and right are colinear
        = 0 if none of both edges lies on ab         
---------------------------------------------------------------------*/ 
int EdgeInsertPosition(
         const vertex& a, const vertex& b,
         int nE, graph_edge **E,
         int *eleft, int *eright,
         int *code_left, int *code_right );

}


#endif
