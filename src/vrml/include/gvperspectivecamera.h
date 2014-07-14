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
#ifndef  _GV_PERSPECTIVE_CAMERA_
#define  _GV_PERSPECTIVE_CAMERA_

#include <gvsfbool.h>
#include <gvsffloat.h>
#include <gvsftime.h>
#include <gvsfstring.h>
#include <gvsfrotation.h>
#include <gvsfvec3f.h>
#include <gvsubnode.h>


class GvPerspectiveCamera : public GvNode {

    GV_NODE_HEADER(GvPerspectiveCamera);

  public:
    GvSFVec3f		position;	// Location of viewpoint
    GvSFRotation	orientation;	// Orientation (rotation with
					// respect to (0,0,-1) vector)
    GvSFFloat	    focalDistance;	// Distance from viewpoint to
					// point of focus.
#ifdef GV_EXTENSIONS
    GvSFFloat	    aspectRatio;
	GvSFFloat   	nearDistance;
	GvSFFloat  		farDistance;
#endif

    GvSFFloat		heightAngle;	// Angle (in radians) of field
					// of view, in height direction

    int Do(GTraversal &state);					     

};

/*

  eventIn      SFBool     set_bind
  exposedField SFFloat    fieldOfView    0.785398
  exposedField SFBool     jump           TRUE
  exposedField SFRotation orientation    0 0 1  0
  exposedField SFVec3f    position       0 0 10
  field        SFString   description    ""
  eventOut     SFTime     bindTime
  eventOut     SFBool     isBound
*/



// VRML 2.0
class GvViewpoint : public GvNode {

    GV_NODE_HEADER(GvViewpoint);

  public:

    // Events In
    //GvSFBool     set_bind
    void set_bind(GvSFBool *bind);

    // Fields;
    GvSFFloat       fieldOfView;   //    0.785398
    GvSFBool        jump;          //     TRUE      
    GvSFRotation	orientation;	// Orientation ,  Default :  0 0 1  0      
    GvSFVec3f		position;	// Location of viewpoint,  Default : 0 0 10      
    GvSFString      description; // ""

    // EventsOut 
    GvSFTime        bindTime;
    GvSFBool        isBound;

	// a field has been changed check, for redraw
	int OnFieldChanged(GvField *field);

	GvBool			isRegistered;		// TRUE if node registered in Scene information
	
	// get/update camera from values in node
	//! get/update camera from values in node
	GvBool	getCamera(GCamera &camera, 
							   BBox &bbox,
							   float visibilityLimitNear=0.0f,float visibilityLimit=0.0f,
							   Matrix *cameraTransform=NULL);

	int Do(GglRenderTraversal &state); // register 

    int Do(GOptimizeTraversal &state); 

};


#endif /* _GV_PERSPECTIVE_CAMERA_ */
