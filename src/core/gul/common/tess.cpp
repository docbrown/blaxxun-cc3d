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
/*
#ifdef new
#undef new
#endif
*/
#include <iostream.h>

#include "gul_types.h"
#include "gul_vector.h"
#include "guge_normalize.h"
#include "gunu_basics.h"
#include "gunu_basics.h"
#include "gunu_divide.h"
#include "guge_linear.h"
#include "gunu_intersect.h"
#include "gunu_linearize.h"

using gul::Ptr;
using gul::hpoint;
using gul::point;
using gul::hpoint2;
using gul::point2;

/* ---- Cylinder 1 --------------------------------- */
/* Axis (5,0,0) to (5,10,0), Radius = 5  */

int Cylinder1nu = 1;
int Cylinder1pu = 1;
int Cylinder1nv = 4;
int Cylinder1pv = 2;

double Cylinder1V[] = {
 0., 0., 0., 0.5, 0.5, 1., 1., 1.
};
double Cylinder1U[] = {
 0., 0., 1., 1.
};
hpoint<double> Cylinder1Pw[] = {  
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
hpoint<double> *Cylinder1Net[] = {
  &Cylinder1Pw[0],
  &Cylinder1Pw[2],
  &Cylinder1Pw[4],
  &Cylinder1Pw[6],
  &Cylinder1Pw[8]
};  

/* --- Cylinder 2 ----------------------------------------------- */
/* Achse (0,5,0) bis (10,5,0), Radius = 5  */

int Cylinder2nu = 4;
int Cylinder2pu = 2;
int Cylinder2nv = 1;
int Cylinder2pv = 1;

double Cylinder2U[] = {
 0., 0., 0., 0.5, 0.5, 1., 1., 1.
};
double Cylinder2V[] = {
 0., 0., 1., 1.
};
hpoint<double> Cylinder2Pw[] = {
  {0., 0., 0.,  0.5},
  {0., 0., 2.5, 0.5},
  {0., 5., 5.,  1.},
  {0., 5., 2.5, 0.5},
  {0., 5., 0.,  0.5},

  {5., 0., 0.,  0.5},
  {5., 0., 2.5, 0.5},
  {10., 5., 5.,  1.},
  {5., 5., 2.5, 0.5},
  {5., 5., 0.,  0.5},
};
hpoint<double> *Cylinder2Net[] = {
  &Cylinder2Pw[0],
  &Cylinder2Pw[5]
};  

template< class T >
inline ostream& operator<<( ostream& s, const point<T>& p )
{
  return( s << p.x << " " << p.y << " " << p.z );
} 
template< class T>
inline ostream& operator<<( ostream& s, const hpoint<T>& p )
{
  return( s << p.x << " " << p.y << " " << p.z << " " << p.w );
} 

void  output_func( void *data, 
             gul::point<double> *p11, gul::point<double> *p12, 
             gul::point<double> *p21, gul::point<double> *p22,
             gul::point<double> *n11, gul::point<double> *n12, 
             gul::point<double> *n21, gul::point<double> *n22,
             double *u1, double *u2, double *v1, double *v2)
{
  cout << "(" << *p11 << "), (" <<  *p12 << "), (" 
       << *p21 << "), (" << *p22 << ")\n";
}



int testmain()
{
  point<double> P;
  int i,j;
  Ptr< double > U,V;
  Ptr< Ptr< hpoint<double> > > Pw;
  gul::rtl<double> tol(1.0e-10,1.0e-10,1.0e-10,1.0e-10);

                  /* try different allocation methods */

  U.reserve_pool( Cylinder1nu+Cylinder1pu+2 );
  for( i = 0; i < Cylinder1nu+Cylinder1pu+2; i++ )
    U[i] = Cylinder1U[i];
  
  cout << "Simple test of gul-library functions\n";

  cout << "Knot Vector U\n";
  for( i = 0; i < Cylinder1nu+Cylinder1pu+2; i++ )
    cout << U[i] << " ";
  cout << "\n";
  
  V.reserve_place( reserve_stack(double,Cylinder1nv+Cylinder1pv+2),
                   Cylinder1nv+Cylinder1pv+2 );
  for( i = 0; i < Cylinder1nv+Cylinder1pv+2; i++ )
    V[i] = Cylinder1V[i];
  
  cout << "Knot Vector V\n";
  for( i = 0; i < Cylinder1nv+Cylinder1pv+2; i++ )
    cout << V[i] << " ";
  cout << "\n";
 
  Pw.reserve_global( Cylinder1nv+1 );
  for( i = 0; i < Cylinder1nv+1; i++ )
    Pw[i].reserve_pool( Cylinder1nu+1 );
   
  for( i = 0; i < Cylinder1nv+1; i++ )
  {
    for( j = 0; j < Cylinder1nu+1; j++ )
      Pw[i][j] = Cylinder1Net[i][j];
  }

  cout << "Control Points:\n";
  for( i = 0; i < Cylinder1nv+1; i++ )
  {
    for( j = 0; j < Cylinder1nu+1; j++ )
      cout << "(" << Pw[i][j] << ") ";
    cout << "\n";
  }

               /* test calculation of function value at u,v */

  gunu::SurfacePoint( 0.3, 0.5, 
                           Cylinder1nu, Cylinder1pu, U, 
                           Cylinder1nv, Cylinder1pv, V,
                           Pw, &P );
  cout << "S(0.3,0.5) = " << P << "\n";

               /* test tesselation of untrimmed surfaces */

  gunu::LinearizeSurface<double>( Cylinder1nu, Cylinder1pu, U, 
                          Cylinder1nv, Cylinder1pv, V, Pw,
                          0.1, tol, output_func, NULL );
  

  return(0);
}
