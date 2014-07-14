/* Viewer -- a viewer for NURBS surfaces
 * Copyright (C) 1998-1999 Norbert Irmer
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include "stdafx.h"

#include <iostream.h>

#include <gul_types.h>
#include <gul_io.h>
#include <gul_vector.h>
#include <guar_exact.h>
#include <gugr_basics.h>
#include <gugr_io.h>
#include <gugr_split.h>
#include <guge_normalize.h>
#include <gunu_basics.h>
#include <gunu_basics.h>
#include <gunu_divide.h>
#include <guge_linear.h>
#include <gunu_intersect.h>
#include <gunu_linearize.h>
#include <gunu_tesselate.h>

using gul::Ptr;
using gul::hpoint;
using gul::hpoint2;
using gul::point;
using gul::point2;
using gunu::TessInfo;
using gunu::LinearizeSurface;
using gugr::GraphInfo;

#ifdef _MSC_VER
  #pragma warning( once : 4305 )
#endif

/* ----------------------------------------------------------------------------
  the half of a cylinder, axis from (5,0,0) to (5,10,0), radius = 5  
-----------------------------------------------------------------------------*/

float CylinderV[] = {
 0., 0., 0., 0.5, 0.5, 1., 1., 1.
};
float CylinderU[] = {
 0., 0., 1., 1.
};

hpoint<float> CylinderPw[] = {  
 { 5., 0.,  0., 0.5},
 { 5., 5.,  0., 0.5},
 
 { 5., 0., 2.5, 0.5},
 { 5., 5., 2.5, 0.5},
 
 { 5., 0.,  5., 1.},
 { 5., 10., 5., 1.},

 { 0., 0., 2.5, 0.5 },
 { 0., 5., 2.5, 0.5 },

 { 0., 0.,  0., 0.5 },
 { 0., 5.,  0., 0.5 }
};

/*************************************************************************
  partition of the u,v domain
*************************************************************************/

/*----------------------------------------------------------------------
  region 1 (consisting of multiple curves and polylines)
-----------------------------------------------------------------------*/

                             /* --- first contour: a polyline --- */

point2<float> R1C1_Pts[] = { {0.,0.7}, {0.,0.}, {0.3,0.0}, {0.3,0.25} };

                             /* --- second contour: a curve --- */

float R1C2_Knots[] = { 0., 0., 0., 1., 1., 1. };

hpoint2<float> R1C2_CPts[] = {
    {0.15,0.125,0.5},  /* (0.3,0.25) */
    {0.15,0.2,0.5},    /* (0.3,0.4)  */
    {0.45,0.4,1.0}
};
                             /* --- third contour: a polyline --- */

point2<float> R1C3_Pts[] = { {0.45,0.4}, {0.7,0.4} };

                            /* --- fourth contour: a curve --- */

float R1C4_Knots[] = { 0., 0., 0., 0.5, 0.5, 1., 1., 1. };

hpoint2<float> R1C4_CPts[] = {
    {0.35,0.2,0.5},  /* (0.7,0.4) */
    {0.5,0.2,0.5},   /* (1.0,0.4) */
    {1.0,0.7,1.0},
    {0.5,0.5,0.5},   /* (1.0,1.0) */
    {0.35,0.5,0.5}   /* (0.7,1.0) */
};
                             /* --- fifth contour: a polyline --- */

point2<float> R1C5_Pts[] = { {0.7,1.0}, {0.3,1.0} };

                            /* --- sixth contour: a curve --- */

float R1C6_Knots[] = { 0., 0., 0., 1., 1., 1. };

hpoint2<float> R1C6_CPts[] = {
    {0.15,0.5,0.5},  /* (0.3,1.0) */
    {0.0,0.5,0.5},   /* (0.0,1.0) */
    {0.0,0.7,1.0}
};

/*----------------------------------------------------------------------
  region 2, just a circle
-----------------------------------------------------------------------*/

float R2C1_Knots[] = { 
        0., 0., 0., 0.25, 0.25, 0.5, 0.5, 0.75, 0.75, 1., 1., 1.};

hpoint2<float> R2C1_CPts[] = {
    {0.3,0.55,1.0},
    {0.075,0.275,0.5},  /* (0.15,0.55) */
    {0.075,0.35,0.5},   /* (0.15,0.7)  */
    {0.075,0.425,0.5},  /* (0.15,0.85) */
    {0.3,0.85,1.0},
    {0.225,0.425,0.5},  /* (0.45,0.85) */
    {0.225,0.35,0.5},   /* (0.45,0.7)  */
    {0.225,0.275,0.5},  /* (0.45,0.55) */
    {0.3,0.55,1.0}
};

/*----------------------------------------------------------------------
  region 3, just a circle
-----------------------------------------------------------------------*/

float R3C1_Knots[] = { 
        0., 0., 0., 0.25, 0.25, 0.5, 0.5, 0.75, 0.75, 1., 1., 1.};

hpoint2<float> R3C1_CPts[] = {
    {0.7,0.55,1.0},
    {0.275,0.275,0.5},  /* (0.55,0.55) */
    {0.275,0.35,0.5},   /* (0.55,0.7)  */
    {0.275,0.425,0.5},  /* (0.55,0.85) */
    {0.7,0.85,1.0},
    {0.425,0.425,0.5},  /* (0.85,0.85) */
    {0.425,0.35,0.5},   /* (0.85,0.7)  */
    {0.425,0.275,0.5},  /* (0.85,0.55) */
    {0.7,0.55,1.0}
};

/* -----------------------------------------------------------
  callback function for undivided quads
-------------------------------------------------------------*/
void  quadfunc( void *data, 
             gul::point<float> *p11, gul::point<float> *p12, 
             gul::point<float> *p21, gul::point<float> *p22,
             gul::point<float> *n11, gul::point<float> *n12, 
             gul::point<float> *n21, gul::point<float> *n22,
             float *u1, float *u2, float *v1, float *v2)
{
  std::cout << "(" << *p11 << "), (" <<  *p12 << "), (" 
            << *p21 << "), (" << *p22 << ")\n";
}
/* ----------------------------------------------------------------------
  callback function for triangles (called when quads were triangulated) 
----------------------------------------------------------------------- */
void  trifunc( void *data, 
             gul::point<float> *p1, gul::point<float> *p2, 
             gul::point<float> *p3,
             gul::point<float> *n1, gul::point<float> *n2, 
             gul::point<float> *n3,
             gul::point2<float> *d1, gul::point2<float> *d2, 
             gul::point2<float> *d3 )
{
   std::cout << "(" << *p1 << "), (" <<  *p2 << "), (" 
             << *p3 << ")\n";
}

/*-----------------------------------------------------------------------
  small utility functions
-----------------------------------------------------------------------*/
inline void build_polyline( int nEl, point2<float> *el,
                            Ptr< point2<float> > *retP )
{
  retP->use_pointer( el, nEl );
}

void linearize_curve( int nKnots, float *knots, hpoint2<float> *CPts, 
                      float domtol, gul::rtl<float>& zerotol, 
                      Ptr< point2<float> > *retP )
{
  Ptr<float> U;
  Ptr< hpoint2<float> > Pw;
  int p,n,dummy;

  U.use_pointer( knots, nKnots ); 
  
  p = 0;
  while( U[p] == 0.0 ) p++;   
  p--;                        /* degree */
  n = nKnots - p - 2;         /* index of last control point */

  Pw.use_pointer( CPts, n+1 );

  gunu::LinearizeCurve( n, p, U, Pw, domtol, zerotol, &dummy, retP );       
}


int main()
{ 
  int nu,nv,pu,pv;    /* for nurbs surface */
  Ptr<float> U,V;
  Ptr< Ptr< hpoint<float> > > Pw;

  Ptr< Ptr< point2<float> > > region;  /* linearized trim regions */

  float domtol = 0.1;   /* tolerance for tesselation of trim curves (2-D) */
  float tesstol = 0.1;  /* tolerance for tesselation of 3-D objects       */

    /* this struct contains tolerances for point and knot coincidence, etc */

  gul::rtl<float> zerotol(1.0e-10,1.0e-10,1.0e-10,1.0e-10);

  Ptr< Ptr< point2<float> > > contour_pts; /* help array */
  int i,j,count,pos;


/* -------------- construct surface ------------------------------- */

                                  /* --- build knot vectors --- */

  nu = sizeof(CylinderU)/sizeof(float); /* number of U knots */ 
  nv = sizeof(CylinderV)/sizeof(float); /* number of V knots */ 
                        
  U.use_pointer( &CylinderU[0], nu );
  V.use_pointer( &CylinderV[0], nv );
   
  pu = 0;
  while( U[pu] == 0.0 ) pu++;   
  pu--;                        /* degree */
  nu = nu - pu - 2;            /* index of last control point */

  pv = 0;
  while( V[pv] == 0.0 ) pv++;   
  pv--;                        /* degree */
  nv = nv - pv - 2;            /* index of last control point */

                               /* ---- control point matrix ------------ */

  Pw.reserve_pool( nv+1 );              /* 2-dimensional array */ 
  for( i = 0; i < nv+1; i++ ) 
    Pw[i].use_pointer( &(CylinderPw[i*(nu+1)]), nu+1 );
 
/* -----------------------------------------------------------------------
  build the trim regions
------------------------------------------------------------------------ */
  region.reserve_pool(3);  /* three regions */

/* ------------- build first trim region -------------------------------- */

  contour_pts.reserve_pool( 6 );  /* reserve help array */ 

  build_polyline( sizeof(R1C1_Pts)/sizeof(point2<float>), R1C1_Pts,
                  &contour_pts[0] );

  linearize_curve( sizeof(R1C2_Knots)/sizeof(float), R1C2_Knots, R1C2_CPts, 
                   domtol, zerotol, &contour_pts[1] );

  build_polyline( sizeof(R1C3_Pts)/sizeof(point2<float>), R1C3_Pts,
                  &contour_pts[2] );

  linearize_curve( sizeof(R1C4_Knots)/sizeof(float), R1C4_Knots, R1C4_CPts, 
                   domtol, zerotol, &contour_pts[3] );

  build_polyline( sizeof(R1C5_Pts)/sizeof(point2<float>), R1C5_Pts,
                  &contour_pts[4] );

  linearize_curve( sizeof(R1C6_Knots)/sizeof(float), R1C6_Knots, R1C6_CPts, 
                   domtol, zerotol, &contour_pts[5] );

                     /* form a closed polyline of all linearized contours */

  count = 0;                              /* reserve needed memory */
  for( i = 0; i < 6; i++ ) count += contour_pts[i].nElems()-1; 
  region[0].reserve_pool(count+1);

  pos = 0;                 /* concatenate the points of the single contours */
  for( i = 0; i < 6; i++ ) 
  {
    for( j = 0; j < contour_pts[i].nElems()-1; j++ ) 
      region[0][pos+j] = contour_pts[i][j];
    pos += contour_pts[i].nElems()-1;
  }
  region[0][pos] = contour_pts[0][0];  /* last point = first point */
  
/* --- build second trim region (easy since this is only one curve :) ----- */

  linearize_curve( sizeof(R2C1_Knots)/sizeof(float), R2C1_Knots, R2C1_CPts, 
                   domtol, zerotol, &region[1] );

/* --- build third trim region (easy since this is only one curve :) ----- */

  linearize_curve( sizeof(R3C1_Knots)/sizeof(float), R3C1_Knots, R3C1_CPts, 
                   domtol, zerotol, &region[2] );


/***********************************************************************
  tesselate trimmed surface
***********************************************************************/

  std::cout << "Knot Vector V\n";
  for( i = 0; i < nv+pv+2; i++ )
    std::cout << V[i] << " ";
  std::cout << "\n";

  std::cout << "Knot Vector U\n";
  for( i = 0; i < nu+pu+2; i++ )
    std::cout << U[i] << " ";
  std::cout << "\n";

  std::cout << "Control Points:\n";
  for( i = 0; i < nv+1; i++ )
  {
    for( j = 0; j < nu+1; j++ )
      std::cout << "(" << Pw[i][j] << ") ";
    std::cout << "\n";
  }

  for( i = 0; i < region.nElems(); i++ )
  {
    std::cout << "region " << i << ":\n";
    std::cout << "====================\n";
    for( j = 0; j < region[i].nElems(); j++ )
    {
      std::cout << "(" << region[i][j].x << ", " << region[i][j].y << ")\n";
    }
  } 

  gunu::LinearizeSurface( 
      nu, pu, U, nv, pv, V, Pw,
      tesstol, zerotol, region, true, quadfunc, trifunc, NULL );

  return(0);
}
