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
 
#ifndef GUNU_LINEARIZE_H
#define GUNU_LINEARIZE_H

namespace gunu {

using gul::Ptr;
using gul::rtl;
using gul::point;
using gul::hpoint;

template< class T >
struct LinearizeCallbackData
{
  int                        much_info;
  TessInfo<T>               *Org;
  rtl<T>                     t;
  void (*usrfunc)( void *, 
                   point<T> *, point<T> *, point<T> *, point<T> *,
                   point<T> *, point<T> *, point<T> *, point<T> *,
                   T *, T *, T *, T * );
  void                      *usrdata;  

  LinearizeCallbackData(
         int               a_much_info,
         TessInfo<T>      *a_Org,
         rtl<T>            a_t,
         void (*a_usrfunc)( void *, 
                   point<T> *, point<T> *, point<T> *, point<T> *,
                   point<T> *, point<T> *, point<T> *, point<T> *,
                   T *, T *, T *, T * ),
         void              *a_usrdata )
  {
    much_info = a_much_info;
    Org = a_Org;
    t = a_t;
    usrfunc = a_usrfunc;
    usrdata = a_usrdata;
  }
};

/*******************************************************************
  approximate NURBS surface with triangles
********************************************************************/

template< class T, class HP, class EP >
EXP
void LinearizeSurface( 
            int max_iter,
            const int nu, const int pu, Ptr< T > U,
            const int nv, const int pv, Ptr< T > V,
            Ptr< Ptr < HP > > Pw,
            const T tol, const rtl<T>& t,
            void (*usrfunc)( void *, 
                   EP *, EP *, EP *, EP *,
                   EP *, EP *, EP *, EP *,
                   T *, T *, T *, T * ),
            void *usrdata );


/**********************************************************************
  linearize curve (with callback function)
**********************************************************************/
template< class T, class HP, class EP >
EXP
void LinearizeCurve( int max_iter,
             const int n, const int p, Ptr< T > U, Ptr< HP > Pw,
             const T tol, const rtl<T>& t,
             void (*usrfunc)( void *, EP *, EP * ), void *usrdata );

/**********************************************************************
  linearize curve (returns a polyline)
**********************************************************************/
template< class T, class HP, class EP >
EXP
void LinearizeCurve( int max_iter,
             const int n, const int p, Ptr< T > U, Ptr< HP > Pw,
             const T tol, const rtl<T>& t,
             int *ret_nP, Ptr< EP > *retP );

}

#endif





