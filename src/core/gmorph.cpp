/*=============================================================================

This code is licensed under the Web3D-blaxxun Community Source License,
provided in distribution file LICENSE.TXT and available online at
http://www.web3D.org/TaskGroups/x3d/blaxxun/Web3D-blaxxunCommunitySourceAgreement.html
and may only be used for non-commercial use as specified in that license.

THE WEB3D CONSORTIUM AND BLAXXUN DO NOT MAKE AND HEREBY DISCLAIM ANY EXPRESS
OR IMPLIED WARRANTIES RELATING TO THIS CODE, INCLUDING BUT NOT LIMITED TO,
WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY OR FITNESS FOR A PARTICULAR
PURPOSE, OR ANY WARRANTIES THAT MIGHT ARISE FROM A COURSE OF DEALING, USAGE
OR TRADE PRACTICE.  THE COMMUNITY SOURCE CODE IS PROVIDED UNDER THIS
AGREEMENT "AS IS," WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED,
INCLUDING, WITHOUT LIMITATION, WARRANTIES THAT THE COMMUNITY SOURCE CODE ARE
FREE OF DEFECTS, MERCHANTABLE, FIT FOR A PARTICULAR PURPOSE OR
NON-INFRINGING OR IN ANY WAY CONSTITUTE THE COMPLETE PRODUCT MARKETED UNDER
THE NAMES GIVEN SAID CODE.

==============================================================================*/
/******************************************************************************
@doc

@module GMorph.cpp - GLView polyhedron vertex morphing	|

Copyright (c) 1994,1995	by Holger Grahn
All rights reserved

Purpose:

Classes:
GMorph

Notes:
        Melt effect based on Glenn M. Lewis, Dr. Dobbs'Journal, 07 1994 P.g 86

Changes:

Todo :

******************************************************************************/

/*
  Overview :
  	A) get vertices and normals of a polyhedron (WEB)
	B) apply any modelling matrices to vertices
	C) normalize vertices to -1 1 unit box or better unit sphere of radius 1
	D) apply some algorithm 
		vnew  = f(vold,t)
	E) undo normalization transformation
	F) replace coordinates 


	D) depending on type
		cylinder, sphere, box ... :
		   compute (u,v,w) mapping from normalized cóordinates to shape
		     e.g. for cylinder  u = atan2(y,x) v = z
			 evaluate destination shape at (u,v,w) e.g. vdest = cylinder(u,v)
			 interpolate between vdest and normalized 
			   vnew=interpolate(vold,vdest,t)
		   


*/

#include "stdafx.h"

#include "gutils.h"

//#include "Point.h"
//#include "matrix.h"
//#include <math.h>
// For GParameter
#include "gclass.h"
#include "gshell.h"
#include "gshellop.h"
#include "gshelli.h"

#include <gvnodes.h>
#include <gvfields.h>
#include <gvtraverse.h>


#ifdef _G_VRML1

#include "gdeform.h"
//#include "gmorph.h"

inline float  Interpolate (const float A, float ta, const float B,float tb)
{ return(A*ta+B*tb);
}

float f_lin(float x);
float f_lin_i(float x);
float f_linclamp(float x);
float f_rect(float x);
float f_saw(float x);
float f_tria(float x);
float f_sqr(float x);
float f_cub(float x);
float f_sin(float x);
float f_smooth(float x);
float f_rnd(float x);



#define  VECCOPY(AA,BB) VECOP2(AA,=,BB)
#define  VECSET(AA,A,B,C) VECOP2_3(AA,=,A,B,C)
#define  VECZERO(AA)  (AA).x=(AA).y=(AA).z=0.0
#define  VECADD(AA,A,B) VECOP3(AA,=,A,+,B)
#define  VECSUB(AA,A,B) VECOP3(AA,=,A,-,B)


#define VECOP3(AAA,OP1,BBB,OP2,CCC)  {(AAA).x OP1 (BBB).x OP2 (CCC).x; \
		 (AAA).y OP1 (BBB).y OP2 (CCC).y; (AAA).z OP1 (BBB).z OP2 (CCC).z;}
#define VECOP3C(AAA,OP1,BBB,OP2,CCC)  {(AAA).x OP1 (BBB).x OP2 CCC; \
		 (AAA).y OP1 (BBB).y OP2 CCC; (AAA).z OP1 (BBB).z OP2 CCC;}

#define VECOP4(AAA,OP1,BBB,OP2,CCC,OP3,DDD)  {(AAA).x OP1 (BBB).x OP2 (CCC).x OP3 (DDD).x; \
		 (AAA).y OP1 (BBB).y OP2 (CCC).y OP3 (DDD).y; (AAA).z OP1 (BBB).z OP2 (CCC).z OP3 (DDD).z;}
#define VECOP4C(AAA,OP1,BBB,OP2,CCC,OP3,DDD)  {(AAA).x OP1 (BBB).x OP2 (CCC).x OP3 (DDD); \
		 (AAA).y OP1 (BBB).y OP2 (CCC).y OP3 (DDD); (AAA).z OP1 (BBB).z OP2 (CCC).z OP3 (DDD);}


/* hermite is invariant to rotation, translation */

void compute_hermite_Point_pt(const Point *p1,const /*Vector*/ Point *r1,
			      const Point *p4,const /*Vector*/ Point *r4,
			      float t,Point *p);

int intersect_ray_sphere(
			/* Ray */
			const Point * ray_p, const /* Vector*/ Point *ray_d,
			/* sphere */
			const Point *sph_center, float sph_radius,
			/* intersection */
			float *isect_t );

int intersect_ray_box (const Point *rayP, const Point *rayD,
		       const Point *box, float *Depth1, float *Depth2);

void box_normal (const Point *box,const Point * IPoint,Point *Result);


class GMorphObject {

public :
  GShell *theShell;
  PointArray orgV;	// original vertices of object
  PointArray orgVN;	// original vertex normals of object

  PointArray mappedV;	// v mapped to mapping space
  PointArray mappedVN;	// vn mapped to mapping space

  BBox box;

  int hasMatrix;
  MatrixAndInverse matrix;


  Point smin,smax;		// range where to map == box
  Point scenter;		// center of bounding sphere
  float sradius;		// radius of  "           " 

  PointArray paramV;	// parametric mapping of coordinates

  GMorphObject() {
  		theShell = NULL;
		hasMatrix = 0;
  	}
  
  // set shell + save orignal data
  int SetShell(GShell *s); 
  // reset saved V + VN to shell
  int Reset(); 
};

// set shell + save orignal data
int GMorphObject::SetShell(GShell *s) 
{
 theShell = s;
 orgV = s->GetV();
 // to do : GShellI

 orgVN = s->GetVN();
 
 box = s->GetBBox();

 if (hasMatrix) box *= matrix.m;

 smin = box.mi;
 smax = box.ma;
 
 Point size = box.ma - box.mi;
// sradius = size.Len() * 0.5;
 sradius = max(size.x,size.y,size.z) * 0.5;
 scenter = box.mi + size *0.5;
 return(s->v.Length() >0);
}

// reset saved V + VN to shell
int GMorphObject::Reset() 
{
  if (theShell) {
   theShell->SetV(orgV);
   theShell->SetVN(orgVN);
   return(1);
  }
  return(0);
}



// constructor in GvMorph.cpp, ResetParams is called
GvMorph::~GvMorph()
{
 for (int i=0; i<objects.Length(); i++) {
 	delete objects[i];
	objects[i]=0;
 }
}


// reset parameters to default value
void GvMorph::ResetParams(void)
{
  type.set(SPHERE);
  hermite.set(TRUE);
  hermite.setDefault(TRUE);
  intersect.set(FALSE);

  rotation.value.set(0,1,0,0);
  rotation.setDefault(TRUE);
  local.set(TRUE);
  rscale.set(1.0);
  
  has_map_matrix=0;

 
//  object dependent  sradius = 1.0;
//  sradius2 = 0.5;

  nscale1.set(1.0);
  nscale1.setDefault(TRUE);

  nscale2.set(1.0);
  nscale1.setDefault(TRUE);

  wave_nwaves.set(1.5);
  wave_offset.set(0.0);
  wave_radius.set(0.25);

  swirl_nwaves.set(1.0);

  amplitude1.set(0.25,0.25);
  amplitude2.set(0.25,0.25);
  phaseShift1.set(0.0);
  phaseShift2.set(1.0);
  phaseScale1.set(1.0,1.0);
  phaseScale2.set(1.0,1.0);
  
  // HG ?
  init = 0;

}




// interpolate between 2 sets of pts & normals
int GvMorph::InterpolatePoints(
        int mode,
        float t,
        int np,
        const Point *p1,const Point *n1,
        const Point *p2,const Point *n2,
        Point *pout, Point *nout )
{ int i;
  if (!pout) return(-1);

  if ((hermite) && (n1 != NULL) && (n2 != NULL)) {
      for(i=np; i>0; i--,p1++,n1++,p2++,n2++,pout++) {
         compute_hermite_Point_pt(p1,n1,p2,n2,t,pout);
      }
  } else {
  /* linear interpolation */
  float t1= 1.0 - t;
  for(i=np; i>0; i--,p1++,p2++,pout++) {
        *pout = Interpolate(*p1,t,*p2,t1);
  }
  }
  return(0);
}

int mapPoints_to_sphere(
        int mode,int intersect,
        int np,
        const Point *p1,const Point *n1,
        const Point *scenter,double sradius,

        Point *pout,Point *nout)
{ int i;
  float t;
  Point p,d;
  for(i=np; i>0; i--,p1++,pout++) {
        p = *p1 - *scenter;
        d = Normalize(p);
        if (intersect) {
           intersect_ray_sphere(&p,&d,NULL,sradius,&t);
           p += *scenter;
           VECOP4C(*pout,=,p,+,d,*,t);
        } else {
          VECOP4C(*pout,=,*scenter,+,d,*,sradius);
        }
        if (nout) {
             *nout=d; nout ++;
        }
  }
  return(0);
}



/* assume x from -1 to 1 */

float f_hantel(double x,double r1,double r2)
{ double t1=0.25;
  double t2=0.75;
  double y;

  x = (x + 1.0) / 2.0;
  x = fabs(x);


  if (x <=t1)   y = r1*sqrt( 1 - sqr(1.0-(x/t1)));
  else if (x>= t2) {
        x= x-t2;
        y = r1*sqrt( 1 - sqr((x/(1.0-t2))));
  } else {
    x = TWOPI * (x-t1) / (t2-t1);
    y = r1- cos(x) * r2 * 0.5;
  }
  return(y);
}

// GGEMS V Pg. 142, Carole Blanc General Implementation of Axial Deformation Mech.

int pinch(const Point *p,float amount, Point *pout)
{  
   double u= p->z*amount;
   pout->Set(p->x* (1.0-u),p->y,p->z);
   return(1);
}

int taper(const Point *p,float amount, Point *pout)
{  
   double u= 1.0 - p->z*amount;
   pout->Set(p->x* u ,p->y *u,p->z);
   return(1);
}


// Rotate the z axis aournd y according to z

int bend(const Point *p,float amount, Point *pout)
{  
   double u= PI*amount * p->z;
   float c = cos(u);
   float s = sin(u);
   u = p->z;
   pout->Set(s * u +  c * p->x ,p->y, c * u -  s *p->x);
   return(1);
}


int mold(const Point *p,float amount, Point *pout)
{  
   double u= atan2(p->y,p->x) / PI;
   // ???
   pout->Set(p->x* u ,p->y *u,p->z);
   return(1);
}


int twist(const Point *p,float amount, Point *pout)
{  
   double u= PI*amount * p->z;
   float c = cos(u);
   float s = sin(u);
   u = p->x;
   pout->Set(c * u -  s * p->y , s * u + c * p->y, p->z);
   return(1);

}


// apply deformation functions on points

int GvMorph::DeformPoints(   
        double t,       
        int np,
        const Point *p1,const Point *n1,
        const Point *scenter,double sradius,
        Point *pout,Point *nout, 
        int (*func)(const Point *p,float amount, Point *pout))
{ 
  int i;
  Point p;

  for(i=np; i>0; i--,p1++,pout++) {
        p = (*p1 - *scenter) ;
        p /= sradius;

        func(&p,t,&p);
        p *= sradius;

        p += *scenter;
        

        *pout = p;
  }
  return(1);
}




int map_wave(const Point *p,float nwaves,float wave_offset,float wave_radius,
                        Point *pout)
{ double x,y;
  x = (wave_offset+p->x) * PI; //  TWOPI; because x goes from -1 to 1 
  y= sin(x*nwaves);
  pout->Set(p->x,p->y,p->z+y*wave_radius);
  return(0);
}

int GvMorph::WavePoints(
        int mode,
        double t,
        int np,
        const Point *p1,const Point *n1,
        const Point *scenter,double sradius,
        Point *pout,Point *nout)
{ int i;
  double t2;
  double l;
  Point p,d;


 // nwaves=params.nwaves,wave_offset=params.wave_offset,wave_radius=params.wave_radius*sradius;
  float wave_radius2=wave_radius*sradius;
  t2=sqr(t);
/*  wave_offset+=2*t; */
  wave_radius2 *= t;

  for(i=np; i>0; i--,p1++,pout++) {
        p = *p1 - *scenter;
        map_wave(&p,wave_nwaves,wave_offset,wave_radius2,&p);
        p += *scenter;

        *pout = p;
        if (nout) {
/*           *nout=d; nout ++; */
        }
  }
  return(0);
}


int map_waving(const Point *p,const Point &Phase,
			   const Point &amplitude, float phaseShift,
               Point *pout)
{ double y;

  double u = p->x;


  float a = Interpolate(amplitude.x, (1.0-u), amplitude.y, u);
  float phase = Interpolate(Phase.x, (1.0-u), Phase.y, u);

  u *=phase;

  u += phaseShift;
  u *= TWOPI;

  y= a*sin(u);

  pout->Set(p->x+y,p->y,p->z);
  return(0);
}


// waving points, p should be normalized to 0..1 
int GvMorph::WavingPoints(
        int mode,
        double t,
        int np,
        const Point *p1,const Point *n1,
        const Point *scenter,double sradius,
        Point *pout,Point *nout)
{ int i;
  double l;
  Point p,d;

  Point phaseScale = Interpolate(phaseScale1,1.0-t,phaseScale2,t);
  Point amplitude = Interpolate(amplitude1,1.0-t,amplitude2,t);
  float phaseShift = Interpolate(phaseShift1,1.0-t,phaseShift2,t);




  for(i=np; i>0; i--,p1++,pout++) {
      p = *p1 - *scenter;
		p /= sradius;

      map_waving(&p,phaseScale,amplitude,phaseShift,&p);

		p *= sradius;

      p += *scenter;

      *pout = p;
      if (nout) {
/*          *nout=d; nout ++; */
      }
  }
  return(0);
}


int map_swirl(const Point *p,float nwaves,float wave_offset,float wave_radius,
                        Point *pout)
{ double x,y,w,sinw,cosw;
  w = ((p->z +1.0) / 2.0) *nwaves * TWOPI;
  sinw=sin(w); cosw=cos(w);
  x = p->x *cosw - p->y * sinw;
  y = p->x *sinw + p->y * cosw;
  pout->Set(x,y,p->z);
  return(0);
}

int GvMorph::SwirlPoints(
        int mode,
        double t,
        int np,
        const Point *p1,const Point *n1,
        const Point *scenter,double sradius,
        Point *pout,Point *nout)
{ int i;
  Point p,d;
 
  float wave_radius2=wave_radius*sradius;

  float nwaves = t * wave_nwaves;

  for(i=np; i>0; i--,p1++,pout++) {
        p= *p1 - *scenter;
        map_swirl(&p,nwaves,wave_offset,wave_radius2,&p);
        p += *scenter;

        *pout = p;
        if (nout) {
/*           *nout=d; nout ++; */
        }
  }
  return(0);
}


int mapPoints_to_hantel(
        int mode,
        int np,
        const Point *p1,const Point *n1,
        const Point *scenter,double sradius,double sradius2,

        Point *pout,Point *nout)
{ int i;
  double l;
  Point p,d;
  for(i=np; i>0; i--,p1++,pout++) {
        double r;
        p = *p1 - *scenter;
        
        l = sqrt(sqr(p.x)+sqr(p.y));
        
        d.Set(p.x,p.y,p.z);;
        
        if ( l>0.0) { d.x = p.x / l;d.y = p.y / l; }
        r = sradius * f_hantel(p1->z/sradius,1.0,sradius2/sradius);

        //r = f_hantel(p1->z,sradius,sradius2);

        pout->Set(scenter->x+d.x*r,scenter->y+d.y*r,p1->z);
        if (nout) {
             *nout=d; nout ++;
        }
  }
  return(0);
}


int mapPoints_to_cylinder(
        int mode,
        int np,
        const Point *p1,const Point *n1,
        const Point *scenter,double sradius,

        Point *pout,Point *nout)
{ int i;
  double l;
  Point p,d;
  for(i=np; i>0; i--,p1++,pout++) {
        p = *p1 - *scenter;
        l = sqrt(sqr(p.x)+sqr(p.y));
        d.Set(p.x,p.y,p.z);;
        if ( l>0.0) { d.x = p.x / l;d.y = p.y / l; }

        pout->Set(scenter->x+d.x*sradius,scenter->y+d.y*sradius,p1->z);
        if (nout) {
			 d.Normalize();
             *nout=d; nout ++;
        }
  }
  return(0);
}

int mapPoints_to_cone(
        int mode,
        int np,
        const Point *p1,const Point *n1,
        const Point *scenter,double sradius,

        Point *pout,Point *nout)
{ int i;
  double l;
  Point p,d;
  for(i=np; i>0; i--,p1++,pout++) {
        p = *p1 - *scenter;
		
		float z = (p.z+sradius) / (2.0*sradius);
		float coneRadius = Interpolate(sradius,(1.0-z),0.0,z);

        l = sqrt(sqr(p.x)+sqr(p.y));

        d.Set(p.x,p.y,p.z);;
        if ( l>0.0) { d.x = p.x / l;d.y = p.y / l; }
        
        pout->Set(scenter->x+d.x*coneRadius,scenter->y+d.y*coneRadius,p1->z);

        if (nout) {
			 d.Normalize();
             *nout=d; nout ++;
        }
  }
  return(0);
}


int mapPoints_to_box(
        int mode,
        int np,
        const Point *p1,const Point *n1,
        const Point *box_center,
        const Point *box,

        Point *pout,Point *nout)
{ int i;
  float t1,t2;
  Point p,d;
  for(i=np; i>0; i--,p1++,pout++) {
        p = *p1;
//        d = *p1 - *box_center;
        d = (*p1 - *box_center) * 3.0; // new

        if (intersect_ray_box(&p,&d,box,&t1,&t2)) {
                   VECOP4C(p,=,p,+,d,*,t1);
/*                 VECPRINT(p); */
        }
        *pout=p;
        if (nout) {
             box_normal(box,&p,&d);
             *nout=d; nout ++;
        }
  }
  return(0);
}

int GvMorph::InterpolatePointsToSphere(int mode,
        float t,
        int np,
        const Point *p1,const Point *n1,
        const Point *scenter,double sradius,
        Point *pout,Point *nout)
{
   PointArray p2(np),n2(np);
   mapPoints_to_sphere(0,intersect,np,p1,n1,scenter,sradius,p2,n2);
   InterpolatePoints(mode,t,np,p1,n1,p2,n2,pout,nout);
   return(0);
}


/*----------------------------------------------------------------------*/
inline double interpolate(double percent, double zero_val, double one_val)
{
    return(percent*one_val + (1.0-percent)*zero_val);
}

/*----------------------------------------------------------------------*/
inline double mini_hermite(double r, double top_r)
{
    if (r<top_r) return(0.0);
    ASSERT(top_r != 1.0);
    r = (r-top_r)/(1.0-top_r);  /* Renormalize */
    return((3-r-r)*r*r);
}
/*----------------------------------------------------------------------*/
void project_Point_to_radius(double newr, Point *newp1)
{
    double r;
    if (newp1->x==0.0 && newp1->y==0.0)
        return;                             /* It *is* the center! */
    /* Normalize the direction vector... */
    r = newr/sqrt(newp1->x*newp1->x + newp1->y*newp1->y);
    newp1->x *= r;
    newp1->y *= r;
    /* z is unchanged */
}
/*----------------------------------------------------------------------*/
inline double calc_ellipse_r(double percent, double r, double a, double max_radius)
{
    return((0.9*percent + 1.1*(1.0-r)*a)*max_radius);
}
/*----------------------------------------------------------------------*/
inline double calc_ellipse_z(double r)
{   /* The following was derived to keep the area under an ellipse constant. */
    return(1.0-sqrt(1.0 - r*r));
}
/*----------------------------------------------------------------------*/
inline double calc_hermite_r(double BOT_RAD, double TOP_RAD, double z_percent)
{   double r = 1.0 - z_percent;
    /* the following was (TOP_RAD + BOT_RAD*r); */
    return (BOT_RAD*r < TOP_RAD ? TOP_RAD : BOT_RAD*r);
}
/*----------------------------------------------------------------------*/
inline double calc_hermite_z(double BOT_RAD, double TOP_RAD, double z_percent)
{
    double r = 1.0 - z_percent;
    return(1.0 - mini_hermite(r, TOP_RAD/BOT_RAD));
}
/*---------------------------------------------------------------------- */
/* const double K=2.0; */
static float K=2.0;
double last_z_percent;

/*---------------------------------------------------------------------- */
void tweakPoints(double percent,
        int np,const Point *p,
        const Point *pmin, const Point *pmax,
        Point *pout)
{
    Point  newp1;
    double ellipse_r, ellipse_z;
    double hermite_r, hermite_z;
    double newr, z_percent;
    double ZSIZE,TOP_RAD,BOT_RAD, fade;
    double a = (1.0 - percent*percent)/(percent + (1.0/K));
    const Point *pp;
    Point *poutp;
    int i;

    Point center;
    double max_radius, height;

    double nx = 0.5*(pmax->x - pmin->x);
    double ny = 0.5*(pmax->y - pmin->y);

    max_radius =(sqrt(nx*nx+ny*ny));

    center.x = 0.5*(pmax->x + pmin->x);
    center.y = 0.5*(pmax->y + pmin->y);
    center.z = 0.5*(pmax->z + pmin->z);
    height   = (pmax->z - pmin->z);

    ZSIZE = percent*height;           /* Original mainobj */
    /* Note that TOP_RAD must NOT ever equal BOT_RAD! */
    TOP_RAD = 0.9*percent*max_radius; /* Original mainobj */
    fade = percent;
    BOT_RAD = interpolate(percent, K*max_radius,max_radius);

    ASSERT(TOP_RAD != BOT_RAD);
    ASSERT(height != 0);
    ASSERT(max_radius != 0);
    last_z_percent = -1.0;

    for (pp=p, poutp=pout,i=0; i<np; i++,pp++,poutp++) {
      newp1.x = pp->x - center.x;   /* translate Point to origin */
      newp1.y = pp->y - center.y;
      newp1.z = pp->z - pmin->z;

      z_percent = newp1.z/height;

      ellipse_r = calc_ellipse_r(percent, z_percent, a, max_radius);
      ellipse_z = calc_ellipse_z(z_percent);                    /* 0..1 */
      hermite_r = calc_hermite_r(BOT_RAD, TOP_RAD, z_percent);
      hermite_z = calc_hermite_z(BOT_RAD, TOP_RAD, z_percent);  /* 0..1 */

      newr    = interpolate(fade, hermite_r, ellipse_r);
      newp1.z = ZSIZE * interpolate(fade, hermite_z, ellipse_z);

      project_Point_to_radius(newr, &newp1);

      newp1.x += center.x;
      newp1.y += center.y;
      newp1.z += pmin->z;
      /* We now have the new Point location... fade to object */
      poutp->x = interpolate(fade, newp1.x, pp->x);
      poutp->y = interpolate(fade, newp1.y, pp->y);
      poutp->z = interpolate(fade, newp1.z, z_percent*ZSIZE+pmin->z);
    }
}


// compute in between for time t
int GvMorph::Do(GMorphObject &object, float t) 
{
  int mode = 0; // ???
  int np = object.orgV.Length();	// number of Points
  
  const Point *p=object.orgV;
  const Point *n=object.orgVN;
  PointArray pm;
  PointArray nm;

  MatrixAndInverse mCompound;
  MatrixAndInverse *mToUse = NULL;  // Matrix to transform points with 

  
  if (object.hasMatrix) {
	int i;
  	pm.Set(np,p);	 
    if (!rotation.isDefault()) {
        Matrix m;
        rotation.get(m);
        mCompound.m= object.matrix.m;
        mCompound.Append(m);
        mToUse = &mCompound;

    }
    else mToUse = &object.matrix; 
	
    for (i=0; i<np;i++) 
		pm[i] *= mToUse->m;
	
	p = pm.Data();;

    Matrix use = Transpose(mToUse->inverse);

  	nm.Set(np,n);	 
	for (i=0; i<np;i++) 
		nm[i] = RotateOnly(use,nm[i]);
	
	n = nm.Data();;

  }

  PointArray pout(np);	// new vertices
  PointArray nout(np);

        switch (type) {
        case MELT :
             tweakPoints(t,np,p,&object.smin,&object.smax,pout);
             break;
        case WAVE :
            WavePoints(0,t,np,p,n,&scenter,sradius,pout,nout);
            break;
        case WAVING :
            WavingPoints(0,t,np,p,n,&scenter,sradius,pout,nout);
            break;
        case SWIRL:
           SwirlPoints(0,t,np,p,n,&scenter,sradius,pout,nout);
           break;

        case PINCH:
           DeformPoints(t,np,p,n,&scenter,sradius,pout,nout,pinch);
           break;

        case TAPER:
           DeformPoints(t,np,p,n,&scenter,sradius,pout,nout,taper);
           break;
        case TWIST:
           DeformPoints(t,np,p,n,&scenter,sradius,pout,nout,twist);
           break;

        case BEND:
           DeformPoints(t,np,p,n,&scenter,sradius,pout,nout,bend);
           break;

        default : {
          PointArray p2(np), n2(np);

          /* compute destination (mapping coordinates) */
          switch (type) {
          case SPHERE:
             mapPoints_to_sphere(0,intersect,np,p,n,&scenter,sradius,p2,n2); break;
          case CYLINDER:
            mapPoints_to_cylinder(0,np,p,n,&scenter,sradius,p2,n2); break;
          case CONE:
            mapPoints_to_cone(0,np,p,n,&scenter,sradius,p2,n2); break;
          case BOX:
            { Point box[2];
              /* */
              sradius=1.0;
              //box[0].Set(scenter.x-sradius,scenter.y-sradius,scenter.z-sradius);
              //box[1].Set(scenter.x+sradius,scenter.y+sradius,scenter.z+sradius);
			  box[0]=object.box.mi;
			  box[1]=object.box.ma;
              mapPoints_to_box(0,np,p,n,&scenter,box,p2,n2);
            }
            break;
          case HANTEL:
            mapPoints_to_hantel(0,np,p,n,&scenter,sradius,sradius*0.5 /* *sradius2*/,p2,n2); 
            break;
          }
          InterpolatePoints(mode,t,np,p,n,p2,n2,pout,nout);
        }
        } // switch


  float scaleFac = 1.0*(1.0-t) + rscale * t;
  
  for(int i=0; i<np;i++) { pout[i]-=scenter; pout[i]*=scaleFac; pout[i]+=scenter; }

  // replace vertex coordinates

  if (mToUse) {
  	pm.Set(np,pout);
	for (int i=0; i<np;i++) 
		pm[i] *= mToUse->inverse;
	 object.theShell->SetV(pm);
  }
  else 
    object.theShell->SetV(pout);

  return(0);
}


//
/* see also Graphic Gems I, Pg. 388 */
int intersect_ray_sphere(
			/* Ray */
			const Point * ray_p, const Point /*vector*/ *ray_d,
			/* sphere */
			const Point *sph_center, float sph_radius,
			/* intersection */
			float *isect_t )
{

	float b, disc, t;
	Point V ;
	float sph_radius2 = sph_radius * sph_radius;

	V= *sph_center- *ray_p;

	b = DotProd(V, *ray_d);

	disc = b * b - DotProd(V, V) + (sph_radius2) ;

	if (disc < 0.0)
		return(0);

	disc = sqrt(disc);

	t = (b - disc < EPS) ? b + disc : b - disc ;

	if (t < EPS) {
		return(0);
	}

	*isect_t = t ;
/*	hit -> isect_enter = VECDOT(V, V) > sp -> sph_radius2 + EPS ? 1 : 0 ; */
	return (1);
}

/* ************************************************************************** */
#ifndef EPSILON
#define EPSILON 1.0e-10
#endif

#ifndef HUGE_VAL
#define HUGE_VAL 1.0e+17
#endif

#define close(x, y) (fabs(x-y) < EPSILON ? 1 : 0)


int intersect_ray_box (const Point *rayP, const Point *rayD,
		       const Point *box, float *Depth1, float *Depth2)
  {
  double t, tmin, tmax;
  Point P, D;

    P = *rayP;
    D = *rayD;

    tmin = 0.0;
    tmax = HUGE_VAL;

  /* Sides first */
  if (D.x < -EPSILON)
    {
    t = (box[0].x - P.x) / D.x;
    if (t < tmin)
      return 0;
    if (t <= tmax)
      tmax = t;
    t = (box[1].x - P.x) / D.x;
    if (t >= tmin)
      {
      if (t > tmax)
	return 0;
      tmin = t;
      }
    }
  else if (D.x > EPSILON)
    {
    t = (box[1].x - P.x) / D.x;
    if (t < tmin)
      return 0;
    if (t <= tmax)
      tmax = t;
    t = (box[0].x - P.x) / D.x;
    if (t >= tmin)
      {
      if (t > tmax)
	return 0;
      tmin = t;
      }
    }
  else if (P.x < box[0].x || P.x > box[1].x)
    return 0;

  /* Check Top/Bottom */
  if (D.y < -EPSILON)
    {
    t = (box[0].y - P.y) / D.y;
    if (t < tmin)
      return 0;
    if (t <= tmax)
      tmax = t;
    t = (box[1].y - P.y) / D.y;
    if (t >= tmin)
      {
      if (t > tmax)
	return 0;
      tmin = t;
      }
    }
  else if (D.y > EPSILON)
    {
    t = (box[1].y - P.y) / D.y;
    if (t < tmin)
      return 0;
    if (t <= tmax)
      tmax = t;
    t = (box[0].y - P.y) / D.y;
    if (t >= tmin)
      {
      if (t > tmax)
	return 0;
      tmin = t;
      }
    }
  else if (P.y < box[0].y || P.y > box[1].y)
    return 0;

  /* Now front/back */
  if (D.z < -EPSILON)
    {
    t = (box[0].z - P.z) / D.z;
    if (t < tmin)
      return 0;
    if (t <= tmax)
      tmax = t;
    t = (box[1].z - P.z) / D.z;
    if (t >= tmin)
      {
      if (t > tmax)
	return 0;
      tmin = t;
      }
    }
  else if (D.z > EPSILON)
    {
    t = (box[1].z - P.z) / D.z;
    if (t < tmin)
      return 0;
    if (t <= tmax)
      tmax = t;
    t = (box[0].z - P.z) / D.z;
    if (t >= tmin)
      {
      if (t > tmax)
	return 0;
      tmin = t;
      }
    }
  else if (P.z < box[0].z || P.z > box[1].z)
    return 0;

  *Depth1 = tmin;
  *Depth2 = tmax;

/*   printf("Box intersects: %g, %g\n", *Depth1, *Depth2); */
#define Small_Tolerance 0.00001
#define Max_Distance 1.0e10

  if ((*Depth1 < Small_Tolerance) || (*Depth1 > Max_Distance))
    if ((*Depth2 < Small_Tolerance) || (*Depth2 > Max_Distance))
      return (0);
    else
      *Depth1 = *Depth2;
  else
    if ((*Depth2 < Small_Tolerance) || (*Depth2 > Max_Distance))
      *Depth2 = *Depth1;
  return (1);
  }

int Point_inside_box (const Point *IPoint, const Point *box)
  {
  Point New_Point;

  /* Test to see if we are inside the box */
  if (New_Point.x < box[0].x || New_Point.x > box[1].x)
    return (0);
  if (New_Point.y < box[0].y || New_Point.y > box[1].y)
    return (0);
  if (New_Point.z < box[0].z || New_Point.z > box[1].z)
    return (0);
  /* Inside the box */
  return 1;
  }

void box_normal (const Point *box,const Point * IPoint,Point *Result)
  {
  Point New_Point;

  New_Point = *IPoint;

   Result->x = 0.0; Result->y = 0.0; Result->z = 0.0;
  if (close(New_Point.x, box[1].x))
    Result->x = 1.0;
  else if (close(New_Point.x, box[0].x))
    Result->x = -1.0;
  else if (close(New_Point.y, box[1].y))
    Result->y = 1.0;
  else if (close(New_Point.y, box[0].y))
    Result->y = -1.0;
  else if (close(New_Point.z, box[1].z))
    Result->z = 1.0;
  else if (close(New_Point.z, box[0].z))
    Result->z = -1.0;
  else
    {
    /* Bad result, should we do something with it? */
      Result->x = 1.0;
    }

  }
#undef close




/* Argument: 0..1, Wert : 0..1 */
float f_lin(float x) { return(x); }
float f_lin_i(float x) { return(1.0-x); }

float f_linclamp(float x)
{ if (x<=0.0) return (0.0);
  if (x>=1.0) return (1.0);
  return(x);
}
float f_rect(float x)
{ if (x<0.5) return (0.0);
  return(1.0);
}

float f_saw(float x)
{ float t0=0.3333333333;
  float t1=2*t0;
  float dt=t1-t0;
  if (x<t0) return (0.0);
  if (x>=t1) return (0.0);
  /* triangle */
  if (x<=0.5) return ((x-t0)/ (0.5-t0));
  return(1.0- (x-0.5)/(t1-0.5));
}


float f_tria(float x)
{ if (x<=0.0) return (0);
  if (x>=1.0) return (0.0);
  if (x<=0.5) return (x*2.0);
  return(1.0- (x-0.5)*2.0);
}

float f_sqr(float x) { return(x*x); }
float f_cub(float x) { return(x*x*x); }
float f_sin(float x) { return(sin(x*PI)); }
float f_smooth(float x) { return((-cos(x*PI)+1)*0.5); }
float f_rnd(float x) { return((float)rand() / (float)RAND_MAX); }

void ft_3d_lin(Point *v,float x)
{ v->Set(x,x,x); }

void ft_3d_sqr(Point *v,float x)
{ x=f_sqr(x);
  v->Set(x,x,x);
}

void ft_3d_cub(Point *v,float x)
{ x=f_cub(x);
  v->Set(x,x,x);
}

void ft_3d_sin(Point *v,float x)
{ x=f_sin(x);
  v->Set(x,x,x);
}

void ft_3d_rnd(Point *v,float x)
{ v->Set(f_rnd(x),f_rnd(x),f_rnd(x)); }

void ft_3d_sin23(Point *v,float x)
{  x=x*PI;
   v->Set(sin(2*x),sin(3*x),sin(4*x));
}

void ft_3d_circlexy(Point *v,float x)
{  x=x*2*PI;
   v->Set(sin(x),cos(x),0);
}



const float bspline_basis[4][4] = {  /* t3 t2 t1 1 */
				{ -1, 3,-3,1},
				{ 3, -6, 0,4},
				{ -3, 3, 3,1},
				{ 1, 0, 0,0 },
			    };
const float bspline_scale = 0.16666666666666;

const float catmull_basis[4][4] = {  /* t3 t2 t1 1 */
				{ -1, 2,-1,0},
				{ 3, -5, 0,2},
				{ -3, 4, 1,0},
				{ 1, -1, 0,0 },
			    };
const float catmull_scale = 0.5;

void compute_spline_basis_Point(const Point *p,
		float scale,const float m[4][4], float t,Point *o)
{ float t2,t3;
  float weight[4];
  Point sum;
  t2=t*t; t3=t2*t;
  weight[0]= scale*(t3*m[0][0] + t2*m[0][1] + t*m[0][2] + m[0][3]);
  weight[1]= scale*(t3*m[1][0] + t2*m[1][1] + t*m[1][2] + m[1][3]);
  weight[2]= scale*(t3*m[2][0] + t2*m[2][1] + t*m[2][2] + m[2][3]);
  weight[3]= scale*(t3*m[3][0] + t2*m[3][1] + t*m[3][2] + m[3][3]);
  VECOP3C(sum, =,p[0],*,weight[0]);
  VECOP3C(sum,+=,p[1],*,weight[1]);
  VECOP3C(sum,+=,p[2],*,weight[2]);
  VECOP4C(*o,=,sum,+,p[3],*,weight[3]);
}

/* hermite is invariant to rotation, translation */

void compute_hermite_Point_pt(const Point *p1,const /*Vector*/ Point *r1,
			      const Point *p4,const /* Vector*/ Point *r4,
			      float t,Point *p)
{ Point sum;
  float t2,t3,w;
  t2=t*t; t3=t2*t;

  w=2.0*t3-3.0*t2+1.0;
  VECOP3C(sum,=,*p1,*,w);

  w=-2.0*t3+3.0*t2;
  VECOP3C(sum,+=,*p4,*,w);

  w=t3-2.0*t2+t;
  VECOP3C(sum,+=,*r1,*,w);

  w=t3-t2;
  VECOP4C(*p,=,sum,+,*r4,*,w);
}

void compute_hermite_curve_Point(int np,const Point *p,float t,Point *o)
{ int parts,part;
  int offset;
  /*Vector */ Point r1,r4;
  float tp;
  parts = (np-1) / 3;
  part = t*parts;
  if (part >= parts) part=parts-1;
  offset = part * 3;
  tp = (t*parts) - (part);

/*  if (t< 0.01) printf("np = %d, parts = %d, part = %d, offset = %d tp=%g\n",np,parts,part,offset,tp); */

  r1 = p[offset+1] - p[offset];
  r4 = p[offset+3] - p[offset+2];
  compute_hermite_Point_pt(&p[offset],&r1,&p[offset+3],&r4,tp,o);
}

void compute_hermite2_curve_Point(int np,const Point *p,float weight,float t,Point *o)
{ int parts,part;
  int offset;
  Point r1,r4;
  Point d1,d2;
  float tp;
  parts = (np-1) / 1;
  part = t*parts;
  if (part >= parts) part=parts-1;
  offset = part * 1;
  tp = (t*parts) - (part);

/*  if (t< 0.01) printf("np = %d, parts = %d, part = %d, offset = %d tp=%g\n",np,parts,part,offset,tp); */
  if (offset > 0) {
     VECSUB(d1,p[offset+1],p[offset-1]);
  } else  { VECSUB(d1,p[offset],p[offset-1]); }
  if ((offset-1) <np) {
     VECSUB(d2,p[offset+2],p[offset]);
  } else
  if ((offset) <np) {
     VECSUB(d2,p[offset+1],p[offset]);
  } else
    { VECZERO(d2); }

  VECOP3C(r1,=,d1,*,weight);
  VECOP3C(r4,=,d2,*,weight);
  compute_hermite_Point_pt(&p[offset],&r1,&p[offset+1],&r4,tp,o);
}

void compute_kochanek_curve_Point(int np,const Point *p,float t,Point *o)
{ int parts,part;
  int offset;
  Point r1,r4;
  Point d1,d2;
  float w1,w2;
  float tp;
  parts = (np-1);
  part = t*parts;
  if (part >= parts) part=parts-1;
  offset = part ;
  tp = (t*parts) - (part);

/*  if (t< 0.01) printf("np = %d, parts = %d, part = %d, offset = %d tp=%g\n",np,parts,part,offset,tp); */
  if (offset > 0) {
     VECSUB(d1,p[offset],p[offset-1]);
  } else  { VECZERO(d1); }
  if (offset <np) {
     VECSUB(d2,p[offset+1],p[offset]);
  } else { VECZERO(d2); }

  w1 = 0.5;
  w2 = 0.5;
  VECOP3C(r1,=,d1,*,w1);
  VECOP3C(r1,+=,d2,*,w2);
  VECOP3C(r4,=,d1,*,w1);
  VECOP3C(r4,+=,d2,*,w2);

  compute_hermite_Point_pt(&p[offset],&r1,&p[offset+1],&r4,tp,o);
}

void compute_bspline_curve_Point(int np,const Point *p,float t,Point *o)
{ int parts,part;
  int offset;
  float tp;
/*
  parts = (np-1) / 3;
  part = t*parts;
  if (part >= parts) part=parts-1;
  offset = part * 3;
*/
  parts = (np-1-2) / 1;
  part = t*parts;
  if (part >= parts) part=parts-1;
  offset = part * 1;
  tp = (t*parts) - (part);

/*  if (t< 0.01) printf("np = %d, parts = %d, part = %d, offset = %d tp=%g\n",np,parts,part,offset,tp); */
  compute_spline_basis_Point(&p[offset],bspline_scale,bspline_basis,tp,o);
}

#if 0
void compute_arc_curve_Point(int np,const Point *p,float t,Point *o)
{ int parts,part;
  int offset;
  Point center;
  float tp;
  parts = (np-1) / 2;
  part = t*parts;
  if (part >= parts) part=parts-1;
  offset = part * 2;
  tp = (t*parts) - (part);
  if (compute_circle_center(&p[offset],&center) )
     polar_ipol_short(&center,&p[offset],&p[offset+2],tp,o);
  else { VECIPOL(*o,p[offset],p[offset+2],tp); }

/*  if (t< 0.01) printf("np = %d, parts = %d, part = %d, offset = %d tp=%g\n",np,parts,part,offset,tp); */
}
#endif

void compute_catmull_curve_Point(int np,const Point *p,float t,Point *o)
{ int parts,part;
  int offset;
  float tp;
  parts = (np-1-2) / 1;
  part = t*parts;
  if (part >= parts) part=parts-1;
  offset = part * 1;
  tp = (t*parts) - (part);
/*  if (t< 0.01) printf("np = %d, parts = %d, part = %d, offset = %d tp=%g\n",np,parts,part,offset,tp); */
  compute_spline_basis_Point(&p[offset],catmull_scale,catmull_basis,tp,o);
}


int GvMorph::Do(GTraversal &state)   
{ 
    for (int i = 0; i < getNumChildren(); i++) {
		if (state.stop) break;
		getChild(i)->Traverse(state);
	}
	return(1);
}

int GvMorph::Do(GRenderTraversal &state)   
{ 
    for (int i = 0; i < getNumChildren(); i++) {
		if (state.stop) break;
		getChild(i)->Traverse(state);
	}
	return(1);
}

class GMorphFindShellsTraversal : public GGeometryTraversal {

public :
//	RT(GMorphFindShellsTraversal);

	GMorphFindShellsTraversal() : GGeometryTraversal() { objects = NULL; }
    Array<GMorphObject*> *objects;



  	void InitializeAttributes() {
  		InitializeGvAttributes(*this);
		GGeometryTraversal :: InitializeAttributes();
   	}

	int Do(GNode *n);

};


//RTIMP(GMorphFíndShellsTraversal,0,GGeometryTraversal);

int GMorphFindShellsTraversal::Do(GNode *n) 
{	GShell * theShell=NULL;
	if (RTISA(n,GShell)) theShell = RTCAST(n,GShell);
	else if (RTISA(n,GvNodeShell)) theShell = ((GvNodeShell*) n)->theShell;
	else return(GGeometryTraversal::Do(n)); // pass down

	if (theShell) {
		 // mode reloadMatrix
		 GMorphObject *newObj;
		 newObj = new GMorphObject();
		 GMatrixStack *m= (GMatrixStack*)Get(GMatrixStack::attrId);
		 if (m) { 
		   newObj->matrix.Set(m->m);
		   newObj->hasMatrix = 1;
		 }

		 newObj->SetShell(theShell);

		 objects->Append(newObj);
		 return(1);

	}
	return (0); 
}


int GvMorph::Do(GglRenderTraversal &state)   
{ 
	GMatrixStack *current,*prev;
	prev = (GMatrixStack*) state.Get(GMatrixStack::attrId);
	GTimeStack  *ts = (GTimeStack*) state.Get(GTimeStack::attrId);

	if (on && ts &&ts->time.On()) 
	{
		
      // get parameter t
      float t=ts->time.t;

 	  if (t<0.0 || (t>1.0)) t = fmod(t,1.0f);

      if (cycle) {
		   t = f_tria(t);
      }

      // map to interval
      t = Interpolate(t0,1.0-t,t1,t);

		if (!init) {
			GMorphFindShellsTraversal traversal;
			traversal.view = state.view;
			traversal.InitializeAttributes();
			traversal.loadInlines = GTraversal::LOAD_ALL;
			traversal.objects = &objects;
			for (int i = 0; i < getNumChildren(); i++) 
				getChild(i)->Traverse(traversal);

			if (objects.Length() > 0) {
			  scenter=objects[0]->scenter;
			  sradius=objects[0]->sradius;
			  init = 1;
			}
		
		}
		// Morph objects
		int i,ret;
		for (i=0; i < objects.Length(); i++)
			ret = Do(*objects[i],t); 
		
	}

	// traverse childs / some nodes may have been modified
    for (int i = 0; i < getNumChildren(); i++) {
		if (state.stop) break;
		getChild(i)->Traverse(state);
	}
	
	return(1);
}


#endif
