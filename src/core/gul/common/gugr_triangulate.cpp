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

#include <iostream.h>
 
#include "gul_types.h"
#include "guar_exact.h"
#include "gugr_basics.h"
#include "gugr_io.h"
#include "gugr_regularize.h"
#include "gugr_triangulate.h"
// #include "gugr_io.h"

namespace gugr {

using gul::AllocError;
using gul::InternalError;


/* ------------------------------------------------------------------------
  Triangulates the regions of a graph. E must be regularized, and V
  sorted. For this purpose the graph is divided into monotone chains.
  Between two monontone chains lies a monotone polygone.
  Only those triangles are calculated, whose edges have a positive face
  index on the inward side 
------------------------------------------------------------------------- */

void Triangulate( graph_edge_list *E, graph_vertex_list *V,
                  const PrecRational& far_left, triangle_list *T )
{
  vertex farleft;
  graph_edge *e, *eb, *ea,*left,*right,**tmpE;
  graph_vertex *v;
  int ileft,iright,codel,coder,nE,Win,Wout,ie,iea,ieb;
  int nP,i;
  graph_edge *e1;
  monpoly_info *P;
  int li,ri;
  
  tmpE = (graph_edge **)alloca( sizeof(graph_edge *)*(E->nElems) );
  if( !tmpE ) throw AllocError();

  if( V->head != NULL ) v = V->head->next;       /* second smallest vertex */
  else return;
  if( v->next == NULL ) return;

  e = E->head;
  while( e != NULL )                            /* initialize weights to 1 */
  { e->reserved[0] = (void *)1; e = e->next; } 
  
/* ---------- weight balancing, first pass ----------------------------- */

  while( v->next != NULL )
  {
    nE = IncidentEdges( v, tmpE );

    farleft = vertex( PrecPoint2D(far_left,v->v.v().m_y) );    
    EdgeInsertPosition( v->v, farleft, nE, tmpE, &ileft, &iright,
                        &codel, &coder );
    left = tmpE[ileft];  right = tmpE[iright];

    if( right->v[0] == v )         /* => leftwards oriented horizontal edge */ 
      e = eb = right->e[0];
    else
      e = eb = right;              /* eb = leftmost incoming edge */

    Win = 0;                       /* sum of weights of incoming edges of v */
    while( e->v[1] == v )
    {
      Win += (int)(e->reserved[0]);
      e = e->e[1];
    }
    Wout = 1;                      /* sum of weights of outgoing edges of v */
    while( e->e[0] != eb )
    {
      Wout++;
      e = e->e[0];
    }
    ea = e;                        /* ea = leftmost outgoing edge */
    
    if( Win > Wout )
      ea->reserved[0] = (void *)(Win-Wout+1);

    v = v->next;
  }

/***** second pass, combined with extraction of monotone polygons *********/

  /* ------- initialize polygon slots at the maximum vertex -------- */

  nP = 0;                         

  nE = IncidentEdges( v, tmpE ); /* edges adjacent to maximum of graph */

  farleft = vertex( PrecPoint2D(far_left,v->v.v().m_y) );    
  EdgeInsertPosition( v->v, farleft, nE, tmpE, &ileft, &iright,
                      &codel, &coder );
  e1 = right = tmpE[iright];

  do 
  {
    e1->reserved[1] = (void *)nP;   /* store index of the polygon on the right*/
    nP += (int)e1->reserved[0];     /* side of this edge                      */
    e1 = e1->e[1];
  }
  while( e1 != right );
  nP--;

  P = (monpoly_info *)alloca( sizeof(monpoly_info) * nP );
  if( P == NULL ) throw AllocError();
  new(P) monpoly_info[nP];

  /*
  std::cout << "number of monotone polygons = " << nP << "\n";
  */

  /* ------- second pass of weight balancing -------- */

  v = v->prev;                  /* v = second largest vertex */

  while( v->prev != NULL )
  {
    nE = IncidentEdges( v, tmpE );

    farleft = vertex( PrecPoint2D(far_left,v->v.v().m_y) );    
    EdgeInsertPosition( v->v, farleft, nE, tmpE, &ileft, &iright,
                        &codel, &coder );
    if( tmpE[iright]->v[0] == v ) /* => leftwards oriented horizontal edge */
      ie = iea = iright;
    else  
      ie = iea = ileft;           /* iea = index of leftmost outgoing edge */

    Wout = 0;                       /* sum of weights of outgoing edges of v */
    do
    {
      Wout += (int)(tmpE[ie]->reserved[0]);

   /* ----- add edge to involved polygons ---------------- */ 

      li = (int)(tmpE[ie]->reserved[1])-1;
      if( (li >= 0) && (tmpE[ie]->f[0] > 0) )
      {
        /*
        std::cout << "adding edge " << (void *)tmpE[ie] << " to polygon " << 
                  li+1 << ", right chain\n";
        */

        if( P[li].AppendRight(*v, *tmpE[ie]->v[1]) )
        {
          /*
          std::cout << "triangulating monotone polygon #" << li+1 << "\n";
          */
          
          TriangulateMonotonePolygon( &P[li].Vl, &P[li].Vr, &P[li].E, T ); 
        }
      }
      ri = li + (int)tmpE[ie]->reserved[0];
      if( (ri < nP) && (tmpE[ie]->f[1] > 0) )
      {     
        /*
        std::cout << "adding edge " << (void *)tmpE[ie] << " to polygon " << 
                  ri+1 << ", left chain\n";
        */
        
        if( P[ri].AppendLeft(*v, *tmpE[ie]->v[1]) )
        {
          /*
          std::cout << "triangulating monotone polygon #" << ri+1 << "\n";
          */

          TriangulateMonotonePolygon( &P[ri].Vl, &P[ri].Vr, &P[ri].E, T ); 
        }
      }
           
   /* ---------------------------------------------------- */

      ie--; if(ie < 0) ie = nE-1;
    }
    while( tmpE[ie]->v[0] == v );

    Win = 0;                      /* sum of weights of incoming edges of v */
    while( tmpE[ie] != tmpE[iea] )
    {
      Win += (int)(tmpE[ie]->reserved[0]);
      ie--; if(ie < 0) ie = nE-1;
    }
    ieb = iea+1;                   /* ieb = index of leftmost incoming edge */
    if( ieb == nE ) ieb = 0;

    if( Wout > Win ) 
      tmpE[ieb]->reserved[0] = (void *)(Wout-Win+(int)tmpE[ieb]->reserved[0]);

  /* -------- set polygon indices for incoming edges -------------------- */ 

    i = (int)tmpE[iea]->reserved[1]; /* index of polygon to the right of */
    e = tmpE[ieb];                   /* leftmost  edges */
    e->reserved[1] = (void *)i;  /* give leftmost incoming edge this index */
    i += (int)e->reserved[0];    /* add multiplicity of edge */
    e = e->e[1];                 /* next edge */
    
    while( e->v[1] == v )        /* is next incoming edge */
    {
      e->reserved[1] = (void *)i;
      i += (int)e->reserved[0];
      e = e->e[1];               /* next edge */
    }
  /* -------------------------------------------------------------------- */

    v = v->prev;
  }

  /* process smallest vertex */

  nE = IncidentEdges( v, tmpE );  /* incident edges to smallest vertex */

  farleft = vertex( PrecPoint2D(far_left,v->v.v().m_y) );    
  EdgeInsertPosition( v->v, farleft, nE, tmpE, &ileft, &iright,
                     &codel, &coder );
  if( tmpE[iright]->IsHorizontal() )  /* => leftwards orient horizontal edge */
    ie = iea = iright;
  else  
    ie = iea = ileft;               /* iea = index of leftmost outgoing edge */

  do    /* add edges to involved polygons */
  {
    li = (int)(tmpE[ie]->reserved[1])-1;
    if( (li >= 0) && (tmpE[ie]->f[0] > 0) )
    {
      /*
      std::cout << "adding edge " << (void *)tmpE[ie] << " to polygon " << 
                li+1 << ", right chain\n";
      */

      if( P[li].AppendRight(*v, *tmpE[ie]->v[1]) )
      {
        /*
        std::cout << "triangulating monotone polygon #" << li+1 << "\n";
        */

        TriangulateMonotonePolygon( &P[li].Vl, &P[li].Vr, &P[li].E, T ); 
      }
    }
    ri = li + (int)tmpE[ie]->reserved[0];
    if( (ri < nP) && (tmpE[ie]->f[1] > 0) )
    {     
      /*
      std::cout << "adding edge " << (void *)tmpE[ie] << " to polygon " << 
                ri+1 << ", left chain\n";
      */

      if( P[ri].AppendLeft(*v, *tmpE[ie]->v[1]) )
      {
        /*
        std::cout << "triangulating monotone polygon #" << ri+1 << "\n";
        */

        TriangulateMonotonePolygon( &P[ri].Vl, &P[ri].Vr, &P[ri].E, T ); 
      }
    }

    ie--; if(ie < 0) ie = nE-1;
  }
  while( ie != iea );

  for( i = 0; i < nP; i++ )
  {
    P[i].Vl.DeleteElems();
    P[i].Vr.DeleteElems();
    P[i].E.DeleteElems();
  }
}



/* ------------------------------------------------------------------------
  Triangulates a monotone polygon

  The vertices in V1 and V2, and the edges in E are deleted
  in this function! 
-------------------------------------------------------------------------- */

void TriangulateMonotonePolygon(
              graph_vertex_list2 *V1, graph_vertex_list2 *V2,
              graph_edge_list *E, triangle_list *T )
      
{
  graph_vertex_list2 Vq, S;
  graph_vertex *vh,*v1,*v2,*vi,*vi_1,*vs,*vt,*v,*vh1,*vh2;
  graph_edge *e;
  triangle *tri;
  int sign;
  graph_vertex_list V;

  /*
  cout << "Triangulation of montone polygon\n";
  cout << "********************************\n";
  cout << "Vertices of left chain\n";
  cout << "======================\n";
  gugr::Dump<float>::dump_vertices( V1->head );
  cout << "Vertices of right chain\n";
  cout << "======================\n";
  gugr::Dump<float>::dump_vertices( V2->head );
  cout << "Edges (of both chains)\n";
  cout << "======================\n";
  gugr::Dump<float>::dump_edges( E->head );
  */

/* ----- merge together the vertices of both monotone chains ------------ */
  
  vh1 = V1->Last();            /* first vertex of chain 1 */
  V1->Remove(vh1);
  V.Append(vh1);

  vh2 = V2->Last();            /* first vertex of chain 2 */
  V2->Remove(vh2);
  V.Append(vh2);

  v1 = V1->Last();
  V1->Remove(v1);
  v1->reserved[0] = (void *)-1;        /* element of left chain */
 
  v2 = V2->Last();
  V2->Remove(v2);
  v2->reserved[0] = (void *)+1;        /* element of right chain */

  if( (V1->Last() == NULL) && (V2->Last() == NULL) )  /* ready, no triangles */
  {
    delete v1; delete v2;
    V1->DeleteElems();
    V2->DeleteElems();
    V.DeleteElems();
    E->DeleteElems();
    return;
  }
                    /* make first vertex of both chains have the same address */
  vh = new graph_vertex( vh2->v, NULL, vh2->reserved[1] );                                              
  Vq.AppendRight(vh);
  vh->reserved[0] = (void *)0;

  e = new graph_edge();
  E->Append(e);
  e->l = v1->e->l;
  e->v[0] = v1;
  e->v[1] = vh;
  v1->e = e;

  e = new graph_edge();
  E->Append(e);
  e->l = v2->e->l;
  e->v[0] = v2;
  e->v[1] = vh;
  v2->e = e;

  while( (v1 != NULL) || (v2 != NULL) )
  {
    if( v1 != NULL )
    {
      if( v2 != NULL )
        sign = Compare( v1->v.v().m_y, v2->v.v().m_y );
      else 
        sign = +1;
    }
    else sign = -1;
    
    if( sign >= 0 )
    {
      if( v1->e->IsHorizontal() )  /* compress together horizontal edges */
      {
        vs = v1;
        vt = V1->Last();
        while( (vt != NULL) && (vt->e->IsHorizontal()) )
        {
          V1->Remove(vt);
          delete v1;
          v1 = vt;
          vt = V1->Last();
        }
        if( v1 != vs )
        {
          e = new graph_edge();
          E->Append(e);
          e->l = v1->e->l;
          e->v[0] = v1;
          e->v[1] = vh;
          v1->e = e;
        }
      }      
      v1->reserved[0] = (void *)-1;
      Vq.AppendRight(v1);
      vh = v1;
      v1 = V1->Last();
      if( v1 != NULL ) V1->Remove(v1);
    }
    else
    {
      if( v2->e->IsHorizontal() )  /* compress together horizontal edges */
      {
        vs = v2;
        vt = V2->Last();
        while( (vt != NULL) && (vt->e->IsHorizontal()) )
        {
          V2->Remove(vt);
          delete v2;
          v2 = vt;
          vt = V2->Last();
        }
        if( v2 != vs )
        {
          e = new graph_edge();
          E->Append(e);
          e->l = v2->e->l;
          e->v[0] = v2;
          e->v[1] = vh;
          v2->e = e;
        }
      }

      v2->reserved[0] = (void *)+1;
      Vq.AppendRight(v2);
      vh = v2;
      v2 = V2->Last();
      if( v2 != NULL ) V2->Remove(v2);
    }
  }

  vh->prev->reserved[0] = (void *)0;    /* this is the last vertex */
  Vq.Remove(vh);              
  delete vh;
  
/* ---- construct the triangles ------------------------------------------- */

  v = Vq.First();     /* put first two vertices on stack */
  Vq.Remove(v);
  S.AppendRight(v);

  v = Vq.First();
  Vq.Remove(v);
  S.AppendRight(v);

  v = Vq.First();
  Vq.Remove(v);

  while( v->reserved[0] != (void *)0 )     /* not the last vertex */
  {
    vi = S.Last();
    v1 = S.First();
    
    e = v->e;
    
    if( e->v[1] == vi )  /* adjacent to top of stack */
    {
      vi_1 = vi->prev;

      sign = DetermineOrientation(v->v.v(),vi->v.v(),vi_1->v.v());
      sign *= (int)v->reserved[0];

      if( (sign == 0) && (v->e->IsHorizontal()) ) /* special case */
      {
        S.Remove(vi);
        V.Append(vi);
        S.AppendRight(v);
      }
      else
      {
        while( (sign > 0) && (S.nElems > 1) )
        {
          tri = new triangle();
          if( (int)vi->reserved[0] == +1 )
          {
            tri->v[0] = v->v;    tri->code0 = (int)v->reserved[1];
            tri->v[1] = vi->v;   tri->code1 = (int)vi->reserved[1];
            tri->v[2] = vi_1->v; tri->code2 = (int)vi_1->reserved[1];
            /*
            cout << "TRIANGLE\n";
            Dump<float>::dump_vertice(v);
            Dump<float>::dump_vertice(vi);
            Dump<float>::dump_vertice(vi_1);
            */
          }
          else     
          {
            tri->v[0] = v->v;    tri->code0 = (int)v->reserved[1];
            tri->v[1] = vi_1->v; tri->code1 = (int)vi_1->reserved[1];
            tri->v[2] = vi->v;   tri->code2 = (int)vi->reserved[1];
            /*
            cout << "TRIANGLE\n";
            Dump<float>::dump_vertice(v);
            Dump<float>::dump_vertice(vi_1);
            Dump<float>::dump_vertice(vi);
            */
          }
          T->Append(tri);

          S.Remove(vi);
          V.Append(vi);
          if( S.nElems > 1 )
          {
            vi = vi_1;
            vi_1 = vi->prev;
            sign = DetermineOrientation(v->v.v(),vi->v.v(),vi_1->v.v());
            sign *= (int)v->reserved[0];
          }
        }
        S.AppendRight(v);
      }
    } 
    else                  /* adjacent to bottom of stack */
    {
      gul::Assert<gul::InternalError>( ndebug || (e->v[1] == v1) );
     
      if( v->e->IsHorizontal() )  /* special case */
      {
        gul::Assert<gul::InternalError>( ndebug || (S.nElems == 2) );
        S.Remove(v1);
        V.Append(v1);
        S.AppendRight(v);
      }
      else
      {
        v2 = v1->next;
        do
        {
          tri = new triangle();
          if( (int)v2->reserved[0] == +1 )
          {
            tri->v[0] = v->v;   tri->code0 = (int)v->reserved[1];
            tri->v[1] = v2->v;  tri->code1 = (int)v2->reserved[1];
            tri->v[2] = v1->v;  tri->code2 = (int)v1->reserved[1];
            /*
            cout << "TRIANGLE\n";
            Dump<float>::dump_vertice(v);
            Dump<float>::dump_vertice(v2);
            Dump<float>::dump_vertice(v1);
            */
          }
          else
          {
            tri->v[0] = v->v;   tri->code0 = (int)v->reserved[1];
            tri->v[1] = v1->v;  tri->code1 = (int)v1->reserved[1];
            tri->v[2] = v2->v;  tri->code2 = (int)v2->reserved[1];
            /*
            cout << "TRIANGLE\n";
            Dump<float>::dump_vertice(v);
            Dump<float>::dump_vertice(v1);
            Dump<float>::dump_vertice(v2);
            */
          }
          T->Append(tri);
    
          S.Remove(v1);
          V.Append(v1);
          v1 = v2;
          v2 = v1->next;            
        }
        while( v2 != NULL );
    
        S.AppendRight(v);   
      }
    }

    v = Vq.First();
    Vq.Remove(v);
  }

  /* last vertex */

  v1 = S.First();
  do
  {
    v2 = v1->next;

    tri = new triangle();
    if( (int)v2->reserved[0] == +1 )
    {
      tri->v[0] = v->v;   tri->code0 = (int)v->reserved[1];
      tri->v[1] = v2->v;  tri->code1 = (int)v2->reserved[1];
      tri->v[2] = v1->v;  tri->code2 = (int)v1->reserved[1];
      /*
      cout << "TRIANGLE\n";
      Dump<float>::dump_vertice(v);
      Dump<float>::dump_vertice(v2);
      Dump<float>::dump_vertice(v1);
      */
    }
    else
    {
      tri->v[0] = v->v;   tri->code0 = (int)v->reserved[1];
      tri->v[1] = v1->v;  tri->code1 = (int)v1->reserved[1];
      tri->v[2] = v2->v;  tri->code2 = (int)v2->reserved[1];
      /*
      cout << "TRIANGLE\n";
      Dump<float>::dump_vertice(v);
      Dump<float>::dump_vertice(v1);
      Dump<float>::dump_vertice(v2);
      */
    }
    T->Append(tri);

    S.Remove(v1);
    V.Append(v1);
    v1 = v2;
    v2 = v1->next;
  }
  while( v2 != NULL );
  S.Remove(v1);
  V.Append(v1);

  delete v;

  gul::Assert<gul::InternalError>( ndebug || 
    ((S.nElems == 0) && (Vq.nElems == 0) &&
     (V1->nElems == 0) && (V2->nElems == 0)) );

//  S.DeleteElems();
//  Vq.DeleteElems();
//  V1->DeleteElems();
//  V2->DeleteElems();
  V.DeleteElems();
  E->DeleteElems();
}

}
