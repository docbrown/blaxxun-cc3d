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
 
#ifndef GUMA_SORTING_H
#define GUMA_SORTING_H

namespace guma {

/*----------------------------------------------------------
  Heapsort (see "Numerical Recipes in C", or Knuth) 
-----------------------------------------------------------*/
void HeapSort( size_t nelem, size_t elsize, void *elptr,
         int (*usrfunc)( void *, void *, void *), void *usrdata );
void PtrHeapSort( size_t nelem, void **elptr,
         int (*usrfunc)( void *, void *, void *), void *usrdata );
         

/***************************************************************

  BALANCED BINARY TREE (see Knuth: "Sorting and Searching")

***************************************************************/

/*---------------------------------------------------------------
  Structure for the nodes of a balanced binary tree
----------------------------------------------------------------*/

typedef struct bbt_node
{
  void                 *key;
    
  struct
  {
    int                b : 2;
    int                l : 2;
  }F;

  struct bbt_node  *left;
  struct bbt_node  *right;
  struct bbt_node  *parent;
}
BBTNode;

/*-------------------------------------------------------------
  Creates an empty balanced binary tree
------------------------------------------------------------*/
void InitBBT( BBTNode *head );

/*----------------------------------------------------------------------
  Search for an element in a balanced binary tree. If the element was found,
  the address of its node is returned, otherwise the position (address of node
  and side) in the tree is returned, under which the element would be
  inserted.  
------------------------------------------------------------------------*/
int SearchElementInBBT( BBTNode *head, void *key,
                        int compare( void *, void * ),
                        BBTNode **element );

/*------------------------------------------------------------------------
  Inserts a node 'element' into a balanced binary tree, given by 'head'.
  The node is inserted under the node 'where' on the side 'side'.
  (side == -1 => left,  side == 1 => right )
------------------------------------------------------------------------*/
void InsertElementIntoBBT( BBTNode *element, 
                           BBTNode *where, int side, BBTNode *head  );

/*------------------------------------------------------------------------
  Removes a node 'element' from a balanced binary tree 'head'.
  The tree will get rebalanced, if necessary.  
------------------------------------------------------------------------*/
void DeleteElementFromBBT( BBTNode *element, BBTNode *head );

/*------------------------------------------------------------------------
  Dump BBT Tree (for debugging)
------------------------------------------------------------------------*/  
void DumpBBTTree( BBTNode *node, int level,  
                  void dumpkey_func( void * ) );
void DumpBBTNode( BBTNode *node, int level );


}

#endif
