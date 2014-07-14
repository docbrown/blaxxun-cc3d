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
#include <gvorthographiccamera.h>

GV_NODE_SOURCE_IMP(GvOrthographicCamera,GvNode);

GvOrthographicCamera::GvOrthographicCamera()
{
    GV_NODE_CONSTRUCTOR(GvOrthographicCamera);
    isBuiltIn = TRUE;

    GV_NODE_ADD_FIELD(position);
    GV_NODE_ADD_FIELD(orientation);
    GV_NODE_ADD_FIELD(focalDistance);
    GV_NODE_ADD_FIELD(height);

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
    height.value = 2.0;

#ifdef GV_EXTENSIONS
//HG
	aspectRatio.value = 1.0;
	nearDistance.value = 1.0; // Inventor defaults
	farDistance.value = 10;
#endif


}

GvOrthographicCamera::~GvOrthographicCamera()
{
}

int GvOrthographicCamera::Do(GTraversal &state) 
{
  return(0);
}

