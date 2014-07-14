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
#include "guma_sorting.h"
#include "gugr_regularize.h"


namespace gugr {

using gul::AllocError;
using guma::BBTNode;
using guma::PtrHeapSort;
using guma::InitBBT;
using guma::InsertElementIntoBBT;
using guma::DeleteElementFromBBT;
using guma::SearchElementInBBT;
using guma::DumpBBTTree;

/* --------------------------------------------------------------------
  Sort vertices. (V2 > V1, if y2 > y1 or x2 < x1 if y2 = y1)
--------------------------------------------------------------------- */

int compare_vertices( void *p1, void *p2, void *data )
{
  const graph_vertex& v1 = *((graph_vertex *)p1);
  const graph_vertex& v2 = *((graph_vertex *)p2);
  int sign;
  
  sign = Compare( v1.v.v().m_y, v2.v.v().m_y );
  if( !sign ) sign = -Compare( v1.v.v().m_x, v2.v.v().m_x );

  return(sign);
}

graph_vertex *SortVertices( graph_vertex_list *V )
{
  graph_vertex **tmpV,*v;
  int i,j;
  
  tmpV = (graph_vertex **)alloca( sizeof(graph_vertex *)*V->nElems );
  if( !tmpV ) throw AllocError();
  
  v = V->head;
  for( i = 0; i < V->nElems; i++ )
  {
    tmpV[i] = v;
    v = v->next;
  }
  
  PtrHeapSort( V->nElems, (void **)tmpV, compare_vertices, NULL );

  V->head = tmpV[0];
  tmpV[0]->prev = NULL;
  for( j = 1; j < V->nElems; j++ )
  {
    tmpV[j-1]->next = tmpV[j];
    tmpV[j]->prev = tmpV[j-1];
  }
  tmpV[V->nElems-1]->next = NULL;

  return(tmpV[V->nElems-1]); /* return last vertex in list */
}

typedef struct
{
  graph_edge   *l,*r;
  graph_vertex *v;
}
edge_interval;

const graph_edge NegInf(0, (graph_vertex *)(-1));
const graph_edge PosInf(0, (graph_vertex *)(+1));
#define EDGE_NEG_INF ((graph_edge *)&NegInf)
#define EDGE_POS_INF ((graph_edge *)&PosInf)

#define IS_INF(e) ( (e).v[0] == NULL )
#define INF_SIGN(e) ( (int)((e).v[1]) )

struct edge_interval_tree
{
  BBTNode root;
  // int icnt,ncnt;

  edge_interval_tree()
  {
    InitBBT(&root);
    // icnt = ncnt = 0;
  }

  BBTNode *NewNode( void )
  {
    BBTNode *node;
    size_t dummy;
    
    node = (BBTNode *)PoolAlloc( sizeof(BBTNode), &dummy );
    if( node == NULL ) throw PoolAllocError();
    node->key = PoolAlloc( sizeof(edge_interval), &dummy );
    if( node->key == NULL ) throw PoolAllocError();

    return(node);
  }

  BBTNode *Head( void ) { return(root.right); }

  void FreeNode( BBTNode *node )
  {
    PoolFree( node->key, sizeof(edge_interval) );
    PoolFree( node, sizeof(BBTNode) );   
  }

  void InsertNode( BBTNode *element, BBTNode *where, int side )
  {
    InsertElementIntoBBT( element, where, side, &root );
  }

  void RemoveNode( BBTNode *element )
  {
    DeleteElementFromBBT( element, &root );
  }

  int SearchNode( void *key, int compare( void *, void * ),
                  BBTNode **element )
  {
    return( SearchElementInBBT(&root, key, compare, element) );
  }

  static void dump_edge_interval( void *p  )
  {
     edge_interval *ei = (edge_interval *)p;
     cout << "key = {l=" << (void *)ei->l << ", r=" << (void *)ei->r <<
          ", v=" << (void *)ei->v << "}";
  }
  /* 
  void Dump()
  {
    std::cout << "EDGE INTERVALLS (" << icnt << ")\n";
    std::cout << "================================\n";
    DumpBBTTree( Head(), 0, dump_edge_interval );
    std::cout << "\n";
    std::cout.flush();
    icnt++;
  }
  void DumpNodes()
  {
    std::cout << "TREE NODES (" << ncnt << ")\n";
    std::cout << "===========================\n";
    DumpBBTNode( Head(), 0 );
    std::cout << "\n";
    std::cout.flush();
    ncnt++;
  }
  */
};


int CompareEdgesBU( const graph_edge *A, const graph_edge *B )
{
  int sign;
  
  if( A == B ) return(0);
  
  if( IS_INF(*A) )
    return( INF_SIGN(*A) );
  if( IS_INF(*B) )
    return( -INF_SIGN(*B) );
  
  if( A->v[0] != B->v[0] ) {
    sign = DetermineOrientation( B->v[0]->v.v(), B->v[1]->v.v(),
                                 A->v[0]->v.v() );
  } else {
    sign = DetermineOrientation( B->v[0]->v.v(), B->v[1]->v.v(),
                                 A->v[1]->v.v() );
  }
  return(-sign);
}
int CompareEdgesTD( const graph_edge *A, const graph_edge *B )
{
  int sign;
  
  if( A == B ) return(0);
  
  if( IS_INF(*A) )
    return( INF_SIGN(*A) );
  if( IS_INF(*B) )
    return( -INF_SIGN(*B) );
  
  if( A->v[1] != B->v[1] ) {
    sign = DetermineOrientation( B->v[0]->v.v(), B->v[1]->v.v(),
                                 A->v[1]->v.v() );
  } else {
    sign = DetermineOrientation( B->v[0]->v.v(), B->v[1]->v.v(),
                                 A->v[0]->v.v() );
  }
  return(-sign);
}

int compare_edge_to_intervalBU( void *p1, void *p2 )
{
 graph_edge *e = (graph_edge *)p1; 
 edge_interval *ei = (edge_interval *)p2;

 if( CompareEdgesBU( e, ei->l ) <= 0 )
   return(-1);
 if( CompareEdgesBU( e, ei->r ) < 0 )
   return(0);

  return(1);
}
int compare_edge_to_intervalTD( void *p1, void *p2 )
{
 graph_edge *e = (graph_edge *)p1; 
 edge_interval *ei = (edge_interval *)p2;

 if( CompareEdgesTD( e, ei->l ) <= 0 )
   return(-1);
 if( CompareEdgesTD( e, ei->r ) < 0 )
   return(0);

  return(1);
}

void EdgeInsert( graph_edge *e, int maxE )
{
  graph_edge *left, *right, **E;
  graph_vertex *v0 = e->v[0], *v1 = e->v[1];
  int ileft, iright, codel, coder, nE;
 
  E = (graph_edge **)alloca( sizeof(graph_edge *) * maxE );
  if( E == NULL ) throw AllocError();
 
  nE = IncidentEdges( v0, E );
  
  EdgeInsertPosition( v0->v, v1->v, nE, E, &ileft, &iright,
                      &codel, &coder );
  left = E[ileft];  right = E[iright];

  if( left->v[0] == v0 )
    left->e[0] = e;
  else
    left->e[1] = e;

  e->e[0] = right; 

  if( left->v[0] == v0 )
    e->f[1] = left->f[0];
  else
    e->f[1] = left->f[1];

  if( right->v[0] == v0 )
    e->f[0] = right->f[1];
  else
    e->f[0] = right->f[0];

  nE = IncidentEdges( v1, E );

  EdgeInsertPosition( v1->v, v0->v, nE, E, &ileft, &iright,
                      &coder, &codel );
  left = E[ileft];  right = E[iright];

  if( left->v[0] == v1 )
    left->e[0] = e;
  else
    left->e[1] = e;

  e->e[1] = right; 
}

void Regularize( graph_edge_list *E, graph_vertex_list *V )
{
  edge_interval_tree Status;
  BBTNode *node,*node1,*node2;
  edge_interval *I;
  graph_edge **TE, *edge,*a,*r;
  graph_vertex *v0,*vmax,*vmin,*v;
  int nTE,sign,regular,j,side;
 
  TE = (graph_edge **)alloca( sizeof(graph_edge *)*(3*E->nElems) );
  if( !TE ) throw AllocError();

  vmax = SortVertices( V );
  vmin = V->head;

/*--- tree of intervalls bounded by two edges for status info -------------*/ 

  node = Status.NewNode();
  
  I = (edge_interval *)node->key;
  I->l = EDGE_NEG_INF;
  I->r = EDGE_POS_INF;
  I->v = NULL;
  
  Status.InsertNode( node, &Status.root, 1 );
  // Status.Dump(); Status.DumpNodes();
  
/****************** Top-Down Sweep **********************************/

  v0 = vmax;
  while( v0 != NULL )
  {
    nTE = IncidentEdges( v0, TE );

    regular = 0;
    for( j = 0; j < nTE; j++ )
    {
      a = TE[j];
      if( a->v[0] == v0 )  /* edge starts in v0 (then v0 cannot be maximum */
      {                    /* of graph) */
        regular = 1;
    
        sign = Compare( a->v[1]->v.v().m_y, v0->v.v().m_y );
        if( sign == 0 ) continue; /* nothing to do for horizontal edges */

                        /* replace the two intervalls [l,a],[a,r] with [l,r] */
        node1 = (BBTNode *)a->reserved[0];
        node2 = (BBTNode *)a->reserved[1];        

        edge = ((edge_interval *)(node2->key))->r;
        edge->reserved[0] = (void *)node1;

        ((edge_interval *)(node1->key))->r = edge;        
        ((edge_interval *)(node1->key))->v = v0;
        
        Status.RemoveNode( node2 );
        Status.FreeNode( node2 );
        // Status.Dump(); Status.DumpNodes();

      }
    }

    if( (!regular) && (v0 != vmax) )
    {
      side = Status.SearchNode( (void *)(v0->e), compare_edge_to_intervalTD, 
                                &node );
      if( side != 0 ) throw InternalError();

      v = ((edge_interval *)node->key)->v;

      edge = new graph_edge();
      E->Append(edge);
            
      edge->v[1] = v;
      edge->v[0] = v0;

      EdgeInsert( edge, E->nElems );
    }

    for( j = 0; j < nTE; j++ )
    {
      a = TE[j];
      if( a->v[1] == v0 )  /* edge ends in v0 */
      {         
        side = Status.SearchNode( (void *)a, compare_edge_to_intervalTD, 
                                &node1 );
        if( side != 0 ) throw InternalError();

        sign = Compare( a->v[0]->v.v().m_y, v0->v.v().m_y );
        if( sign == 0 )             /*special case: horizontal edge */
        {
          ((edge_interval *)(node1->key))->v = a->v[0];
        }
        else
        {
          r = ((edge_interval *)node1->key)->r;
          ((edge_interval *)node1->key)->r = a;
          ((edge_interval *)node1->key)->v = v0;
          a->reserved[0] = (void *)node1;
 
          node2 = Status.NewNode();

          ((edge_interval *)node2->key)->l = a;
          ((edge_interval *)node2->key)->r = r;
          ((edge_interval *)node2->key)->v = v0;
          a->reserved[1] = (void *)node2;
          r->reserved[0] = (void *)node2;
          
          side = Status.SearchNode( (void *)a, compare_edge_to_intervalTD, 
                                    &node );
          if( side == 0 ) throw InternalError();

          Status.InsertNode( node2, node, side );
          // Status.Dump(); Status.DumpNodes();

        }
      }
    }

    v0 = v0->prev;
  }


/********* Bottom Up Sweep ****************************************/

  node = Status.Head();
  
  I = (edge_interval *)node->key;
  I->v = NULL;

  gul::Assert<InternalError>( ndebug || 
                        ((node->left == NULL) || (node->right == NULL) ||
                         (I->l == EDGE_NEG_INF) || (I->r == EDGE_POS_INF)) );
  v0 = vmin;
  while( v0 != NULL )
  {
    nTE = IncidentEdges( v0, TE );

    regular = 0;
    for( j = 0; j < nTE; j++ )
    {
      a = TE[j];
      if( a->v[1] == v0 )    /* edge ends in v0 (then v0 cannot be minimum */
      {                      /* of graph) */
        regular = 1;
    
        sign = Compare( a->v[0]->v.v().m_y, v0->v.v().m_y );
        if( sign == 0 ) continue; /* nothing to do for horizontal edges */

                        /* replace the two intervalls [l,a],[a,r] with [l,r] */
        node1 = (BBTNode *)a->reserved[0];
        node2 = (BBTNode *)a->reserved[1];        

        edge = ((edge_interval *)(node2->key))->r;
        edge->reserved[0] = (void *)node1;

        ((edge_interval *)(node1->key))->r = edge;        
        ((edge_interval *)(node1->key))->v = v0;
        
        Status.RemoveNode( node2 );
        Status.FreeNode( node2 );
      }
    }

    if( (!regular) && (v0 != vmin) )
    {
      side = Status.SearchNode( (void *)(v0->e), compare_edge_to_intervalBU, 
                                &node );
      if( side != 0 ) throw InternalError();

      v = ((edge_interval *)node->key)->v;

      edge = new graph_edge();
      E->Append(edge);
      
      edge->v[1] = v0;
      edge->v[0] = v;

      EdgeInsert( edge, E->nElems );
    }

    for( j = 0; j < nTE; j++ )
    {
      a = TE[j];
      if( a->v[0] == v0 )  /* edge starts in v0 */
      {         
        side = Status.SearchNode( (void *)a, compare_edge_to_intervalBU, 
                                &node1 );
        if( side != 0 ) throw InternalError();

        sign = Compare( a->v[1]->v.v().m_y, v0->v.v().m_y );
        if( sign == 0 )  /*special case: horizontal edge */
        {
          ((edge_interval *)(node1->key))->v = a->v[1];
        }
        else
        {
          r = ((edge_interval *)node1->key)->r;
          ((edge_interval *)node1->key)->r = a;
          ((edge_interval *)node1->key)->v = v0;
          a->reserved[0] = (void *)node1;
 
          node2 = Status.NewNode();
 
          ((edge_interval *)node2->key)->l = a;
          ((edge_interval *)node2->key)->r = r;
          ((edge_interval *)node2->key)->v = v0;
          a->reserved[1] = (void *)node2;
          r->reserved[0] = (void *)node2;
          
          side = Status.SearchNode( (void *)a, compare_edge_to_intervalBU, 
                                    &node );
          if( side == 0 ) throw InternalError();

          Status.InsertNode( node2, node, side );
        }
      }
    }

    v0 = v0->next;
  }

  node = Status.Head();
  
  I = (edge_interval *)node->key;
  gul::Assert<InternalError>( ndebug || 
                        ((node->left == NULL) || (node->right == NULL) ||
                         (I->l == EDGE_NEG_INF) || (I->r == EDGE_POS_INF)) );

  Status.RemoveNode( node );
  Status.FreeNode( node );
}

}
