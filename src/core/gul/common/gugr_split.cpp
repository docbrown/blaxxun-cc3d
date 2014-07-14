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
#include "gul_error.h"
#include "guma_sorting.h"
#include "guar_exact.h"
#include "gugr_basics.h"
#include "gugr_split.h"
#include "gugr_io.h"

namespace gugr {

// using namespace guar;
using gul::AllocError;
using gul::Ptr;

/* ------------------------------------------------------------------------
  Sort "Cut records" according to their value
------------------------------------------------------------------------- */

int compare_cut_records( void *p1, void *p2, void *data )
{
  PrecRational &r1 = ((cut_record *)p1)->val, &r2 = ((cut_record *)p2)->val;
  int sign;
  
  sign = Compare( r1, r2 );
  return(sign);
}

void sort_cut_records( int ns, Ptr< cut_info > S )
{
  cut_record *rec, **tmpR;
  int i,j,max_nR;
  
  max_nR = 0;
  for( i = 1; i <= ns; i++ )
    if( S[i].R.nElems > max_nR ) max_nR = S[i].R.nElems;

  if( max_nR > 0 )
  {
    tmpR = (cut_record **)alloca( sizeof(cut_record *)*max_nR );
    if( !tmpR ) throw AllocError();
  }
  else tmpR = 0;  // make compiler happy :)
    
  for( i = 1; i <= ns; i++ )
  {
    if( S[i].R.nElems > 1 )
    {
      rec = S[i].R.head;
      for( j = 0; j < S[i].R.nElems; j++ )
      {
        tmpR[j] = rec;
        rec = rec->next;
      } 
      guma::PtrHeapSort( S[i].R.nElems,(void **)tmpR, compare_cut_records,NULL);

      S[i].R.head = tmpR[0];
      tmpR[0]->prev = NULL;
      for( j = 1; j < S[i].R.nElems; j++ )
      {
        tmpR[j-1]->next = tmpR[j];
        tmpR[j]->prev = tmpR[j-1];
      }
      tmpR[S[i].R.nElems-1]->next = NULL;
    }
  }
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  Special utility function, used in the tesselation of nurbs surfaces.
  It inserts the diagonal of a tesselation quad into the graph for this
  quad. It is assumed that width and height of quad are equal, i.e.
  x2-x1 = y2-y1
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/*-------------------------------------------------------------------------
  calculates the intersections of a graph with a diagonal line
-------------------------------------------------------------------------*/
inline void IntersectWithDiagonal( 
              const graph_edge_list& E, const graph_vertex_list& V,
              const PrecRational &Qx0, const PrecRational& Qy0, // quad lowerleft
              cut_record_list& R )
{
  PrecRational x,y,Qh,x1,y1,mu,dx,dy,One(ULong(1));
  graph_edge *e;
  graph_vertex *v;
  int i,k,sign;
  cut_record *rec, **tmpR;
  
  v = V.head;
  while( v != NULL )
  {
    x = v->v.v().m_x - Qx0;
    y = v->v.v().m_y - Qy0;
    sign = Compare(y,x); // above, on or below diagonal
    v->reserved[1] = (void *)sign;
    if( sign == 0 )      // new record, when on diagonal
    {
      rec = new cut_record();
      R.Append( rec );      
      rec->v = v;
      rec->e = NULL;
      rec->val = v->v.v().m_x;
    }
    v = v->next;
  }
  Qh = Qx0 - Qy0;
    
  e = E.head;
  while( e != NULL )  
  {
    i = (int)(e->v[0]->reserved[1]);
    k = (int)(e->v[1]->reserved[1]);

    if( k*i < 0 ) // edge intersects diagonal 
    {
      if( e->l.IsNULL() )
        e->CalcLine();
      x1 = e->l.v().m_x;      
      y1 = e->l.v().m_y;
      dx = e->l.dx();

      if( !dx.IsZero() ) // not a vertical line
      {
        dy = e->l.dy();
        if( !dy.IsZero() )  // not a horizontal line
        {
          mu = (Qh - x1 + y1) / (dx - One);
          x = x1 + mu*dx;
          y = y1 + mu;
        }
        else
        {
          y = y1;
          x = y1 + Qh;
        }
      }
      else
      {
        x = x1;
        y = x1 - Qh;
      }
      rec = new cut_record();
      R.Append( rec );
      rec->v = new graph_vertex( PrecPoint2D(x,y), NULL );
      rec->v->reserved[1] = (void *)0;  // on diagonal
      rec->e = e;          
      rec->val = x;
    }
    e = e->next;
  }
                                         /* --- sort cut_records (on x) ---- */
  tmpR = (cut_record **)alloca( sizeof(cut_record *)*R.nElems );
  if( !tmpR ) throw AllocError();

  rec = R.head;
  for( i = 0; i < R.nElems; i++ )
  {
    tmpR[i] = rec;
    rec = rec->next;
  } 
  guma::PtrHeapSort( R.nElems,(void **)tmpR, compare_cut_records,NULL);

  R.head = tmpR[0];
  tmpR[0]->prev = NULL;
  for( i = 1; i < R.nElems; i++ )
  {
    tmpR[i-1]->next = tmpR[i];
    tmpR[i]->prev = tmpR[i-1];
  }
  tmpR[R.nElems-1]->next = NULL;  
}

/*--------------------------------------------------------------------------
  inserts a diagonal into the graph of a quad
  uses the reserved[1] field of vertices 
---------------------------------------------------------------------------*/
void InsertDiagonal( 
              graph_edge_list *E, graph_vertex_list *V,
              graph_vertex *P11, graph_vertex *P22 )            
{
  cut_record_list Recs;
  cut_record *rec;
  int nhE,L,R,match,cR,cL,ivh,ivl,maxE;
  graph_edge *e,*eh,*el,**hE;
  graph_vertex *lv;
  vertex *lowerleft = &P11->v;
  vertex *upperright = &P22->v;

  IntersectWithDiagonal( *E, *V, lowerleft->v().m_x, lowerleft->v().m_y, Recs );

  maxE = 3*E->nElems + V->nElems;
  hE = (graph_edge **)alloca( sizeof(graph_edge *)*maxE );
  if( hE == NULL ) throw AllocError();

  rec = Recs.head;
  lv = rec->v;
  gul::Assert<InternalError>( ndebug || (lv == P11) );
  rec = rec->next;

  while( rec != NULL )
  {
    if( rec->e == NULL )    /* vertex on diagonal */
    {
      nhE = IncidentEdges( rec->v, hE );
      match = EdgeInsertPosition( rec->v->v, *lowerleft, nhE, hE, 
                                  &L, &R, &cL, &cR );
      if( cR != 2 ) // have to create a new edge
      {
        e = new graph_edge();
        E->Append(e);

        e->v[0] = lv;
        e->v[1] = rec->v;

        if( hE[L]->v[0] == rec->v ) // connect upper end of new edge
        {
          e->e[1] = hE[L]->e[0];
          hE[L]->e[0] = e;
          e->f[0] = e->f[1] = hE[L]->f[0];
        }
        else
        {
          e->e[1] = hE[L]->e[1];
          hE[L]->e[1] = e;
          e->f[0] = e->f[1] = hE[L]->f[1];
        }

        nhE = IncidentEdges( lv, hE );
        match = EdgeInsertPosition( lv->v, *upperright, nhE, hE, 
                                    &L, &R, &cL, &cR ); 
        if( hE[L]->v[0] == lv ) // connect lower end of new edge
        {
          e->e[0] = hE[L]->e[0];
          hE[L]->e[0] = e;
        }
        else
        {
          e->e[0] = hE[L]->e[1];
          hE[L]->e[1] = e;
        }
      }
    }
    else           /* diagonal intersects an edge */
    {
      el = rec->e;

      eh = new graph_edge();  // new edge for half of edge above diagonal  
      E->Append(eh);
      rec->v->e = eh;         // give new vertex an incident edge
      V->Append(rec->v);      // append it to vertex list        

      if( (int)el->v[0]->reserved[1] > 0 ) // part with v0 above diagonal 
      {
        ivh = 0;
        ivl = 1;
      }
      else                                 // part with v1 above diagonal
      {
        ivh = 1;
        ivl = 0;
      }
      eh->v[ivl] = rec->v;
      eh->v[ivh] = el->v[ivh];
      eh->f[0] = el->f[0];
      eh->f[1] = el->f[1];
        
      nhE = EdgeCycle( el, el->v[ivh], hE );
      if( nhE > 1 )
      {
        eh->e[ivh] = el->e[ivh];
        if( hE[nhE-1]->e[0] == el ) 
          hE[nhE-1]->e[0] = eh; 
        else 
          hE[nhE-1]->e[1] = eh;
      }
      else
        eh->e[ivh] = eh;
        
      el->v[ivh]->e = eh;      // eh replaces original edge in upper edge cycle
      el->v[ivh] = rec->v;     // shorten intersecting edge to lower half

                                   /* ---- create new edge for diagonal ---- */
      e = new graph_edge();
      E->Append(e);
      e->v[0] = lv;
      e->v[1] = rec->v;
      e->f[0] = e->f[1] = el->f[ivl];
      
      nhE = IncidentEdges( lv, hE );
      match = EdgeInsertPosition( lv->v, *upperright, nhE, hE, 
                                    &L, &R, &cL, &cR ); 
      if( hE[L]->v[0] == lv ) // connect lower end of new edge
      {
        e->e[0] = hE[L]->e[0];
        hE[L]->e[0] = e;
      }
      else
      {
        e->e[0] = hE[L]->e[1];
        hE[L]->e[1] = e;
      }
      eh->e[ivl] = e;
      e->e[1] = el;
      el->e[ivh] = eh;
    }
    lv = rec->v;
    rec = rec->next;
  }
  gul::Assert<InternalError>( ndebug || (lv == P22) );

  Recs.DeleteElems();
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

  Functions for splitting a graph into horizontal strips

++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/* -------------------------------------------------------------------------
  Calculates the intersection of a graph with horizontal cutting lines 
-------------------------------------------------------------------------- */

void IntersectWithHorizontals( 
          graph_edge_list *E, graph_vertex_list *V,
          const PrecRational &deltay,
          const int nS, Ptr< cut_info > S )
{
  int ns = nS-1, i,j,k;
  graph_vertex *v,*v_next;
  graph_edge *e, *e_next;
  PrecRational y,t,q,r,x0,y0,x,dx,dy;
  bool rz;
  cut_record *rec;

  v = V->head;

  while( v != NULL )
  {
    v_next = v->next;

    y = v->v.v().m_y;
  
    t = (y-S[1].val)/deltay;
    t.DivMod( &q, &r ); 
    i = ((int)q) + 1;  
    if( i <= 0 ) { i = 0; rz = false; }
    else if( i > ns ) { i = ns; rz = false; }
    else { rz = r.IsZero(); }
  
    v->reserved[0] = (void *)i;
    v->reserved[1] = (void *)rz;
    
    V->Remove( v );
    S[i].V.Append( v );

    if( rz )
    { 
      rec = new cut_record();
      S[i].R.Append( rec );
           
      rec->v = v;
      rec->e = NULL;
      rec->val = v->v.v().m_x;
    }

    v = v_next;
  }

  e = E->head;

  while( e != NULL )  
  {
    e_next = e->next;

    i = (int)(e->v[0]->reserved[0]);
    k = (int)(e->v[1]->reserved[0]);

    E->Remove( e );
    S[k].E.Append( e );

    if( i !=  k )
    {
      if( e->l.IsNULL() )
      {
        e->CalcLine();
      }

      x0 = e->l.v().m_x;
      y0 = e->l.v().m_y;
    
      dx = e->l.dx();

      if( (int)e->v[1]->reserved[1] ) k--;

      for( j = i+1; j <= k; j++ )
      {
        if( !dx.IsZero() )
          x = x0 + (S[j].val-y0)*dx;
        else
          x = x0;
          
        rec = new cut_record();
        S[j].R.Append( rec );
              
        rec->v = NULL;
        rec->e = e;          
        rec->val = x;
      }
    }

    e = e_next;
  }

  sort_cut_records( ns, S );
}

/* -------------------------------------------------------------------------
  Divide graph into an independant upper and lower part
-------------------------------------------------------------------------- */
 
void DivideHorizontalStrips( 
              cut_info *Sa, cut_info *Sb, int maxE,
              const PrecRational& minx, const PrecRational maxx,
              const PrecRational& far_left, const PrecRational& far_right,
              graph_vertex **minSaV, graph_vertex **minSbV,
              graph_vertex **maxSaV, graph_vertex **maxSbV )
{
  graph_edge **E;
  cut_record *rec;
  int nE,L1,L2,R1,R2,match1,match2,cL1,cR1,cL2,cR2;
  vertex farleft,farright;
  graph_edge *ah,*el,*bl,*bh,*er,*al, *e, *eu, *ew;
  graph_vertex *v, *lva, *lvb, *vu, *vl, *delv;
  int fa,fb,nfa,nfb;
  line hori;

  farleft = vertex( PrecPoint2D(far_left,Sa->val) );
  farright = vertex( PrecPoint2D(far_right,Sa->val) );
  hori = line( farleft.v(), PrecRational(ULong(1)), PrecRational() );
 
  E = (graph_edge **)alloca( sizeof(graph_edge *)*maxE );
  if( E == NULL ) throw AllocError();
  
  rec = Sa->R.head;
  fa=fb=nfa=nfb=0;
                                   /* new vertex for strip above cutting line*/
  v = new graph_vertex( PrecPoint2D(minx,Sa->val), NULL );
  Sa->V.Append(v);
  lva = *minSaV = v;
                                  /* new vertex for strip below cutting line */
  v = new graph_vertex(lva->v, NULL);     
  Sb->V.Append(v);
  lvb = *minSbV = v;
  
  delv = 0;
  while( rec != NULL )
  {
    if( rec->v != NULL )    /* vertex on cutting line */
    {
      nE = IncidentEdges( rec->v, E );

      match1 = EdgeInsertPosition( rec->v->v, farleft, nE, E, 
                                   &L1, &R1, &cL1, &cR1 );
      cL1 = 2-(cL1-2);  cR1 = 2-(cR1-2);
      
      if( cL1 == 1 )      /* special case: E[L1] below cutting line */
      {
        bh= E[L1];
        er = al = ah = NULL;
        
        if( cR1 == 2 )  
        {
          el = E[R1];
          bl = E[R1]->e[0];
        }
        else
        {
          el = NULL;
          bl = E[R1];
        }
      }
      else if( cL1 == 2 ) /* special case: E[L1] on cutting line */
      {
        er = E[L1];
        al = ah = NULL;

        if( cR1 == 2 )  
        {
          el = E[R1];
          if( el->e[0] == er )
            bl = NULL;
          else
            bl = el->e[0];
        }
        else
        {
          bl = E[R1];
          el = NULL;
        }
        bh = bl;
        if( bh != NULL )
          while( bh->e[1] != er ) bh = bh->e[1];
      }
      else                   /* E[L1] above cutting line */
      {
        ah = E[L1];
        
        if( cR1 == 3 )  /* E[R1] above cutting line */
        {
          el = bl = bh = er = NULL;
          al = E[R1];
        }
        else if( (cR1 == 2) && (!match1) ) /* E[R1] on cutting line     */
        {                                  /* but different orientation */
          el = bl = bh = NULL;
          er = E[R1];
          al = E[R1]->e[1];     /* using edge orientatation and
                                   that cutting line is horizontal ! */
        }
        else
        {
          match2 = EdgeInsertPosition( rec->v->v, farright, nE, E, 
                                       &L2, &R2, &cL2, &cR2 );
          if( match1 )   /* E[R1] on cutting line, same orientation */
          {
            el = E[R1];
            if( cL2 == 1 ) 
            {
              bl = E[R1]->e[0];
              bh = E[L2];
            }
            else
              bl = bh = NULL;
          }
          else
          {
            el = NULL;            
            bl = E[R1];
            bh = E[L2];
          } 
         
          if( cR2 == 2 )  /* R2 on cutting line */
          {
            er = E[R2];
            al = E[R2]->e[1];
          }
          else
          {
            er = NULL;
            al = E[R2];
          }
        }
      }
     
      if( el != NULL )
      {
        fb = el->f[0];
        fa = el->f[1];
      }

      if( al != NULL )
      {
                                      /* create new vertex: */
        v = new graph_vertex( ah->v[0]->v, ah );
        Sa->V.Append(v);
                
        ah->e[0] = al;   /* connect upper edges to new vertex */
        e = al;           
        do
        {
          e->v[0] = v;
          e = e->e[0];
        }
        while( e != al );

        if( el == NULL )
          fa = ah->f[0];
                          /* create new edge */
        e = new graph_edge();
        Sa->E.Append(e);

        e->l = hori;
          
        e->f[0] = fb;
        e->f[1] = fa;

        e->e[0] = al;
        ah->e[0] = e;

        if( lva->e != NULL )
        {
          e->e[1] = lva->e->e[0];
          lva->e->e[0] = e;
        }
        else
        {
          e->e[1] = e;
          lva->e = e;
        }
        v->e = e;
      
        e->v[0] = v;
        e->v[1] = lva;
          
        lva = v;
        nfa = nfb = al->f[1];
      }

      if( bl != NULL )
      {
                                 /* create new vertex: */
        v = new graph_vertex( bl->v[1]->v, bh );
        Sb->V.Append(v);

        bh->e[1] = bl;   /* connect edges below to new vertex */
        e = bl;           
        do
        {
          Sa->E.Remove(e);
          Sb->E.Append(e);
          e->v[1] = v;
          e = e->e[1];
        }
        while( e != bl );

        if( el == NULL )
          fb = bl->f[0];
                                 /* create new edge */
        e = new graph_edge();
        Sb->E.Append(e);

        e->l = hori;
        
        e->f[0] = fb;
        e->f[1] = fa;

        e->e[0] = bl;
        bh->e[1] = e;

        if( lvb->e != NULL )
        {
          e->e[1] = lvb->e->e[1];
          lvb->e->e[1] = e;
        }
        else
        {
          e->e[1] = e;
          lvb->e = e;
        } 
      
        e->v[0] = v;
        e->v[1] = lvb;
        
        lvb = v;
        nfa = nfb = bh->f[1];
      }
        
      if( er != NULL )
      {
        fa = er->f[1];
        fb = er->f[0];
      }
      else
      {
        fa = nfa;
        fb = nfb;
      }

      Sa->V.Remove( rec->v );            /* remove original vertex */
      delete delv;               /* delayed deletion of vertex from original graph */
      delv = rec->v;
      
      if( el != NULL ) {          /* remove original horizontal edge */
        Sa->E.Remove(el);
        delete el;
      }
    }
    else           /* cutting line intersects edge */
    {
      ew = rec->e;
                                  /* new vertex for strip above cutting line */
      vu = new graph_vertex( PrecPoint2D(rec->val,Sa->val), NULL );
      Sa->V.Append(vu);
                                /* new vertex for strip below cutting line */
      vl = new graph_vertex( vu->v, NULL );
      Sb->V.Append(vl);
                            /* new edge for upper halve of intersecting edge */
      eu = new graph_edge();
      Sa->E.Append(eu);

      eu->l = ew->l;

      eu->f[0] = ew->f[0];
      eu->f[1] = ew->f[1];
      eu->v[0] = vu;
      eu->v[1] = ew->v[1];

      eu->e[0] = NULL;
      if( ew->e[1] != ew )
      {
        eu->e[1] = ew->e[1];
        nE = EdgeCycle( ew, ew->v[1], E ); e = E[nE-1];
        if( e->e[0] == ew ) e->e[0] = eu; else e->e[1] = eu;
      }
      else eu->e[1] = eu;
      ew->v[1]->e = eu;
        
                     /* shorten intersecting edge and add it to lower strip */
      Sa->E.Remove(ew);
      Sb->E.Append(ew);

      ew->v[1] = vl;
      ew->e[1] = NULL;
      
      vu->e = eu;         /* set vertex pointers to an incident edge */
      vl->e = ew;
                              /* create new horizontal edge for upper strip */
      e = new graph_edge();
      Sa->E.Append(e);

      e->l = hori;
        
      e->f[0] = e->f[1] = ew->f[0]; /* same face below and above */

      e->e[0] = eu;
      eu->e[0] = e;

      if( lva->e != NULL )
      {
        e->e[1] = lva->e->e[0];
        lva->e->e[0] = e;
      }
      else
      {
        e->e[1] = e;
        lva->e = e;
      }
      vu->e = e;
      
      e->v[0] = vu;
      e->v[1] = lva;
                              /* create new horizontal edge for lower strip */
      e = new graph_edge();
      Sb->E.Append(e);
      
      e->l = hori;
      
      e->f[0] = e->f[1] = ew->f[0]; /* same face below and above */

      e->e[0] = ew;
      ew->e[1] = e;

      if( lvb->e != NULL )
      {
        e->e[1] = lvb->e->e[1];
        lvb->e->e[1] = e;
      }
      else
      {
        e->e[1] = e;
        lvb->e = e;
      }
      vl->e = ew;
      
      e->v[0] = vl;
      e->v[1] = lvb;

      lva = vu;
      lvb = vl;
      fa = fb = ew->f[1];
    }

    rec = rec->next;
  }
  delete delv;
                                  /* new vertex for strip above cutting line */
  v = new graph_vertex( PrecPoint2D(maxx,Sa->val), NULL );
  *maxSaV = v;
  Sa->V.Append(v);
                               /* create new horizontal edge for upper strip */
  e = new graph_edge();
  Sa->E.Append(e);
  
  e->l = hori;
    
  e->f[0] = fb;
  e->f[1] = fa;

  e->e[0] = e;

  if( lva->e != NULL )
  {
    e->e[1] = lva->e->e[0];
    lva->e->e[0] = e;
  }
  else
  {
    e->e[1] = e;
    lva->e = e;
  }
  v->e = e;
      
  e->v[0] = v;
  e->v[1] = lva;
                                  /* new vertex for strip below cutting line */
  v = new graph_vertex( v->v, NULL );
  *maxSbV = v;
  Sb->V.Append(v);
                                /* create new horizontal edge for lower strip */
  e = new graph_edge();
  Sb->E.Append(e);

  e->l = hori;
  
  e->f[0] = fb;
  e->f[1] = fa;

  e->e[0] = e;

  if( lvb->e != NULL )
  {
    e->e[1] = lvb->e->e[1];
    lvb->e->e[1] = e;
  }
  else
  {
    e->e[1] = e;
    lvb->e = e;
  } 
  v->e = e;
      
  e->v[0] = v;
  e->v[1] = lvb; 
}



/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

  Functions for splitting a graph into vertical strips

++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/* -------------------------------------------------------------------------
  Calculates the intersection of a graph with vertical cutting lines 
-------------------------------------------------------------------------- */

void IntersectWithVerticals( 
          graph_edge_list *E, graph_vertex_list *V,
          const PrecRational &deltax,
          const int nS, Ptr< cut_info > S )
{
  int ns = nS-1, i,j,k,ii,ik,sign;
  graph_vertex *v,*v_next;
  graph_edge *e, *e_next;
  PrecRational y,t,q,r,x0,y0,x,dx,dy;
  bool rz;
  cut_record *rec;

  v = V->head;

  while( v != NULL )
  {
    v_next = v->next;

    x = v->v.v().m_x;
  
    t = (x-S[1].val)/deltax;
    t.DivMod( &q, &r ); 
    i = ((int)q) + 1;  
    if( i <= 0 ) { i = 0; rz = false; }
    else if( i > ns ) { i = ns; rz = false; }
    else { rz = r.IsZero(); }
  
    v->reserved[0] = (void *)i;
    v->reserved[1] = (void *)rz;
    
    V->Remove( v );
    S[i].V.Append( v );

    if( rz )
    { 
      rec = new cut_record();
      S[i].R.Append( rec );
           
      rec->v = v;
      rec->e = NULL;
      rec->val = v->v.v().m_y;
    }

    v = v_next;
  }

  e = E->head;

  while( e != NULL )  
  {
    e_next = e->next;

    sign = Compare(e->v[1]->v.v().m_x, e->v[0]->v.v().m_x);
    if( sign >= 0 ) {
      ii = 0;
      ik = 1;
    } else {
      ii = 1;
      ik = 0;
    }    

    i = (int)(e->v[ii]->reserved[0]);
    k = (int)(e->v[ik]->reserved[0]);

    E->Remove( e );
    S[k].E.Append( e );

    if( i !=  k )
    {
      if( e->l.IsNULL() )
      {
        e->CalcLine();
      }

      x0 = e->l.v().m_x;
      y0 = e->l.v().m_y;
    
      dy = e->l.dy();

      if( (int)e->v[ik]->reserved[1] ) k--;

      for( j = i+1; j <= k; j++ )
      {
        if( !dy.IsZero() )
          y = y0 + (S[j].val-x0)*dy;
        else
          y = y0;
          
        rec = new cut_record();
        S[j].R.Append( rec );
              
        rec->v = NULL;
        rec->e = e;          
        rec->val = y;
      }
    }

    e = e_next;
  }

  sort_cut_records( ns, S );
}

/* -------------------------------------------------------------------------
  Divide graph into an independant left and right part
-------------------------------------------------------------------------- */
 
void DivideVerticalStrips( 
              cut_info *Sb, cut_info *Sa, int maxE,
              const PrecRational& miny, const PrecRational maxy,
              const PrecRational& far_bottom, const PrecRational& far_top,
              graph_vertex **minSbV, graph_vertex **minSaV,
              graph_vertex **maxSbV, graph_vertex **maxSaV )
{
  graph_edge **E;
  cut_record *rec;
  int nE,L1,L2,R1,R2,match1,match2,cL1,cR1,cL2,cR2,iv,sign,ew_ori;
  vertex farbottom,fartop;
  graph_edge *ah,*el,*bl,*bh,*er,*al, *e, *eu, *ew;
  graph_vertex *v, *lva, *lvb, *vu, *vl, *delv;
  int fa,fb,nfa,nfb;
  line verti;

  farbottom = vertex( PrecPoint2D(Sb->val,far_bottom) );
  fartop = vertex( PrecPoint2D(Sb->val,far_top) );
  verti = line( farbottom.v(), PrecRational(), PrecRational(ULong(1)) );
 
  E = (graph_edge **)alloca( sizeof(graph_edge *)*maxE );
  if( E == NULL ) throw AllocError();
  
  rec = Sb->R.head;
  fa=fb=nfa=nfb=0;

                                /* new vertex for strip left from cutting line*/
  v = *minSaV = new graph_vertex( PrecPoint2D(Sb->val,miny), NULL );
  Sa->V.Append(v);
  lva = v;
                              /* new vertex for strip right from cutting line */
  v = *minSbV = new graph_vertex(lva->v, NULL);     
  Sb->V.Append(v);
  lvb = v;

  delv = 0;  
  while( rec != NULL )
  {
    if( rec->v != NULL )    /* vertex on cutting line */
    {
      nE = IncidentEdges( rec->v, E );

      match1 = EdgeInsertPosition( rec->v->v, farbottom, nE, E, 
                                   &L1, &R1, &cL1, &cR1 );
      cL1 = 2-(cL1-2);  cR1 = 2-(cR1-2);
     
      if( cL1 == 1 )      /* special case: E[L1] left from cutting line */
      {
        bh = E[L1];
        er = al = ah = NULL;
        
        if( cR1 == 2 )  
        {
          el = E[R1];
          bl = E[R1]->e[1];
        }
        else
        {
          el = NULL;
          bl = E[R1];
        }
      }
      else if( cL1 == 2 ) /* special case: E[L1] on cutting line */
      {
        er = E[L1];
        al = ah = NULL;
        
        if( cR1 == 2 )  
        {
          el = E[R1];
          if( el->e[1] == er )
            bl = NULL;
          else
            bl = el->e[1];
        }
        else
        {
          bl = E[R1];
          el = NULL;
        }        
        bh = bl;
        if( bh != NULL )
        {
          e = (bh->v[0] == rec->v ? bh->e[0] : bh->e[1]);
          while( e != er ) 
          {
            bh = e;
            e = (bh->v[0] == rec->v ? bh->e[0] : bh->e[1]);
          }
        }
      }
      else    /* E[L1] right from cutting line */
      {
        ah = E[L1];
        
        if( cR1 == 3 )  /* E[R1] right from cutting line */
        {
          el = bl = bh = er = NULL;
          al = E[R1];
        }
        else if( (cR1 == 2) && (!match1) ) /* E[R1] on cutting line     */
        {                                  /* but different orientation */
          el = bl = bh = NULL;
          er = E[R1];
          al = E[R1]->e[0];     /* using edge orientatation and
                                   that cutting line is vertical ! */
        }
        else
        {
          match2 = EdgeInsertPosition( rec->v->v, fartop, nE, E, 
                                       &L2, &R2, &cL2, &cR2 );
          if( match1 )   /* E[R1] on cutting line, same orientation */
          {
            el = E[R1];
            if( cL2 == 1 ) 
            {
              bl = E[R1]->e[1];
              bh = E[L2];
            }
            else
              bl = bh = NULL;
          }
          else
          {
            el = NULL;            
            bl = E[R1];
            bh = E[L2];
          } 
         
          if( cR2 == 2 )  /* R2 on cutting line */
          {
            er = E[R2];
            al = E[R2]->e[0];
          }
          else
          {
            er = NULL;
            al = E[R2];
          }
        }
      }
     
      if( el != NULL )
      {
        fb = el->f[1];
        fa = el->f[0];
      }

      if( al != NULL )
      {
                                      /* create new vertex: */
        iv = (ah->v[0] == rec->v ? 0 : 1); 
        v = new graph_vertex( rec->v->v, ah );
        Sa->V.Append(v);
                
        ah->e[iv] = al;   /* connect edges to the left to new vertex */
        e = al;           
        do
        {
          Sb->E.Remove(e);
          Sa->E.Append(e);
          if( e->v[0] == rec->v )
          { e->v[0] = v;
            e = e->e[0];
          } else
          { e->v[1] = v;
            e = e->e[1];
          }
        }
        while( e != al );

        if( el == NULL )
          fa = ah->f[iv];
                          /* create new edge */
        e = new graph_edge();
        Sa->E.Append(e);

        e->l = verti;
          
        e->f[0] = fa;
        e->f[1] = fb;

        e->e[1] = al;
        ah->e[iv] = e;

        if( lva->e != NULL )
        {
          if( lva->e->v[0] == lva ) {
            e->e[0] = lva->e->e[0];
            lva->e->e[0] = e;
          } else {
            e->e[0] = lva->e->e[1];
            lva->e->e[1] = e;
          }
        }
        else
        {
          e->e[0] = e;
          lva->e = e;
        }
        v->e = e;
      
        e->v[1] = v;
        e->v[0] = lva;
          
        lva = v;
        nfa = nfb = (al->v[0] == v ? al->f[1] : al->f[0]);
      }

      if( bl != NULL )
      {
                                 /* create new vertex: */
        v = new graph_vertex( rec->v->v, bh );
        Sb->V.Append(v);
                            /* connect edges on the right to new vertex */
        if( bh->v[0] == rec->v )
          bh->e[0] = bl;   
        else        
          bh->e[1] = bl; 
        e = bl;           
        do
        {
          if( e->v[0] == rec->v )
          { e->v[0] = v;
            e = e->e[0];       
          } else {
            e->v[1] = v;
            e = e->e[1];
          }
        }
        while( e != bl );

        if( el == NULL )
          if( bl->v[1] == v )
            fb = bl->f[0];
          else
            fb = bl->f[1];
                                 /* create new edge */
        e = new graph_edge();
        Sb->E.Append(e);

        e->l = verti;
        
        e->f[0] = fa;
        e->f[1] = fb;

        e->e[1] = bl;
        if( bh->v[0] == v ) bh->e[0] = e; else bh->e[1] = e;

        if( lvb->e != NULL )
        {
          if( lvb->e->v[0] == lvb ) {
            e->e[0] = lvb->e->e[0];
            lvb->e->e[0] = e;
          } else {
            e->e[0] = lvb->e->e[1];
            lvb->e->e[1] = e;
          }
        }
        else
        {
          e->e[0] = e;
          lvb->e = e;
        } 
      
        e->v[1] = v;
        e->v[0] = lvb;
        
        lvb = v;
        nfa = nfb = (bh->v[1] == v ? bh->f[1] : bh->f[0]);
      }

      Sb->V.Remove( rec->v );            /* remove original vertex */
      delete delv;                /* delayed deletion of vertex from original graph */
      delv = rec->v;
        
      if( er != NULL )
      {
        fa = er->f[0];
        fb = er->f[1];
      }
      else
      {
        fa = nfa;
        fb = nfb;
      } 

      if( el != NULL ) {                 /* remove original vertical edge */
        Sb->E.Remove(el); 
        delete el;
      }
    }
    else           /* cutting line intersects edge */
    {
      ew = rec->e;
                              /* new vertex for strip right from cutting line */
      vu = new graph_vertex( PrecPoint2D(Sb->val,rec->val), NULL );
      Sb->V.Append(vu);
                             /* new vertex for strip left from cutting line */
      vl = new graph_vertex( vu->v, NULL );
      Sa->V.Append(vl);
                            /* new edge for right halve of intersecting edge */
      eu = new graph_edge();
      Sb->E.Append(eu);

      eu->l = ew->l;

      eu->f[0] = ew->f[0];
      eu->f[1] = ew->f[1];

      sign = Compare(ew->v[1]->v.v().m_x, ew->v[0]->v.v().m_x);
      if( sign > 0 ) ew_ori = 1; else ew_ori = 0; 

      if( ew_ori )                    /*** ew oriented from left to right ***/
      {
        eu->v[0] = vu;
        eu->v[1] = ew->v[1];

        eu->e[0] = NULL;
        if( ew->e[1] != ew )
        {
          eu->e[1] = ew->e[1];
          nE = EdgeCycle( ew, ew->v[1], E ); e = E[nE-1];
          if( e->e[0] == ew ) e->e[0] = eu; else e->e[1] = eu;
        }
        else eu->e[1] = eu;
        ew->v[1]->e = eu;

                     /* shorten intersecting edge and add it to left strip */
        Sb->E.Remove(ew);
        Sa->E.Append(ew);

        ew->v[1] = vl;
        ew->e[1] = NULL;
      
        vu->e = eu;         /* set vertex pointers to an incident edge */
        vl->e = ew;
                              /* create new vertical edge for right strip */
        e = new graph_edge();
        Sb->E.Append(e);

        e->l = verti;
        
        e->f[0] = e->f[1] = ew->f[1]; /* same face on left and right side */
                                      /* of edge */
        eu->e[0] = e;
      }
      else                            /*** ew oriented from right to left ***/
      {
        eu->v[1] = vu;
        eu->v[0] = ew->v[0];

        eu->e[1] = NULL;
        if( ew->e[0] != ew )
        {
          eu->e[0] = ew->e[0];
          nE = EdgeCycle( ew, ew->v[0], E ); e = E[nE-1];
          if( e->e[0] == ew ) e->e[0] = eu; else e->e[1] = eu;
        }
        else eu->e[0] = eu;
        ew->v[0]->e = eu;

                     /* shorten intersecting edge and add it to lower strip */
        Sb->E.Remove(ew);
        Sa->E.Append(ew);

        ew->v[0] = vl;
        ew->e[0] = NULL;
      
        vu->e = eu;         /* set vertex pointers to an incident edge */
        vl->e = ew;
                              /* create new vertical edge for right strip */
        e = new graph_edge();
        Sb->E.Append(e);

        e->l = verti;
        
        e->f[0] = e->f[1] = ew->f[0]; /* same face on left and right side */
                                      /* of edge */
        eu->e[1] = e;
      } 

      e->e[1] = eu;

      if( lvb->e != NULL )
      {
        if( lvb->e->v[0] == lvb ) {
          e->e[0] = lvb->e->e[0];
          lvb->e->e[0] = e;
        } else {
          e->e[0] = lvb->e->e[1];
          lvb->e->e[1] = e;
        }
      }
      else
      {
        e->e[0] = e;
        lvb->e = e;
      }
      vu->e = eu;

      e->v[1] = vu;
      e->v[0] = lvb;
                              /* create new vertical edge for left strip */
      e = new graph_edge();
      Sa->E.Append(e);
      
      e->l = verti;
      
      if( ew_ori )                  /*** ew oriented from left to right ***/
      {
        e->f[0] = e->f[1] = ew->f[1]; /* same face on left and right side */

        e->e[1] = ew;
        ew->e[1] = e;

        fa = fb = ew->f[0];      /* for next intersection */
      }
      else                          /*** ew oriented from left to right ***/
      {
        e->f[0] = e->f[1] = ew->f[0]; /* same face on left and right side */

        e->e[1] = ew;
        ew->e[0] = e;

        fa = fb = ew->f[1];       /* for next intersection */
      }
      
      if( lva->e != NULL )
      {
        if( lva->e->v[0] == lva ) {
          e->e[0] = lva->e->e[0];
          lva->e->e[0] = e;
        } else {
          e->e[0] = lva->e->e[1];
          lva->e->e[1] = e;
        }
      }
      else
      {
        e->e[0] = e;
        lva->e = e;
      }
      vl->e = e;
      
      e->v[1] = vl;
      e->v[0] = lva;

      lvb = vu;
      lva = vl;
    }

    rec = rec->next;
  }
  delete delv;
                               /* new vertex for strip left from cutting line */
  v = new graph_vertex( PrecPoint2D(Sb->val,maxy), NULL );
  *maxSaV = v;
  Sa->V.Append(v);
                               /* create new vertical edge for left strip */
  e = new graph_edge();
  Sa->E.Append(e);
  
  e->l = verti;
    
  e->f[0] = fa;
  e->f[1] = fb;

  e->e[1] = e;

  if( lva->e != NULL )
  {
    if( lva->e->v[0] == lva ) {
      e->e[0] = lva->e->e[0];
      lva->e->e[0] = e;
    } else {
      e->e[0] = lva->e->e[1];
      lva->e->e[1] = e;
    }
  }
  else
  {
    e->e[0] = e;
    lva->e = e;
  }
  v->e = e;
      
  e->v[1] = v;
  e->v[0] = lva;
                             /* new vertex for strip right from cutting line */
  v = new graph_vertex( v->v, NULL );
  *maxSbV = v;
  Sb->V.Append(v);
                                /* create new vertical edge for right strip */
  e = new graph_edge();
  Sb->E.Append(e);

  e->l = verti;
  
  e->f[0] = fa;
  e->f[1] = fb;

  e->e[1] = e;

  if( lvb->e != NULL )
  {
    if( lvb->e->v[0] == lvb ) {
      e->e[0] = lvb->e->e[0];
      lvb->e->e[0] = e;
    } else {
      e->e[0] = lvb->e->e[1];
      lvb->e->e[1] = e;
    }
  }
  else
  {
    e->e[0] = e;
    lvb->e = e;
  } 
  v->e = e;
  
  e->v[1] = v;
  e->v[0] = lvb; 
}

/* ---------------------------------------------------------------------------
  splits a graph into four independent parts
---------------------------------------------------------------------------- */
template< class T >
void SplitGraph( GraphInfo *G, T xmed, T ymed,
                 GraphConvInfo<T> *Gconv, GraphInfo4& sub )
{
  graph_vertex *Vy1Sl, *Vy1Sr, *Vy2Sl, *Vy2Sr, *Vx1Sa, *Vx1Sb, *Vx2Sa, *Vx2Sb;
  graph_vertex *hvLeftT, *hvRightT;
  graph_edge *e, *el, **hE;
  graph_vertex *v, *v0;
  int nhE, nMaxE, i, j;
  PrecRational X1,X2,X3,DeltaX;
  PrecRational Y1,Y2,Y3,DeltaY;
  unsigned long *cbuf = (unsigned long *)alloca(gul::rtr<double>::mantissa_length());
  T y2,x2;
  Ptr< gugr::cut_info > CutsX, CutsY;

  /*
  cout << "input graph\n";
  cout << "***********\n";
  gugr::Dump<T>::dump_vertices( G->V.head );
  gugr::Dump<T>::dump_edges( G->E.head );
  */

  PrecRational& MinX = Gconv->MinX;    // short aliases
  PrecRational& MaxX = Gconv->MaxX;
  PrecRational& FarMinX = Gconv->FarMinX;
  PrecRational& FarMaxX = Gconv->FarMaxX;
  PrecRational& MinY = Gconv->MinY;
  PrecRational& MaxY = Gconv->MaxY;
  PrecRational& FarMinY = Gconv->FarMinY;
  PrecRational& FarMaxY = Gconv->FarMaxY;
 
  CutsX.reserve_place( reserve_stack(cut_info,4), 4 );
  CutsY.reserve_place( reserve_stack(cut_info,4), 4 );

  x2 = (xmed - Gconv->minx)/Gconv->scale;
  X2 = PrecRational(coord2int(x2,cbuf),cbuf);
  y2 = (ymed - Gconv->miny)/Gconv->scale;
  Y2 = PrecRational(coord2int(y2,cbuf),cbuf);

  CutsX[1].val = X1 = G->P[0][0]->v.v().m_x;
  CutsX[2].val = X2;
  CutsX[3].val = X3 = G->P[0][1]->v.v().m_x;

  CutsY[1].val = Y1 = G->P[0][0]->v.v().m_y;
  CutsY[2].val = Y2;
  CutsY[3].val = Y3 = G->P[1][0]->v.v().m_y;

  DeltaX = X2-X1;
  DeltaY = Y2-Y1;

  nMaxE = 10*G->E.nElems;      // this is a hack, but its enough even in the
                               // in the worst case
  hE = (graph_edge **)alloca( sizeof(graph_edge *) * nMaxE );
  
                                           // split graph into vertical strips
  IntersectWithVerticals( &G->E, &G->V, DeltaX, 4, CutsX );

                                // isolate first vertical strip and discard it
  DivideVerticalStrips( &CutsX[3], &CutsX[2], nMaxE,
                        MinY, MaxY, FarMinY, FarMaxY,
                        &Vy1Sr, &Vy1Sl, &Vy2Sr, &Vy2Sl );  

  /*
  cout << "after disconnecting X-Strip " << 3 << " and " << 2 << "\n";
  cout << "*******************************************************\n";
  for( int k = 3; k >= 0; k-- )
  {
    cout << "X-Strip " << k << "\n";
    gugr::Dump<T>::dump_vertices( CutsX[k].V.head );
    gugr::Dump<T>::dump_edges( CutsX[k].E.head );
  }
  */

  CutsX[3].R.DeleteElems();
  CutsX[3].E.DeleteElems();
  CutsX[3].V.DeleteElems();

  for( i = 2; i > 0; i-- )
  {
    DivideVerticalStrips( &CutsX[i], &CutsX[i-1], nMaxE,
                          MinY, MaxY, FarMinY, FarMaxY,
                          &Vy1Sr, &Vy1Sl, &Vy2Sr, &Vy2Sl );

    /*
    cout << "after disconnecting X-Strip " << i << " and " << i-1 << "\n";
    cout << "*********************************************************\n";
    for( int k = i; k >= 0; k-- )
    {
      cout << "X-Strip " << k << "\n";
      gugr::Dump<T>::dump_vertices( CutsX[k].V.head );
      gugr::Dump<T>::dump_edges( CutsX[k].E.head );
    }
    */
                               // split vertical strips into horizontal strips 
    IntersectWithHorizontals( &CutsX[i].E, &CutsX[i].V, DeltaY, 4, CutsY );

                                         // isolate first strip and discard it
    DivideHorizontalStrips( &CutsY[3], &CutsY[2], nMaxE,
                            MinX, MaxX, FarMinX, FarMaxX,
                            &Vx1Sa, &Vx1Sb, &Vx2Sa, &Vx2Sb );   

    /*
    cout << "after disconnecting Y-Strip " << 3 << " and " << 2 << "\n";
    cout << "*******************************************************\n";
    for( int k = 3; k >= 0; k-- )
    {
      cout << "Y-Strip " << k << "\n";
      gugr::Dump<T>::dump_vertices( CutsY[k].V.head );
      gugr::Dump<T>::dump_edges( CutsY[k].E.head );
    }
    */
  
    CutsY[3].R.DeleteElems();
    CutsY[3].E.DeleteElems();
    CutsY[3].V.DeleteElems();

    hvLeftT = Vx1Sb; hvRightT = Vx2Sb;  // for next strip
    
                               // divide vertical strips into horizontal strips    
    for( j = 2; j > 0; j-- )
    {
      DivideHorizontalStrips( &CutsY[j], &CutsY[j-1], nMaxE,
                              MinX, MaxX, FarMinX, FarMaxX,
                              &Vx1Sa, &Vx1Sb, &Vx2Sa, &Vx2Sb );

      /*
      cout << "after disconnecting Y-Strip " << j << " and " << j-1 << "\n";
      cout << "*********************************************************\n";
      for( int k = j; k >= 0; k-- )
      {
        cout << "Y-Strip " << k << "\n";
        gugr::Dump<T>::dump_vertices( CutsY[k].V.head );
        gugr::Dump<T>::dump_edges( CutsY[k].E.head );
      }
      */
                              // remove help edges from the corner edge cycles
      v0 = hvLeftT;
      e = v0->e;
      sub[2*(j-1)+i-1]->face = e->e[0]->f[1]; // face index of quad, if it is
      v = e->v[0];                            // not divided
      sub[2*(j-1)+i-1]->P[1][0] = v;
      nhE = EdgeCycle( e, v, hE );
      el = hE[nhE-1];
      if( el->v[0] == v ) el->e[0] = e->e[0]; else el->e[1] = e->e[0];
      v->e = el;
      CutsY[j].V.Remove(v0); CutsY[j].E.Remove(e);
      delete v0; delete e;
      
      v0 = hvRightT;
      e = v0->e;
      v = e->v[1];
      sub[2*(j-1)+i-1]->P[1][1] = v;
      nhE = EdgeCycle( e, v, hE );
      el = hE[nhE-1];
      if( el->v[0] == v ) el->e[0] = e->e[1]; else el->e[1] = e->e[1];
      v->e = el;
      CutsY[j].V.Remove(v0); CutsY[j].E.Remove(e);
      delete v0; delete e;
 
      v0 = Vx2Sa;
      e = v0->e;
      v = e->v[1];
      sub[2*(j-1)+i-1]->P[0][1] = v;
      nhE = EdgeCycle( e, v, hE );
      el = hE[nhE-1];
      if( el->v[0] == v ) el->e[0] = e->e[1]; else el->e[1] = e->e[1];
      v->e = el;
      CutsY[j].V.Remove(v0); CutsY[j].E.Remove(e);
      delete v0; delete e;
 
      v0 = Vx1Sa;
      e = v0->e;
      v = e->v[0];
      sub[2*(j-1)+i-1]->P[0][0] = v;
      nhE = EdgeCycle( e, v, hE );
      el = hE[nhE-1];
      if( el->v[0] == v ) el->e[0] = e->e[0]; else el->e[1] = e->e[0];
      v->e = el;
      CutsY[j].V.Remove(v0); CutsY[j].E.Remove(e);
      delete v0; delete e;
    
      hvLeftT = Vx1Sb;  hvRightT = Vx2Sb;   // for next loop

      CutsY[j].R.DeleteElems();             // free cut records only
      sub[2*(j-1)+i-1]->E = CutsY[j].E;
      sub[2*(j-1)+i-1]->V = CutsY[j].V;
      CutsY[j].E.ReleaseElems();            // clear for next strip
      CutsY[j].V.ReleaseElems();
    }

    CutsY[0].R.DeleteElems();
    CutsY[0].E.DeleteElems();
    CutsY[0].V.DeleteElems();

    CutsX[i].R.DeleteElems();
    CutsX[i].E.DeleteElems();
    CutsX[i].V.DeleteElems();
  }
  
  CutsX[0].R.DeleteElems();
  CutsX[0].E.DeleteElems();
  CutsX[0].V.DeleteElems();
}
template 
void SplitGraph( GraphInfo *G, float xmed, float ymed,
                 GraphConvInfo<float> *Gconv, GraphInfo4& sub );
template 
void SplitGraph( GraphInfo *G, double xmed, double ymed,
                 GraphConvInfo<double> *Gconv, GraphInfo4& sub );

/* ---------------------------------------------------------------------------
  inits a graph info
---------------------------------------------------------------------------- */
template< class T >
void InitGraph( 
       const PrecRational& X1, const PrecRational& X2,
       const PrecRational& Y1, const PrecRational& Y2,
       GraphConvInfo<T> *Gconv, graph_edge_list *E, graph_vertex_list *V,
       GraphInfo *G )
{

  graph_vertex *Vy1Sl, *Vy1Sr, *Vy2Sl, *Vy2Sr, *Vx1Sa, *Vx1Sb, *Vx2Sa, *Vx2Sb;
  graph_vertex *hvLeftT, *hvRightT;
  graph_edge *e, *el, **hE;
  graph_vertex *v, *v0;
  int nhE, nMaxE;
  PrecRational DeltaX, DeltaY;
  Ptr< cut_info > CutsX, CutsY;

  PrecRational& MinX = Gconv->MinX;    // short aliases
  PrecRational& MaxX = Gconv->MaxX;
  PrecRational& FarMinX = Gconv->FarMinX;
  PrecRational& FarMaxX = Gconv->FarMaxX;
  PrecRational& MinY = Gconv->MinY;
  PrecRational& MaxY = Gconv->MaxY;
  PrecRational& FarMinY = Gconv->FarMinY;
  PrecRational& FarMaxY = Gconv->FarMaxY;
 
  CutsX.reserve_place( reserve_stack(cut_info,3), 3 );
  CutsY.reserve_place( reserve_stack(cut_info,3), 3 );

  CutsX[1].val = X1;
  CutsX[2].val = X2;

  CutsY[1].val = Y1;
  CutsY[2].val = Y2;

  DeltaX = X2-X1;
  DeltaY = Y2-Y1;

  nMaxE = 10 * E->nElems;      // this is a hack, but its enough even in the
                               // worst case
  hE = (graph_edge **)alloca( sizeof(graph_edge *) * nMaxE );
  
  /*
  cout << "input graph\n";
  cout << "***********\n";
  gugr::Dump<T>::dump_vertices( V->head );
  gugr::Dump<T>::dump_edges( E->head );
  */
                                           // split graph into vertical strips
  IntersectWithVerticals( E, V, DeltaX, 3, CutsX );

                                // isolate first vertical strip and discard it
  DivideVerticalStrips( &CutsX[2], &CutsX[1], nMaxE,
                        MinY, MaxY, FarMinY, FarMaxY,
                        &Vy1Sr, &Vy1Sl, &Vy2Sr, &Vy2Sl );  
  /*
  cout << "after disconnecting X-Strip " << 2 << " and " << 1 << "\n";
  cout << "*******************************************************\n";
  for( int k = 2; k >= 0; k-- )
  {
    cout << "X-Strip " << k << "\n";
    gugr::Dump<T>::dump_vertices( CutsX[k].V.head );
    gugr::Dump<T>::dump_edges( CutsX[k].E.head );
  }
  */

  CutsX[2].R.DeleteElems();
  CutsX[2].E.DeleteElems();
  CutsX[2].V.DeleteElems();

  DivideVerticalStrips( &CutsX[1], &CutsX[0], nMaxE,
                        MinY, MaxY, FarMinY, FarMaxY,
                        &Vy1Sr, &Vy1Sl, &Vy2Sr, &Vy2Sl );
  /*
  cout << "after disconnecting X-Strip " << 1 << " and " << 0 << "\n";
  cout << "*******************************************************\n";
  for( int k = 1; k >= 0; k-- )
  {
    cout << "X-Strip " << k << "\n";
    gugr::Dump<T>::dump_vertices( CutsX[k].V.head );
    gugr::Dump<T>::dump_edges( CutsX[k].E.head );
  }
  */
                               // split vertical strips into horizontal strips 
  IntersectWithHorizontals( &CutsX[1].E, &CutsX[1].V, DeltaY, 3, CutsY );

                                         // isolate first strip and discard it
  DivideHorizontalStrips( &CutsY[2], &CutsY[1], nMaxE,
                          MinX, MaxX, FarMinX, FarMaxX,
                          &Vx1Sa, &Vx1Sb, &Vx2Sa, &Vx2Sb );   
  /*
  cout << "after disconnecting Y-Strip " << 2 << " and " << 1 << "\n";
  cout << "*******************************************************\n";
  for( int k = 2; k >= 0; k-- )
  {
    cout << "Y-Strip " << k << "\n";
    gugr::Dump<T>::dump_vertices( CutsY[k].V.head );
    gugr::Dump<T>::dump_edges( CutsY[k].E.head );
  }
  */
  CutsY[2].R.DeleteElems();
  CutsY[2].E.DeleteElems();
  CutsY[2].V.DeleteElems();

  hvLeftT = Vx1Sb; hvRightT = Vx2Sb;  // for next strip
    
                               // divide vertical strips into horizontal strips
  DivideHorizontalStrips( &CutsY[1], &CutsY[0], nMaxE,
                          MinX, MaxX, FarMinX, FarMaxX,
                          &Vx1Sa, &Vx1Sb, &Vx2Sa, &Vx2Sb );
  /*
  cout << "after disconnecting Y-Strip " << 1 << " and " << 0 << "\n";
  cout << "*******************************************************\n";
  for( int k = 1; k >= 0; k-- )
  {
    cout << "Y-Strip " << k << "\n";
    gugr::Dump<T>::dump_vertices( CutsY[k].V.head );
    gugr::Dump<T>::dump_edges( CutsY[k].E.head );
  }
  */

  v0 = hvLeftT;           // remove help edges from the corner edge cycles
  e = v0->e;
  G->face = e->e[0]->f[1];        // face index of quad, if it is
  v = e->v[0];                    // not divided
  G->P[1][0] = v;
  nhE = EdgeCycle( e, v, hE );
  el = hE[nhE-1];
  if( el->v[0] == v ) el->e[0] = e->e[0]; else el->e[1] = e->e[0];
  v->e = el;
  CutsY[1].V.Remove(v0); CutsY[1].E.Remove(e);
  delete v0; delete e;
      
  v0 = hvRightT;
  e = v0->e;
  v = e->v[1];
  G->P[1][1] = v;
  nhE = EdgeCycle( e, v, hE );
  el = hE[nhE-1];
  if( el->v[0] == v ) el->e[0] = e->e[1]; else el->e[1] = e->e[1];
  v->e = el;
  CutsY[1].V.Remove(v0); CutsY[1].E.Remove(e);
  delete v0; delete e;
 
  v0 = Vx2Sa;
  e = v0->e;
  v = e->v[1];
  G->P[0][1] = v;
  nhE = EdgeCycle( e, v, hE );
  el = hE[nhE-1];
  if( el->v[0] == v ) el->e[0] = e->e[1]; else el->e[1] = e->e[1];
  v->e = el;
  CutsY[1].V.Remove(v0); CutsY[1].E.Remove(e);
  delete v0; delete e;
 
  v0 = Vx1Sa;
  e = v0->e;
  v = e->v[0];
  G->P[0][0] = v;
  nhE = EdgeCycle( e, v, hE );
  el = hE[nhE-1];
  if( el->v[0] == v ) el->e[0] = e->e[0]; else el->e[1] = e->e[0];
  v->e = el;
  CutsY[1].V.Remove(v0); CutsY[1].E.Remove(e);
  delete v0; delete e;

  CutsY[1].R.DeleteElems();             // free cut records only

  G->E = CutsY[1].E;
  G->V = CutsY[1].V;

  CutsY[0].R.DeleteElems();
  CutsY[0].E.DeleteElems();
  CutsY[0].V.DeleteElems();

  CutsX[1].R.DeleteElems();
  CutsX[1].E.DeleteElems();
  CutsX[1].V.DeleteElems();
  
  CutsX[0].R.DeleteElems();
  CutsX[0].E.DeleteElems();
  CutsX[0].V.DeleteElems();
}

/* ---------------------------------------------------------------------------
  inits a graph info
---------------------------------------------------------------------------- */
template
void InitGraph( 
       const PrecRational& X1, const PrecRational& X2,
       const PrecRational& Y1, const PrecRational& Y2,
       GraphConvInfo<float> *Gconv, graph_edge_list *E, graph_vertex_list *V,
       GraphInfo *G );
template
void InitGraph( 
       const PrecRational& X1, const PrecRational& X2,
       const PrecRational& Y1, const PrecRational& Y2,
       GraphConvInfo<double> *Gconv, graph_edge_list *E, graph_vertex_list *V,
       GraphInfo *G );

}
