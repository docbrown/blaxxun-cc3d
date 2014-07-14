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
#ifndef  _GV_SPOT_LIGHT_
#define  _GV_SPOT_LIGHT_

#include <gvsfbool.h>
#include <gvsfcolor.h>
#include <gvsffloat.h>
#include <gvsfvec3f.h>
#include <gvsubnode.h>
#include <gvdirectionallight.h>

class GvSpotLight : public GvLight {

    GV_NODE_HEADER(GvSpotLight);

  public:
    // Fields:
    //GvSFBool	on;		// Whether light is on
    //GvSFFloat	intensity;	// Source intensity (0 to 1)
    //GvSFColor	color;		// RGB source color
    GvSFVec3f	location;	// 0 0 0, Source location
    GvSFVec3f	direction;	// 0 0 -1 Primary direction of illumination

	// VRML 1.0
    GvSFFloat	dropOffRate;	// Rate of intensity drop-off from primary
				// direction: 0 = constant intensity,
				// 1 = sharp drop-off
	// VRML 1 + 2 
    GvSFFloat	cutOffAngle;	// 0.785398 Angle (in radians) outside of which
				// intensity is zero, measured from
				// edge of cone to other edge

	// VRML 2
	GvSFFloat	beamWidth;		//         1.570796
	GvSFFloat	radius;		// Default : 100


	Point		currentPosition; // internal : last global position
	Point		currentDirection; // internal : last global direction

	// return bbox
	GvBool getBBoxEx(BBox &bbox);

	
	//
	int Do(GglRenderTraversal &state); 

	// set as light i in glState
	virtual void glSet(int i,GglRenderTraversal &state);

	// set as light i in glState
	virtual GvBool glSet(RenderState *glState,int i,GCamera *camera,const Matrix &m);

	// copy on proto instancing
    GvBool computeCopyOnInstance(GvCopyContext &context);




};

#endif /* _GV_SPOT_LIGHT_ */
