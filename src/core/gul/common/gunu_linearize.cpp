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

// #if !defined(_MSC_VER) || defined(NEED_NEW_H)
//  #include <new.h>
// #endif

#include "gul_types.h"
#include "gust_new.h"
#include "gul_vector.h"
#include "guge_linear.h"
#include "guge_normalize.h"
#include "gunu_basics.h"
#include "gunu_derivatives.h"
#include "gunu_divide.h"
#include "gunu_intersect.h"
#include "gunu_linearize.h"

namespace gunu {

using gul::rtl;
using gul::List;
using gul::List2;
using gul::ListNode;
using gul::point;
using gul::hpoint;
using gul::point2;
using gul::hpoint2;
using gul::curve;
using gul::Min;
using gul::set;
using gul::is_zero;
using gul::normalize;
using gul::euclid;

/************************************************************************
  Callback fuer Dreiecksausgabe
  *************************************************************************/
template< class T >
void LinearizeCallback( TessInfo<T> *A, void *usrdata )
{
  point<T> P00,P01,P10,P11,T00,T01,T10,T11;
  int nu,nv,pu,pv;
  LinearizeCallbackData<T> *cb = (LinearizeCallbackData<T> *)usrdata;
  Ptr< Ptr< hpoint<T> > > Pw;

  P00 = A->org->P00;
  P01 = A->org->P01;
  P10 = A->org->P10;
  P11 = A->org->P11;

  if( cb->much_info == 0 )
  {
    cb->usrfunc( cb->usrdata, 
                 &P00, &P01, &P10, &P11, NULL, NULL, NULL, NULL,
                 NULL, NULL, NULL, NULL );
  }
  else                             /* Calc Normals */
  {
    nu = A->org->nu;
    pu = A->org->pu;
    nv = A->org->nv;
    pv = A->org->pv;
    Pw = A->org->Pw;
  
    CalcNormal_U0V0<T>( nu, pu, nv, pv, Pw, &T00, cb->t );
    CalcNormal_U1V0<T>( nu, pu, nv, pv, Pw, &T01, cb->t );
    CalcNormal_U0V1<T>( nu, pu, nv, pv, Pw, &T10, cb->t );
    CalcNormal_U1V1<T>( nu, pu, nv, pv, Pw, &T11, cb->t );

    cb->usrfunc( cb->usrdata, &P00, &P01, &P10, &P11,
                 &T00, &T01, &T10, &T11, &A->u1, &A->u2, &A->v1, &A->v2 );
  }
}
/************************************************************************
  Callback fuer Dreiecksausgabe
  *************************************************************************/
template void LinearizeCallback( TessInfo<float> *A, void *usrdata );
template void LinearizeCallback( TessInfo<double> *A, void *usrdata );

/*******************************************************************
  NURBS Flaeche durch Dreiecke approximieren
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
            void *usrdata )
{
  TessInfo<T> A,B;
  LinearizeCallbackData<T> Cb(0,0,t,0,0);
  
/* Flaechen Info initialisieren */

  A.org->nu = nu;
  A.org->pu = pu;  
  A.org->U =  U;
  A.org->nv = nv;
  A.org->pv = pv;  
  A.org->V =  V;
  A.org->Pw = Pw;
/*
  guge::CalcBoundingBox( (A.org->nu+1)*(A.org->nv+1), 
               A.org->Pw[0], A.x1, A.x2, A.y1, A.y2, A.z1, A.z2 ); 
*/
  A.org->P00 = euclid( A.org->Pw[0][0] );
  A.org->P01 = euclid( A.org->Pw[0][A.org->nu] );
  A.org->P10 = euclid( A.org->Pw[A.org->nv][0] );
  A.org->P11 = euclid( A.org->Pw[A.org->nv][A.org->nu] );

  A.u1 = 0.0;
  A.u2 = 1.0; 
  A.v1 = 0.0;
  A.v2 = 1.0; 

/* Daten fuer Callback initialisieren */

  B.org->nu = nu;
  B.org->pu = pu;  
  B.org->U =  U;
  B.org->nv = nv;
  B.org->pv = pv;  
  B.org->V =  V;
  B.org->Pw = Pw;
/*
  guge::CalcBoundingBox( (B.org->nu+1)*(B.org->nv+1), 
               B.org->Pw[0], B.x1, B.x2, B.y1, B.y2, B.z1, B.z2 ); 
*/
  B.org->P00 = euclid( B.org->Pw[0][0] );
  B.org->P01 = euclid( B.org->Pw[0][B.org->nu] );
  B.org->P10 = euclid( B.org->Pw[B.org->nv][0] );
  B.org->P11 = euclid( B.org->Pw[B.org->nv][B.org->nu] );

  B.u1 = 0.0;
  B.u2 = 1.0; 
  B.v1 = 0.0;
  B.v2 = 1.0;   
  
  Cb.much_info = 1;
  Cb.Org = &B;
  Cb.usrfunc = usrfunc;
  Cb.usrdata = usrdata;

  LinearizeSurface<T>( 1, max_iter, &A, tol, t, LinearizeCallback<T>, &Cb );
}


/*******************************************************************
  NURBS Flaeche durch Dreiecke approximieren
********************************************************************/
template EXP
void LinearizeSurface( 
            int max_iter,
            const int nu, const int pu, Ptr< float > U,
            const int nv, const int pv, Ptr< float > V,
            Ptr< Ptr < hpoint<float> > > Pw,
            const float tol, const rtl<float>& t,
            void (*usrfunc)( void *, 
                 point<float> *, point<float> *, point<float> *, point<float> *,
                 point<float> *, point<float> *, point<float> *, point<float> *,
                 float *, float *, float *, float * ),
            void *usrdata );
template EXP
void LinearizeSurface( 
          int max_iter,
          const int nu, const int pu, Ptr< double > U,
          const int nv, const int pv, Ptr< double > V,
          Ptr< Ptr < hpoint<double> > > Pw,
          const double tol, const rtl<double>& t,
          void (*usrfunc)( void *, 
             point<double> *, point<double> *, point<double> *, point<double> *,
             point<double> *, point<double> *, point<double> *, point<double> *,
             double *, double *, double *, double * ),
          void *usrdata );


/**********************************************************************
  linearize curve, internal recurrent function
**********************************************************************/
template< class T, class HP, class EP >
void DoLinearizeCurve( int current_iter, int max_iter,
                       curve<T,HP> *C, const T tol, const rtl<T>& t,
                       void (*usrfunc)( void *, EP *, EP * ), void *usrdata )   
{
  if( (current_iter < max_iter) &&
      (!guge::ProjectionIsLinear<T,HP,EP>( C->cpt.n, C->cpt.Pw, tol, t )) )
  {   
    curve<T,HP> C1,C2;

    SplitCurve<T,HP>( C, 0.5, &C1, &C2 );
    C->cpt.Pw.reset();
    C->knt.U.reset();
    
    DoLinearizeCurve( current_iter+1, max_iter, &C1, tol, t, usrfunc, usrdata );
    DoLinearizeCurve( current_iter+1, max_iter, &C2, tol, t, usrfunc, usrdata );
  }
  else
  {
    EP P1,P2;

    P1 = euclid( C->cpt.Pw[0] );
    P2 = euclid( C->cpt.Pw[C->cpt.n] );
    C->cpt.Pw.reset();
    C->knt.U.reset();

    usrfunc( usrdata, &P1, &P2 );
  }
}
/**********************************************************************
  linearize curve, internal recurrent function
**********************************************************************/
template void DoLinearizeCurve( int current_iter, int max_iter,
                  curve< float,hpoint<float> > *C,
                  const float tol, const rtl<float>& t,
                  void (*usrfunc)( void *, point<float> *, point<float> * ),
                  void *usrdata );
template void DoLinearizeCurve( int current_iter, int max_iter,
                  curve< double,hpoint<double> > *C,
                  const double tol, const rtl<double>& t,
                  void (*usrfunc)( void *, point<double> *, point<double> * ),
                  void *usrdata );

template void DoLinearizeCurve( int current_iter, int max_iter,
                  curve< float,hpoint2<float> > *C,
                  const float tol, const rtl<float>& t,
                  void (*usrfunc)( void *, point2<float> *, point2<float> * ),
                  void *usrdata );
template void DoLinearizeCurve( int current_iter, int max_iter,
                  curve< double,hpoint2<double> > *C,
                  const double tol, const rtl<double>& t,
                  void (*usrfunc)( void *, point2<double> *, point2<double> * ),
                  void *usrdata );

/**********************************************************************
  linearize curve
**********************************************************************/
template< class T, class HP, class EP >
EXP
void LinearizeCurve( int max_iter,
             const int n, const int p, Ptr< T > U, Ptr< HP > Pw,
             const T tol, const rtl<T>& t,
             void (*usrfunc)( void *, EP *, EP * ), void *usrdata )
{
  curve<T,HP> C;

  C.p = p;
  C.knt.m = n+p+1;
  C.knt.U = U;

  C.cpt.n = n;
  C.cpt.Pw = Pw;

  DoLinearizeCurve( 1, max_iter, &C, tol, t, usrfunc, usrdata );
}
/**********************************************************************
  linearize curve
**********************************************************************/
template EXP
void LinearizeCurve( int max_iter,
           const int n, const int p, Ptr< double > U, Ptr< hpoint<double> > Pw,
           const double tol, const rtl<double>& t,
           void (*usrfunc)( void *, point<double> *, point<double> * ),
           void *usrdata );
template EXP
void LinearizeCurve( int max_iter,
           const int n, const int p, Ptr< float > U, Ptr< hpoint<float> > Pw,
           const float tol, const rtl<float>& t,
           void (*usrfunc)( void *, point<float> *, point<float> * ),
           void *usrdata );

template EXP
void LinearizeCurve( int max_iter,
           const int n, const int p, Ptr< double > U, Ptr< hpoint2<double> > Pw,
           const double tol, const rtl<double>& t,
           void (*usrfunc)( void *, point2<double> *, point2<double> * ),
           void *usrdata );
template EXP
void LinearizeCurve( int max_iter,
           const int n, const int p, Ptr< float > U, Ptr< hpoint2<float> > Pw,
           const float tol, const rtl<float>& t,
           void (*usrfunc)( void *, point2<float> *, point2<float> * ),
           void *usrdata );

/**********************************************************************
  linearize curve, returns a polyline
**********************************************************************/

template< class EP >
void LinearizeLineCallback( void *usrdata, EP *p1, EP *p2 )
{
  List< ListNode<EP> > *L = (List< ListNode<EP> > *)usrdata;
  ListNode<EP> *node = new ListNode<EP>(*p2);
  L->Append(node);
}
template void LinearizeLineCallback( void *usrdata, point2<float> *p1, point2<float> *p2 );
template void LinearizeLineCallback( void *usrdata, point2<double> *p1, point2<double> *p2 );
template void LinearizeLineCallback( void *usrdata, point<float> *p1, point<float> *p2 );
template void LinearizeLineCallback( void *usrdata, point<double> *p1, point<double> *p2 );



template< class T, class HP, class EP >
EXP
void LinearizeCurve( int max_iter,
             const int n, const int p, Ptr< T > U, Ptr< HP > Pw,
             const T tol, const rtl<T>& t,
             int *ret_nP, Ptr< EP > *retP )
{
  List< ListNode<EP> > L;
  ListNode<EP> *node = new ListNode<EP>(euclid(Pw[0]));
  int i,nEl;
  Ptr< EP > P;

  L.Append( node );

  LinearizeCurve( max_iter, n, p, U, Pw, tol, t, 
                  LinearizeLineCallback<EP>, (void *)&L );

  nEl = L.nElems;
  node = L.head;
  P.reserve_pool( nEl );
  for( i = 0; i < nEl; i++ )
  {
    P[nEl-1-i] = node->el;
    node = node->next;
  }
  *ret_nP = nEl;
  *retP = P;
  
  L.DeleteElems();
}

template EXP
void LinearizeCurve( int max_iter,
          const int n, const int p, Ptr< double > U, Ptr< hpoint<double> > Pw,
          const double tol, const rtl<double>& t,
          int *nP, Ptr< point<double> > *P );
template EXP
void LinearizeCurve( int max_iter,
          const int n, const int p, Ptr< float > U, Ptr< hpoint<float> > Pw,
          const float tol, const rtl<float>& t,
          int *nP, Ptr< point<float> > *P );

template EXP
void LinearizeCurve( int max_iter,
          const int n, const int p, Ptr< double > U, Ptr< hpoint2<double> > Pw,
          const double tol, const rtl<double>& t,
          int *nP, Ptr< point2<double> > *P );
template EXP
void LinearizeCurve( int max_iter,
          const int n, const int p, Ptr< float > U, Ptr< hpoint2<float> > Pw,
          const float tol, const rtl<float>& t,
          int *nP, Ptr< point2<float> > *P );

}



