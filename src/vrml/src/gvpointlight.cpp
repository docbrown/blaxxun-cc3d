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
#include <gvpointlight.h>

GV_NODE_SOURCE_IMP(GvPointLight,GvLight);

GvPointLight::GvPointLight() 
{
    GV_NODE_CONSTRUCTOR(GvPointLight);
    isBuiltIn = TRUE;
	// from GvLight
    GV_NODE_ADD_FIELD(ambientIntensity);
    GV_NODE_ADD_FIELD(attenuation);
    GV_NODE_ADD_FIELD(color);
    GV_NODE_ADD_FIELD(intensity);
    GV_NODE_ADD_FIELD(on);
	//
    GV_NODE_ADD_FIELD(location);
    GV_NODE_ADD_FIELD(radius);

    location.value[0] = location.value[1] = 0.0;
    // VRML 1.0 location.value[2] = 1.0;
    location.value[2] = 0.0;
    radius.value = 100.0;
	currentPosition.x = -1.0e30;
}

GvPointLight::~GvPointLight()
{
}

// copy on proto instancing
GvBool GvPointLight::computeCopyOnInstance(GvCopyContext &context)
{
	setCopyOnInstance(TRUE);
	return GvNode::computeCopyOnInstance(context);
}


GvBool GvPointLight::getBBoxEx(BBox &bbox)
{
	bbox.SetCenterSize(location,Point(radius,radius,radius)); // bit to large, so what
	return (!bbox.IsEmpty());
}

