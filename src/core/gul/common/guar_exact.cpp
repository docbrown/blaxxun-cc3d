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
#include <math.h>
#include "gul_types.h"
#include "guar_exact.h"

namespace guar {

/* ----------------------------------------------------------------
  Calculates the sum of two operands
------------------------------------------------------------------ */

void IntAdd( const int na, const unsigned long *a, 
             const int nb, const unsigned long *b, 
             int *nc, unsigned long *c )
{
  uint64 l,z=0;
  int i,len;
  
  if( na < nb )
  {
    len = nb+1;
    
    for( i = 0; i < na; i++ )
    {
      l = (uint64)a[i] + (uint64)b[i] + z;
      c[i] = (unsigned long)(l & 0xffffffff);
      z = (l >> 32) & 0xffffffff;
    }
    for( i = na; i < nb; i++ )
    {
      l = (uint64)b[i] + z;
      c[i] = (unsigned long)(l & 0xffffffff);
      z = (l >> 32) & 0xffffffff;
    }
    if( (c[i] = (unsigned long)z) == 0 ) len -= 1;
  }
  else
  {
    len = na+1;
    
    for( i = 0; i < nb; i++ )
    {
      l = (uint64)a[i] + (uint64)b[i] + z;
      c[i] = (unsigned long)(l & 0xffffffff);
      z = (l >> 32) & 0xffffffff;
    }
    for( i = nb; i < na; i++ )
    {
      l = (uint64)a[i] + z;
      c[i] = (unsigned long)(l & 0xffffffff);
      z = (l >> 32) & 0xffffffff;
    }
    if( (c[i] = (unsigned long)z) == 0 ) len -= 1;
  }

  *nc = len;
}

/* ----------------------------------------------------------------
  Calculates the difference of two operands
------------------------------------------------------------------ */

int IntSub( const int na, const unsigned long *a, 
            const int nb, const unsigned long *b,
            int *nc, unsigned long *c )
{
  uint64 l,z=1;
  int i,len,sign;

  /* test if b > a */
  
  if( nb > na )
    sign = -1;
  else if( na > nb )
    sign = 0;
  else        /* na == nb */
  {
    i = na-1;
    while( (a[i] == b[i]) && (i >= 0) )
      i--;
    sign = b[i] > a[i] ? -1 : 0;
  }
 
  if( sign )       /* calc b - a, na <= nb */
  {
    for( i = 0; i < na; i++ )
    {
      l = 0xffffffff + (uint64)b[i] - (uint64)a[i] + z;
      c[i] = (unsigned long)(l & 0xffffffff);
      z = (l >> 32) & 0xffffffff;
    }
    for( i = na; i < nb; i++ )
    {
      l = 0xffffffff + (uint64)b[i] + z;
      c[i] = (unsigned long)(l & 0xffffffff);
      z = (l >> 32) & 0xffffffff;
    }   
    len = nb;
  }
  else             /* calc a - b, na >= nb */
  {
    for( i = 0; i < nb; i++ )
    {
      l = 0xffffffff + (uint64)a[i] - (uint64)b[i] + z;
      c[i] = (unsigned long)(l & 0xffffffff);
      z = (l >> 32) & 0xffffffff;
    }
    for( i = nb; i < na; i++ )
    {
      l = 0xffffffff + (uint64)a[i] + z;
      c[i] = (unsigned long)(l & 0xffffffff);
      z = (l >> 32) & 0xffffffff;
    }    
    len = na;
  }

  /* strip leading zeros */

  for( i = len-1; (i >= 0) && (c[i] == 0); i-- ) len--;

  *nc = len;
  return( sign );
}

/* ---------------------------------------------------------------
  Short Multiplication
---------------------------------------------------------------- */

unsigned long IntMultShort( 
         const int na, const unsigned long *a, const unsigned long b,
         unsigned long *c )
{
  uint64 z, l, m;
  int i;
  
  z = 0;
  m = (uint64)b;

  for( i = 0; i < na; i++ )
  {
    l = (uint64)a[i]*m + z;
    c[i] = (unsigned long)(l & 0xffffffff);
    z = (l >> 32) & 0xffffffff;
  }
  return((unsigned long)z);
}


/* ----------------------------------------------------------------
  Calculates the product of two operands, slow but simple
  TODO: Multiplication with FFT
------------------------------------------------------------------ */

void IntMult( const int na, const unsigned long *a, 
              const int nb, const unsigned long *b, 
              int *nc, unsigned long *c )
{
  uint64 l,z;
  int i,j;

  if( (na == 0) || (nb == 0) )
  { *nc = 0; return; }

  for( i = 0; i < na; i++ )
    c[i] = 0;

  for( j = 0; j < nb; j++ )
  {
    z = 0;
    for( i = 0; i < na; i++ )
    {
      l = (uint64)a[i]*(uint64)b[j] + 
          (uint64)c[i+j] + z;
      c[i+j] = (unsigned long)(l & 0xffffffff);
      z = (l >> 32) & 0xffffffff;
    }
    c[i+j] = (unsigned long)z;
  }

  if( c[na+nb-1] == 0 ) *nc = na+nb-1;
  else *nc = na+nb;
}

/* ---------------------------------------------------------
  Divide a int-string by a single int
----------------------------------------------------------- */

unsigned long IntDivShort( 
       const int na, const unsigned long *a, const unsigned long b, 
       int *nq, unsigned long *q )
{
  uint64 l, z, d = b;
  int i;
  
  z = 0;
  for( i = na-1; i >= 0; i-- )
  {
    l = (z<<32) + ((uint64)a[i]);
    q[i] = (unsigned long)(l / d);
    z = l % d;
  }

  if( q[na-1] == 0 )
    *nq = na-1;
  else
    *nq = na;

  return((unsigned long)z);
}

/* ---------------------------------------------------------
  Divide a int-string by a int-string
----------------------------------------------------------- */

int IntDiv( const int na, const unsigned long *a, 
            const int nb, const unsigned long *b, 
            int *nq, unsigned long *q, 
            int *nr, unsigned long *r )
{
  unsigned long *ad, *bd;
  uint64 d, z, l, b1, b2, m, ml, qh;
  int i,j,sign;

  if( na == 0 )
  {
    *nq = 0;
    *nr = 0;
    return(-1);
  }

  if( nb < 2 )
  {
    if( nb == 0 ) return(0);
    
    r[0] = IntDivShort( na, a, b[0], nq, q );
    if( r[0] != 0 ) *nr = 1; else *nr = 0;
    return(-1);
  }

  if( na < nb )
  {
    memcpy( r, a, na*sizeof(unsigned long) );
    *nr = na;
    *nq = 0;
    return(-1);
  }
  
  ad = (unsigned long *)alloca( sizeof(unsigned long)*(na+1) );
  if( ad == NULL ) return(0);
  bd = (unsigned long *)alloca( sizeof(unsigned long)*nb );
  if( bd == NULL ) return(0);

  d = LL(0x100000000)/(((uint64)b[nb-1]) + LL(1));
  
  ad[na] = IntMultShort( na, a, (unsigned long)d, ad );
  IntMultShort( nb, b, (unsigned long)d, bd );
  
  b1 = bd[nb-1];
  b2 = bd[nb-2];

  for( i = na; i >= nb; i-- )
  {
    l = (((uint64)ad[i])<<32) + (uint64)ad[i-1];

    if( ad[i] == bd[nb-1] )
      qh = 0xffffffff;
    else
      qh =  l/b1;
      
    while( b2*qh > ((l - b1*qh)<<32) + (uint64)ad[i-2] ) qh--;
        
    z = 1;    
    sign = 0;
    m = 0;
    for( j = 0; j < nb; j++ )
    {
      m = qh*(uint64)bd[j] + m;
      ml = m & 0xffffffff;
      m = m >> 32;
      
      l = 0xffffffff + (uint64)ad[i-nb+j] - ml + z;
      ad[i-nb+j] = (unsigned long)(l & 0xffffffff);
      z = (l >> 32) & 0xffffffff;
    }        
    l = 0xffffffff + (uint64)ad[i] - m + z;
    ad[i] = (unsigned long)(l & 0xffffffff);
    z = (l >> 32) & 0xffffffff;
    sign = (z - LL(1) != 0 ? -1 : 0);
    
    if( sign )        /* add borrow back */
    {
      qh--;

      z = 0;
      for( j = 0; j < nb; j++ )
      {
        l = (uint64)ad[i-nb+j] + (uint64)bd[j] + z;
        ad[i-nb+j] = (unsigned long)(l & 0xffffffff);
        z = (l >> 32) & 0xffffffff;
      }        
      l = (uint64)ad[i] + z;
      ad[i] = (unsigned long)(l & 0xffffffff);
    }

    q[i-nb] = (unsigned long)qh;
  }
  
  for( i = nb-1; (i >= 0) && (ad[i]==0); i-- );
  i++;
  if( i > 0 )
    IntDivShort( nb, ad, (unsigned long)d, nr, r );
  else
    *nr = 0;

  if( q[na-nb] == 0 )
    *nq = na-nb;
  else
    *nq = na-nb+1; 

  return(-1);
}


/* ---------------------------------------------------------
  Converts double to int-string
----------------------------------------------------------- */

int DoubleToInt( const double d, const int na, unsigned long *a,
                 int *retLen )
{
  double f,m;
  int i,sign,len;
  
  if( d < 0.0 )
  {
    sign = -1;
    m = fabs( d );
  }
  else
  {
    sign = 0;
    m = d;
  }

  len = 0;
  while( m >= 1.0 )
  {
    m = m / (double)(LL(0x100000000));
    len++;
  }
  for( i = len-1; i >= 0; i-- )
  {
    m = m*(double)(LL(0x100000000));
    f = floor(m); 
    a[i] = (unsigned long)f;
    m -= f;
  }

  *retLen = len;
  return(sign);
}

/* ------------------------------------------------------------
  interval arithmetic functions
-------------------------------------------------------------- */

Interval operator*( const Interval &a, const Interval &b )
{
  Interval c;
  double f1,f2;
  
  if( a.m_low >= 0.0 )       /* 0 <= alo <= ahi                   */
  {
    if( b.m_low >= 0.0 )     /* 0 <= alo <= ahi,  0 <= blo <= bhi */ 
    {
      c.m_low = a.m_low * b.m_low;
      c.m_high = a.m_high * b.m_high;
    }
    else                     /* blo < 0 */
    {
      if( b.m_high >= 0 )    /*  0 <= alo <= ahi,  blo < 0 <= bhi */
      {
        c.m_low = a.m_high * b.m_low;
        c.m_high = a.m_high * b.m_high;
      }
      else                   /*  0 <= alo <= ahi,  blo <= bhi < 0 */
      {
        c.m_low = a.m_high * b.m_low;
        c.m_high = a.m_low * b.m_high;
      }
    }
  }
  else                       /* alo < 0 */
  {
    if( a.m_high < 0.0 )     /* alo <= ahi < 0 */
    {
      if( b.m_low >= 0 )     /* alo <= ahi < 0,  0 <= blo <= bhi */
      {
        c.m_low = a.m_low * b.m_high;
        c.m_high = a.m_high * b.m_low;
      }
      else                   /* alo <= ahi < 0,  blo < 0 */
      {
        if( b.m_high < 0 )   /* alo <= ahi < 0,  blo <= bhi < 0 */
        {
          c.m_low = a.m_high * b.m_high;
          c.m_high = a.m_low * b.m_low;
        }
        else     /* alo <= ahi < 0,  blo < 0 <= bhi */
        {
          c.m_low = a.m_low * b.m_high;
          c.m_high = a.m_high * b.m_low;
        }
      }
    }
    else    /* alo < 0 <= ahi */
    {
      if( b.m_low >= 0 )   /* alo < 0 <= ahi,  0 <= blo <= bhi */
      {
        c.m_low = a.m_low * b.m_high;
        c.m_high = a.m_high * b.m_high;
      }
      else                 /* alo < 0 <= ahi,  blo < 0 */
      {
        if( b.m_high < 0 ) /* alo < 0 <= ahi,  blo <= bhi < 0 */
        { 
          c.m_low = a.m_high * b.m_low;
          c.m_high = a.m_low * b.m_high;
        }
        else              /* alo < 0 <= ahi,  blo < 0 <= bhi */
        {
          f1 = a.m_low * b.m_high;
          f2 = a.m_high * b.m_low;
          c.m_low = f1 < f2 ? f1 : f2;

          f1 = a.m_low * b.m_low;
          f2 = a.m_high * b.m_high;
          c.m_high = f1 > f2 ? f1 : f2;
        }
      }
    }
  }
      
  if( !isinf(c.m_low) ) c.m_low = c.m_low - DBL_EPSILON*fabs(c.m_low);
  if( !isinf(c.m_high) ) c.m_high = c.m_high + DBL_EPSILON*fabs(c.m_high);
    
  return( c );
}

/* ---------------------------------------------------------------------
  functions for fractions
----------------------------------------------------------------------- */

PrecRational operator*( const PrecRational& a, const PrecRational& b )
{
  if( (a.m->m_na == 0) || (b.m->m_na == 0) )  /* a = 0 or b = 0 */
  {
    PrecRational c;
    return(c);
  }
  else
  {
    PrecRational c( a.m->m_sign ^ b.m->m_sign, 
                      a.m->m_na + b.m->m_na, a.m->m_nb + b.m->m_nb );

    IntMult( a.m->m_na, a.m->m_a, b.m->m_na, b.m->m_a, 
             &c.m->m_na, c.m->m_a );

    if( (a.m->m_nb != 0) && (b.m->m_nb != 0) )
    {
      IntMult( a.m->m_nb, a.m->m_b, b.m->m_nb, b.m->m_b, 
                    &c.m->m_nb, c.m->m_b );
    }
    else
    {
      if( b.m->m_nb != 0 )
      {
        memcpy( c.m->m_b, b.m->m_b, b.m->m_nb*sizeof(unsigned long) );
        c.m->m_nb = b.m->m_nb; 
      }       
      else if( a.m->m_nb != 0 )
      {
        memcpy( c.m->m_b, a.m->m_b, a.m->m_nb*sizeof(unsigned long) );
        c.m->m_nb = a.m->m_nb;
      }
    }
      
    return(c);
  }
}

PrecRational operator+( const PrecRational& A, const PrecRational& B )
{
  unsigned long *a1b2,*a2b1,*a1b2_a2b1,*b1b2;
  int na1b2,na2b1,n_a1b2_a2b1,nb1b2;
  size_t size_a1b2,size_a2b1,size_a1b2_a2b1,size_b1b2;
  int sign,sign2;
  
  if( A.m->m_na == 0 ) return(B);
  if( B.m->m_na == 0 ) return(A);
  
  PrecRational c;
   
  if( B.m->m_nb == 0 )
  {
    a1b2 = A.m->m_a;
    na1b2 = A.m->m_na;
    size_a1b2 = 0;
  }
  else
  {
    a1b2 = (unsigned long *)gust::PoolAlloc( 
                     (A.m->m_na+B.m->m_nb)*sizeof(unsigned long), &size_a1b2 );
    if( a1b2 == NULL ) throw gul::PoolAllocError();
    IntMult( A.m->m_na, A.m->m_a, B.m->m_nb, B.m->m_b, 
             &na1b2, a1b2 );
  }

  if( A.m->m_nb == 0 )
  {
    a2b1 = B.m->m_a;
    na2b1 = B.m->m_na;
    size_a2b1 = 0;
  }
  else
  {
    a2b1 = (unsigned long *)gust::PoolAlloc( 
                     (B.m->m_na+A.m->m_nb)*sizeof(unsigned long), &size_a2b1 );
    if( a2b1 == NULL ) throw gul::PoolAllocError();
    IntMult( B.m->m_na, B.m->m_a, A.m->m_nb, A.m->m_b, 
             &na2b1, a2b1 );
  }

  n_a1b2_a2b1 = na1b2 > na2b1 ? na1b2 : na2b1;
  if( n_a1b2_a2b1 == 0 )
  {
    if( size_a1b2 ) gust::PoolFree( a1b2, size_a1b2 );
    if( size_a2b1 ) gust::PoolFree( a2b1, size_a2b1 );
    return(c);
  }

  sign = A.m->m_sign ^ B.m->m_sign;
  if( !sign ) n_a1b2_a2b1++; 

  a1b2_a2b1 = (unsigned long *)gust::PoolAlloc( 
                 n_a1b2_a2b1*sizeof(unsigned long), &size_a1b2_a2b1 );
  if( a1b2_a2b1 == NULL ) throw gul::PoolAllocError();

  if( sign )
  {
    sign2 = IntSub( na1b2, a1b2, na2b1, a2b1, &n_a1b2_a2b1, a1b2_a2b1 );
    if( n_a1b2_a2b1 == 0 ) 
    {
      gust::PoolFree( a1b2_a2b1, size_a1b2_a2b1 );
      a1b2_a2b1 = NULL;
    }
  }
  else
  {
    sign2 = 0;
    IntAdd( na1b2, a1b2, na2b1, a2b1, &n_a1b2_a2b1, a1b2_a2b1 );
  }

  if( size_a1b2 ) gust::PoolFree( a1b2, size_a1b2 );
  if( size_a2b1 ) gust::PoolFree( a2b1, size_a2b1 );
  
  if( n_a1b2_a2b1 == 0 )  /* numerator = 0 */
  {
    sign = 0;
    b1b2 = NULL;
    nb1b2 = 0;
    size_b1b2 = 0;
  }
  else                    /* numerator != 0 */
  {  
    if( A.m->m_sign )
      sign = sign2 ^ -1;
    else
      sign = sign2;
     
    if( A.m->m_nb == 0 )
    {
      if( B.m->m_nb == 0 )
      {
        b1b2 = NULL;
        nb1b2 = 0;
        size_b1b2 = 0;
      }
      else
      {
        nb1b2 = B.m->m_nb;
        b1b2 = (unsigned long *)gust::PoolAlloc( 
                                    nb1b2*sizeof(unsigned long), &size_b1b2 );
        if( b1b2 == NULL ) throw gul::PoolAllocError();
        memcpy( b1b2, B.m->m_b, nb1b2*sizeof(unsigned long) );
      }
    }
    else if( B.m->m_nb == 0 )
    {
      nb1b2 = A.m->m_nb;
      b1b2 = (unsigned long *)gust::PoolAlloc( 
                                    nb1b2*sizeof(unsigned long), &size_b1b2 );
      if( b1b2 == NULL ) throw gul::PoolAllocError();
      memcpy( b1b2, A.m->m_b, nb1b2*sizeof(unsigned long) );
    }
    else
    {
      b1b2 = (unsigned long *)gust::PoolAlloc( 
                (A.m->m_nb+B.m->m_nb)*sizeof(unsigned long), &size_b1b2 );
      if( b1b2 == NULL ) throw gul::PoolAllocError();
  
      IntMult( A.m->m_nb, A.m->m_b, B.m->m_nb, B.m->m_b,
               &nb1b2, b1b2 );
    }
  }
  
  c.m->m_sign = sign;
  c.m->m_na = n_a1b2_a2b1;
  c.m->m_size_a = size_a1b2_a2b1;
  c.m->m_a = a1b2_a2b1;
  c.m->m_nb = nb1b2;
  c.m->m_size_b = size_b1b2;
  c.m->m_b = b1b2;
  return(c);
}

/* --------------------------------------------------------------------
  Divides numerator by denominator (modulo)
---------------------------------------------------------------------- */

void PrecRational::DivMod( PrecRational *q, PrecRational *r ) const
{
  if( (m->m_na == 0) || (m->m_nb == 0) )
  {
    *q = *this;
    *r = PrecRational(); 
    return;
  }
  else if( m->m_na < m->m_nb ) 
  {
    *q = PrecRational();
    *r = PrecRational( 0, m->m_na, 0 );
    memcpy( r->m->m_a, m->m_a, m->m_na*sizeof(unsigned long) );
  }
  else 
  {
    PrecRational Q( 0, m->m_na - m->m_nb + 1, 0 );
    PrecRational R( 0, m->m_nb, 0 );

    if( !IntDiv( m->m_na, m->m_a, m->m_nb, m->m_b,
                    &Q.m->m_na, Q.m->m_a, &R.m->m_na, R.m->m_a ) )
      throw gul::Error();

    *q = Q; *r = R;
  }

  if( m->m_sign )
  {
    *q = *q + PrecRational(ULong(1));
    q->m->m_sign = -1;
    
    PrecRational h( 0, m->m_nb, 0 );
    memcpy( h.m->m_a, m->m_b, m->m_nb*sizeof(unsigned long) );
    *r = h - *r;
  }
}


}







