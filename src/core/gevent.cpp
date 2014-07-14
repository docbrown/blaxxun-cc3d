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

@module GEvent.cpp - GLView event handler classes implementation|

Copyright (c) 1996	by Holger Grahn
All rights reserved

Purpose:

Classes:
<c GEventHandler>
<c GView>

Notes:

Changes:

Todo :



******************************************************************************/
#include "stdafx.h"


#include "gcamera.h"
#include "quaternation.h"

#include "grender.h"
#include "greporter.h"

#include "gmodel.h"

#ifdef _GV
// VRML 
#include <gvscene.h>
#include <gvnodes.h>
#include <gvsensors.h>

#include <gvtraverse.h>

// VRML 1.0 
#include <gvtransform.h>
#include <gvscale.h>

#endif

#include "grayselect.h"
#include "gfindnode.h"


#include "gevent.h"

#ifndef _GLVIEW_DLL
#include "glviewhdr.h"
#endif


//
// GEventHandler 
//
RTIMP(GEventHandler,0,RTRoot);

GEventHandler::~GEventHandler()
{
}

int GEventHandler::OnLButtonDown(UINT nFlags, Point &point)
{
	TRACE("%s:OnLButtonDown (%f %f %f) %x \n",this->ClassName(),point.x,point.y,point.z,nFlags);
	firstPoint = point;
	firstFlags = nFlags;
	return(EV_OK);
}

int GEventHandler::OnLButtonDblClk(UINT nFlags, Point &point)
{
	TRACE("%s:OnLButtonDblClk (%f %f %f) %x \n",this->ClassName(),point.x,point.y,point.z,nFlags);
	return(EV_DONE);
}

int GEventHandler::OnMouseMove(UINT nFlags, Point &point)
{
	TRACE("%s:OnMouseMove (%f %f %f) %x \n",this->ClassName(),point.x,point.y,point.z,nFlags);
	return(EV_OK);
}

int GEventHandler::OnLButtonUp(UINT nFlags,  Point &point)
{
	TRACE("%s:OnLButtonUp (%f %f %f) %x \n",this->ClassName(),point.x,point.y,point.z,nFlags);
	return(EV_DONE);
}


int GEventHandler::OnTimer()
{
	TRACE("%s:OnTimer %s \n",this->ClassName());
	return(EV_DONE);
}




// Compute hit ray with scene and initialize path

gbool GPathHandler::ComputeRayHit(RayRange	 &ray)
{
	GRaySelectTraversal traversal;
		traversal.method = GRaySelectTraversal::BY_OBJECT; 
		traversal.storeFullHitInfo = 1;
		traversal.storePath = 1;

		traversal.SetCamera(view->globalCamera);
		traversal.SetPhysicalCamera(view->GetPhysicalCamera());

        //traversal.viewInfo = &viewInfo;
	    // set current time
	    GTimeStack *ts= GTimeStack::NewElement();
	    ts->time = view->time; 
	    traversal.Set(ts); 
      
        traversal.ray = ray; 

		InitializeGvAttributes(traversal);
		traversal.InitializeAttributes();

		view->Traverse(traversal);
		
		GHitInfo *hit = NULL;

		hit = traversal.GetHit();

		if (hit) {	// ****** got a hit 

			lastHit = *hit;
			return TRUE;
			
		}
		else return FALSE;
}


//
// GPathHandler 
//
RTIMP(GPathHandler,0,GEventHandler);

GPathHandler::~GPathHandler()
{
}


//



//
// GSensorHandler 
//
RTIMP(GSensorHandler,0,GPathHandler);

GSensorHandler::~GSensorHandler()
{
}

#ifdef abc
gbool GSensorHandler::PointsOnSphere(float p1x, float p1y, float p2x, float p2y,float trackballSize) 
{

        Point a;		/* Axis of rotation */
        float phi;      /* how much to rotate about axis */
        Point p1, p2, d;
        float t;

        if (p1x == p2x && p1y == p2y)
        {
                Set(0,0,0,1.0); /* Zero rotation */
                return FALSE;
        }

/*
 * First, figure out z-coordinates for projection of P1 and P2 to
 * deformed sphere
 */
        p1.Set(p1x,p1y,tb_project_to_sphere(trackballSize,p1x,p1y));
        p2.Set(p2x,p2y,tb_project_to_sphere(trackballSize,p2x,p2y));

/*
 *      Now, we want the cross product of P1 and P2
 */
        a = CrossProd(p2,p1);

/*
 *      Figure out how much to rotate around that axis.
 */
        d = p1-p2;
        t = (d.Length()) / (2.0*trackballSize);
        /*
         * Avoid problems with out-of-control values...
         */
        if (t > 1.0) t = 1.0;
        if (t < -1.0) t = -1.0;
        phi = 2.0 * asin(t);

		SetAxisRotation(a,phi);
		return(TRUE);
}

float GSensorHandler::project_to_sphere(float r, float x, float y)
{
        float d, t, z;

        d = sqrt(x*x + y*y);
        if (d < r*M_SQRT1_2)    /* Inside sphere */
        z = sqrt(r*r - d*d);
        else
        {                       /* On hyperbola */
                t = r / M_SQRT2;
                z = t*t / d;
        }
        return z;
}
#endif


// map input ps point to world pw and local coordinates
// do plane projection if appropriate mapping mode 
// same as GTransformPathHandler::MapPoint
gbool GSensorHandler::MapPoint(const Point &ps /* screen */ ,Point &pw /* world */,Point &pl /* local */) 
{
	
	if (mapMode == MAP_SCREEN)
	{
		TRACE("GSensorHandler::MapPoint: MAP_SCREEN ");
		view->viewInfo.View2World(ps,pw);
		World2Local(pw,pl);
		TRACE("pw(%f %f %f) pl(%f %f %f) \n", pw.x, pw.y, pw.z, pl.x, pl.y, pl.z);
		return gtrue;
	}


	//
	// a ray is transformed into world coord space
	// a ray hit test is performed with a map plane (default z=0)
	//
	else if (mapMode == MAP_WORLD)
	{
		RayRange ray;
		view->ComputeWorldRay(ps,ray);
		TRACE("GSensorHandler::MapPoint: MAP_WORLD ");
		float t;
		if (mapPlane.Intersect(ray.p,ray.d,t)) {
			pw = ray.Eval(t);
			World2Local(pw,pl);
			TRACE("pw(%f %f %f) pl(%f %f %f) \n", pw.x, pw.y, pw.z, pl.x, pl.y, pl.z);
			return gtrue;
		}

	} 
	
	//
	// a ray is transformed into local coord space
	// a ray hit test with a map plane is performed (default z=0)
	//
	else if (mapMode == MAP_LOCAL) 
	{
		TRACE("GSensorHandler::MapPoint: MAP_LOCAL \n");
		RayRange ray;
		view->ComputeWorldRay(ps,ray);
		TRACE("Ray world  (%f %f %f) (%f %f %f) \n",ray.p.x,ray.p.y,ray.p.z,ray.d.x,ray.d.y,ray.d.z);

		// transform ray to local space 
		Point p1 = ray.p+ray.d;
		Point p2 = ray.p;
		
		p1 *= m.Inverse();
		p2 *= m.Inverse();
		ray.d = p1-p2;
		ray.p = p2;


		//ray *= m.Inverse();
		TRACE("Ray local  (%f %f %f) (%f %f %f) \n",ray.p.x,ray.p.y,ray.p.z,ray.d.x,ray.d.y,ray.d.z);
		/*
		double t;
		
		double dot = DotProd(mapPlane.n,ray.d);
		
		if (dot ==0 ) return(gfalse);

	    
		// compute the t-parameter
		t = (mapPlane.d - DotProd(mapPlane.n,ray.p)) / dot;

		pl.Set(ray.p.x+ray.d.x*t, ray.p.y+ray.d.y*t,ray.p.z+ray.d.z*t);
*/
		float t;
		if (mapPlane.Intersect(ray.p,ray.d,t)) 
		{
		   pl = ray.Eval(t);
		   // pw = m.m * pl;
		   Local2World(pl,pw);
		   TRACE("pw(%f %f %f) pl(%f %f %f) \n", pw.x, pw.y, pw.z, pl.x, pl.y, pl.z);
		   return gtrue;
		}
	}
	
	//
	//no hit!
	//
	TRACE("GSensorHandler::MapPoint: No hit!\n");
	view->viewInfo.View2World(ps,pw);
	World2Local(pw,pl);
	return(gfalse);

}



void GSensorHandler::SetSensor(GvNode *newSensor)
{

	sensor = newSensor;
	if (RTISA(newSensor,GvTouchSensor)) {
		SetMapMode(MAP_SCREEN);
		sensorType = TOUCH_SENSOR;
	}
	else if (RTISA(newSensor,GvPlaneSensor)) {
		SetMapMode(MAP_LOCAL);
		//SetMapMode(MAP_SCREEN);
		sensorType = PLANE_SENSOR;
	}
	else if (RTISA(newSensor,GvSphereSensor)) {
		SetMapMode(MAP_SCREEN);
		//SetMapMode(MAP_LOCAL);

		sensorType = SPHERE_SENSOR;
	}
	else if (RTISA(newSensor,GvCylinderSensor)) {
		SetMapMode(MAP_SCREEN);
		//SetMapMode(MAP_LOCAL);
		sensorType = CYLINDER_SENSOR;
	}
	else  { sensorType = NO_SENSOR;
			sensor = NULL;
		  }
}


gbool 
GSensorHandler::InitPath()
{
  path.Flush();

  GvPath &p = lastHit.path;
  int l = p.path.Length();

  // Find inner most Transform in path containing sensor
  int i;
  i=l-1;
  while (i>=0) {		
	  if (RTISA((GvNode*)p.path[i].node,Gv2Group)) {
		  if (( ((Gv2Group *)(GvNode*)p.path[i].node)->children.find(sensor)) >=0) 
				break;
	  }	
	  i--;
  }		
  if (i<0) 
	  return FALSE;



  // create path from root to Transform 
  path.path.SetMaxLength(i);

  for(l=0; l<=i; l++) {
	  //TRACE("%s \n",p.path[i].node->ClassName());
	  path.Append(p.path[l]);

  }	

  // get matrix of path 
	Matrix mm;
	if (path.getMatrix(mm))
		  m.Set(mm);
	else m.SetIdentity();


  return(TRUE);
}




int GSensorHandler::OnLButtonDown(UINT nFlags, Point &point)
{

	TRACE("%s:OnLButtonDown (%f %f %f) %x \n",this->ClassName(),point.x,point.y,point.z,nFlags);
	firstPoint = point;
	firstFlags = nFlags;
	Point pw,pl;

	// get starting point in view and local object coordinate 
	view->viewInfo.World2View(lastHit.pw,lastS);
	World2Local(lastHit.pw,lastO);
	point.z = lastS.z;


	MapPoint(point,pw,pl);
	
	pw = lastHit.pw;

	firstS = point;
	firstO = pl;


	GvSensor *s = (GvSensor*) (GvNode*) sensor;
	if (!s) return EV_POP;


		switch(sensorType) {
		case TOUCH_SENSOR : 
			{
				GHitInfo *hit = /*view->*/ &lastHit; // set from to GHitElement in gmodel.cpp

				GvTouchSensor *s = (GvTouchSensor *) (GvNode*)sensor;

				if (s->enabled && hit && s->isOver ) 
				{
					MatrixAndInverse m;

					hit->GetSensorMatrix(s,m);
					if (hit->vn.Length()>0) { 
						Point v=RotateOnly(hit->m,hit->vn); // transform global 
						v=RotateOnly(m.Inverse(),v); // transform to sensor matrix 
						v.Normalize();
						s->hitNormal.set(v);
					}	
					s->hitTexCoord.set(hit->vp);

					s->hitPoint.set(m.Inverse()*hit->pw);	// to do : in what coordinate sys


				}

				break;
			};
		case PLANE_SENSOR: 
			{
				GvPlaneSensor *s = (GvPlaneSensor *) (GvNode *)sensor;
				
				//intput: lastHit.pw
				pw = lastHit.pw;
				World2Local(pw,pl);
				pl.z *= -1;
				TRACE("Plane Sensor : pl start  %f %f %f \n",pl.x,pl.y,pl.z);
				mapPlane.Set(pl,Vector(0,0,1));
				//MapPoint(point,pw,pl);
				firstS = point;
				firstO = pl;

				// plane parallel to z in local speace through bearing point
#if 0				
				pw = lastHit.pw;
				World2Local(pw,pl);
				TRACE("Plane Sensor : pl start  %f %f %f \n",pl.x,pl.y,pl.z);

				mapPlane.Set(pl,Vector(0,0,-1));
		
				mapPlane = Transpose(m.Inverse()) * mapPlane;
				SetMapMode(MAP_WORLD);  // new 

				MapPoint(point,pw,pl);

				TRACE("Plane Sensor : pl start  %f %f %f \n",pl.x,pl.y,pl.z);

				firstS = point;
				firstO = pl;
#endif

				break;
			};


		case SPHERE_SENSOR: 
			{
				GvSphereSensor *s = (GvSphereSensor *) (GvNode *)sensor;

#if 0
				// Trackball center is local origin of sensors coordinate space 
				//transform sphere center to screen
				Point p(0,0,0);
				Local2World(p,p);
				view->World2Screen(p,centerS);


				//transform hit point to screen
				p = lastHit.pw;
				view->World2Screen(p,p);
				radiusS = sqrt(sqr(centerS.x-p.x)+ sqr(centerS.y-p.y)+ sqr(centerS.z-p.z));


				// new alles lokal
				Point pL;
				centerL.Set(0,0,0);
				pL = lastHit.pw;
				World2Local(pL,pL);
				
				radiusL = sqrt(sqr(pL.x)+ sqr(pL.y)+ sqr(pL.z));
/**/

				// initial 
				// 
				rotation.Set(0,0,0,1);
				//s->offset.get(rotation);

#endif

				//on lbuttondown sphere
				//
				//transformiere Punkte in camera space
				//

				// 
				//compute center in local and in camera space
				centerL.Set(0,0,0);
				Local2World(centerL,centerS);
				view->World2Cam(centerS,centerS);
				//centerS.x*=-1;
				TRACE("centerL (%f %f %f) centerS(%f %f %f)\n", centerL.x, centerL.y, centerL.z, centerS.x, centerS.y, centerS.z); 

				//hit in local and in camera space
				Point pL, pS;
				pL = lastHit.pw;
				view->World2Cam(pL,pS);
				//pS.x*=-1;
				World2Local(pL,pL); // hit in local coordinates == point on sphere
				TRACE("first hit local(%f %f %f) screen(%f %f %f)\n", pL.x, pL.y, pL.z, pS.x, pS.y, pS.z);



				//compute radiusS, radiusL
				pS = pS - centerS;
				TRACE("first hit on sphere screen(%f %f %f)\n", pS.x, pS.y, pS.z);
				radiusS = sqrt( sqr(pS.x) + sqr(pS.y) + sqr(pS.z) );	
				radiusL = sqrt( sqr(pL.x)+ sqr(pL.y)+ sqr(pL.z)); // radius of track sphere
				

				V0 = (pL - centerL)/radiusL; 
				//V0 = (pS - centerS)/radiusS; 
				
				//
				// initial 
				//
				rotation.SetAxisRotation(0,1,0,0);
				//s->rotation.get(rotation);
				//TRACE("Rotation (%f %f %f %f)\n",  rotation.x, rotation.y, rotation.z, rotation.w); 



				break;

			}
		case CYLINDER_SENSOR: 
			{
				GvCylinderSensor *s = (GvCylinderSensor *) (GvNode *)sensor;
				
				// Trackball center is local origin of sensors coordinate space 
				// 2screen: centerS
				Point p(0,0,0);
				Local2World(p,p);
				view->World2Screen(p,centerS);

				// compute angle between y axis, and bearing vector 
				// 2local: V0L
				p = lastHit.pw;
				World2Local(p,p);
				V0L = p;
				V0L.y = 0.f;

				Point y(0,1,0);


				// 2screen: VectorS, V0
				Local2World(y, VectorS);
				view->World2Screen(VectorS, VectorS);
				Local2World(V0L, V0);
				view->World2Screen(V0, V0);


				//
				// check for diskmode
				//


						Point a,b;
						RayRange ray;
				
						view->ComputeWorldRay(point, ray);
						World2Local(ray.p, a);
						World2Local( (ray.p + ray.d), b);
						p = a - b;




				p.Normalize();
				float acs = DotProd(p,y);

				if (acs < -1.0f) acs=-1.0f; else if (acs > 1.0f) acs=1.0f; // hg for savety
				float angle = acos(acs);

				float angle2 = fabs(PI - angle);
				if (angle2 < angle) angle = angle2;

				diskMode = angle < s->diskAngle;


				//
				// radiusL, radiusS
				//
				radiusL = sqrt(sqr(V0L.x)+ sqr(V0L.z));
				V0L /= radiusL;
				radiusS = sqrt(sqr(centerS.x-V0.x)+ sqr(centerS.y-V0.y)+ sqr(centerS.z-V0.z));

				// initial rotation 
				s->offset.get(angle);
				//rotation.SetAxisRotation(0,1,0,angle);
				totalRotation = angle;
				break;

			}
	};

	s->setActive(TRUE);

	SetState(1);

	return(EV_OK);
	// we assume sensor already picked 

}



int GSensorHandler::OnLButtonDblClk(UINT nFlags, Point &point)
{
	TRACE("%s:OnLButtonDblClk (%f %f %f) %x \n",this->ClassName(),point.x,point.y,point.z,nFlags);
	return(EV_POP);
}



int GSensorHandler::OnLButtonUp(UINT nFlags,  Point &point)
{
	//TRACE("%s:OnLButtonUp (%f %f %f) %x \n",this->ClassName(),point.x,point.y,point.z,nFlags);
	SetState(2);
	GvSensor *s = (GvSensor*) (GvNode*) sensor;
	if (!s) return EV_POP;



		switch(sensorType) {
		case TOUCH_SENSOR : 
			{
				GvTouchSensor *s = (GvTouchSensor *) (GvNode*)sensor;

				if (s->enabled) {
					// do i need to send point again ? 
					
					if (s->isActive) {
						s->setActive(FALSE);
						if (s->isOver)
							s->touchTime.set(view->getTime());
					}

				}
				else 
					s->setActive(FALSE);

				break;
			};
		case PLANE_SENSOR : 
			{
				GvPlaneSensor *s = (GvPlaneSensor *) (GvNode*)sensor;
				s->setActive(FALSE);
				if (s->autoOffset) {
					Point p; 
					s->translation.get(p);
					s->offset.set(p);
				}
			}
			break;

		case SPHERE_SENSOR: 
			{
				GvSphereSensor *s = (GvSphereSensor *) (GvNode *)sensor;
				s->setActive(FALSE);

				if (s->autoOffset) {
					//s->offset.set(s->rotation.value);
					Quaternation qoffset;
					s->offset.get(qoffset);
					s->offset.set(rotation*qoffset);
				}
			}
			break;

		case CYLINDER_SENSOR: 
			{
				GvCylinderSensor *s = (GvCylinderSensor *) (GvNode *)sensor;
				s->setActive(FALSE);

				if (s->autoOffset) {
					float v[4];
					v[0] = 0.f;
					v[1] = 1.f;
					v[2] = 0.f;
					v[3] = totalRotation;
				    //rotation.GetAxisRotation(v[0], v[1], v[2], v[3]);

					s->offset.set(v[3]);
				}

			}

		default :
			break;
		}

	view->Redraw();

	return(EV_POP);
}




int GSensorHandler::OnMouseMove(UINT nFlags, Point &point)
{

// this was new test 
 // get matrix of path 
/*	Matrix mm;
	if (path.getMatrix(mm))
		  m.Set(mm);
	else m.SetIdentity();
*/
	// wasn't here before InitPath();

	TRACE("%s:OnMouseMove (%f %f %f) %x \n",this->ClassName(),point.x,point.y,point.z,nFlags);


	if (State()==1 || sensorType == TOUCH_SENSOR) { // mouse pressed 
		
		// map new point to same depth plane 

		point.z = lastS.z;
		Point pw,pl;

		MapPoint(point,pw,pl);
		

		switch(sensorType) {
		case TOUCH_SENSOR : 
			{

				GHitInfo *hit = view->lastHit; // is this ok ??????

				GvTouchSensor *s = (GvTouchSensor *) (GvNode*)sensor;

				if (s->enabled && hit && s->isOver ) 
				{
					MatrixAndInverse m;

					hit->GetSensorMatrix(s,m);
					if (hit->vn.Length()>0) { 
						Point v=RotateOnly(hit->m,hit->vn); // transform global 
						v=RotateOnly(m.Inverse(),v); // transform to sensor matrix 
						v.Normalize();
						s->hitNormal.set(v);
					}	
					s->hitTexCoord.set(hit->vp);

					s->hitPoint.set(m.Inverse()*hit->pw);	// to do : in what coordinate sys


				}
				else {
/*
					if (s->isActive) {
						TRACE("ts no longer active \n");
						s->isActive.set(FALSE);

					}
*/
				}

				if (!s->isOver) {
						TRACE("ts no longer over \n");

				}
				break;
			};

		case PLANE_SENSOR : 
			{
				GvPlaneSensor *s = (GvPlaneSensor *) (GvNode*)sensor;

				if (s->enabled) {
					TRACE("Plane Sensor : pl  %f %f %f \n",pl.x,pl.y,pl.z);
					s->trackPoint.set(pl);

					Point p;
					
					MapPoint(firstS, p, firstO);//p is only a dummy
					Point delta = pl - firstO;

					s->offset.get(p);
					delta.z = 0; //xxx
					TRACE("Plane Sensor : delta  %f %f %f \n",delta.x,delta.y,delta.z);
					
					p += delta;

					if (s->minPosition.value[0] <= s->maxPosition.value[0] ) {
						p.x=limit(p.x,s->minPosition.value[0], s->maxPosition.value[0]);
					} 
					if (s->minPosition.value[1] <= s->maxPosition.value[1] ) {
						p.y=limit(p.y,s->minPosition.value[1], s->maxPosition.value[1]);
					} 

					s->translation.set(p);
				}
			}
			break;




		case SPHERE_SENSOR: 
		/*////////////////////////////////////////////////////////////////////////////
		
		on basis of the last and the current screen points two rays are computed
		and an intersection test with a sphere is done.
		if we are outside the sphere a linear stepping depending on the last sphere
		delta is computed

		at this stage everything is transformed to local coordinate space due
		to problems with non-uniform scale
		problems with computation in cam-space could not be solved!

		ideally the drag is based on a hyberbolic area around the sphere
		the attempt to avoid ray-hit test with real hyberbolic by computing z failed!

		tbd: compute hyberbolic in local space lying *parallel* to screen and do ray hit
		how to compute hyberbolic?
		
		////////////////////////////////////////////////////////////////////////////////*/
		{
				GvSphereSensor *s = (GvSphereSensor *) (GvNode *)sensor;
				if (s->enabled) {

					// initial rotation offset
					Quaternation qoffset;
					s->offset.get(qoffset);



					Quaternation quat;
					Point V1,a,b,d;
					RayRange ray, rayScreen;
					float t, alpha;
					Vector cross;

				
					view->ComputeWorldRay(point, ray);
					//TRACE("Ray world  (%f %f %f) (%f %f %f) \n",ray.p.x,ray.p.y,ray.p.z,ray.d.x,ray.d.y,ray.d.z);
					
					//compute a ray in screen coords
					//view->World2Cam(ray.p, a);
					//view->World2Cam( (ray.p + ray.d), b);
					//rayScreen.SetByPoints(a, b);
						
					//compute a ray in local coords	
					World2Local(ray.p, a);
					World2Local( (ray.p + ray.d), b);
					ray.SetByPoints(a, b);
					//TRACE("Ray local  (%f %f %f) (%f %f %f) \n",ray.p.x,ray.p.y,ray.p.z,ray.d.x,ray.d.y,ray.d.z);


					// map to 3D sphere by ray-select 
					if  (IntersectRaySphere(ray.p, ray.d,centerL, radiusL,/* intersection */t )>0) 
					{
					
						V1 = ray.Eval(t);
						
						TRACE("hit (%f %f %f)\n", V1.x, V1.y, V1.z);
						//normalize, V1 is point on unit sphere
						V1 = (V1-centerL)/radiusL;
						cross = CrossProd(V0, V1);
						float asn=cross.Length();
						if (asn < -1.0f) asn=-1.0f;
						else if (asn > 1.0f) asn= 1.0f; // hg for savety
						alpha = asin(asn);
					
						if ( DotProd(V0,V1) < 0)
							alpha += _PI / 2;
						
						quat.SetAxisRotation(cross.x,cross.y,cross.z,alpha);

						s->trackPoint.set(V1); //position on sphere !!!!
						//TRACE("Rotation (%f %f %f %f) * quat(%f %f %f %f)\n",  rotation.x, rotation.y, rotation.z, rotation.w,  quat.x, quat.y, quat.z, quat.w); 
						rotation =  quat * rotation;


						//preperations for the drag
						rotationAxis.Set(cross.x,cross.y,cross.z);
						lastRotation = alpha;
						mouseMove = point - lastS;
						mouseMoveLength = mouseMove.Length();
						//TRACE("mouseMove(%f %f %f) = point(%f %f %f) - lastS(%f %f %f)\n", mouseMove.x, mouseMove.y, mouseMove.z, point.x, point.y, point.z, lastS.x, lastS.y, lastS.z);
					} 
					else
					{ 
						mouseMove.Normalize();
						//TRACE("lastRotation (%f), rotationAxis(%f %f %f), mouseMove (%f %f %f)\n", lastRotation, rotationAxis.x, rotationAxis.y, rotationAxis.z, mouseMove.x, mouseMove.y, mouseMove.z); 
						float dot = DotProd(mouseMove, (point - lastS)/mouseMoveLength );
						alpha = lastRotation * dot;	
						//TRACE("alpha (%f) = lastRotation * dot(%f)\n", alpha, dot);
						quat.SetAxisRotation(rotationAxis,alpha);
						
						rotation =  quat * rotation;


						//set the trackpoint						
						TRACE("pl (%f %f %f), centerL(%f %f %f), radiusL (%f)\n", pl.x, pl.y, pl.z, centerL.x, centerL.y, centerL.z, radiusL); 

						Point tp = (pl-centerL) / radiusL;
						s->trackPoint.set(tp);
					}
						
						
						// hg 
/*						{

						// reget pl in current coordinate system
						Matrix mm;
						if (path.getMatrix(mm))	m.Set(mm);	else m.SetIdentity();
						World2Local(pw,pl);
*/						
			/*			
						Vector y(0,1,0);
						view->viewInfo.View2World(y,y);
						World2Local(y,y);
						y.Normalize();
						
						Vector x(1,0,0);
						view->viewInfo.View2World(x,x);
						World2Local(x,x);
						x.Normalize();
						
						Point V1= (pl-centerL) / radiusL;
						s->trackPoint.set(V1);

						float alpha =  _PI/2 * ((pl.x -centerL.x) / radiusL);


						quat.SetAxisRotation(y,alpha); 
						Quaternation qy;
						float alphay =  _PI/2 * ((pl.y -centerL.y) / radiusL);
						qy.SetAxisRotation(x,-alphay); 

						rotation = quat * qy;


					}
		
					else 
					{

						// MAP to 2D circle in local plane

						point.z = centerS.z; // z-plane through center 

						//MapPoint(point,pw,pl);
						view->viewInfo.View2World(point,pw);
						World2Local(pw,pl);


						V1 = pl;
						//normalize, V1 is point on unit sphere
						V1 = (V1-centerL)/radiusL;
						V1.Normalize();
					}
					*/			
					//TRACE("Rotation (%f %f %f %f) * qoffset(%f %f %f %f)\n",  rotation.x, rotation.y, rotation.z, rotation.w,  qoffset.x, qoffset.y, qoffset.z, qoffset.w); 
					s->rotation.set(rotation*qoffset);

					// incrementally add
					V0 = V1;

				}

			}
			break;

		case CYLINDER_SENSOR: 
			{
				GvCylinderSensor *s = (GvCylinderSensor *) (GvNode *)sensor;

				if (s->enabled) {

					Quaternation qoffset;
					float offsetAngle;

					s->offset.get(offsetAngle);
					qoffset.SetAxisRotation(0,1,0,offsetAngle);

					if (diskMode) {
						//TODO: s->trackPoint.set(V1);

						// float angle =(point.x -lastS.x) * TWOPI;
						//radiusS;
						float w1 =atan2(point.x-centerS.x,point.y-centerS.y);
						float w2 = atan2(lastS.x-centerS.x,lastS.y-centerS.y); 
						//float w2 = atan2(firstS.x-centerS.x,firstS.y-centerS.y); 
						float angle = w1 - w2;
						//if (angle <0) angle += TWOPI; 
										 
						totalRotation += angle;
						TRACE("diskmode: totalRotation (%f)\n", totalRotation);
						//q.SetAxisRotation(0,1,0,angle);
						//rotation =  q * qoffset;

					}
					
					else
					{
						//Quaternation quat;
						Point V1,a,b, a0, b0;
						RayRange ray, ray0;
						float in, out;
						float alpha;

				
						view->ComputeWorldRay(point, ray);
						//view->ComputeWorldRay(firstS, ray0);
						view->ComputeWorldRay(lastS, ray0);
						//TRACE("Ray world  (%f %f %f) (%f %f %f) \n",ray.p.x,ray.p.y,ray.p.z,ray.d.x,ray.d.y,ray.d.z);

						//view->World2Screen(ray.p, a);
						//view->World2Screen( (ray.p + ray.d), b);
						World2Local(ray.p, a);
						World2Local( (ray.p + ray.d), b);
						ray.SetByPoints(a, b);
						//TRACE("Ray local  (%f %f %f) (%f %f %f) \n",ray.p.x,ray.p.y,ray.p.z,ray.d.x,ray.d.y,ray.d.z);

						World2Local(ray0.p, a0);
						World2Local( (ray0.p + ray0.d), b0);
						ray0.SetByPoints(a0, b0);

						// map to 3D cylinder by ray-select 
						// currently in local coords, encountered problems using screen in non uniform scale
						if //(intcyl(ray.p, ray.d, centerS, VectorS, radiusS, in, out) )
							( IntersectRayCylinder(ray.p, ray.d, Point(0,0,0), Vector(0,1,0), radiusL, in, out) )
						{
							V1 = ray.Eval(in);
						
							//
							//get new V0
							//
							//must be evaluated because local coord system might be changed !!!
							IntersectRayCylinder(ray0.p, ray0.d, Point(0,0,0), Vector(0,1,0), radiusL, in, out);
							V0 = ray0.Eval(in);

							//project to y=0 plane, because movement is restricted to rotation around y
							V0.y = 0.f;
							V0 = V0/radiusL;
							TRACE("hit V0(%f %f %f)\n", V0.x, V0.y, V0.z);
							
							//transform to local
							//Matrix screen2local;
							//screen2local = m.Inverse() * view->viewInfo.view.Inverse();
							//V1 = screen2local * V1;
							TRACE("hit V1(%f %f %f)\n", V1.x, V1.y, V1.z);
							s->trackPoint.set(V1);
							
							//normalize
							V1.y = 0.f;
							V1 = (V1-centerL)/radiusL;
		
							//
							// cross = V0 X V1 (V0 and V1 are in y=0 plane!)
							//
							Vector cross;
							cross = CrossProd(V0, V1);
							float asn=cross.Length();

							//
							// compute angle
							//
							if (asn < -1.0f) asn=-1.0f;
							else if (asn > 1.0f) asn= 1.0f; // hg for savety
							alpha = asin(asn);
							if ( DotProd(V0,V1) < 0) alpha += _PI/2;

							//q.SetAxisRotation(cross.x,cross.y,cross.z,alpha);				
							//rotationAxis.Set(cross.x,cross.y,cross.z);//for drag

							//accumulated rotation
							//rotation = q * rotation;

							//get everything ready for the drag
							int direction = cross.y/asn;
							lastRotation = alpha*direction;
							mouseMove = point - lastS;
							mouseMoveLength = mouseMove.Length();

							//we need the total rotation for the limit logic
							totalRotation +=  alpha*direction;
						}
						else 

						{ 
							//normalize the screen delta
							mouseMove.Normalize();
								//TRACE("lastRotation (%f), rotationAxis(%f %f %f)\n", lastRotation, rotationAxis.x, rotationAxis.y, rotationAxis.z); 
							Point currentMM =  (point - lastS)/mouseMoveLength;
							float dot = DotProd(mouseMove, currentMM);
								//TRACE("currentMM(%f %f %f), mouseMove (%f %f %f)\n", currentMM.x, currentMM.y, currentMM.z, mouseMove.x, mouseMove.y, mouseMove.z); 
							alpha = lastRotation * dot;
							totalRotation += alpha;
								//TRACE("alpha (%f) = lastRotation * dot(%f)\n", alpha, dot);
							//q.SetAxisRotation(rotationAxis,alpha);
						
							//rotation =  q * rotation;


							//set the trackpoint						
							Point tp = (pl-centerL) / radiusL;
							s->trackPoint.set(tp);
						}

					//	{ // hg 
/*
							// reget pl in current coordinate system
							Matrix mm;
							if (path.getMatrix(mm))	m.Set(mm);	else m.SetIdentity();
							World2Local(pw,pl);


							Point V1= (pl-centerL) / radiusL;
							s->trackPoint.set(V1);
							float alpha =  _PI/2 * ((pl.x -centerL.x) / radiusL);
							q.SetAxisRotation(0,1,0,alpha); 
							rotation =  q * qoffset;
						}*/

					}
					
					float v[4];
					v[0] = 0.f;
					v[1] = 1.f;
					v[2] = 0.f;
					v[3] = totalRotation;// + offsetAngle;
					
					if (s->minAngle.value <= s->maxAngle.value) { 
					    //rotation.GetAxisRotation(v[0], v[1], v[2], v[3]);
						//v[3] *= v[1];

						//v[3] = limit(v[3]*2,s->minAngle.value, s->maxAngle.value);
						
						TRACE("totalRotation %f\n", totalRotation);
							
						v[3] = limit(totalRotation, s->minAngle.value, s->maxAngle.value);
						

					}
					//else
					  //s->rotation.set(rotation*qoffset);

					s->rotation.set(v);		
					TRACE("set %f %f %f %f\n", v[0], v[1], v[2], v[3]);	
				}

			}
			break;

		default : 
			break;
		}
		

		//TRACE("%s:OnMouseMove (%f %f %f) %x \n",this->ClassName(),delta.x,delta.y,delta.z,nFlags);

		lastS = point;
		lastO = pl;

	}
	return(EV_OK);
}


//
// GMultitSensorHandler 
//
RTIMP(GMultitSensorHandler,0,GEventHandler);

GMultitSensorHandler::~GMultitSensorHandler()
{
	for (int i=0; i<handlers.Length(); i++) {
		GSensorHandler * h = handlers[i];
		handlers[i]= NULL;
		h->unref();
	}

}

// add a new handler to list
void GMultitSensorHandler::add(GSensorHandler * newHandler)
{
	newHandler->ref();
	handlers.Append(newHandler);
	hasTouchSensors |=  RTISA(newHandler,GvTouchSensor);
}

// remove a new handler from list
void GMultitSensorHandler::remove(int i)
{
	ASSERT(i < handlers.Length());

	GSensorHandler * h = handlers[i];
	handlers[i]= NULL;
	handlers.Delete(i,1);
	h->unref();
}


int GMultitSensorHandler::OnLButtonDown(UINT nFlags, Point &point)
{	int ret;
	firstPoint = point;
	firstFlags = nFlags;

	ref();
	// call each handler 	
	int i=0;
	while (i<handlers.Length()) { 
		ret = handlers[i]->OnLButtonDown(nFlags,point);
		if (ret == EV_POP) { // handler is finished, remove it
			remove(i);
		}
		else i++;
	}
	if (handlers.Length()==0) ret = EV_POP;
	unref();
	return(ret);
}

int GMultitSensorHandler::OnLButtonDblClk(UINT nFlags, Point &point)
{	int ret;

	firstPoint = point;
	firstFlags = nFlags;

	ref();

	// call each handler 	
	int i=0;
	while (i<handlers.Length()) { 
		ret = handlers[i]->OnLButtonDblClk(nFlags,point);
		if (ret == EV_POP) { // handler is finished, remove it
			remove(i);
		}
		else i++;
	}
	if (handlers.Length()==0) ret = EV_POP;
	unref();

	return(ret);
}

int GMultitSensorHandler::OnMouseMove(UINT nFlags, Point &point)
{	int ret;
	
	// call each handler 	
	int i=0;
	ref();

	while (i<handlers.Length()) { 
		ret = handlers[i]->OnMouseMove(nFlags,point);
		if (ret == EV_POP) { // handler is finished, remove it
			remove(i);
		}
		else i++;
	}
	if (handlers.Length()==0) ret = EV_POP;
	
	unref();
	return(ret);
}

int GMultitSensorHandler::OnLButtonUp(UINT nFlags, Point &point)
{	int ret;
	
	// call each handler 	
	int i=0;
	ref();

	while (i<handlers.Length()) { 
		ret = handlers[i]->OnLButtonUp(nFlags,point);
		if (ret == EV_POP) { // handler is finished, remove it
			remove(i);
		}
		else i++;
	}
	if (handlers.Length()==0) 
		ret = EV_POP;

#ifndef _GLVIEW_DLL
	
	if (view->pView->lastMessageType == HIT_MESSAGE) // clear message
				view->pView->Message("");
#else
	// to do:

#endif
	unref();
	return(ret);
}


