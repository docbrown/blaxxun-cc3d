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

#ifndef GUL_ERROR_H
#define GUL_ERROR_H

namespace gul {

class Error
{
public:
  virtual int number() { return(1); }
  virtual const char *descr() { return("General error\n"); }
};

class AllocError : public Error
{
public:
  virtual int number() { return(2); }
  virtual const char *descr() { return("Alloc error\n"); }
};

class PoolAllocError : public Error
{
public:
  virtual int number() { return(3); }
  virtual const char *descr() { return("PoolAlloc error\n"); }
};

class InternalError : public Error
{
public:
  virtual int number() { return(4); }
  virtual const char *descr() 
  { return("Internal error - please contact authors\n"); }
};

class IntervalDivZeroError : public Error
{
public:
  virtual int number() { return(5); }
  virtual const char *descr() 
  { return("Division by neighborhood of 0 interval\n"); }
};

template < class X, class A > 
inline void Assert( A assertion )
{
 if(!assertion) throw X();
}

/*
template<> void Assert( bool assertion )
{
  if(!assertion) throw InternalError();
}
*/
}
 
#endif
