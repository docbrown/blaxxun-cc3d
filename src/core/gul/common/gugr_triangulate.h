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

#ifndef GUGR_TRIANGULATE_H
#define GUGR_TRIANGULATE_H

namespace gugr {

/* ------------------------------------------------------------------------
  Triangulates the regions of a graph. E must be regularized, and V
  sorted. For this purpose the graph is divided into monotone chains.
  Between two monontone chains lies a monotone polygone.
  Only those triangles are calculated, whose edges have a positive face
  index on the inward side 
------------------------------------------------------------------------- */
void Triangulate( graph_edge_list *E, graph_vertex_list *V,
                  const PrecRational& far_left, triangle_list *T );
                     
/* ------------------------------------------------------------------------
  Triangulates a monotone polygon

  The vertices in V1 and V2, and the edges in E are deleted
  in this function! 
-------------------------------------------------------------------------- */
void TriangulateMonotonePolygon(
              graph_vertex_list2 *V1, graph_vertex_list2 *V2,
              graph_edge_list *E, triangle_list *T );

}

#endif

