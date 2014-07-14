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
#include "gul_vector.h" 
#include "guge_normalize.h"
#include "guge_linear.h"
#include "guge_intersect.h"
#include "gunu_basics.h"
#include "gunu_divide.h"
#include "gunu_intersect.h"

namespace gunu {

using guge::ProjectionIsLinear;
using guge::CalcBoundingBoxE;

/*--------------------------------------------------------------------
  Check if a NURBS surface can be approximated with two triangles through
  its four corners. If not the surface is subdivided into four parts
----------------------------------------------------------------------*/

template< class T >
inline void LinearizeOrDivide( TessInfo<T> *A, const T tol, 
                               const rtl<T> t, bool need_bbox )
{
  int nu,nv,i,pu,pv;
  Ptr< T > U,V;
  Ptr< Ptr< hpoint< T > > > Pw;
  point<T> P00, P01, P10, P11, SP0, SP1, SP2, SP3, CP;
  bool subdiv;
  surface< T, hpoint<T> > S[4], surf;
  Ptr< hpoint<T> > buf;
  gul::Ptr< int > test;
 
  nu = A->org->nu;
  pu = A->org->pu;
  nv = A->org->nv;
  pv = A->org->pv;
  U = A->org->U;
  V = A->org->V;
  Pw = A->org->Pw;

  P00 = A->org->P00;
  P01 = A->org->P01;
  P10 = A->org->P10;
  P11 = A->org->P11;
  
  buf.reserve_place( reserve_stack(hpoint<T>,nv+1), nv+1 );
  
                          /*** Pruefen, ob Randkurven linear approximierbar: ***/
  subdiv = false;
                                                   /* --- obere Randkurve: --- */
  if( !guge::ProjectionIsLinear< T,hpoint<T>,point<T> >( nu, Pw[0], tol, t ) ) 
  {
    subdiv = true; 
    CurvePoint< T, hpoint<T>, point<T> >( (T)0.5, nu, pu, U, Pw[0], &SP0 );
  }
  else
  {
    SP0 = (T)0.5 * (P00 + P01);
  }
                                                  /* --- untere Randkurve: --- */
  if( !guge::ProjectionIsLinear<T,hpoint<T>,point<T> >( nu, Pw[nv], tol, t ) ) 
  {
    subdiv = true;
    CurvePoint< T, hpoint<T>, point<T> >( (T)0.5, nu, pu, U, Pw[nv], &SP1 );  
  }
  else
  {
    SP1 = (T)0.5 * (P10 + P11);
  }
                                                   /* --- linke Randkurve: --- */
  for( i = 0; i <= nv; i++ )
    buf[i] = Pw[i][0];
  if( !guge::ProjectionIsLinear<T,hpoint<T>,point<T> >( nv, buf, tol, t ) ) 
  {
    subdiv = true;
    CurvePoint< T, hpoint<T>, point<T> >( (T)0.5, nv, pv, V, buf, &SP2 );
  }
  else
  {
    SP2 = (T)0.5 * (P00 + P10);
  }
                                                  /* --- rechte Randkurve: --- */
  for( i = 0; i <= nv; i++ )
    buf[i] = Pw[i][nu];
  if( !guge::ProjectionIsLinear<T,hpoint<T>,point<T> >( nv, buf, tol, t ) ) 
  {
    subdiv = true;
    CurvePoint< T, hpoint<T>, point<T> >( (T)0.5, nv, pv, V, buf, &SP3 );
  }
  else
  {
    SP3 = (T)0.5 * (P01 + P11);
  }  
                          /* --- Pruefen ob Flaeche linear approxmierbar --- */
  if( !subdiv )
  {
    if( !guge::ProjectionIsPlanar<T>( nu, nv, Pw, tol, t ) )
      subdiv = true;
  }
                /*** Flaeche nicht linear approximierbar, also unterteilen: ***/
  if( subdiv )
  {
    surf.pu = pu;
    surf.pv = pv;
    surf.net.nu = nu;
    surf.net.nv = nv;  
    surf.net.Pw = Pw;
    surf.knu.m = nu+pu+1;
    surf.knu.U = U;
    surf.knv.m = nv+pv+1;
    surf.knv.U = V;
   
    SplitSurface< T, hpoint<T> >( &surf, 0.5, 0.5, &S[0], &S[1], &S[2], &S[3] );

    A->f.divided = 1;
    delete A->org; /* Speicher fuer ungeteilte Flaeche freigeben */
    A->org = 0;  
    for( i = 0; i < 4; i++ ) A->sub[i] = new TessInfo<T>();

    for( i = 0; i < 4; i++ )
    {
      if( need_bbox )
      {
        guge::CalcBoundingBoxH( (S[i].net.nu+1)*(S[i].net.nv+1), S[i].net.Pw[0], 
                                A->sub[i]->x1, A->sub[i]->x2, 
                                A->sub[i]->y1, A->sub[i]->y2,
                                A->sub[i]->z1, A->sub[i]->z2 );
      }
      A->sub[i]->org->nu = S[i].net.nu;
      A->sub[i]->org->pu = S[i].pu;
      A->sub[i]->org->nv = S[i].net.nv;
      A->sub[i]->org->pv = S[i].pv;
      A->sub[i]->org->U = S[i].knu.U;
      A->sub[i]->org->V = S[i].knv.U;
      A->sub[i]->org->Pw = S[i].net.Pw;
    }
    CP = euclid( A->sub[0]->org->Pw[ A->sub[0]->org->nv ][ A->sub[0]->org->nu ] );

    A->sub[0]->org->P00 = P00;
    A->sub[0]->org->P01 = SP0;
    A->sub[0]->org->P10 = SP2;
    A->sub[0]->org->P11 = CP;
    A->sub[0]->u1 = A->u1;
    A->sub[0]->u2 = (A->u1 + A->u2) / (T)2.0;
    A->sub[0]->v1 = A->v1;
    A->sub[0]->v2 = (A->v1 + A->v2) / (T)2.0;    
    
    A->sub[1]->org->P00 = SP0;
    A->sub[1]->org->P01 = P01;
    A->sub[1]->org->P10 = CP;
    A->sub[1]->org->P11 = SP3;
    A->sub[1]->u1 = (A->u1 + A->u2) / (T)2.0;;
    A->sub[1]->u2 = A->u2;
    A->sub[1]->v1 = A->v1;
    A->sub[1]->v2 = (A->v1 + A->v2) / (T)2.0;    

    A->sub[2]->org->P00 = SP2;
    A->sub[2]->org->P01 = CP;
    A->sub[2]->org->P10 = P10;
    A->sub[2]->org->P11 = SP1;
    A->sub[2]->u1 = A->u1;
    A->sub[2]->u2 = (A->u1 + A->u2) / (T)2.0;
    A->sub[2]->v1 = (A->v1 + A->v2) / (T)2.0;
    A->sub[2]->v2 = A->v2;    

    A->sub[3]->org->P00 = CP;
    A->sub[3]->org->P01 = SP3;
    A->sub[3]->org->P10 = SP1;
    A->sub[3]->org->P11 = P11;
    A->sub[3]->u1 = (A->u1 + A->u2) / (T)2.0;;
    A->sub[3]->u2 = A->u2;
    A->sub[3]->v1 = (A->v1 + A->v2) / (T)2.0;
    A->sub[3]->v2 = A->v2;    
  }
  else
  {                    /****** Flaeche durch 2 Dreiecke approximierbar *****/
    A->f.linearized = 1;
    A->f.divided = 0;
  }
  return;
}

/*--------------------------------------------------------------------
  Check if a NURBS surface can be approximated with two triangles through
  its four corners. If not the surface is subdivided into four parts
----------------------------------------------------------------------*/
template
void LinearizeOrDivide( TessInfo<float> *A, const float tol, 
                        const rtl<float> t );
template
void LinearizeOrDivide( TessInfo<double> *A, const double tol, 
                        const rtl<double> t );

/*-------------------------------------------------------------------
  Approximate NURBS-Surface with triangles
--------------------------------------------------------------------*/
template< class T >
void LinearizeSurface( 
                 int current_iter, int max_iter,
                 TessInfo<T> *A,
                 const T tol, const rtl<T>& t,
                 void    outfunc( TessInfo<T> *, void * ),                                   
                 void   *outfunc_data )
{
  int nA,i;
  TessInfo<T> **pA;  

/* --- A vierteln falls A nicht durch Dreiecke approximierbar: */    

  if( !A->f.linearized && !A->f.divided )
  {      
    if( current_iter < max_iter )
      gunu::LinearizeOrDivide( A, tol, t, false );
    else
      A->f.linearized = 1;
  }

  if( !A->f.linearized )
  {
    nA = 4;
    pA = A->sub;

    for( i = 0; i < nA; i++ )          /* Rekursion */
    {
      LinearizeSurface( current_iter+1, max_iter, pA[i], tol, t,
                        outfunc, outfunc_data );
      delete pA[i];
      pA[i] = 0;      
    }
  }
  else       /* ------- Dreiecke ausgeben ------------------ */  
  {
    outfunc( A, outfunc_data ); 
  }
}
/*-------------------------------------------------------------------
  Approximate NURBS-Surface with triangles
--------------------------------------------------------------------*/
template
void LinearizeSurface( 
                 int current_iter, int max_iter,
                 TessInfo<float> *A,
                 const float tol, const rtl<float>& t,
                 void    outfunc( TessInfo<float> *, void * ),                                   
                 void   *outfunc_data );
template
void LinearizeSurface( 
                 int current_iter, int max_iter,
                 TessInfo<double> *A,
                 const double tol, const rtl<double>& t,
                 void    outfunc( TessInfo<double> *, void * ),                                   
                 void   *outfunc_data );

#if 0
/*---------------------------------------------------------------------
  Intersect linear approximation of two NURBS-Surfaces
----------------------------------------------------------------------*/
template< class T >
void IntersectSurfaces( 
                 IntersectInfo<T> *A, IntersectInfo<T> *B,
                 T tol, const rtl<T>& t,
                 void    outfunc( IntersectionLineInfo<T> *, void * ),                                   
                 void   *outfunc_data )
{
  int result, nA, nB, i, j;
  IntersectInfo<T> **pA, **pB;  
  IntersectionLineInfo<T> linfo;
  T u,v;
  
/* --- Ueberschneiden sich die Bounding Boxen ? ----------- */  

  if( (A->x1 > B->x2) || (A->x2 < B->x1) ||
      (A->y1 > B->y2) || (A->y2 < B->y1) ||
      (A->z1 > B->z2) || (A->z2 < B->z1) )
  {
    return;
  }   
    
/* --- A vierteln falls A nicht durch Dreiecke approximierbar: */    

  if( !A->f.linearized && !A->f.divided )
  {      
    LinearizeOrDivide<T>( A, tol, t );
  }
  if( !B->f.linearized && !B->f.divided )
  {      
    LinearizeOrDivide<T>( B, tol, t );
  }

/* --- falls noch nicht beide Flaechen durch Dreiecke darstellbar Rekursion: */

  if( !A->f.linearized || !B->f.linearized )
  {
    if( !A->f.linearized )
    {
      nA = 4;
      pA = A->sub;
    }
    else
    {
      nA = 1;
      pA = &A;        
    }

    if( !B->f.linearized )
    {
      nB = 4;
      pB = B->sub;
    }
    else
    {
      nB = 1;
      pB = &B;        
    }

    for( i = 0; i < nA; i++ )          /* Rekursion */
    {
      for( j = 0; j < nB; j++ )
      {
        IntersectSurfaces( pA[i], pB[j], tol, t, outfunc, outfunc_data );
      }      
    }
  }
  else       /* ------- Dreiecke schneiden ------------------ */  
  {
    point<T> P00,P01,P10,P11;
    triangle<T> Atri[2], Btri[2];
    
    P00 = A->org.P00;
    P01 = A->org.P01;
    P10 = A->org.P10;
    P11 = A->org.P11;

    Atri[0].P1 = P00;
    Atri[0].P2 = P10;
    Atri[0].P3 = P01;

    Atri[1].P1 = P10;
    Atri[1].P2 = P11;
    Atri[1].P3 = P01;

    P00 = B->org.P00;
    P01 = B->org.P01;
    P10 = B->org.P10;
    P11 = B->org.P11;

    Btri[0].P1 = P00;
    Btri[0].P2 = P10;
    Btri[0].P3 = P01;

    Btri[1].P1 = P10;
    Btri[1].P2 = P11;
    Btri[1].P3 = P01;


    result = guge::IntersectTriangles<T>( Atri[0], Btri[0],
                                    &linfo.P1, &linfo.P2 );
    if( result != 0 )
    {
      /* --- Parameterraumdarstellung bzgl. beider Flaechen bestimmen: --- */

      BarycentricUV<T>( Atri[0].P1, Atri[0].P3, Atri[0].P2,
                        linfo.P1, &u, &v ); 
      linfo.F1.P1.x = A->u1 + u * (A->u2 - A->u1);
      linfo.F1.P1.y = A->v1 + v * (A->v2 - A->v1);

      BarycentricUV( Atri[0].P1, Atri[0].P3, Atri[0].P2,
                     linfo.P2, &u, &v ); 
      linfo.F1.P2.x = A->u1 + u * (A->u2 - A->u1);
      linfo.F1.P2.y = A->v1 + v * (A->v2 - A->v1);

      /* --- 2. Flaeche: --- */

      BarycentricUV<T>( Btri[0].P1, Btri[0].P3, Btri[0].P2,
                        linfo.P1, &u, &v ); 
      linfo.F2.P1.x = B->u1 + u * (B->u2 - B->u1);
      linfo.F2.P1.y = B->v1 + v * (B->v2 - B->v1);

      BarycentricUV<T>( Btri[0].P1, Btri[0].P3, Btri[0].P2,
                        linfo.P2, &u, &v ); 
      linfo.F2.P2.x = B->u1 + u * (B->u2 - B->u1);
      linfo.F2.P2.y = B->v1 + v * (B->v2 - B->v1);

      /* --- abspeichern: --- */

      outfunc( &linfo, outfunc_data );
    }
    
/* ------------------------------------------------------------------------ */

    result = guge::IntersectTriangles<T>( Atri[0], Btri[1],
                                    &linfo.P1, &linfo.P2 );
    if( result != 0 )
    {
      /* --- Parameterraumdarstellung bzgl. beider Flaechen bestimmen: --- */

      BarycentricUV<T>( Atri[0].P1, Atri[0].P3, Atri[0].P2,
                        linfo.P1, &u, &v ); 
      linfo.F1.P1.x = A->u1 + u * (A->u2 - A->u1);
      linfo.F1.P1.y = A->v1 + v * (A->v2 - A->v1);

      BarycentricUV<T>( Atri[0].P1, Atri[0].P3, Atri[0].P2,
                        linfo.P2, &u, &v ); 
      linfo.F1.P2.x = A->u1 + u * (A->u2 - A->u1);
      linfo.F1.P2.y = A->v1 + v * (A->v2 - A->v1);

      /* --- 2. Flaeche: --- */

      BarycentricUV<T>( Btri[1].P2, Btri[1].P1, Btri[1].P3,
                        linfo.P1, &u, &v ); 
      linfo.F2.P1.x = B->u2 - u * (B->u2 - B->u1);
      linfo.F2.P1.y = B->v2 - v * (B->v2 - B->v1);

      BarycentricUV<T>( Btri[1].P2, Btri[1].P1, Btri[1].P3,
                        linfo.P2, &u, &v ); 
      linfo.F2.P2.x = B->u2 - u * (B->u2 - B->u1);
      linfo.F2.P2.y = B->v2 - v * (B->v2 - B->v1);

      /* --- abspeichern: --- */

      outfunc( &linfo, outfunc_data ); 
    }

/* ------------------------------------------------------------------------ */

    result = guge::IntersectTriangles<T>( Atri[1], Btri[0],
                                    &linfo.P1, &linfo.P2 );
    if( result != 0 )
    {
      /* --- Parameterraumdarstellung bzgl. beider Flaechen bestimmen: --- */

      BarycentricUV<T>( Atri[1].P2, Atri[1].P1, Atri[1].P3,
                        linfo.P1, &u, &v ); 
      linfo.F1.P1.x = A->u2 - u * (A->u2 - A->u1);
      linfo.F1.P1.y = A->v2 - v * (A->v2 - A->v1);

      BarycentricUV<T>( Atri[1].P2, Atri[1].P1, Atri[1].P3,
                        linfo.P2, &u, &v ); 
      linfo.F1.P2.x = A->u2 - u * (A->u2 - A->u1);
      linfo.F1.P2.y = A->v2 - v * (A->v2 - A->v1);

      /* --- 2. Flaeche: --- */

      BarycentricUV<T>( Btri[0].P1, Btri[0].P3, Btri[0].P2,
                        linfo.P1, &u, &v ); 
      linfo.F2.P1.x = B->u1 + u * (B->u2 - B->u1);
      linfo.F2.P1.y = B->v1 + v * (B->v2 - B->v1);

      BarycentricUV<T>( Btri[0].P1, Btri[0].P3, Btri[0].P2,
                        linfo.P2, &u, &v ); 
      linfo.F2.P2.x = B->u1 + u * (B->u2 - B->u1);
      linfo.F2.P2.y = B->v1 + v * (B->v2 - B->v1);

      /* --- abspeichern: --- */

      outfunc( &linfo, outfunc_data ); 
    }

/* ---------------------------------------------------------------------- */

    result = guge::IntersectTriangles<T>( Atri[1], Btri[1],
                                    &linfo.P1, &linfo.P2 );
    if( result != 0 )
    {
      /* --- Parameterraumdarstellung bzgl. beider Flaechen bestimmen: --- */

      BarycentricUV<T>( Atri[1].P2, Atri[1].P1, Atri[1].P3,
                        linfo.P1, &u, &v ); 
      linfo.F1.P1.x = A->u2 - u * (A->u2 - A->u1);
      linfo.F1.P1.y = A->v2 - v * (A->v2 - A->v1);

      BarycentricUV<T>( Atri[1].P2, Atri[1].P1, Atri[1].P3,
                        linfo.P2, &u, &v ); 
      linfo.F1.P2.x = A->u2 - u * (A->u2 - A->u1);
      linfo.F1.P2.y = A->v2 - v * (A->v2 - A->v1);

      /* --- 2. Flaeche: --- */

      BarycentricUV<T>( Btri[1].P2, Btri[1].P1, Btri[1].P3,
                        linfo.P1, &u, &v ); 
      linfo.F2.P1.x = B->u2 - u * (B->u2 - B->u1);
      linfo.F2.P1.y = B->v2 - v * (B->v2 - B->v1);

      BarycentricUV<T>( Btri[1].P2, Btri[1].P1, Btri[1].P3,
                        linfo.P2, &u, &v ); 
      linfo.F2.P2.x = B->u2 - u * (B->u2 - B->u1);
      linfo.F2.P2.y = B->v2 - v * (B->v2 - B->v1);

      /* --- abspeichern: --- */

      outfunc( &linfo, outfunc_data ); 
    }
  }
}
/*---------------------------------------------------------------------
  Intersect linear approximation of two NURBS-Surfaces
----------------------------------------------------------------------*/
template
void IntersectSurfaces( 
                 IntersectInfo<float> *A, IntersectInfo<float> *B,
                 float tol, const rtl<float>& t,
                 void    outfunc( IntersectionLineInfo<float> *, void * ),                                   
                 void   *outfunc_data );
template
void IntersectSurfaces( 
                 IntersectInfo<double> *A, IntersectInfo<double> *B,
                 double tol, const rtl<double>& t,
                 void    outfunc( IntersectionLineInfo<double> *, void * ),                                   
                 void   *outfunc_data );
#endif

}
