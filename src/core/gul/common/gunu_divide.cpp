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
#include "gunu_basics.h"
#include "gunu_divide.h"

namespace gunu {

using gul::rtr;
using gul::point;
using gul::point1;
using gul::point2;
using gul::hpoint;
using gul::hpoint1;
using gul::hpoint2;
using gul::set;
using gul::euclid;
using gul::ortho;

/*----------------------------------------------------------------------
  Splits a NURBS curve at point 'u' into two parts
----------------------------------------------------------------------- */ 
template< class T, class HP >
void SplitCurve( const curve<T,HP> *Curve, const T u,
                 curve<T,HP> *C1, curve<T,HP> *C2 ) 
{
  int i,j,k,s,r,ir,n,n1,n2,p;
  T alpha;
  Ptr< T > U,U1,U2;
  Ptr< HP > P,R,NP,Q1,Q2;

  p = Curve->p;
  n = Curve->cpt.n;
  P = Curve->cpt.Pw;
  U = Curve->knt.U;
  k = FindSpanMultip( u, n, p, U, &s );
  r = p - s;
  
/* Speicher fuer Ausgabekurven C1,C2 reservieren */

  n1 = k - s;
  n2 = n + r - (k - s);

  Q1.reserve_pool( n1+1 );
  Q2.reserve_pool( n2+1 );
  U1.reserve_pool( n1 + p + 2 );
  U2.reserve_pool( n2 + p + 2 );
    
  C1->p = p;
  C1->cpt.n = n1;
  C1->cpt.Pw = Q1;
  C1->knt.m = n1 + p + 1;
  C1->knt.U = U1;

  C2->p = p;
  C2->cpt.n = n2;
  C2->cpt.Pw = Q2;
  C2->knt.m = n2 + p + 1;
  C2->knt.U = U2;

/* Array fuer Hilfspunkte reservieren */

  R.reserve_place( reserve_stack(HP,p+1), p+1 );
  if( p-s+r-1 > 0 ) NP.reserve_place( reserve_stack(HP, p-s+r-1), p-s+r-1 );
  
  for( i = 0; i <= p - s; i++ )
  {
    R[i] = P[k - p + i]; 
  }

  for( ir = 1; ir <= r; ir++ )
  {
    for( i = 0; i <= p - s - ir; i++ )
    {
      alpha = (u - U[k-p+ir+i]) / (U[k+i+1] - U[k-p+ir+i]);

      R[i] = (alpha * R[i+1]) + (((T)1.0 - alpha) * R[i]);
    }

    NP[ir - 1] = R[0];
    NP[p - s + r - 1 - ir] = R[p - s - ir];
  }

  for( i = 1; i < p - s - r; i++ ) 
  {
    NP[r - 1 + i] = R[i];
  }

  for( i = 0; i <= k - p; i++ )
  {
    Q1[i] = P[i];  
  }                                          

  for( i = 0; i <= p - s - 1; i++ )
  {
    j = k - p + 1 + i;
    Q1[j] = NP[i];
  }

  for( i = p - s - 1; i <= p - s + r - 2; i++ )
  {
    j = i - (p - s - 1);  
    Q2[j] = NP[i];
  }
    
  for( i = k - s; i <= n; i++ )
  {
    j = r + i - (k - s);
    Q2[j] = P[i];    
  }

/* Knotenvektoren der 2 Teilkurven bilden */

  for( i = 0; i < k - s + 1; i++ )
    U1[i] = U[i];
  for( i = 0; i <= p; i++ )
    U1[k - s + 1 + i] = u;
  NormalizeKnotVector( n1, p, U1 );

  for( i = 0; i < n + p + 1 - k; i++ )
    U2[p+1+i] = U[k+1+i];
  for( i = 0; i <= p; i++ )
    U2[i] = u;
  NormalizeKnotVector( n2, p, U2 );
}
/*----------------------------------------------------------------------
  Splits a NURBS curve at point 'u' into two parts
----------------------------------------------------------------------- */ 
template
void SplitCurve( 
           const curve< float, hpoint<float> > *Curve, const float u,
           curve< float, hpoint<float> > *C1, 
           curve< float, hpoint<float> > *C2 );
template
void SplitCurve( 
           const curve< double, hpoint<double> > *Curve, const double u,
           curve< double, hpoint<double> > *C1, 
           curve< double, hpoint<double> > *C2 );

template
void SplitCurve( 
           const curve< float, hpoint2<float> > *Curve, const float u,
           curve< float, hpoint2<float> > *C1, 
           curve< float, hpoint2<float> > *C2 );
template
void SplitCurve( 
           const curve< double, hpoint2<double> > *Curve, const double u,
           curve< double, hpoint2<double> > *C1, 
           curve< double, hpoint2<double> > *C2 );

/*----------------------------------------------------------------------
  Splits a NURBS surface at point (u,v) into four parts
----------------------------------------------------------------------- */ 
template< class T, class HP >
void SplitSurface( 
            const surface<T,HP> *Surf, const T u, const T v, 
            surface<T,HP> *S11, surface<T,HP> *S12, 
            surface<T,HP> *S21, surface<T,HP> *S22 )
{
  int nrows,ncols;
  int i,j,ir;
  int pu,pv;
  int nu,nv,nu1,nu2,nv1,nv2;
  int ku,kv,su,sv,ru,rv,col,row;

  Ptr< T > U,V,U11,U12,U21,U22,V11,V12,V21,V22;
  Ptr< HP * > NR;
  Ptr< HP > NR_buf,R;
  Ptr< Ptr< HP > > NP,P,Q11,Q12,Q21,Q22;
  Ptr< Ptr< T > > alpha;

/* Lokale Notation: */

  pu = Surf->pu;
  pv = Surf->pv;
  nu = Surf->net.nu;
  nv = Surf->net.nv;
  P =  Surf->net.Pw;
  U = Surf->knu.U;
  V = Surf->knv.U;

  ku = FindSpanMultip( u, nu, pu, U, &su );
  kv = FindSpanMultip( v, nv, pv, V, &sv );
  ru = pu - su;
  rv = pv - sv;

/* Speicher fuer Ausgabeflaechen S11,S12,S21,S22 reservieren */

  nv1 = kv - sv;
  nv2 = nv + rv - (kv - sv);

  Q11.reserve_pool( nv1 + 1 );
  Q12.reserve_pool( nv1 + 1 );
  Q21.reserve_pool( nv2 + 1 );
  Q22.reserve_pool( nv2 + 1 );

  nu1 = ku - su; 
  nu2 = nu + ru - (ku - su);

  for( i = 0; i <= nv1; i++ )
  {
    Q11[i].reserve_pool( nu1 + 1 );
    Q12[i].reserve_pool( nu2 + 1 );
  }
  for( i = 0; i <= nv2; i++ )
  {
    Q21[i].reserve_pool( nu1 + 1 );
    Q22[i].reserve_pool( nu2 + 1 );
  }
  
  U11.reserve_pool( nu1 + pu + 2 );
  U21.reserve_pool( nu1 + pu + 2 );
  U12.reserve_pool( nu2 + pu + 2 );
  U22.reserve_pool( nu2 + pu + 2 );

  V11.reserve_pool( nv1 + pv + 2 );
  V12.reserve_pool( nv1 + pv + 2 );
  V21.reserve_pool( nv2 + pv + 2 );
  V22.reserve_pool( nv2 + pv + 2 );

  S11->pu = pu;
  S11->pv = pv;
  S11->net.nu = nu1;
  S11->net.nv = nv1;
  S11->net.Pw = Q11;
  S11->knu.m = nu1 + pu + 1;
  S11->knu.U = U11;
  S11->knv.m = nv1 + pv + 1;
  S11->knv.U = V11;

  S12->pu = pu;
  S12->pv = pv;
  S12->net.nu = nu2;
  S12->net.nv = nv1;
  S12->net.Pw = Q12;
  S12->knu.m = nu2 + pu + 1;
  S12->knu.U = U12;  
  S12->knv.m = nv1 + pv + 1;
  S12->knv.U = V12;
  
  S21->pu = pu;
  S21->pv = pv;
  S21->net.nu = nu1;
  S21->net.nv = nv2;
  S21->net.Pw = Q21;
  S21->knu.m = nu1 + pu + 1;
  S21->knu.U = U21;
  S21->knv.m = nv2 + pv + 1;
  S21->knv.U = V21;

  S22->pu = pu;
  S22->pv = pv;
  S22->net.nu = nu2;
  S22->net.nv = nv2;
  S22->net.Pw = Q22;
  S22->knu.m = nu2 + pu + 1;
  S22->knu.U = U22;  
  S22->knv.m = nv2 + pv + 1;
  S22->knv.U = V22;

/* Array fuer alphas reservieren */

  nrows = rv > ru ? rv : ru;
  ncols = pv > pu ? pv : pu; 

  alpha.reserve_place( reserve_stack(Ptr< T >,nrows), nrows );
  for( i = 0; i < nrows; i++ )
    alpha[i].reserve_place( reserve_stack(T,ncols), ncols );

/* Array fuer Hilfspunkte reservieren */

  R.reserve_place( reserve_stack(HP,ncols+1), ncols+1 );

  NR.reserve_place( reserve_stack(HP*,nv+rv+1), nv+rv+1 );
  for( i = 0; i <= kv - pv; i++ )
    NR[i] = P[i].get();
  for( i = kv - sv; i <= nv; i++ )
  {
    j = rv + i; 
    NR[j] = P[i].get();   
  } 
  if( rv > 0 )
  {
    NR_buf.reserve_place( reserve_stack(HP,(nu+1)*(pv-sv+rv-1)),
                          (nu+1)*(pv-sv+rv-1) );

    for( i = 0; i < pv - sv + rv - 1; i++ )
    {
      j = kv - pv + 1 + i; 
      NR[j] = &NR_buf[i * (nu + 1)];
    }
  }
  
  if( ru > 0 )
  {
    NP.reserve_place( reserve_stack(Ptr< HP >,nv+rv+1), nv+rv+1 );
    for( i = 0; i < nv+rv+1; i++ )
      NP[i].reserve_place( reserve_stack(HP,pu-su+ru-1), pu-su+ru-1 ); 
  }

/* Alphas fuer V-Richtung berechnen: */

  for( ir = 1; ir <= rv; ir++ )
  {
    for( i = kv - pv + ir; i <= kv - sv; i++ )
    {
      alpha[ir - 1][i - (kv - pv + ir)] = 
                                   (v - V[i]) /  (V[i + pv - ir + 1] - V[i]);
    } 
  }

/* Neue Kontrollpunkt-Zeilen berechnen, NR enthaelt nach Ausfuehrung 
   neue + unveraenderte Zeilen */

  ncols = nu;
  
  for( col = 0; col <= ncols; col++ )
  {
    /* Hilfspunkte R initialisieren */

    for( i = 0; i <= pv - sv; i++ )
    {
      R[i] = P[kv - pv + i][col];
    }

    for( ir = 1; ir <= rv; ir++ )
    {
      for( i = 0; i <= pv - sv - ir; i++ )
      {
        R[i] = (alpha[ir-1][i] * R[i+1]) + (((T)1.0 - alpha[ir-1][i]) * R[i]); 
      }

      NR[kv - pv + ir][col] = R[0];    
      NR[kv - sv + rv - ir][col] = R[pv - sv - ir];    
    }

    for( i = 1; i < pv - sv - rv; i++ ) 
      NR[kv - pv + rv + i][col] = R[i];
  }

/* Neue Kontrollpunkte berechnen fuer jede Zeile row */
/* NP enthaelt nach Ausfuehrung die neu entstandenen Kontrollpunkte */

/* Alphas berechnen: */

  for( ir = 1; ir <= ru; ir++ )
  {
    for( i = ku - pu + ir; i <= ku - su; i++ )
    {
      alpha[ir - 1][i - (ku - pu + ir)] = 
                                    (u - U[i]) /  (U[i + pu - ir + 1] - U[i]);
    } 
  }

/* Neue Kontrollpunkte NP berechnen, fuer jede Zeile row */

  nrows = nv + rv;
  
  for( row = 0; row <= nrows; row++ )
  {
    /* Hilfspunkte R initialisieren */

    for( i = 0; i <= pu - su; i++ )
    {
      R[i] = NR[row][ku - pu + i];
    }

    for( ir = 1; ir <= ru; ir++ )
    {
      for( i = 0; i <= pu - su - ir; i++ )
      {
        R[i] = (alpha[ir-1][i] * R[i+1]) + (((T)1.0 - alpha[ir-1][i]) * R[i]); 
      }

      NP[row][ir-1] = R[0];    
      NP[row][pu - su + ru - 1 - ir] = R[pu - su - ir];    
    }

    for( i = 1; i < pu - su - ru; i++ ) 
      NP[row][ru - 1 + i] = R[i];
  }

/* Kontrollpunkt-Netze der 4 Teilflaechen bilden */

  for( row = 0; row <= kv - sv; row++ )
  {
    for( i = 0; i <= ku - pu; i++ )
    {
      Q11[row][i] = NR[row][i];  
    }
    
    for( i = 0; i <= pu - su - 1; i++ )
    {
      j = ku - pu + 1 + i;
      Q11[row][j] = NP[row][i];
    }

    for( i = pu - su - 1; i <= pu - su + ru - 2; i++ )
    {
      j = i - (pu - su - 1);  
      Q12[row][j] = NP[row][i];
    }
    
    for( i = ku - su; i <= nu; i++ )
    {
      j = ru + i - (ku - su);
      Q12[row][j] = NR[row][i];    
    }
  }

  for( row = kv - sv; row <= nv + rv; row++ )
  {
    for( i = 0; i <= ku - pu; i++ )
    {
      Q21[row - (kv - sv)][i] = NR[row][i];  
    }
    
    for( i = 0; i <= pu - su - 1; i++ )
    {
      j = ku - pu + 1 + i;
      Q21[row - (kv - sv)][j] = NP[row][i];
    }
  
    for( i = pu - su - 1; i <= pu - su + ru - 2; i++ )
    {
      j = i - (pu - su - 1);  
      Q22[row - (kv - sv)][j] = NP[row][i];
    }

    for( i = ku - su; i <= nu; i++ )
    {
      j = ru + i - (ku - su);
      Q22[row - (kv - sv)][j] = NR[row][i];    
    }
  }

/* Knotenvektoren der 4 Teilflaechen bilden */

  for( i = 0; i <  ku - su + 1; i++ )
    U11[i] = U[i];
  for( i = 0; i <= pu; i++ )
    U11[ku - su + 1 + i] = u;
  NormalizeKnotVector( nu1, pu, U11 );
  for( i = 0; i < ku - su + pu + 2; i++ )
    U21[i] = U11[i];

  for( i = 0; i < nu + pu + 1 - ku; i++ )
    U12[pu+1+i] = U[ku+1+i];
  for( i = 0; i <= pu; i++ )
    U12[i] = u;
  NormalizeKnotVector( nu2, pu, U12 );
  for( i = 0; i < nu2 + pu + 2; i++ )
    U22[i] = U12[i];


  for( i = 0; i < kv - sv + 1; i++ )
    V11[i] = V[i];
  for( i = 0; i <= pv; i++ )
    V11[kv - sv + 1 + i] = v;
  NormalizeKnotVector( nv1, pv, V11 );
  for( i = 0; i < kv - sv + pv + 2; i++ )
    V12[i] = V11[i];

  for( i = 0; i < nv + pv + 1 - kv; i++ )
    V21[pv+1+i] = V[kv+1+i];
  for( i = 0; i <= pv; i++ )
    V21[i] = v;
  NormalizeKnotVector( nv2, pv, V21 );
  for( i = 0; i < nv2 + pv + 2; i++ )
    V22[i] = V21[i];
}
/*----------------------------------------------------------------------
  Splits a NURBS surface at point (u,v) into four parts
----------------------------------------------------------------------- */ 
template void SplitSurface( 
   const surface< float, hpoint<float> > *Surf, const float u, const float v, 
   surface< float, hpoint<float> > *S11, surface< float, hpoint<float> > *S12, 
   surface< float, hpoint<float> > *S21, surface< float, hpoint<float> > *S22 );

template void SplitSurface( 
const surface< double, hpoint<double> > *Surf, const double u, const double v, 
surface< double, hpoint<double> > *S11, surface< double, hpoint<double> > *S12, 
surface< double, hpoint<double> > *S21, surface< double, hpoint<double> > *S22);


/*----------------------------------------------------------------------
  Extracts a part of a NURBS curve (ranging from 'u1' to 'u2')
----------------------------------------------------------------------*/  
template< class T, class HP >
void ExtractCurve( 
         const T u1, const T u2,
         const int n, const int p, 
         Ptr< T > U, Ptr< HP > Pw,
         int *retn, Ptr< T > *retU, Ptr< HP > *retPw )
{
  int a,ra,sa,b,rb,sb,N,i,r; 
  HP v1,v2;
  T alpha;

  Ptr< HP > Cw;
  Ptr< T > Uh;

  a = FindSpanMultip( u1, n, p, U, &sa );
  if( sa > p )          /* Klammerknoten am Anfang */
    ra = 0;
  else
    ra = p - sa;

  b = FindSpanMultip( u2, n, p, U, &sb );
  if( sb > p )         /* Klammerknoten am Ende */
  {
   sb = p;
   rb = 0;
   b += p;
  }
  else 
    rb = p - sb;
   
  N = (b-sb) - a + p;

  Cw.reserve_pool( N+1 );
  Uh.reserve_pool( N+p+2 );
 
/* ---- Linken Knoten einfuegen, bis Vielfachheit = p ------------------- */

  for( i = 0; i <= p; i++ )
  {
    Cw[i] = Pw[a-p+i];
    Uh[i] = u1;
  }
     
  for( r = 1; r <= ra; r++ )
  {
    for( i = 0; i <= ra-r; i++ )
    {
      alpha = (u1 - U[a-p+r+i]) / (U[a+r+i] - U[a-p+r+i]);
      v1 = ((T)1.0-alpha) * Cw[i];
      v2 = alpha * Cw[i+1];
      Cw[i] =  v1 + v2;
    }
  }     

/* ---- Unveraenderten Teil uebernehmen ----------------------------------- */

  for( i = 0; i < (b-sb)-a; i++ )
  {
    Uh[p+1+i] = U[a+1+i];
    Cw[p+1+i] = Pw[a+1+i];
  }
  
/* ---- Rechten Knoten einfuegen, bis Vielfachheit = p --------------------- */   

  for( r = 1; r <= rb; r++ )
  {
    for( i = 0; i <= rb-r; i++ )
    {
      alpha = (u2 - U[b-sb-i]) / (U[b-sb-i+p] - U[b-sb-i]);
      v1 = alpha * Cw[N-i];
      v2 = ((T)1.0-alpha) * Cw[N-i-1];
      Cw[N-i] = v1 + v2;      
    }
  }

  for( i = 0; i <= p; i++ )
    Uh[N+1+i] = u2;

  NormalizeKnotVector( N, p, Uh );

  *retn = N;
  *retPw = Cw;
  *retU = Uh;  
}         
/*----------------------------------------------------------------------
  Extracts a part of a NURBS curve (ranging from 'u1' to 'u2')
----------------------------------------------------------------------*/  
template
void ExtractCurve( 
         const float u1, const float u2,
         const int n, const int p, 
         Ptr< float > U, Ptr< hpoint<float> > Pw,
         int *retn, Ptr< float > *retU, Ptr< hpoint<float> > *retPw );
template
void ExtractCurve( 
         const double u1, const double u2,
         const int n, const int p, 
         Ptr< double > U, Ptr< hpoint<double> > Pw,
         int *retn, Ptr< double > *retU, Ptr< hpoint<double> > *retPw );
template
void ExtractCurve( 
         const float u1, const float u2,
         const int n, const int p, 
         Ptr< float > U, Ptr< hpoint2<float> > Pw,
         int *retn, Ptr< float > *retU, Ptr< hpoint2<float> > *retPw );
template
void ExtractCurve( 
         const double u1, const double u2,
         const int n, const int p, 
         Ptr< double > U, Ptr< hpoint2<double> > Pw,
         int *retn, Ptr< double > *retU, Ptr< hpoint2<double> > *retPw );



/*----------------------------------------------------------------------
  Extracts a part of a NURBS surface (ranging from 'u1' to 'u2' and 
  'v1' to 'v2')
----------------------------------------------------------------------*/  
template< class T, class HP >
void ExtractSurface( 
         const T u1, const T u2,
         const T v1, const T v2,
         const int nu, const int pu, Ptr< T > U,
         const int nv, const int pv, Ptr< T > V,
         Ptr< Ptr< HP > > Pw,
         int *ret_nu, Ptr< T > *retU, 
         int *ret_nv, Ptr< T > *retV, 
         Ptr< Ptr< HP > > *retPw )
{
  int a,sa,ra,b,sb,rb,c,sc,rc,d,sd,rd,r,Nv,Nu,i,j,row,col;
  HP V1,V2;
  T alpha;

  Ptr< Ptr< HP > > Sw;
  Ptr< T > Uh,Vh;

  a = FindSpanMultip( v1, nv, pv, V, &sa );
  if( sa > pv )          /* Klammerknoten am Anfang */
    ra = 0;
  else
    ra = pv - sa;

  b = FindSpanMultip( v2, nv, pv, V, &sb );
  if( sb > pv )         /* Klammerknoten am Ende */
  {
   sb = pv;
   rb = 0;
   b += pv;
  }
  else 
    rb = pv - sb;
   
  Nv = (b-sb) - a + pv;

  c = FindSpanMultip( u1, nu, pu, U, &sc );
  if( sc > pu )          /* Klammerknoten am Anfang */
    rc = 0;
  else
    rc = pu - sc;

  d = FindSpanMultip( u2, nu, pu, U, &sd );
  if( sd > pu )         /* Klammerknoten am Ende */
  {
   sd = pu;
   rd = 0;
   d += pu;
  }
  else 
    rd = pu - sd;
   
  Nu = (d-sd) - c + pu;

  Sw.reserve_pool( Nv+1 );
  for( i = 0; i < Nv+1; i++ )
    Sw[i].reserve_pool( Nu+1 ); 

  Uh.reserve_pool( Nu+pu+2 );
  Vh.reserve_pool( Nv+pv+2 );

  for( i = 0; i <= Nv; i++ )
    for( j = 0; j < Nu+1; j++ )
      Sw[i][j] = Pw[a-pv+i][c-pu+j];
 
/*-----------------------------------------------------------------------
  V-Richtung
----------------------------------------------------------------------- */  

/* ---- Oberen Knoten einfuegen, bis Vielfachheit = p ------------------- */

  for( i = 0; i <= pv; i++ )
    Vh[i] = v1;

  for( r = 1; r <= ra; r++ )
  {
    for( i = 0; i <= ra-r; i++ )
    {
      alpha = (v1 - V[a-pv+r+i]) / (V[a+r+i] - V[a-pv+r+i]);
      for( col = 0; col <= Nu; col++ )
      {     
        V1 = ((T)1.0-alpha) * Sw[i][col];
        V2 = alpha * Sw[i+1][col];
        Sw[i][col] = V1 + V2;
      }
    }
  }     

/* ---- Unveraenderten Teil uebernehmen ----------------------------------- */

  for( i = 0; i < b-sb-a; i++ )
    Vh[pv+1+i] = V[a+1+i];

/* ---- Unteren Knoten einfuegen, bis Vielfachheit = p --------------------- */   

  for( r = 1; r <= rb; r++ )
  {
    for( i = 0; i <= rb-r; i++ )
    {
      alpha = (v2 - V[b-sb-i]) / (V[b-sb-i+pv] - V[b-sb-i]);
      for( col = 0; col <= Nu; col++ )
      {     
        V1 = alpha * Sw[Nv-i][col];
        V2 = ((T)1.0-alpha) * Sw[Nv-i-1][col];
        Sw[Nv-i][col] = V1 + V2;      
      }
    }
  }

  for( i = 0; i <= pv; i++ )
    Vh[Nv+1+i] = v2;


/*-----------------------------------------------------------------------
  U-Richtung
----------------------------------------------------------------------- */  

/* ---- Linken Knoten einfuegen, bis Vielfachheit = p ------------------- */

  for( i = 0; i <= pu; i++ )
    Uh[i] = u1;

  for( r = 1; r <= rc; r++ )
  {
    for( i = 0; i <= rc-r; i++ )
    {
      alpha = (u1 - U[c-pu+r+i]) / (U[c+r+i] - U[c-pu+r+i]);
      for( row = 0; row <= Nv; row++ )
      {     
        V1 = ((T)1.0-alpha) * Sw[row][i];
        V2 = alpha * Sw[row][i+1];
        Sw[row][i] = V1 + V2;
      }
    }
  }  

/* ---- Unveraenderten Teil uebernehmen ----------------------------------- */

  for( i = 0; i < d-sd-c; i++ )
    Uh[pu+1+i] = U[c+1+i];

/* ---- Rechten Knoten einfuegen, bis Vielfachheit = p --------------------- */   

  for( r = 1; r <= rd; r++ )
  {
    for( i = 0; i <= rd-r; i++ )
    {
      alpha = (u2 - U[d-sd-i]) / (U[d-sd-i+pu] - U[d-sd-i]);
      for( row = 0; row <= Nv; row++ )
      {     
        V1 = alpha * Sw[row][Nu-i];
        V2 = ((T)1.0-alpha) * Sw[row][Nu-i-1];
        Sw[row][Nu-i] = V1 + V2;      
      }
    }
  }

  for( i = 0; i <= pu; i++ )
    Uh[Nu+1+i] = u2;

/* ------------------------------------------------------------------------- */

  NormalizeKnotVector( Nu, pu, Uh );
  NormalizeKnotVector( Nv, pv, Vh );

  *ret_nu = Nu;
  *ret_nv = Nv;
  *retU = Uh;
  *retV = Vh;
  *retPw = Sw;  
}
/*----------------------------------------------------------------------
  Extracts a part of a NURBS surface (ranging from 'u1' to 'u2' and 
  'v1' to 'v2')
----------------------------------------------------------------------*/  
template
void ExtractSurface( 
         const float u1, const float u2,
         const float v1, const float v2,
         const int nu, const int pu, Ptr< float > U,
         const int nv, const int pv, Ptr< float > V,
         Ptr< Ptr< hpoint<float> > > Pw,
         int *ret_nu, Ptr< float > *retU, 
         int *ret_nv, Ptr< float > *retV, 
         Ptr< Ptr < hpoint<float> > > *retPw );
template
void ExtractSurface( 
         const double u1, const double u2,
         const double v1, const double v2,
         const int nu, const int pu, Ptr< double > U,
         const int nv, const int pv, Ptr< double > V,
         Ptr< Ptr< hpoint<double> > > Pw,
         int *ret_nu, Ptr< double > *retU, 
         int *ret_nv, Ptr< double > *retV, 
         Ptr< Ptr < hpoint<double> > > *retPw );

template
void ExtractSurface( 
         const float u1, const float u2,
         const float v1, const float v2,
         const int nu, const int pu, Ptr< float > U,
         const int nv, const int pv, Ptr< float > V,
         Ptr< Ptr< point1<float> > > Pw,
         int *ret_nu, Ptr< float > *retU, 
         int *ret_nv, Ptr< float > *retV, 
         Ptr< Ptr < point1<float> > > *retPw );
template
void ExtractSurface( 
         const double u1, const double u2,
         const double v1, const double v2,
         const int nu, const int pu, Ptr< double > U,
         const int nv, const int pv, Ptr< double > V,
         Ptr< Ptr< point1<double> > > Pw,
         int *ret_nu, Ptr< double > *retU, 
         int *ret_nv, Ptr< double > *retV, 
         Ptr< Ptr < point1<double> > > *retPw );

}














