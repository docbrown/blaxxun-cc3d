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

@module GvInfo.cpp - GLView VRML 2.0 info nodes|

Copyright (c) 1996 - 1998	by Holger Grahn
All rights reserved

Purpose:

Classes:


Notes:


  Changes:
  22.03.98 - fixed a serious bug for defaults in NavigationInfo was 0.75, 1.6, undefined
			instead of 0.25 1.6 00.75 !

$Revision: 1.5 $
$Log:


Todo :

******************************************************************************/

#include "stdafx.h"
#include <gvinfo.h>

#include "grender.h"
#include "gmodel.h"



#ifdef _G_VRML1
//
// GvInfo
//
GV_NODE_SOURCE_IMP(GvInfo,GvNode);

GvInfo::GvInfo()
{
    GV_NODE_CONSTRUCTOR(GvInfo);
    isBuiltIn = TRUE;

    GV_NODE_ADD_FIELD(string);

    string.value = "<Undefined info>";
}

GvInfo::~GvInfo()
{
}

#endif _G_VRML1

//
// GvWorldInfo
//
GV_NODE_SOURCE_IMP(GvWorldInfo,GvNode);

GvWorldInfo::GvWorldInfo()
{
    GV_NODE_CONSTRUCTOR(GvWorldInfo);
    isBuiltIn = TRUE;

    // GV_NODE_ADD_FIELD(_info);
    // add _info field as info (there is an info member in rtriit
    if (firstInstance)							      
		 fieldData->addField(this, "info", &this->_info);    
    this->_info.setContainer(this);

    GV_NODE_ADD_FIELD(title);

}

GvWorldInfo::~GvWorldInfo()
{
}


//
// GvNavigationInfo
//
GV_NODE_SOURCE_IMP(GvNavigationInfo,GvNode);

GvNavigationInfo::GvNavigationInfo() : 
        avatarSize(3),headlight(TRUE),speed(1.0),type(2),
            visibilityLimit(0.0)
        
{
    GV_NODE_CONSTRUCTOR(GvNavigationInfo);
    isBuiltIn = TRUE;

    GV_NODE_ADD_FIELD(avatarSize);
    GV_NODE_ADD_FIELD(headlight);
    GV_NODE_ADD_FIELD(speed);
    GV_NODE_ADD_FIELD(type);
    GV_NODE_ADD_FIELD(visibilityLimit);
   
	GV_NODE_ADD_EVENT_IN(set_bind,GvSFBool);

    GV_NODE_ADD_EVENT_OUT(isBound);

    avatarSize.values[0] = 0.25;
    avatarSize.values[1] = 1.6;
    avatarSize.values[2] = 0.75;

    type.values[0]="WALK";
    type.values[1]="ANY";


}

// send bind message to browser
void GvNavigationInfo::set_bind(GvSFBool *bind)
{
    TRACE("%s::set_bind \n",ClassName());
	GView *view = (GView*) getBrowser();
	ASSERT(view != NULL);

	if (view)
		view->BindNavigationInfo(this,*bind);
}

GvNavigationInfo::~GvNavigationInfo()
{
}

//
// GvBackground
//

GV_NODE_SOURCE_IMP(GvBackground,GvNode);

GvBackground::GvBackground() : 
    groundAngle(0),groundColor(0),backUrl(0),
    bottomUrl(0),frontUrl(0),leftUrl(0),rightUrl(0),topUrl(0),skyAngle(0),
    skyColor(1)
{
    GV_NODE_CONSTRUCTOR(GvBackground);
    isBuiltIn = TRUE;

    GV_NODE_ADD_FIELD(groundAngle);
    GV_NODE_ADD_FIELD(groundColor);
    GV_NODE_ADD_FIELD(backUrl);
    GV_NODE_ADD_FIELD(bottomUrl);
    GV_NODE_ADD_FIELD(frontUrl);
    GV_NODE_ADD_FIELD(leftUrl);
    GV_NODE_ADD_FIELD(rightUrl);
    GV_NODE_ADD_FIELD(topUrl);
    GV_NODE_ADD_FIELD(skyAngle);
    GV_NODE_ADD_FIELD(skyColor);

    GV_NODE_ADD_EVENT_IN(set_bind,GvSFBool);
    GV_NODE_ADD_EVENT_OUT(isBound);
    skyColor.values[0] = skyColor.values[1] = skyColor.values[2] = 0.0; 

}

// send bind message to browser
void GvBackground::set_bind(GvSFBool *bind)
{
    TRACE("%s::set_bind \n",ClassName());
	GView *view = (GView*) getBrowser();
	ASSERT(view != NULL);

	if (view)
		view->BindBackground(this,*bind);

}

// a field has been changed
int GvBackground::OnFieldChanged(GvField *field)
{
  if (isBound) {
	  if (field != &isBound) {
		 setRedraw();
	  }
  }
  return GvNode::OnFieldChanged(field);
}


GvBackground::~GvBackground()
{
}


//
// GvFog
//

GV_NODE_SOURCE_IMP(GvFog,GvNode);

GvFog::GvFog() : 
    visibilityRange(0)
{
    GV_NODE_CONSTRUCTOR(GvFog);
    isBuiltIn = TRUE;

    GV_NODE_ADD_FIELD(color);
    GV_NODE_ADD_FIELD(fogType);
    GV_NODE_ADD_FIELD(visibilityRange);

    GV_NODE_ADD_EVENT_IN(set_bind,GvSFBool);
    GV_NODE_ADD_EVENT_OUT(isBound);

    fogType.value = "LINEAR";
    color.value[0] = color.value[1] = color.value[2] = 1.0; 

}

// send bind message to browser
void GvFog::set_bind(GvSFBool *bind)
{
    TRACE("%s::set_bind \n",ClassName());
	GView *view = (GView*) getBrowser();
	ASSERT(view != NULL);
	if (view)
		view->BindFog(this,*bind);
}

// a field has been changed
int GvFog::OnFieldChanged(GvField *field)
{
  if (isBound) {
	  if (field != &isBound) {
		 setRedraw();
	  }
  }
  return GvNode::OnFieldChanged(field);
}


GvFog::~GvFog()
{
}


//
// GvFog2
//

GV_NODE_SOURCE_IMP(GvFog2,GvFog);

GvFog2::GvFog2(): visibilityStart(0),density(1.0f)
{
    GV_NODE_CONSTRUCTOR(GvFog);
    isBuiltIn = FALSE;

    GV_NODE_ADD_FIELD(color);
    GV_NODE_ADD_FIELD(fogType);
    GV_NODE_ADD_FIELD(visibilityRange);
    GV_NODE_ADD_FIELD(visibilityStart);
    GV_NODE_ADD_FIELD(density);

    GV_NODE_ADD_EVENT_IN(set_bind,GvSFBool);
    GV_NODE_ADD_EVENT_OUT(isBound);

    fogType.value = "LINEAR";
    color.value[0] = color.value[1] = color.value[2] = 1.0; 

}


GvFog2::~GvFog2()
{
}
