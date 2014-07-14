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
 
#ifndef GUNU_TESSELATE_H
#define GUNU_TESSELATE_H

namespace gunu {

using gul::Ptr;
using gul::rtr;
using gul::rtl;
using gul::point;
using gul::point2;
using gul::hpoint;
using gul::hpoint2;
using gul::line2;
using gul::cross_product;
using gul::euclid;

/*-------------------------------------------------------------------------
  approximate trimmed NURBS surface with triangles, another user interface
  function
-------------------------------------------------------------------------*/
template< class T >
struct TessCbData
{
  int                        need_normals : 1;
  gul::rtl<T>                t;

  void (*quadfunc)( void *, 
                   gul::point<T> *, gul::point<T> *, gul::point<T> *, gul::point<T> *,
                   gul::point<T> *, gul::point<T> *, gul::point<T> *, gul::point<T> *,
                   T *, T *, T *, T * );
  void (*trifunc)( void *, 
                   gul::point<T> *, gul::point<T> *, gul::point<T> *,
                   gul::point<T> *, gul::point<T> *, gul::point<T> *,
                   gul::point2<T> *, gul::point2<T> *, gul::point2<T> * );
  void *usrdata;  

  TessCbData(                              /* --- constructor --- */
         int     normal_flag,
         rtl<T>  a_t,
         void (*a_quadfunc)( void *, 
                   gul::point<T> *, gul::point<T> *, gul::point<T> *, gul::point<T> *,
                   gul::point<T> *, gul::point<T> *, gul::point<T> *, gul::point<T> *,
                   T *, T *, T *, T * ),
         void (*a_trifunc)( void *, 
                   gul::point<T> *, gul::point<T> *, gul::point<T> *,
                   gul::point<T> *, gul::point<T> *, gul::point<T> *,
                   gul::point2<T> *, gul::point2<T> *, gul::point2<T> * ),
         void *a_usrdata )
  {
    need_normals = normal_flag;
    t = a_t; 
    quadfunc = a_quadfunc;
    trifunc = a_trifunc;
    usrdata = a_usrdata;
  }
};

/*-------------------------------------------------------------------
  approximate trimmed NURBS surface with triangles
--------------------------------------------------------------------*/
template< class T >
void LinearizeTrimmedSurface( 
            int current_iter, int max_iter,
            TessInfo<T> *A,
            gugr::GraphInfo *G, gugr::GraphConvInfo<T> *Gconv,
            const T tol, const rtl<T>& t,
            void outfunc1( TessInfo<T> *, void * ),                                   
            void outfunc2( TessInfo<T> *, gugr::GraphInfo *, gugr::GraphConvInfo<T> *, 
                           void * ),
            void *usrdata );
            
/*-------------------------------------------------------------------
  approximate trimmed NURBS surface with triangles
--------------------------------------------------------------------*/
template< class T >
EXP
void LinearizeSurface( 
            int max_iter,
            const int nu, const int pu, Ptr< T >& KnotsU,
            const int nv, const int pv, Ptr< T >& KnotsV,
            Ptr< Ptr< hpoint<T> > >& Pw,
            const T tol, const rtl<T>& t,
            Ptr< Ptr< point2<T> > >& contour,
            void (*outfunc1)( TessInfo<T> *, void * ),                                  
            void (*outfunc2)( TessInfo<T> *, gugr::GraphInfo *, gugr::GraphConvInfo<T> *,
                           void * ),
            void *usrdata );

/*-------------------------------------------------------------------------
  another user interface function with other callbacks
-------------------------------------------------------------------------*/

template< class T > EXP
void TessQuadCb( TessInfo<T> *A, void *usrdata );
template< class T > EXP
void TessTriCb( TessInfo<T> *, gugr::GraphInfo *, gugr::GraphConvInfo<T> *, void * );

template< class T, class HP, class EP, class DP >
inline void LinearizeSurfaceX( 
            int max_iter,
            const int nu, const int pu, Ptr< T >& KnotsU,
            const int nv, const int pv, Ptr< T >& KnotsV,
            Ptr< Ptr < HP > >& Pw,
            const T tol, const rtl<T>& t,
            Ptr< Ptr< DP > >& contour,
            bool normal_flag,
            void (*quadfunc)( void *, 
                   EP *, EP *, EP *, EP *,
                   EP *, EP *, EP *, EP *,
                   T *, T *, T *, T * ),
            void (*trifunc)( void *, 
                   EP *, EP *, EP *,
                   EP *, EP *, EP *,
                   DP *, DP *, DP * ),
            void *usrdata )
{
  TessCbData<T> Cb(normal_flag,t,quadfunc,trifunc,usrdata);

  LinearizeSurface<T>( max_iter,nu,pu,KnotsU,nv,pv,KnotsV,Pw,tol,t,contour,
                       TessQuadCb<T>,TessTriCb<T>,(void *)&Cb );
}



}

#endif

