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
#include <gvdirectionallight.h>


#ifdef _G_VRML1

//GvLightModel
GV_NODE_SOURCE_IMP(GvLightModel,GvNode);


GvLightModel::GvLightModel()
{
    GV_NODE_CONSTRUCTOR(GvLightModel);
    isBuiltIn = TRUE;

    GV_NODE_ADD_FIELD(model);

    model.value = PHONG;

    GV_NODE_DEFINE_ENUM_VALUE(LightModel, BASE_COLOR);
    GV_NODE_DEFINE_ENUM_VALUE(LightModel, PHONG);

    GV_NODE_SET_SF_ENUM_TYPE(model, LightModel);
}

GvLightModel::~GvLightModel()
{
}

#endif _G_VRML1

//
// GvLight
//


GV_NODE_SOURCE_IMP(GvLight,GvNode);

GvLight::GvLight() : on(TRUE),ambientIntensity(0.0),intensity(1.0),isRegistered(FALSE),global(FALSE)
{
    GV_NODE_CONSTRUCTOR(GvDirectionalLight);
    isBuiltIn = FALSE;

    GV_NODE_ADD_FIELD(on);
    GV_NODE_ADD_FIELD(ambientIntensity);

    GV_NODE_ADD_FIELD(intensity);
    GV_NODE_ADD_FIELD(color);

    GV_NODE_ADD_FIELD(attenuation);

    color.value[0] = color.value[1] = color.value[2] = 1.0;

    attenuation.value[0] = 1.0;
    attenuation.value[1] = attenuation.value[2] = 0.0;


}

GvLight::~GvLight()
{
}

// set redraw bit if a field changed 
int GvLight::OnFieldChanged(GvField *field)
{
	changed = 1; // mark changed bit 
	if (field == &on) setRedraw();
	else { // if ! on ignore redraw 
		if (on)  setRedraw(); 
	}
	return GvNode::OnFieldChanged(field);
}

//
// GvDirectionalLight
// 


GV_NODE_SOURCE_IMP(GvDirectionalLight,GvLight);

GvDirectionalLight::GvDirectionalLight() 
{
    GV_NODE_CONSTRUCTOR(GvDirectionalLight);
    isBuiltIn = TRUE;

    GV_NODE_ADD_FIELD(on);
    GV_NODE_ADD_FIELD(ambientIntensity);
    GV_NODE_ADD_FIELD(intensity);
    GV_NODE_ADD_FIELD(color);
    GV_NODE_ADD_FIELD(direction);

    GV_NODE_ADD_FIELD(attenuation);	 // non vrml 2

    direction.value[0] = 0.0;
    direction.value[1] = 0.0;
    direction.value[2] = -1.0;

	currentDirection.x = -1.0e30;
	global = FALSE;

}


GvDirectionalLight::~GvDirectionalLight()
{
}
