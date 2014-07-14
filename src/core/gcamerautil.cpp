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

#include "stdafx.h"
#include "point.h"

#include "gcamera.h"
#include "quaternation.h"


#include <gvperspectivecamera.h>
#include <gvorthographiccamera.h>

#include "gcamerautil.h"


void 
GCameraValue::Interpolate(const GCameraValue &a,const GCameraValue &b, float alpha)
{
    //orientation.Interpolate(a.orientation,b.orientation,alpha); // sometimes get 360 deg z spin ??
    orientation.InterpolateSpin(a.orientation,b.orientation,alpha);
    position = ::Interpolate(a.position,1.0-alpha,b.position,alpha);
    fieldOfView = ::Interpolate(a.fieldOfView,1.0-alpha,b.fieldOfView,alpha);
   
}


// set camera value by camera settings in c
void 
GCameraValue::Set(GCamera &camera)
{

    position = camera.position;
    fieldOfView = camera.GetFovY();
    Vector axis;
    float angle;
    ConvertCameraToOrientation(camera.position, camera.target, camera.up,
                    axis, angle);

    orientation.SetAxisRotation(axis,angle);
}


// set value by one of the camera nodes
int
GCameraValue::Set(GvNode *n)
{
    if (RTISA(n,GvPerspectiveCamera)) 
    {
  		GvPerspectiveCamera *c=RTCAST(n,GvPerspectiveCamera);
		c->orientation.get(orientation);
        position = c->position;
        fieldOfView = (float) c->heightAngle;
    }
    else if (RTISA(n,GvViewpoint)) 
    {
  		GvViewpoint *c=RTCAST(n,GvViewpoint);
		c->orientation.get(orientation);
        position = c->position;
        fieldOfView = (float) c->fieldOfView;
    }
     else if (RTISA(n,GvOrthographicCamera)) 
    {
	  		GvOrthographicCamera *c=RTCAST(n,GvOrthographicCamera);
		    c->orientation.get(orientation);
            position = c->position;
            fieldOfView = 0;

    } else {return(0);}
    return(1);
}

int
GCameraValue::Set(GvNode *n,const Matrix *cameraTransform)
{
	if (!cameraTransform) {
		return(Set(n));
	}

    if (RTISA(n,GvPerspectiveCamera)) 
    {
  		GvPerspectiveCamera *c=RTCAST(n,GvPerspectiveCamera);
		c->orientation.get(orientation);
        position = c->position;
        fieldOfView = (float) c->heightAngle;
    }
    else if (RTISA(n,GvViewpoint)) 
    {
  		GvViewpoint *c=RTCAST(n,GvViewpoint);
		c->orientation.get(orientation);
        position = c->position;
        fieldOfView = (float) c->fieldOfView;
    }
     else if (RTISA(n,GvOrthographicCamera)) 
    {
	  		GvOrthographicCamera *c=RTCAST(n,GvOrthographicCamera);
		    c->orientation.get(orientation);
            position = c->position;
            fieldOfView = 0;

    } else {return(0);}

    if (cameraTransform) { // transform by matrix
			Matrix m;
			Point dir(0,0,-1.0);
			Point up(0,1.0,0);

		    Vector axis;
			float angle;



			orientation.GetMatrix(m);
			TRACE("Old orient %g %g %g %g \n",orientation.x,orientation.y,orientation.z,orientation.w);
			
            // apply orientation to standard dir and up
            dir *= m;	
			up *= m;	up.Normalize();

			// debug
			ConvertCameraToOrientation(position, position+dir,up,axis, angle);
			orientation.SetAxisRotation(axis,angle);
			TRACE("same  orient %g %g %g %g \n",orientation.x,orientation.y,orientation.z,orientation.w);

			
            if (cameraTransform) {
                position *= *cameraTransform;
                dir = RotateOnly(*cameraTransform,dir);
                up = RotateOnly(*cameraTransform,up);
            }
            dir.Normalize();
            up.Normalize();

			ConvertCameraToOrientation(position, position+dir,up,axis, angle);
			orientation.SetAxisRotation(axis,angle);
			TRACE("New orient %g %g %g %g \n",orientation.x,orientation.y,orientation.z,orientation.w);
	}


    return(1);
}




#if 0

void ConvertCameraToOrientation(const Point &position, const Point &target, const Point &up, 
							Vector &axis, float &angle)
{
	Point tdir(0,0,-1.0);
	Point tup(0,1.0,0);
 
    Point dir = target-position;
    dir.Normalize();

    // rotate default view direction tdir onto dir
    Point rotV = NormCrossProd(tdir,dir);
    float rotDot = DotProd(tdir,dir);
    float rotA = acos(rotDot);


    Quaternation rot;

//	rot.Set(tdir,dir);
#if 1

#if 0
    if (fabs(rotV.Length()) <= 0.001) { // tdir & dir are colinear ?? 
        rot.SetAxisRotationCos(Point(0,1,0),rotDot);
		//rot.Set(0,0,0,1);
	}
	else {
		rot.SetAxisRotationCos(rotV,rotDot);
	}
#endif 

    if (fabs(rotA) <= 0.001) { // nothing to rotate
       rot.SetAxisRotation(tdir,0);
    }
    else { 
       rot.SetAxisRotation(rotV,rotA);
    }
#endif


    rot.GetAxisRotation(axis.x,axis.y,axis.z,angle);

    // apply computed rotation to standard up vector tup
    Matrix m = RotationAxisMatrix(axis, angle);
	//Matrix m;
	//rot.GetMatrix(m);
    
	Point     newTUp = RotateOnly(m,tup);
    newTUp.Normalize();

#if 1
    // Point     newDir = RotateOnly(m,tdir);
    // newDir.Normalize();

    Point rot1V = NormCrossProd(newTUp,up);
    float rot1Dot = DotProd(newTUp,up);
    float rot1A = acos(rot1Dot);

#if 0
    if (fabs(rot1V.Length()) <= 0.001) { // newTup and up are colinear ?? 
		rot1V.Set(0,0,1);
	}
#endif
	
    // any rotation left

    if (fabs(rot1A) <= 0.001) { // nothing to rotate

    }
    else 	
    { 
       Quaternation rot1;
       rot1. SetAxisRotationCos(rot1V,rot1Dot);
       rot = /* outer */ rot1 *  /* inner */ rot;

    }
#endif 


#if 0
    Quaternation rot1;
	rot1.Set(tup,up);
    rot = /* outer */ rot1 *  /* inner */ rot;
#endif 


    rot.GetAxisRotation(axis.x,axis.y,axis.z,angle);

    {
        // Debug
#if 0
        Matrix m = RotationAxisMatrix(axis, angle);
        Point     newTUp = RotateOnly(m,tup);
        newTUp.Normalize();

        Point     newDir = RotateOnly(m,tdir);
        newDir.Normalize();
#endif
    }

}
#else

void ConvertCameraToOrientation(const Point &position, const Point &target, const Point &up, 
								Vector &axis, float &angle)

// based on  Copyright (C) 1995  Stephen Chenney (schenney@cs.berkeley.edu)


{
   Point dir = target - position;
// Point dir = position - target;

 Quaternation  norm_quat,inv_norm_quat;

 dir.Normalize();
 
 Point upn = Normalize(up);
 
 float tmp = DotProd(upn,dir);
 Point tmp_v =dir*tmp;

 Point v = upn - tmp_v;
 v.Normalize();


 axis.Set(dir.y,-dir.x,0);
 
 if (DotProd(axis,axis) < 1E-8) {
    if (dir.z> 0) {
		norm_quat.Set(0,1,0,  0);
	} else {
		norm_quat.Set(0,0,0,  1);
	}
 } else {
 	axis.Normalize();

    norm_quat.SetAxisRotationCos(axis, -dir.z);
 }

 // norm_quat rotates to line up view directions


	/* Find the inverse rotation. */
    inv_norm_quat.Set(-norm_quat.x,-norm_quat.y,-norm_quat.z,norm_quat.w);

	/* Rotate the y. */
	Quaternation y_quat,new_y_quat;

	y_quat.Set(0, 1, 0, 0.0);
	new_y_quat = norm_quat*y_quat;
	new_y_quat = new_y_quat*inv_norm_quat;
	
	Vector new_y;
	
	new_y.Set(new_y_quat.x,new_y_quat.y,new_y_quat.z);


	/* Now need to find out how much to rotate about n to line up y. */
	Vector temp_v;

	temp_v= CrossProd(new_y,v);

	if (DotProd(temp_v,temp_v) < 1E-8) {
		/* The old and new may be pointing in the same or opposite. Need
		** to generate a vector perpendicular to the old or new y.
		*/
		temp_v.Set(0, -v.z, v.y);

		if ( DotProd(temp_v,temp_v)  < 1.e-8 )
			temp_v.Set(v.z, 0, -v.x);
	}
	temp_v.Normalize();



	Quaternation rot_y_quat,rot_quat;
	rot_y_quat.SetAxisRotationCos(temp_v, DotProd(new_y, v));

	/* rot_y_quat holds the rotation about the initial camera direction needed
	** to align the up vectors in the final position.
	*/

	/* Put the 2 rotations together. */
	rot_quat =  rot_y_quat * norm_quat;

	/* Extract the axis and angle from the quaternion. */
	double angled;

	rot_quat.GetAxisRotation(axis, angled);
	angle = angled;
}

#endif




// set the camera from GCameraValuse c
void SetCamera(GCamera *camera,const GCameraValue &c,const Matrix *cameraTransform)
{

  			float targetDistance = camera->TargetDistance(); // use old distance 
            float oldRatio = camera->width/ camera->height;

			Matrix m;
			//c.orientation.GetMatrix(m);
            Vector axis;
            double angle;
            c.orientation.GetAxisRotation(axis,angle);
            m = RotationAxisMatrix(axis,angle);


            Point dir(0,0,-1.0);
			Point up(0,1.0,0);
			
            // apply orientation transform to get current dir & up vectors 
            dir *= m;	
			up *= m;	up.Normalize();
			
			camera->position=c.position;
            
            if (cameraTransform) {
                camera->position *= *cameraTransform;
                dir = RotateOnly(*cameraTransform,dir);
                up = RotateOnly(*cameraTransform,up);
            }
            dir.Normalize();
            up.Normalize();
    

			camera->target = camera->position + targetDistance*dir;
			camera->up = up;
            camera->height = 2.0 * tan(c.fieldOfView*0.5)*targetDistance;
			camera->width = camera->height *  oldRatio;

            //TRACE("New camera (%f %f %f) (%f %f %f) %f \n",camera->position.x,camera->position.y,camera->position.z,
             //               dir.x,dir.y,dir.z,c.fieldOfView);
		    camera->OnChanged();
}
