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

#include "gul_types.h"
#include "gust_pool.h"


namespace gust {

/*
int pooldbg::freekey = 0;
int pooldbg::curmark = 0;
int pooldbg::count = 0;
pooldbg *pooldbg::head;
*/

EXP
extern PoolChunkList Pools[6] =
{
  { 0, 0, 4,    256 },
  { 0, 0, 8,    128 },
  { 0, 0, 16,   64  },
  { 0, 0, 64,   32  },  // for testing
  { 0, 0, 256,  16  },
  { 0, 0, 2048, 4   }
};
}
#include "gust_pooltbl.h" 

namespace gust {

void PoolGrow( PoolChunkList *pool )
{
  size_t nelems,elemsize;
  char *chunk,*start,*end,*p;

  nelems = pool->growrate;
  pool->growrate = nelems * 3/2;
  elemsize = pool->elemsize;

/* important: sizeof(char) has to be ONE !!!!!!!!!!! */
 
  chunk = (char *)malloc( sizeof(void *) + nelems * elemsize );
  if( chunk == NULL ) return;
  
  start = chunk + sizeof(void *);
  end = start + (nelems-1)*elemsize;

  for( p = start; p < end; p += elemsize )
    *((void **)p) = p + elemsize;
  
  *((void **)end) = NULL;
  
  pool->head = start;
  *((void **)chunk) = pool->chunks;
  pool->chunks = chunk;
}

void PoolFreeAll( void )
{
  void *ch, *next_ch;
  int nPools = sizeof(Pools)/sizeof(PoolChunkList), i;
  PoolChunkList *pool;

  for( i = 0; i < nPools; i++ )
  {
    pool = &Pools[i];

    ch = pool->chunks;
    while( ch )
    {
      next_ch = *((void **)ch);
      free( ch );
      ch = next_ch;
    }
    pool->head = 0;
    pool->chunks = 0;
  }
}



}

#if 0
/* -------------------------------------------------------------------------
  Program to create the Pool-Table
--------------------------------------------------------------------------- */
#include <stdio.h>

typedef struct
{
  void *head;
  void *chunks;

  size_t elemsize;
  size_t growrate;
}
PoolChunkList;

PoolChunkList Pools[] =
{
  { 0, 0, 4,    256 },
  { 0, 0, 8,    128 },
  { 0, 0, 16,   64  },
  { 0, 0, 64,   16  },
  { 0, 0, 256,  16  },
  { 0, 0, 2048, 4   }
};

#include <iostream.h>

int main()
{
  cout << "PoolChunkList *PoolTab[2049] = {\n";
  size_t pP = 0;
  for( size_t i = 0; i < 2049; i++ )
  {
    if( Pools[pP].elemsize < i ) pP++;

    if( pP > sizeof(Pools)/sizeof(PoolChunkList) ) break;
       
    cout << "  &Pools[" << pP << "], /* " << i << "*/\n"; 
  }
  cout << "};\n";
}
#endif





