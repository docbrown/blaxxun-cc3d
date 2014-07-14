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
#include <gvperspectivecamera.h>

#include "grender.h"
#include "gmodel.h"


#include <gvscene.h>

GV_NODE_SOURCE_IMP(GvPerspectiveCamera,GvNode);

GvPerspectiveCamera::GvPerspectiveCamera()
{
    GV_NODE_CONSTRUCTOR(GvPerspectiveCamera);
    isBuiltIn = TRUE;

    GV_NODE_ADD_FIELD(position);
    GV_NODE_ADD_FIELD(orientation);
    GV_NODE_ADD_FIELD(focalDistance);
    GV_NODE_ADD_FIELD(heightAngle);
#ifdef GV_EXTENSIONS
	GV_NODE_ADD_FIELD(aspectRatio);
	GV_NODE_ADD_FIELD(nearDistance);
	GV_NODE_ADD_FIELD(farDistance);
#endif


    position.value[0] = 0.0;
    position.value[1] = 0.0;
    position.value[2] = 1.0;
    orientation.value.axis[0] = 0.0;
    orientation.value.axis[1] = 0.0;
    orientation.value.axis[2] = 1.0;
    orientation.value.angle = 0.0;
    focalDistance.value = 5.0;
#ifdef GV_EXTENSIONS
//HG
	aspectRatio.value = 1.0;
	nearDistance.value = 1.0; // Inventor defaults
	farDistance.value = 10;
#endif

    heightAngle.value = 0.785398; /// M_PI_4; // 45 degrees
}

GvPerspectiveCamera::~GvPerspectiveCamera()
{
}

int GvPerspectiveCamera::Do(GTraversal &state) 
{
  return(0);
}



//
// GvViewpoint
//

GV_NODE_SOURCE_IMP(GvViewpoint,GvNode);

GvViewpoint::GvViewpoint() : fieldOfView(0.785398),jump(TRUE),isBound(FALSE),isRegistered(FALSE)
{
    GV_NODE_CONSTRUCTOR(GvViewpoint);
    isBuiltIn = TRUE;

    GV_NODE_ADD_EVENT_IN(set_bind,GvSFBool);

    //
    GV_NODE_ADD_FIELD(fieldOfView)
    GV_NODE_ADD_FIELD(jump)
    GV_NODE_ADD_FIELD(orientation);
    GV_NODE_ADD_FIELD(position);
    GV_NODE_ADD_FIELD(description);

    GV_NODE_ADD_EVENT_OUT(bindTime);
    GV_NODE_ADD_EVENT_OUT(isBound);

    position.value[0] = 0.0;
    position.value[1] = 0.0;
    position.value[2] = 10.0;  // fixed 10.06.97, was 1
    orientation.value.axis[0] = 0.0;
    orientation.value.axis[1] = 0.0;
    orientation.value.axis[2] = 1.0;
    orientation.value.angle = 0.0;

/* HG removed 25.10.97, no proper global scene 
    GvScene *scene = getScene();
    if (scene)  scene->addViewpoint(this);
*/

}

// get/update camera from values in node
GvBool	GvViewpoint::getCamera(GCamera &camera, 
							   BBox &bbox,
							   float visibilityLimitNear,float visibilityLimit,
							   Matrix *cameraTransform)
{
	Matrix m;
    float viewAngle = 0.785398;
	float aspectRatio = 1.0f;
    orientation.get(m);
	camera.position=position;
    viewAngle = (float) fieldOfView;
	Point dir(0,0,-1.0);
	Point up(0,1.0,0);
			
    // apply orientation to standard dir and up vectors
    dir *= m;	
	up *= m;	up.Normalize();
			
    if (cameraTransform) {
        camera.position *= *cameraTransform;
        dir = RotateOnly(*cameraTransform,dir);
        up = RotateOnly(*cameraTransform,up);
        // near far focalDistance ??????????
    }
    dir.Normalize();
    up.Normalize();

	Point size = bbox.Size(); // computed bounding box
		    
	float field = max(max(fabs(size.x),fabs(size.y)),fabs(size.z));
			
	int positionInBox = bbox.Inside(camera.position);
  			
	if (bbox.IsEmpty() || (field<=1E-20f)) 
		field = 2.0f; // no bounding box yet, bad 
  			
            // compute distance to target point
  			//xx float targetDistance = field*2.0f;
  			float targetDistance = field*1.0f;
			
			// viewpoint inside scene 
			if (positionInBox) targetDistance = 0.2 * field; 

            camera.targetDistanceIsDefault=1;
            camera.zrangeIsDefault=1;

            // compute a reasonable z-range 

				if (visibilityLimit >0.0f) {
					camera.zfar = visibilityLimit;
					camera.zrangeIsDefault=0;
				}
				else {
					if (positionInBox) 
						camera.zfar = field*1.5;
					else camera.zfar = field*3.0f;
					
					Point center = bbox.Center();
					Point d = camera.position - center;
					float dist = d.Length();
					
					// make shure object is visible from viewpoint 
					if ((dist+field) > camera.zfar)
						camera.zfar = dist + field;

				}

				if (visibilityLimitNear > 0.0f)
				    camera.znear = visibilityLimitNear;
				else 
					 camera.znear = camera.zfar * camera.znearFactor;

            // compute target 
			camera.target = camera.position + targetDistance*dir;
			camera.up = up;

            // field of view 
			camera.height = 2.0 * tan(viewAngle * 0.5)*targetDistance;
			camera.width = camera.height *  aspectRatio;

			if (!bbox.IsEmpty())
				camera.SetWorldReference(bbox);

			camera.ComputeWorldUpFromUp(); 

            camera.OnChanged();

	return gtrue;

}


GvViewpoint::~GvViewpoint()
{
/* HG removed 25.10.97, no proper global scene 
   GvScene *scene = getScene();
   if (scene) scene->removeViewpoint(this);
*/
}

// send bind message to browser
void GvViewpoint::set_bind(GvSFBool *bind)
{
    TRACE("%s::set_bind \n",ClassName());
	GView *view = (GView*) getBrowser();

	ASSERT(view != NULL);

	if (view)
		view->BindViewpoint(this,*bind);
}

// a field has been changed
int GvViewpoint::OnFieldChanged(GvField *field)
{
  if (isBound) {
	  if (field != &bindTime && field !=&isBound && field != &description) {
		 setRedraw();
	  }
  }
  return GvNode::OnFieldChanged(field);

}

int GvViewpoint::Do(GglRenderTraversal &state)
{
	if (!isRegistered) {
		state.getBrowser()->Register(this);
	}
	return(0);
}






