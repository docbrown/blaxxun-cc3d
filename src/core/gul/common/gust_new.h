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
 
#ifndef GUST_NEW_H
#define GUST_NEW_H

#ifndef _MSC_VER
  #include <new.h>
#else
  #ifdef new
    #undef new
  #endif
  #include <new.h>
#endif

#define reserve_stack(T,n) (T *)alloca((n)*sizeof(T))

namespace gust {

// using namespace gul;

/* -------------------------------------------------------------------------
  Low level memory management
-------------------------------------------------------------------------- */

inline void release( void *buf, unsigned int t, unsigned int s )
{
  if( (t == 0) || (buf == NULL) ) return;
  if( t == 1 ) { free(buf); return; }
  PreferPoolFree( buf, s );
}

inline void *reserve_global( size_t s, size_t *ret_s )
{
  void *p = malloc( s );
  if( p == NULL ) throw gul::AllocError();
  *ret_s = s;
  return(p);
}

inline void *reserve_pool( size_t s, size_t *ret_s )
{
  void *p = PreferPoolAlloc( s, ret_s);
  if( p == NULL ) throw gul::AllocError();
  return(p);
}





}



#endif
 
