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

#ifndef GUST_POOL_H
#define GUST_POOL_H

namespace gust {

typedef struct
{
  void *head;
  void *chunks;

  size_t elemsize;
  size_t growrate;
}
PoolChunkList;


const size_t PoolTabSize=2049;        /* must not be changed */
EXP
extern PoolChunkList *PoolTab[2049];
EXP
extern PoolChunkList  Pools[6];


EXP
void PoolGrow( PoolChunkList *pool );
EXP
void PoolFreeAll( void );

/*
struct pooldbg
{
  static int freekey;
  static int curmark;
  static int count;
  static pooldbg *head;

  int key;
  int mark;
  size_t insize;
  size_t outsize;
  void   *address;
 
  pooldbg *next;

  pooldbg( void *p, size_t sin, size_t sout )
  {
    count++;

    address = p;
    insize = sin;
    outsize = sout;

    key = ++freekey;
    mark = curmark;

    next = head;
    head = this;
  }
  static void remove( void *p )
  {
    pooldbg *n = head;
    pooldbg *nprev = 0;

    while( (n != 0) && (n->address != p) )
    {
      nprev = n;
      n = n->next;
    }
    if( n != 0 )
    {
      if( nprev == 0 ) head = n->next;
      else nprev->next = n->next;
      delete n;
    }
  } 
  static void incmark()
  {
    curmark++;
  }
  ~pooldbg()
  {
    count--;
  }
};
*/

inline void *PoolAlloc( const size_t insize, size_t *outsize  )
{
  PoolChunkList *pool;
  void *head;
  
  pool = PoolTab[insize];

  head = pool->head;
  
  if( head == NULL )
  {
    PoolGrow( pool );
    head = pool->head;
  }

  pool->head = *((void **)head);
  *outsize = pool->elemsize; 
//  new pooldbg( (void *)head, insize, *outsize );
  return((void *)head); 
}

inline void PoolFree( void *ptr, const size_t size )
{
  PoolChunkList *pool = PoolTab[size];

  *((void **)ptr) = pool->head;
  pool->head = ptr;
//  pooldbg::remove( ptr );
}

inline void *PreferPoolAlloc( const size_t insize, size_t *outsize  )
{
  PoolChunkList *pool;
  void *head;

  if( insize >= PoolTabSize )
  {
    *outsize = insize;

    void *p = malloc(insize);
//    new pooldbg( p, insize, *outsize );
    return p;
  }
  
  pool = PoolTab[insize];

  head = pool->head;
  
  if( head == NULL )
  {
    PoolGrow( pool );
    head = pool->head;
  }

  pool->head = *((void **)head);
  *outsize = pool->elemsize; 
//  new pooldbg( (void *)head, insize, *outsize );
  return((void *)head); 
}

inline void PreferPoolFree( void *ptr, const size_t size )
{
  if( size >= PoolTabSize ) 
  {
    free(ptr);
//    pooldbg::remove( ptr );
    return;
  }

  PoolChunkList *pool = PoolTab[size];

  *((void **)ptr) = pool->head;
  pool->head = ptr;
//  pooldbg::remove( ptr );
}

}

#endif
