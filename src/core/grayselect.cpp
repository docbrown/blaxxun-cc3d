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

@module GRaySelect.cpp - GLView picking traversal routines	|

Copyright (c) 1995 - 1998	by Holger Grahn
All rights reserved

Purpose:

Classes:
GRaySelectTraversal

Notes:

Changes:
    10.01.97 changed cull test (inverted !! was !)
	28.03.97 culltest only for mode select 

$Revision: 1.37 $
$Log: grayselect.cpp,v $
Revision 1.37  1999/11/29 20:30:11  holger
empty message

Revision 1.16  1998/10/07 18:34:25  holger
empty message

Revision 1.15  1998/10/07 13:22:05  holger
empty message


Todo :

    culling + VRML 1.0 shapeHints
	IndexedLineSet support

	Gv2SPhere normal + coord

******************************************************************************/

#include "stdafx.h"

#include "gclass.h"
#include "gshell.h"
#include "gshelli.h"
#include "ggeom.h"
#include "gcamera.h"
#include <gvfields.h>

#include <gvseparator.h>
#include <gvlod.h>
#include <gvwwwanchor.h>
#include <gvwwwinline.h>
#include <gvelement.h>
#include <gvwebfx.h>
#include <gv2geometry.h>
#include <gvindexedlineset.h>
#include <gvproto.h>

#include "greporter.h"

#include "grayselect.h"
#include "gvisibility.h"

#include "collision/rapid/rapid.h"


//
// GHitInfo
//

GHitInfo::~GHitInfo()
{
}

// search node in path, and retreive effective matrix of sensor
GvBool GHitInfo::GetSensorMatrix(GvNode *sensor, MatrixAndInverse &mret)
{
	Matrix m;
	m.Identity();
	GvPath &p = path;
	int l = p.path.Length();

	// Find inner most Group in path containing sensor
	int i;
	i=l-1;

	// mm Sensor could be itself in PROTO, need to eval grrrr.

	while (i>=0) {	
	  GvNode *n= (GvNode*)p.path[i].node;
	  if (RTISA(n,Gv2Group)) {
		  if (( ((Gv2Group *)n)->children.find(sensor)) >=0) 
				break;
	  }	
	  else if (RTISA(n,GvProtoInstance)) {
		  if (( ((GvProtoInstance *)n)->children.find(sensor)) >=0) 
				break;
	  }	
	  i--;
	}		
	if (i<0) {
#ifdef _DEBUG
		p.Trace();
		Reporter.Error("Couldn't find sensor in path, no matrix \n");
#endif		
		return FALSE;
	}
	p.getMatrix(m,i);
	mret.Set(m);
	return TRUE;

}
// search node in path, and retreive effective matrix of sensor
GvBool GHitInfo::GetSensorMatrix(GvNode *parent,GvNode *sensor, MatrixAndInverse &mret)
{
	Matrix m;
	m.Identity();
	GvPath &p = path;
	int l = p.path.Length();

	// Find inner most Group in path containing sensor
	int i;
	i=l-1;


	while (i>=0) {	
	  GvNode *n= (GvNode*)p.path[i].node;
	  if (n == parent) break;
	  i--;
	}		

	if (i<0) {
#ifdef _DEBUG
		p.Trace();
		Reporter.Error("Couldn't find sensor in path, no matrix \n");
#endif		
		return FALSE;
	}
	p.getMatrix(m,i);
	mret.Set(m);
	return TRUE;

}


// for mouse sensor find first ProtoInstance or inline after group
// containing sensor, otherwise use lowes level node

GvBool GHitInfo::GetTargetNode(GvNode *sensor,MatrixAndInverse &mret,GvNodeHandle &target)
{
	Matrix m;
	m.Identity();
	GvPath &p = path;
	int l = p.path.Length();

	// Find inner most Group in path containing sensor
	int i;
	i=l-1;

	// mm Sensor could be itself in PROTO, need to eval grrrr.

	while (i>=0) {	
	  GvNode *n= (GvNode*)p.path[i].node;
	  if (RTISA(n,Gv2Group)) {
		  if (( ((Gv2Group *)n)->children.find(sensor)) >=0) 
				break;
	  }	
	  else if (RTISA(n,GvProtoInstance)) {
		  if (( ((GvProtoInstance *)n)->children.find(sensor)) >=0) 
				break;
	  }	
	  i--;
	}		
	if (i<0) {
#ifdef _DEBUG
		p.Trace();
		Reporter.Error("Couldn't find sensor in path, no matrix \n");
#endif		
		return FALSE;
	}


	p.getMatrix(m,i);
	mret.Set(m);
	while (i<l) {
	  GvNode *n= (GvNode*)p.path[i].node;
	  if (RTISA(n,GvProtoInstance)) {
		 break;
	  }	
	  else if (RTISA(n,Gv2Inline)) {
		 break;
	  }	
	  i=i+1;
	}
	if (i>=l) i=l-1;
	if (i>=0) 
		target = p.path[i].node;
	return TRUE;

}



int GNode::Do(GRaySelectTraversal &state) // declared in GClass.h
{
   return Do(*((GPathTraversal *) &state));		
}


//
// class GRaySelectTraversal
//
RTIMP(GRaySelectTraversal,0,GPathTraversal);

void GRaySelectTraversal::InitializeAttributes()
{	
 	GGeometryTraversal::InitializeAttributes();
}

GRaySelectTraversal::~GRaySelectTraversal()
{
}



#ifndef EPSILON
	#define EPSILON 1.0e-10
#endif


#define close(x, y) (fabs(x-y) < EPSILON ? 1 : 0)

/*
  2.04.97 
  changed t to float, otherwith t>tmax yield true even if same values

  added EPSILON compare to d[i]== 0 case, is this necessary ?? 

*/


static int IntersectRayBox (const Point &P, const Point &D,
		       const Point *box, float *Depth1, float *Depth2,
               float tmin/*=0.0*/,
               float tmax=FLT_MAX,
               float Max_Distance=1.0e10)

{
  // double t;
  float t; // use always same precisiion 
  // tmin, tmax;

//  tmin = 0.0;
//  tmax = FLT_MAX;

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
  else if (P.x < (box[0].x-EPSILON) || P.x > (box[1].x+EPSILON))
    return 0;

  //TRACE("Box intersects x: %g, %g\n", tmin, tmax);

  /* Check Top/Bottom */
  if (D.y < -EPSILON)
    {
    t = (box[0].y - P.y) / D.y;

	//TRACE("y- 1: %g, %g %g %d \n", tmin, tmax,t,D.y);

    if (t < tmin)
      return 0;
    if (t <= tmax)
      tmax = t;

    t = (box[1].y - P.y) / D.y;
	
	//TRACE("y 2: %g, %g %g \n", tmin, tmax,t);

    if (t >= tmin)
      {
      if (t > tmax)	 //xxx problem is t was double !!!
		return 0;
	  tmin = t;
      }
    }
  else if (D.y > EPSILON)
    {
    t = (box[1].y - P.y) / D.y;
	
	//TRACE("y+ 1: %g, %g %g %d \n", tmin, tmax,t,D.y);

    if (t < tmin)
      return 0;
    if (t <= tmax)
      tmax = t;
    t = (box[0].y - P.y) / D.y;

	//TRACE("y 2: %g, %g %g \n", tmin, tmax,t);
    
	if (t >= tmin)
      {
      if (t > tmax)
		return 0;
	  tmin = t;
      }
    }
  else if (P.y < (box[0].y-EPSILON) || P.y > (box[1].y+EPSILON))
    return 0;

  //TRACE("Box intersects y: %g, %g\n", tmin, tmax);

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
  else if (P.z < (box[0].z-EPSILON) || P.z > (box[1].z+EPSILON))
    return 0;

  *Depth1 = tmin;
  *Depth2 = tmax;

  //TRACE("Box intersects: %g, %g\n", *Depth1, *Depth2);
/*
why 2.04.97 ??
#define Small_Tolerance 0.00001

  if ((*Depth1 < Small_Tolerance) || (*Depth1 > Max_Distance))
    if ((*Depth2 < Small_Tolerance) || (*Depth2 > Max_Distance))
      return (0);
    else
      *Depth1 = *Depth2;
  else
    if ((*Depth2 < Small_Tolerance) || (*Depth2 > Max_Distance))
      *Depth2 = *Depth1;
*/
  return (1);
}


// Transform a direction, "vec" using the transform "tx"
// Transform a point by a matrix.
Point
MultVector (const Matrix& M, const Point& v)
{
    Point ret;


    ret.x = v.x * M.x[0][0] + v.y * M.x[1][0] + v.z * M.x[2][0] ;
    ret.y = v.x * M.x[0][1] + v.y * M.x[1][1] + v.z * M.x[2][1] ;
    ret.z = v.x * M.x[0][2] + v.y * M.x[1][2] + v.z * M.x[2][2] ;
/*
    float denom;
    denom = M.x[0][3] + M.x[1][3] + M.x[2][3] + M.x[3][3];
    if (denom != 1.0)
	ret /= denom;
*/
    return ret;
}

#if 0
void
TxVec3(Vec out, Vec v, Transform *tx)
{
   int i;
   Vec tv;

   for (i=0;i<3;i++)
      tv[i] = v[0] * tx->matrix[0][i] +
              v[1] * tx->matrix[1][i] +
              v[2] * tx->matrix[2][i];
   for (i=0;i<3;i++)
      out[i] = tv[i];
}
#endif



/*  Graphic Gems I, Pg. 388 */
int IntersectRaySphere(
			/* Ray */
			const Point &ray_p, const Point &ray_d,
			/* sphere */
			const Point &sph_center, float sph_radius,
			/* intersection */
			float &isect_t )
{

	float b, disc, t;
	Point V ;
	float sph_radius2 = sph_radius * sph_radius;

	V = sph_center - ray_p;

	b = DotProd(V,ray_d);

	disc = b * b - DotProd(V, V) + (sph_radius2) ;
	// disc = sph_radius2 - (DotProd(V, V) - b*b)

	if (disc < 0.0)
		return(0);

	disc = sqrt(disc);

	t = (b - disc < EPS) ? b + disc : b - disc ;

	if (t < EPS) {
		return(0);
	}

	isect_t = t ;
/*	hit -> isect_enter = VECDOT(V, V) > sp -> sph_radius2 + EPS ? 1 : 0 ; */
	return (1);
}

//Gems IV p353
/* ---- intcyl - Intersect a ray with a cylinder. --------------------- */
/*									*/
/*									*/
/*	Description:							*/
/*	    Intcyl determines the intersection of a ray with a		*/
/*	    cylinder.							*/
/*									*/
/*	On entry:							*/
/*	    raybase = The base point of the intersecting ray.		*/
/*	    raycos  = The direction cosines of the above ray. (unit)	*/
/*	    base    = The base location of the cylinder.		*/
/*	    axis    = The axis of symmetry for the cylinder.  (unit)	*/
/*	    radius  = The radius of the cylinder.			*/
/*									*/
/*	On return:							*/
/*	    in	    = The entering distance of the intersection.	*/
/*	    out	    = The leaving  distance of the intersection.	*/
/*									*/
/*	Returns:  True if the ray intersects the cylinder.		*/
/*									*/
/*	Note:	  In and/or out may be negative indicating the		*/
/*		  cylinder is located behind the origin of the ray.	*/
/*									*/
/* -------------------------------------------------------------------- */

gbool IntersectRayCylinder(
	const Point		&raybase,	/* Base of the intersection ray */
	const Vector	&raycos,	/* Direction cosines of the ray */
	const Point		&base,		/* Base of the cylinder		*/
	const Vector	&axis,		/* Axis of the cylinder		*/
	float		radius,		/* Radius of the cylinder	*/
	float		&in,		/* Entering distance		*/
	float		&out)		/* Leaving distance		*/

{
	gbool  		hit;		/* True if ray intersects cyl	*/
	Vector		RC;		/* Ray base to cylinder base	*/
	float		d;		/* Shortest distance between	*/
						/*   the ray and the cylinder	*/
	float		t, s;		/* Distances along the ray	*/
	Vector		n, D, O;
	float		ln;

	const	float pinf = FLT_MAX;	/* Positive infinity		*/


	RC.x = raybase.x - base.x;
	RC.y = raybase.y - base.y;
	RC.z = raybase.z - base.z;
	n = CrossProd (raycos, axis);

	if  ( (ln = n.Length()) == 0. ) {	/* ray parallel to cyl	*/
	    d	 = DotProd (RC,axis);
	    D.x	 = RC.x - d*axis.x;
	    D.y	 = RC.y - d*axis.y;
	    D.z	 = RC.z - d*axis.z;
	    d	 = D.Length();
	    in	 = -pinf;
	    out =  pinf;
	    return (d <= radius);		/* true if ray is in cyl*/
	}

	n.Normalize();
	d    = fabs (DotProd (RC,n));		/* shortest distance	*/
	hit  = (d <= radius);

	if  (hit) {				/* if ray hits cylinder */
	    O = CrossProd(RC,axis);
	    t = - DotProd (O,n) / ln;
	    O = CrossProd(n,axis);
	    O.Normalize();
	    s = fabs (sqrt(radius*radius - d*d) / DotProd (raycos,O));
	    in	 = t - s;			/* entering distance	*/
	    out = t + s;			/* exiting  distance	*/
	}

	return (hit);
}

// fill hitInfo with required state information
void GRaySelectTraversal::FillHitInfo(GHitInfo &hit) 
{
		// store anchor / sensor elements 
		if (1) {
				GvWWWAnchorElement *current;
				current = (GvWWWAnchorElement*) this->Get(GvWWWAnchorElement::attrId);

				// store all elements at this level !!!!!
				if (current)  {

					hit.sensors.SetLength(0); // flush sensor list 

					int myLevel = current->level;

					// if (current->data) 	hit.wwwInline = current->data;

					do {
						if (current->level != myLevel) break;
				   
						if (current->data)
							hit.sensors.Append(current->data);
					
						// follow next sensor / 
						current = (GvWWWAnchorElement*) current->prevThisAttr;


					} while(current != NULL);
				}
		}

		if (storeFullHitInfo) {
		    GvWWWInlineElement *current;
			current = (GvWWWInlineElement*) this->Get(GvWWWInlineElement::attrId);
			if (current && current->data) 
				hit.wwwInline = current->data;
		}
		if (storePath) {
			hit.path.SetByList(topPath);
		}
}

// append/Replace hit
// Return 1 if hit was stored

int GRaySelectTraversal::AppendHit(GHitInfo &hit,gbool fillHit) 
{
    if (hits.Length() == 0) {
		if (fillHit) FillHitInfo(hit);

		if (selectNearestOnly) {
			// often fails with epsilon ASSERT(hit.t <= ray.tmax);
			ray.tmax = hit.t;  // limit ray to hit found  
		}

	    hits.Append(hit);
        return(1);
    }
    if (selectNearestOnly) {
        if (hit.t < hits[0].t) {    // new hit is nearer than current
			if (fillHit) FillHitInfo(hit);
			ASSERT(hit.t <= ray.tmax);
			ray.tmax = hit.t;  // limit ray to hit found  
            hits[0] = hit;
            return(1);
        }
        else return(0);
    }
	if (fillHit) FillHitInfo(hit);
    hits.Append(hit);
    return(1);
}

// get nearest object found, NULL if nothing has been hit
GHitInfo *GRaySelectTraversal::GetHit()

{ GHitInfo *minHit  = NULL;

  int nHits = hits.Length();
  if (nHits == 0) return(NULL);
  if (nHits == 1) return(&hits.Data()[0]);
  
  // Problem array is by value, hits[0] returns values, not a reference !!!!!!!!!!!!!!!
  // remove const ? 
  
  minHit = &hits.Data()[0];
  
  for(int i=1; i<nHits; i++)  {
    if (hits[i].t < minHit->t) minHit= &hits.Data()[i];
  }
  return(minHit);
}

void GRaySelectTraversal::FlushHits()
{
	hits.Flush();
}

/*
  check against Group nodes bboxSize bboxCenter bounding box 
*/

gbool GRaySelectTraversal::CullCheck(GvSFVec3f &bboxCenter,GvSFVec3f &bboxSize)
{
	if (bboxSize[0]>0) { 
		
	
	Point s = bboxSize * 0.5f; 
	BBox bbox(bboxCenter-s,bboxCenter+s);

	const Matrix &m = GetMatrixRef();

	if (mode >= MODE_RAPID) {
		// quick bbox bbox *m check 
		if (BoxBoxOutside(speedBoxVolume->bbox,m,bbox)) {
			//TRACE("Cull check BBOX outside \n");
			return 0;
		}
	}

	// Ray-Mode
	const Matrix &mI = GetMatrixInverse();

	// transform world ray to object coordinates

    RayRange tRay;

    tRay = ray;
    tRay *= mI;

   	float t1,t2;
    if (IntersectRayBox (tRay.p, tRay.d, &bbox.mi, &t1,&t2,tRay.tmin,tRay.tmax))

	{

        if (tRay.InRange(t1)) {        
			return gtrue;
        
		}

	}
	return gfalse;
	} 
	else  return gtrue; // empty bbox

}

gbool GRaySelectTraversal::CullCheck(BBox &bbox)
{
	if (!bbox.IsEmpty()) { 
	
	const Matrix &m = GetMatrixRef();

	if (mode >= MODE_RAPID) {
		// quick bbox bbox *m check 
		if (BoxBoxOutside(speedBoxVolume->bbox,m,bbox)) {
			//TRACE("Cull check BBOX outside \n");
			return 0;
		}
	}


	const Matrix &mI = GetMatrixInverse();


	// transform world ray to object coordinates
    RayRange tRay;

    tRay = ray;
    tRay *= mI;

   	float t1,t2;
    if (IntersectRayBox (tRay.p, tRay.d, &bbox.mi, &t1,&t2,tRay.tmin,tRay.tmax))

	{
        if (tRay.InRange(t1)) {        
			return gtrue;
        
		}

	}
	return gfalse;
	} else 
	return gtrue;

}




// check box for hit, if hit, store hit
int GRaySelectTraversal::CheckForHitByBox(GNode *n, BBox &box)
{
	const Matrix &m = GetMatrixRef();
	const Matrix &mI = GetMatrixInverse();



	// transform world ray to object coordinates

    RayRange tRay;

    tRay = ray;
    tRay *= mI;

   	float t1,t2;


#if 0

    
	TRACE("Transformed Ray (%f %f %f) (%f %f %f) (%f- %f)  \n",tRay.p.x,tRay.p.y,tRay.p.z,
                    tRay.d.x,tRay.d.y,tRay.d.z,
                    tRay.tmin,tRay.tmax);
	




    //TRACE("Ray (%f %f f) (%f %f %f )", ray.p.x,ray.p.y,ray.p.z, ray.d.x,ray.d.y,ray.d.z);
	Point p = mI * ray.p;
	///// Point d= mI* (ray.p+ray.d) -p;

/*	
	Point d= state.ray.d;
    Matrix use = Transpose(m);
    d = RotateOnly(use, d);
*/

	Point d= MultVector(mI,ray.d);
    TRACE("==> (%f %f %f) (%f %f %f ) \n",p.x,p.y,p.z,d.x,d.y,d.z);

	Point d1= RotateOnly(mI,ray.d);

    if (IntersectRayBox (p, d, &box.mi, &t1,&t2))
#endif

    if (IntersectRayBox (tRay.p, tRay.d, &box.mi, &t1,&t2,tRay.tmin,tRay.tmax))

	{

        if (tRay.InRange(t1)) {        
        
        Point pi = tRay.p + t1 *tRay.d;	// intersection point object
		
        Point piW = m * pi;		    // intersection point world
		float tiW = ray.Map(piW);   // intersection t world


        //ray.tmax = min(ray.tmax,tiW); // update range to nearest hit found  in root ray


        //TRACE("Hit ..%f %f in Range %d world t = %f \n",t1,t2,tRay.InRange(t1),tiW);

		
		GHitInfo *hit = new GHitInfo();
		hit->node = n;
		hit->t = tiW;
		hit->p =  pi;
		hit->pw =  piW;	  

		AppendHit(*hit,TRUE);

		delete hit;
		return(1);
        }
  	}
	return(0);
}


/*

  Check bbox for hit by current ray,

  Return Code >0, hit detected
    hit info updated with hit data (p, pw, m, bbox,  ...
    tRay contains global ray transformed to local coordinates

*/


int GRaySelectTraversal::CheckForHitByBox(GNode *n, BBox &box,GHitInfo &hit,RayRange &tRay)
{
	const Matrix &m = GetMatrixRef();
	const Matrix &mI = GetMatrixInverse();

    // transform world ray to object coordinates
    tRay = ray;
    tRay *= mI;

   	float t1,t2;

    if (IntersectRayBox (tRay.p, tRay.d, &box.mi, &t1,&t2,tRay.tmin,tRay.tmax))

	{
        if (tRay.InRange(t1)) {        
        Point pi = tRay.p + t1 *tRay.d;	// intersection point object
        Point piW = m * pi;		    // intersection point world
		float tiW = ray.Map(piW);   // intersection t world

        //ray.tmax = min(ray.tmax,tiW); // update range to nearest hit found  in root ray

        //TRACE("Hit ..%f %f in Range %d world t = %f \n",t1,t2,tRay.InRange(t1),tiW);

		
		hit.node = n;
		hit.t = tiW;
		hit.p =  pi;
		hit.pw =  piW;	  
		
        hit.m = m;
		hit.bbox= box;

		return(1);
        }
  	}
	return(0);
}



int GPolyline::Do(GRaySelectTraversal &state) // declared in GShell.h
{

   if (state.mode == GRaySelectTraversal::MODE_COLLISION) return(0);
   // currently only BBox check 
   return(state.CheckForHitByBox(this,GetBBox()));
}

int GPolylineI::Do(GRaySelectTraversal &state) // declared in GShellI.h
{

   if (state.mode == GRaySelectTraversal::MODE_COLLISION) return(0);
   // currently only BBox check 
   return(state.CheckForHitByBox(this,GetBBox()));
}
 
int GPoints::Do(GRaySelectTraversal &state) // declared in GShell.h
{

   if (state.mode == GRaySelectTraversal::MODE_COLLISION) return(0);
   // currently only BBox check 
   return(state.CheckForHitByBox(this,GetBBox()));
}

int GShell::Do(GRaySelectTraversal &state) // declared in GShell.h
{
	if (state.mode == GRaySelectTraversal::MODE_RAPID || state.mode == GRaySelectTraversal::MODE_RAPID_RECURSIVE)
	{

		// quick bbox bbox *m check 
		if (state.mode != GRaySelectTraversal::MODE_RAPID_RECURSIVE && BoxBoxOutside(state.speedBoxVolume->bbox,state.GetMatrixRef(),GetBBox())) {
			//TRACE("GShell::Do: Quick check outside \n");
			return 0;
		}


#if 0
		// coll test using convex volume
		//
		// transform speedBoxVolume to local space 
		GConvexVolume localVolume(state.GetMatrixInverse(),Transpose(state.GetMatrixRef()),state.speedBoxVolume->v,state.speedBoxVolume->planes);

		GHitInfo hitInfo;
		GCollisionResponse response;

		int status = CollideFaces(f,localVolume, response, hitInfo);

		if (status == OUTSIDE) return 0;


		TRACE("shape overlap  collision volume ...\n");
		state.collision = TRUE;
		state.collideShell = this;
		return 1;


#else

		if (subMeshes) { // do test on submeshes
			// & need v & fi information 
			if (actions & SHELL_COMPUTE_EXBUF) { 
				actions &= ~ (SHELL_COMPUTE_EXBUF|SHELL_UPDATE_EXBUF_VERT);	
				CreateSubMeshes();

			}
			else if (actions & SHELL_UPDATE_EXBUF_VERT) {
				actions &= ~ (SHELL_UPDATE_EXBUF_VERT);	
				UpdateSubMeshVertices();
			}
			GSubMesh *m=subMeshes;

			int ret = 0;
			while (m) {
				ret=m->s->Do(state);
				m = m->next;
			}
			return ret;

		} // if (subMeshes)


		// Rapid mode

		if (rapidModel == NULL || (actions  & SHELL_UPDATE_RAPID_MODEL))
			ComputeRapidModel();

		const Matrix &m = state.GetMatrixRef();
		Matrix inverse;

		int contents = m.contents | state.speedBoxMat.contents;


		if (contents & (Matrix::HAS_NON_UNIFORM_SCALE | Matrix::HAS_NEGATIVE_SCALE) )
		{
			GShell *tmpSpeedBox = state.speedBoxGlobal;

			Matrix volume2Local;
			inverse = state.GetMatrixInverse();

			volume2Local= state.speedBoxMat * inverse;
			tmpSpeedBox->SetV(state.speedBox->v);
			tmpSpeedBox->v.Mult(volume2Local);
			tmpSpeedBox->ComputeRapidModel();

			Matrix tmp;
			tmp.Identity();
			state.myRapid->Collide(tmp, (RAPID_model*)tmpSpeedBox->rapidModel, tmp, (RAPID_model*)rapidModel, state.myRapid->RAPID_contact_flag);
		}
		else if (contents & Matrix::HAS_SCALE)
			state.myRapid->CollideWithScale(state.speedBoxMat, (RAPID_model*)state.speedBox->rapidModel, m, (RAPID_model*)rapidModel, state.myRapid->RAPID_contact_flag);
		else
			state.myRapid->Collide(state.speedBoxMat, (RAPID_model*)state.speedBox->rapidModel, m, (RAPID_model*)rapidModel, state.myRapid->RAPID_contact_flag);

		state.colCyclesPerFrame++;
		if (state.myRapid->RAPID_num_box_tests > 1)
			TRACE("GShell::Do: Num box tests: %d\n", state.myRapid->RAPID_num_box_tests);
	
			
		//if hit adapt speedbox	
		if (state.myRapid->RAPID_num_contacts > 0) {
			//CD TRACE("GShell::Do: Num contact pairs: %d\n", state.myRapid->RAPID_num_contacts);

			Point lastDiff, diff;
			lastDiff = diff = (state.speedBox->v[0] -state.speedBox->v[4])/2;
			int counter, fullContact;
			counter = fullContact = 0;
		
			//compute a smaller speedbox and call only the hit-shell
			while (counter<10)
			{
				//
				//compute a smaller speedbox
				//
				//CD TRACE("diff      %f %f %f\n", diff.x, diff.y, diff.z);
				counter++;
				state.speedBox->v[0] = state.speedBox->v[4] + diff;
				state.speedBox->v[1] = state.speedBox->v[5] + diff;
				state.speedBox->v[2] = state.speedBox->v[6] + diff;
				state.speedBox->v[3] = state.speedBox->v[7] + diff;
			
				state.speedBox->ComputeRapidModel();
			
			
				
				//
				//do the rapid collision test
				//
				if (contents & (Matrix::HAS_NON_UNIFORM_SCALE | Matrix::HAS_NEGATIVE_SCALE) )
				{	
					//we can not handle nonunfiorm scale
					//trick at the penalty of cpu cost
					//we transform the speedboxes into the local coord sys
					Matrix volume2Local;
					GShell *tmpSpeedBox = state.speedBoxGlobal;
					volume2Local= state.speedBoxMat * inverse;
					tmpSpeedBox->SetV(state.speedBox->v);
					tmpSpeedBox->v.Mult(volume2Local);
					tmpSpeedBox->ComputeRapidModel();
				
					Matrix tmp;
					tmp.Identity();
					state.myRapid->Collide(tmp, (RAPID_model*)tmpSpeedBox->rapidModel, tmp, (RAPID_model*)rapidModel, state.myRapid->RAPID_contact_flag);
				}
				else if (contents & Matrix::HAS_SCALE)
					state.myRapid->CollideWithScale(state.speedBoxMat, (RAPID_model*)state.speedBox->rapidModel, m, (RAPID_model*)rapidModel, state.myRapid->RAPID_contact_flag);
				else
					state.myRapid->Collide(state.speedBoxMat, (RAPID_model*)state.speedBox->rapidModel, m, (RAPID_model*)rapidModel, state.myRapid->RAPID_contact_flag);
			
			
				//
				//results
				//
				lastDiff /= 2.f;
			
				
				
				if (state.myRapid->RAPID_num_contacts > 0) 
				//we have a hit, so we decrease the size of the speedbox
				{
					//CD TRACE("GShell::Do: Num contact pairs: %d\n", state.myRapid->RAPID_num_contacts);
					//CD TRACE("GShell::Do: hit, we decrease the size of the speedbox\n");
					diff -= lastDiff;
					fullContact += 1;
				}
				else
				{
					//we had no hit, so we increase the size of the speedbox
					TRACE("GShell::Do: no hit, increase the size of the speedbox\n");
					diff += lastDiff;
				}
			}//end while

			
			
			if (fullContact == 10) 
				//if i am in front of a wall, diff has to be 0 exactly
				//otherwise I go through the wall after infinite number of cycles
			{	
				diff.x = diff.y = diff.z = 0.f;
			}
			// is rapid accurate?
			// if not we multiply for safety reasons
			//else 
			//	diff *= .5;//test testtesttesttest
		

			if (state.hits.Length() != 0) 
				state.hits.Flush();

			GHitInfo hit;
			hit.m = m;
			hit.node = this;
			hit.mInverse = inverse;
			hit.t = diff.Length(); //this is a dummy-value, rapid approximates to collision-distance
			hit.p.Set(diff);
			state.AppendHit(hit,TRUE);

			TRACE("GShell::Do:   hit.p    %f %f %f\n", hit.p.x, hit.p.y, hit.p.z);

		}//end adapt speedbox

		state.collision = TRUE;

#endif
		return 0;
	} // RAPID MODE 

    if (state.method == GRaySelectTraversal::BY_BBOX) { // only BBox check
        return(state.CheckForHitByBox(this,GetBBox()));
    }
    else {
        GHitInfo hit;
        RayRange ray;

		if (GetFaceCount() <= 2 && f.Length() < 12) { // only few face, no bbox check 2.04.97

			GMatrixStack *m=state.GetCurrentMatrix(); 
	
			const Matrix &mI = m->GetMatrixInverse(); //state.GetMatrixInverse();

			// transform world ray to object coordinates
			ray = state.ray;
			ray *= mI;
			hit.node = this;

	        hit.m = state.GetMatrixRef();
			//hit.bbox= GetBBox();

			return (IntersectFaces(state,f,ray,hit));    

		}
        else {


			// BBox check first
			if (state.CheckForHitByBox(this, GetBBox(),hit,ray)>0) {

				if (subMeshes) { // new 24.09.98 do test on submeshes
					// & need v & fi information 
				if (actions & SHELL_COMPUTE_EXBUF) { 
					actions &= ~ (SHELL_COMPUTE_EXBUF|SHELL_UPDATE_EXBUF_VERT);	
					CreateSubMeshes();

				}
				else if (actions & SHELL_UPDATE_EXBUF_VERT) {
					actions &= ~ (SHELL_UPDATE_EXBUF_VERT);	
					UpdateSubMeshVertices();
				}
				GSubMesh *m=subMeshes;

				int ret = 0;
				while (m) {
					// ret=m->s->Do(state);
					if (state.CheckForHitByBox(this, m->s->GetBBox(),hit,ray)>0)
						ret = m->s->IntersectFaces(state,m->s->f,ray,hit); 
					m = m->next;
				}
				return ret;

				} // if (subMeshes)

				return (IntersectFaces(state,f,ray,hit));    
			} 
		}
		
		return(0);
    }
}



//GNodeShell::Do
// set owner


int GSphereSubdiv::Do(GRaySelectTraversal &state) // declared in GGeom.h
{
	return(GShell::Do(state));
}

#ifdef _G_VRML1

int GvIndexedLineSet::Do(GRaySelectTraversal &state)
{
	return(0); // not supported yet 
	if (!theShell) BuildShell(state);

	if (!theShell) return (0);

	// apply shape hints 
    state.SetCulling(FALSE,FALSE);

	// apply traversal 
	return theShell->Traverse(state);

}
#endif _G_VRML1

int Gv2IndexedLineSet::Do(GRaySelectTraversal &state)
{
	return(0); // not supported yet 

	if (!theShell) 	BuildShell(state); 

	if (!theShell) return (0);

	// apply shape hints 
    state.SetCulling(FALSE,FALSE);

	// apply traversal 

	int ret;
	GPUSH_PATH(state,this);
		ret = theShell->Traverse(state);
	GPOP_PATH(state);
	return ret;

}

int Gv2PointSet::Do(GRaySelectTraversal &state)
{

	return(0); // not supported yet 

	if (!theShell) BuildShell(state);

	if (!theShell) return (0);

	return(0); // for now 

	// apply shape hints 
    state.SetCulling(FALSE,FALSE);

	// apply traversal 

	int ret;
	GPUSH_PATH(state,this);
		ret = theShell->Traverse(state);
	GPOP_PATH(state);
	return ret;

}


int Gv2Sphere::Do(GRaySelectTraversal &state) 
{   int ret = 0;

	if (state.mode == GRaySelectTraversal::MODE_RAPID) {

		// apply shape hints 
		state.SetCulling(Solid(),gfalse);

		// we need the shell, even if not rendered eg for Collision.proxy field
		if (!theShell) 	BuildShell(state); 

		if (!theShell) return (0);
		// apply traversal 
		GPUSH_PATH(state,this);
			ret = theShell->Traverse(state);
		GPOP_PATH(state);
		return ret;

	} 


/*
    if (state.method == GRaySelectTraversal::BY_BBOX) { // only BBox check
        return(state.CheckForHitByBox(this,GetBBox()));
    }
    else 

*/

	// apply shape hints 
    state.SetCulling(Solid(),gfalse);
	GPUSH_PATH(state,this);


	{
		RayRange tRay;

		const Matrix &m = state.GetMatrixRef();
		const Matrix &mI = state.GetMatrixInverse();

		// transform world ray to object coordinates
		tRay = state.ray;
		tRay *= mI;

   		float t1;
		Point center(0,0,0);

		if (IntersectRaySphere(tRay.p, tRay.d,center, radius.value, t1)) {
			if (tRay.InRange(t1)) { 
				Point pi = tRay.p + t1 *tRay.d;	// intersection point object
				Point piW = m * pi;		    // intersection point world
				float tiW = state.ray.Map(piW);   // intersection t world

				GHitInfo hit;
	 		    hit.node = this;
				hit.t = tiW;
				hit.p =  pi;
				hit.pw =  piW;	  
				hit.m = m;

			//	hit.bbox= box;
		
				state.AppendHit(hit,TRUE);
				ret = 1;
		
			}

		}
    }
	GPOP_PATH(state);

	return(ret);
}

#ifdef _G_VRML1

// to do: redo camera computation
// LOD could be instanced multiple times
int GvLOD::Do(GRaySelectTraversal &state)
{
	int ret;
	if (state.traverseAll) { return(GvGroup::Do(state)); }

    int nChilds = getNumChildren();
    if (nChilds == 0) return(0);

    int child = lastRenderedChild;
    if (lastRenderedChild >= nChilds) child = nChilds -1;

	GPUSH_PATH(state,this);
		GSET_PATH_INDEX(state,child);
		ret = getChild(child)->Traverse(state);
	GPOP_PATH(state);
	return ret;

}
#endif _G_VRML1

int Gv2LOD::Do(GPathTraversal &state)
{	int ret = 0;
	GPUSH_PATH(state,this);
		if (state.traverseAll) {
   			ret = level.Traverse(state);
		}
		else { 
			int nChilds = level.Length();
			if (nChilds > 0) {
				int child = lastRenderedChild; //Bad 

		        if (child >= nChilds) child = nChilds -1;
				if (child >=0 && child <nChilds) {
					GSET_PATH_FIELD(state,level);
					GSET_PATH_INDEX(state,child);
					ret = level[child]->Traverse(state);
				}
			}
		}
	GPOP_PATH(state);
	return(ret);
}





#ifdef _G_VRML1
int GvWWWAnchor::Do(GRaySelectTraversal &state) 
{
	return Do(* ((GPathTraversal *) &state));
}
#endif _G_VRML1


// need to overwrite because of Gv2Group::
int Gv2Anchor:: Do(GRaySelectTraversal &state)
{	int ret = 0;
	if (!state.CullCheck(bboxCenter,bboxSize)) return GV_NODE_CULLED;

    state.Push();

    // put Anchor node in state to resolve anchor picks (deepest first)
	if (state.mode == GRaySelectTraversal::MODE_SELECTION) {
		GvWWWAnchorElement *current,*prev;
		prev = (GvWWWAnchorElement*) state.Get(GvWWWAnchorElement::attrId);
		if (prev) {
			current = (GvWWWAnchorElement *) prev->NewInstance();								
			current->data = this;
			state.Push(GvWWWAnchorElement::attrId,current);	
		
			if (!homeUrlInit) { // get the URL of current scene / inline 
				homeUrl = state.GetHomeUrl();
				homeUrlInit = TRUE;
			}
		}
	
		GPUSH_PATH(state,this);

			// put all touchsensors in state
			ret = ApplyGeometrySensors(state);
    		ret = TraverseChildren(state);

		GPOP_PATH(state);

	} else {
		GPUSH_PATH(state,this);
    		ret = TraverseChildren(state);
		GPOP_PATH(state);
	}

	state.Pop();
	return(ret);
}





#ifdef _G_VRML1

// select a non loaded WWWInline 
int GvWWWInline::Do(GRaySelectTraversal &state)
{

  if (loader.Ok()) { 
  	//return GvGroup::Do(state);
	return Do(*((GTraversal *) &state));

  }

  if (bboxIsEmpty()) return(0);

  BBox box;
  Point s = bboxSize * 0.5;
  box.mi = bboxCenter-s;
  box.ma = bboxCenter+s;
 
  // hit box ?
  return(state.CheckForHitByBox(this,box));
}

#endif

int Gv2Collision::Do(GRaySelectTraversal &state)
{	int ret = 0;

	if (!state.CullCheck(bboxCenter,bboxSize))
		return GV_NODE_CULLED;


    state.Push();
	GPUSH_PATH(state,this);

	int bboxOk = 1;
	// if ((bboxSize[0]>=0.0)) bboxOk = state.TestForHitByBox(this, bboxCenter,bboxSize)>0 ; 

	if (bboxOk) {

	if (state.mode >= GRaySelectTraversal::MODE_COLLISION) {

		if (!collide) 
			ret = 0; // check collide field 
		else {
			// push collision node as WWWAnchor Element 
			if (collideTime.connections != NULL) { //hasOutRoutes()) 
				GvWWWAnchorElement *current,*prev;
				prev = (GvWWWAnchorElement*) state.Get(GvWWWAnchorElement::attrId);
				if (prev) {
					current = (GvWWWAnchorElement *) prev->NewInstance();								
					current->data = this;
					state.Push(GvWWWAnchorElement::attrId,current);	
				}
			}

			if (proxy != (GvNode*) NULL)
				ret = proxy.Traverse(state);
			else 
			   ret = TraverseChildren(state);
			// pop collision node 
		}
	}
    else {

		if (state.mode == GRaySelectTraversal::MODE_SELECTION)
			ret = ApplyGeometrySensors(state);

		ret = TraverseChildren(state);
	}
	
	} // bboxOk

	GPOP_PATH(state);
	state.Pop();
	return(ret);
}

int Gv2Selection::Do(GRaySelectTraversal &state)
{	int ret = 0;

    state.Push();
	GPUSH_PATH(state,this);

	if (state.mode == GRaySelectTraversal::MODE_COLLISION) {

		if (!collide) ret = 0; // check collide field 
		else {
			if (!state.CullCheck(bboxCenter,bboxSize)) return GV_NODE_CULLED;
			if (proxy != (GvNode*) NULL)
				ret = proxy.Traverse(state);
			else 
			   ret = TraverseChildren(state);
		}
	}
    else 
	if (state.mode == GRaySelectTraversal::MODE_SELECTION) {
		

		if (!select) ret = 0; // check collide field 
		else {
			if (!state.CullCheck(bboxCenter,bboxSize)) return GV_NODE_CULLED;

			if (proxy != (GvNode*) NULL)
				ret = proxy.Traverse(state);
			else 
			   ret = TraverseChildren(state);
		}

	} else {
		if (!state.CullCheck(bboxCenter,bboxSize)) return GV_NODE_CULLED;
		ret = TraverseChildren(state);
	}
	

	GPOP_PATH(state);
	state.Pop();
	return(ret);
}




// twice the signed area of triangle a,b,c
// positive if a,b,c ccw, negative if cw
//
float area2(float ax,float ay, float bx,float by, float cx,float cy)
{
	return	ax * by - ay * bx +
			ay * cx - ax * cy +
			bx * cy - cx * by;

}

// is point c on the left of line a,b ?
BOOL LeftOf(float ax,float ay, float bx,float by, float cx,float cy)
{
  return(area2(ax,ay,bx,by,cx,cy) >0);
}

// is point c on the left of line a,b ?
BOOL LeftOn(float ax,float ay, float bx,float by, float cx,float cy)
{
  return(area2(ax,ay,bx,by,cx,cy)>=0);
}

BOOL Collinear(float ax,float ay, float bx,float by, float cx,float cy)
{
  return(area2(ax,ay,bx,by,cx,cy)==0); // <EPS 
}


int IntersectTriangle(const Point &p0,const Point &p1,const Point &p2,  const Point &mNormal,
        RayRange& pRay, GHitInfo &info) 
{
    int _HIT = 0;
 
    
    float tT, tDot;

    //// Determine the dominant axis of the normal

    int mDominantAxis = mNormal.Dominant();

    int X_AXIS,Y_AXIS;

    mNormal.ProjectIndex(X_AXIS,Y_AXIS, mDominantAxis);

   
    // intersect w/ the plane
    //    if (!mValid)
    ///      goto L_EXIT;

    tDot = DotProd(mNormal,pRay.d);

    float mDist = DotProd(mNormal,p0);  // Parameter D of plane equetation

    if (fabs(tDot) < EPSILON) 
      return(0);

    
    // compute the t-parameter
    tT = (mDist - DotProd(mNormal,pRay.p)) / tDot;

    if(!pRay.InRange(tT)) 
      return(0);
    
    // project the I-point along the dominant axis
    float tU, tV;
    pRay.Eval(tT).Project(tU, tV, mDominantAxis);

    // is this point inside the bounding rechtangle ?
    //if (tU < mUmin || tU > mUmax || tV < mVmin || tV > mVmax)
    //  goto L_EXIT;


    float A,B,C;
   
    // P0-P1
    A = p0[Y_AXIS] - p1[Y_AXIS] ;
    B = p1[X_AXIS] - p0[X_AXIS] ;

    C = A * p0[X_AXIS]  + B * p0[Y_AXIS] ;

	// 

#if 1
	// hg should work irrespective off handedness of triangle 
    int  s1 = ( A * tU + B * tV < C ); 

	{

        A = p1[Y_AXIS] - p2[Y_AXIS] ;
        B = p2[X_AXIS] - p1[X_AXIS] ;
        C = A * p1[X_AXIS]  + B * p1[Y_AXIS] ;


        int s2 = ( A * tU + B * tV < C ); 
		
		if (s1 != s2) return(0);
		
		{
            A = p2[Y_AXIS] - p0[Y_AXIS] ;
            B = p0[X_AXIS] - p2[X_AXIS] ;
            C = A * p2[X_AXIS] + B * p2[Y_AXIS] ;

	        int s3 = ( A * tU + B * tV <= C );

			if (s3 == s2)
			{
                info.t = tT;
		        /////////rInter.add( GInterSection( self, tT, (tDot < 0.0)));
		        _HIT = 1 ;
	        }
	    } 
    } 

#else
    if ( A * tU + B * tV < C ) {

        A = p1[Y_AXIS] - p2[Y_AXIS] ;
        B = p2[X_AXIS] - p1[X_AXIS] ;
        C = A * p1[X_AXIS]  + B * p1[Y_AXIS] ;


        if ( A * tU + B * tV < C ) {
            A = p2[Y_AXIS] - p0[Y_AXIS] ;
            B = p0[X_AXIS] - p2[X_AXIS] ;
            C = A * p2[X_AXIS] + B * p2[Y_AXIS] ;

	        if ( A * tU + B * tV <= C ) {
                info.t = tT;
		        /////////rInter.add( GInterSection( self, tT, (tDot < 0.0)));
		        _HIT = 1 ;
	        }
	    } 
    } 
#endif
  

//    L_EXIT:
    
    return (_HIT);

}


int IntersectTriangleBary(const Point &p0,const Point &p1,const Point &p2,  const Point &mNormal,
        RayRange& pRay, GHitInfo &info) 
{

    float tT, tDot;

    // Determine the dominant axis of the normal

    int mDominantAxis = mNormal.Dominant();
    int X,Y;

    mNormal.ProjectIndex(X,Y, mDominantAxis);

   
    // intersect with  the plane

    tDot = DotProd(mNormal,pRay.d);


    if (fabs(tDot) < EPSILON) 
		return(0);

    float mDist = DotProd(mNormal,p0);  // Parameter -D of plane equtation
    
    // compute the t-parameter
    tT = (mDist - DotProd(mNormal,pRay.p)) / tDot;


    if(!pRay.InRange(tT)) 
      return(0);
    
    // get intersection point 
    Point p = pRay.Eval(tT);
	

    // is this point inside the bounding rechtangle ?
    //if (tU < mUmin || tU > mUmax || tV < mVmin || tV > mVmax)
    //  return(0);

	float u0 = p[X] - p0[X];
	float v0 = p[Y] - p0[Y];
	double u1,v1;
	double u2,v2;

	u1 = p1[X] - p0[X]; 	v1 = p1[Y] - p0[Y];

	u2 = p2[X] - p0[X]; 	v2 = p2[Y] - p0[Y];

	double alpha,beta;

	if (u1 == 0.0) {
		beta = u0/u2; 
		if ( (beta>=0.0) &&  (beta <= 1.0)) {
			alpha = (v0 - beta *v2) / v1;
			if ((alpha>=0.0) && ((alpha+beta) <= 1.0)) {
                info.t = tT;
				info.p = p;

				info.tu = alpha;
				info.tv = beta;
				return(1);
			}

		}
	}
	else  {
		beta = (v0*u1 - u0*v1) / (v2*u1 - u2*v1);
		if (beta>=0.0 && beta <= 1.0) {
			alpha = (u0 - beta *u2) / u1;
			if ((alpha>=0.0) && ((alpha+beta) <= 1.0)) {
                info.t = tT;
				info.p = p;
				info.tu = alpha;
				info.tv = beta;
				return(1);
			}
		}
	}

    return (0);
}


#define TEPS 0.01
#define BEPS_ZERO (-TEPS)
#define BEPS_ONE (1.0+TEPS)


int PointInTriangleBary(int X,int Y, const Point &p, const Point &p0,const Point &p1,const Point &p2,  
        RayRange& pRay, GHitInfo &info) 
{

	

    // is this point inside the bounding rechtangle ?
    //if (tU < mUmin || tU > mUmax || tV < mVmin || tV > mVmax)
    //  return(0);

	float u0 = p[X] - p0[X];
	float v0 = p[Y] - p0[Y];
	double u1,v1;
	double u2,v2;

	u1 = p1[X] - p0[X]; 	v1 = p1[Y] - p0[Y];
	u2 = p2[X] - p0[X]; 	v2 = p2[Y] - p0[Y];

	double alpha,beta;

	if (u1 == 0.0) {

		if (u2 == 0.0) {
			// hg new mm degenerate triangle ??????
			return 0;

		}
		beta = u0/u2; 
		if ( (beta>=BEPS_ZERO) &&  (beta <= BEPS_ONE)) {
			alpha = (v0 - beta *v2) / v1;
			if ((alpha>=BEPS_ZERO) && ((alpha+beta) <= BEPS_ONE)) {

				double gamma = 1.0 - (alpha + beta);
				info.p = p0 * gamma;
				info.p += p1 * alpha;
				info.p += p2 * beta;

				info.tu = alpha;
				info.tv = beta;
				return(1);
			}

		}
	}
	else  {
		beta = (v0*u1 - u0*v1) / (v2*u1 - u2*v1);
		if (beta>=BEPS_ZERO && beta <= BEPS_ONE) {
			alpha = (u0 - beta *u2) / u1;
			if ((alpha>=BEPS_ZERO) && ((alpha+beta) <= BEPS_ONE)) {
				// interpolate params 
				double gamma = 1.0 - (alpha + beta);
				info.p = p0 * gamma;
				info.p += p1 * alpha;
				info.p += p2 * beta;

				// info.p = p;
				info.tu = alpha;
				info.tv = beta;
				return(1);
			}
		}
	}

    return (0);
}



int
GShell::IntersectFaces(GRaySelectTraversal &state, const GFaceList &f,RayRange &ray, GHitInfo &hitInfo)
{
   const int *fp =f;			   // face list stepping pointers
   const int *fpend= fp+f.Length();
   int fi=0;	// face index

#ifdef _DEBUG
    int planetest=0;
	int raytest=0;
    int tritest = 0;
	int culltest =0;
	int bboxtest = 0;

#endif
   
   if (f.Length() <= 0) return(0);

   if (nfaces <=0) {
		nfaces= CountFaces();
   }
   if (nfaces <=0) return(0);

   // need face normals for computation
   const PointArray &fnormals = GetFN();
   if (fnormals.Length() == 0) return(-1);

   int intersections=0;

 
   fi = -1;
   do { 		// for all faces
		 int n = *fp ++; // length of this face
		 int ishole=0;
         int inHole = 0;
		 Point p;

		 if (n<0) { ishole=1; n=-n; 
            TRACE("Hole selection not supported !\n");

         }
		 else fi++;

		 do { // dummy loop for break
			if (n>2) {
                const Point &fn = fnormals[fi]; // face normal

				// intersect ray with the polygon plane
			    float tDot = DotProd(fn,ray.d);

				#ifdef _DEBUG
					planetest++;
				#endif 

				if (fabs(tDot) < EPSILON) 
					break;

				int p1 = *fp++;
				int p2 = *fp++;
				n-=2;


				float mDist = DotProd(fn,v[p1]);  // Parameter -D of plane equtation
    
				// compute the t-parameter
				float tT = (mDist - DotProd(fn,ray.p)) / tDot;

				#ifdef _DEBUG
					raytest++;
				#endif 

				if(!ray.InRange(tT)) 
					break;

    
				// get intersection point 
				// Point p = ray.Eval(tT); VC does not expand this to inline 

				p.x = ray.p.x + ray.d.x * tT,
				p.y = ray.p.y + ray.d.y * tT,
				p.z = ray.p.z + ray.d.z * tT;

				// to check : bbox test ????

				// Determine the dominant axis of the normal
				int mDominantAxis;
				int X,Y;
				// mDominantAxis = fn.Dominant();
				//fn.ProjectIndex(X,Y, mDominantAxis);

				// manual inline, VC does not Inline it properly
				mDominantAxis=0;
				X = 1; Y = 2;
				float tMax = fabs(fn.x);
				if (fabs(fn.y) > tMax) { 
					tMax = fabs(fn.y);	  
					mDominantAxis = 1;
					X = 0; Y = 2;
				}
				if (fabs(fn.z) > tMax)	{
					mDominantAxis = 2;	
					X = 0; Y = 1;					
				}	



				while (n>0) {
				    int p3 = *fp++;
				    n--;

				    //TRACE("IN %d %d  %d (Face %d) \n",p1,p2,p3,fi);
                    //if (IntersectTriangleBary(v[p1],v[p2],v[p3],fn,ray,hitInfo))
					#ifdef _DEBUG
						tritest++;
					#endif 

					if (state.cw) {
						int x=p2;
						p2=p3;
						p3=x;
					}
                    
					if (PointInTriangleBary(X,Y,p,v[p1],v[p2],v[p3],ray,hitInfo))
                    {
						hitInfo.t = tT;

						// culling check 
						if (state.solid && state.mode == GRaySelectTraversal::MODE_SELECTION) {
							#ifdef _DEBUG
								culltest++;
							#endif 

							Point view = state.GetMatrixInverse() * state.physicalCamera->position;
							view-=p;
							view.Normalize();

							float dot = DotProd(view,fn);
							
							if (NormalsFlipped()) { 
							
								if (dot <  0.0) break;

							} else {

								if (!!state.cw) {
									if (dot >0.0) break;
								} else
									if (dot <0.0) break;
							}
						}


                        hitInfo.findex = fi;
                        hitInfo.tvindex0 = p1;
                        hitInfo.tvindex1 = p2;
                        hitInfo.tvindex2 = p3;


						if (state.storeFullHitInfo) {

							// interpolate params 
							float gamma = 1.0 - (hitInfo.tu + hitInfo.tv);

							if (vn.Length() >0) {
								hitInfo.vn = vn[p1] * gamma;
								hitInfo.vn += vn[p2] * hitInfo.tu;
								hitInfo.vn += vn[p3] * hitInfo.tv;
							}
							else hitInfo.vn = fn; // .Zero();

							if (vp.Length() >0) {
								hitInfo.vp = vp[p1] * gamma;
								hitInfo.vp += vp[p2] * hitInfo.tu;
								hitInfo.vp += vp[p3] * hitInfo.tv;
							}
							else hitInfo.vp = hitInfo.p;

							if (vc.Length() >0) {
								hitInfo.vc = vc[p1] * gamma;
								hitInfo.vc += vc[p2] * hitInfo.tu;
								hitInfo.vc += vc[p3] * hitInfo.tv;
							}
							else hitInfo.vc.x = -1.0;

						}

                        //TRACE("Triangle hit %d %f %f %f \n",hitInfo.findex,hitInfo.pw.x,hitInfo.pw.y,hitInfo.pw.z);

                        if (ishole) { inHole++; }
                        else {
							if (state.selectNearestOnly) {
								ray.tmax = hitInfo.t;  // limit transformed ray to hit found  
							}
							// hitInfo.p = ray.Eval(hitInfo.t); // already computed
							hitInfo.pw = hitInfo.m * hitInfo.p;	

							hitInfo.t = state.ray.Map(hitInfo.pw); // intersection t in world

							intersections ++;

							state.AppendHit(hitInfo,TRUE);

							break; // dont test this polygon further 
						}

                    }
					if (!state.cw) 
						p2=p3;

		    } // while (n>0)
			} // n>2
        } while(0);
		
		fp+=n;
   }
   while (fp < fpend);

#ifdef _DEBUG
   //TRACE(" intersections:%d planetest %d raytest %d tritest %d culltest %d  t = %g\n",
   //	       intersections,planetest,raytest,tritest,culltest,hitInfo.t);
#endif

   
   return(intersections);
 //  if (NormalsFlipped()) fn *= Point(-1.0,-1.0,-1.0);  // flip normals
}

int
GMesh::IntersectFaces(GRaySelectTraversal &state, const GFaceList &f,RayRange &ray, GHitInfo &hitInfo)
{
	if (f.Length() == 0)
		ComputeFaceList();
	return GShell::IntersectFaces(state,f,ray,hitInfo);
}


int
GShellI::IntersectFaces(GRaySelectTraversal &state, const GFaceList &f, RayRange &ray, GHitInfo &hitInfo)
{
   const int *fp =f;			   // face list stepping pointers
   const int *fpend= fp+f.Length();
   int fi=0;	// face index
   
   if (f.Length() <= 0) return(0);

   if (nfaces <=0) {
		nfaces= CountFaces();
   }
   if (nfaces <=0) return(0);

   // need face normals for computation
   const PointArray &fnormals = GetFN();
   if (fnormals.Length() == 0) return(-1);

   int intersections=0;

   //KatmaiPrepareIntersect(fn,fv, ray.p,ray.d)
 
   fi = -1;
   do { 		// for all faces
		 int n = *fp ++; // length of this face
		 int ishole=0;
         int inHole = 0;
		 Point p;
		 
		 if (n<0) { ishole=1; n=-n; 
            TRACE("Hole selection not supported !\n");

         }
		 else fi++;

		 do { // dummy loop for break
			if (n>2) {
                const Point &fn = fnormals[fi]; // face normal

				// intersect ray with the polygon plane
			    float tDot = DotProd(fn,ray.d);

				if (fabs(tDot) < EPSILON) 
					break;

				int p1 = *fp++;
				int p2 = *fp++;
				n-=2;

				int p1v=verts[p1].v;
				int p2v=verts[p2].v;

				float mDist = DotProd(fn,v[p1v]);  // Parameter -D of plane equtation
    
				// compute the t-parameter
				float tT = (mDist - DotProd(fn,ray.p)) / tDot;

				if(!ray.InRange(tT)) 
					break;
    
				// get intersection point 
				// Point p = ray.Eval(tT); VC does not expand this to inline 

				p.x = ray.p.x + ray.d.x * tT,
				p.y = ray.p.y + ray.d.y * tT,
				p.z = ray.p.z + ray.d.z * tT;

				// Determine the dominant axis of the normal
				int mDominantAxis;
				int X,Y;
				// mDominantAxis = fn.Dominant();
				//fn.ProjectIndex(X,Y, mDominantAxis);

				// manual inline, VC does not Inline it properly
				mDominantAxis=0;
				X = 1; Y = 2;
				float tMax = fabs(fn.x);
				if (fabs(fn.y) > tMax) { 
					tMax = fabs(fn.y);	  
					mDominantAxis = 1;
					X = 0; Y = 2;
				}
				if (fabs(fn.z) > tMax)	{
					mDominantAxis = 2;	
					X = 0; Y = 1;					
				}	


				while (n>0) {
				    int p3 = *fp++;
					int p3v=verts[p3].v;

				    n--;

				    //TRACE("IN %d %d  %d (Face %d) \n",p1,p2,p3,fi);
                    //if (IntersectTriangleBary(v[p1],v[p2],v[p3],fn,ray,hitInfo))
                    
					if (PointInTriangleBary(X,Y,p,v[p1v],v[p2v],v[p3v],ray,hitInfo))
                    {
						hitInfo.t = tT;

						// culling check 
						if (state.solid && state.mode == GRaySelectTraversal::MODE_SELECTION) {
							Point view = state.GetMatrixInverse() * state.physicalCamera->position;
							view-=p;
							view.Normalize();
							float dot = DotProd(view,fn);

							if (NormalsFlipped()) { // 02.04.97
								if (dot > 0.0) break;
							} else {
								if (!!state.cw) {
									if (dot >0.0) break;
								} else
									if (dot <0.0) break;
								}
							}


                        hitInfo.findex = fi;
                        hitInfo.tvindex0 = p1;
                        hitInfo.tvindex1 = p2;
                        hitInfo.tvindex2 = p3;


						if (state.storeFullHitInfo) {

							// interpolate params 
							float gamma = 1.0 - (hitInfo.tu + hitInfo.tv);

							if (vn.Length() >0) {
								hitInfo.vn = vn[verts[p1].n] * gamma;
								hitInfo.vn += vn[verts[p2].n] * hitInfo.tu;
								hitInfo.vn += vn[verts[p3].n] * hitInfo.tv;
							}
							else hitInfo.vn = fn; // .Zero();

							if (vp.Length() >0) {
								hitInfo.vp = vp[verts[p1].p] * gamma;
								hitInfo.vp += vp[verts[p2].p] * hitInfo.tu;
								hitInfo.vp += vp[verts[p3].p] * hitInfo.tv;
							}
							else hitInfo.vp = hitInfo.p;

							if (vc.Length() >0) {
								hitInfo.vc = vc[verts[p1].c] * gamma;
								hitInfo.vc += vc[verts[p2].c] * hitInfo.tu;
								hitInfo.vc += vc[verts[p3].c] * hitInfo.tv;
							}
							else hitInfo.vc.x = -1.0;

						}

                        //TRACE("Triangle hit %d %f %f %f \n",hitInfo.findex,hitInfo.pw.x,hitInfo.pw.y,hitInfo.pw.z);

                        if (ishole) { inHole++; }
                        else {
							if (state.selectNearestOnly) {
								ASSERT(hitInfo.t <= ray.tmax);
								ray.tmax = hitInfo.t;  // limit transformed ray to hit found  
							}
							// hitInfo.p = ray.Eval(hitInfo.t); // alreay computed
							hitInfo.pw = hitInfo.m * hitInfo.p;	

							hitInfo.t = state.ray.Map(hitInfo.pw); // intersection t in world

							state.AppendHit(hitInfo,TRUE);

							intersections ++;

							break; // dont test this polygon further 
						}

                    }
				    p2=p3;
					p2v=p3v;
		    } // while (n>0)
			} // n>2
        } while(0);
		
		fp+=n;
   }
   while (fp < fpend);
   
   return(intersections);
 //  if (NormalsFlipped()) fn *= Point(-1.0,-1.0,-1.0);  // flip normals
}



#if 0


// Transform a point by a matrix.

float *
Transform(const Point& v,const Matrix& M, float ret[4] )
{
    Point ret;

    ret[0] = v.x * M.x[0][0] + v.y * M.x[1][0] + v.z * M.x[2][0] + M.x[3][0];
    ret[1] = v.x * M.x[0][1] + v.y * M.x[1][1] + v.z * M.x[2][1] + M.x[3][1];
    ret[2] = v.x * M.x[0][2] + v.y * M.x[1][2] + v.z * M.x[2][2] + M.x[3][2];
    ret[3] = M.x[0][3] + M.x[1][3] + M.x[2][3] + M.x[3][3];
    return ret;
}


//
// poly_clip_to_box: Clip the convex polygon p1 to the screen space box
// using the homogeneous screen coordinates (sx, sy, sz, sw) of each
// vertex, testing if v[0]/v[3] > box.mi.x and v[0]/v[3] < box.ma.x,
// and similar tests for y and z, for each vertex v of the polygon.
// If polygon is entirely inside box, then POLY_CLIP_IN is returned.
// If polygon is entirely outside box, then POLY_CLIP_OUT is returned.
// Otherwise, if the polygon is cut by the box, p1 is modified and
// POLY_CLIP_PARTIAL is returned.
//


#define STATUS_INSIDE 1
#define STATUS_UNKNOWN 0
#define STATUS_OUTSIDE -1



static int
PointVolumeStatus(int np,const Point *p, Matrix &object2clip, const BBox clipBox)
{
   int x0out = 0, x1out = 0;
   int y0out = 0, y1out = 0;
   int z0out = 0, z1out = 0;
   int i;

   // count vertices "outside" with respect to each
   // of the six planes
   const Point *pp = p;
   float v[4];

   for (i=np; i>0; i--, p++) {

      Transform(*p,object2clip,v);


      if (v[0] < box.mi.x * v[3]) x0out++;	// out on left
      if (v[0] > box.ma.x * v[3]) x1out++;	// out on right
      if (v[1] < box.mi.y * v[3]) y0out++;	// out on top
      if (v[1] > box.ma.y * v[3]) y1out++;	// out on bottom
      if (v[2] < box.mi.z * v[3]) z0out++;	// out on near
      if (v[2] > box.ma.z * v[3]) z1out++;	// out on far
      }

   // Check if all vertices inside
   if (x0out+x1out+y0out+y1out+z0out+z1out == 0)
      return STATUS_INSIDE;

   // Check if all vertices are "outside" any of the six planes
   if (x0out== np || x1out==np || y0out==np ||
       y1out==np || z0out==np || z1out==np) {
      np=0;
      return STATUS_OUSIDE;
      }
   return(STATUS_UNKNOWN);
}



int IsVisible(const BBox &box)
{
    Point p[2];
    p[0] = box.mi;
    p[1] = box.ma;

    int status = PointVolumeStatus(2,p, object2clip, clipBox);
    if (status == STATUS_OUTSIDE) return(0);
    return(1);
}



#endif

#if 1

int
GShell::CollideFaces(const GFaceList &f,GConvexVolume &volume, GCollisionResponse &response, GHitInfo &hitInfo)
{
   const int *fp =f;			   // face list stepping pointers
   const int *fpend= fp+f.Length();

   int fi=0;	// face index

   gbool newCollision = gfalse;

#ifdef _DEBUG
    int planetest=0;
	int raytest=0;
    int tritest = 0;
	int culltest =0;
	int bboxtest = 0;

#endif

   if (volume.Outside(GetBBox())) {
	   return OUTSIDE;
   }		
   
   if (f.Length() <= 0) return(OUTSIDE);

   if (nfaces <=0) {
		nfaces= CountFaces();
   }
   if (nfaces <=0) return(OUTSIDE);

   Array<GPlaneMask> masks;

   int status = volume.ComputePlaneMasks(v,masks);

   if (status == OUTSIDE) {
	   TRACE("Vertices are outside of a plane \n");
	   return OUTSIDE;
   }	
   
   if (status == INSIDE) {
	   TRACE("object is inside of collision volume \n");
	   // to do : if object is small ignore collision
	   // else compute a reasonable collission distance

	   return INSIDE;
   }	
   // status  == OVERLAP	
  

   // need face normals for computation
   const PointArray &fnormals = GetFN();
   if (fnormals.Length() == 0) return(-1);


   int intersections=0;

 
   fi = -1;
   do { 		// for all faces
		 int n = *fp++; // length of this face
		 int ishole=0;
         int inHole = 0;

		 if (n<0) { ishole=1; n=-n; 
            TRACE("Hole selection not supported !\n");

         }
		 else fi++;

		 do { // dummy loop for break
			if (n>0) {
                const Point &fn = fnormals[fi]; // face normal
				if (volume.Outside(n,fp,masks)) break;

				float mDist = -DotProd(fn,v[fp[0]]);  // Parameter -D of plane equtation

				if (PlaneStatus(volume.v,Plane(fn,mDist)) != OVERLAP) break;

				if (1 || volume.ComputePenetration(n,fp,v,masks,response))
				{
					response.faceIndex = fi;
					TRACE("Face %d collided \n",fi);
					newCollision = gtrue;
					intersections ++;
					return OVERLAP;
				}


				#ifdef _DEBUG
					planetest++;
				#endif 

			} // n>2
		} while(0);
		
		fp+=n;
   }
   while (fp < fpend);

#ifdef _DEBUG
   //TRACE(" intersections:%d planetest %d raytest %d tritest %d culltest %d  t = %g\n",
   //	       intersections,planetest,raytest,tritest,culltest,hitInfo.t);
#endif

   
   return(OUTSIDE);
 //  if (NormalsFlipped()) fn *= Point(-1.0,-1.0,-1.0);  // flip normals
}

int
GShellI::CollideFaces(const GFaceList &f,GConvexVolume &volume, GCollisionResponse &response, GHitInfo &hitInfo)
{
   const int *fp =f;			   // face list stepping pointers
   const int *fpend= fp+f.Length();

   int fi=0;	// face index

   gbool newCollision = gfalse;

#ifdef _DEBUG
    int planetest=0;
	int raytest=0;
    int tritest = 0;
	int culltest =0;
	int bboxtest = 0;

#endif

   if (volume.Outside(GetBBox())) {
	   return OUTSIDE;
   }		
   
   if (f.Length() <= 0) return(OUTSIDE);

   if (nfaces <=0) {
		nfaces= CountFaces();
   }
   if (nfaces <=0) return(OUTSIDE);

   Array<GPlaneMask> masks;

   int status = volume.ComputePlaneMasks(v,masks);

   if (status == OUTSIDE) {
	   TRACE("Vertices are outside of a plane \n");
	   return OUTSIDE;
   }	
   
   if (status == INSIDE) {
	   TRACE("object is inside of collision volume \n");
	   // to do : if object is small ignore collision
	   // else compute a reasonable collission distance

	   return INSIDE;
   }	
   // status  == OVERLAP	
  

   // need face normals for computation
   const PointArray &fnormals = GetFN();
   if (fnormals.Length() == 0) return(-1);


   int intersections=0;

 
   fi = -1;
   do { 		// for all faces
		 int n = *fp++; // length of this face
		 int ishole=0;
         int inHole = 0;

		 if (n<0) { ishole=1; n=-n; 
            TRACE("Hole selection not supported !\n");

         }
		 else fi++;

		 do { // dummy loop for break
			if (n>0) {
                const Point &fn = fnormals[fi]; // face normal
				int p0;

				GPlaneMask m = masks[p0=verts[*fp++].v];
				n--;
				while (n>0) {
					m &= masks[verts[*fp++].v];
					n--;
				}
				if (m !=0) break; // outside

				float mDist = -DotProd(fn,v[p0]);  // Parameter -D of plane equtation

				//to do transform plane 
				if (PlaneStatus(volume.v,Plane(fn,mDist)) != OVERLAP) break;

				if (1 || volume.ComputePenetration(n,fp,v,masks,response))
				{
					response.faceIndex = fi;
					TRACE("Face %d collided \n",fi);
					newCollision = gtrue;
					intersections ++;
					return OVERLAP;
				}


				#ifdef _DEBUG
					planetest++;
				#endif 

			} // n>2
		} while(0);
		
		fp+=n;
   }
   while (fp < fpend);

#ifdef _DEBUG
   //TRACE(" intersections:%d planetest %d raytest %d tritest %d culltest %d  t = %g\n",
   //	       intersections,planetest,raytest,tritest,culltest,hitInfo.t);
#endif

   
   return(OUTSIDE);
 //  if (NormalsFlipped()) fn *= Point(-1.0,-1.0,-1.0);  // flip normals
}

#endif


