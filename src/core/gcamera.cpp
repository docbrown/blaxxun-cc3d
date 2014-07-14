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

@module GCamera - GLView camera implementation	|

Copyright (c) 1995-1997	by Holger Grahn
All rights reserved

Purpose:
Maintain a viewpoint and implement incremental viewing functions

Classes:
GCamera

Notes:

Changes:

Todo :

******************************************************************************/


#include "stdafx.h"

#include "gutils.h"


#include "gclass.h"
#include "gcamera.h"

#include "grender.h"



// Limiting values for navigation **Max functions 

float GCamera::znearFactor = 0.001;
float GCamera::panMax= PI * 1.0; //
float GCamera::orbitMax= PI * 1.0; // 
float GCamera::rollMax= PI * 1.0; // 
float GCamera::zoomMax=2.0;
float GCamera::dollyMax=1.0; 

// compute znearFactor based on z-buffer depth 
float GCamera::SetZNearFactor(int zbufferDepth)
{
    if (zbufferDepth <= 16)
        znearFactor = 0.001;  // 0.001 to near
    else if (zbufferDepth <= 24)
        znearFactor = 0.0001;
    else 
        znearFactor = 0.00001;

    return(znearFactor);
}

//
// class GCamera
//

GCamera::GCamera() 
{
	viewMatrix.Identity();
	projectionMatrix.Identity();

    SetByVolume(-1.0,1.0,-1.0,1.0);
    perspective = 1;
    changed=0;
    changeLevel = 0;
    isDefault = 0;
    zrangeIsDefault = 0;
    targetDistanceIsDefault=0;
    aspect = 1.0;
    worldUp = up;
    worldUpDist = 0.0;
    worldCenter.Set(0,0,0);
    worldBBox.mi.Set(-1,-1,-1);
    worldBBox.ma.Set(1,1,1);
}


void GCamera::OnChanged() 
{  changed++; 
   isDefault = 0; 
   world2pValid=p2worldValid=0;
};


GCamera& GCamera::SetByVolume(float x1,float x2,float y1,float y2)
{ float f;
  width = x2-x1;
  height = y2-y1;
  float field = max(width,height);
  target.Set((x1+x2)*0.5,(y1+y2)*0.5,0.0);
  position.Set(target.x,target.y,field*1.0);
  orbitCenter = target;
  up.Set(0.0,1.0,0.0);  // UP is y
  f = TargetDistance();
  
  zfar = f*4.0;
  znear = zfar * znearFactor;	 // new 27.05.96

  zrangeIsDefault = 1;


  OnChanged();
  return(*this);
}

// recompute z range, to view scene
// keep in sync with GView::SetCamera !!!
int GCamera::SetZRange(const BBox &bbox)
{
	Point size = bbox.Size();
  	float field = max(max(fabs(size.x),fabs(size.y)),fabs(size.z));
    if (field <= EPS) return(-1);

 	int positionInBox = bbox.Inside(position);

	//m_distance = field*2.0f;
    //znear = min(field *0.001f,1.0f);
  	// zfar = max(znear,field * 4.0f,field*2.0f);
	if (positionInBox) 
		zfar = field*1.5;
	else zfar = field*3.0f;
   
    /// znear = zfar * znearFactor;	 // new 27.05.96
	// uncommented 15.03. because VRML 2.0 znear based on collisision distance 
    
    zrangeIsDefault = 1;
    OnChanged();
    return(0);
}


GCamera& GCamera::SetByVolume(const Point &p1,const Point &p2)
{
  Point size = p2-p1;
  width = fabs(size.x);
  height = fabs(size.y);

  float field = max(fabs(size.x),fabs(size.y),fabs(size.z));

  width = field,height = field;

  target = (p1+p2)*0.5;
  orbitCenter = target;

  float dist;
  dist = field*1.5;
  dist =  (field * 0.5 ) / atan(DEG2RAD(45) / 2.0);
  position.Set(target.x,target.y,target.z+dist);
  up.Set(0.0,1.0,0.0);  // UP is y
  zfar = field*5.0;
  znear = zfar * znearFactor;	
  zrangeIsDefault = 1;
  
  OnChanged();
  return(*this);
}


void GCamera::SetWorldReference(const BBox &bbox, const Point &center, const Point &up, float distance)
{
  worldBBox = bbox;
  worldCenter = center;
  orbitCenter = center;
  worldUp = up;
  worldUpDist = distance;
}


// find the "dominant" direction of the up vector
// and set worldUp accordingly
//
#define SGN(X) ((X)<0.0 ? -1 : 1)

void GCamera::ComputeWorldUpFromUp()
{
   Point d(fabs(up.x),fabs(up.y),fabs(up.z));

   if (up.z > up.y && up.z > up.x) {
	  	worldUp.Set(0,0,SGN(up.z)); worldUpDist = position.z;
   } else
   if (up.x > up.y && up.x > up.z) {
	  	worldUp.Set(SGN(up.x),0,0); worldUpDist = position.x;
   } else {
     	worldUp.Set(0,SGN(up.y),0); worldUpDist = position.y;
   }

}

void GCamera::SetWorldReference(const BBox &bbox)
{
  ComputeWorldUpFromUp();
  worldBBox = bbox;
  worldCenter = worldBBox.Center();
}

void GCamera::SetWorldLimit(const BBox &bbox,float scale)
{
  	worldLimit = bbox;
}


// Vector pointing to the "right", in world x axis of the camera 
Point GCamera::WorldRight() 
{ 
  // what if colinear ?
  return CrossProd(worldUp,NormDirection()); 
}

Point GCamera::WorldNormRight() 
{ Point dir(WorldRight());
  dir.Normalize(); 
  return(dir);
}

// vector pointing to target
Point GCamera::WorldTargetDirection() 
{ 
   return CrossProd(worldUp,WorldNormRight());
}

Point GCamera::WorldNormTargetDirection() 
{  Point dir = CrossProd(worldUp,WorldNormRight()); 
   dir.Normalize();
   return(dir);
};


// translate camera by delta
GCamera& GCamera::operator+=(const Point &p)
{
	position+=p;
    target+=p;
    OnChanged();
    return(*this);
}

#if 0
// make up vector orthonormal
GCamera& GCamera::CheckUp()
{
	// get camera right vector
	right = NormRight();


	// calculate new camera up vector
	up = NormCrossProd(NormDirection(), right);

	if (up.Length() > 0.9) (

	}
	else up.Set(0,1,0);		
    OnChanged();
    return(*this);
}
#endif

GCamera& GCamera::OrbitX(float dw,int worldRelative) 
{
	
    if (dw != 0.0) {
       Point axis(worldRelative ? worldUp : up);
       axis.Normalize();
       position.Rotate(target,axis,dw);
       OnChanged();
    }
    return(*this);
}

GCamera& GCamera::OrbitY(float dw,int worldRelative) {
    if (dw != 0.0) {
       Point axis(worldRelative ? WorldNormRight() : NormRight());
       position.Rotate(target,axis,dw);
       up=CrossProd(NormDirection(),axis);	// ????
       up.Normalize();
       OnChanged();
    }
    return(*this);
}

GCamera& GCamera::Roll(float dw) {
    if (dw != 0.0) {
        Point p= target + up;
        p.Rotate(target,NormDirection(),dw);
		up = p - target;
        up.Normalize();
        OnChanged();
    }
    return(*this);
}


GCamera& GCamera::Orbit(float wx,float wy,int worldRelative)
{  float f1,f2,df;
   f1 = TargetDistance();
   OrbitX(wx,worldRelative);
   OrbitY(wy,worldRelative);
   f2=TargetDistance();
   df = f2-f1;
   return(*this);
}

GCamera& GCamera::OrbitCenter(float wx,float wy,int worldRelative)
{
	// standard orbitCenter is target , make orbitCenter the target
	Point delta = orbitCenter-target;
	position += delta;
	target += delta;

	// is this ok ?  viewer should look onto orbitCenter
	Orbit(wx,wy,worldRelative);

	position -= delta;
	target -= delta;

	return(*this);
}

// rotate target around up (y) axis 
GCamera& GCamera::PanX(float dw,int worldRelative) {
    if (dw != 0.0) {
       Point axis(worldRelative ? worldUp : up);
       target.Rotate(position,axis,dw);
       OnChanged();
    }
    return(*this);
}

// rotate target around right (x) axis 

GCamera& GCamera::PanY(float dw,int worldRelative) {
    if (dw != 0.0) {
       Point axis(worldRelative ? WorldNormRight() : NormRight());

       target.Rotate(position,axis,dw);
       up=CrossProd(NormDirection(),axis); /// ???????
       up.Normalize();
       OnChanged();
    }
    return(*this);
}

GCamera& GCamera::Pan(float wx,float wy,int worldRelative)
{
   PanX(wx,worldRelative);
   PanY(wy,worldRelative);
   return(*this);
}


// translate in view right (x) direction 
GCamera& GCamera::DollyX(float d,int worldRelative)
{
    if (d != 0.0) {
       	Point dir(worldRelative ? WorldRight() : Right());

        dir.Normalize();
       	(*this)+=dir*d;
       	OnChanged();
    }
    return(*this);
}

// translate in view up (y) direction 
GCamera& GCamera::DollyY(float d,int worldRelative)
{
    if (d != 0.0) {
       	Point dir(worldRelative ? worldUp : up);
        dir.Normalize();
       	(*this)+=dir*d;
       	OnChanged();
    }
    return(*this);
}

// translate in view in/out (z) direction 
GCamera& GCamera::DollyZ(float d,int worldRelative)
{
    if (d != 0.0) {
      	Point dir(worldRelative ? WorldTargetDirection() : TargetDirection());
       	dir.Normalize();
       	(*this)+=dir*d;
       	OnChanged();
    }
    return(*this);
}

GCamera& GCamera::Dolly(float dx,float dy,float dz,int worldRelative)
{
   DollyX(dx,worldRelative);
   DollyY(dy,worldRelative);
   DollyZ(dz,worldRelative);
   return(*this);
}

GCamera& GCamera::Zoom(float f)
{
	if ((f != 1.0) && (f != 0.0)) {
        width*=f;
        height*=f;
        OnChanged();
    }
    return(*this);
}

GCamera& GCamera::ZoomDistance(float f)
{
	if ((f != 1.0) && (f != 0.0)) {
   		Point d = target-position;

		float dist = d.Len();

		f = (dist *f) / dist;
		target = position + (d*f);
        OnChanged();
    }
    return(*this);
}

// set direction to target using current targetDistance 
void GCamera::SetTargetDirection(const Point &dir)
{
    Point oldDir = target - position;
	float ol = oldDir.Len();
	float l = dir.Len();

	if (l>0.0f) {
		target = position + dir * (ol/l);
		OnChanged();	
		CheckUpVector();
	}
}

void GCamera::CheckUpVector()
{
    Point dir = Normalize(target - position);
    up -= dir * DotProd(up, dir);  // make up perpendicular to dir 
    up.Normalize();
	OnChanged();

	if ( fabs(DotProd(up,dir)) > (1.0-EPS)) {
		TRACE("Dir + up colinear \n");
	    up.Set(0,1,0);		

	}

}


// get vertical field of view in radians 
double GCamera::GetFovY()
{
   return fovy=(2.0* atan2(height*0.5f,TargetDistance()));
}


// Compute the  view orientation matrix
int 
GCamera::GetViewMatrix( Matrix &m)
{
    m = ViewMatrix(target,position,up);
    return(1);
}

const Matrix& GCamera::World2p()
{
	if (!world2pValid) {
		//world2p = projectionMatrix * viewMatrix;
		world2p = viewMatrix * projectionMatrix; // 15.11.96
		world2pValid = 1;
	}
	return(world2p);
}

const Matrix& GCamera::P2World()
{
   	if (!p2worldValid) 
	{
		p2world = Invert(World2p());
		p2worldValid =1;
	}
	return(p2world);
}


void GCamera::Project(const Point &p, Point &pout)
{
	const Matrix &m = World2p();
	pout = m * p;
}

int GCamera::UnProject(const Point &p, Point &pout) 
{
	const Matrix &m = P2World();
	pout = m * p;
	return(1);

}


// transform by matrix
// reorient position target up 
GCamera& GCamera::Transform(const Matrix &m) 
{
	Point p= m * position ;
	Point t= m * target;
	Point u = RotateOnly(m,up);

	float viewAngle = GetFovY();
//	float viewAngleX =   2.0* atan2(width*0.5f,TargetDistance()));

	float aspectRatio = width/height;
	
	u.Normalize();

	Point oldDir = target-position;

	Point d = t-p; // new dir 
	float scale = oldDir.Length() / d.Length();
	// new values 
	position = p;
	target = p + d * scale;
	up = u;

     // keep field of view  constant 
	height = 2.0 * tan(viewAngle * 0.5)*TargetDistance(); // new 4.07.97
	width = height *  aspectRatio;

	OnChanged();

	return *this;
}

// compute a ray defined by start & ending point in world space
// input : x,y coordinate in normalized view space 0..1

// output objP, objP1 start / end point 
void GCamera::ComputeWorldRay(float x,float y,Point &objP,Point &objP1,
							  float znear,float zfar)
{
   Point po;
   po.Set(x,y,0); 
   // x now in 0..1

   po.Set(po.x * 2.0 -1.0, po.y *2.0 -1.0,0.0);
   po.z = znear;

   const Matrix& p2w = P2World();
   objP = p2w * po;
   
   po.z = zfar; 
   objP1 = p2w * po;
}



// compute the view & projection matrix
int GCamera::UpdateViewProjectionMatrix()
{

   float w,h,r;
   
   float xl,xr,yb,yt; 	

   widthWorld = w = width;
   heightWorld = h = height;

   // Adapt to window aspect
   r = w / h;
   if (r < aspect) {
	   widthWorld = w=h*aspect;
   } else {
       heightWorld = h = w / aspect;
   }
   xl = -w *0.5;
   xr = w * 0.5;
   yb = -h * 0.5;
   yt = h *0.5;

   if (perspective) {

      float d = TargetDistance();
   	  
	  projectionMatrix.Identity();  

  	  float zd = (zfar - znear); 

      projectionMatrix.x[0][0] = -2.0f * d / w;
      projectionMatrix.x[1][1] = 2.0f * d / h;

      projectionMatrix.x[2][3] = 1.0f;		// perspective divide 
	  projectionMatrix.x[2][2] = (zfar) / zd; // scale z 


	  // version before
      //projectionMatrix.x[2][2] = - (zfar + znear) / zd;
      //projectionMatrix.x[3][2] = - (2.0f * zfar * znear) / zd;

      projectionMatrix.x[3][2] = -(zfar / zd) * znear;  

	  // D3D version :
      //projectionMatrix.x[2][2] = (zfar) / zd; // scale z 
      //projectionMatrix.x[3][2] = -(zfar * znear) /  zd ;  

      //projectionMatrix.x[2][3] = -1.0f;

      projectionMatrix.x[3][3] = 0.0f;

	  projectionMatrix.contents = Matrix::HAS_TRANSLATION|Matrix::HAS_SCALE|Matrix::HAS_NON_UNIFORM_SCALE|Matrix::HAS_PERSPECTIVE;
    
   
   }
   else {
//      glOrtho(xl,xr,yb,yt,znear,zfar);
//      glCheckError();
   }

   viewMatrix = ViewMatrix(target, position, up);
   
   //gluLookAt(position.x,position.y,position.z,target.x,target.y,target.z,up.x,up.y,up.z);

   world2pValid=p2worldValid=0;
   return 1;
}


#ifdef _OGL
// Set Camera as current GL projection matrix
void GCamera::glSet()
{  float w,h,r;
   float xl,xr,yb,yt; 	

   widthWorld = w = width;
   heightWorld = h = height;

   // Adapt to window aspect
   r = w / h;
   if (r < aspect) {
	   widthWorld = w=h*aspect;
   } else {
       heightWorld = h = w / aspect;
   }
   xl = -w *0.5;
   xr = w * 0.5;
   yb = -h * 0.5;
   yt = h *0.5;

   glMatrixMode(GL_PROJECTION);
   glCheckError();

   glLoadIdentity();
   glCheckError();

   if (perspective) {
      float d = TargetDistance();
   	  
	  projectionMatrix.Identity();  

  	  float zd = (zfar - znear); 

      projectionMatrix.x[0][0] = 2.0f * d / w;
      projectionMatrix.x[1][1] = 2.0f * d / h;

	  // version before
      projectionMatrix.x[2][2] = - (zfar + znear) / zd;
      projectionMatrix.x[3][2] = - (2.0f * zfar * znear) / zd;

	  // D3D version :
      //projectionMatrix.x[2][2] = (zfar) / zd; // scale z 
      //projectionMatrix.x[3][2] = -(zfar * znear) /  zd ;  

      projectionMatrix.x[2][3] = -1.0f;
      projectionMatrix.x[3][3] = 0.0f;
	  projectionMatrix.contents = Matrix::HAS_TRANSLATION|Matrix::HAS_SCALE|Matrix::HAS_NON_UNIFORM_SCALE|Matrix::HAS_PERSPECTIVE;

      
      glLoadMatrixf(&projectionMatrix.x[0][0]);
/*
      
      glMatrix mg;
      mg.Set(m);
      mg.glSet();
*/


/*
      float f = znear / TargetDistance();
	  xl *=f;	  xr *=f;	// frustum describes rectangle on near plane
	  yb *=f;	  yt *=f;
     
      glFrustum(xl,xr,yb,yt,znear,zfar);
*/

     // using gluPerspective
     //float fovy=(2.0* atan2(h*0.5,TargetDistance()));
	 //gluPerspective(RAD2DEG(fovy), w / h, znear, zfar);
     //glCheckError();
   
   }
   else {
      glOrtho(xl,xr,yb,yt,znear,zfar);
      glCheckError();
   }


// hg new 
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
//

   viewMatrix = ViewMatrix(target, position, up);
   //glMultMatrixf(&viewMatrix.x[0][0]);

   gluLookAt(position.x,position.y,position.z,target.x,target.y,target.z,up.x,up.y,up.z);
   glCheckError();

   glMatrixMode(GL_MODELVIEW);
   glCheckError();

   world2pValid=p2worldValid=0;


}

#endif

