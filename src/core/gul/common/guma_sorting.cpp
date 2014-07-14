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

#include <stdio.h>
#include <stdlib.h>
#include <iostream.h>
#include <math.h>
#include "gul_types.h"
#include "guma_sorting.h"


namespace guma {

/*----------------------------------------------------------
  Heapsort (see "Numerical Recipes in C", or Knuth) 
-----------------------------------------------------------*/
void HeapSort( size_t nelem, size_t elsize, void *elptr,
               int (*usrfunc)( void *, void *, void *), void *usrdata )                
{
  char *ra, *rra;
  size_t n = nelem, l, ir, i, j;

  rra = (char *)alloca( elsize );
  if( rra == NULL ) return;

  ra = (char *)elptr;

  if( n < 2 ) return;
  l = (n >> 1) + 1;
  ir = n;
  
  for(;;)
  {
    if( l > 1 )
    {
      memcpy( rra, &ra[(l-2)*elsize], elsize );
      l--;    
    }
    else
    {
      memcpy( rra, &ra[(ir-1)*elsize], elsize );
      memcpy( &ra[(ir-1)*elsize], ra, elsize );
      if( --ir == 1 )
      {
        memcpy( ra, rra, elsize );
        break;
      }
    }
    i = l;
    j = l+l;

    while( j <= ir )
    {
      if( (j < ir) &&
          (usrfunc( &ra[(j-1)*elsize], &ra[j*elsize], usrdata ) < 0) )
        j++;
       
      if( usrfunc(rra, &ra[(j-1)*elsize], usrdata) < 0 )
      {
        memcpy( &ra[(i-1)*elsize], &ra[(j-1)*elsize], elsize );
        i = j;
        j <<= 1;
      }
      else
        j = ir + 1;
    }
    memcpy( &ra[(i-1)*elsize], rra, elsize );
  }
  return;
}

void PtrHeapSort( size_t nelem, void **elptr,
                  int (*usrfunc)( void *, void *, void *), void *usrdata )                
{
  void **ra, *rra;
  size_t n = nelem, l, ir, i, j;

  ra = elptr;

  if( n < 2 ) return;
  l = (n >> 1) + 1;
  ir = n;
  
  for(;;)
  {
    if( l > 1 )
    {
      rra = ra[l-2];
      l--;    
    }
    else
    {
      rra = ra[ir-1];
      ra[ir-1] = ra[0];
      
      if( --ir == 1 )
      {
        ra[0] = rra;
        break;
      }
    }
    i = l;
    j = l+l;

    while( j <= ir )
    {
      if( (j < ir) &&
          (usrfunc( ra[j-1], ra[j], usrdata ) < 0) )
        j++;
  
      if( usrfunc( rra, ra[j-1], usrdata) < 0 )
      {
        ra[i-1] = ra[j-1];
        i = j;
        j <<= 1;
      }
      else
        j = ir + 1;
    }

    ra[i-1] = rra;
  }
}

/***************************************************************

  BALANCED BINARY TREE (see Knuth: "Sorting and Searching")

***************************************************************/

/*-------------------------------------------------------------
  Creates an empty balanced binary tree
------------------------------------------------------------*/

void InitBBT( BBTNode *head )
{
  head->F.b = 1;
  head->F.l = 0;
 
  (*((int *)(&head->left))) = 0;  /* height = 0 */
  head->right = NULL;
}


/*----------------------------------------------------------------------
  Search for an element in a balanced binary tree. If the element was found,
  the address of its node is returned, otherwise the position (address of node
  and side) in the tree is returned, under which the element would be
  inserted.  
------------------------------------------------------------------------*/

int SearchElementInBBT( BBTNode *head, void *key,
                        int compare( void *, void * ),
                        BBTNode **element )
{
  int result;
  BBTNode *P,*el;
  
  el = head;
  result = 1;
  P = head->right;

  while( P != NULL )      /* Knoten suchen */
  {               
    el = P;
    result = compare( key, P->key );

    if( result < 0 )
    {     
      P = P->left;
    }
    else if( result > 0 )
    {
      P = P->right;      
    }
    else
      break;
  }

  *element = el;
  return(result);
}  


/*------------------------------------------------------------------------
  Inserts a node 'element' into a balanced binary tree, given by 'head'.
  The node is inserted under the node 'where' on the side 'side'.
  (side == -1 => left,  side == 1 => right )
------------------------------------------------------------------------*/

void InsertElementIntoBBT( BBTNode *element, 
                           BBTNode *where, int side, BBTNode *head  )
{
  BBTNode *Q, *Pk, *Pk_1, *A, *B, *X;
  int ak, ak_1;

  Q = element;                    /* neuen Knoten initialisieren */
  Q->left = Q->right = NULL;    
  Q->F.b = 0;

  Pk = where;
  ak = side;

  if( ak == -1 )
  {
    Pk->left = Q;
    if( Pk->left != NULL )
    {
      Pk->left->parent = Pk;
      Pk->left->F.l = -1;
    }
  }  
  else
  {
    Pk->right = Q;  
    if( Pk->right != NULL )
    {
      Pk->right->parent = Pk;
      Pk->right->F.l = 1;
    }
  }  
  
  while( (Pk->F.b == 0) && (Pk != head) )  /* Balance Faktoren aktualisieren */
  {
    Pk->F.b = ak;
    ak = Pk->F.l;                 /* k-- */
    Pk = Pk->parent;
  }

  if( Pk == head )
  {
    (*((int *)(&head->left)))++;  /* Baumhoehe im Headerknoten erhoehen */
    return;                      /*** fertig ***/
  }

  if( ak == -1 ) /*** eventuell Rebalancieren, Ausfuehrung fuer (ak == -1) ***/
  {
    A = Pk;
    Pk_1 = Pk->parent;
    ak_1 = Pk->F.l;
    
    if( A->F.b == 1 )             /* (A->B == -ak)   */    
    {
      A->F.b = 0;
      return;                    /*** fertig ***/
    }
    else                          /* A->B == ak      */
    {
      B = Pk->left;               /* B = LINK(A,ak)  */

      if( B->F.b == -1 )          /* (B->B == ak) => Single Rotation */
      {
        A->left = B->right;       /* LINK(A,ak) = LINK(B,-ak) */     
        if( A->left != NULL )
        {
          A->left->parent = A;
          A->left->F.l = -1;
        }
            
        B->right = A;             /* LINK(B,-ak) = A  */
        if( B->right != NULL )
        {
          B->right->parent = B;
          B->right->F.l = 1;
        }
            
        A->F.b = B->F.b = 0;

        ak = ak_1;                          /* k-- */
        Pk = Pk_1;
          
        if( ak == -1 )                      /* Wurzel Teilbaum = B */
        {
          Pk->left = B;
          if( Pk->left != NULL )
          {
            Pk->left->parent = Pk;
            Pk->left->F.l = -1;
          }
        }  
        else
        {
          Pk->right = B;  
          if( Pk->right != NULL )
          {
            Pk->right->parent = Pk;
            Pk->right->F.l = 1;
          }
        }

        return;   /*** fertig ***/
      }
      else                        /* (B->B == -ak) => Double Rotation */
      {
        X = B->right;             /* X = LINK(B,-ak)                  */

        A->left = X->right;       /* LINK(A,ak) = LINK(X,-ak)         */
        if( A->left != NULL )
        {
          A->left->parent = A;
          A->left->F.l = -1;
        }
            
        B->right = X->left;       /* LINK(B,-ak) = LINK(X,ak)         */
        if( B->right != NULL )
        {
          B->right->parent = B;
          B->right->F.l = 1;
        }
            
        X->left = B;              /* LINK(X,ak) = B                   */
        if( X->left != NULL )
        {
          X->left->parent = X;
          X->left->F.l = -1;
        }
            
        X->right = A;             /* LINK(X,-ak) = A                  */
        if( X->right != NULL )
        {
          X->right->parent = X;
          X->right->F.l = 1;
        }
    
        if( X->F.b == 1 )         /* (X.B == -ak)                     */
        {
          B->F.b = -1;            /* B->B = ak                        */
          A->F.b = 0;             /* A->B = 0                         */
        }  
        else if( X->F.b == -1 )   /* (X.B == ak )                     */
        {
          B->F.b = 0;             /* B->B = 0                         */
          A->F.b = 1;             /* A->B = -ak                       */
        }
        else           /* (X.B == 0), (Anm: ist der Fall, wenn X == Q)*/ 
        {
          B->F.b = 0;             /* B->B = 0                         */
          A->F.b = 0;             /* A->B = 0                         */
        }
        X->F.b = 0;               /* X->B = 0                         */

        ak = ak_1;                          /* k-- */
        Pk = Pk_1;
          
        if( ak == -1 )                      /* Wurzel Teilbaum = X */
        {
          Pk->left = X;
          if( Pk->left != NULL )
          {
            Pk->left->parent = Pk;
            Pk->left->F.l = -1;
          }
        }  
        else
        {
          Pk->right = X;  
          if( Pk->right != NULL )
          {
            Pk->right->parent = Pk;
            Pk->right->F.l = 1;
          }
        }

        return;   /*** fertig ***/
      }
    }
  }


  else         /*** Ausfuehrung fuer (ak == 1) ***/
  {
    A = Pk;
    Pk_1 = Pk->parent;
    ak_1 = Pk->F.l;
    
    if( A->F.b == -1 )            /* (A->B == -ak)   */    
    {
      A->F.b = 0;
      return;                    /*** fertig ***/
    }
    else                          /* A->B == ak      */
    {
      B = Pk->right;              /* B = LINK(A,ak)  */

      if( B->F.b == 1 )           /* (B->B == ak) => Single Rotation */
      {
        A->right = B->left;       /* LINK(A,ak) = LINK(B,-ak) */     
        if( A->right != NULL )
        {
          A->right->parent = A;
          A->right->F.l = 1;
        }
        
        B->left = A;              /* LINK(B,-ak) = A  */
        if( B->left != NULL )
        {
          B->left->parent = B;
          B->left->F.l = -1;
        }
        
        A->F.b = B->F.b = 0;

        ak = ak_1;                       /* k-- */
        Pk = Pk_1;
          
        if( ak == -1 )                      /* Wurzel Teilbaum = B */
        {
          Pk->left = B;
          if( Pk->left != NULL )
          {
            Pk->left->parent = Pk;
            Pk->left->F.l = -1;
          }
        }  
        else
        {
          Pk->right = B;  
          if( Pk->right != NULL )
          {
            Pk->right->parent = Pk;
            Pk->right->F.l = 1;
          }
        }

        return;   /*** fertig ***/
      }
      else                        /* (B->B == -ak) => Double Rotation */
      {
        X = B->left;              /* X = LINK(B,-ak)                  */

        A->right = X->left;       /* LINK(A,ak) = LINK(X,-ak)         */
        if( A->right != NULL )
        {
          A->right->parent = A;
          A->right->F.l = 1;
        }

        B->left = X->right;       /* LINK(B,-ak) = LINK(X,ak)         */
        if( B->left != NULL )
        {
          B->left->parent = B;
          B->left->F.l = -1;
        }
    
        X->right = B;             /* LINK(X,ak) = B                   */
        if( X->right != NULL )
        {
          X->right->parent = X;
          X->right->F.l = 1;
        }
            
        X->left = A;              /* LINK(X,-ak) = A                  */
        if( X->left != NULL )
        {
          X->left->parent = X;
          X->left->F.l = -1;
        }
        
        if( X->F.b == -1 )        /* (X.B == -ak)                     */
        {
          B->F.b = 1;             /* B->B = ak                        */
          A->F.b = 0;             /* A->B = 0                         */
        }  
        else if( X->F.b == 1 )    /* (X.B == ak )                     */
        {
          B->F.b = 0;             /* B->B = 0                         */
          A->F.b = -1;            /* A->B = -ak                       */
        }
        else           /* (X.B == 0), (Anm: ist der Fall, wenn X == Q)*/ 
        {
          B->F.b = 0;             /* B->B = 0                         */
          A->F.b = 0;             /* A->B = 0                         */
        }
        X->F.b = 0;               /* X->B = 0                         */

        ak = ak_1;                          /* k-- */
        Pk = Pk_1;
          
        if( ak == -1 )                      /* Wurzel Teilbaum = X */
        {
          Pk->left = X;
          if( Pk->left != NULL )
          {
            Pk->left->parent = Pk;
            Pk->left->F.l = -1;
          }
        }  
        else
        {
          Pk->right = X;  
          if( Pk->right != NULL )
          {
            Pk->right->parent = Pk;
            Pk->right->F.l = 1;
          }
        }

        return;   /*** fertig ***/
      }
    }
  }
}



/*------------------------------------------------------------------------
  Removes a node 'element' from a balanced binary tree 'head'.
  The tree will get rebalanced, if necessary.  
------------------------------------------------------------------------*/

void DeleteElementFromBBT( BBTNode *element, BBTNode *head )
{
  BBTNode *P,*Q,*Pk,*Pk_1,*A,*B,*X;
  int ak, ak_1, ak_ini;
  
  P = head->right;
  if( P == NULL ) return;
    
  P = Q = element;

  if( P->right == NULL )    /* Knoten suchen, bei dem rechter oder */
  {                         /* linker Ast = NULL:                  */
    Pk = P;
    ak = 1;
    Pk_1 = Pk->parent;
    ak_1 = ak_ini = Pk->F.l;
  }
  else if( P->left == NULL )
  {
    Pk = P;
    ak = -1;
    Pk_1 = Pk->parent;
    ak_1 = ak_ini = Pk->F.l;
  }
  else
  {
    P = P->right;                    /* nach rechts gehen */    

    if( P->left == NULL )
    {
      Pk = Q;

      if( Q->F.l == 1 )     
      {
        Q->parent->right = P;
        P->F.l = 1;
      }
      else
      {
        Q->parent->left = P;
        P->F.l = -1; 
      }       
      P->parent = Q->parent;
      P->F.b = Q->F.b;

      ak = 1;
      Pk_1 = P;
      ak_1 = -1;    
      ak_ini = 1;
    }
    else
    {
      do            /* suchen bis Nachfolger in symmetrischer */
      {             /* Ordnung gefunden                       */
        Pk = P;
        P = P->left;
      }
      while( P != NULL );

      ak = -1;
      Pk_1 = Pk->parent;
      ak_1 = ak_ini = Pk->F.l;
    }
  }            

/* --- Knoten loeschen: -------------------------------------------- */

  if( ak_1 == -1 )                                /* wenn Pk linker Sohn */
  {
    Pk_1->left = (ak == -1 ? Pk->right : Pk->left);
    if( Pk_1->left != NULL )
    {
      Pk_1->left->parent = Pk_1;
      Pk_1->left->F.l = -1;
    }
  }    
  else
  {
    Pk_1->right = (ak == -1 ? Pk->right : Pk->left);
    if( Pk_1->right != NULL )
    {
      Pk_1->right->parent = Pk_1;
      Pk_1->right->F.l = 1;
    }
  }  

  if( Pk != Q )
  {
    Pk->left = Q->left;                /* Pk uebernimmt nun die Rolle von Q */
    if( Pk->left != NULL )
      Pk->left->parent = Pk;
    Pk->right = Q->right;
    if( Pk->right != NULL )
      Pk->right->parent = Pk;

    Pk->F.b = Q->F.b;
  
    Pk->parent = Q->parent;
    Pk->F.l = Q->F.l;
    if( Pk->F.l == -1 )
      Pk->parent->left = Pk;
    else
      Pk->parent->right = Pk;
  } 
                                     /* Q kann jetzt freigeben werden */

/* --- Balance-Faktoren updaten ------------------------------------- */  

  ak = ak_ini;
  Pk = Pk_1;

  while( 1 )
  {
    if( Pk == head )                       /* Listenkopf */
    {
      (*((int *)&head->left))--;           /* Hoehe des Baums um 1 erniedrigen */
      break;                 /*** fertig ***/
    }
    else if( Pk->F.b == ak )
    {
      Pk->F.b = 0;

      ak = Pk->F.l;                       /* k-- */
      Pk = Pk->parent;
    }
    else if( Pk->F.b == 0 )
    {
      Pk->F.b = -ak;
      break;                  /*** fertig ***/
    }
    else                                 /* rebalancieren erforderlich, */
    {                                    /* da (Pk->B == -ak) */
      if( ak == -1 )          /*** Ausfuehrung fuer (ak == -1) ***/
      {
        A = Pk;
        ak_1 = Pk->F.l;
        Pk_1 = Pk->parent; 

        B = A->right;                     /* B = Zweig auf der anderen Seite */
      
        if( B->F.b == 1 )                 /* B->B == -ak => Single Rotation */
        {
          A->right = B->left;             /* LINK(A,-ak) = LINK(B,ak) */
          if( A->right != NULL )
          {
            A->right->parent = A;
            A->right->F.l = 1;
          }

          B->left = A;                    /* LINK(B,ak) = A */
          if( B->left != NULL )
          {
            B->left->parent = B;
            B->left->F.l = -1;
          }

          A->F.b = B->F.b = 0;
          
          ak = ak_1;                       /* k-- */
          Pk = Pk_1;
          
          if( ak == -1 )
          {
            Pk->left = B;
            if( Pk->left != NULL )
            {
              Pk->left->parent = Pk;
              Pk->left->F.l = -1;
            }
          }  
          else
          {
            Pk->right = B;  
            if( Pk->right != NULL )
            {
              Pk->right->parent = Pk;
              Pk->right->F.l = 1;
            }
          }
        }
        else if( B->F.b == -1 )           /* (B->B == ak) => Double Rotation */
        {
          X = B->left;                    /* X = LINK(B,ak) */
 
          A->right = X->left;             /* LINK(A,-ak) = LINK(X,ak) */
          if( A->right != NULL )
          {
            A->right->parent = A;
            A->right->F.l = 1;
          }

          B->left = X->right;             /* LINK(B,ak) = LINK(X,-ak) */
          if( B->left != NULL )
          {
            B->left->parent = B;
            B->left->F.l = -1;
          }
          
          X->left = A;                    /* LINK(X,ak) = A           */
          if( X->left != NULL )
          {
            X->left->parent = X;
            X->left->F.l = -1;
          }

          X->right = B;                   /* LINK(X,-ak) = B          */
          if( X->right != NULL )
          {
            X->right->parent = X;
            X->right->F.l = 1;
          }
          
          if( X->F.b == -1 )              /* (X->B == ak)             */
          {
            A->F.b = 0;                   /* A->B = 0                 */
            B->F.b = 1;                   /* B->B = -ak               */
          }
          else if( X->F.b == 1 )          /* (X->B == -ak)            */
          {
            A->F.b = -1;                  /* A->B = ak                */
            B->F.b = 0;                   /* B->B = 0                 */
          }
          else                            /* B->B = 0                 */
          {
            A->F.b = 0;
            B->F.b = 0;
          }
          X->F.b = 0; 

          ak = ak_1;                       /* k-- */
          Pk = Pk_1;

          if( ak == -1 )
          {
            Pk->left = X;
            if( Pk->left != NULL )
            {
              Pk->left->parent = Pk;
              Pk->left->F.l = -1;
            }
          }  
          else
          {
            Pk->right = X;  
            if( Pk->right != NULL )
            {
              Pk->right->parent = Pk;
              Pk->right->F.l = 1;
            }
          }
        }
        else                               /* (B->B == 0) => Single Rotation */
        {
          A->right = B->left;              /* LINK(A,-ak) = LINK(B,ak)     */
          if( A->right != NULL )
          {
            A->right->parent = A;
            A->right->F.l = 1;
          }
          
          B->left = A;                     /* LINK(B,ak) = A               */
          if( B->left != NULL )
          {
            B->left->parent = B;
            B->left->F.l = -1;
          }

          B->F.b = -1;                     /* B->B = ak                    */

          ak = ak_1;                    /* k-- */
          Pk = Pk_1;

          if( ak == -1 )
          {
            Pk->left = B;
            if( Pk->left != NULL )
            {
              Pk->left->parent = Pk;
              Pk->left->F.l = -1;
            }
          }  
          else
          {
            Pk->right = B;  
            if( Pk->right != NULL )
            {
              Pk->right->parent = Pk;
              Pk->right->F.l = 1;
            }
          }

          break;              /*** fertig ***/
        }


      }
      else                    /*** dasselbe nochmal fuer (ak == +1) ***/
      {
        A = Pk;
        ak_1 = Pk->F.l;
        Pk_1 = Pk->parent;
        
        B = A->left;                      /* B = Zweig auf der anderen Seite */
      
        if( B->F.b == -1 )                /* (B->B == -ak) => Single Rotation */
        {
          A->left = B->right;             /* LINK(A,-ak) = LINK(B,ak) */
          if( A->left != NULL )
          {
            A->left->parent = A;
            A->left->F.l = -1;
          }
                
          B->right = A;
          if( B->right != NULL )
          {
            B->right->parent = B;
            B->right->F.l = 1;
          }
          
          A->F.b = B->F.b = 0;
          
          ak = ak_1;                       /* k-- */
          Pk = Pk_1;
        
          if( ak == -1 )
          {
            Pk->left = B;
            if( Pk->left != NULL )
            {
              Pk->left->parent = Pk;
              Pk->left->F.l = -1;
            }
          }  
          else
          {
            Pk->right = B;  
            if( Pk->right != NULL )
            {
              Pk->right->parent = Pk;
              Pk->right->F.l = 1;
            }
          }
        }
        else if( B->F.b == 1 )            /* (B->B == ak) => Double Rotation */
        {
          X = B->right;                   /* X = LINK(B,ak) */
 
          A->left = X->right;             /* LINK(A,-ak) = LINK(X,ak) */
          if( A->left != NULL )
          {
            A->left->parent = A;
            A->left->F.l = -1;
          }
          
          B->right = X->left;             /* LINK(B,ak) = LINK(X,-ak) */
          if( B->right != NULL )
          {
            B->right->parent = B;
            B->right->F.l = 1;
          }

          X->right = A;                   /* LINK(X,ak) = A           */
          if( X->right != NULL )
          {
            X->right->parent = X;
            X->right->F.l = 1;
          }

          X->left = B;                    /* LINK(X,-ak) = B          */
          if( X->left != NULL )
          {
            X->left->parent = X;
            X->left->F.l = -1;
          }

          if( X->F.b == 1 )               /* (X->B == ak)             */
          {
            A->F.b = 0;                   /* A->B = 0                 */
            B->F.b = -1;                  /* B->B = -ak               */
          }
          else if( X->F.b == -1 )         /* (X->B == -ak)            */
          {
            A->F.b = 1;                   /* A->B = ak                */
            B->F.b = 0;                   /* B->B = 0                 */
          }
          else                            /* B->B = 0                 */
          {
            A->F.b = 0;
            B->F.b = 0;
          }  
          X->F.b = 0;

          ak = ak_1;                       /* k-- */
          Pk = Pk_1;
        
          if( ak == -1 )
          {
            Pk->left = X;
            if( Pk->left != NULL )
            {
              Pk->left->parent = Pk;
              Pk->left->F.l = -1;
            }
          }  
          else
          {
            Pk->right = X;  
            if( Pk->right != NULL )
            {
              Pk->right->parent = Pk;
              Pk->right->F.l = 1;
            }
          }
        }
        else                               /* (B->B == 0) => Single Rotation */
        {
          A->left = B->right;              /* LINK(A,-ak) = LINK(B,ak)     */
          if( A->left != NULL )
          {
            A->left->parent = A;
            A->left->F.l = -1;
          }

          B->right = A;                    /* LINK(B,ak) = A               */
          if( B->right != NULL )
          {
            B->right->parent = B;
            B->right->F.l = 1;
          }
          
          B->F.b = 1;                      /* B->B = ak                    */

          ak = ak_1;                    /* k-- */
          Pk = Pk_1;
        
          if( ak == -1 )
          {
            Pk->left = B;
            if( Pk->left != NULL )
            {
              Pk->left->parent = Pk;
              Pk->left->F.l = -1;
            }
          }  
          else
          {
            Pk->right = B;  
            if( Pk->right != NULL )
            {
              Pk->right->parent = Pk;
              Pk->right->F.l = 1;
            }
          }

          break;              /*** fertig ***/
        }
      }
    }    /*** Ende "while"-Schleife ***/
  }
    
  return;
}                                   
                 
/*------------------------------------------------------------------------
  Dump BBT Tree (for debugging)
------------------------------------------------------------------------*/  

void DumpBBTTree( BBTNode *node, int level,  
                  void dumpkey_func( void * ) )
{
  int i;

  if( node == NULL )
    return;
    
  if( node->left != NULL )
    DumpBBTTree( node->left, level+4, dumpkey_func );

  for( i = 0; i < level; i++ )
  {
    cout << " ";
  }

  if( node->key != NULL )
    dumpkey_func( node->key );
  cout << "\n";

  if( node->right != NULL )
    DumpBBTTree( node->right, level+4, dumpkey_func );
}

/*------------------------------------------------------------------------
  Dump BBT Tree (for debugging)
------------------------------------------------------------------------*/  

void DumpBBTNode( BBTNode *node, int level )
{
  int i;

  if( node == NULL )
    return;
    
  if( node->left != NULL )
    DumpBBTNode( node->left, level+4 );

  for( i = 0; i < level; i++ )
  {
    cout << " ";
  }
  cout << (void *)node << ":[b:" << node->F.b << ", l:" << node->F.l <<
       "] (l=" << (void *)node->left << ", r=" <<
       (void *)node->right << ", p=" << (void *)node->parent << ")\n";

  if( node->right != NULL )
    DumpBBTNode( node->right, level+4 );
}


}










